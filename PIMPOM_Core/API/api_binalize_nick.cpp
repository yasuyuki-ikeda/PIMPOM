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
//NICK ��2�l��
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#define NICK_MIN_K_VALUE            (-1.0f)
#define NICK_MAX_K_VALUE            0.0f

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




// �ϕ��摜���� 
template <class DATA>
void get_image_integral(
	DATA		  *pSrc,         // (I) ���摜
	int           width,         // (I) ���摜��
	int           height,        // (I) ���摜����
	int           x1,            // (I) �Ώۋ�`������W
	int           y1,            // (I) �V
	int           integralWidth, // (I) �ϕ��摜�� 
	int           integralHeight,// (I) �ϕ��摜���� 
	float         *pItg          // (O) �ϕ��摜 
) {
	int           x, y;
	float         *pPos = 0;
	float         *pPreXY = 0;
	float         *pPreY = 0;
	float         *pPreX = 0;
	DATA		  *pSrcImage;

	// �ŏ��̍s���O�ŏ����� 
	for (x = 0; x < integralWidth; x++) {
		pItg[x] = 0;
	}

	// �v�Z�p�T���v�����O�ʒu�������� 
	pPos = pPreXY = pPreY = pItg;
	pPos += integralWidth;
	pPreX = pPos;

	for (y = 1; y < integralHeight; y++) {

		pSrcImage = pSrc + (y1 + y - 1) * width + x1;

		// �ŏ��̗���O�ŏ����� 
		*pPos++ = 0;

		pPreY++;
		for (x = 1; x < integralWidth; x++) {
			*pPos++ = (*pPreX++) + (*pPreY++) - (*pPreXY++) + (float)(*pSrcImage++);
		}
		pPreX++;
		pPreXY++;
	}
}

// �c�����݂̂̓��ϕ��摜�쐬 
template <class DATA>
static void get_image_row_integral2(
	DATA	      *pSrc,         // (I) ���摜 
	int           width,         // (I) ���摜�� 
	int           height,        // (I) ���摜���� 
	int           x1,            // (I) �Ώۋ�`������W 
	int           y1,            // (I) �V 
	int           integralWidth, // (I) �ϕ��摜�� 
	int           integralHeight,// (I) �ϕ��摜���� 
	float         *pItg          // (O) ���c�ϕ��摜 
) {
	int           x, y;
	float         *pPos = 0x00;
	float         *pPreY = 0x00;
	DATA		  *pSrcImage;

	// �ŏ��̍s���O�ŏ����� 
	for (x = 0; x < integralWidth; x++) {
		pItg[x] = 0;
	}

	// �v�Z�p�T���v�����O�ʒu�������� 
	pPos = pPreY = pItg;
	pPos += integralWidth;

	for (y = 1; y < integralHeight; y++) {

		pSrcImage = pSrc + (y1 + y - 1) * width + x1;

		// �ŏ��̗���O�ŏ����� 
		*pPos++ = 0;

		pPreY++;
		for (x = 1; x < integralWidth; x++) {
			*pPos++ = (*pPreY++) + (float)(*pSrcImage * *pSrcImage);
			pSrcImage++;
		}
	}
}



