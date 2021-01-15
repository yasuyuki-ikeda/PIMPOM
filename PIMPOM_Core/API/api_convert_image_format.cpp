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
//�摜�������̌^�ύX
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

// *************************************
//         ��    ��    �Q    ��         
// *************************************


#if 0 //�����̏����� CDataUnit�̃����o�֐��Ɉڍs
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//�\���摜��DATA�^�̃o�b�t�@�ɃR�s�[
template <class DATA>
void	disp_to_buffer( CDataUnit *p_du, DATA *p_buffer, int rgb_gain[])
{
	CSize size = p_du->DataSize;

	if( (p_du->DataType==RGB_FORMAT || p_du->DataType==RGB_3D_FORMAT) && p_du->DispRGB, int colorGrayType)//RGB�\��
	{
		if (colorGrayType == 0)//���x
		{
			for(int j=0 ; j<size.cy ; j++)
			{
				for(int i=0 ; i<size.cx ; i++)
				{	
					int		intensity;
					BYTE	*p_disp_buf_rgb;

					p_disp_buf_rgb = p_du->pDispBufferRGB + (size.cy - 1 - j) * p_du->DispBufferRowRGB + i*3;
					intensity = ( *(p_disp_buf_rgb)   *rgb_gain[0] 
						+ *(p_disp_buf_rgb+1) *rgb_gain[1] 
						+ *(p_disp_buf_rgb+2) *rgb_gain[2] )/1000;
					*(p_buffer + i + j*size.cx ) = (BYTE)intensity;
				}
			}
		}		
		else if (colorGrayType == 1)
		{//�ʓx
			for(int j=0 ; j<size.cy ; j++)
			{
				for(int i=0 ; i<size.cx ; i++)
				{	
					int satulation;
					BYTE	*p_disp_buf_rgb;
					p_disp_buf_rgb = p_du->pDispBufferRGB + (size.cy - 1 - j) * p_du->DispBufferRowRGB + i*3;

					int max = MAX3( *(p_disp_buf_rgb), *(p_disp_buf_rgb+1) , *(p_disp_buf_rgb+2) );
					int min = MIN3( *(p_disp_buf_rgb), *(p_disp_buf_rgb+1) , *(p_disp_buf_rgb+2) );

					if (max == 0){
						*(p_buffer + i + j*size.cx ) = 0;
					}
					else{
						satulation = (max - min) * 1024 / max;
						*(p_buffer + i + j*size.cx ) = (BYTE)(brightness / 1024);
					}
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

//�f�[�^��DATA�^�̃o�b�t�@�ɃR�s�[
template <class DATA>
void	data_to_buffer( CDataUnit *p_du, DATA *p_data, DATA *p_buffer, int rgb_gain[], int colorGrayType)
{
	CSize size = p_du->DataSize;
	
	if((p_du->DataType==RGB_FORMAT || p_du->DataType==RGB_3D_FORMAT) && p_du->DispRGB)//RGB�\��
	{
	
		if (colorGrayType == 0)//���x
		{
			int brightness;
			for (int i = 0; i < size.cx*size.cy; i++)
			{
				brightness = (int)*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i) * rgb_gain[0]
							+ (int)*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy) * rgb_gain[1]
							+ (int)*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy * 2) * rgb_gain[2];
				*(p_buffer + i) = (BYTE)(brightness / 1000.0);
			}
		}
		else if (colorGrayType == 1)
		{//�ʓx
			int satulation;
			for(int i = 0; i < size.cx*size.cy; i++)
			{
				int max = MAX3(*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i),
					*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy),
					*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy * 2));

				int min = MIN3(*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i),
					*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy),
					*(p_du->pByteData + (p_du->DispPage * p_du->ChannelNumber) * size.cx * size.cy + i + size.cx*size.cy * 2));

				if (max == 0){
					*(p_buffer + i) = 0;
				}
				else{
					satulation = (max - min) * 1024 / max;
					*(p_buffer + i) = (BYTE)(brightness / 1024);
				}

			}

		}
	}
	else
	{
		int memsize = size.cx * size.cy * sizeof(DATA);
		memcpy( p_buffer, p_data + (p_du->DispPage * p_du->ChannelNumber + p_du->DispChannel) * size.cx * size.cy , memsize);//���݂̃f�[�^��ʃ������Ɋm��
	}
}

