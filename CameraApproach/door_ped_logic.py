import numpy as np
import imutils
from Overlap import overlap
import cv2

def logic(ped_o,ped_n,over_o,over_n): #N,N
    
    
    d_occup=0
    

    if (not over_o) & over_n: #(N,Y)
        print("1:something is overlapping;","\nover_o:",over_o,"over_n:",over_n,"\nped_n:",ped_n,"ped_o:",ped_o)
        if ped_n > ped_o:
            #Someone has entered the hallway through door (IMPORTANT)
            d_occup = -1 #Someone has left a room
            print("\n\nSomeone has left a room\n\n")

    elif over_o & (not over_n): #(Y,N)
        print("2:something is overlapping;","\nover_o:",over_o,"over_n:",over_n,"\nped_n:",ped_n,"ped_o:",ped_o)

        if ped_o > ped_n:
            #Someone has left the hallway through a door (IMPORTANT)
            d_occup = 1 #someone has  left a room
            print("\n\nSomeone has entered the room\n\n")

    return d_occup
            
        