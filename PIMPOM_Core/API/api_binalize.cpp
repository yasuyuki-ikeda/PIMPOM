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
//2�l��
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "filter\template\filter_gauss.h"
#include "filter\template\filter_log.h"
#include "opencv_headers.h"

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



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�O���[�o��臒l�ɂ��2�l��
template <class DATA, class DATA2>
bool	binalize_global(
				 DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
				 DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
				 CSize			image_size,		//(in)�摜�T�C�Y
				 double			min_thresh,		//(in)�ŏ�臒l
				 double			max_thresh,		//(in)�ő�臒l
				 bool			inverse,		//(in)�Q�l���](true)
				 double			fg_val,			//(in)�O�i�l
				 double			bg_val,			//(in)�w�i�l
				 CRect			work_area		//(in)�����̈�
				 )
{
	double fg, bg;

	if(!inverse){
		fg = fg_val;
		bg = bg_val;
	}else{
		fg = bg_val;
		bg = fg_val;
	}

	for(int j = work_area.top ; j<=work_area.bottom ; j++)
	{
		for(int i = work_area.left ; i<=work_area.right ; i++)
		{
			double val = *(p_src_data + i + j*image_size.cx);

			if(val >= min_thresh && val<= max_thresh){
				*(p_dst_data + i + j*image_size.cx) = fg;
			}else{
				*(p_dst_data + i + j*image_size.cx) = bg;
			}
		}
	}

	return true;
}


//�K�E�V�A�����ȍ����ɂ��2�l��
template <class DATA, class DATA2>
bool	binalize_gauss(
				 DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
				 DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
				 CSize			image_size,		//(in)�摜�T�C�Y
				 int			filter_size,	//(in)
				 int			offset,			//(in)
				 int			inverse,		//(in)�Q�l���](true)
				 double			fg_val,			//(in)�O�i�l
				 double			bg_val,			//(in)�w�i�l
				 CRect			work_area,		//(in)�����̈�
				 int			pad_type		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
				 )
{
	bool ret=true;

	float *p_buffer = new float[image_size.cx * image_size.cy ];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memset(p_buffer, 0, image_size.cx * image_size.cy * sizeof(float));//�o�b�t�@������

	ret = filter_gauss_unshade(p_src_data, p_buffer, image_size, filter_size, -1, offset, 1, work_area, FLT_MAX, -FLT_MAX, pad_type,NULL);//�K�E�V�A���t�B���^��������

	if(ret){
		binalize_global(p_buffer, p_dst_data, image_size, 0 ,FLT_MAX, inverse, fg_val, bg_val, work_area);
	}

	delete p_buffer;

	return ret;
}



//���v���V�A���I�t�K�E�V�A���ɂ��2�l��
template <class DATA, class DATA2>
bool	binalize_laplace_gauss(
				 DATA			*p_src_data,	//(in)���͉摜�z��̐擪�|�C���^
				 DATA2			*p_dst_data,	//(out)�o�͉摜�z��̐擪�|�C���^
				 CSize			image_size,		//(in)�摜�T�C�Y
				 int			filter_size,	//(in)
				 int			offset,			//(in)
				 int			inverse,		//(in)�Q�l���](true)
				 double			fg_val,			//(in)�O�i�l
				 double			bg_val,			//(in)�w�i�l
				 CRect			work_area,		//(in)�����̈�
				 int			pad_type		//(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
				 )
{
	bool ret=true;

	float *p_buffer = new float[image_size.cx * image_size.cy ];//�o�b�t�@�������m��
	if(!p_buffer)	return false;

	memset(p_buffer, 0, image_size.cx * image_size.cy * sizeof(float));//�o�b�t�@������

	ret = filter_lapcace_gauss(p_src_data, p_buffer, image_size, filter_size, -1, work_area, 0,1, FLT_MAX, -FLT_MAX , pad_type, NULL);//�K�E�V�A���t�B���^��������

	if(ret){
		binalize_global(p_buffer, p_dst_data, image_size, offset ,FLT_MAX, inverse, fg_val, bg_val, work_area);
	}

	delete[] p_buffer;

	return ret;
}


/********************************************************************
�@  �\  ��  �� : �摜��2�l�����Ăׂ̉摜�������ɓ����
��    ��    �� : Binalize
��          �� : int			src_number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_number			(in)2�l����̉摜�����郁����
				 double			 min_thresh			(in)�ŏ�臒l
				 double			 max_thresh			(in)�ő�臒l
				 bool			inverse				(in)�Q�l���](true)
				 BYTE			fg_val				(in)�O�i�l
				 BYTE			bg_val				(in)�w�i�l
				 bool			show_message		(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩

��    ��    �l : ���������true
�@          �\ : �V����BYTE�^���������擾���A臒l�ȓ��̉�f��on_label/����ȊO��off_label�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::Binalize(long src_number, long dst_number, double min_thresh, double max_thresh, bool inverse, BYTE fg_val, BYTE bg_val, bool show_message, bool toMultiImage)
{
	bool ret=true;
	CDataUnit	*p_du=NULL, *p_dst_du=NULL;
	CRect		calc_area;


	//�����`�F�b�N
	if( (p_du = GetDataUnit(src_number))==NULL)	return false;

	if(dst_number >= GetDataUnitNumber() || dst_number<0)	return false;

	if(src_number == dst_number)	return false;//�����ԍ��̎w��͂ł��Ȃ�

	if(GetDataUnit(dst_number)){//���ʂ̊i�[��Ƃ��Ďw�肳�ꂽ�摜�������ɁA���łɕʂ̃f�[�^�������Ă���ꍇ�͖₢���킹
		if(show_message){
			CString str;
			str.Format("�摜������%d�͏㏑������܂�",dst_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
				return false;
			}
		}
	}


	
	int pageStart, pageEnd, channelStart, channelEnd, dstPageNum;
	if (toMultiImage)//�����y�[�W�E�`���l���Ή�
	{
		pageStart = 0;
		pageEnd = p_du->PageNumber-1;
		channelStart = 0;
		channelEnd = p_du->ChannelNumber-1;
		dstPageNum = p_du->PageNumber * p_du->ChannelNumber;
	}
	else{
		pageStart = pageEnd = p_du->DispPage;
		channelStart = channelEnd = p_du->DispChannel;
		dstPageNum = 1;
	}



	if (Get3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//�V����BYTE�^���������擾

	CopyDataUnit(src_number, dst_number, false, false, true, false, true,true);//�\���ʒu�E�}�X�N�E�����̈�E�}�[�N���󂯌p��

	p_dst_du = GetDataUnit(dst_number);



	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//�摜�̃T�C�Y�i��f���j
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�


	if (p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || toMultiImage){//�O���[�摜�ɑ΂��鏈��

		int dstPageCnt = 0;
		for (int page = pageStart; page <= pageEnd; page++)
		{
			for (int channel = channelStart; channel <= channelEnd; channel++)
			{
				if (p_du->pByteData){	//8bit�摜
					ret = binalize_global(
						p_du->pByteData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pShortData){	//16bit�摜
					ret = binalize_global(
						p_du->pShortData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pWordData){	//16bit�摜
					ret = binalize_global(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pLongData){	//32bit�摜
					ret = binalize_global(
						p_du->pLongData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pDwordData){	//32bit�摜
					ret = binalize_global(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}
				else if (p_du->pFloatData){//���������_
					ret = binalize_global(
						p_du->pFloatData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

				}

				dstPageCnt++;
			}
		}

		if (toMultiImage){//�����y�[�W�E�`���l���Ή�
			p_dst_du->DispPage = p_du->DispPage;
			p_dst_du->DispChannel = p_du->DispChannel;
		}

	}else{//RGB�摜�ɑ΂��鏈��
		unsigned char*pBrightness = new unsigned char[imsize];
		if(pBrightness==NULL)	return false;

		//�������񖾓x�l�݂̂̉摜�ɂ���
		for(int j=calc_area.top; j<=calc_area.bottom ; j++){
			for(int i=calc_area.left ; i<=calc_area.right ; i++){
				*(pBrightness + i + j*p_du->DataSize.cx) = p_du->GetDataValue(CPoint(i,j),-1,-1);
			}
		}

		ret = binalize_global(
				pBrightness,
				p_dst_du->pByteData,
				p_du->DataSize, min_thresh, max_thresh, inverse, fg_val, bg_val, calc_area);

		delete[]	pBrightness;
	}


	return ret;
}

/********************************************************************
�@  �\  ��  �� : �摜��2�l�����ă}�X�N�ɓ����
��    ��    �� : Binalize
��          �� : int			src_number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 double			 min_thresh			(in)�ŏ�臒l
				 double			 max_thresh			(in)�ő�臒l
				 bool			inverse				(in)�Q�l���]
��    ��    �l : ���������true
�@          �\ : 臒l�ȓ��̉�f�̃}�X�N��On/����ȊO��Off�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::Binalize(long src_number, double min_thresh, double max_thresh, bool inverse)
{
	CDataUnit	*p_du=NULL;
	CRect		calc_area;

	if( (p_du = GetDataUnit(src_number))==NULL)	return false;

	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�
	
	p_du->ClearMask();//�}�X�N����U�N���A

	for(int j=calc_area.top; j<=calc_area.bottom ; j++)
	{
		for(int i=calc_area.left ; i<=calc_area.right ; i++)
		{
			CPoint pt(i,j);

			if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(pt) )//��Ɨ̈�ɓ����Ă��邩�ǂ�������
			{
				double	val;
				bool	onoff;

				//2�l��
				val= p_du->GetDataValue(pt,-1,-1);
				if(val < min_thresh || max_thresh < val){	onoff=false;	}
				else{	onoff=true;	}

				if(inverse)	onoff = !onoff;

				if(onoff)	p_du->SetMaskValue(i,j,UNMASKED_PIXEL);
				else		p_du->SetMaskValue(i,j,MASKED_PIXEL);
			}
		}
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : ��Â̎�@�łQ�l��臒l�����߂�
��    ��    �� : BinalizeThresholdOtsu
��          �� : int			src_number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : 臒l��Ԃ��@���s������0�ɂȂ�
�@          �\ : �O���[�o���Q�l��臒l�̌���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
double	CPimpomAPI::BinalizeThresholdOtsu(long src_number)
{
	CDataUnit	*p_du=NULL;
	CRect		calc_area;
	float		min_val, max_val;
	int			histgram[256] = {0};//�q�X�g�O����
	double		sum = 0;
	int			pixcount=0;
	int			max_hist_id;
	double		max_hist = 0.0;
	double		average, average1, average2;



	if( (p_du = GetDataUnit(src_number))==NULL)	return false;
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�


	//�q�X�g�O��������
	p_du->GetDataMinMax(&min_val, &max_val);//�f�[�^�̍ő�E�ŏ�
	pixcount=0;
	for(int j=calc_area.top; j<=calc_area.bottom ; j++)
	{
		for(int i=calc_area.left ; i<=calc_area.right ; i++)
		{
			CPoint pt(i,j);
			if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(pt) )//��Ɨ̈�ɓ����Ă��邩�ǂ�������
			{
				double val = p_du->GetDataValue(pt,-1,-1);

				//256�i�K��˽ĸ��э쐬
				double idx = (double)((val - min_val)/(max_val-min_val)*255.0);
				histgram[(int)idx]++;	
				sum += val;
				pixcount++;
			}
		}
	}
	if(pixcount==0)	return 0;

	average = sum / pixcount;
	
	for(int i=0; i<256; i++)
	{
		int count1 = 0, count2 = 0;
		long data = 0;
		double breakup1 = 0.0, breakup2 = 0.0;
		double tmp = 0;
		double sigma1, sigma2;
		
		//�N���X1
		data = 0;
		for(int j=0;j<i;j++){
			count1 += histgram[j];
			data += histgram[j] * j;
		}
		if(count1 != 0){
			average1 = (double)data / (double)count1;/*���� =(�f�[�^�̑��a / ��)*/

			for(int j=0;j<i;j++){
				breakup1 += pow( (j- average1), 2 ) * histgram[j];/*���U=(�f�[�^ - ���ϒl)^2�̑��a/�� */
			}
			breakup1 /= (double)count1; 
		}
		
		//�N���X2
		data = 0;
		for(int j=i;j<256;j++){
			count2 += histgram[j];
			data += histgram[j] * j;
		}
		if(count2 != 0){
			average2 = (double)data / (double)count2;/*���� =(�f�[�^�̑��a / ��)*/

			for(int j=i;j<256;j++){
				breakup2 += pow( (j - average2), 2 ) * histgram[j];/*���U=(�f�[�^ - ���ϒl)^2�̑��a/�� */
			}
			breakup2 /= (double)count2;
		}
		
		
		sigma1 = (count1 * breakup1 + count2 * breakup2);/*�N���X�����U*/
		sigma2 = count1 * pow( (average1 - average), 2 ) + count2 * pow( (average2 - average), 2 );/*�N���X�ԕ��U*/
		
		tmp = sigma2 / sigma1;
		if(max_hist < tmp){
			max_hist = tmp;
			max_hist_id = i;
		}
	}
	
	return (double)max_hist_id / 255.0*(max_val-min_val) + min_val;
}