template <class DATA>
bool binarize_nick(
	DATA		  *pSrc,		//(in)���͉摜
	int           width,		//(in)���͉摜��
	int           height,		//(in)���͉摜����
	int           x1,			//(in)�����̈捶��X
	int           y1,			//(in)�����̈捶��Y
	int           x3,			//(in)�����̈�E��X
	int           y3,			//(in)�����̈�E��Y
	int           windowSize,	//(in)NICK�E�B���h�E�T�C�Y
	float         k,			//(in)NICK��k
	BYTE		  fg_val,		//(in)
	BYTE		  bg_val,		//(in)
	unsigned char *pDst		//(out)(out)2�l�摜 
)
{
	int          areaWidth, areaHeight;
	int          integralWidth, integralHeight;
	float        *pIntegral, *pIntegral2;
	int          x, y;
	int          left, right, top, bottom;
	float        leftTopData, rightTopData, leftBottomData, rightBottomData;
	int          writePos;
	float        aveSum, aveSum2;
	unsigned int aveCount;
	float        term1, term2;
	float        average;
	float		 *pWork;


	if (width <= 0 || height <= 0) {
		return false;
	}
	if (x1 < 0 || x3 < x1) {
		return false;
	}
	if (y1 < 0 || y3 < y1) {
		return false;
	}
	if (x3 < 0 || width <= x3) {
		return false;
	}
	if (y3 < 0 || height <= y3) {
		return false;
	}
	if (windowSize <= 0 ) {
		return false;
	}
	if (k < NICK_MIN_K_VALUE || NICK_MAX_K_VALUE < k) {
		return false;
	}


	areaWidth = x3 - x1 + 1;
	areaHeight = y3 - y1 + 1;
	integralWidth = areaWidth + 1;
	integralHeight = areaHeight + 1;

	//���[�N�������擾
	pWork = new float[integralWidth * integralHeight * 2];
	if (pWork == NULL)	return false;



	// NICK decision ��臒l���ȉ��ɐݒ肷��
	// TH = m + k \sqrt{ \frac{ \sum P_i^2 - m^2 }{ NP } }
	//  TH  : threshold
	//  m   : mean
	//  k   : -0.2(less noise but broken labels appear) �� -0.1(unbroken but noisey)
	//  P_i : pixel value
	//  NP  : number of pixel




	// �ϕ��摜 
	pIntegral = pWork;
	pIntegral2 = pWork + integralWidth * integralHeight;

	get_image_integral(pSrc, width, height, x1, y1, integralWidth, integralHeight, pIntegral);
	get_image_row_integral2(pSrc, width, height, x1, y1, integralWidth, integralHeight, pIntegral2);

	for (y = y1; y <= y3; y++) {

		// �ϕ��摜�A�N�Z�X�ʒu�ݒ� 
		left = 0;

		right = windowSize + 1;
		if (right >= integralWidth) {
			right = integralWidth - 1;
		}

		top = y - y1 - windowSize;
		if (top < 0) {
			top = 0;
		}
		if (top >= integralHeight) {
			top = integralHeight - 1;
		}

		bottom = y - y1 + windowSize + 1;
		if (bottom < 0) {
			bottom = 0;
		}
		if (bottom >= integralHeight) {
			bottom = integralHeight - 1;
		}

		aveSum2 = 0;
		for (x = left; x < right; x++) {
			aveSum2 += pIntegral2[bottom * integralWidth + x] - pIntegral2[top * integralWidth + x];
		}

		for (x = x1; x <= x3; x++) {

			// �������݈ʒu 
			writePos = y*width + x;//(y - y1) * areaWidth + x - x1;

			left = x - x1 - windowSize;
			if (left < 0) {
				left = 0;
			}
			if (left >= integralWidth) {
				left = integralWidth - 1;
			}

			// left ���͈͊O�̂Ƃ��́Aleft = 0 �ł���A�ϕ��摜�̒l�͂O�Ȃ̂ŁA
			// ����������Ȃ����Ƃɓ�����
			aveSum2 -= pIntegral2[bottom * integralWidth + left] - pIntegral2[top * integralWidth + left];

			right = x - x1 + windowSize + 1;

			if (right < 0) {
				right = 0;
			}
			if (right >= integralWidth) {
				right = integralWidth - 1;
				// �͈͊O�̂Ƃ���aveSum2�ɑ����l�͂Ȃ� 
			}
			else {
				aveSum2 += pIntegral2[bottom * integralWidth + right] - pIntegral2[top * integralWidth + right];
			}

			// �ϕ��摜�Q�� 
			leftTopData = pIntegral[top * integralWidth + left];
			rightTopData = pIntegral[top * integralWidth + right];
			leftBottomData = pIntegral[bottom * integralWidth + left];
			rightBottomData = pIntegral[bottom * integralWidth + right];

			// ���ϒl���v 
			aveSum = leftTopData + rightBottomData - rightTopData - leftBottomData;

			// �ʐ� 
			aveCount = (right - left) * (bottom - top);





			// NICK decision 
			if (pSrc[y * width + x] * aveCount >= aveSum) {
				pDst[writePos] = fg_val;
			}
			else {
				// (P_i - m)^2
				term1 = (aveSum - pSrc[y * width + x] * aveCount) / aveCount;
				term1 *= term1;

				// (\sum P_i^2 - m^2) / NP
				average = aveSum / aveCount;
				term2 = (aveSum2 - average * average) / aveCount;



				// k^2 (\sum P_i^2 - m^2) / NP > (P_i - m)^2
				if (k*k * term2 < term1) {
					pDst[writePos] = bg_val;
				}
				else {
					pDst[writePos] = fg_val;
				}
			}
		}
	}

	delete[]	pWork;

	return true;
}



