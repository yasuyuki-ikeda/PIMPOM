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
//CDataUnit
//�摜�f�[�^�𓝍��I�Ɉ����N���X
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

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


CMark::CMark()
{
}


CMark::CMark(float x, float y)
{
	type = MARK_TYPE_POINT;
	figure.point.x = x;
	figure.point.y = y;

	memset(attrib, 0, sizeof(attrib));
}

CMark::CMark(float x1, float y1, float x2, float y2)
{
	type = MARK_TYPE_LINE;
	figure.line.x1 = x1;
	figure.line.y1 = y1;
	figure.line.x2 = x2;
	figure.line.y2 = y2;

	memset(attrib, 0, sizeof(attrib));
}

CMark::~CMark()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�f�[�^���\���Z�x���v�Z
template <class DATA>
void	calc_bright( CDataUnit *p_du , DATA *p_src_data)
{
	int				i, j;
	DATA			*p_data;
	BYTE			*p_mask, *p_disp_buf,*p_disp_buf_rgb;
	double			bri_tilt, bri_offset, temp_bri, temp_data;
	CSize			data_size;

	data_size = p_du->DataSize;//�摜�f�[�^�̃T�C�Y

	if (p_du->BrightMax - p_du->BrightMin > 0)
	{	
		bri_tilt = 256.0 / (p_du->BrightMax - p_du->BrightMin);//�f�[�^�@���@�\���Z�x�@�̕ϊ�����
		bri_offset = p_du->BrightMin;//�\���Œ�Z�x
	}
	else{
		bri_tilt = 0;
		bri_offset = 0;
	}
	



	if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT ) || p_du->DispRGB == false  )//�O���[�\��
	{
		p_data = p_src_data + data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel ) ;//�f�[�^��\���|�C���^�̏�����
		p_mask = p_du->pMaskData;//�}�X�N��\���|�C���^��������

		for( j = 0   ;   j < data_size.cy   ;   j ++)
		{
			p_disp_buf = p_du->pDispBuffer + (data_size.cy - 1 - j) * p_du->DispBufferRow;//�\���摜�o�b�t�@��\���|�C���^�̐ݒ�

			for( i = 0   ;   i < data_size.cx   ;   i++ )
			{
				if( p_du->DoUseMask  &&  (*p_mask == MASKED_PIXEL) )//�}�X�N��������ꍇ�̏���
				{
					*p_disp_buf = 0;//�}�X�N���������Ă����f�͍��ɂ���
				}
				else
				{//�}�X�N�������Ȃ��ꍇ�̏���
					temp_data = (double)(*p_data);

					//�摜�f�[�^����\���摜�̔Z�x���v�Z����	
					temp_bri = bri_tilt * ( temp_data  -  bri_offset );
					if( temp_bri < 0.0)				temp_bri = 0.0;
					if( temp_bri > 255.0 )			temp_bri = 255.0;

					*p_disp_buf = (BYTE)temp_bri;
				}

			//�e�f�[�^�̃|�C���^�����̉�f�ɂ��炷
			p_data++;
			p_disp_buf++;
			p_mask++;
			}
		}
	}
	else//RGB�\��
	{
		p_data = p_src_data + data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber );//�f�[�^��\���|�C���^�̏�����
		p_mask = p_du->pMaskData;//�}�X�N��\���|�C���^��������

		for( j = 0   ;   j < data_size.cy   ;   j ++)
		{
			p_disp_buf_rgb = p_du->pDispBufferRGB + (data_size.cy - 1 - j) * p_du->DispBufferRowRGB;//�\���摜�o�b�t�@��\���|�C���^�̐ݒ�

			for( i = 0   ;   i < data_size.cx   ;   i++ )
			{
				if( p_du->DoUseMask  &&  (*p_mask == MASKED_PIXEL) )//�}�X�N��������ꍇ�̏���
				{
					//�}�X�N���������Ă����f�͍��ɂ���
					*p_disp_buf_rgb = 0;//r
					*(p_disp_buf_rgb+1) = 0;//g
					*(p_disp_buf_rgb+2) = 0;//b
				}
				else//�}�X�N�������Ȃ��ꍇ�̏���
				{
					//r
					temp_data = (double)(*p_data);	
					temp_bri = bri_tilt * ( temp_data  -  bri_offset );
					if( temp_bri < 0.0)				temp_bri = 0.0;
					if( temp_bri > 255.0 )			temp_bri = 255.0;

					*(p_disp_buf_rgb + 2) = (BYTE)temp_bri;

					//g
					temp_data = (double)*(p_data + data_size.cx * data_size.cy);
					temp_bri = bri_tilt * ( temp_data  -  bri_offset );
					if( temp_bri < 0.0)				temp_bri = 0.0;
					if( temp_bri > 255.0 )			temp_bri = 255.0;

					*(p_disp_buf_rgb + 1) = (BYTE)temp_bri;
			
					//b
					temp_data = (double)*(p_data + data_size.cx * data_size.cy*2);
					temp_bri = bri_tilt * ( temp_data  -  bri_offset );
					if( temp_bri < 0.0)				temp_bri = 0.0;
					if( temp_bri > 255.0 )			temp_bri = 255.0;

					*(p_disp_buf_rgb + 0) = (BYTE)temp_bri;
				}

			//�e�f�[�^�̃|�C���^�����̉�f�ɂ��炷
			p_data++;
			p_disp_buf_rgb+=3;
			p_mask++;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ɨ̈�̒��̕��ςƕW���΍����v�Z���C�q�X�g�O����������
template <class DATA>
void	calc_hist_and_stat( CDataUnit *p_du , 
							DATA *p_src_data , 
							CRect work_area, 
							int rgb_gain[]
			)
{
	int				i, j, pix_counter, data_addr_offset, disp_buf_addr_offset , data_addr_page_offset;
	DATA			*p_data;
	BYTE			*p_mask, *p_disp_buf, *p_disp_buf_rgb;
	double			sum, sum_2, temp_data;
	int				intensity;
	CSize			data_size;


	data_size = p_du->DataSize;//�摜�f�[�^�̃T�C�Y
	

	//�e�ϐ��̏�����
	sum = 0;
	sum_2 = 0;
	ZeroMemory( p_du->BrightLevel, sizeof(long)*256 );
	pix_counter = 0;
	
	if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB == false  )//�O���[�\��
	{
		data_addr_page_offset = data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel );//�\���y�[�W�ԍ��ɂ��A�h���X�̃I�t�Z�b�g�l

		for( j= work_area.top   ;   j<=work_area.bottom   ;   j++)
		{
			//��f�̃A�h���X�̌v�Z
			data_addr_offset = j * data_size.cx  +  work_area.left;//��f�ɂ��f�[�^�̃A�h���X�̃I�t�Z�b�g�l�̌���
			disp_buf_addr_offset = (data_size.cy - 1 - j) * p_du->DispBufferRow + work_area.left;//��f�ɂ��\���摜�o�b�t�@�̃I�t�Z�b�g�l�̌���

			p_data = p_src_data + data_addr_offset + data_addr_page_offset;//�f�[�^
			p_disp_buf = p_du->pDispBuffer + disp_buf_addr_offset;//�\���摜�o�b�t�@
			p_mask = p_du->pMaskData + data_addr_offset;//�}�X�N

			for( i=work_area.left   ;   i<=work_area.right   ;   i++ )
			{
				if( !p_du->DoUseMask  ||  (*p_mask == UNMASKED_PIXEL) )//�}�X�N�̏���
				{
					if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//��Ɨ̈���̏���
					{
						temp_data = (double)(*p_data);

						//�\���摜�̔Z�x���z�̓x�����z�����
						p_du->BrightLevel[ *p_disp_buf ]++;

						//���ςƕW���΍��𓾂邽�߂̌v�Z�i�a�j������
						sum += temp_data;
						sum_2 += temp_data * temp_data;
						
						//�v�Z�Ɏg����f�̐��𐔂���
						pix_counter++;
					}
				}
			p_data++;
			p_disp_buf++;
			p_mask++;
			}
		}
	}
	else//RGB�\��
	{
		data_addr_page_offset = data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber);//�\���y�[�W�ԍ��ɂ��A�h���X�̃I�t�Z�b�g�l

		for( j= work_area.top   ;   j<=work_area.bottom   ;   j++)
		{
			//��f�̃A�h���X�̌v�Z
			data_addr_offset = j * data_size.cx  +  work_area.left;//��f�ɂ��f�[�^�̃A�h���X�̃I�t�Z�b�g�l�̌���
			disp_buf_addr_offset = (data_size.cy - 1 - j) * p_du->DispBufferRowRGB + work_area.left*3;//��f�ɂ��\���摜�o�b�t�@�̃I�t�Z�b�g�l�̌���

			p_data = p_src_data + data_addr_offset + data_addr_page_offset;//�f�[�^
			p_disp_buf_rgb = p_du->pDispBufferRGB + disp_buf_addr_offset;//�\���摜�o�b�t�@
			p_mask = p_du->pMaskData + data_addr_offset;//�}�X�N

			for( i=work_area.left   ;   i<=work_area.right   ;   i++ )
			{
				if( !p_du->DoUseMask  ||  (*p_mask == UNMASKED_PIXEL) )//�}�X�N�������Ȃ��ꍇ�̏���
				{
					intensity = ( ((int)*p_data ) * rgb_gain[0] 
							    + ((int)*(p_data + data_size.cx * data_size.cy )) * rgb_gain[1]
							    + ((int)*(p_data + data_size.cx * data_size.cy*2 )) * rgb_gain[2]  )/1000;
					temp_data = (double)intensity;

					//�\���摜�̔Z�x���z�̓x�����z�����
					intensity = ( ((int)*(p_disp_buf_rgb))    * rgb_gain[0] 
								+ ((int)*(p_disp_buf_rgb +1)) * rgb_gain[1]
								+ ((int)*(p_disp_buf_rgb +2)) * rgb_gain[2]  )/1000;
					p_du->BrightLevel[ intensity ]++;

					//���ςƕW���΍��𓾂邽�߂̌v�Z�i�a�j������
					sum += temp_data;
					sum_2 += temp_data * temp_data;
					
					//�v�Z�Ɏg����f�̐��𐔂���
					pix_counter++;
				}
			p_data++;
			p_disp_buf_rgb+=3;
			p_mask++;
			}
		}
	}

	p_du->Average = (float)(sum / (double)pix_counter);//����
	p_du->Stdiv = (float)sqrt( fabs( p_du->Average * p_du->Average
										- sum_2 / (double)pix_counter ) );//�W���΍�
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ɨ̈�̒��̍ő�l���v�Z����
template <class DATA>
void	calc_min_max( CDataUnit *p_du , 
					 DATA *p_src_data , 
					 CRect work_area, 
					 float *p_min, 
					 float *p_max, 
					 int rgb_gain[])
{
	int				i, j, data_addr_offset , data_addr_page_offset;
	DATA			*p_data;
	BYTE			*p_mask;
	double			temp_max, temp_min;
	int				intensity;
	CSize			data_size;


	data_size = p_du->DataSize;//�摜�f�[�^�̃T�C�Y
	
	if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB == false  )//�O���[�\��
	{
		data_addr_page_offset = data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel );//�\���y�[�W�ԍ��ɂ��A�h���X�̃I�t�Z�b�g�l

		//�ϐ��̏�����
		temp_max = (double)*(p_src_data + data_addr_page_offset);
		temp_min = (double)*(p_src_data + data_addr_page_offset);

		for( j= work_area.top   ;   j<=work_area.bottom   ;   j++)
		{
			//��f�̃A�h���X�̌v�Z
			data_addr_offset = j * data_size.cx  +  work_area.left;//��f�ɂ��f�[�^�̃A�h���X�̃I�t�Z�b�g�l�̌���

			p_data = p_src_data + data_addr_offset + data_addr_page_offset;//�f�[�^
			p_mask = p_du->pMaskData + data_addr_offset;//�}�X�N

			for( i=work_area.left   ;   i<=work_area.right   ;   i++ )
			{
					if( !p_du->DoUseMask  ||  (*p_mask == UNMASKED_PIXEL) )//�}�X�N�������Ȃ��ꍇ
					{
						if( temp_min > (double)(*p_data) )		temp_min = (double)(*p_data);//�ŏ��l
						if( temp_max < (double)(*p_data) )		temp_max = (double)(*p_data);//�ő�l
					}
				p_data++;
				p_mask++;
			}
		}
	}
	else//RGB�\��
	{
		data_addr_page_offset = data_size.cx * data_size.cy * (p_du->DispPage * p_du->ChannelNumber );//�\���y�[�W�ԍ��ɂ��A�h���X�̃I�t�Z�b�g�l

		//�ϐ��̏�����
		intensity = ( ((int)*(p_src_data + data_addr_page_offset)) * rgb_gain[0]
					 +((int)*(p_src_data + data_addr_page_offset  + data_size.cx * data_size.cy)) * rgb_gain[1]
					 +((int)*(p_src_data + data_addr_page_offset  + data_size.cx * data_size.cy*2)) * rgb_gain[2] )/1000;
		temp_max = (double)intensity;
		temp_min = (double)intensity;

		for( j= work_area.top   ;   j<=work_area.bottom   ;   j++)
		{
			//��f�̃A�h���X�̌v�Z
			data_addr_offset = j * data_size.cx  +  work_area.left;//��f�ɂ��f�[�^�̃A�h���X�̃I�t�Z�b�g�l�̌���

			p_data = p_src_data + data_addr_offset + data_addr_page_offset;//�f�[�^
			p_mask = p_du->pMaskData + data_addr_offset;//�}�X�N

			for( i=work_area.left   ;   i<=work_area.right   ;   i++ )
			{
					if( !p_du->DoUseMask  ||  (*p_mask == UNMASKED_PIXEL) )//�}�X�N�������Ȃ��ꍇ
					{
						intensity = ( ((int)*p_data) * rgb_gain[0]
									 +((int)*(p_data  + data_size.cx * data_size.cy)) * rgb_gain[1]
									 +((int)*(p_data  + data_size.cx * data_size.cy*2)) * rgb_gain[2] )/1000;

						if( temp_min > (double)intensity )		temp_min = (double)intensity;//�ŏ��l
						if( temp_max < (double)intensity )		temp_max = (double)intensity;//�ő�l
					}
				p_data++;
				p_mask++;
			}
		}
	}

	*p_min = (float)temp_min;
	*p_max = (float)temp_max;
}


