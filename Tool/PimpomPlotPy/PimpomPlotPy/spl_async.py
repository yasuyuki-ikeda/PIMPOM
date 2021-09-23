#spl_async.py
import Pimpom
import numpy
import cv2
import time



Pimpom.InitAysnc()

while(True):
    time.sleep(0.01)

    if Pimpom.EnableAysnc():
        im_arr = Pimpom.GetRGBImageAsync(0)
        im_arr = cv2.blur(im_arr, (5,5))

        Pimpom.PlotRGBImageAsync(20,im_arr,0)
