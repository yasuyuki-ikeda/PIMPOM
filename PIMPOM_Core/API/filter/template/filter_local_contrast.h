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
//
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "PIMPOM_API.h"
#include "..\..\resize_data.h"

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
	double         *pItg          // (O) �ϕ��摜 
) {
	int           x, y;
	double         *pPos = 0;
	double         *pPreXY = 0;
	double         *pPreY = 0;
	double         *pPreX = 0;
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
			*pPos++ = (*pPreX++) + (*pPreY++) - (*pPreXY++) + (double)(*pSrcImage++);
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
	double         *pItg          // (O) ���c�ϕ��摜 
) {
	int           x, y;
	double         *pPos = 0x00;
	double         *pPreY = 0x00;
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
			*pPos++ = (*pPreY++) + (double)(*pSrcImage * *pSrcImage);
			pSrcImage++;
		}
	}
}


//�Ǐ��R���g���X�g�g��(sliding window�� 255*( x - ave - s*��)/(2*��) )
template <class DATA>
bool filter_local_contrast(
	DATA		  *pSrc,		//(in)���͉摜
	DATA          *pDst,		//(out)�W���΍� 
	SIZE          image_size,	//(in)�摜�T�C�Y
	RECT          area,			//(in)�����̈�
	int           windowSize,	//(in)�E�B���h�E�T�C�Y
	float         contrastScale,//(in)�R���g���X�g�X�P�[��
	float         data_max,		//(in)�f�[�^����l
	float         data_min		//(in)�f�[�^�����l	
)

{
	int          areaWidth, areaHeight;
	int          integralWidth, integralHeight;
	double        *pIntegral, *pIntegral2;
	int          x, y;
	int          left, right, top, bottom;
	double        leftTopData, rightTopData, leftBottomData, rightBottomData;
	int          writePos;
	double        aveSum, aveSum2;
	double		 aveCount;
	double        average;
	double		 *pWork;


	if (image_size.cx <= 0 || image_size.cy <= 0) {
		return false;
	}
	if (area.left < 0 || area.right < area.left) {
		return false;
	}
	if (area.top < 0 || area.bottom < area.top) {
		return false;
	}
	if (area.right < 0 || image_size.cx <= area.right) {
		return false;
	}
	if (area.bottom < 0 || image_size.cy <= area.bottom) {
		return false;
	}
	if (windowSize <= 0 ) {
		return false;
	}



	areaWidth = area.right - area.left + 1;
	areaHeight = area.bottom - area.top + 1;
	integralWidth = areaWidth + 1;
	integralHeight = areaHeight + 1;

	//���[�N�������擾
	pWork = new double[integralWidth * integralHeight * 2];
	if (pWork == NULL)	return false;





	// �ϕ��摜 
	pIntegral = pWork;
	pIntegral2 = pWork + integralWidth * integralHeight;

	get_image_integral(pSrc, image_size.cx, image_size.cy, area.left, area.top, integralWidth, integralHeight, pIntegral);
	get_image_row_integral2(pSrc, image_size.cx, image_size.cy, area.left, area.top, integralWidth, integralHeight, pIntegral2);

	for (y = area.top; y <= area.bottom; y++) {

		// �ϕ��摜�A�N�Z�X�ʒu�ݒ� 
		left = 0;

		right = windowSize + 1;
		if (right >= integralWidth) {
			right = integralWidth - 1;
		}

		top = y - area.top - windowSize;
		if (top < 0) {
			top = 0;
		}
		if (top >= integralHeight) {
			top = integralHeight - 1;
		}

		bottom = y - area.top + windowSize + 1;
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

		for (x = area.left; x <= area.right; x++) {

			// �������݈ʒu 
			writePos = y * image_size.cx + x;

			left = x - area.left - windowSize;
			if (left < 0) {
				left = 0;
			}
			if (left >= integralWidth) {
				left = integralWidth - 1;
			}

			// left ���͈͊O�̂Ƃ��́Aleft = 0 �ł���A�ϕ��摜�̒l�͂O�Ȃ̂ŁA
			// ����������Ȃ����Ƃɓ�����
			aveSum2 -= pIntegral2[bottom * integralWidth + left] - pIntegral2[top * integralWidth + left];

			right = x - area.left + windowSize + 1;

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
			aveCount = (double)(right - left) * (double)(bottom - top);

			
			average = aveSum / (double)aveCount;
			double var2 = aveSum2 / (double)aveCount - average*average;
			float std = sqrt(var2);
			float offset = average - contrastScale*std;
			float drange = 2.0*contrastScale*std;

			double val = (pSrc[y * image_size.cx + x] - offset) * 255 / drange;
			//double val = var2;

			if (val < data_min)		val = data_min;
			else if (val > data_max)	val = data_max;
			
			pDst[writePos] = val;
		}
	}

	delete[]	pWork;

	return true;
}