/********************************************************************
�@  �\  ��  �� : P�^�C���łQ�l��臒l�����߂�
��    ��    �� : BinalizeThresholdPTile
��          �� : int			src_number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			param				(in)0�`100 %
��    ��    �l : 臒l��Ԃ����s������0�ɂȂ�
�@          �\ : �O���[�o���Q�l��臒l�̌���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
double	CPimpomAPI::BinalizeThresholdPTile(long src_number, int param)
{
	CDataUnit	*p_du=NULL;
	CRect		calc_area;
	float		min_val, max_val;
	int			histgram[256] = {0};//�q�X�g�O����
	double		sum = 0;
	int			pixcount=0;
	double		max_hist = 0.0;




	if( (p_du = GetDataUnit(src_number))==NULL)	return false;
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�


	//�q�X�g�O��������
	p_du->GetDataMinMax(&min_val, &max_val);//�f�[�^�̍ő�E�ŏ�
	pixcount=0;
	for(int j=calc_area.top; j<=calc_area.bottom ; j++)
	{
		for(int i=calc_area.left ; i<=calc_area.right ; i++)
		{
			CPoint pt(i,j);
			if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(pt) )//��Ɨ̈�ɓ����Ă��邩�ǂ�������
			{
				double val = p_du->GetDataValue(pt,-1,-1);

				//256�i�K��˽ĸ��э쐬
				double idx = (double)((val - min_val)/(max_val-min_val)*255.0);
				histgram[(int)idx]++;	
				sum += val;
				pixcount++;
			}
		}
	}
	if(pixcount==0)	return 0;


	int thresh=0;
	int cnt=0;
	for(int i=0; i<256; i++)
	{
		cnt += histgram[i];

		if( (double)cnt/(double)pixcount >= (double)param/100){
			thresh = i;
			break;
		}
	}
	
	return thresh;
}


/********************************************************************
�@  �\  ��  �� : �摜���Ǐ�2�l�����Ăׂ̉摜�������ɓ����
��    ��    �� : BinalizeLocalGauss
��          �� : int			src_number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_number			(in)2�l����̉摜�����郁����
				 int			filter_size,		(in)�K�E�V�A���t�B���^�T�C�Y(�t�B���^�T�C�Y�̔��l��3��)
				 int			 offset				(in)�I�t�Z�b�g
				 bool			inverse				(in)�Q�l���](true)
				 BYTE			fg_val				(in)�O�i�l
				 BYTE			bg_val				(in)�w�i�l 
				 int			pad_type			(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
				 bool			show_message		(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
��    ��    �l : ���������true
�@          �\ : �K�E�V�A���t�B���^���������摜�ƁA���摜�̍����̕�����2�l��
			�@�@�@�V����BYTE�^���������擾���A臒l�ȓ��̉�f��fg_val/����ȊO��bg_val�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::BinalizeLocalGauss(long src_number, long dst_number, int filter_size, int offset, bool inverse, BYTE fg_val, BYTE bg_val, int pad_type, bool show_message, bool toMultiImage)
{
	bool ret=true;
	CDataUnit	*p_du=NULL, *p_dst_du=NULL;
	CRect		calc_area;

	//�����`�F�b�N
	if( (p_du = GetDataUnit(src_number))==NULL)	return false;

	if(dst_number >= GetDataUnitNumber() || dst_number<0)	return false;

	if(src_number == dst_number)	return false;//�����ԍ��̎w��͂ł��Ȃ�

	if(GetDataUnit(dst_number)){//���ʂ̊i�[��Ƃ��Ďw�肳�ꂽ�摜�������ɁA���łɕʂ̃f�[�^�������Ă���ꍇ�͖₢���킹
		if(show_message){
			CString str;
			str.Format("�摜������%d�͏㏑������܂�",dst_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
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



	if (Get3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//�V����BYTE�^���������擾

	CopyDataUnit(src_number, dst_number, false, false, true, false, true,true);//�\���ʒu�E�}�X�N�E�����̈�E�}�[�N���󂯌p��
	
	p_dst_du = GetDataUnit(dst_number);


	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//�摜�̃T�C�Y�i��f���j
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�


	

	if (p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || toMultiImage){//�O���[�摜�ɑ΂��鏈��

		int dstPageCnt = 0;
		for (int page = pageStart; page <= pageEnd; page++)
		{
			for (int channel = channelStart; channel <= channelEnd; channel++)
			{
				if (p_du->pByteData){	//8bit�摜
					ret = binalize_gauss(
						p_du->pByteData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pShortData){	//16bit�摜
					ret = binalize_gauss(
						p_du->pShortData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pWordData){	//16bit�摜
					ret = binalize_gauss(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pLongData){	//32bit�摜
					ret = binalize_gauss(
						p_du->pLongData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pDwordData){	//32bit�摜
					ret = binalize_gauss(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}
				else if (p_du->pFloatData){//���������_
					ret = binalize_gauss(
						p_du->pFloatData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
						p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}


				dstPageCnt++;
			}
		}

		if (toMultiImage){//�����y�[�W�E�`���l���Ή�
			p_dst_du->DispPage = p_du->DispPage;
			p_dst_du->DispChannel = p_du->DispChannel;
		}


	}else{//RGB�摜�ɑ΂��鏈��
		unsigned char*pBrightness = new unsigned char[imsize];
		if(pBrightness==NULL)	return false;

		//�������񖾓x�l�݂̂̉摜�ɂ���
		for(int j=calc_area.top; j<=calc_area.bottom ; j++){
			for(int i=calc_area.left ; i<=calc_area.right ; i++){
				*(pBrightness + i + j*p_du->DataSize.cx) = p_du->GetDataValue(CPoint(i,j),-1,-1);
			}
		}

		ret = binalize_gauss(
				pBrightness,
				p_dst_du->pByteData,
				p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

		delete[]	pBrightness;
	}


	return ret;
}


/********************************************************************
�@  �\  ��  �� : �摜���Ǐ�2�l�����Ăׂ̉摜�������ɓ����
��    ��    �� : BinalizeLocalLaplaceGauss
��          �� : int			src_number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int			dst_number			(in)2�l����̉摜�����郁����
				 int			filter_size,		(in)�K�E�V�A���t�B���^�T�C�Y(�t�B���^�T�C�Y�̔��l��3��)
				 int			 offset				(in)�I�t�Z�b�g
				 bool			inverse				(in)�Q�l���](true)
				 BYTE			fg_val				(in)�O�i�l
				 BYTE			bg_val				(in)�w�i�l
				 int			pad_type			(in)padding�̃^�C�v  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��
				 bool			show_message		(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
��    ��    �l : ���������true
�@          �\ : �K�E�V�A���t�B���^���������摜�ƁA���摜�̍����̕�����2�l��
			�@�@�@�V����BYTE�^���������擾���A臒l�ȓ��̉�f��fg_val/����ȊO��bg_val�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::BinalizeLocalLaplaceGauss(long src_number, long dst_number, int filter_size, int offset, bool inverse, BYTE fg_val, BYTE bg_val, int pad_type, bool show_message, bool toMultiImage)
{
	bool ret=true;
	CDataUnit	*p_du=NULL, *p_dst_du=NULL;
	CRect		calc_area;

	//�����`�F�b�N
	if( (p_du = GetDataUnit(src_number))==NULL)	return false;

	if(dst_number >= GetDataUnitNumber() || dst_number<0)	return false;

	if(src_number == dst_number)	return false;//�����ԍ��̎w��͂ł��Ȃ�

	if(GetDataUnit(dst_number)){//���ʂ̊i�[��Ƃ��Ďw�肳�ꂽ�摜�������ɁA���łɕʂ̃f�[�^�������Ă���ꍇ�͖₢���킹
		if(show_message){
			CString str;
			str.Format("�摜������%d�͏㏑������܂�",dst_number);
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
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


	if (Get3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//�V����BYTE�^���������擾
	
	CopyDataUnit(src_number, dst_number, false, false, true, false, true,true);//�\���ʒu�E�}�X�N�E�����̈�E�}�[�N���󂯌p��

	p_dst_du = GetDataUnit(dst_number);


	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//�摜�̃T�C�Y�i��f���j
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�


	

	if(p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT)){//�O���[�摜�ɑ΂��鏈��

		int dstPageCnt = 0;
		for (int page = pageStart; page <= pageEnd; page++)
		{
			for (int channel = channelStart; channel <= channelEnd; channel++)
			{
				if(p_du->pByteData){	//8bit�摜
					ret = binalize_laplace_gauss(
						p_du->pByteData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pShortData){	//16bit�摜
					ret = binalize_laplace_gauss(
						p_du->pShortData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pWordData){	//16bit�摜
					ret =binalize_laplace_gauss(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pLongData){	//32bit�摜
					ret = binalize_laplace_gauss(
						p_du->pLongData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pDwordData){	//32bit�摜
					ret = binalize_laplace_gauss(
						p_du->pDwordData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}else if(p_du->pFloatData){//���������_
					ret =binalize_laplace_gauss(
						p_du->pFloatData + imsize * (page * p_du->ChannelNumber + channel),
						p_dst_du->pByteData + dstPageCnt*imsize,
							p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

				}

				dstPageCnt++;
			}
		}


		if (toMultiImage){//�����y�[�W�E�`���l���Ή�
			p_dst_du->DispPage = p_du->DispPage;
			p_dst_du->DispChannel = p_du->DispChannel;
		}

	}else{//RGB�摜�ɑ΂��鏈��
		unsigned char*pBrightness = new unsigned char[imsize];
		if(pBrightness==NULL)	return false;

		//�������񖾓x�l�݂̂̉摜�ɂ���
		for(int j=calc_area.top; j<=calc_area.bottom ; j++){
			for(int i=calc_area.left ; i<=calc_area.right ; i++){
				*(pBrightness + i + j*p_du->DataSize.cx) = p_du->GetDataValue(CPoint(i,j),-1,-1);
			}
		}

		ret = binalize_laplace_gauss(
				pBrightness,
				p_dst_du->pByteData,
				p_du->DataSize, filter_size, offset, inverse, fg_val, bg_val, calc_area, pad_type);

		delete[]	pBrightness;
	}


	return ret;
}

/********************************************************************
�@  �\  ��  �� : 2�l���摜�����x�����O���Ăׂ̉摜�������ɓ����
��    ��    �� : Label
��          �� : long			src_number			(in)2�l����̉摜�����郁����(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long			dst_number			(in)���x���摜(���x���ԍ�)�����郁����
				 double			 fg_val				(in)�O�i�l
				 double			 bg_val				(in)�w�i�l
				 bool			sort_label			(in)���x���ԍ������x���ʐςŃ\�[�g���邩 
				 int			min_area			(in)���x���ŏ��ʐ�
				 int			area[],				(out)���x���ʐϔz��
				 int			gx[],				(out)���x���d�Sx�̔z��
				 int			gy[],				(out)���x���d�Sy�̔z��
				 bool			show_message		(in)�ϊ��O�ɖ₢���킹�_�C�A���O��\�����邩
��    ��    �l : ���x�����@���������s�����ꍇ�� -1
�@          �\ : �Q�l���摜�ɑ΂��ă��x�����O����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CPimpomAPI::Label(
				long	src_number, 
				long	dst_number, 
				bool	sort_label, 
				int		min_area,  
				int		*area,		
				float	*gx,
				float	*gy,
				BYTE	fg_val,
				BYTE	bg_val,
				bool	show_message, 
				bool	toMultiImage)
{
	CDataUnit	*p_du=NULL, *p_dst_du=NULL;
	CRect		calc_area;
	int			labelCnt=0;


	if( (p_du = GetDataUnit(src_number))==NULL)	return -1;
	if (p_du->DataType != BYTE_FORMAT && p_du->DataType != THREE_D_FORMAT)	return -1;//8bit�摜�łȂ��Ə������Ȃ�
	if(src_number == dst_number)	return -1;//�����ԍ��̎w��͂ł��Ȃ�




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

	if (GetF3DMemory(dst_number, p_du->DataSize, dstPageNum, true) == NULL)	return false;//�V����F3D�^���������擾

	CopyDataUnit(src_number, dst_number, false, false, true, false, true,true);//�\���ʒu�E�}�X�N�E�����̈�E�}�[�N���󂯌p��

	p_dst_du = GetDataUnit(dst_number);
	
	
	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//�摜�̃T�C�Y�i��f���j


	cv::Mat src;
	ConvertToCvImage2(src_number, &src);

	int dstPageCnt = 0;
	for (int page = pageStart; page <= pageEnd; page++)
	{
		for (int channel = channelStart; channel <= channelEnd; channel++)
		{

			//���׃����O����
			cv::Mat LabelImg;
			cv::Mat stats;
			cv::Mat centroids;
			int nLab = cv::connectedComponentsWithStats(src, LabelImg, stats, centroids);

			labelCnt = 0;
			for (int i = 1; i < nLab && labelCnt < BINALIZE_LABEL_MAX ; i++)
			{
				int *param = stats.ptr<int>(i);
				int labelarea = param[cv::ConnectedComponentsTypes::CC_STAT_AREA];
				if (labelarea >= min_area)
				{
						if (!toMultiImage)
						{//���x�����
							if (gx != NULL && gy != NULL)
							{
								gx[labelCnt] = centroids.at<double>(i * 2);
								gy[labelCnt] = centroids.at<double>(i * 2 + 1);
							}

							if (area != NULL) {
								area[labelCnt] = labelarea;
							}
						}
						labelCnt++;
				}
			}

			//���ʊi�[
			for (int i = 0; i < imsize; i++) {
				*(p_dst_du->pFloatData + imsize*dstPageCnt + i) = ((int*)LabelImg.data)[i];
			}


			dstPageCnt++;
		}
	}

	if (toMultiImage){//�����y�[�W�E�`���l���Ή�
		p_dst_du->DispPage = p_du->DispPage;
		p_dst_du->DispChannel = p_du->DispChannel;
	}

	return labelCnt;
}