//�T�u�s�N�Z���l���v�Z
template <class DATA>
double	calc_sub_pix_internal( CDataUnit *p_du , 
								DATA *p_src_data, 
								double x, 
								double y
								)
{
	double	w0, w1;
	double	v1,v2,v3,v4;

	w0 = x - (int)x;
	w1 = y - (int)y;

	if(w0==0.0 && w1==0.0)
	{
		v1 = (double)*(p_src_data);
		v2 = 0;	v3=0;	v4=0;
	}
	else
	{
		v1 = (double)*(p_src_data                        ) * (1.0 - w0)*(1.0 - w1);
		v2 = (double)*(p_src_data                     + 1) * (      w0)*(1.0 - w1);
		v3 = (double)*(p_src_data + p_du->DataSize.cx    ) * (1.0 - w0)*(      w1);
		v4 = (double)*(p_src_data + p_du->DataSize.cx + 1) * (      w0)*(      w1);
	}

	return v1+v2+v3+v4;
}


//�T�u�s�N�Z���l���v�Z
template <class DATA>
double	calc_sub_pix( CDataUnit *p_du , 
					  DATA		*p_src_data, 
					  double x, 
					  double y, 
					  long page, 
					  int channel, 
					  int rgb_gain[]
						)
{
	int		addr;
	int imsize = p_du->DataSize.cx * p_du->DataSize.cy;//�摜�̃T�C�Y�i��f���j


	if(page<0)	page=p_du->DispPage;
	if(channel<0)
	{
		if( (p_du->DataType==RGB_FORMAT || p_du->DataType==RGB_3D_FORMAT) && p_du->DispRGB)//RGB�\���̏ꍇ�͖��x
		{
			addr = (int)x + ((int)y)*p_du->DataSize.cx + imsize * (page * p_du->ChannelNumber );
			return  (  calc_sub_pix_internal(p_du, p_src_data + addr, x, y) * rgb_gain[0]
					 + calc_sub_pix_internal(p_du, p_src_data + addr + imsize, x, y) * rgb_gain[1]
					 + calc_sub_pix_internal(p_du, p_src_data + addr + imsize*2, x, y)  * rgb_gain[2] ) /1000;
		}

		channel=p_du->DispChannel;
	}

	addr = (int)x + ((int)y)*p_du->DataSize.cx + imsize * (page * p_du->ChannelNumber + channel);
	return calc_sub_pix_internal(p_du, p_src_data + addr, x, y);

}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�W���̃R���X�g���N�^
CDataUnit::CDataUnit()
{

}