template <class DATA>
bool filter_local_contrast_overwrite(
	DATA		  *pSrc,		//(in)���͉摜
	SIZE          image_size,	//(in)�摜�T�C�Y
	RECT          area,			//(in)�����̈�
	int           windowSize,	//(in)�E�B���h�E�T�C�Y
	float         contrastScale,//(in)�R���g���X�g�X�P�[��
	float         data_max,		//(in)�f�[�^����l
	float         data_min		//(in)�f�[�^�����l
	)
{
	bool	ret = true;
	DATA	*p_buffer = NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if (!p_buffer)	return false;

	memcpy(p_buffer, pSrc, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_local_contrast(pSrc, p_buffer, image_size, area, windowSize, contrastScale, data_max, data_min);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(pSrc, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}




//�Ǐ��R���g���X�g�g��(�p�b�`�������� (x-min)/(max-min))
template <class DATA>
bool filter_local_contrast_patch(
	DATA		  *pSrc,		//(in)���͉摜
	DATA          *pDst,		//(out)�W���΍� 
	SIZE          image_size,	//(in)�摜�T�C�Y
	RECT          area,			//(in)�����̈�
	int           patchCnt,		//(in)������
	float         data_max,		//(in)�f�[�^����l
	float         data_min		//(in)�f�[�^�����l	
	)
{
	bool ret = true;
	SIZE patch_size = { 0 };
	SIZE area_size={ 0 };
	POINT offset = { 0 };
	DATA *pMaxPatch = NULL, *pMinPatch = NULL, *pMaxImage = NULL, *pMinImage = NULL, *pSrcBuffer=NULL ,*pBuffer = NULL;
	
	area_size.cx = area.right - area.left + 1;
	area_size.cy = area.bottom - area.top + 1;
	

	if (area_size.cx <= patchCnt || area_size.cy <= patchCnt)	return false;


	pBuffer = new DATA[area_size.cx *area_size.cy * 3 + patchCnt*patchCnt * 2];
	if (pBuffer == NULL){
		ret = false;
		goto filter_local_contrast_patch_DeInit;
	}
	else
	{
		patch_size.cx = patch_size.cy = patchCnt;
		pMaxPatch = pBuffer;
		pMinPatch = pBuffer + patchCnt*patchCnt;
		pMaxImage = pBuffer + patchCnt*patchCnt * 2;
		pMinImage = pBuffer + patchCnt*patchCnt * 2 + area_size.cx *area_size.cy;
		pSrcBuffer = pBuffer + patchCnt*patchCnt * 2 + area_size.cx *area_size.cy * 2;

		for (int y = area.top; y <= area.bottom; y++)
		{
			for (int x = area.left; x <= area.right; x++)
			{
				pSrcBuffer[(x - area.left) + (y - area.top)*area_size.cx] = pSrc[x + y*image_size.cx];
			}
		}
	}
	



	//�摜�k��
	if (!(ret = resize_image(pSrcBuffer, pMaxPatch, area_size, 1, 1, patch_size, true, offset, 1))){//max�k��
		goto filter_local_contrast_patch_DeInit;
	}
	if (!(ret = resize_image(pSrcBuffer, pMinPatch, area_size, 1, 1, patch_size, true, offset, 2))){//min�k��
		goto filter_local_contrast_patch_DeInit;
	}

	//�摜�g��
	if (!(ret = resize_image(pMaxPatch, pMaxImage, patch_size, 1, 1, area_size, true, offset, 0))){//max�l�摜
		goto filter_local_contrast_patch_DeInit;
	}

	if (!(ret = resize_image(pMinPatch, pMinImage, patch_size, 1, 1, area_size, true, offset, 0))){//min�l�摜
		goto filter_local_contrast_patch_DeInit;
	}

	//���x�ϊ�
	int cnt = 0;
	for (int y = area.top; y <= area.bottom; y++)
	{
		for (int x = area.left; x <= area.right; x++)
		{
			int writePos = y * image_size.cx + x;

			float val = (pSrcBuffer[cnt] - pMinImage[cnt]) * 255.0 / (pMaxImage[cnt] - pMinImage[cnt] + 1);

			if (val < data_min)		val = data_min;
			else if (val > data_max)	val = data_max;

			pDst[writePos] = val;
			cnt++;
		}
	}



filter_local_contrast_patch_DeInit:
	delete[]	pBuffer;

	return ret;
}




template <class DATA>
bool filter_local_contrast_patch_overwrite(
	DATA		  *pSrc,		//(in)���͉摜
	SIZE          image_size,	//(in)�摜�T�C�Y
	RECT          area,			//(in)�����̈�
	int           patchCnt,		//(in)������
	float         data_max,		//(in)�f�[�^����l
	float         data_min		//(in)�f�[�^�����l
	)
{
	bool	ret = true;
	DATA	*p_buffer = NULL;

	p_buffer = new DATA[image_size.cx * image_size.cy];//�o�b�t�@�������m��
	if (!p_buffer)	return false;

	memcpy(p_buffer, pSrc, image_size.cx * image_size.cy * sizeof(DATA));//�o�b�t�@������

	//���s
	ret = filter_local_contrast_patch(pSrc, p_buffer, image_size, area, patchCnt, data_max, data_min);

	//�o�b�t�@�̒l���摜�������ɖ߂�
	memcpy(pSrc, p_buffer, image_size.cx * image_size.cy * sizeof(DATA));
	delete[]	p_buffer;

	return ret;
}
