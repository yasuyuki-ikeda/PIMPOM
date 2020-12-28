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
//�t�@�C��IO(AVI)
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include <vfw.h>

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

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         ��    ��    �Q    ��         
// *************************************


/********************************************************************
�@  �\  ��  �� :  AVI�t�@�C����ǂݍ���
��    ��    �� : LoadAvi
��          �� : long		image_number(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		filename	(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �쐬�r���ł��܂��������Ȃ�
********************************************************************/
bool	CPimpomAPI::LoadAvi(long image_number, CString	filename)
{
	bool	ret=true;
	PAVIFILE aviFile = NULL;	//AVI �t�@�C���I�u�W�F�N�g
	PAVISTREAM aviStream = NULL;	//AVI �t�@�C���̃X�g���[���I�u�W�F�N�g
	PGETFRAME aviFrame = NULL;		//AVI �t�@�C������t���[�����𓀂���I�u�W�F�N�g
	int		streamLen;
	BITMAPINFOHEADER *pBmpInfoHeader=NULL;
	CSize bmSize;//�r�b�g�}�b�v�T�C�Y
	long bmpWidthGap = 0;//DIB�f�[�^����RAW�f�[�^���̍�


    if (AVIFileOpen(&aviFile , filename , OF_READ , NULL)) {//AVIFile ���J��
        ret = false;
		goto LoadAvi_Deinit;
	}

    if (AVIFileGetStream(aviFile , &aviStream , streamtypeVIDEO , 0)) {//AVI �X�g���[�����擾����
        ret = false;
		goto LoadAvi_Deinit;
    }

    aviFrame = AVIStreamGetFrameOpen(aviStream , NULL);//�t���[�����𓀂��邽�߂̃I�u�W�F�N�g���擾
    if (aviFrame == NULL) {
        ret = false;
		goto LoadAvi_Deinit;
    }

	streamLen = AVIStreamLength(aviStream);//
	if(streamLen<=0){
        ret = false;
		goto LoadAvi_Deinit;
	}

	pBmpInfoHeader = (BITMAPINFOHEADER *)AVIStreamGetFrame(aviFrame,0);//�r�b�g�}�b�v�̌`���m�F
	if(pBmpInfoHeader==NULL){
        ret = false;
		goto LoadAvi_Deinit;
	}
	bmSize = CSize(pBmpInfoHeader->biWidth, pBmpInfoHeader->biHeight);
	


	//�f�[�^�ǂݏo��
	if(pBmpInfoHeader->biBitCount==8)//8bit
	{
		BYTE *ptr = Get3DMemory(image_number, bmSize, streamLen, true);
		if(ptr==NULL){
	        ret = false;
			goto LoadAvi_Deinit;
		}

		//DIB�f�[�^����RAW�f�[�^���̍�
		if( bmSize.cx % 4 == 0)	bmpWidthGap = 0;	
		else			bmpWidthGap = 4 - bmSize.cx % 4;

		//�J���[�p���b�g�̓ǂݍ���
		CDataUnit *pdu = GetDataUnit(image_number);
		memcpy( pdu->pDispBufferBmpInfo->bmiColors, pBmpInfoHeader + sizeof(BITMAPINFOHEADER), 1024);


		//�f�[�^�̓ǂݍ���
		for(int n=0 ; n<streamLen ; n++)
		{
			pBmpInfoHeader = (BITMAPINFOHEADER *)AVIStreamGetFrame(aviFrame,n);
			if(pBmpInfoHeader==NULL){
				ret = false;
				goto LoadAvi_Deinit;
			}

			//�f�[�^�ǂݍ���
			for(int j = bmSize.cy-1  ;  j>=0  ;	j-- ){//�㉺���]
				memcpy( ptr + bmSize.cx*(bmSize.cy-1-j) , 
						pBmpInfoHeader + sizeof(BITMAPINFOHEADER) + 1024 + (bmSize.cx + bmpWidthGap)*j , 
						bmSize.cx  );
			}
		}
	}
	else if(pBmpInfoHeader->biBitCount==24)//RGB
	{
		BYTE *ptr = GetRGB3DMemory(image_number, bmSize, streamLen, true);
		if(ptr==NULL){
	        ret = false;
			goto LoadAvi_Deinit;
		}

		//DIB�f�[�^����RAW�f�[�^���̍�
		if( (bmSize.cx*3) % 4 == 0)	bmpWidthGap = 0;	
		else				bmpWidthGap = 4 - (bmSize.cx*3) % 4;

		//�f�[�^�̈ꎞ�ۊǏꏊ�̍쐬
		BYTE *ptmp_rgb = (BYTE*)malloc( (bmSize.cx*3)*bmSize.cy );
		if(!ptmp_rgb){
			ret = false;
			goto LoadAvi_Deinit;
		}


		//�f�[�^�̓ǂݍ���
		for(int n=0 ; n<streamLen ; n++)
		{
			pBmpInfoHeader = (BITMAPINFOHEADER *)AVIStreamGetFrame(aviFrame,n);
			if(pBmpInfoHeader==NULL){
				free(ptmp_rgb);
				ret = false;
				goto LoadAvi_Deinit;
			}

			//�f�[�^�ǂݍ���
			for(int j = bmSize.cy-1  ;  j>=0  ;	j-- ){//�㉺���]
				memcpy( ptmp_rgb + (bmSize.cx*3)*(bmSize.cy-1-j) , 
						pBmpInfoHeader + sizeof(BITMAPINFOHEADER) + 1024 + (bmSize.cx*3 + bmpWidthGap)*j , 
						bmSize.cx*3  );
			}

			//�f�[�^�̕ϊ�
			for(int j=0 ; j<bmSize.cy ; j++){
				for(int i=0 ; i<bmSize.cx ; i++){
					*(ptr + i + j*bmSize.cx )					   = *(ptmp_rgb + i*3 + j*(bmSize.cx*3)+2);//r
					*(ptr + i + j*bmSize.cx + bmSize.cx*bmSize.cy)   = *(ptmp_rgb + i*3 + j*(bmSize.cx*3)+1);//g
					*(ptr + i + j*bmSize.cx + bmSize.cx*bmSize.cy*2) = *(ptmp_rgb + i*3 + j*(bmSize.cx*3)+0);//b
				}
			}
			
		}

		free(ptmp_rgb);//�f�[�^�̈ꎞ�ۊǏꏊ�̊J��

	}






LoadAvi_Deinit:
    if (aviFrame) AVIStreamGetFrameClose(aviFrame);
    if (aviStream) AVIStreamRelease(aviStream);
    if (aviFile) AVIFileRelease(aviFile);

	return ret;
}