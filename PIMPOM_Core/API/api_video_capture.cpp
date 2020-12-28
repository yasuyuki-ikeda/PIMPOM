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
//�r�f�I�L���v�`��
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"

#include "opencv_headers.h"
#include "PIMPOM_API.h"


// *************************************
//         �}  �N  ��   ��   �`         
// *************************************

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

// *************************************
//         ��    �I    ��    ��         
// *************************************
static  cv::VideoCapture  VCap;//�r�f�I�L���v�`��(opencv3)

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         ��    ��    �Q    ��         
// *************************************

/********************************************************************
�@  �\  ��  �� : �L���v�`���h���C�o���J��
��    ��    �� : CaptureInit
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
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
			box.RegistVar("�摜��", &w);
			box.RegistVar("�摜����", &h);
			box.RegistVar("�摜�������ԍ�", &capture_image_number);
			box.SetTitle("�L���v�`���摜");
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
�@  �\  ��  �� : �L���v�`���h���C�o�����
��    ��    �� : CaptureDeInit
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::CaptureDeInit()
{
	if (VCap.isOpened()) 
	{
		VCap.release();
	}
}


/********************************************************************
�@  �\  ��  �� : �摜�P�܂���荞��
��    ��    �� : CaptureOne
��          �� : bool doDrawImage	(in)�ĕ`�悷�邩�ǂ���
��    ��    �l : ���������true
�@          �\ : OpenCV�𗘗p����VideoForWindows�ɑΉ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::CaptureOne(bool doDrawImage)
{
	if (VCap.isOpened()) {

		cv::Mat frame;
		try {
			VCap >> frame; // �J��������V�����t���[�����擾
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
�@  �\  ��  �� : �r�f�I�L���v�`�����J����Ă��邩�ǂ���
��    ��    �� : IsCaptureOpened
��          �� : 
��    ��    �l : �J����Ă����true
�@          �\ : OpenCV�𗘗p����VideoForWindows�ɑΉ�
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::IsCaptureOpened()
{
		return VCap.isOpened();
}