"""
ON THE RASPI: roslaunch raspicam_node camerav2_320x240.launch enable_raw:=true

   0------------------> x (cols) Image Frame
   |
   |        c    Camera frame
   |         o---> x
   |         |
   |         V y
   |
   V y (rows)

"""
#!/usr/bin/python
import numpy as np
import RPi.GPIO as GPIO
import sys, getopt, math, os, cv2
import threading
import serial
import time

SCREEN_WIDTH = 320
SCREEN_HEIGHT = 240
data_AT=0

piSerial = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=1.0)
gData = ""
dist = 25

def main():

    global Mode_State
    global x_pos
    global y_pos
    global dist
    countnum=0
    capture = cv2.VideoCapture(-1)   
      
    while(True):

        ret, frame = capture.read()
        frame = cv2.resize(frame, (0,0), fx=0.5, fy=0.5)
        
        #Green
        #HSV_MIN_G = np.array((40, 160, 160))
        #HSV_MAX_G = np.array((70, 255, 255))
        
        HSV_MIN_G = np.array((30, 100, 100))
        HSV_MAX_G = np.array((50, 255, 255))

                 
        hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        
        #GREEN
        mask_G = cv2.inRange(hsv_frame, HSV_MIN_G, HSV_MAX_G)
        
        mask_contoursG, hierarchyG = cv2.findContours(mask_G, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        mask_contours=mask_contoursG
        countnum=0

    # Finding position of all contours
        if len(mask_contours) != 0 :
            for mask_contour in mask_contours:
                if cv2.contourArea(mask_contour) > 400:
                    countnum=countnum+1
                    x, y, w, h = cv2.boundingRect(mask_contour)
                    cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2) #drawing rectangle
                    
                    text1 = 'Tennis ball:{}'.format(countnum)
                    cv2.putText(frame,text1,(x, y-3),cv2.FONT_HERSHEY_SIMPLEX,(0.5),(0, 255, 255))
                    
                    if countnum ==1 :
                        G_x=x
                        G_y=y
                        G_w=w
                        G_h=h
                    
                    m=G_w//2
                    k=G_h//2
                    
                    x_pos=160-(G_x+m)
                    y_pos=G_y+k
                    #  320 240
                    cv2.line(frame, (160,120), (G_x+m, G_y+k), (0,255,255),2,cv2.LINE_AA)
                    cv2.line(frame, (0,150), (320, 150), (255,255,0),2,cv2.LINE_AA)
                    cv2.line(frame, (0,180), (320, 180), (255,255,0),2,cv2.LINE_AA)
                    
                    #print('X={} Y={} width={} height={} m={} C={}'.format( x, y, w, h, m, x_pos))
                
                    font=cv2.FONT_HERSHEY_SIMPLEX
        
                    text1 = 'Sensor:{}'.format(x_pos)
                    text2 = 'Left'
                    text3 = 'Right'
                    text4 = 'Center'
                    cendR="r"
                    cendL="l"
                    cendC="c"
                    cendG="g"
                    cendK="k"
                    #x_pos=160-(x+m)
                    #320.280/0.5/160.140
                    frame = cv2.putText(frame, text1,(5, 230),font, 0.5, (0, 255, 255), 1)
                    print(y_pos)
                    if 150< y_pos< 180:
                        
                        piSerial.write(cendK.encode())
                        
                    if y_pos>180:
                        
                        piSerial.write(cendG.encode())    
        
                    if x_pos>dist:
                        
                        piSerial.write(cendL.encode())
                        frame = cv2.putText(frame, text2,(160, 230),font, 0.5, (0, 255, 255), 1)
                        
                    elif x_pos<-dist:
                        
                        piSerial.write(cendR.encode())
                        frame = cv2.putText(frame, text3,(160, 230),font, 0.5, (0, 255, 255), 1)
                        
                    else:
                        
                        piSerial.write(cendC.encode())
                        frame = cv2.putText(frame, text4,(160, 230),font, 0.5, (0, 255, 255), 1)
        else:
            cendN="n"
            piSerial.write(cendN.encode())
        
        cv2.imshow("original image",frame)
        cv2.imshow("mask_G image",mask_G)
        

        if cv2.waitKey(1) == ord('q'):
            break
        
    cv2.destroyAllWindows()
    GPIO.cleanup()


if __name__ == "__main__":
    main()
    #piSerial.close()





