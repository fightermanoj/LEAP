import glob
import serial

ports = glob.glob("/tmp/iot-serial/tty-*")

if not ports:
    print("No virtual serial ports found")
else:
    for p in ports:
        print(f"Found port: {p}")

    port = ports[0]
    ser = serial.Serial(port, 115200, timeout=1)
    print(f"Connected to {port}")
while True:
    line = ser.readline().decode("utf-8", errors="ignore").strip()
    if line:
        print(line)

