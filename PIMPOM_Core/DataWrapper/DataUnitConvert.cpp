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
//CDataUnitConvert
//�摜�f�[�^�𓝍��I�Ɉ����N���X 
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include <limits>

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX3(X,Y,Z)		((X)>(Y) ? (X):(Y)) > (Z) ?  ((X)>(Y) ? (X):(Y)) : (Z)
#define MIN3(X,Y,Z)		((X)<(Y) ? (X):(Y)) < (Z) ?  ((X)<(Y) ? (X):(Y)) : (Z)

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
extern	CPimpomAPI	API;

// *************************************
//         ��    ��    �Q    ��         
// *************************************


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�\���摜��DATA�^�ɃR�s�[
template <class DATA>
void	disp_to_data(CDataUnit *p_du, DATA *p_buffer, int color_gray_method)
{
	CSize size = p_du->DataSize;

	if( (p_du->DataType==RGB_FORMAT || p_du->DataType==RGB_3D_FORMAT) && p_du->DispRGB)//RGB�\��
	{
		if (color_gray_method == 1)//�ʓx
		{
			int satulation;
			BYTE	*p_disp_buf_rgb;

			for (int j = 0; j<size.cy; j++)
			{
				for (int i = 0; i<size.cx; i++)
				{
					p_disp_buf_rgb = p_du->pDispBufferRGB + (size.cy - 1 - j) * p_du->DispBufferRowRGB + i * 3;

					int max = MAX3(*(p_disp_buf_rgb), *(p_disp_buf_rgb + 1), *(p_disp_buf_rgb + 2));
					int min = MIN3(*(p_disp_buf_rgb), *(p_disp_buf_rgb + 1), *(p_disp_buf_rgb + 2));

					if (max == 0){
						*(p_buffer + i + j*size.cx) = 0;
					}
					else{
						satulation = (max - min) * 255 * 1024 / max;
						*(p_buffer + i + j*size.cx) = (BYTE)(satulation / 1024);
					}
				}
			}
		}
		else if (color_gray_method == 2)//�F��
		{

		}
		else//���x
		{
			BYTE	*p_disp_buf_rgb;
			int		intensity;
			for (int j = 0; j < size.cy; j++)
			{
				for (int i = 0; i < size.cx; i++)
				{
					p_disp_buf_rgb = p_du->pDispBufferRGB + (size.cy - 1 - j) * p_du->DispBufferRowRGB + i * 3;

					intensity = (int)*(p_disp_buf_rgb)*API.rgb_gain[2]	//b
						+ (int)*(p_disp_buf_rgb + 1) *API.rgb_gain[1]	//g
						+ (int)*(p_disp_buf_rgb + 2) *API.rgb_gain[0];	//r

					*(p_buffer + i + j*size.cx) = (BYTE)( intensity / 1000);
				}
			}
		}
	}
	else
	{
		for(int j=0 ; j<size.cy ; j++)
		{
			for(int i=0 ; i<size.cx ; i++)
			{
				*(p_buffer + i + j*size.cx ) = *(p_du->pDispBuffer + i + (size.cy - j -1)*p_du->DispBufferRow );
			}
		}
	}
}


//�}�X�N�摜��DATA�^�ɃR�s�[
template <class DATA>
void	mask_to_data(CDataUnit *p_du, DATA *p_buffer)
{
	CSize size = p_du->DataSize;

	for (int j = 0; j<size.cy; j++)
	{
		for (int i = 0; i<size.cx; i++)
		{
			*(p_buffer + i + j*size.cx) = *(p_du->pMaskData + i + size.cx*j)*255;
			*(p_du->pMaskData + i + size.cx*j) = UNMASKED_PIXEL;
		}
	}
}


//DATA_SRC�^�f�[�^��DATA_DST�^�f�[�^�ɃR�s�[
template <class DATA_SRC, class DATA_DST>
void	data_to_data(	CDataUnit	*p_src_du,		//(in)�R�s�[����CDataUnit
						DATA_SRC	*p_src_data,	//(in)p_src_du�����̉摜�f�[�^�擪�|�C���^
						DATA_DST	*p_dst_data,	//(out)�R�s�[��摜�f�[�^�擪�|�C���^
						double		dst_data_min,	//(in)p_dst_data�̉����l
						double		dst_data_max,	//(in)p_dst_data�̏���l
						int			color_gray_method	//(in)RGB->�O���[�ϊ����@ (1:�ʓx 2:�F��  �����ȊO:���x)
						)
{
	CSize size = p_src_du->DataSize;
	
	if((p_src_du->DataType==RGB_FORMAT || p_src_du->DataType==RGB_3D_FORMAT) && p_src_du->DispRGB)//RGB�\��
	{
		if (color_gray_method == 1)//�ʓx
		{
			int satulation;
			BYTE *pr = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy;
			BYTE *pg = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy + size.cx*size.cy;
			BYTE *pb = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy + size.cx*size.cy * 2;
			DATA_DST *pdst = p_dst_data;

			for (int i = 0; i < size.cx*size.cy; i++)
			{
				int max = MAX3(*pr, *pg, *pb);
				int min = MIN3(*pr, *pg, *pb);

				if (max == 0){
					*pdst = 0;
				}
				else{
					satulation = (max - min) * 255 * 1024 / max;
					*pdst = (DATA_DST)(satulation / 1024);
				}

				pr++;
				pg++;
				pb++;
				pdst++;
			}
		}
		else if (color_gray_method == 2)//�F��
		{

		}
		else//���x
		{
			int intensity;
			BYTE *pr = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy;
			BYTE *pg = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy + size.cx*size.cy;
			BYTE *pb = p_src_du->pByteData + (p_src_du->DispPage * p_src_du->ChannelNumber) * size.cx * size.cy + size.cx*size.cy * 2;
			DATA_DST *pdst = p_dst_data;

			for (int i = 0; i < size.cx*size.cy; i++)
			{
				intensity = (int)*pr * API.rgb_gain[0]	//r
					+ (int)*pg * API.rgb_gain[1]		//g
					+ (int)*pb * API.rgb_gain[2];		//b
				*pdst = (DATA_DST)(intensity / 1000.0);

				pr++;
				pg++;
				pb++;
				pdst++;
			}
		}

	}
	else
	{
		DATA_SRC *psrc = p_src_data +  (p_src_du->DispPage * p_src_du->ChannelNumber + p_src_du->DispChannel) * size.cx * size.cy;
		DATA_DST *pdst = p_dst_data;
		double tmp;
		

		if(dst_data_min<dst_data_max)//�㉺���ŖO�a������
		{
			for(int i=0 ; i<size.cx*size.cy  ; i++)
			{
				tmp = *psrc;
				if(tmp<dst_data_min)		tmp = dst_data_min;
				if(tmp>dst_data_max)		tmp = dst_data_max;

				*pdst = (DATA_DST)tmp;
		
				psrc++;
				pdst++;
			}
		}else{//���̂܂܂���
			for(int i=0 ; i<size.cx*size.cy  ; i++)
			{
				*pdst = (DATA_DST)*psrc;
		
				psrc++;
				pdst++;
			}
		}
	}
}



//�R���o�[�g���s
template <class DATA>
bool	data_convert(CDataUnit				*pSrcDU,	//(in)�ϊ��O��CDataUnit�ւ̃|�C���^
					 DATA					*pSrcData,	//(in)pSrcDU�����̉摜�f�[�^�擪�|�C���^�i�^�����ׂ��邽�߁j
					 CDataUnit				*pDstDU,	//(out)�ϊ����CDataUnit�ւ̃|�C���^
					 CONVERT_DATA_OPTION	option		//(in)�ϊ��I�v�V����
					 )
{
	CSize		size;
	double		dst_data_min=0, dst_data_max=0;

	if(option.satulate)//�f�[�^�㉺���������Ƃ��O�a�����邩�ǂ���
	{
		dst_data_min = pDstDU->GetMinLimitValue();
		dst_data_max = pDstDU->GetMaxLimitValue();
	}

	size = pSrcDU->DataSize;

	//�f�[�^��ϊ�
		switch( option.type_to )
		{
			case BYTE_FORMAT:
				if(option.byte_from==1)
				{
					disp_to_data(pSrcDU, pDstDU->pByteData, option.color_gray_method);//disp->BYTE
				}
				else if (option.byte_from==2)
				{
					mask_to_data(pSrcDU, pDstDU->pByteData);//mask->BYTE
				}
				else{
					data_to_data(pSrcDU, pSrcData, pDstDU->pByteData, dst_data_min, dst_data_max, option.color_gray_method);//data->BYTE
				}
				break;

			case SHORT16_FORMAT://data->short
				data_to_data(pSrcDU, pSrcData, pDstDU->pShortData, dst_data_min, dst_data_max, option.color_gray_method);
				break;

			case WORD_FORMAT://data->WORD
				data_to_data(pSrcDU, pSrcData, pDstDU->pWordData, dst_data_min, dst_data_max, option.color_gray_method);
				break;

			case LONG32_FORMAT://data->LONG
				data_to_data(pSrcDU, pSrcData, pDstDU->pLongData, dst_data_min, dst_data_max, option.color_gray_method);
				break;

			case DWORD_FORMAT://data->DWORD
				data_to_data(pSrcDU, pSrcData, pDstDU->pDwordData, dst_data_min, dst_data_max, option.color_gray_method);
				break;

			case RGB_FORMAT://data->RGB
				if(pSrcDU->DataType == RGB_FORMAT || pSrcDU->DataType == RGB_3D_FORMAT)//RGB���m�̕ϊ�
				{
					//���̂܂܂̒l���R�s�[
					memcpy(pDstDU->pByteData, pSrcDU->pByteData + pSrcDU->DispPage * pSrcDU->ChannelNumber * size.cx * size.cy , 
														size.cx * size.cy * pSrcDU->ChannelNumber * sizeof(BYTE) );
				}
				else
				{
					if( option.rgb_to == RGB_TO_B )//B�ɓ����
					{
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 2, dst_data_min, dst_data_max, 0);
					}
					else if( option.rgb_to == RGB_TO_G )//G�ɓ����
					{
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 1, dst_data_min, dst_data_max, 0);
					}
					else if( option.rgb_to == RGB_TO_R )//R�ɓ����
					{
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 0, dst_data_min, dst_data_max, 0);
					}
					else//RGB�S�Ăɓ����
					{
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 2, dst_data_min, dst_data_max, 0);
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 1, dst_data_min, dst_data_max, 0);
						data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy * 0, dst_data_min, dst_data_max, 0);
					}
				}
				break;

			case FLOAT_FORMAT://data->FLOAT
				data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData, 0, 0, option.color_gray_method);//�f�[�^���o�b�t�@�ɓ����
				break;

			case XYZ_FORMAT://data->XYZ
				{
					data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData + size.cx * size.cy * 2, 0, 0, option.color_gray_method);//z�ɓ����

					//�f�[�^x,y�f�[�^����
					float		*px, *py ;
					for(int j=0 ; j<size.cy ; j++)
					{
						for(int i=0 ; i<size.cx ; i++)
						{
							px = pDstDU->pFloatData + i + j*size.cx;
							py = px + size.cx * size.cy ;
							//pz = px + size.cx * size.cy * 2;

							*px = (float)(i - size.cx/2);
							*py = (float)(-j + size.cy/2);
							//*pz = (float)*(p_buffer + i + j*size.cx);
						}
					}
				}
				break;

			case COMPLEX_FORMAT://data->COMPLLEX
				if( option.comp_to == COMP_TO_REAL )
				{//�����ɓ����
					data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData, 0, 0, option.color_gray_method);
				}
				else
				{//�����ɓ����
					data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData + size.cx * size.cy, 0, 0, option.color_gray_method);
				}
				pDstDU->CreateComplexPolarComponents();
				break;

			case THREE_D_FORMAT://data->3D
				for (int i = 0, j = option.threeD_offset; i<pSrcDU->PageNumber && j<pDstDU->PageNumber; i++, j++)
				{
					if (j < 0)	continue;

					pSrcDU->DispPage = i;
					data_to_data(pSrcDU, pSrcData, pDstDU->pByteData + size.cx * size.cy  * j, dst_data_min, dst_data_max, option.color_gray_method);
				}
				break;

			case FLOAT_3D_FORMAT://data->F3D
				for (int i = 0, j = option.threeD_offset ; i<pSrcDU->PageNumber && j<pDstDU->PageNumber; i++, j++)
				{
					if (j < 0)	continue;

					pSrcDU->DispPage = i;
					data_to_data(pSrcDU, pSrcData, pDstDU->pFloatData + size.cx * size.cy * j, dst_data_min, dst_data_max, option.color_gray_method);
				}
				break;
		}


	return true;
}



