import pygame
import random
import math

pygame.init()
win = pygame.display.set_mode((600, 600))
center = (300, 300)

running = True
while running:
    for e in pygame.event.get():
        if e.type == pygame.QUIT:
            running = False

    angle = random.uniform(0, 2 * math.pi)
    dist = random.randint(50, 250)

    x = int(center[0] + dist * math.cos(angle))
    y = int(center[1] + dist * math.sin(angle))

    pygame.draw.circle(win, (255, 255, 255), (x, y), 2)
    pygame.draw.circle(win, (255, 0, 0), center, 5)

    pygame.display.flip()
    pygame.time.delay(20)

pygame.quit()
