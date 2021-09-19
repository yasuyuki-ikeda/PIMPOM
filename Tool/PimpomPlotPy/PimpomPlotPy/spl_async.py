#PimpomPyTest.py
import Pimpom
import numpy
import cv2
import time


#im = cv2.imread("C:/tmp/test.bmp")
#im_arr = numpy.asarray(im)
#Pimpom.PlotRGBImage(0,im_arr,0)






while(True):
    time.sleep(0.01)
    im_arr = Pimpom.GetRGBImageAsync(0)
    if im_arr.shape[0]==1:
        continue

    im_arr = cv2.blur(im_arr, (5,5))



    Pimpom.PlotRGBImageAsync(20,im_arr,0)
