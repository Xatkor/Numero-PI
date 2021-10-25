import numpy as np
import math
import random
import matplotlib.pyplot as plt
import pygame

pygame.init()
pygame.font.init()


WIDTH, HEIGTH = 600, 600
WIN =  pygame.display.set_mode((WIDTH, HEIGTH))
pygame.display.set_caption('PI usando Montecarlo')

FPS = 60

YELLOW = (243, 255 ,0)
PINK = (255, 0, 212)
BLACK = (0, 0, 0)
BLUE=(0,0,255)
WHITE = (255, 255, 255)

def NumberPI():
    randomX = random.uniform(WIDTH - 500, WIDTH - 100)
    randomY = random.uniform(WIDTH - 500, WIDTH - 100)
    
    return randomX , randomY

def CountPI(x, y, circle, total):
        
    if (x - 300) ** 2 + (y - 300) ** 2 < 200 ** 2:
        circle += 1
        total += 1
    else :
        total += 1
        
    return (4 * circle) / total

def main():
    WIN.fill(BLACK)
    circle = 0
    total = 0
    clock = pygame.time.Clock()
    run = True
    while run:
        clock.tick(FPS)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False

        posicionX, posicionY = NumberPI()

        myfont = pygame.font.SysFont('comicsans', 10)
        myfont2 = pygame.font.SysFont('Arial', 40)

        if (posicionX - 300) ** 2 + (posicionY - 300) ** 2 < 200 ** 2:
            label = myfont.render('o', 1,   YELLOW)
            circle += 1
            total += 1
        else:
            label = myfont.render('o', 1,   PINK)
            total += 1
            

        pygame.draw.rect(WIN, BLACK, (100,60,400,40))
        pygame.draw.rect(WIN, BLACK, (100,510,400,40))
        solucion = CountPI(posicionX, posicionY, circle, total)
        Pi = myfont2.render('{}'.format(solucion), 10, WHITE)
        Total = myfont2.render('Lanzamientos: {}'.format(total), 10, WHITE)
        WIN.blit(Pi, (110, 60))
        WIN.blit(Total, (110, 510))
        WIN.blit(label, (posicionX, posicionY))
        pygame.display.update()
        #pygame.time.delay(50)

    pygame.quit()

if __name__ == '__main__':
    main()