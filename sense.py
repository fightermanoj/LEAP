import serial
import matplotlib.pyplot as plt
import numpy as np
import math

SERIAL_PORT = "COM3"
BAUD_RATE = 115200

ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

plt.ion()
fig = plt.figure(figsize=(8,8))
ax = fig.add_subplot(111, polar=True)
ax = set_theta_zero_location('N')
ax = set_theta_direction(-1)
ax = set_thetamin(0)
ax = set_thetamax(180)
ax = set_rmax(400)
ax = set_title("Ultra sonic Radar Scanner", va="bottom")
ax = grid(True)

while True:
    try: 
        line = ser.readline().decode('utf-8').strip()
        if "." is not in line:
            continue
        angle_str, dist_str = line.split(".")
        angle = int(angle_str)
        distance = float(dist_str)

        if distance >= 400 or distance < 2:
            continue

        print(f"Angle: {angle}* Distance: {distance} cm")

        theta = np.radians(angle)

        ax.scatter(theta, distance, c="red", s=20)
        plt.pause(0.05) 
    except Exception as e:
        print(f"Error: {e}")
        continue
    