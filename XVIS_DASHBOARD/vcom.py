import glob
import serial

def find_port(pattern="/tmp/iot-serial/tty-*"):
    ports = glob.glob(pattern)
    if not ports:
        return "/dev/ttyUSB0"
    return ports[0]

def open_serial(port=None, baud=115200, timeout=0.05):
    if not port:
        port = find_port()
    return serial.Serial(port, baud, timeout=timeout)

def send_cmd(ser, cmd):
    if ser:
        ser.write(cmd.encode())

def parse_line(raw):
    line = raw.decode(errors="ignore").strip()
    if "." not in line:
        return None
    parts = line.split(".")
    if len(parts) != 2:
        return None
    try:
        ang = int(parts[0])
        dist = float(parts[1])
        if 0 <= ang <= 180 and 2 < dist < 400:
            return (ang, dist)
    except:
        return None
    return None

def read_sample(ser):
    if not ser or ser.in_waiting == 0:
        return None
    raw = ser.readline()
    return parse_line(raw)

def read_batch(ser):
    samples = []
    if not ser:
        return samples
    while ser.in_waiting > 0:
        pt = read_sample(ser)
        if pt:
            samples.append(pt)
    return samples
