import cv2
import numpy as np
 

#Simulation with 4 doors and 1 ped

doors = np.array([[20,20,40,80],
                  [312,20,332,80],
                  [20,208,40,268],
                  [312,208,332,268]
                  ]) #Scanned in order [top left to bottom right] from nature of HOG/Cascade detection
#Doors always stay the same



#Ped size is 20x40

#Calculate ped base on centres: for c = centres, ped = [x-10,y-20,x+10,y+20]
centres=np.array([
    [176,144], #Centre 
    [176,50],
    [100,50],
    [70,50], #slight overlap 
    [30,50], #Maximum Overlap
    [],
    [],
    [],
    [],
    [30,50],
    [30,50],
    [70,50],
    [100,50],
    [176,50],
    [176,144]
    #Code as exit frame
    ])
#This is pedestrians going in; reverse for going out 

ped = np.empty(shape=(len(centres),),dtype=np.object)

ct =0


for object in centres:
    if len(centres[ct])==0:
        ped[ct]=[]
    
    else:
        (x,y)=centres[ct]
        ped[ct]=[x-10,y-20,x+10,y+20]
        
    
    ct+=1

    
    
ped=np.append(ped,np.empty((0,0)))#Please fix 