//�f�[�^�ԃR�s�[
template <class DATA_SRC, class DATA_DST>
void	data_copy(DATA_SRC *p_src, DATA_DST *p_dst, CSize size, CONVERT_DATA_OPTION option)
{
	double	min,max;

	//DATA_DST�^�̍ő�l�E�ŏ��l
	min = (double)(std::numeric_limits<DATA_DST>::min)();
	max = (double)(std::numeric_limits<DATA_DST>::max)();

	for(int i=0 ; i<size.cx*size.cy ; i++)
	{
		if(option.satulate)
		{
			if(min > (double)*p_src){			*p_dst = (DATA_DST)min;		}
			else if( max < (double)*p_src ){	*p_dst = (DATA_DST)max;		}
			else{								*p_dst = (DATA_DST)*p_src; 	}
		}
		else
		{
			*p_dst = (DATA_DST)*p_src; 
		}

		p_src++;
		p_dst++;
	}
}

//�R���o�[�g���s
template <class DATA>
bool	data_convert(CPimpomAPI *pAPI, int target_data_number, DATA *p_src , CONVERT_DATA_OPTION option, int rgb_gain[])
{
	int i,j;
	CDataUnit	*p_du;
	CSize		size;
	DATA		*p_buffer;
	BYTE		*p_dest_byte;
	float		*p_dest_float, *px, *py, *pz;
	short		*p_dest_short;
	WORD		*p_dest_word;
	long		*p_dest_long;
	DWORD		*p_dest_dword;
	BYTE		*p_mask_buffer;


	if( ( p_du = pAPI->GetDataUnit(target_data_number) ) == NULL )	return false;
	size = p_du->DataSize;
	

	//���݂̃f�[�^����U�i�[���郁�������m��
	p_buffer = new DATA[ size.cx * size.cy * p_du->PageNumber * p_du->ChannelNumber];
	if(p_buffer==NULL)	return false;

	p_mask_buffer = new BYTE[size.cx * size.cy];
	if(p_mask_buffer==NULL){
		delete[]	p_buffer;
		return false;
	}

	memcpy(p_mask_buffer, p_du->pMaskData, size.cx * size.cy);//�}�X�N�f�[�^���o�b�t�@�փR�s�[

	//�f�[�^��ϊ�
		switch( option.type_to )
		{
			case BYTE_FORMAT:
					if(option.byte_from_disp)
					{
						disp_to_buffer(p_du, p_buffer, rgb_gain, option.color_gray_mathod);//disp->BYTE
					}
					else
					{
						data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//data->BYTE
					}
					p_dest_byte = pAPI->GetByteMemory( target_data_number, size, true);//BYTE�^�̃������擾
					data_copy(p_buffer, p_dest_byte, size, option);//�o�b�t�@����f�[�^��߂�
				break;

			case SHORT16_FORMAT://data->short
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//�f�[�^���o�b�t�@�ɓ����
					p_dest_short = pAPI->GetShortMemory( target_data_number , size, true);//�V�������������擾
					data_copy(p_buffer, p_dest_short, size, option);//�o�b�t�@����f�[�^��߂�
				break;

			case WORD_FORMAT://data->WORD
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//�f�[�^���o�b�t�@�ɓ����
					p_dest_word = pAPI->GetWordMemory( target_data_number , size, true);//�V�������������擾
					data_copy(p_buffer, p_dest_word, size, option);//�o�b�t�@����f�[�^��߂�
				break;

			case LONG32_FORMAT://data->LONG
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//�f�[�^���o�b�t�@�ɓ����
					p_dest_long = pAPI->GetLongMemory( target_data_number , size, true);//�V�������������擾
					data_copy(p_buffer, p_dest_long, size, option);//�o�b�t�@����f�[�^��߂�
				break;

			case DWORD_FORMAT://data->DWORD
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//�f�[�^���o�b�t�@�ɓ����
					p_dest_dword = pAPI->GetDwordMemory( target_data_number , size, true);//�V�������������擾
					data_copy(p_buffer, p_dest_dword, size, option);//�o�b�t�@����f�[�^��߂�
				break;

			case RGB_FORMAT://data->RGB
				if(p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)//RGB���m�̕ϊ�
				{
					BYTE	*p_buffer_rgb = new BYTE[ size.cx * size.cy * p_du->ChannelNumber ];
					if(!p_buffer_rgb)	return false;

					memcpy(p_buffer_rgb, p_du->pByteData + p_du->DispPage * p_du->ChannelNumber * size.cx * size.cy , 
														size.cx * size.cy * p_du->ChannelNumber * sizeof(BYTE) );//���݂̃f�[�^���o�b�t�@�֊i�[
					p_dest_byte = pAPI->GetRGBMemory( target_data_number , size, true);//�V�������������擾

					memcpy(p_dest_byte, p_buffer_rgb, size.cx * size.cy * p_du->ChannelNumber * sizeof(BYTE) );//�o�b�t�@�R�s�[

					delete[]	p_buffer_rgb;
				}
				else
				{
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain);//�f�[�^���o�b�t�@�ɓ����
					p_dest_byte = pAPI->GetRGBMemory( target_data_number , size, true);//�V�������������擾

					if( option.rgb_to == RGB_TO_B )//B�ɓ����
					{
						data_copy(p_buffer, p_dest_byte + size.cx * size.cy * 2, size, option);
					}
					else if( option.rgb_to == RGB_TO_G )//G�ɓ����
					{
						data_copy(p_buffer, p_dest_byte + size.cx * size.cy, size, option);
					}
					else if( option.rgb_to == RGB_TO_R )//R�ɓ����
					{
						data_copy(p_buffer, p_dest_byte, size, option);
					}
					else//RGB�S�Ăɓ����
					{
						data_copy(p_buffer, p_dest_byte, size, option);
						data_copy(p_buffer, p_dest_byte + size.cx * size.cy, size, option);
						data_copy(p_buffer, p_dest_byte + size.cx * size.cy * 2, size, option);
					}
				}
				break;

			case FLOAT_FORMAT://data->FLOAT
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//�f�[�^���o�b�t�@�ɓ����
					p_dest_float = pAPI->GetFloatMemory( target_data_number , size, true);//�V�������������擾

					//�f�[�^����ւ�
					for(j=0 ; j<size.cy ; j++)
					{
						for(i=0 ; i<size.cx ; i++)
						{
							px = p_dest_float + i + j*size.cx;
							*px = (float)*(p_buffer + i + j*size.cx);
						}
					}
				break;

			case XYZ_FORMAT://data->XYZ
					data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//�f�[�^���o�b�t�@�ɓ����
					p_dest_float = pAPI->GetXYZMemory( target_data_number , size, true);//�V�������������擾

					//�f�[�^����ւ�
					for(j=0 ; j<size.cy ; j++)
					{
						for(i=0 ; i<size.cx ; i++)
						{
							px = p_dest_float + i + j*size.cx;
							py = px + size.cx * size.cy ;
							pz = px + size.cx * size.cy * 2;

							*px = (float)(i - size.cx/2);
							*py = (float)(-j + size.cy/2);
							*pz = (float)*(p_buffer + i + j*size.cx);
						}
					}
				break;

			case COMPLEX_FORMAT://data->COMP
				data_to_buffer(p_du, p_src, p_buffer, rgb_gain, option.color_gray_mathod);//�f�[�^���o�b�t�@�ɓ����
				p_dest_float = pAPI->GetComplexMemory( target_data_number , size, true);//�V�������������擾

				if( option.comp_to == COMP_TO_REAL ){//�����ɓ����
				}else{//�����ɓ����
					p_dest_float += size.cx * size.cy;
				}

				//�f�[�^����ւ�
				for(j=0 ; j<size.cy ; j++)
				{
					for(i=0 ; i<size.cx ; i++)
					{
						px = p_dest_float + i + j*size.cx;
						*px = (float)*(p_buffer + i + j*size.cx);
					}
				}
				p_du->CreateComplexPolarComponents();
				break;

			case THREE_D_FORMAT://data->3D
				for(int i=0 ; i<p_du->PageNumber ; i++)//�f�[�^���o�b�t�@�ɓ����
				{
					p_du->DispPage = i;
					data_to_buffer(p_du, p_src, p_buffer + size.cx * size.cy * i, rgb_gain, option.color_gray_mathod);
				}
				p_dest_byte = pAPI->Get3DMemory( target_data_number , size, p_du->PageNumber, true);//�V�������������擾

				if( (p_du = pAPI->GetDataUnit(target_data_number)) != NULL)
				{
					for(int i=0 ; i<p_du->PageNumber ; i++)//�o�b�t�@����f�[�^��߂�
					{
						p_du->DispPage = i;
						data_copy(p_buffer + size.cx * size.cy * i, p_dest_byte + size.cx * size.cy * i, size, option);
					}
				}

		}

	memcpy(pAPI->GetMaskMemory(target_data_number), p_mask_buffer,  size.cx * size.cy);//�}�X�N�f�[�^��߂�


	delete[]	p_buffer;
	delete[]	p_mask_buffer;


	

	return true;
}
#endif

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃^�C�v�ϊ�
��    ��    �� : ConvertDataUnit
��          �� : long					number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int					type_to			(in)�ϊ���̉摜�������̌^
				 int					rgb_to			(in)RGB�^�ɕϊ�����ꍇ�̓��͐� �i1:R���� 2:G���� 3:B���� 0:RGB�S�����j
				 int					comp_to			(in)���f���^�̑}����i0:�����@1:�����j
				 bool					satulate		(in)�^���̏㉺���𒴂���l��ϊ�����Ƃ��O�a�����邩�ǂ���
				 int					byte_from	�@�@(in)�\���������܂��̓}�X�N����ϊ����邩�ǂ����itype_to��BYTE�^�̂Ƃ��̂ݗL���j�i0:�f�[�^���� 1:�\���摜���� 2:�}�X�N����j
				bool					show_message	(in)�ϊ���Ƀ��b�Z�[�W��\�����邩
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ConvertDataUnit(long number, int type_to, int rgb_to, int comp_to, bool satulate, int byte_from, int color_gray_method, bool show_message)
{
	CONVERT_DATA_OPTION option = {0};
	option.type_to = type_to;
	option.rgb_to = rgb_to;
	option.comp_to = comp_to;
	option.satulate = satulate;
	option.byte_from = byte_from;
	option.color_gray_method = color_gray_method;

	return ConvertDataUnit(number,option,show_message );
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃^�C�v�ϊ�
��    ��    �� : ConvertDataUnit
��          �� : long					number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CONVERT_DATA_OPTION		option			(in)�ϊ��I�v�V����
				bool					show_message	(in)�ϊ���Ƀ��b�Z�[�W��\�����邩
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ConvertDataUnit(long number, CONVERT_DATA_OPTION option, bool show_message)
{
	CDataUnit	*p_du;

	if( (p_du = GetDataUnit(number) )==NULL)	return false;

	if(show_message){
		if( AfxMessageBox( "�f�[�^�^�C�v�̕ύX��͂��Ƃ̃f�[�^�͏�������܂�" , MB_OKCANCEL | MB_ICONINFORMATION ) != IDOK ){
			return false;
		}
	}

	return p_du->Convert(option);
}



/********************************************************************
�@  �\  ��  �� : �\���f�[�^��BYTE�^�ɃR���o�[�g
��    ��    �� : ConvertDispToByte
��          �� : long					number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ConvertDispToByte(long number)
{
	CONVERT_DATA_OPTION option={0};
	option.type_to = BYTE_FORMAT;
	option.byte_from = 1;

	return ConvertDataUnit(number, option, false);
}


/********************************************************************
�@  �\  ��  �� : �}�X�N��BYTE�^�ɃR���o�[�g
��    ��    �� : ConvertMaskToByte
��          �� : long					number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ��������� true
�@          �\ : �ϊ����ʂ́A���摜�������ɏ㏑�������
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ConvertMaskToByte(long number)
{
	CONVERT_DATA_OPTION option = { 0 };
	option.type_to = BYTE_FORMAT;
	option.byte_from = 2;

	return ConvertDataUnit(number, option, false);
}