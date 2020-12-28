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
//Canny̨��
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

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         ��    ��    �Q    ��         
// *************************************


/********************************************************************
�@  �\  ��  �� : �������o
��    ��    �� : HoughLines
��          �� : long		image_num			(in)�ΏۃG�b�W�摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int		pixSkip				(in)���[��Ԃ̋�������\[pix]
				 int		dirSkip				(in)���[��Ԃ̊p�x����\[deg]
				 int		voteThresh			(in)���[��臒l�p�����[�^�D�\���ȕ[�i  >\texttt{threshold} �j�𓾂������݂̂��o�͂���܂��D
				 double		minLineLength		(in)�ŏ��̐������D������Z�������͊��p����܂�
				 double		maxLineGap			(in)2�_�����������ɂ���ƌ��Ȃ��ꍇ�ɋ��e�����ő勗���D
��    ��    �l : ���o����������
�@          �\ : Canny�t�B���^�ȂǂŒ��o�����G�b�W�摜�ɑ΂��A�m���IHough�ϊ����s��
				�@
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int  CPimpomAPI::HoughLines(long image_num, int pixSkip, int dirSkip, int voteThresh, double minLineLength, double maxLineGap )
{
	CDataUnit	*p_du;


	//�����`�F�b�N

	if(image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}else if(image_num >= GetDataUnitNumber() || image_num<0){
		return 0;
	}


	p_du = GetDataUnit(image_num);
		if(!p_du)	return	0;

	CRect calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�

		if(p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) )//�O���[�摜�ɑ΂��鏈��
		{
			if(p_du->pByteData){//8bit�摜
				
				cv::Mat srcMat, subSrcMat;
				
				if( !ConvertToCvImage2(image_num, (void*)&srcMat, (void*)&subSrcMat) )//�摜��������cv::Mat
				{
					return false;
				}
			
				std::vector<Vec4i> lines;
				cv::HoughLinesP( subSrcMat, lines, pixSkip, CV_PI/180.0*(double)dirSkip, voteThresh, minLineLength, maxLineGap );//�m���IHough�ϊ����s

				//�������o���ʂ��}�[�N�ɔ��f


				p_du->ClearMark();
				for( size_t i = 0; i < lines.size() && i< MARK_NUMBER_MAX; i++ )
				{ 
					CMark mark( (float)lines[i][0] + calc_area.left, 
								(float)lines[i][1] + calc_area.top, 
								(float)lines[i][2] + calc_area.left, 
								(float)lines[i][3] + calc_area.top);
					p_du->AddMark(mark);
				}
				
				SetMarkEditMode(1);

				return (int)lines.size();
				
			}else if(p_du->pShortData){//16bit�摜

				MessageBox("this function is supported only for BYTE format");
				return 0;

			}else if(p_du->pWordData){//16bit�摜
				MessageBox("this function is supported only for BYTE format");
				return 0;
			}else if(p_du->pLongData){//32bit�摜
				MessageBox("this function is supported only for BYTE format");
				return 0;
			}else if(p_du->pDwordData){//32bit�摜
				MessageBox("this function is supported only for BYTE format");
				return 0;
			}else if(p_du->pFloatData){//�����摜
				MessageBox("this function is supported only for BYTE format");
				return 0;
			}
		}
		else//RGB�摜�ɑ΂��鏈��
		{
			MessageBox("this function is supported only for BYTE format");
			return 0;
	
		}
		return 0;
}


