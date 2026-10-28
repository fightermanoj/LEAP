import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import math 

ser = serial.Serial('COM3', 9600, timeout=1)

class SonarSLAM: 
    def __init__(self):
        self.points_x = []
        self.points_y = []
        self.robot_x = 0
        self.robot_y = 0
        self.robot_theta = 0

        self.fig, self.ax = plt.subplots(figsize=(10, 10))
        self.scatter = self.ax.scatter([], [], s=5, c='blue')
        self.robot_marker = self.ax.plot([], [], 'ro', markersize=10)[0]
        self.ax.set_xlim(-200, 200)
        self.ax.set_ylim(-200, 200)
        self.ax.set_xlabel('X (cm)')
        self.ax.set_ylabel('Y (cm)')
        self.ax.set_title("SLAM point cloud Map")
        self.ax.grid(True)

    def read_sonar_data(self):
        try:
            line = ser.readline().decode('utf-8').strip()

            if line: 
                angle, distance = map(float, line.split(','))
                return angle, distance
        except Exception as e: 
            print(f"Exception {e}")
        return None, None


    def polar_to_cartesian(self, angle, distance):
        total_angle = math.radians(angle) + self.robot_theta 

        x = self.robot_x + distance * math.cos(total_angle)
        y = self.robot_y + distance * math.sin(total_angle)

        return x, y

    def update_robot_pose(self, dx, dy, dtheta):
        self.robot_x += dx 
        self.robot_y += dy 
        self.robot_theta += dtheta 

    def update_plot(self, frame):
        angle, distance = self.read_sonar_data()

        if angle is not None and distance is not None and distance < 400:
            x, y = self.polar_to_cartesian(angle, distance)
            self.points_x.append(x)
            self.points_y.append(y)

            self.scatter.set_offsets(np.c_[self.points_x, self.points_y])
            self.robot_marker.set_data([self.robot_x], [self.robot_y])
        return self.scatter, self.robot_marker

    def run(self):
        self.ani = FuncAnimation(self.fig, self.update_plot, interval=100, blit=True, cache_frame_data=False)
        plt.show()

if __name__ == "__main__":
    slam = SonarSLAM()
    slam.run()