/********************************************************************
�@  �\  ��  �� : �R���X�g���N�^
��    ��    �� : CDataUnit
��          �� : int			type		(in)�摜�������̃^�C�v
				 CSize			size		(in)�摜�T�C�Y
				 long			page		(in)�摜�̉��s��
��    ��    �l : 
�@          �\ : CDataUnit�N���X�̍\�z
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CDataUnit::CDataUnit(long type, CSize size, long page)
{
	Format(type,size,page);
}

/********************************************************************
�@  �\  ��  �� : �����f�[�^������
��    ��    �� : Format
��          �� : int			type		(in)�摜�������̃^�C�v
				 CSize			size		(in)�摜�T�C�Y
				 long			page		(in)�摜�̉��s��
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::Format(long type, CSize size, long page)
{
	//�����`�F�b�N
	if(size.cx<=0 || size.cy<=0)	return false;
	if(page<=0)		return false;


	//�f�[�^�Ɋւ���ϐ�������������
	complex_image_power_log = false;
	DataType = type;
	DataSize = size;
	DataName.Empty();
	SrcFilePath.Empty();
	DispPage = 0;
	DispChannel = 0;
	DispRGB = true;
	pByteData	= NULL;
	pFloatData	= NULL;
	pShortData	= NULL;
	pWordData	= NULL;
	pLongData	= NULL;
	pDwordData	= NULL;
	Average = 0.0;
	Stdiv = 0.0;
	ZeroMemory( BrightLevel, sizeof(long)*256 );//�\���Z�x�̓x�����z��S��0�ŏ���������
	MarkNum = 0;
	memset(pMarkMap, 0, sizeof(pMarkMap));


	//��Ɨ̈�Ɋւ���ϐ�������������
	WorkArea.Initialize(size);//��Ɨ̈���摜�̃T�C�Y�ɂ��킷
	DoUseWorkArea = false;//��Ɨ̈���g��Ȃ���Ԃŏ���������

	//�摜�f�[�^������������
		switch( DataType )
		{
			case BYTE_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pByteData = new BYTE[ DataSize.cx * DataSize.cy ];
				if(!pByteData)	return false;
				memset(pByteData, 0, DataSize.cx * DataSize.cy * sizeof(BYTE));
				DataName = "BYTE";
				break;

			case FLOAT_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pFloatData = new float[ DataSize.cx * DataSize.cy];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * sizeof(float));
				DataName = "FLOAT";
				break;

			case COMPLEX_FORMAT:
				PageNumber = 1;
				ChannelNumber = 4;
				pFloatData = new float[ DataSize.cx * DataSize.cy * ChannelNumber ];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * ChannelNumber * sizeof(float));
				DataName = "COMPLEX";
				break;

			case THREE_D_FORMAT:
				PageNumber = page;
				ChannelNumber = 1;
				pByteData = new BYTE[ DataSize.cx * DataSize.cy * PageNumber * ChannelNumber ];
				if(!pByteData)	return false;
				memset(pByteData, 0, DataSize.cx * DataSize.cy * PageNumber * ChannelNumber * sizeof(BYTE));
				DataName = "3D";
				break;

			case XY_FORMAT:
				PageNumber = 1;
				ChannelNumber = 2;
				pFloatData = new float[ DataSize.cx * DataSize.cy * ChannelNumber ];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * ChannelNumber * sizeof(float));
				DataName = "PHASE XY";
				break;

			case XYZ_FORMAT:
				PageNumber = 1;
				ChannelNumber = 3;
				pFloatData = new float[ DataSize.cx * DataSize.cy * ChannelNumber ];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * ChannelNumber * sizeof(float));
				DataName = "3D SHAPE";
				break;

			case FLOAT_3D_FORMAT:
				PageNumber = page;
				ChannelNumber = 1;
				pFloatData = new float[ DataSize.cx * DataSize.cy * PageNumber ];
				if(!pFloatData)	return false;
				memset(pFloatData, 0, DataSize.cx * DataSize.cy * PageNumber * sizeof(float));
				DataName = "FLOAT 3D";
				break;

			case RGB_FORMAT:
				PageNumber = 1;
				ChannelNumber = 3;
				pByteData = new BYTE[ DataSize.cx * DataSize.cy * ChannelNumber ];
				if(!pByteData)	return false;
				memset(pByteData, 0, DataSize.cx * DataSize.cy * ChannelNumber * sizeof(BYTE));
				DataName = "RGB COLOR";
				break;

			case SHORT16_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pShortData = new short[ DataSize.cx * DataSize.cy ];
				if(!pShortData)	return false;
				memset(pShortData, 0, DataSize.cx * DataSize.cy * sizeof(short));
				DataName = "SHORT";
				break;

			case WORD_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pWordData = new WORD[ DataSize.cx * DataSize.cy ];
				if(!pWordData)	return false;
				memset(pWordData, 0, DataSize.cx * DataSize.cy * sizeof(WORD));
				DataName = "WORD";
				break;

			case LONG32_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pLongData = new long[ DataSize.cx * DataSize.cy ];
				if(!pLongData)	return false;
				memset(pLongData, 0, DataSize.cx * DataSize.cy * sizeof(long));
				DataName = "LONG";
				break;

			case DWORD_FORMAT:
				PageNumber = 1;
				ChannelNumber = 1;
				pDwordData = new DWORD[ DataSize.cx * DataSize.cy ];
				if(!pDwordData)	return false;
				memset(pDwordData, 0, DataSize.cx * DataSize.cy * sizeof(DWORD));
				DataName = "DWORD";
				break;

			case RGB_3D_FORMAT:
				PageNumber = page;
				ChannelNumber = 3;
				pByteData = new BYTE[ DataSize.cx * DataSize.cy * PageNumber * ChannelNumber ];
				if(!pByteData)	return false;
				memset(pByteData, 0, DataSize.cx * DataSize.cy * PageNumber * ChannelNumber * sizeof(BYTE));
				DataName = "RGB 3D";
				break;

			default:
				return false;
		}

	///�\���摜�o�b�t�@������������
	if( DataSize.cx % 4 == 0)		DispBufferRow = DataSize.cx;
	else							DispBufferRow = DataSize.cx + (4 - DataSize.cx % 4);

	pDispBuffer = new BYTE[ DispBufferRow * DataSize.cy ];//�\���摜�o�b�t�@�̃������擾����
	if(!pDispBuffer){
		Destroy();
		return false;
	}
	memset( pDispBuffer , 0, DispBufferRow * DataSize.cy  );//�\���摜�̔Z�x���[���N���A����

	BrightMax = 256.0;//�\������ő�Z�x��256�ŏ���������
	BrightMin = 0.0;//�\������Œ�Z�x��0�ŏ���������
	DispScale = 1.0;//�X�P�[����1�ŏ���������
	DataDispOffset = CPoint(0,0);//�I�t�Z�b�g�i�\�������`�̈�̍���̍��W�j���摜�̌��_�ŏ���������
	set_bmp_info(size);//�r�b�g�}�b�v����ݒ肷��
	ChangePalette( PALETTE_GRAYSCALE );//�O���[�X�P�[���ŃJ���[�p���b�g������������

	if(DataType==RGB_FORMAT || DataType==RGB_3D_FORMAT)
	{
		///RGB�p�摜�o�b�t�@������������ 2006.05.19
		if( (DataSize.cx*3) % 4 == 0)		DispBufferRowRGB = DataSize.cx*3;
		else								DispBufferRowRGB = DataSize.cx*3 + (4 - (DataSize.cx*3) % 4);

		pDispBufferRGB = new BYTE[ DispBufferRowRGB * DataSize.cy ];//�\���摜�o�b�t�@�̃������擾����
		if(!pDispBufferRGB){
			Destroy();
			return false;
		}
		ZeroMemory( pDispBufferRGB , DispBufferRowRGB * DataSize.cy  );//�\���摜�̔Z�x���[���N���A����
	}
	else
	{
		pDispBufferRGB = NULL;
		DispBufferRowRGB = 0;
	}

	///�}�X�N�f�[�^������������
	pMaskData = new BYTE[ DataSize.cx * DataSize.cy ];//�}�X�N�f�[�^�̃������擾����
	if(!pMaskData){
		Destroy();
		return false;
	}

	ClearMask();//�}�X�N�f�[�^���[���N���A�i�S�Ẳ�f���}�X�N���������Ă��Ȃ���Ԃŏ���������j
	DoUseMask = true;


	//�}�[�N������������
	//pMark = new CMark[MARK_NUMBER_MAX];//�}�[�N�̃������擾����
	//if(!pMark){
	//	Destroy();
	//	return false;
	//}

	//for(int i=0 ; i<MARK_NUMBER_MAX ; i++){
	//	pMark[i].x = pMark[i].y = 0;
	//	memset(pMark[i].attrib , 0, MARK_ATTRIB_NUM*sizeof(double));
	//}
	/*MarkNum = 0;*/


	return true;
}


