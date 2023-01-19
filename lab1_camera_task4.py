# Links: 
    
# https://answers.opencv.org/question/200861/drawing-a-rectangle-around-a-color-as-shown/

import cv2 as cv
import numpy as np

cap = cv.VideoCapture(0)

while(1): # Constantly execute

    _, frame = cap.read() # Read a frame

    hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV) # Convert from BGR to HSV

    lower_green = np.array([80,30,100]) # Lower range R,G,B of detection
    upper_green = np.array([150,100,70]) # Upper range R,G,B of detection
    
    mask = cv.inRange(hsv, lower_green, upper_green) # Apply threshold for the HSV


    contours, _ =  cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        
    if len(contours)>0: 
        green_area = max(contours, key=cv.contourArea)
        if cv.contourArea(green_area) > 1000: 
            (xg,yg,wg,hg) = cv.boundingRect(green_area) # Add bounding rectangle
            cv.rectangle(frame,(xg,yg),(xg+wg, yg+hg),(0,255,0),2)

    cv.imshow('frame', frame) # Display / show the camera screen
    
    k = cv.waitKey(5) & 0xFF
    if k == 27:
        break

cv.destroyAllWindows()