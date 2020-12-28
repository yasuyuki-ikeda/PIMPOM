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
//�摜�������̑���
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "Shlwapi.h"

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
�@  �\  ��  �� : ���ݑI�𒆂̉摜�������̔ԍ����擾����
��    ��    �� : GetCurrentImageNumber
��          �� : 
��    ��    �l : ���ݑI�𒆂̉摜�f�[�^�̔ԍ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
long		CPimpomAPI::GetDataUnitNumber()
{
	return		data_number;
}

/********************************************************************
�@  �\  ��  �� : ���ݑI�𒆂̉摜�������̔ԍ����擾����
��    ��    �� : GetCurrentImageNumber
��          �� : 
��    ��    �l : ���ݑI�𒆂̉摜�f�[�^�̔ԍ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
long		CPimpomAPI::GetCurrentImageNumber()
{
	return		current_image_number;
}

/********************************************************************
�@  �\  ��  �� : //���ݑI�𒆂̉摜�f�[�^�̔ԍ���ύX����i�ĕ`��͂��Ȃ��j
��    ��    �� : SetCurrentImageNumber
��          �� : 
��    ��    �l : ���������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool		CPimpomAPI::SetCurrentImageNumber(long number)
{
	//�����`�F�b�N
	if(number<0 || number>= GetDataUnitNumber())	return false;

	current_image_number = number;

	return true;
}

/********************************************************************
�@  �\  ��  �� : DataUnit�N���X�ւ̃|�C���^�����擾����
��    ��    �� : GetDataUnit
��          �� : long		number		(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : DataUnit�N���X�ւ̃|�C���^�B�f�[�^�������ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CDataUnit	*CPimpomAPI::GetDataUnit(long number)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>= GetDataUnitNumber())	return NULL;

	return		pp_data_unit[number];
}

/********************************************************************
�@  �\  ��  �� : �摜�������̌^�E�T�C�Y�E���s���擾
��    ��    �� : GetDataTypeSize
��          �� : long		number		(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 int		*ptype		(out)�摜�������̌^
				 int		*pwidth		(out)�摜��
				 int		*pheight	(out)�摜����
				 int		*ppages		(out)�摜�̉��s
��    ��    �l : �f�[�^�������ꍇ��false ����ȊO��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::GetDataTypeSize(long number, int *ptype, int *pwidth, int *pheight, int *ppages)
{
	CDataUnit *pdu = GetDataUnit(number);
	if(!pdu)	return false;

	*ptype = pdu->DataType;
	*pwidth = pdu->DataSize.cx;
	*pheight = pdu->DataSize.cy;
	*ppages = pdu->PageNumber;

	return true;
}

/********************************************************************
�@  �\  ��  �� : �摜�������̍ő�E�ŏ���Ԃ�
��    ��    �� : GetDataTypeSize
��          �� : long		number		(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 float		*pMin		(out)�ŏ��l
				 float		*pMax		(out)�ő�l
��    ��    �l : �f�[�^�������ꍇ��false ����ȊO��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::GetDataMinMax(long number, float *pMin, float *pMax)
{
	CDataUnit *pdu = GetDataUnit(number);
	if(!pdu)	return false;

	pdu->GetDataMinMax(pMin, pMax);

	return true;
}


/********************************************************************
�@  �\  ��  �� : �\���f�[�^�̃y�[�W��ݒ肷��
��    ��    �� : SetDispPage
��          �� : long		number		(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		page		(in)�y�[�W
��    ��    �l : �f�[�^�������ꍇ��false ����ȊO��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::SetDispPage(long number, long page)
{
	CDataUnit *pdu = GetDataUnit(number);
	if(!pdu)	return false;

	pdu->SetDispPage(page);

	return true;
}

/********************************************************************
�@  �\  ��  �� : �\���f�[�^�̃`���l����ݒ肷��
��    ��    �� : GetDataTypeSize
��          �� : long		number		(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		channel		(in)�`���l��
��    ��    �l : �f�[�^�������ꍇ��false ����ȊO��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::SetDispChannel(long number, long channel)
{
	CDataUnit *pdu = GetDataUnit(number);
	if(!pdu)	return false;

	pdu->SetDispChannel(channel);

	return true;
}


/********************************************************************
�@  �\  ��  �� : �w�肵���`���̉摜�������iDataUnit�j���擾����
��    ��    �� : FormatDataUnit
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		type			(in)�摜�f�[�^�̃^�C�v
				 CSize		size			(in)�摜�f�[�^�̃T�C�Y
				 long		page			(in)�摜�f�[�^�̉��s��
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : DataUnit�N���X�ւ̃|�C���^�B���s�����ꍇ��NULL
�@          �\ : number�Ŏw�肳�ꂽ�ԍ���DataUnit�̃��������m�ۂ̏ꍇ�́A�V���Ƀ������m�ۂ���
				�w�肳�����^�C�v�E�T�C�Y�ƈقȂ郁����������ꍇ�́A�����f�[�^�̂݃�������蒼��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
2013.05.06   Y.Ikeda         GetDataUnit�œ����|�C���^�������N�؂���N�����Ȃ��悤�ɏC��
********************************************************************/
CDataUnit	*CPimpomAPI::FormatDataUnit(long number, long type, CSize size, long page, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>= GetDataUnitNumber())	return NULL;


	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��
	
	if(page<=0 )	return NULL;



	if(!do_clear_memory){//�ė��p
		CDataUnit *p_du;
		if( p_du = GetDataUnit(number),  p_du != NULL){//���łɃ����������݂���	
			if( p_du->Compare(type, size, page) ){//�������^�C�v�E�T�C�Y�����Ȃ�
				return	p_du;
			}
		}
	}
			
	if( pp_data_unit[number] == NULL )
	{
		pp_data_unit[number] = new CDataUnit(type, size, page);//��������V�K�m�ۂ���
	}
	else
	{
		pp_data_unit[number]->Destroy();//���ݓ����Ă���f�[�^���폜����

		if(!pp_data_unit[number]->Format(type, size, page)){//DataUnit�̃��������Ƃ�Ȃ���
			delete	pp_data_unit[number];
			pp_data_unit[number]=NULL;
		}
	}

	return	pp_data_unit[number];
}

