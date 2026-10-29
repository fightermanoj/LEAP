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
    if "," not in line and "." not in line:
        return None
    sep = "," if "," in line else "."
    parts = line.split(sep)

    if len(parts) >= 12:
        try:
            dists = [float(parts[i]) for i in range(5)]
            servos = [int(float(parts[5 + i])) for i in range(5)]
            pitch = float(parts[10])
            yaw = float(parts[11])
            return (dists, servos, {"pitch": round(pitch, 1), "yaw": round(yaw, 1)})
        except:
            return None

    if len(parts) >= 10:
        try:
            dists = [float(parts[i]) for i in range(5)]
            servos = [int(float(parts[5 + i])) for i in range(5)]
            return (dists, servos, {"pitch": 0.0, "yaw": 0.0})
        except:
            return None

    if len(parts) >= 6:
        try:
            dists = [float(parts[i]) for i in range(5)]
            servo = int(float(parts[5]))
            return (dists, [servo] * 5, {"pitch": 0.0, "yaw": 0.0})
        except:
            return None

    if len(parts) >= 2:
        try:
            return ([float(parts[0])] * 5, [int(float(parts[1]))] * 5, {"pitch": 0.0, "yaw": 0.0})
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
