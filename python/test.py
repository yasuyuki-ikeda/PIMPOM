import numpy as np
import Pimpom


def make_image_data(dtype,channels,width,height):
    
    if channels==1:
        dummy = np.zeros(height*width, dtype=dtype).reshape(height,width)
        for j in range(0, height):
            for i in range(0, width):
                if dtype == "uint8":
                    dummy[j,i] = (i+j) % 256
                else:
                    dummy[j,i] = (i+j)
    else:
        dummy = np.zeros(height*width*channels, dtype=dtype).reshape(channels,height,width)
        for c in range(0, channels):
            for j in range(0, height):
                for i in range(0, width):
                    if dtype == "uint8":
                        dummy[c,j,i] = (i+j+c*4) % 256
                    else:
                        dummy[c,j,i] = (i+j+c*4)

    return dummy



Pimpom.ShowMessage("Hello, PIMPOM")

width=640
height=480
Pimpom.PlotByteImage(0, make_image_data("uint8",1,width,height) )
Pimpom.PlotShortImage(1, make_image_data("int16",1,width,height) )
Pimpom.PlotWordImage(2, make_image_data("uint16",1,width,height) )
Pimpom.PlotLongImage(3, make_image_data("int32",1,width,height) )
Pimpom.PlotDwordImage(4, make_image_data("uint32",1,width,height) )
Pimpom.PlotFloatImage(5, make_image_data("float32",1,width,height) )
Pimpom.PlotRGBImage(6, make_image_data("uint8",3,width,height) )
Pimpom.Plot3DImage(7, make_image_data("uint8",10,width,height) )
Pimpom.PlotF3DImage(8, make_image_data("float32",10,width,height) )
