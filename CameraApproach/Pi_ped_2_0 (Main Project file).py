from imutils.object_detection import non_max_suppression
from imutils import paths
import numpy as np
import imutils
import cv2
import time

import detectors
from Overlap import overlap #Check implementation for this fcn
from door_ped_logic import logic 

from picamera import PiCamera
from picamera.array import PiRGBArray

import array #XBee
from digi.xbee.devices import XBeeDevice,XBee64BitAddress,RemoteXBeeDevice


import demo_files
#Note to self; after testing please put all the code into a main() fcn

def dat_rec_callback(message):
    global message_received
    global xbData
    
    print("\nMessage recieved from:",message.remote_device.get_64bit_addr(),"\n")
    print("Data:\n",list(message.data)) #Byte arrary to list
    print(message.data.decode(),"\n") #Byte array to string
    message_received=True #Global interupt flag 
    xbData=message.data #Acts like register 
    



time.sleep(0.5) #Ensure Camera boot

resolution = (352,288) #Far too slow

cam = PiCamera()
cam.resolution = resolution #Set resolution 
rawcapture = PiRGBArray(cam,size=resolution)


#Load HOG detection params
hog = cv2.HOGDescriptor()
hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())

#Detection Vars
doors=[]#Just in case doors is empty
door_timer_init = (cv2.getTickCount())/(cv2.getTickFrequency())
has_door_been_init=False
zero_f_exists = False
debug = False
debug_ct=0

#Xbee vars
message_received=False

try:
    device = XBeeDevice('/dev/serial/by-id/usb-FTDI_FT231X_USB_UART_DN03FGOJ-if00-port0', 9600)
    remote_device=RemoteXBeeDevice(device,XBee64BitAddress.from_hex_string("0013A200417669CD"))
    device.open()
    time.sleep(0.2)
    device.add_data_received_callback(dat_rec_callback)
    
except:
    print("\nXBee initialization error\n")
    #device.close()
    

for frame in cam.capture_continuous(rawcapture, format = "bgr", use_video_port=True):
    
    
    
    
    tt= cv2.getTickCount() #record time it takes to run main loop
    current_frame = frame.array
    current_frame=cv2.flip(current_frame,0) #If Camera upside-down
    image_bgr = current_frame
    image = cv2.cvtColor(image_bgr,cv2.COLOR_BGR2GRAY)#convert to gray for processing
    
    if has_door_been_init == False: #Ran only at the beginning
        if debug==False:
            doors=detectors.door_det(image)
            
        else:
            doors=demo_files.doors
        
        has_door_been_init = True
        door_occup=np.zeros(len(doors),dtype=np.int16)
        print(doors,"\n",door_occup)
            
    
        
       
    #HOG sliding window is 64x128
    
    if debug == False:
        ped = detectors.ped_det(image) #Ped np array
        
    else:
        if len(demo_files.ped) >= debug_ct+1:
            if len(demo_files.ped[debug_ct]) ==0:
                ped = demo_files.ped[debug_ct]
            else:
                ped = np.array([demo_files.ped[debug_ct]])
            
            debug_ct+=1
            print("\nPed:",ped,"\n")
        else:
            input("Reached end of demo file")
            break
            
        

    
     
    
    if zero_f_exists == False: #First frame
        ovlp_o=[False]*len(doors)#create empty array to fill in/vector of 1xn 
        ct=0
        for object in doors:
            ovlp_o[ct] = overlap(object, ped, 0.3)
            ct+=1
        ped_len_o = len(ped)
        zero_f_exists = True
        print("Init conditions scanned")
        
    
    else:
        ct_d=0
        ped_len_n = len(ped)
        ovlp_n=[False]*len(doors)
        for object in doors: #Object is door num
        
            ovlp_n_v = overlap(object, ped, 0.3)
            ovlp_n[ct_d]=ovlp_n_v
         
            
            delta_door_occ= logic(ped_len_o,ped_len_n,ovlp_o[ct_d],ovlp_n_v) #There is some error here
            door_occup[ct_d] += np.int16(delta_door_occ) 
            
            if door_occup[ct_d] < 0: #Check if less than 0
                print("\nWarning: Door occupancy less than zero; reset back to zero\n")
                door_occup[ct_d] = np.int16(0);
        
            
            ct_d+=1
        
        print("People in rooms:",door_occup)
        ped_len_o=ped_len_n #Redefine Vars
        ovlp_o = ovlp_n
    
    
    for (xA,yA,xB,yB) in ped:
        cv2.rectangle(image_bgr,(xA,yA),(xB,yB),(0,255,0),2)#Write to image_bgr
        
    for (xA,yA,xB,yB) in doors:
        cv2.rectangle(image_bgr,(xA,yA),(xB,yB),(255,0,0),2)
    
    if message_received==True: #add message weightings later
        print("\nSending Data...") 
        print("Message acknowlegded")
        #Data processing 
        sending_message=bytes(door_occup)
        device.send_data(remote_device,sending_message)
        print("Sent\n")
        message_received=False
        
    
    

    tt = cv2.getTickCount() - tt
    frametime = tt/(cv2.getTickFrequency())
    print('Detected',len(ped),'person(s) and',len(doors),'door(s) at',"%.2f"% (frametime),'seconds at', "%.2f"% (1/frametime), 'fps')
    cv2.imshow('Ped Detect',image_bgr)
    
    rawcapture.truncate(0) #Clear Current Frame fro next frame
    
    if debug==True:
        debug_ct+=1
        #time.sleep(0.5)
        exit_t=input()
        if exit_t == 'q':
            break
        
    
    if cv2.waitKey(25) & 0xFF == ord('q'): #Stop 
        device.close()
        input("\nStopping Code...\n")
        break
    elif cv2.waitKey(25) & 0xFF == ord('p'): #Pause 
        input("\nPaused...\n")

cv2.destroyAllWindows()