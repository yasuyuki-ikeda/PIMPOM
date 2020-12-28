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
//�摜�ԉ��Z�@��f�P�ʂ̌v�Z
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#define PIXWISE_SUM	(0)
#define PIXWISE_SUMABS	(1)
#define PIXWISE_MUL	(2)
#define PIXWISE_DIV	(3)

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
�@  �\  ��  �� : 2�摜�̉��Z���s��
��    ��    �� : Calc2DataUnit
��          �� : int			calc_type			(in)���Z�^�C�v
                 int			src_imageA			(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			src_imageB			(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_type			(in)���ʉ摜�̃t�H�[�}�b�g
				 double         weightA				(in)���摜A�ɂ�����d��
				 double         weightA				(in)���摜A�ɂ�����d��
				 double         bias				(in)�o�C�A�X
				 bool			satulate			(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
				 
��    ��    �l : ��������� true
�@          �\ : ���摜A�ƌ��摜B�ɑ΂��āA�ȉ��̂��Âꂩ�̉��Z�������Ȃ�
					calc_type=0:  [���ʉ摜] = [���摜A] * weightA + [���摜B] * weightB + bias
					calc_type=1:  [���ʉ摜] = | [���摜A] * weightA + [���摜B] * weightB + bias |
					calc_type=2:  [���ʉ摜] = [���摜A] * weightA * [���摜B] * weightB + bias
					calc_type=3:  [���ʉ摜] = ([���摜A] * weightA) / ( [���摜B] * weightB ) + bias

                 �摜A �� �摜B�̃T�C�Y�������łȂ���΂����Ȃ�
				 �摜A �� �摜B�Ƃ�RGB�J���[�̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
              Y.Ikeda         �V�K�쐬
********************************************************************/

bool	CPimpomAPI::Calc2DataUnit(int calc_type, int src_imageA, int src_imageB, int dst_image, int dst_type, double weightA, double weightB, double bias, bool satulate )
{
	CDataUnit *pDU_A = GetDataUnit(src_imageA);
	CDataUnit *pDU_B = GetDataUnit(src_imageB);

	if (!pDU_A || !pDU_B)	return false;
	if (pDU_A->DataSize != pDU_B->DataSize)	return false;


	CDataUnit *pDstDU = FormatDataUnit(dst_image, dst_type, pDU_A->DataSize, 1, true);
	if (!pDstDU)	return false;

	bool rgb = false;
	if ((pDU_A->DataType == RGB_FORMAT || pDU_A->DataType == RGB_3D_FORMAT) && 
		(pDU_B->DataType == RGB_FORMAT || pDU_B->DataType == RGB_3D_FORMAT) && 
		(pDstDU->DataType == RGB_FORMAT || pDstDU->DataType == RGB_3D_FORMAT)
		)
	{//RGB 3�`���l���𑊎�ɂ���
		rgb = true;
	}


	for (int c = 0; c < pDstDU->ChannelNumber; c++)
	{
		for (int j = 0; j < pDstDU->DataSize.cy; j++)
		{
			for (int i = 0; i < pDstDU->DataSize.cx; i++)
			{
				double val0 = pDU_A->GetDataValue(CPoint(i, j), pDU_A->DispPage, rgb ? c : -1) * weightA;
				double val1 = pDU_B->GetDataValue(CPoint(i, j), pDU_B->DispPage, rgb ? c : -1) * weightB;
				double valD = bias;

				if (calc_type == PIXWISE_SUM)
				{
					valD += val0 + val1;
				}
				else if (calc_type == PIXWISE_SUMABS)
				{
					valD += val0 + val1;
					valD = fabs(valD);
				}
				else if (calc_type == PIXWISE_MUL)
				{
					valD += val0 * val1;
				}
				else if (val1 != 0){
					valD += val0 / val1;
				}

				pDstDU->SetDataValue(i, j, valD, 0, c, satulate);
			}
		}
	}

	return true;
}


/********************************************************************
�@  �\  ��  �� : 2�摜�̐��`���Z���s��
��    ��    �� : Linear2DataUnit
��          �� : 
				int			src_imageA			(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				int			src_imageB			(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				int			dst_type			(in)���ʉ摜�̃t�H�[�}�b�g
				double      weightA				(in)���摜A�ɂ�����d��
				double      weightB				(in)���摜B�ɂ�����d��
				double      bias				(in)�o�C�A�X
				bool	    satulate			(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)

��    ��    �l : ��������� true
�@          �\ : ���摜A�ƌ��摜B�ɑ΂��āA�ȉ��̉��Z�������Ȃ�
					[���ʉ摜] = [���摜A] * weightA + [���摜B] * weightB + bias
				�摜A �� �摜B�̃T�C�Y�������łȂ���΂����Ȃ�
				�摜A �� �摜B�Ƃ�RGB�J���[�̏ꍇ�������Ƃ�RGB�\���̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::Linear2DataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, double weightA, double weightB, double bias, bool satulate)
{
	return Calc2DataUnit(PIXWISE_SUM, src_imageA, src_imageB, dst_image, dst_type, weightA, weightB, bias, satulate);
}


/********************************************************************
�@  �\  ��  �� : 2�摜�̉��Z���s��
��    ��    �� : SumDataUnit
��          �� : int			src_imageA			(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
                 int			src_imageB			(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_type			(in)���ʉ摜�̃t�H�[�}�b�g
				 bool			satulate			(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
��    ��    �l : ��������� true
�@          �\ : ���摜A + ���摜B
                 �摜A �� �摜B�̃T�C�Y�������łȂ���΂����Ȃ�
                 �摜A �� �摜B�Ƃ�RGB�J���[�̏ꍇ�������Ƃ�RGB�\���̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
              Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SumDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_SUM, src_imageA, src_imageB, dst_image, dst_type, 1, 1, 0 , satulate );
}



/********************************************************************
�@  �\  ��  �� : 2�摜�̌��Z���s��
��    ��    �� : SubtractDataUnit
��          �� : int			src_imageA			(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
                 int			src_imageB			(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
                 int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
                 int			dst_type			(in)���ʉ摜�̃t�H�[�}�b�g
                 bool			satulate			(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)                     
��    ��    �l : ��������� true
�@          �\ : ���摜A - ���摜B
                 �摜A �� �摜B�̃T�C�Y�������łȂ���΂����Ȃ�
                 �摜A �� �摜B�Ƃ�RGB�J���[�̏ꍇ�������Ƃ�RGB�\���̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
              Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SubtractDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_SUM, src_imageA, src_imageB, dst_image, dst_type, 1, -1, 0, satulate);
}


/********************************************************************
�@  �\  ��  �� : 2�摜�̌��Z���s��
��    ��    �� : SubAbsDataUnit
��          �� : int			src_imageA			(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				int			src_imageB			(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				int			dst_type			(in)���ʉ摜�̃t�H�[�}�b�g
bool			satulate			(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
��    ��    �l : ��������� true
�@          �\ :
				| ���摜A - ���摜B |
				�摜A �� �摜B�̃T�C�Y�������łȂ���΂����Ȃ�
				�摜A �� �摜B�Ƃ�RGB�J���[�̏ꍇ�������Ƃ�RGB�\���̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SubAbsDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_SUMABS, src_imageA, src_imageB, dst_image, dst_type, 1, -1, 0, satulate);
}


/********************************************************************
�@  �\  ��  �� : 2�摜�̐ώZ���s��
��    ��    �� : MultiplyDataUnit
��          �� : int			src_imageA			(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			src_imageB			(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_type			(in)���ʉ摜�̃t�H�[�}�b�g
				 bool			satulate			(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
��    ��    �l : ��������� true
�@          �\ : ���摜A * ���摜B
				�摜A �� �摜B�̃T�C�Y�������łȂ���΂����Ȃ�
				�摜A �� �摜B�Ƃ�RGB�J���[�̏ꍇ�������Ƃ�RGB�\���̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
			Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::MultiplyDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_MUL, src_imageA, src_imageB, dst_image, dst_type, 1, 1, 0, satulate);
}


/********************************************************************
�@  �\  ��  �� : 2�摜�̏��Z���s��
��    ��    �� : DivideDataUnit
��          �� : int			src_imageA			(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			src_imageB			(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_type			(in)���ʉ摜�̃t�H�[�}�b�g
				 bool			satulate			(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)

��    ��    �l : ��������� true
�@          �\ : ���摜A / ���摜B
				�摜A �� �摜B�̃T�C�Y�������łȂ���΂����Ȃ�
				�摜A �� �摜B�Ƃ�RGB�J���[�̏ꍇ�������Ƃ�RGB�\���̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�
				�摜B�̒l��0�̂Ƃ��́A���ʂ̒l��0�ɂȂ�

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
			Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::DivideDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate)
{
	return Calc2DataUnit(PIXWISE_DIV, src_imageA, src_imageB, dst_image, dst_type, 1, 1, 0, satulate);
}



/********************************************************************
�@  �\  ��  �� : 1�摜�̐��`���Z���s��
��    ��    �� : LinearDataUnit
��          �� : int			src_image			(in)���摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
                 int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_type			(in)���ʉ摜�̃t�H�[�}�b�g
                 double         weight				(in)���摜�ɑ΂���d��
				 double			bias				(in)�o�C�A�X
				 bool			satulate			(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)

��    ��    �l : ��������� true
�@          �\ :
                [���ʉ摜] = [���摜] * weight + bias
				���摜��RGB�J���[�̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
			Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LinearDataUnit(int src_image, int dst_image, int dst_type, double weight, double bias, bool satulate)
{
	CDataUnit *pDU = GetDataUnit(src_image);

	if (!pDU )	return false;

	CDataUnit *pDstDU = FormatDataUnit(dst_image, dst_type, pDU->DataSize, 1, true);
	if (!pDstDU)	return false;

	bool rgb = false;
	if ((pDU->DataType == RGB_FORMAT || pDU->DataType == RGB_3D_FORMAT) &&
		(pDstDU->DataType == RGB_FORMAT || pDstDU->DataType == RGB_3D_FORMAT)
		)
	{//RGB 3�`���l���𑊎�ɂ���
		rgb = true;
	}


	for (int c = 0; c < pDstDU->ChannelNumber; c++)
	{
		for (int j = 0; j < pDU->DataSize.cy; j++)
		{
			for (int i = 0; i < pDU->DataSize.cx; i++)
			{
				double valS = pDU->GetDataValue(CPoint(i, j), pDU->DispPage, rgb ? c : -1);
				double valD = valS * weight + bias;

				pDstDU->SetDataValue(i, j, valD, 0, c, satulate);
			}
		}
	}

	return true;
}



/********************************************************************
�@  �\  ��  �� : �摜�̐�Βl���Ƃ�
��    ��    �� : AbsDataUnit
��          �� : int			src_image			(in)���摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_image			(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ��������� true
�@          �\ : 
			[���ʉ摜] = [���摜]�̐�Βl
			���摜��RGB�J���[�̏ꍇ����RGB�\���̏ꍇ�́ARGB���ꂼ��ɑ΂��ĉ��Z�������Ȃ�

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
			Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::AbsDataUnit(int src_image, int dst_image)
{
	CDataUnit *pDU = GetDataUnit(src_image);

	if (!pDU)	return false;

	CDataUnit *pDstDU = FormatDataUnit(dst_image, pDU->DataType, pDU->DataSize, 1, true);
	if (!pDstDU)	return false;

	int channels = 1;
	if ((pDU->DataType == RGB_FORMAT || pDU->DataType == RGB_3D_FORMAT) && pDU->DispRGB)
	{//RGB 3�`���l���𑊎�ɂ���
		channels = 3;
	}


	for (int c = 0; c < channels; c++)
	{
		for (int j = 0; j < pDU->DataSize.cy; j++)
		{
			for (int i = 0; i < pDU->DataSize.cx; i++)
			{
				double valS = pDU->GetDataValue(CPoint(i, j), pDU->DispPage, c);
				double valD = fabs(valS);

				pDstDU->SetDataValue(i, j, valD, 0, c, true);
			}
		}
	}

	return true;
}