/********************************************************************
�@  �\  ��  �� : �摜�������iDataUnit�j���폜����
��    ��    �� : DeleteDataUnit
��          �� : long		number		(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::DeleteDataUnit(long number)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>= GetDataUnitNumber())	return false;

	if( pp_data_unit[number] != NULL ){

		delete	pp_data_unit[number];//���ݓ����Ă���f�[�^���폜����
		pp_data_unit[number] = NULL;
		DrawImage(number);

		return true;
	}else{
		return false;
	}
}


/********************************************************************
�@  �\  ��  �� : �摜�������̓��e���R�s�[����
��    ��    �� : CopyDataUnit
��          �� : long		src_number		(in)�R�s�[���摜�������ԍ�
				 long		dest_number		(in)�R�s�[��摜�������ԍ�(src_number�Ɠ����l�͎w��s��)
				 bool		show_confirm	(in)�R�s�[���s���Ɍx�����b�Z�[�W�{�b�N�X���o�����ǂ���
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::CopyDataUnit(long src_number, 
								 long dest_number, 
								 bool show_confirm)
{
	//�����`�F�b�N
	if(src_number<0 || src_number>=GetDataUnitNumber() )	return false;//src_number���s��
	if(dest_number<0 || dest_number>=GetDataUnitNumber() )	return false;//dest_number���s��
	if( src_number == dest_number )		return	false;//�����摜�������ԍ�
	if( pp_data_unit[ src_number ] == NULL)		return	false;//�R�s�[���ɉ����Ȃ�


	CDataUnit	*p_dest_du;
	CConfirmDlg confirm;
	bool		do_copy_data=true;
	bool		do_copy_mask=true;
	bool		do_copy_workarea=true;
	bool		do_copy_range=true;
	bool		do_copy_mark=true;
	bool		do_copy_pos_scale=true;

	p_dest_du = pp_data_unit[ dest_number ];
	if(p_dest_du != NULL && show_confirm)
	{
		confirm.SetWidth(100, 0);
		confirm.SetTitle("�R�s�[����f�[�^");
		confirm.RegistVar("�摜�f�[�^", &do_copy_data);
		confirm.RegistVar("�}�X�N", &do_copy_mask);
		confirm.RegistVar("�}�[�N", &do_copy_mark);
		confirm.RegistVar("�\���Z�x�����W", &do_copy_range);
		confirm.RegistVar("��Ɨ̈�", &do_copy_workarea);
		confirm.RegistVar("�\���ʒu", &do_copy_pos_scale);

		if(confirm.DoModal()!=IDOK)		return	false;//�R�s�[����f�[�^��ނ̖₢���킹
	}
	
	return CopyDataUnit(src_number, dest_number, do_copy_data, do_copy_mask, do_copy_workarea, do_copy_range, do_copy_mark, do_copy_pos_scale);
}

/********************************************************************
�@  �\  ��  �� : �摜�������̓��e���R�s�[����
��    ��    �� : CopyDataUnit
��          �� : long		src_number			(in)�R�s�[���摜�������ԍ�
				 long		dest_number			(in)�R�s�[��摜�������ԍ�(src_number�Ɠ����l�͎w��s��)
				 bool		do_copy_data		(in)�摜�������̒����R�s�[�i�摜�������̌^/�T�C�Y/�y�[�W/�`���l�����Ⴄ�ꍇ�̓������Ď擾�j
				 bool		do_copy_mask		(in)�}�X�N���R�s�[�i�T�C�Y�������ꍇ�̂݁j
				 bool		do_copy_workarea	(in)��Ɨ̈���R�s�[
				 bool		do_copy_range		(in)�\���摜�̔Z�x�����W���R�s�[
				 bool		do_copy_mark		(in)�}�[�N���R�s�[���邩�ǂ���
				 bool		do_copy_pos_scale	(in)�\���ʒu�ƃX�P�[�����R�s�[�i�T�C�Y�������ꍇ�̂݁j
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::CopyDataUnit(long src_number, 
								 long dest_number, 
								 bool do_copy_data, 
								 bool do_copy_mask, 
								 bool do_copy_workarea,
								 bool do_copy_range,
								 bool do_copy_mark,
								 bool do_copy_pos_scale)
{
	CDataUnit	*p_src_du, *p_dest_du;
	bool		data_rearoc_flg = false;//�R�s�[��ɉ惁����(��)�擾���邩�ǂ���


	//�����`�F�b�N
	if(src_number<0 || src_number>=GetDataUnitNumber() )	return false;//src_number���s��
	if(dest_number<0 || dest_number>=GetDataUnitNumber() )	return false;//dest_number���s��
	if( src_number == dest_number )		return	false;//�����摜�������ԍ�
	if( ( p_src_du = pp_data_unit[ src_number ] ) == NULL)		return	false;//�R�s�[���̃f�[�^���Ȃ�
		


	p_dest_du = pp_data_unit[ dest_number ];

		if( p_dest_du == NULL)//�R�s�[��ɉ摜���������Ȃ��ꍇ�̓������擾����
		{
			data_rearoc_flg = true;
		}
		else
		{	
			if(do_copy_data)//�摜�f�[�^���R�s�[����ꍇ
			{
				if(p_dest_du->DataSize != p_src_du->DataSize ||
					p_dest_du->DataType != p_src_du->DataType ||
					p_dest_du->PageNumber != p_src_du->PageNumber ||
					p_dest_du->ChannelNumber != p_src_du->ChannelNumber 
					)
				{
						data_rearoc_flg = true;//�R�s�[��Ɖ摜�������̌^/�T�C�Y/�y�[�W/�`���l�����Ⴄ�ꍇ�̓������Ď擾����
				}
			}
		}

		if(data_rearoc_flg)
		{//�R�s�[��ɉ摜�������擾
			p_dest_du = FormatDataUnit(dest_number, p_src_du->DataType, p_src_du->DataSize, p_src_du->PageNumber, true);//�R�s�[��ɂ����炵�����������擾����
			if( p_dest_du == NULL)		return	false;//�������擾���s
		}


		//�摜�f�[�^���R�s�[����
		if( !p_dest_du->Copy( p_src_du, do_copy_data,
					do_copy_mask,
					do_copy_workarea,
					do_copy_range,
					do_copy_mark,
					do_copy_pos_scale))	return	false;




	return	true;//����������true��Ԃ�
}



/********************************************************************
�@  �\  ��  �� : BYTE�^�̉摜���������擾
��    ��    �� : GetByteMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
unsigned char	*CPimpomAPI::GetByteMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��


	//�摜���������擾
	CDataUnit	*p_du = FormatDataUnit(number, BYTE_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pByteData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}


/********************************************************************
�@  �\  ��  �� : FLOAT�^�̉摜���������擾
��    ��    �� : GetFloatMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
float			*CPimpomAPI::GetFloatMemory(long number, CSize size, bool do_clear_memory)
{	
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��
	

	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, FLOAT_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}


/********************************************************************
�@  �\  ��  �� : 8bit����^�̉摜���������擾
��    ��    �� : Get3DMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 long		page			(in)�摜���s��
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
unsigned char	*CPimpomAPI::Get3DMemory(long number, CSize size, long page, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��

	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, THREE_D_FORMAT, size, page, do_clear_memory);

		if(p_du != NULL)		return	p_du->pByteData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : 32bit����^�̉摜���������擾
��    ��    �� : GetF3DMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 long		page			(in)�摜���s��
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
float			*CPimpomAPI::GetF3DMemory(long number, CSize size, long page, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��


	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, FLOAT_3D_FORMAT, size, page, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : ���f���^�̉摜���������擾
��    ��    �� : GetComplexMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
float			*CPimpomAPI::GetComplexMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��

	
	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, COMPLEX_FORMAT, size, 4, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : XY�^�̉摜���������擾
��    ��    �� : GetXYMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
float			*CPimpomAPI::GetXYMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return false;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return false;//�摜�T�C�Y���s��


	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, XY_FORMAT, size, 2, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : XYZ�^�̉摜���������擾
��    ��    �� : GetXYZMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
float			*CPimpomAPI::GetXYZMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return false;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return false;//�摜�T�C�Y���s��


	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, XYZ_FORMAT, size, 3, do_clear_memory);

		if(p_du != NULL)		return	p_du->pFloatData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : RGB�װ�^�̉摜���������擾
��    ��    �� : GetRGBMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
unsigned char	*CPimpomAPI::GetRGBMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��
		

	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, RGB_FORMAT, size, 3, do_clear_memory);

		if(p_du != NULL)		return	p_du->pByteData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : RGB�װ����^�̉摜���������擾
��    ��    �� : GetRGBMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 long		page			(in)�摜���s��
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
unsigned char	*CPimpomAPI::GetRGB3DMemory(long number, CSize size, long page, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��
		

	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, RGB_3D_FORMAT, size, page, do_clear_memory);

		if(p_du != NULL)		return	p_du->pByteData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : 16bit�����t���^�̉摜���������擾
��    ��    �� : GetShortMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
short			*CPimpomAPI::GetShortMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��

	
	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, SHORT16_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pShortData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : 16bit�^�̉摜���������擾
��    ��    �� : GetWordMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
WORD			*CPimpomAPI::GetWordMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��

	
	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, WORD_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pWordData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : 32bit�������^�̉摜���������擾
��    ��    �� : GetLongMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
long			*CPimpomAPI::GetLongMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��
	

	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, LONG32_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pLongData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : 32bit�^�̉摜���������擾
��    ��    �� : GetDwordMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CSize		size			(in)�摜�T�C�Y
				 bool		do_clear_memory	(in)���ɓ��^�C�v�E�T�C�Y�̃��������m�ۂ���Ă���ꍇ�A���݂̃f�[�^���N���A���邩�ǂ���
��    ��    �l : �摜�������ւ̃|�C���^�B�擾���s�����ꍇ��NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
DWORD			*CPimpomAPI::GetDwordMemory(long number, CSize size, bool do_clear_memory)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	if(size.cx<=0 || size.cy<=0)	return NULL;//�摜�T�C�Y���s��
	

	//�摜���������擾
	CDataUnit *p_du = FormatDataUnit(number, DWORD_FORMAT, size, 1, do_clear_memory);

		if(p_du != NULL)		return	p_du->pDwordData;
		else					return	NULL;//�������擾�Ɏ��s�����ꍇ
}

/********************************************************************
�@  �\  ��  �� : �}�X�N���������擾
��    ��    �� : GetLongMemory
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : �}�X�N�������ւ̃|�C���^
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BYTE			*CPimpomAPI::GetMaskMemory(long number)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return NULL;//src_number���s��

	//�摜���������擾
	CDataUnit *p_du = GetDataUnit(number);

		if(p_du != NULL)		return	p_du->pMaskData;
		else					return	NULL;//�������������ꍇ
}


/********************************************************************
�@  �\  ��  �� : �ǂݍ��݌��̃t�@�C���̐�΃p�X�擾
��    ��    �� : GetSrcFilePathName
��          �� : long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : �t�@�C������ǂݍ��܂ꂽ�摜�������ɑ΂��A�ǂݍ��݌��̃t�@�C���̃t���p�X��Ԃ�
				 �t�@�C������ǂݍ��܂�Ă��Ȃ��摜�������ł́A�k���������A��
				 �摜�����������݂��Ȃ��ꍇ�̓k���������A��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CString			CPimpomAPI::GetSrcFilePathName(long number)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return CString("");//src_number���s��

	//�摜���������擾
	CDataUnit *p_du = GetDataUnit(number);

		if(p_du != NULL)		return	p_du->SrcFilePath;
		else					return	CString("");//�������������ꍇ
}


CString			CPimpomAPI::GetSrcFileName(long number)
{
	CString full_path = GetSrcFilePathName(number);

	return CString( PathFindFileName(full_path) );
}

CString			CPimpomAPI::GetSrcFilePath(long number)
{
	CString full_path = GetSrcFilePathName(number);
	CString name = GetSrcFileName(number);

	return full_path.Left(full_path.GetLength()-name.GetLength());
}


CString			CPimpomAPI::GetSrcFileTitle(long number)
{
	CString filename = GetSrcFileName(number);
	CString filetitle = filename;

	for(int i=filename.GetLength()-1 ; i>1 ; i--){
		if(filename[i]=='.'){
			filetitle = filename.Left(i);
			break;
		}
	}

	return filetitle;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��Ɨ̈�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈��ݒ�(��`)
��    ��    �� : SetWorkArea
��          �� : RECT		rect			(in)��`
				 long		number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long		figure_number	(in)�}�`�ԍ�
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, RECT rect, bool ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	pdu->WorkArea.SetWorkArea(figure_number, rect, ornot);
	return true;
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈��ݒ�(��������)
��    ��    �� : SetWorkArea
��          �� : FIGURE_LINE		line			(in)��������
				 long				number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long				figure_number	(in)�}�`�ԍ�
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, FIGURE_LINE line, bool ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	pdu->WorkArea.SetWorkArea(figure_number, line, ornot);
	return true;
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈��ݒ�(�ȉ~)
��    ��    �� : SetWorkArea
��          �� : FIGURE_ELLIPSE	ellipse			(in)�ȉ~
				 long				number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long				figure_number	(in)�}�`�ԍ�
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, FIGURE_ELLIPSE ellipse, bool ornot)
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	return pdu->WorkArea.SetWorkArea(figure_number, ellipse, ornot);
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈��ݒ�(�����~��)
��    ��    �� : SetWorkArea
��          �� : FIGURE_CIRCLE	circle			(in)�����~��
				 long				number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long				figure_number	(in)�}�`�ԍ�
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, FIGURE_CIRCLE circle, bool ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	return pdu->WorkArea.SetWorkArea(figure_number, circle, ornot);
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈��ݒ�(�����~��)
��    ��    �� : SetWorkArea
��          �� : FIGURE_ARC		arc				(in)�����~��
				 long				number			(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long				figure_number	(in)�}�`�ԍ�
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool		CPimpomAPI::SetWorkArea(long image_number, long figure_number, FIGURE_ARC arc, bool ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return false;

	return pdu->WorkArea.SetWorkArea(figure_number, arc, ornot);
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈���擾
��    ��    �� : GetWorkArea
��          �� : FIGURE_UNION	*area			(out)��Ɨ̈�̐}�`
				 bool				*ornot			(out)true:OR�̈�@false:NOT�̈�
				 long				image_number	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long				figure_number	(in)�}�`�ԍ�
��    ��    �l : ��Ɨ̈�̎�ށB���s������-1
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int		CPimpomAPI::GetWorkArea(long image_number, long figure_number, FIGURE_UNION *area, bool *ornot )
{
	CDataUnit *pdu;
	if( !(pdu = GetDataUnit(image_number)) )		return -1;

	if(figure_number<0 || figure_number>=WORKAREA_FIG_NUM)	return -1;
	
	memcpy(area, &pdu->WorkArea.Figure[figure_number], sizeof(FIGURE_UNION));
	*ornot = pdu->WorkArea.OrNot[figure_number];
	return pdu->WorkArea.Type[figure_number];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�}�[�N
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : �}�[�N��ǉ�����
��    ��    �� : AddMark
��          �� : long				image_number	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 float				x,y				(in)�}�[�N���W
				 double				*p_attrib		(in)�����z��
��    ��    �l : ����������ǉ����ꂽ�}�[�N�ԍ���Ԃ����s������-1��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
long	CPimpomAPI::AddMark(long image_number, float x, float y, double *p_attrib)
{
	CMark mark = CMark(x,y);
	if(p_attrib){
		memcpy(mark.attrib, p_attrib, sizeof(double)*MARK_ATTRIB_NUM);
	}else{
		memset(mark.attrib,0,sizeof(double)*MARK_ATTRIB_NUM);
	}

	return AddMark(image_number,mark);
}


long	CPimpomAPI::AddLineMark(long image_number, float x1, float y1, float x2, float y2, double *p_attrib)
{
	CMark mark = CMark(x1,y1,x2,y2);
	if(p_attrib){
		memcpy(mark.attrib, p_attrib, sizeof(double)*MARK_ATTRIB_NUM);
	}else{
		memset(mark.attrib,0,sizeof(double)*MARK_ATTRIB_NUM);
	}

	return AddMark(image_number,mark);
}


long	CPimpomAPI::AddMark(long image_number, CMark mark)
{
	CDataUnit	*pdu= GetDataUnit(image_number);
	if( pdu==NULL )	return -1;//�f�[�^���Ȃ��Ƃ�

	int num = pdu->AddMark(mark);
	if(num<0)		return -1;
			
	SetCheck( pPIMMOMDlg->m_hWnd , IDC_BUTTON_PIMPON_DLG_MARK, true);//���C���E�B���h�E�̃}�[�N�\��ON

	//�}�[�N�E�B���h�E�ɔ��f
	if(pMarkDlg){
		pMarkDlg->ShowWindow(SW_SHOW);
		pMarkDlg->ShowMarkVal(image_number);
	}

	return num;
}

/********************************************************************
�@  �\  ��  �� : �}�[�N����������
��    ��    �� : DeleteMark
��          �� : long				image_number	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long				number			(in)�}�[�N�ԍ�
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::DeleteMark(long image_number, long mark_number)
{
	CDataUnit	*pdu = GetDataUnit(image_number);

	if( pdu==NULL )		return	false;//�f�[�^�������Ƃ�

	return pdu->DeleteMark(mark_number);
}


/********************************************************************
�@  �\  ��  �� : ���ׂẴ}�[�N����������
��    ��    �� : DeleteMark
��          �� : long				image_number	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ClearMark(long number)
{
	CDataUnit	*pdu = GetDataUnit(number);

	if( pdu==NULL )		return	false;//�f�[�^�������Ƃ�

	pdu->ClearMark();

	return true;
}

/********************************************************************
�@  �\  ��  �� : �}�[�N�̍��W�ƃf�[�^�l���擾����
��    ��    �� : GetMarkVal
��          �� : long				image_number	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long				number			(in)�}�[�N�ԍ�
				 float				*px,*py			(out)�}�[�N���W
				 double				*p_val			(out)�}�[�N���W�̃f�[�^�l
				 double				*p_attrib		(out)�}�[�N�̑���(�z��)
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetMarkVal(long image_number, long mark_number, float *px, float *py, double *p_val, double *p_attrib)
{
	CDataUnit	*pdu = GetDataUnit(image_number);
	if( pdu==NULL )		return	false;//�f�[�^�������Ƃ�
	
	CMark mark;
	if( !pdu->GetMark(mark_number, &mark))	return false;

	*px = mark.figure.point.x;//�}�[�N�̍��W���擾
	*py = mark.figure.point.y;
	*p_val = pdu->GetDataValue( *px, *py ,-1,-1);//�}�[�N���W�̒l���擾

	if(p_attrib){
		memcpy( p_attrib, mark.attrib, MARK_ATTRIB_NUM*sizeof(double));//�}�[�N�̑������擾
	}

	return	true;
}

/********************************************************************
�@  �\  ��  �� : �}�[�N�̐����擾����
��    ��    �� : GetMarkNumber
��          �� : long				image_number	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : �}�[�N�̐� ���s������-1��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
long	CPimpomAPI::GetMarkNumber(long image_number)
{
	CDataUnit	*pdu;

	pdu = GetDataUnit(image_number);
		if( pdu==NULL )		return	-1;//�f�[�^�������Ƃ�

	return		pdu->GetMarkNumber();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�o�b�N�A�b�v
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : �摜�f�[�^���o�b�N�A�b�v����
��    ��    �� : BackupDataUnit
��          �� : long				image_number	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::BackupDataUnit(long number)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return false;//src_number���s��

	CDataUnit *psrc_du = GetDataUnit(number);
		if( !psrc_du )		return	false;//�o�b�N�A�b�v���̉摜��������΂Ȃɂ����Ȃ�
		
		if( backup_data!=NULL )	delete	backup_data;

	backup_data = new CDataUnit(	psrc_du->DataType, psrc_du->DataSize, psrc_du->PageNumber);
	if(!backup_data)	return false;

	return	backup_data->Copy(psrc_du);
}

/********************************************************************
�@  �\  ��  �� : �o�b�N�A�b�v���Ă���摜�f�[�^�𕜊�������
��    ��    �� : RecoverDataUnit
��          �� : long				image_number	(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::RecoverDataUnit(long number)
{
	//�����`�F�b�N
	if(number == CURRENT_IMAGE)	number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	else if(number<0 || number>=GetDataUnitNumber() )	return false;//src_number���s��

	if( !backup_data )	return	false;//�o�b�N�A�b�v�f�[�^���Ȃ�

	CDataUnit *pdst_du = FormatDataUnit(number, backup_data->DataType,backup_data->DataSize, backup_data->PageNumber,true);
		if(!pdst_du)	return	false;
		if( !pdst_du->Copy(backup_data))	return	false;

	DrawImage(number);

	return	true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���Z���ŉ摜����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : ���Z���ŉ摜����
��    ��    �� : CalcDataValue
��          �� : long				number			(in)���ʊi�[����摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CString			com_str[]		(in)���Z��
				 int				com_str_cnt		(in)���Z��
				 bool				satulate		(in)�㉺���𒴂����ꍇ�ɖO�a������(false�ɂ���ƃI�[�o�[�t���[����)
				 CString			*pErr_str		(out)�G���[
				 CProgressCtrl		*pProg			(in)�v���O���X�o�[�ւ̃|�C���^(�v���O���X�o�[���g��Ȃ��ꍇ��NULL)
��    ��    �l : ����������true
�@          �\ : com_str�Ŏw�肳�ꂽ���Z�������߂��āA��f���ƂɎ��s���A
				 ���ʂ�image_number�Ŏw�肳�ꂽ�摜�������ɓ����B
				 �摜�����������擾�Ȃ�΃G���[�ɂȂ�
				 pProg���w�肷��Ə����r���o�߂��\�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::CalcDataValue(long number, CString com_str, bool satulate, CString *pErr_str, CProgressCtrl *pProg)
{
	return CalcDataValue(number, -1, CSize(1,1),com_str,satulate,pErr_str,pProg);
}

/********************************************************************
�@  �\  ��  �� : ���Z���ŉ摜����
��    ��    �� : CalcDataValue
��          �� : long				number			(in)���ʊi�[����摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 long				type			(in)�摜�������̃^�C�v
				 CSize				size			(in)�摜�������̃T�C�Y
				 CString			com_str			(in)���Z��
				 bool				satulate		(in)�㉺���𒴂����ꍇ�ɖO�a������(false�ɂ���ƃI�[�o�[�t���[����)
				 CString			*pErr_str		(out)�G���[
				 CProgressCtrl		*pProg			(in)�v���O���X�o�[�ւ̃|�C���^(�v���O���X�o�[���g��Ȃ��ꍇ��NULL)
��    ��    �l : ����������true
�@          �\ : com_str�Ŏw�肳�ꂽ���Z�������߂��āA��f���ƂɎ��s���A
				 ���ʂ�image_number�Ŏw�肳�ꂽ�摜�������ɓ����B
				 ��������type,size�Ŏw�肳�ꂽ���̂��V�����擾�����
				 pProg���w�肷��Ə����r���o�߂��\�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::CalcDataValue(long number, long type, CSize size, CString com_str, bool satulate, CString *pErr_str, CProgressCtrl *pProg)
{
	int	i,j;
	CCalculate		calc;
	double			result;
	CDataUnit		*pdu=NULL;
	CString			err_str, com_strs[3];
	bool			ret=false;
	CRect			calc_area;
	char			strbuf[CALC_STRING_LEN_MAX];



	if(type<0){//���݂��郁�������g��
		if( !(pdu = GetDataUnit(number)) ){
			err_str = "���̃������ɂ͉摜�����݂��܂���";
			goto CalcDataValue_post_proc;	
		}
	}else{//�V�������������m�ۂ���
		if(!(pdu = FormatDataUnit(number, type, size, 1, true))){
			goto CalcDataValue_post_proc;	
		}
	}
	calc_area = pdu->GetRectArea();//�v���̈�


	//�R�}���h�o�^
	com_strs[0] = com_str;
	if( !calc.RegisterCommandString(com_strs, 1, &err_str) )	goto CalcDataValue_post_proc;

	//�R�}���h�e�X�g���s
	if( !calc.Execute(0, 1,1, &err_str) )	goto CalcDataValue_post_proc;
	if( calc.GetResult(0,&result, strbuf, &err_str ) != 0 )	goto CalcDataValue_post_proc;


	if(pProg)	pProg->SetRange(0, (short)pdu->DataSize.cy-1);//�v���O���X�o�[����

	//�R�}���h����f���ƂɎ��s
	for(j=calc_area.top ; j<=calc_area.bottom ; j++)
	{
		for(i=calc_area.left ; i<=calc_area.right ; i++)
		{
			//�}�X�N����
			if (pdu->DoUseMask) {
				if (pdu->GetMaskValue(CPoint(i, j)) == MASKED_PIXEL)		continue;
			}


			//���[�N�G���A����
			if (pdu->DoUseWorkArea)
			{
				if( !pdu->WorkArea.PtInWorkArea(CPoint(i, j)))		continue;
			}



			if ((pdu->DataType == RGB_FORMAT || pdu->DataType == RGB_3D_FORMAT) && pdu->DispRGB)
			{//RGB���ꂼ��ɑ΂��鏈��
					for (int c = 0; c < 3; c++)
					{
						if (!calc.Execute(0, i, j, &err_str, c))		goto CalcDataValue_post_proc;
						if (calc.GetResult(0, &result, strbuf, &err_str) != 0)	goto CalcDataValue_post_proc;

						pdu->SetDataValue(i, j, result, -1, c);
					}
			}
			else//�O���[�l�ɑ΂��鏈��
			{
					if (!calc.Execute(0, i, j, &err_str))			goto CalcDataValue_post_proc;
					if (calc.GetResult(0, &result, strbuf, &err_str) != 0)	goto CalcDataValue_post_proc;

					pdu->SetDataValue(i, j, result, -1, -1, satulate);
			}
		}
		if(pProg)	pProg->SetPos(j);//�v���O���X�o�[�i�߂�
	}
	if(pProg)	pProg->SetPos(0);//�v���O���X�o�[�I��

	
	//DrawImage( number, true);//�ĕ`��
	ret = true;

CalcDataValue_post_proc:
	if(pErr_str){	*pErr_str = err_str;}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : ���Z���ŉ摜�����i�}�X�N�p�j
��    ��    �� : CalcMaskValue
��          �� : long				number			(in)���ʊi�[����摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CString			com_str			(in)���Z��
				 double				mask_max		(in)�}�X�NOFF�Ƃ���ő�l
				 double				mask_min		(in)�}�X�NOFF�Ƃ���ŏ��l
				 CString			*pErr_str		(out)�G���[
				 CProgressCtrl		*pProg			(in)�v���O���X�o�[�ւ̃|�C���^(�v���O���X�o�[���g��Ȃ��ꍇ��NULL)
��    ��    �l : ����������true
�@          �\ : com_str�Ŏw�肳�ꂽ���Z�������߂��āA��f���ƂɎ��s���A
				 ���ʂ���image_number�Ŏw�肳�ꂽ�摜�������̃}�X�N�����
				 �摜�����������擾�Ȃ�΃G���[�ɂȂ�
				 pProg���w�肷��Ə����r���o�߂��\�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::CalcMaskValue(long number, CString com_str, double mask_max, double mask_min, CString *pErr_str, CProgressCtrl *pProg)
{
	int	i,j;
	CCalculate		calc;
	double			result;
	CDataUnit		*pdu=NULL;
	CString			err_str, com_strs[3];;
	bool			ret=false;
	CRect			calc_area;
	char			strbuf[CALC_STRING_LEN_MAX];




		if( !(pdu = GetDataUnit(number)) ){
			err_str = "���̃������ɂ͉摜�����݂��܂���";
			goto CalcMaskValue_post_proc;	
		}
	calc_area = pdu->GetRectArea();//�v���̈�


	//�R�}���h�o�^
	com_strs[0] = com_str;
	if( !calc.RegisterCommandString(com_strs,1, &err_str) )	goto CalcMaskValue_post_proc;


	//�R�}���h�e�X�g���s
	if( !calc.Execute(0, 1,1, &err_str) )	goto CalcMaskValue_post_proc;
	if( calc.GetResult(0,&result, strbuf, &err_str ) != 0 )	goto CalcMaskValue_post_proc;


	if(pProg)	pProg->SetRange(0, (short)pdu->DataSize.cy-1);//�v���O���X�o�[����

	//�R�}���h����f���ƂɎ��s
	for(j=calc_area.top ; j<=calc_area.bottom ; j++)
	{
		for(i=calc_area.left ; i<=calc_area.right ; i++)
		{
			int val;
			if((pdu->DataType==RGB_FORMAT || pdu->DataType==RGB_3D_FORMAT) && pdu->DispRGB )
			{//RGB���ꂼ��ɑ΂��鏈��
				double color_reslt[3]={0};
				for(int c=0 ; c<3 ; c++)
				{
					if(!calc.Execute(0,i,j,&err_str,c))				goto CalcMaskValue_post_proc;
					if( calc.GetResult(0,&color_reslt[c], strbuf, &err_str ) != 0 )	goto CalcMaskValue_post_proc;
				}
				result = ( color_reslt[0]*rgb_gain[0] + color_reslt[1]*rgb_gain[1] + color_reslt[2]*rgb_gain[2] )/1000;
			}
			else//�O���[�l�ɑ΂��鏈��
			{
				if(!calc.Execute(0, i,j,&err_str))			goto CalcMaskValue_post_proc;
				if( calc.GetResult(0, &result, strbuf, &err_str ) != 0 )	goto CalcMaskValue_post_proc;
			}

			if( mask_min <= result && mask_max>=result ){	val = UNMASKED_PIXEL;	}
			else										{	val = MASKED_PIXEL;		}
			pdu->SetMaskValue(i,j, val);
		}
		if(pProg)	pProg->SetPos(j);//�v���O���X�o�[�i�߂�
	}
	if(pProg)	pProg->SetPos(0);//�v���O���X�o�[�I��

	
	//DrawImage( number, true);//�ĕ`��
	ret = true;

CalcMaskValue_post_proc:
	if(pErr_str){	*pErr_str = err_str;}
	return ret;
}