/********************************************************************
�@  �\  ��  �� : �f�[�^�̌^��ϊ�����
��    ��    �� : Convert
��          �� : CONVERT_DATA_OPTION option  (in)
��    ��    �l : ����������true��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::Convert(CONVERT_DATA_OPTION option)
{
	//���f�[�^���m�̕ϊ��̏ꍇ�͏������X�L�b�v
	//if( DataType==option.type_to)
	//{
	//	if( option.type_to != BYTE_FORMAT  ||  option.byte_from_disp==false ){//�\���f�[�^����ϊ�����ꍇ�̂ݗL���ɂ���
	//		return true;
	//	}
	//}


	//
	CDataUnit *pSrcDU = new CDataUnit(DataType, DataSize, PageNumber);
	if(!pSrcDU)	return false;

	if(!pSrcDU->Copy(this)){
		delete pSrcDU;
		return false;
	}


	Destroy();

	int page = pSrcDU->PageNumber;
	if (option.type_to == THREE_D_FORMAT || option.type_to == FLOAT_3D_FORMAT || option.type_to == RGB_3D_FORMAT)
	{
		if (option.threeD_page > 0 && option.threeD_page<pSrcDU->PageNumber)
		{
			page = option.threeD_page;
		}
	}
	if (!Format(option.type_to, pSrcDU->DataSize, page)){
		delete pSrcDU;
		return false;
	}


	
	//�摜�f�[�^�R�s�[

	bool ret = true;
	switch(pSrcDU->DataType)
	{
		case BYTE_FORMAT://BYTE����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pByteData, this, option);
			break;

		case THREE_D_FORMAT://3D����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pByteData, this, option);
			break;

		case RGB_FORMAT://RGB����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pByteData, this, option);
			break;

		case RGB_3D_FORMAT://RGB3D����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pByteData, this, option);
			break;

		case SHORT16_FORMAT://SHORT����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pShortData, this, option);
			break;

		case WORD_FORMAT://WORD����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pWordData, this, option);
			break;

		case LONG32_FORMAT://LONG����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pLongData, this, option);
			break;

		case DWORD_FORMAT://DWORD����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pDwordData, this, option);
			break;

		case FLOAT_FORMAT://FLOAT����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		case COMPLEX_FORMAT://���f������ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		case XYZ_FORMAT://XYZ����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		case XY_FORMAT://XY����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		case FLOAT_3D_FORMAT://F3D����ϊ�
			ret = data_convert(pSrcDU, pSrcDU->pFloatData, this, option);
			break;

		default:
			delete pSrcDU;
			return false;
	}

	if(!ret){
		delete pSrcDU;
		return false;
	}


	//���̑��f�[�^�R�s�[
	
	DataName = pSrcDU->DataName;//�f�[�^��
	SrcFilePath = pSrcDU->SrcFilePath;//�ǂݍ��񂾃t�@�C���p�X
	WorkArea.Copy(&pSrcDU->WorkArea);//��Ɨ̈�
	DataDispOffset = pSrcDU->DataDispOffset;//�\���ʒu
	DispScale = pSrcDU->DispScale;//�\���{��

	for(int i=0 ; i< pSrcDU->MarkNum ; i++){//�}�[�N���W�̕��A
		CMark mark;
		pSrcDU->GetMark(i, &mark);
		AddMark( mark);
	}

	delete pSrcDU;

	return true;
}