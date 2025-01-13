'''
                   License Agreement
    Ploaform for IMage Procession Obserber and Modifier

Copyright(c) 2020, Yasuyuki Ikeda All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met :

*Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and / or other materials provided with the distribution.
* The name of the copyright holders may not be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''

#######################################################
# PythonプログラムからPIMPOMにデータを出力するための関数群
#######################################################

import numpy as np
import struct
import win32gui
import win32con
import win32api
import struct
from multiprocessing import shared_memory



#######################################################
#タイトルバーに特定の文字列を含むウィンドウを検索
#######################################################
def find_window_by_title(title_substring):
    hwnd_list = []
    def enum_window_callback(hwnd, _):
        if win32gui.IsWindowVisible(hwnd):
            title = win32gui.GetWindowText(hwnd)
            if title_substring in title:
                hwnd_list.append(hwnd)

    win32gui.EnumWindows(enum_window_callback, None)
    return hwnd_list


#######################################################
# ウィンドウにメッセージを送信
#######################################################
def pimpom_plot_send_data(meg):
    hwnds = find_window_by_title("- PIMPOM")
    if len(hwnds) > 0:
        win32api.SendMessage(hwnds[0], win32con.WM_USER + meg,0, 0)


#######################################################
#  共有メモリを作成し、データを書き込む
#######################################################
def create_shared_memory(data):
    # シリアライズ
    packed_data = struct.pack(
        '7i3Bi5000s', # 10個のint, 3個のBYTE, 5000バイトの文字列
        data['type'],  data['image_num'], 
        data['x0'],  data['y0'],  data['x1'], data['y1'], data['size'], 
        data['red'],  data['green'],  data['blue'], 
        data['copy_src'],
        data['message'].encode('utf-8')
    )

    # 共有メモリを作成
    shm = shared_memory.SharedMemory(name="PIMPOM_SHAREDMEM", create=True, size=len(packed_data))
    shm.buf[:len(packed_data)] = packed_data
    return shm
    
#######################################################
#  画像用の共有メモリを作成し、データを書き込む
#######################################################
def create_shared_memory_image(data):
    # 構造体全体をパック
    packed_header = struct.pack(
        '7i',  # format, image_num, width, height, page, channel, size
        data['format'], data['image_num'], data['width'], data['height'],
        data['page'], data['channel'], data['size']
    )

    # 構造体のpData配列サイズ
    p_data_size = data['width'] * data['height'] * data['page'] * data['channel']

    # 配列データ（pData[]）
    format = data['format']
    if format == 0:#BYTE型メモリ
        packed_p_data = struct.pack(f'{p_data_size}B', *data['pData'])
    elif format == 1:#SHORT型メモリ
        packed_p_data = struct.pack(f'{p_data_size}h', *data['pData'])
    elif format == 2:#WORD型メモリ
        packed_p_data = struct.pack(f'{p_data_size}H', *data['pData'])
    elif format == 3:#LONG型メモリ
        packed_p_data = struct.pack(f'{p_data_size}i', *data['pData'])
    elif format == 4:#DWORD型メモリ
        packed_p_data = struct.pack(f'{p_data_size}I', *data['pData'])
    elif format == 5:#float型メモリ
        packed_p_data = struct.pack(f'{p_data_size}f', *data['pData'])  
    elif format == 10:#3D型メモリ
        packed_p_data = struct.pack(f'{p_data_size}B', *data['pData'])
    elif format == 11:#F3D型メモリ
        packed_p_data = struct.pack(f'{p_data_size}f', *data['pData']) 
    elif format == 20:#RGB型メモリ
        packed_p_data = struct.pack(f'{p_data_size}B', *data['pData'])
    elif format == 21:#マスクメモリ
        packed_p_data = struct.pack(f'{p_data_size}B', *data['pData'])  
    else:
        raise ValueError("format error!")

    # 全体のデータ
    packed_data = packed_header + packed_p_data

    # 共有メモリを作成
    shm = shared_memory.SharedMemory(name="PIMPOM_SHAREDMEM", create=True, size=len(packed_data))
    shm.buf[:len(packed_data)] = packed_data
    print("共有メモリ 'PIMPOM_SHAREDMEM' にデータを書き込みました")
    return shm


#######################################################
# PIMPOMにテキストデータを出力する
#######################################################
def pimpom_draw_text(type, num, x, y, message, r, g, b, copy_src):
    # 構造体データ
    data = {
        'type': type,'image_num': num,
        'x0': x,'y0': y, 'x1': 0,'y1': 0,'size': 0,
        'red': r,'green': g,'blue': b,
        'copy_src': copy_src,
        'message': message
    }
    shm = create_shared_memory(data)
    pimpom_plot_send_data(101)
    shm.close()
    shm.unlink()

    
#######################################################
# PIMPOMに画像データを出力する
# format :0:BYTE 1:short 2:WORD 3:long 4:DWORD 5:FLOAT 10:3D 11:FLOAT3D: 20 RGB: 21 MASK
#######################################################
def pimpom_plot_image(format, num, pData):

    if format == 0:#BYTE型メモリ
        ndim = 2
        dtype = "uint8"
        pix_size= 1

    elif format == 1:#SHORT型メモリ
        ndim = 2
        dtype = "int16"
        pix_size= 2
    elif format == 2:#WORD型メモリ
        ndim = 2
        dtype = "uint16"
        pix_size= 2
    elif format == 3:#LONG型メモリ
        ndim = 2
        dtype = "int32"
        pix_size= 4
    elif format == 4:#DWORD型メモリ
        ndim = 2
        dtype = "uint32"
        pix_size= 4
    elif format == 5:#float型メモリ
        ndim = 2
        dtype = "float32"
        pix_size= 4  
    elif format == 10:#3D型メモリ
        ndim = 3
        dtype = "uint8"
        pix_size= 1  
    elif format == 11:#F3D型メモリ
        ndim = 3
        dtype = "float32"
        pix_size= 4  
    elif format == 20:#RGB型メモリ
        ndim = 3
        dtype = "uint8"
        pix_size= 1 
    elif format == 21:#マスクメモリ
        ndim = 2
        dtype = "uint8"
        pix_size= 1 
    else:
        raise ValueError("format error!")

    if pData.dtype != dtype:
        raise ValueError("second argument must be " + dtype +" array!")

    if pData.ndim != ndim:
        raise ValueError("second argument must be " + str(ndim) +"-dimensional!")

    
    page = 1
    channel = 1
    
    if format == 10:#3D型メモリ
        page = pData.shape[0]
        height = pData.shape[1]
        width = pData.shape[2]
    elif format == 11:#F3D型メモリ
        page = pData.shape[0]
        height = pData.shape[1]
        width = pData.shape[2]
    elif format == 20:#RGB型メモリ
        channel = pData.shape[0]
        height = pData.shape[1]
        width = pData.shape[2]
        if channel!=3:
            raise ValueError("RGB image must have 3 channels!")
    else:
        height = pData.shape[0]
        width = pData.shape[1]

    # 構造体データ
    data = {
        'format': format,
        'image_num': num,
        'width': width,
        'height': height,
        'page': page,
        'channel': channel,
        'size': width * height * page * channel *pix_size + 7*4,  
        'pData': pData.flatten()  
    }

    shm = create_shared_memory_image(data)
    pimpom_plot_send_data(100)
    shm.close()
    shm.unlink()



#######################################################
# PIMPOMにメッセージを出す
#######################################################
def ShowMessage(message):
    pimpom_draw_text(11, 0, 0, 0, message, 0, 0, 0, -1)


#######################################################
# PIMPOMにBYTE(uint8)画像データを出力する
#######################################################
def PlotByteImage(num, pData):
    pimpom_plot_image(0, num, pData)

#######################################################
# PIMPOMにshort(int16)画像データを出力する
#######################################################
def PlotShortImage(num, pData):
    pimpom_plot_image(1, num, pData)

#######################################################
# PIMPOMにword(uint16)画像データを出力する
#######################################################
def PlotWordImage(num, pData):
    pimpom_plot_image(2, num, pData)

#######################################################
# PIMPOMにlong(int32)画像データを出力する
#######################################################
def PlotLongImage(num, pData):
    pimpom_plot_image(3, num, pData)

#######################################################
# PIMPOMにdword(uint32)画像データを出力する
#######################################################
def PlotDwordImage(num, pData):
    pimpom_plot_image(4, num, pData)

#######################################################
# PIMPOMにfloat32画像データを出力する
#######################################################
def PlotFloatImage(num, pData):
    pimpom_plot_image(5, num, pData)

#######################################################
# PIMPOMにマルチチャネルuint8画像データ（CHW)を出力する
#######################################################
def Plot3DImage(num, pData):
    pimpom_plot_image(10, num, pData)

#######################################################
# PIMPOMにマルチチャネルfloat32画像データ（CHW)を出力する
#######################################################
def PlotF3DImage(num, pData):
    pimpom_plot_image(11, num, pData)

#######################################################
# PIMPOMにマルチチャネルuint8 RGB画像データ（CHW)を出力する
#######################################################
def PlotRGBImage(num, pData):
    pimpom_plot_image(20, num, pData)



