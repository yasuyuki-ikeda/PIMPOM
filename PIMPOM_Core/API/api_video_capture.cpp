/*******************************************************************
                           License Agreement
            Ploaform for IMage Procession Obserber and Modifier

Copyright (c) 2020, Yasuyuki Ikeda All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* The name of the copyright holders may not be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************/
//CPimpomAPI
//ビデオキャプチャ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"

#include "opencv_headers.h"
#include "PIMPOM_API.h"


// *************************************
//         マ  ク  ロ   定   義         
// *************************************

// *************************************
//         外    部    宣    言         
// *************************************

// *************************************
//         構  造  体   定   義         
// *************************************

// *************************************
//         静    的    変    数         
// *************************************
static  cv::VideoCapture  VCap;//ビデオキャプチャ(opencv3)

// *************************************
//         外    部    変    数         
// *************************************

// *************************************
//         関    数    参    照         
// *************************************

/********************************************************************
機  能  名  称 : キャプチャドライバを開く
関    数    名 : CaptureInit
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::CaptureInit(CSize size)
{
	double w, h;
	CConfirmDlg	box;

	w = size.cx;
	h = size.cy;

	if (!VCap.isOpened())
	{
		if (w <= 0 || h <= 0) {
			w = 640;
			h = 480;
			box.RegistVar("画像幅", &w);
			box.RegistVar("画像高さ", &h);
			box.RegistVar("画像メモリ番号", &capture_image_number);
			box.SetTitle("キャプチャ画像");
			box.SetWidth(80, 50);
			if (box.DoModal() != IDOK)	return false;
		}


		VCap = cv::VideoCapture(0);
		if (!VCap.isOpened()) {
			return false;
		}

		VCap.set(cv::CAP_PROP_FRAME_WIDTH, w);
		VCap.set(cv::CAP_PROP_FRAME_HEIGHT, h);

	}
	else {
		AfxMessageBox("capture has been opened");
	}

	
	return true;
}

/********************************************************************
機  能  名  称 : キャプチャドライバを閉じる
関    数    名 : CaptureDeInit
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void	CPimpomAPI::CaptureDeInit()
{
	if (VCap.isOpened()) 
	{
		VCap.release();
	}
}


/********************************************************************
機  能  名  称 : 画像１まい取り込み
関    数    名 : CaptureOne
引          数 : bool doDrawImage	(in)再描画するかどうか
戻    り    値 : 成功すればtrue
機          能 : OpenCVを利用してVideoForWindowsに対応
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::CaptureOne(bool doDrawImage)
{
	if (VCap.isOpened()) {

		cv::Mat frame;
		try {
			VCap >> frame; // カメラから新しいフレームを取得
		}
		catch (...)
		{
			return false;
		}
		


		if (!ConvertFromCvImage2(capture_image_number, (void*)&frame)) {
			return false;
		}

		if (doDrawImage)
			DrawImage(capture_image_number, true, false, false);


		return true;
	}
	else {
		return false;
	}
}


/********************************************************************
機  能  名  称 : ビデオキャプチャが開かれているかどうか
関    数    名 : IsCaptureOpened
引          数 : 
戻    り    値 : 開かれていればtrue
機          能 : OpenCVを利用してVideoForWindowsに対応
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::IsCaptureOpened()
{
		return VCap.isOpened();
}