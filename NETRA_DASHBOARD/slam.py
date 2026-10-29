import math

GRID_SIZE = 100
CELL_SIZE_CM = 10.0
SENSOR_ANGLES = [180.0, 0.0, 115.0, 65.0, 270.0]

grid = [[0 for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)]
diver_pose = [500.0, 500.0, 0.0]
path_history = []
obstacle_points = set()

def world_to_grid(x, y):
    gx = int(x / CELL_SIZE_CM)
    gy = int(y / CELL_SIZE_CM)
    if 0 <= gx < GRID_SIZE and 0 <= gy < GRID_SIZE:
        return (gx, gy)
    return None

def bresenham(x0, y0, x1, y1):
    points = []
    dx = abs(x1 - x0)
    dy = abs(y1 - y0)
    sx = 1 if x0 < x1 else -1
    sy = 1 if y0 < y1 else -1
    err = dx - dy

    cx, cy = x0, y0
    while True:
        points.append((cx, cy))
        if cx == x1 and cy == y1:
            break
        e2 = 2 * err
        if e2 > -dy:
            err -= dy
            cx += sx
        if e2 < dx:
            err += dx
            cy += sy
    return points

def step_diver(yaw, speed_cm=0.0):
    rad = math.radians(yaw)
    diver_pose[0] += speed_cm * math.cos(rad)
    diver_pose[1] += speed_cm * math.sin(rad)
    diver_pose[2] = yaw

    cell = world_to_grid(diver_pose[0], diver_pose[1])
    if cell and (not path_history or path_history[-1] != cell):
        path_history.append(cell)

def update_slam(dists, yaw, speed_cm=0.0):
    step_diver(yaw, speed_cm)

    start_cell = world_to_grid(diver_pose[0], diver_pose[1])
    if not start_cell:
        return get_slam_state()

    for i in range(min(5, len(dists))):
        d = dists[i]
        if d <= 2.0 or d >= 60.0:
            continue

        global_angle = (yaw + SENSOR_ANGLES[i]) % 360.0
        rad = math.radians(global_angle)

        ox = diver_pose[0] + d * math.cos(rad)
        oy = diver_pose[1] + d * math.sin(rad)

        end_cell = world_to_grid(ox, oy)
        if not end_cell:
            continue

        ray = bresenham(start_cell[0], start_cell[1], end_cell[0], end_cell[1])
        for pt in ray[:-1]:
            grid[pt[1]][pt[0]] = -1

        grid[end_cell[1]][end_cell[0]] = 1
        obstacle_points.add(end_cell)

    return get_slam_state()

def get_slam_state():
    return {
        "diver": [round(diver_pose[0], 1), round(diver_pose[1], 1), round(diver_pose[2], 1)],
        "path": list(path_history)[-80:],
        "obstacles": list(obstacle_points)
    }

def reset_slam():
    global grid, diver_pose, path_history, obstacle_points
    grid = [[0 for _ in range(GRID_SIZE)] for _ in range(GRID_SIZE)]
    diver_pose = [500.0, 500.0, 0.0]
    path_history = []
    obstacle_points = set()

