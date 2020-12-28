#PimpomPyTest.py
import Pimpom
import numpy
import cv2



im = cv2.imread("C:/tmp/test.bmp")
im_arr = numpy.asarray(im)
Pimpom.PlotRGBImage(0,im_arr,0)






mask=Pimpom.GetMaskImage(0)
imsize = mask.shape

for j in range(imsize[0]):
   for i in range(imsize[1]):
    if (i-imsize[1]/2)*(i-imsize[1]/2) + (j-imsize[0]/2)*(j-imsize[0]/2) < 100*100:
        mask[j,i] = 0
    else:
        mask[j,i] = 1

Pimpom.SetMaskImage(0,mask)

Pimpom.SetWorkArea(0,0,100,100,200,200,1)

area=Pimpom.GetWorkArea(0,0)
print(area)