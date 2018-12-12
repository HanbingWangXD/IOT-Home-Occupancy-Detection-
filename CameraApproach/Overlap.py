import numpy as np
import imutils

import cv2


def overlap_ref (ref,rect,overlap_threshold):
    #Calculates overlap given:
    #1xn array of [ref]
    #mxn array of ped
    #and overlap threshold
    if len(ref)==0|len(rect)==0:#Checks if either one of them are empty 
        overlaps_greater_than_thres=[]
    else:
        (xa1,ya1,xa2,ya2) = ref
        overlaps_greater_than_thres=[False]*len(rect)
        array_pos = 0
        for (xb1,yb1,xb2,yb2) in rect:
            s_ref = (ya2-ya1)*(xa2-xa1)
            s_rect = (yb2-yb1)*(xb2-xb1)
        
            s_intersect = max(0,min(xb2,xa2)-max(xa1,xb1))*max(0,min(ya2,yb2)-max(ya1,yb1))
            s = s_ref + s_rect - s_intersect
            overlap = s_intersect/s
        
            if overlap > overlap_threshold:
                overlaps_greater_than_thres[array_pos] = True
        
            array_pos+=1
            
def overlap (ref,rect,overlap_threshold):
    overlaps_greater_than_thres=False
    #Calculates overlap given:
    #1xn array of [ref]
    #mxn array of ped
    #and overlap threshold 
        
    if ~(len(ref)==0|len(rect)==0): #Ran if either one is not zero element vector
        (xa1,ya1,xa2,ya2) = ref
        for (xb1,yb1,xb2,yb2) in rect:
            s_ref = (ya2-ya1)*(xa2-xa1)
            s_rect = (yb2-yb1)*(xb2-xb1)
        
            s_intersect = max(0,min(xb2,xa2)-max(xa1,xb1))*max(0,min(ya2,yb2)-max(ya1,yb1))
            s = s_ref + s_rect - s_intersect
            overlap = s_intersect/s
        
            if overlap > overlap_threshold:
                overlaps_greater_than_thres=True
                print("\nRect is overlapping ref\n")
                break 
        
    
    
    return overlaps_greater_than_thres

