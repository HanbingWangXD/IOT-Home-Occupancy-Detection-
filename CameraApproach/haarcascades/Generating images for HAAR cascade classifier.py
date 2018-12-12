import urllib.request
import cv2
import numpy as np
import os

def store_raw_images ():
    neg_images_link = 'http://image-net.org/api/text/imagenet.synset.geturls?wnid=n04082710'
    neg_image_urls = urllib.request.urlopen(neg_images_link).read().decode()

    if not os.path.exists('neg'):
        os.makedirs('neg')

    pic_num = 3688

    for i in neg_image_urls.split('\n'):
        try:
            print(i)
            name= "neg/"+str(pic_num)+'.jpg'
            urllib.request.urlretrieve(i, name)
            image = cv2.imread(name)
            image =cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
            #cv2.imshow("Gray",image)
            resized_image = cv2.resize(image, (100,100))
            #cv2.imshow("Resized",image)
            cv2.imwrite(name, resized_image)
            pic_num = pic_num + 1
            print (pic_num)
            print (name)


        except Exception as e:
            print(str(e))

#store_raw_images()

def find_uglies():
    for file_type in ['neg']:
        for image in os.listdir(file_type):
            for ugly in os.listdir('uglies'):
                
                try:
                    current_image_path = str(file_type)+'/'+str(image)
                    ugly = cv2.imread('uglies/'+str(ugly))
                    question = cv2.imread(current_image_path)

                    if ugly.shape == question.shape and not(np.bitwise_xor(ugly,question).any()):
                        print('dayyyyyummmmm girl u ugly!')
                        print(current_image_path)
                        os.remove(current_image_path)
                    

                except Excpetion as e:
                    print(str(e))

#find_uglies()

def create_pos_n_neg():
    for file_type in ['neg']:

        for image in os.listdir(file_type):
            if file_type =='neg':
                line = file_type+'/'+image+'\n'
                with open('bg.txt','a') as f:
                    f.write(line)

            elif file_type == 'pos':
                line = file_type+'/'+image+' 1 0 0 50 75\n'
                with open('info.dat','a') as f:
                    f.write(line)

create_pos_n_neg()



































                    