/********************************************************************
�@  �\  ��  �� : �摜��NICK�̎�@��2�l�����Ăׂ̉摜�������ɓ����
��    ��    �� : BinalizeNick
��          �� : 
	int			src_number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
	int			dst_number			(in)2�l����̉摜�����郁����
	int			windowSize			(in)NICK�E�B���h�E�T�C�Y
	float		k					(in)NICK��k
	bool		inverse				(in)�Q�l���](true)
	BYTE		fg_val				(in)�O�i�l
	BYTE		bg_val				(in)�w�i�l
	bool		show_message		(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩

��    ��    �l : ���������true
�@          �\ : �V����BYTE�^���������擾���A臒l�ȓ��̉�f��on_label/����ȊO��off_label�ɂ���

   NICK�@�F
   �@�@�ȉ��̎��ŃE�B���h�E���Ƃɂ�2�l��臒lT�����߂�
	     T = ave + k * sqrt{ (��I^2 + ave^2)/np }

		  I: pixel value
		  ave: averaged pixel value within the window
		  np: number of pixels within the window

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::BinalizeNick(long src_number, long dst_number, int windowSize, float k, bool inverse, BYTE fg_val, BYTE bg_val, bool show_message, bool toMultiImage)
{
	bool ret = true;
	CDataUnit	*p_du = NULL, *p_dst_du = NULL;
	CRect		calc_area;
	BYTE		fval, bval;

	//�����`�F�b�N
	if ((p_du = GetDataUnit(src_number)) == NULL)	return false;

	if (dst_number >= GetDataUnitNumber() || dst_number<0)	return false;

	if (src_number == dst_number)	return false;//�����ԍ��̎w��͂ł��Ȃ�

	if (GetDataUnit(dst_number)) {//���ʂ̊i�[��Ƃ��Ďw�肳�ꂽ�摜�������ɁA���łɕʂ̃f�[�^�������Ă���ꍇ�͖₢���킹
		if (show_message) {
			CString str;
			str.Format("�摜������%d�͏㏑������܂�", dst_number);
			if (AfxMessageBox(str, MB_OKCANCEL | MB_ICONINFORMATION) != IDOK) {
				return false;
			}
		}
	}


	int pageStart, pageEnd, channelStart, channelEnd, dstPageNum;
	if (toMultiImage)//�����y�[�W�E�`���l���Ή�
	{
		pageStart = 0;
		pageEnd = p_du->PageNumber - 1;
		channelStart = 0;
		channelEnd = p_du->ChannelNumber - 1;
		dstPageNum = p_du->PageNumber * p_du->ChannelNumber;
	}
	else{
		pageStart = pageEnd = p_du->DispPage;
		channelStart = channelEnd = p_du->DispChannel;
		dstPageNum = 1;
	}


	if (Get3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//�V����3D�^���������擾

	CopyDataUnit(src_number, dst_number, false, false, true, false, true, true);//�\���ʒu�E�}�X�N�E�����̈�E�}�[�N���󂯌p��

	p_dst_du = GetDataUnit(dst_number);



	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//�摜�̃T�C�Y�i��f���j
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�


	if (inverse) {
		fval = bg_val;
		bval = fg_val ;
	}
	else {
		fval = fg_val;
		bval = bg_val;
	}

	if (p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT)) {//�O���[�摜�ɑ΂��鏈��

		int dstPageCnt = 0;
		for (int page = pageStart; page <= pageEnd; page++)
		{
			for (int channel = channelStart; channel <= channelEnd; channel++)
			{

				if (p_du->pByteData) {	//8bit�摜
					ret = binarize_nick(
						p_du->pByteData + imsize * (page * p_du->ChannelNumber + channel),
						p_du->DataSize.cx, p_du->DataSize.cy,
						calc_area.left, calc_area.top, calc_area.right, calc_area.bottom,
						windowSize, k, fval, bval, 
						p_dst_du->pByteData + dstPageCnt*imsize);

				}
				else if (p_du->pShortData) {	//16bit�摜
					ret = binarize_nick(
						p_du->pShortData + imsize * (page * p_du->ChannelNumber + channel),
						p_du->DataSize.cx, p_du->DataSize.cy,
						calc_area.left, calc_area.top, calc_area.right, calc_area.bottom,
						windowSize, k, fval, bval, 
						p_dst_du->pByteData + dstPageCnt*imsize);

				}
				else if (p_du->pWordData) {	//16bit�摜
					ret = binarize_nick(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_du->DataSize.cx, p_du->DataSize.cy,
						calc_area.left, calc_area.top, calc_area.right, calc_area.bottom,
						windowSize, k, fval, bval, 
						p_dst_du->pByteData + dstPageCnt*imsize);

				}
				else if (p_du->pLongData) {	//32bit�摜
					ret = binarize_nick(
						p_du->pLongData + imsize * (page * p_du->ChannelNumber + channel),
						p_du->DataSize.cx, p_du->DataSize.cy,
						calc_area.left, calc_area.top, calc_area.right, calc_area.bottom,
						windowSize, k, fval, bval, 
						p_dst_du->pByteData + dstPageCnt*imsize);

				}
				else if (p_du->pDwordData) {	//32bit�摜
					ret = binarize_nick(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_du->DataSize.cx, p_du->DataSize.cy,
						calc_area.left, calc_area.top, calc_area.right, calc_area.bottom,
						windowSize, k, fval, bval, 
						p_dst_du->pByteData + dstPageCnt*imsize);

				}
				else if (p_du->pFloatData) {//���������_
					ret = binarize_nick(
						p_du->pFloatData + imsize * (page * p_du->ChannelNumber + channel),
						p_du->DataSize.cx, p_du->DataSize.cy,
						calc_area.left, calc_area.top, calc_area.right, calc_area.bottom,
						windowSize, k, fval, bval, 
						p_dst_du->pByteData + dstPageCnt*imsize);

				}

				dstPageCnt++;
			}
		}

		if (toMultiImage){//�����y�[�W�E�`���l���Ή�
			p_dst_du->DispPage = p_du->DispPage;
			p_dst_du->DispChannel = p_du->DispChannel;
		}

	}
	else {//RGB�摜�ɑ΂��鏈��
		unsigned char*pBrightness = new unsigned char[imsize];
		if (pBrightness == NULL)	return false;

		//�������񖾓x�l�݂̂̉摜�ɂ���
		for (int j = calc_area.top; j <= calc_area.bottom; j++) {
			for (int i = calc_area.left; i <= calc_area.right; i++) {
				*(pBrightness + i + j*p_du->DataSize.cx) = p_du->GetDataValue(CPoint(i, j), -1, -1);
			}
		}

		ret = binarize_nick(
			pBrightness,
			p_du->DataSize.cx, p_du->DataSize.cy,
			calc_area.left, calc_area.top, calc_area.right, calc_area.bottom,
			windowSize, k, fval, bval, p_dst_du->pByteData);

		delete[]	pBrightness;
	}


	return ret;
}