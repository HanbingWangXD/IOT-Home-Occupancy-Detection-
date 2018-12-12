from imutils.object_detection import non_max_suppression
import numpy as np
import imutils
import cv2

#Detection params initialization 
hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())
door_cascade = cv2.CascadeClassifier('haarcascades/door_cascade1.xml')
    
def door_det(img):
    
    
    doors=door_cascade.detectMultiScale(img, 1.3, 5)
    
    pick = non_max_suppression(doors, probs=None,overlapThresh=0.15)#Apply NMS
            
    return pick
        
def ped_det(image):
    
    
    (rects, weights) = hog.detectMultiScale(image, winStride=(4,4),padding=(8,8),scale=1.035)#Tune Scale for accuracy (decrease for more accuracy) and winstride for HOG sliding window step size 
   
    rects = np.array([[x,y,x+w,y+h] for (x,y,w,h) in rects]) #Rearrange to for NMS; define locations of the rect
    pick = non_max_suppression(rects, probs=None,overlapThresh=0.65)#Apply NMS

    
    return pick

def main_doors():
    
    img = cv2.imread('/home/pi/Desktop/OpenCV/door.jpg',1)
    
    gray =cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
    
    doors = door_dect(gray)
    
    for(x,y,w,h) in doors:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)
        roi = img[y:y+h,x:x+w]
    
    cv2.imshow('Gray',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
    return 








    
    
    
    
    
    