/********************************************************************
�@  �\  ��  �� : BITMAPINFOHEADER�̏������i8�r�b�g�r�b�g�}�b�v�ŏ���������j
��    ��    �� : CDataUnit
��          �� : CSize			size		(in)�摜�T�C�Y
��    ��    �l : 
�@          �\ : pDispBufferBmpInfo��8�r�b�g�r�b�g�}�b�v�ŏ���������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::set_bmp_info(CSize size)
{
	pDispBufferBmpInfo = (BITMAPINFO*)new char[ sizeof(BITMAPINFOHEADER) + 1024 ];//BITMAPINFOHEADER + �J���[�p���b�g�̃��������擾����
	pDispBufferBmpInfo->bmiHeader.biSize = 40;//�w�b�_�̃T�C�Y
	pDispBufferBmpInfo->bmiHeader.biWidth = size.cx;//�摜�f�[�^�̕�
	pDispBufferBmpInfo->bmiHeader.biHeight = size.cy;//�摜�f�[�^�̍���
	pDispBufferBmpInfo->bmiHeader.biPlanes = 1;
	pDispBufferBmpInfo->bmiHeader.biBitCount = 8;//�\���摜�o�b�t�@��1��f������̃T�C�Y
	pDispBufferBmpInfo->bmiHeader.biCompression = 0;//�\���摜�o�b�t�@�̈��k�`��
	pDispBufferBmpInfo->bmiHeader.biSizeImage   = 0;
	pDispBufferBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	pDispBufferBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	pDispBufferBmpInfo->bmiHeader.biClrUsed = 0;
	pDispBufferBmpInfo->bmiHeader.biClrImportant = 0;

	pDispBufferBmpInfoRGB = (BITMAPINFO*)new char[ sizeof(BITMAPINFOHEADER) ];//BITMAPINFOHEADER�̃��������擾����
	pDispBufferBmpInfoRGB->bmiHeader.biSize = sizeof(BITMAPINFOHEADER) ;//�w�b�_�̃T�C�Y
	pDispBufferBmpInfoRGB->bmiHeader.biWidth = size.cx;//�摜�f�[�^�̕�
	pDispBufferBmpInfoRGB->bmiHeader.biHeight = size.cy;//�摜�f�[�^�̍���
	pDispBufferBmpInfoRGB->bmiHeader.biPlanes = 1;
	pDispBufferBmpInfoRGB->bmiHeader.biBitCount = 24;//�\���摜�o�b�t�@��1��f������̃T�C�Y
	pDispBufferBmpInfoRGB->bmiHeader.biCompression = 0;//�\���摜�o�b�t�@�̈��k�`��
	pDispBufferBmpInfoRGB->bmiHeader.biSizeImage   = 0;
	pDispBufferBmpInfoRGB->bmiHeader.biXPelsPerMeter = 0;
	pDispBufferBmpInfoRGB->bmiHeader.biYPelsPerMeter = 0;
	pDispBufferBmpInfoRGB->bmiHeader.biClrUsed = 0;
	pDispBufferBmpInfoRGB->bmiHeader.biClrImportant = 0;
}

/********************************************************************
�@  �\  ��  �� : �f�X�g���N�^
��    ��    �� : ~CDataUnit
��          �� : 
��    ��    �l : 
�@          �\ : CDataUnit�̔j��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CDataUnit::~CDataUnit()
{
	Destroy();
}

/********************************************************************
�@  �\  ��  �� : �����f�[�^���
��    ��    �� : Destroy
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::Destroy()
{
	if(pByteData)			delete[]	pByteData;
	if(pFloatData)			delete[]	pFloatData;
	if(pShortData)			delete[]	pShortData;
	if(pWordData)			delete[]	pWordData;
	if(pLongData)			delete[]	pLongData;
	if(pDwordData)			delete[]	pDwordData;
	if(pMaskData)			delete[]	pMaskData;
	if(pDispBuffer)			delete[]	pDispBuffer;
	if(pDispBufferBmpInfo)	delete[]	pDispBufferBmpInfo;
	if(pDispBufferRGB)			delete[]	pDispBufferRGB;
	if(pDispBufferBmpInfoRGB)	delete[]	pDispBufferBmpInfoRGB;
	
	for(int i=0 ; i<MARK_NUMBER_MAX ; i++){
		if(pMarkMap[i]){
			delete pMarkMap[i];
			pMarkMap[i]=NULL;
		}
	}


	complex_image_power_log = false;
	DataType = -1;
	DataSize = CSize(0,0);
	DataName.Empty();
	SrcFilePath.Empty();
	DispPage = 0;
	DispChannel = 0;
	DispRGB = true;
	pByteData	= NULL;
	pFloatData	= NULL;
	pShortData	= NULL;
	pWordData	= NULL;
	pLongData	= NULL;
	pDwordData	= NULL;
	Average = 0.0;
	Stdiv = 0.0;
	MarkNum = 0;
	ZeroMemory( BrightLevel, sizeof(long)*256 );//�\���Z�x�̓x�����z��S��0�ŏ���������
	BrightMax = 256.0;
	BrightMin = 0.0;
	DispScale = 1.0;
	DataDispOffset = CPoint(0,0);

}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�@���@�\���摜�@�ɕϊ�
��    ��    �� : UpdateDispBuffer
��          �� : 
��    ��    �l : 
�@          �\ : ���x�ϊ��A�}�X�N����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CDataUnit::UpdateDispBuffer()
{
	CRect			calc_area = GetRectArea();//�����Ώۂ̋�`�͈�	

	//�\���Z�x�ϊ�
	if( pByteData )
	{				
		calc_bright( this , pByteData);
		calc_hist_and_stat( this , pByteData,  calc_area, API.rgb_gain);
	}
	else if( pFloatData )
	{		
		calc_bright( this , pFloatData );
		calc_hist_and_stat( this , pFloatData,  calc_area, API.rgb_gain);
	}
	else if( pShortData )
	{		
		calc_bright( this , pShortData );
		calc_hist_and_stat( this , pShortData,  calc_area, API.rgb_gain);
	}
	else if( pWordData )
	{		
		calc_bright( this , pWordData );
		calc_hist_and_stat( this , pWordData,  calc_area, API.rgb_gain);
	}
	else if( pLongData )
	{		
		calc_bright( this , pLongData );
		calc_hist_and_stat( this , pLongData,  calc_area, API.rgb_gain);
	}
	else if( pDwordData )
	{		
		calc_bright( this , pDwordData );
		calc_hist_and_stat( this , pDwordData,  calc_area, API.rgb_gain);
	}

	//TRACE("UPDATE!\n");
}

/********************************************************************
�@  �\  ��  �� : �\���Z�x�����W��ύX����
��    ��    �� : SetDispRange
��          �� : float			min			(in)�ŏ��Z�x
				 float			max			(in)�ő�Z�x
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::SetDispRange(float min, float max)
{
	if(max < min)	return	false;//�\�������W�Ƃ��Ďw�肵��2�l�̑召�֌W���t�̂Ƃ��͂Ȃɂ����Ȃ�

	BrightMax = max;
	BrightMin = min;

	return	true;
}

/********************************************************************
�@  �\  ��  �� : �\���̊g�嗦��ύX����
��    ��    �� : SetDispScale
��          �� : float			scale		(in)�g�嗦
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::SetDispScale(float scale)
{
	DispScale = scale;
	DataDispOffset = CPoint(0,0);
}

/********************************************************************
�@  �\  ��  �� : �\���y�[�W�ԍ��i����̉��s���ԍ��j��ύX����
��    ��    �� : MoveDispPage
��          �� : int			offset		(in) �\���y�[�W�ύX��
��    ��    �l : ����������true
�@          �\ : 3D,F3D,RGB3D�^�C�v�ɂ����āA�\���摜�̉��s���ԍ���ύX����
				 ���ݕ\�����̔ԍ�����̑��Έʒu�Ŏw�肷��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::MoveDispPage(int offset)
{
	int	dest_page = DispPage + offset;
	if( dest_page < 0	||	dest_page >= PageNumber)	return  false;

	DispPage = dest_page;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �\���y�[�W�ԍ��i����̉��s���ԍ��j��ύX����
��    ��    �� : SetDispPage
��          �� : int			page		(in)�\���y�[�W�ԍ�
��    ��    �l : ����������true
�@          �\ : 3D,F3D,RGB3D�^�C�v�ɂ����āA�\���摜�̉��s���ԍ���ύX����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::SetDispPage(int page)
{
	if( page < 0	||	page >= PageNumber)	return false;

	DispPage = page;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �\���`���l����ύX����
��    ��    �� : SetDispPage
��          �� : int	channel		(in)�\��channel�ԍ� RGB�\���̏ꍇ��-1
��    ��    �l : ����������true
�@          �\ : RGB,RGB3D�^�C�v�ɂ����āA�\���摜�̃`���l����ύX����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::SetDispChannel(int channel)
{

	if(channel==-1){
		DispRGB=true;
	}

	if( channel < 0	||	channel >= ChannelNumber)	return false;

	DispRGB = false;
	DispChannel = channel;

	return	true;
}


/********************************************************************
�@  �\  ��  �� : �\���摜�̃J���[�p���b�g��ύX����
��    ��    �� : ChangePalette
��          �� : short			palette_id		(in)�J���[�p���b�g��ID					
��    ��    �l : ����������true
�@          �\ : RGB,RGB3D�ȊO�̉摜�f�[�^�\�����́A�[���J���[�\�����@��ύX����
				 �ݒ�ł����ނ͉��L
					PALETTE_GRAYSCALE�F		�O���[�X�P�[��
					PALETTE_NEGATIVE�F		�O���[�X�P�[�����]
					PALETTE_RAINBOW�F		�a���f���q�Ɋ��炩�ɕω�
					PALETTE_BLACK_RAINBOW�F	�a���f���q�Ɋ��炩�ɕω��B0�����͍�
					PALETTE_COSINE�F		�����g��̔Z�x�ω�
					PALETTE_BLUE_YELLOW�F	�a���x�Ɋ��炩�ɕω�
					PALETTE_GREEN_RED�F		�f���q�Ɋ��炩�ɕω�
					PALETTE_LABEL:			�Z�x�P���Ƃɕ\���F���傫���ω�����(���x���p)
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::ChangePalette(short palette_id)
{
	int i;

	switch( palette_id )
	{
		case	PALETTE_GRAYSCALE:
			for(i=0 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbRed = i;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = i;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = i;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_NEGATIVE:
			for(i=0 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbRed = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_BLACK_RAINBOW:
			//no break
		case	PALETTE_RAINBOW:
			for(i=0 ; i<64 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 255;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = i*4;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}

			for(i=64 ; i<128 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 255 - (i-64)*4;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}

			for(i=128 ; i<192 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = (i-128)*4;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}

			for(i=192 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255 - (i-192)*4;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = 255;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}

			if( palette_id == PALETTE_BLACK_RAINBOW )//0�̂Ƃ��������ɂ���
			{
				pDispBufferBmpInfo->bmiColors[0].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[0].rgbGreen = 0;
				pDispBufferBmpInfo->bmiColors[0].rgbRed = 0;
			}
			break;

		case	PALETTE_COSINE:
			for(i=0 ; i<256 ; i++)
			{
				double temp_kido = 127.5 * cos (i*2.0*PI/255.0) + 127.5;
				pDispBufferBmpInfo->bmiColors[i].rgbRed = (unsigned char)temp_kido;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = (unsigned char)temp_kido;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = (unsigned char)temp_kido;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_BLUE_YELLOW:
			for(i=0 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbRed = i;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = i;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_GREEN_RED:
			for(i=0 ; i<256 ; i++)
			{
				pDispBufferBmpInfo->bmiColors[i].rgbRed = i;
				pDispBufferBmpInfo->bmiColors[i].rgbGreen = 255-i;
				pDispBufferBmpInfo->bmiColors[i].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
			}
			break;

		case	PALETTE_LABEL:
			{
				BYTE	r[]={0,		255,	255,	255,	0,		0};
				BYTE	g[]={255,	0,		255,	0,		255,	0};
				BYTE	b[]={255,	255,	0,		0,		0,		255};

				for(i=1 ; i<256 ; i++)
				{
					pDispBufferBmpInfo->bmiColors[i].rgbRed = r[i%6];
					pDispBufferBmpInfo->bmiColors[i].rgbGreen = g[i%6];
					pDispBufferBmpInfo->bmiColors[i].rgbBlue = b[i%6];
					pDispBufferBmpInfo->bmiColors[i].rgbReserved = 0;
				}
				//0�̂Ƃ��������ɂ���
				pDispBufferBmpInfo->bmiColors[0].rgbBlue = 0;
				pDispBufferBmpInfo->bmiColors[0].rgbGreen = 0;
				pDispBufferBmpInfo->bmiColors[0].rgbRed = 0;
			}
			break;

		default:
			break;
	}
}

/********************************************************************
�@  �\  ��  �� : �}�X�N���N���A
��    ��    �� : ClearMask
��          �� : 
��    ��    �l : 
�@          �\ : �}�X�N�̒l�����ׂ�UNMASKED_PIXEL�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::ClearMask()
{
	ZeroMemory( pMaskData, DataSize.cx * DataSize.cy  );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�f�[�^�쐬
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : ���f���摜�̎����E��������p���[�X�y�N�g���E�ʑ����z������
��    ��    �� : CreateComplexPolarComponents
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::CreateComplexPolarComponents()
{
	if( DataType!=COMPLEX_FORMAT )		return;
	if( !pFloatData )	return;

	float	*real_data , *imag_data , *power_data , *phase_data ;

	real_data = pFloatData;
	imag_data = pFloatData + DataSize.cx * DataSize.cy;
	power_data = pFloatData + DataSize.cx * DataSize.cy * 2;
	phase_data = pFloatData + DataSize.cx * DataSize.cy * 3;

	for(int i=0 ; i<DataSize.cx * DataSize.cy ; i++)
	{
		if( complex_image_power_log )//�p���[�X�y�N�g���őΐ����Ƃ�
		{
			*power_data = (float)log( sqrt((*real_data) * (*real_data) + (*imag_data) * (*imag_data))  );
		}
		else
		{
			*power_data = (float) sqrt((*real_data) * (*real_data) + (*imag_data) * (*imag_data))  ;
		}

		*phase_data = (float)atan2( *imag_data , *real_data);

		real_data ++ ;
		imag_data ++ ;
		power_data ++ ;
		phase_data ++ ;
	}
}

/********************************************************************
�@  �\  ��  �� : ���f���摜�̃p���[�X�y�N�g���E�ʑ����z��������E���������
��    ��    �� : CreateComplexDescartesComponents
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::CreateComplexDescartesComponents()
{
	if( DataType != COMPLEX_FORMAT )		return;
	if( !pFloatData )						return;	

	float	*real_data , *imag_data , *power_data , *phase_data ;
	int		i;

	real_data = pFloatData;
	imag_data = pFloatData + DataSize.cx * DataSize.cy;
	power_data = pFloatData + DataSize.cx * DataSize.cy * 2;
	phase_data = pFloatData + DataSize.cx * DataSize.cy * 3;


		for(i=0 ; i<DataSize.cx * DataSize.cy ; i++)
		{
			if( complex_image_power_log )//�p���[�̑ΐ����Ƃ�
			{
				float	temp = (float) exp( *power_data );	
				*real_data = temp * cos( *phase_data );
				*imag_data = temp * sin( *phase_data );
			}
			else
			{
				*real_data = ( *power_data ) * cos( *phase_data );
				*imag_data = ( *power_data ) * sin( *phase_data );
			}		

			power_data++;
			phase_data++;
			real_data++;
			imag_data++;
		}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�f�[�^�擾
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : ��`�̈���擾����
��    ��    �� : GetRectArea
��          �� : 
��    ��    �l : ��Ɨ̈���g���ꍇ�́A�O�ڂ����`��Ԃ��B
				 �g��Ȃ��ꍇ�͉摜�S�̃T�C�Y�̋�`��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CRect CDataUnit::GetRectArea()
{
	if( DoUseWorkArea )//��Ɨ̈���g���ꍇ
	{
		CRect rect = WorkArea.GetCircumRect();

		if(rect.left<0)	rect.left=0;
		if(rect.right>=DataSize.cx)	rect.right = DataSize.cx-1;
		if(rect.top<0)	rect.top=0;
		if(rect.bottom>=DataSize.cy)	rect.bottom = DataSize.cy-1;

		return rect;
	}
	else//��Ɨ̈���g��Ȃ��ꍇ
	{
		return  CRect(0, 0, DataSize.cx-1, DataSize.cy-1);
	}
}

/********************************************************************
�@  �\  ��  �� : �摜�������̃^�C�v�E�T�C�Y�E���s�������`�F�b�N����
��    ��    �� : Compare
��          �� : long		type		(in)�摜�������̃^�C�v
				 CSize		size		(in)�摜�������̃T�C�Y
				 long		page		(in)�摜�������̉��s����
��    ��    �l : �����Ŏw�肳�ꂽ���̂Ɠ����Ȃ�true��Ԃ��@�قȂ��false��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::Compare(long type, CSize size, long page)
{
	if( DataType == type   &&   DataSize == size   &&   page == page )		return	true;
	else																	return	false;
}

/********************************************************************
�@  �\  ��  �� : �摜�������̃^�C�v�E�T�C�Y�E���s�������r����
��    ��    �� : Compare
��          �� : CDataUnit		*p_du		(in)��r�Ώۂ�CDataUnit�ւ̃|�C���^
��    ��    �l : �����Ŏw�肳�ꂽ���̂Ɠ����Ȃ�true��Ԃ��@�قȂ��false��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::Compare(CDataUnit *p_du)
{
	if( !p_du )		return	false;
	return		Compare(p_du->DataType , p_du->DataSize , p_du->PageNumber);
}

/********************************************************************
�@  �\  ��  �� : �摜�������̍ő�l�E�ŏ��l�����߂�
��    ��    �� : GetDataMinMax
��          �� : float			*p_min		(out)�摜�������̍ŏ��l
				 float			*p_max		(out)�摜�������̍ő�l
��    ��    �l : 
�@          �\ : �`���l���E���s������������摜�̏ꍇ�́A�\�����̉摜�̍ő�E�ŏ������߂�
				 RGB�\���̏ꍇ�́A�O���[�������l�����߂�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::GetDataMinMax(float *p_min, float *p_max)
{
	CRect			calc_area = GetRectArea();//�����Ώۂ̋�`�͈�

	////�v�Z
		if( pByteData )
		{				
			calc_min_max( this , pByteData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pFloatData )
		{		
			calc_min_max( this , pFloatData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pShortData )
		{	
			calc_min_max( this , pShortData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pWordData )
		{	
			calc_min_max( this , pWordData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pLongData )
		{	
			calc_min_max( this , pLongData,  calc_area, p_min, p_max, API.rgb_gain);
		}
		else if( pDwordData )
		{	
			calc_min_max( this , pDwordData,  calc_area, p_min, p_max, API.rgb_gain);
		}
}

/********************************************************************
�@  �\  ��  �� : �w�肳�ꂽ���W�̃f�[�^�l�����߂�
��    ��    �� : GetDataValue
��          �� : CPoint			point		(in)���W
				 int			page		(in)���s���i-1�̏ꍇ�͌��ݕ\�����̂��́j
				 int			channel		(in)�`���l���i-1�̏ꍇ�͌��ݕ\�����̂��́j
��    ��    �l : 
�@          �\ : �͂ݏo�����ꍇ��0��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
double CDataUnit::GetDataValue(CPoint point, long page, int channel)
{
#if 1 //�������x�Z�k��
	if(point.x<0  ||  point.x>=DataSize.cx)		return	0;
	if(point.y<0  ||  point.y>=DataSize.cy)		return	0;


	if(page<0)	page=DispPage;
	if(channel<0)
	{
		if( (DataType==RGB_FORMAT || DataType==RGB_3D_FORMAT) && DispRGB)//RGB�\���̏ꍇ�͖��x
		{
			return GetDataValue(point.x, point.y, page, channel);
		}

		channel=DispChannel;
	}
	
	long	addr_page_offset = DataSize.cx * DataSize.cy * (page * ChannelNumber + channel);

	if( pByteData )				
		return	(double)*(pByteData + point.x + point.y * DataSize.cx + addr_page_offset);

	else if( pFloatData )		
		return	(double)*(pFloatData + point.x + point.y * DataSize.cx + addr_page_offset);	

	else if( pShortData )		
		return	(double)*(pShortData + point.x + point.y * DataSize.cx + addr_page_offset);

	else if( pWordData )		
		return	(double)*(pWordData + point.x + point.y * DataSize.cx + addr_page_offset);	

	else if( pLongData )		
		return	(double)*(pLongData + point.x + point.y * DataSize.cx + addr_page_offset);	

	else if( pDwordData )		
		return	(double)*(pDwordData + point.x + point.y * DataSize.cx + addr_page_offset);	


	return 0;
#else
	return GetDataValue(point.x, point.y, rgb_type,page,channel);
#endif
}

/********************************************************************
�@  �\  ��  �� : �w�肳�ꂽ���W�̃f�[�^�l�����߂�i�T�u�s�N�Z���Ŏw��j
��    ��    �� : GetDataValue
��          �� : double			x			(in)���Wx
				 double			y			(in)���Wy
				 int			page		(in)���s���i-1�̏ꍇ�͌��ݕ\�����̂��́j
				 int			channel		(in)�`���l���i-1�̏ꍇ�͌��ݕ\�����̂��́j
��    ��    �l : �w�肳�ꂽ���W�̃f�[�^�l�i�T�u�s�N�Z���j �͂ݏo�����ꍇ��0��Ԃ�
�@          �\ : �T�u�s�N�Z���ł̒l�����߂�
				 �͂ݏo�����ꍇ��0��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
double CDataUnit::GetDataValue(double x, double y, long page, int channel)
{
	if(x<0  ||  x>DataSize.cx-1)		return	0;
	if(y<0  ||  y>DataSize.cy-1)		return	0;
	if ( page>PageNumber - 1)		return 0;
	if ( channel>ChannelNumber - 1)		return 0;


	if( pByteData )	
	{
		return	calc_sub_pix( this , pByteData, x, y, page, channel, API.rgb_gain);
	}
	else if( pFloatData )
	{
		return	calc_sub_pix( this  ,pFloatData, x, y, page, channel, API.rgb_gain);	
	}
	else if( pShortData )
	{
		return	calc_sub_pix( this , pShortData, x, y, page, channel, API.rgb_gain);
	}
	else if( pWordData )
	{
		return	calc_sub_pix( this , pWordData, x, y, page, channel, API.rgb_gain);
	}
	else if( pLongData )
	{
		return	calc_sub_pix( this , pLongData, x, y, page, channel, API.rgb_gain);	
	}
	else if( pDwordData )
	{
		return	calc_sub_pix( this , pDwordData, x, y, page, channel, API.rgb_gain);	
	}

	return 0;
}

/********************************************************************
�@  �\  ��  �� : �w�肳�ꂽ���W�̃}�X�N�l���擾����
��    ��    �� : GetMaskValue
��          �� : CPoint			point		(in)���W
��    ��    �l : �w�肳�ꂽ���W�̃}�X�N�l  0:UNMASKED_PIXEL 1:MASKED_PIXEL
�@          �\ : �͂ݏo�����ꍇ��0��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BYTE CDataUnit::GetMaskValue(CPoint point)
{
		if(point.x<0  ||  point.x>=DataSize.cx)		return	0;
		if(point.y<0  ||  point.y>=DataSize.cy)		return	0;

		return pMaskData[point.x + point.y*DataSize.cx];
}

/********************************************************************
�@  �\  ��  �� : �w�肳�ꂽ���W�̃f�[�^�l��ݒ肷��
��    ��    �� : SetDataValue
��          �� : int			x			(in)���Wx
				 int			y			(in)���Wy
				 double			data		(in)�f�[�^�l
				 int			page		(in)���s���i-1�̏ꍇ�͌��ݕ\�����̂��́j
				 int			channel		(in)�`���l���i-1�̏ꍇ�͌��ݕ\�����̂��� �iRGB�\���̏ꍇ�͂��ׂẴ`���l���ɓ����l���ݒ�j�j
				 bool			satulate	(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
��    ��    �l : ���������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::SetDataValue(int x, int y, double data, long page, int channel, bool satulate)
{
	//�����`�F�b�N
	if(x<0  ||  x>DataSize.cx-1)		return	false;
	if(y<0  ||  y>DataSize.cy-1)		return	false;

	if(page<0)	page=DispPage;

	if(channel<0)
	{
		if( (DataType==RGB_FORMAT || DataType==RGB_3D_FORMAT) && DispRGB)//RGB�\���̏ꍇ�͂��ׂẴ`���l���ɓ����l������
		{
			for(int c=0 ; c<3 ; c++)
			{
				int addr = x + y*DataSize.cx + DataSize.cx*DataSize.cy * (page * ChannelNumber + c)	;
				if(data<0)			*(pByteData + addr) = 0;
				else if(data>255)	*(pByteData + addr) = 255;
				else				*(pByteData + addr) = (BYTE)data;
			}		
		}

		channel=DispChannel;
	}


	int addr = x + y*DataSize.cx + DataSize.cx*DataSize.cy * (page * ChannelNumber + channel)	;

	if( pByteData )	
	{
		if(satulate){
			if(data<0.0)			*(pByteData + addr) = 0;
			else if(data>255.0)	*(pByteData + addr) = 255;
			else				*(pByteData + addr) = (BYTE)data;
		}else{
			*(pByteData + addr) = (BYTE)data;
		}
	}
	else if( pFloatData )
	{
		*(pFloatData + addr) = (float)data;
	}
	else if( pShortData )
	{
		if(satulate){
			if(data<-32768.0)		*(pShortData + addr) = (short)-32768;
			else if(data>32767.0)	*(pShortData + addr) = (short)32767;
			else				*(pShortData + addr) = (short)data;
		}else{
			*(pShortData + addr) = (short)data;
		}
	}
	else if( pWordData )
	{
		if(satulate){
			if(data<0.0)			*(pWordData + addr) = (WORD)0;
			else if(data>65535.0)	*(pWordData + addr) = (WORD)65535;
			else				*(pWordData + addr) = (WORD)data;
		}else{
			*(pWordData + addr) = (WORD)data;
		}
	}
	else if( pLongData )
	{
		if(satulate){
			if(data<-2147483648.0)		*(pLongData + addr) = (long)-2147483648;
			else if(data>2147483647.0)	*(pLongData + addr) = (long)2147483647;
			else						*(pLongData + addr) = (long)data;
		}else{
			*(pLongData + addr) = (long)data;
		}
	}
	else if( pDwordData )
	{
		if(satulate){
			if(data<0.0)					*(pDwordData + addr) = (DWORD)0;
			else if(data>4294967295.0)	*(pDwordData + addr) = (DWORD)4294967295;
			else						*(pDwordData + addr) = (DWORD)data;
		}else{
			*(pDwordData + addr) = (DWORD)data;
		}
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : �w�肳�ꂽ���W�̃f�[�^�l��ݒ肷��
��    ��    �� : SetMaskValue
��          �� : int			x			(in)���Wx
				 int			y			(in)���Wy
				 BYTE			mask		(in)�}�X�N�l 0:UNMASKED_PIXEL 1:MASKED_PIXEL
��    ��    �l : ���������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::SetMaskValue(int x, int y, BYTE mask)
{
		if(x<0  ||  x>DataSize.cx-1)		return	false;
		if(y<0  ||  y>DataSize.cy-1)		return	false;

		pMaskData[x + y*DataSize.cx] = mask;
		return true;
}

/********************************************************************
�@  �\  ��  �� : �w�肳�ꂽ���W�̕\���摜�̔Z�x��Ԃ�
��    ��    �� : GetDisplayIntensity
��          �� : CPoint			point		(in)���W
��    ��    �l : �Z�x�l�@�͂ݏo�����ꍇ��0��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
unsigned char CDataUnit::GetDisplayIntensity(CPoint point)
{
	int		intensity;

	if(point.x<0  ||  point.x>=DataSize.cx)		return	0;
	if(point.y<0  ||  point.y>=DataSize.cy)		return	0;

	if( (DataType != RGB_FORMAT && DataType != RGB_3D_FORMAT)  ||  DispRGB == false )//�O���[�X�P�[��
	{
		return	*(pDispBuffer + point.x  + (DataSize.cy - 1 - point.y) * DispBufferRow);
	}
	else//RGB�J���[
	{
		intensity = ( API.rgb_gain[0] * (int)*(pDispBufferRGB + point.x*3  + (DataSize.cy - 1 - point.y) * DispBufferRowRGB)		//r
				    + API.rgb_gain[1] * (int)*(pDispBufferRGB + point.x*3  + (DataSize.cy - 1 - point.y) * DispBufferRowRGB + 1)	//g
				    + API.rgb_gain[2] * (int)*(pDispBufferRGB + point.x*3  + (DataSize.cy - 1 - point.y) * DispBufferRowRGB + 2)  )/1000;//b
		return (unsigned char)intensity;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�f�[�^�̕ύX
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �}�[�N��ǉ�����
��    ��    �� : AddMark
��          �� : 
��    ��    �l : ����������ǉ����ꂽ�}�[�N�ԍ���Ԃ����s������-1��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CDataUnit::AddMark(CMark mark)
{
	int idx = MarkNum;
	if( idx +1>= MARK_NUMBER_MAX )	return -1;//�}�[�N�̐��`�F�b�N

	if(pMarkMap[idx]==NULL){//�V���Ƀ}�[�N�p�̃��������Ƃ�
		pMarkMap[idx] = new CMark();
		if(pMarkMap[idx]==NULL)	return -1;
	}

	//�}�[�N���R�s�[����
	pMarkMap[idx]->type = mark.type;
	memcpy( &pMarkMap[idx]->figure, &mark.figure, sizeof(FIGURE_UNION));
	for(int i=0 ; i<MARK_ATTRIB_NUM ; i++){
		pMarkMap[idx]->attrib[i] = mark.attrib[i];
	}


	MarkNum++;//�}�[�N�̐�������₷

	return idx;
}

/********************************************************************
�@  �\  ��  �� : �}�[�N����������
��    ��    �� : DeleteMark
��          �� : long				id			(in)�}�[�N�ԍ�
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::DeleteMark(long idx)
{
	if( idx>= MarkNum || idx<0)	return false;//�����`�F�b�N

	if(pMarkMap[idx])
	{
		delete pMarkMap[idx];//idx�Ԗڂ̃��������폜

		for(int i=idx ; i<MARK_NUMBER_MAX-1 ; i++){//�|�C���^�����炷
			pMarkMap[i] = pMarkMap[i+1];
		}

		pMarkMap[MARK_NUMBER_MAX-1]=NULL;//�Ō��NULL�ɂ���

		MarkNum--;//�}�[�N�����P���炷

		return true;
	}else{
		return false;
	}
}

/********************************************************************
�@  �\  ��  �� : ���ׂẴ}�[�N�N���A
��    ��    �� : ClearMark
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::ClearMark()
{
	for(int i=0 ; i<MARK_NUMBER_MAX ; i++){
		if(pMarkMap[i]){
			delete pMarkMap[i];
			pMarkMap[i]=NULL;
		}
		MarkNum=0;
	}
}

/********************************************************************
�@  �\  ��  �� : �}�[�N�̃f�[�^�l���擾����
��    ��    �� : GetMark
��          �� : 
				 long				idx			(in)�}�[�N�ԍ�

��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::GetMark(long idx, CMark *pmark)
{
	if( idx>= MarkNum || idx<0)	return false;//�����`�F�b�N

	if(pMarkMap[idx])
	{
		pmark->type = pMarkMap[idx]->type;
		memcpy( &pmark->figure, &pMarkMap[idx]->figure, sizeof(FIGURE_UNION));
		for(int i=0 ; i<MARK_ATTRIB_NUM ; i++){
			pmark->attrib[i]  = pMarkMap[idx]->attrib[i];
		}
		return true;
	}else{
		return false;
	}
}

/********************************************************************
�@  �\  ��  �� : �}�[�N�̃f�[�^�l��ݒ肷��
��    ��    �� : SetMark
��          �� : 
				 long				idx			(in)�}�[�N�ԍ�

��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::SetMark(long idx, CMark mark)
{
	if( idx>= MarkNum || idx<0)	return false;//�����`�F�b�N

	if(pMarkMap[idx])
	{
		pMarkMap[idx]->type = mark.type;
		memcpy( &pMarkMap[idx]->figure, &mark.figure, sizeof(FIGURE_UNION));
		for(int i=0 ; i<MARK_ATTRIB_NUM ; i++){
			pMarkMap[idx]->attrib[i] = mark.attrib[i];
		}
		return true;
	}
	else
	{
		return false;
	}

}


long CDataUnit::GetMarkNumber()
{
	return MarkNum;
}


/********************************************************************
�@  �\  ��  �� : CDataUnit�̃����o���R�s�[
��    ��    �� : Copy
��          �� : CDataUnit		*p_src_du			(in)�R�s�[���̉摜������
				 bool			do_copy_data		(in)�摜�������̒����R�s�[�i�t�H�[�}�b�g/�T�C�Y/�y�[�W/�`���l���������ꍇ�̂݁j
				 bool			do_copy_mask		(in)�}�X�N���R�s�[�i�T�C�Y�������ꍇ�̂݁j
				 bool			do_copy_workarea	(in)��Ɨ̈���R�s�[
				 bool			do_copy_range		(in)�\���摜�̔Z�x�����W���R�s�[
				 bool			do_copy_mark		(in)�}�[�N���R�s�[���邩�ǂ���
				 bool			do_copy_pos_scale	(in)�\���ʒu�ƃX�P�[�����R�s�[�i�T�C�Y�������ꍇ�̂݁j
��    ��    �l : ���������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//�f�[�^���w�肵�ăR�s�[
bool CDataUnit::Copy(CDataUnit *p_src_du, 
					 bool do_copy_data, 
					 bool do_copy_mask, 
					 bool do_copy_workarea,
					 bool do_copy_range, 
					 bool do_copy_mark, 
					 bool do_copy_pos_scale)
{

	if( !p_src_du )		return	false;//�R�s�[���Ƀf�[�^��������Ή������Ȃ�



	//�摜�f�[�^�̃R�s�[
	if(do_copy_data)
	{
		if( p_src_du->DataType == DataType && 
			p_src_du->DataSize == DataSize && 
			p_src_du->PageNumber == PageNumber && 
			p_src_du->ChannelNumber == ChannelNumber)
		{ 
			//�f�[�^�Ɋւ���ϐ��̃R�s�[
			if (do_copy_data){
				DataName = p_src_du->DataName;
			}

			DispPage = p_src_du->DispPage;
			DispChannel = p_src_du->DispChannel;
			DispRGB  = p_src_du->DispRGB ;
			Average = p_src_du->Average;
			Stdiv = p_src_du->Stdiv;
			memcpy( BrightLevel , p_src_du->BrightLevel , sizeof(long)*256 );
			SrcFilePath = p_src_du->SrcFilePath;


			////�摜�f�[�^�̃R�s�[
			if( pByteData )
			{
				memcpy( pByteData , p_src_du->pByteData , DataSize.cx * DataSize.cy * sizeof(BYTE) *PageNumber * ChannelNumber);
			}
			else if( pFloatData )
			{
				memcpy( pFloatData , p_src_du->pFloatData , DataSize.cx * DataSize.cy * sizeof(float) *PageNumber * ChannelNumber);
			}
			else if( pShortData )
			{
				memcpy( pShortData , p_src_du->pShortData , DataSize.cx * DataSize.cy * sizeof(short) *PageNumber * ChannelNumber);
			}
			else if( pWordData )
			{
				memcpy( pWordData , p_src_du->pWordData , DataSize.cx * DataSize.cy * sizeof(WORD) *PageNumber * ChannelNumber);
			}
			else if( pLongData )
			{
				memcpy( pLongData , p_src_du->pLongData , DataSize.cx * DataSize.cy * sizeof(long) *PageNumber * ChannelNumber);
			}
			else if( pDwordData )
			{
				memcpy( pDwordData , p_src_du->pDwordData , DataSize.cx * DataSize.cy * sizeof(DWORD) *PageNumber * ChannelNumber);
			}

			///	�\���摜�̃R�s�[
			memcpy( pDispBuffer , p_src_du->pDispBuffer , DispBufferRow * DataSize.cy );

			///	RGB�\���摜�̃R�s�[
			if(p_src_du->pDispBufferRGB != NULL)
			{
				memcpy( pDispBufferRGB , p_src_du->pDispBufferRGB , DispBufferRowRGB * DataSize.cy );
			}


			//�[���J���[�p���b�g�̃R�s�[
			memcpy( pDispBufferBmpInfo->bmiColors , 
				p_src_du->pDispBufferBmpInfo->bmiColors , 1024 );
		}
	}

	//�}�X�N�f�[�^�̃R�s�[
	if(do_copy_mask)
	{
		if( p_src_du->DataSize == DataSize )
		{
			memcpy( pMaskData , p_src_du->pMaskData , DataSize.cx * DataSize.cy );
			DoUseMask = p_src_du->DoUseMask;
		}
	}

	//��Ɨ̈�̃R�s�[
	if(do_copy_workarea)
	{
		//if( p_src_du->DataSize == DataSize )
		{
			WorkArea.Copy( &p_src_du->WorkArea );
			DoUseWorkArea = p_src_du->DoUseWorkArea;
		}
	}

	//�\���Z�x�����W�̃R�s�[
	if(do_copy_range)
	{
		BrightMax = p_src_du->BrightMax;
		BrightMin = p_src_du->BrightMin;
	}

	//�}�[�N�̃R�s�[
	if(do_copy_mark)
	{
		ClearMark();
		for(int i=0 ; i<p_src_du->GetMarkNumber() ; i++)
		{
			CMark mark;
			p_src_du->GetMark(i, &mark);
			AddMark(mark);
		}
	}

	//�\���ʒu�ƃX�P�[���̃R�s�[
	if(do_copy_pos_scale)
	{
		if(p_src_du->DataSize == DataSize)
		{
			DispScale = p_src_du->DispScale;
			DataDispOffset = p_src_du->DataDispOffset;
		}
	}

	return	true;
}

//���ׂẴ����o���R�s�[����
bool CDataUnit::Copy(CDataUnit *p_src_du)
{
	return Copy(p_src_du, true, true, true, true, true, true);
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈悩��}�X�N�����
��    ��    �� : SetMaskByWorkArea
��          �� : 
��    ��    �l : ���������true
�@          �\ : ��Ɨ̈�O�ɂ��ׂă}�X�N��������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataUnit::SetMaskByWorkArea()
{
	for(int j=0 ; j<DataSize.cy ; j++)
	{
		for(int i=0 ; i<DataSize.cx ; i++)
		{
			if(WorkArea.PtInWorkArea(CPoint(i,j)))
			{
				*(pMaskData + i + j*DataSize.cx) = UNMASKED_PIXEL;
			}else{
				*(pMaskData + i + j*DataSize.cx) = MASKED_PIXEL;
			}
		}
	}
	return true;
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̏�����
��    ��    �� : InitWorkArea
��          �� : 
��    ��    �l : 
�@          �\ : ��Ɨ̈���摜�S�̂�������`�݂̂ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataUnit::InitWorkArea()
{
	WorkArea.Initialize(DataSize);
}



/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : GetMinLimitValue
��          �� : 
��    ��    �l : �f�[�^�l�̉����l
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
double CDataUnit::GetMinLimitValue()
{
	//�摜�f�[�^������������
		switch( DataType )
		{
			case BYTE_FORMAT:
				return 0;

			case FLOAT_FORMAT:
				return -FLT_MAX;

			case COMPLEX_FORMAT:
				return -FLT_MAX;

			case THREE_D_FORMAT:
				return 0;

			case XY_FORMAT:
				return -FLT_MAX;

			case XYZ_FORMAT:
				return -FLT_MAX;

			case FLOAT_3D_FORMAT:
				return -FLT_MAX;

			case RGB_FORMAT:
				return 0;

			case SHORT16_FORMAT:
				return SHRT_MIN;

			case WORD_FORMAT:
				return 0;

			case LONG32_FORMAT:
				return LONG_MIN;

			case DWORD_FORMAT:
				return 0;

			case RGB_3D_FORMAT:
				return 0;

		}
	return 0;
}



/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : GetMaxLimitValue
��          �� : 
��    ��    �l : �f�[�^�l�̏���l
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
double CDataUnit::GetMaxLimitValue()
{
	//�摜�f�[�^������������
		switch( DataType )
		{
			case BYTE_FORMAT:
				return UCHAR_MAX;

			case FLOAT_FORMAT:
				return FLT_MAX;

			case COMPLEX_FORMAT:
				return FLT_MAX;

			case THREE_D_FORMAT:
				return UCHAR_MAX;

			case XY_FORMAT:
				return FLT_MAX;

			case XYZ_FORMAT:
				return FLT_MAX;

			case FLOAT_3D_FORMAT:
				return FLT_MAX;

			case RGB_FORMAT:
				return UCHAR_MAX;

			case SHORT16_FORMAT:
				return SHRT_MAX;

			case WORD_FORMAT:
				return USHRT_MAX;

			case LONG32_FORMAT:
				return LONG_MAX;

			case DWORD_FORMAT:
				return ULONG_MAX;

			case RGB_3D_FORMAT:
				return UCHAR_MAX;

		}
	return 0;
}



