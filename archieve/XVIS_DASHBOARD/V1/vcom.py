import glob
import serial

def find_port(pattern="/tmp/iot-serial/tty-*"):
    ports = glob.glob(pattern)
    if not ports:
        return "/dev/ttyUSB0"
    return ports[0]

def open_serial(port=None, baud=115200, timeout=0.01):
    if not port:
        port = find_port()
    return serial.Serial(port, baud, timeout=timeout)

def send_cmd(ser, cmd):
    if ser:
        ser.write(cmd.encode())

def parse_line(raw):
    line = raw.decode(errors="ignore").strip()
    if "Distance:" in line and "Servo:" in line:
        try:
            d_part = line.split("Distance:")[1].split("cm")[0].strip()
            s_part = line.split("Servo:")[1].strip()
            dist = float(d_part)
            servo = int(float(s_part))
            if 1 < dist < 400:
                return (dist, servo)
        except:
            return None
    if "," in line or "." in line:
        sep = "," if "," in line else "."
        parts = line.split(sep)
        if len(parts) >= 2:
            try:
                dist = float(parts[0])
                servo = int(float(parts[1]))
                if 1 < dist < 400:
                    return (dist, servo)
            except:
                return None
    return None

def read_latest(ser):
    if not ser or ser.in_waiting == 0:
        return None
    last = None
    while ser.in_waiting > 0:
        raw = ser.readline()
        pt = parse_line(raw)
        if pt:
            last = pt
    return last
