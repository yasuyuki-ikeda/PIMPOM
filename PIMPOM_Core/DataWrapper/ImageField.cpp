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
//CImageField
//�摜�̉�ʕ\���������N���X
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

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
//�W���̃R���X�g���N�^
CImageField::CImageField()
{

}

/********************************************************************
�@  �\  ��  �� : �R���X�g���N�^
��    ��    �� : CImageField
��          �� : HWND			wnd			(in)�摜�\������E�B���h�E�̃n���h��
				 CSize			field_size	(in)ImageField�̃T�C�Y
				 CPoint			field_coor	(in)ImageField�̃E�B���h�E���̈ʒu
��    ��    �l : 
�@          �\ : CImageField�N���X�̍\�z
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CImageField::CImageField(HWND wnd, CSize field_size, CPoint field_coor)
{
	hImageWnd = wnd;
	FieldSize = field_size;
	FieldCoordinate = field_coor;
}

//�f�X�g���N�^
CImageField::~CImageField()
{

}


/********************************************************************
�@  �\  ��  �� : ���T�C�Y
��    ��    �� : Resize
��          �� : CSize size					�V�����T�C�Y
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CImageField::Resize(CSize size)
{
	FieldSize = size;

	return true;
}


/********************************************************************
�@  �\  ��  �� : �ړ�
��    ��    �� : Move
��          �� : CPoint point					�V����������W
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CImageField::Move(CPoint point)
{
	FieldCoordinate	= point;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�`��֐�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : �摜��\������
��    ��    �� : DrawImage
��          �� : CDataUnit		*p_du					(in)�摜������
				 bool			do_redraw_background	(in)�w�i���ĕ`�悷�邩�ǂ���
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CImageField::DrawImage(CDataUnit *p_du, bool do_redraw_background)
{
	CSize	src_size;

		if( !p_du ){
			fill_field( RGB(0,0,0) );
			return;//�f�[�^���Ȃ��Ƃ��͗̈�������h��Ԃ�
		}

		if( GetSrcSize(p_du, &src_size) ){
			DrawPiece(p_du, p_du->DataDispOffset, src_size , false , false , do_redraw_background);
		}
}

/********************************************************************
�@  �\  ��  �� : �摜�̈ꕔ����\������
��    ��    �� : DrawPiece
��          �� : CDataUnit		*p_du							(in)�摜������
				 CPoint			src_coor						(in)�摜�������̕`��̈�̍�����W
				 CSize			src_size						(in)�摜�������̕`��̈�̍����ƕ�
				 bool			do_adjust_scale_as_field_size	(in) true�@�ɂ���ƁC�c�����ImageFiled�̑傫���ɂ��킵�ĕ\������
																	false �ɂ���ƁC�w�肵��DataUnit (*p_du) �̃X�P�[�����O���ŕ\������
				 bool			do_update_scale					(in)do_adjust_scale_as_field_size��true�̂Ƃ��A
																	p_du�̃X�P�[�����O�����X�V���邩�ǂ���
				 bool			do_redraw_background			(in)�w�i���ĕ`�悷�邩�ǂ���
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CImageField::DrawPiece(CDataUnit *p_du, CPoint src_coor, CSize src_size, bool do_adjust_scale_as_field_size, bool do_update_scale, bool do_redraw_background)
{
	float	src_aspect_ratio, dest_aspect_ratio, scale;
	CSize	dest_size;
	CPoint	src_coor_dib;//DIB�`���ɂ������̉摜�]�����̍��W

		if(p_du == NULL){
			fill_field( RGB(0,0,0) );
			return;//�f�[�^���Ȃ��Ƃ���ImageFiled���������h��Ԃ�
		}

	//�摜�̏c���䒲��
		if( do_adjust_scale_as_field_size ){//ImageField�̑傫���ɂ��킷
			src_aspect_ratio  = (float)src_size.cx / (float)src_size.cy;//�摜�]������`�̈��	�@��/����
			dest_aspect_ratio = (float)FieldSize.cx / (float)FieldSize.cy;//ImageField�́@��/����

				if(  src_aspect_ratio == dest_aspect_ratio ){//�摜�]������`�̈��ImageField�́@��/���� �������ꍇ
					scale = (float)FieldSize.cx / (float)src_size.cx ;
					dest_size = FieldSize;

				}else  if(  src_aspect_ratio > dest_aspect_ratio  ){//�摜�]�����̈�̂ق���ImageField���������ł��遨�g���̃f�[�^�̕���ImageField�̕��ɂ��킹��
					scale = (float)FieldSize.cx / (float)src_size.cx ;
					dest_size = CSize( FieldSize.cx , (int)(FieldSize.cx / src_aspect_ratio) );

				}else{//�摜�]�����̈�̂ق���ImageField�����c���ł��遨�g���̃f�[�^�̍�����ImageFiled�̍����ɂ��킹��
					scale = (float)FieldSize.cy / (float)src_size.cy ;
					dest_size = CSize( (int)(FieldSize.cy * src_aspect_ratio) , FieldSize.cy );
				}

				if(do_update_scale)		ScaleImage(p_du,scale);//p_du->SetDispScale(scale);

		}else{//DataUnit (*p_du) �̃X�P�[�����O���ɂ��킷
			 GetDestSize(p_du, &dest_size);
		}

	//DIB�̍��W�ɕϊ�
	src_coor_dib = CPoint( src_coor.x ,  p_du->DataSize.cy - src_size.cy - src_coor.y);

	//�w�i�\��
		if( do_redraw_background )
			if( dest_size.cx < FieldSize.cx   ||   dest_size.cy < FieldSize.cy  ){
				//�摜�]����̋�`�̈悪ImageFiled��苷���Ƃ��C�w�i��h��Ԃ�
				fill_field( API.default_bg_color , &dest_size);
			}

	//�摜�]��
	HDC	hdc = ::GetDC( hImageWnd );
//		if(scale < 1)//�k������Ƃ�
			::SetStretchBltMode( hdc, HALFTONE);//���ԐF�ŕ�Ԃ���
	
			if((p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB == false){
				::StretchDIBits( hdc,	FieldCoordinate.x , FieldCoordinate.y , dest_size.cx , dest_size.cy ,
							src_coor_dib.x ,	 src_coor_dib.y , src_size.cx , src_size.cy,
							p_du->pDispBuffer,  p_du->pDispBufferBmpInfo,  DIB_RGB_COLORS,  SRCCOPY);

			}else{//RGB
				::StretchDIBits( hdc,	FieldCoordinate.x , FieldCoordinate.y , dest_size.cx , dest_size.cy ,
							src_coor_dib.x ,	 src_coor_dib.y , src_size.cx , src_size.cy,
							p_du->pDispBufferRGB,  p_du->pDispBufferBmpInfoRGB,  DIB_RGB_COLORS,  SRCCOPY);
			}
	::ReleaseDC(hImageWnd, hdc);
}

/********************************************************************
�@  �\  ��  �� : ImageFiled�̑傫���ɍ��킹�ĉ摜��\������
��    ��    �� : DrawAdjustedImage
��          �� : CDataUnit		*p_du					(in)�摜������
				 bool			do_update_scale			(in) p_du�̃X�P�[�����O�����X�V���邩�ǂ���
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CImageField::DrawAdjustedImage(CDataUnit *p_du, bool do_update_scale)
{
		if(p_du == NULL){
			fill_field( RGB(0,0,0) );
			return;//�f�[�^���Ȃ��Ƃ��͗̈�������h��Ԃ�
		}

	DrawPiece( p_du, CPoint(0,0), p_du->DataSize, true, do_update_scale, true);
}

/********************************************************************
�@  �\  ��  �� : ImageFiled����P��F�œh��Ԃ�
��    ��    �� : fill_field
��          �� : COLORREF		color 					(in)�h��Ԃ��F
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CImageField::fill_field( COLORREF color ,CSize* pDrawImgSize)
{
	CBrush	brush( color );
	CPen	pen(PS_SOLID,1,color);
	HDC		hdc = ::GetDC( hImageWnd );
	if(hdc==NULL)	return;

	HBRUSH	old_brush = (HBRUSH)::SelectObject(hdc, (HBRUSH)brush );
	HPEN old_pen = (HPEN)::SelectObject(hdc, (HPEN)pen);
	CRect	area = GetFieldRect();
	if (pDrawImgSize)
	{// ImageFiled������`�悷��摜�T�C�Y��������������
		if (area.Height() > pDrawImgSize->cy) 
		{
			::Rectangle(hdc, area.left, area.top + pDrawImgSize->cy, area.right, area.bottom);

			if (area.Width() > pDrawImgSize->cx)
			{
				::Rectangle(hdc, area.left + pDrawImgSize->cx, area.top, area.right, area.top + pDrawImgSize->cy);
			}
		}
		else {
			if (area.Width() > pDrawImgSize->cx) 
			{
				::Rectangle(hdc, area.left + pDrawImgSize->cx, area.top, area.right, area.bottom);
			}
		}
	}
	else
	{// ImageFiled���S��
		::Rectangle(hdc, area.left, area.top, area.right, area.bottom);
	}
	::SelectObject(hdc, old_brush );
	::SelectObject(hdc, old_pen);
	ReleaseDC( hImageWnd, hdc );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�\�������̕ύX
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : �摜�\���̊g�嗦��ύX����
��    ��    �� : ScaleImage
��          �� : CDataUnit		*p_du			(in)�摜������
				 float			scale			(in)�g�嗦
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
			 Y.Ikeda		 ���ݕ\������Ă��钆�S���W��ۂ����܂܊g�嗦�ύX
********************************************************************/
void CImageField::ScaleImage(CDataUnit *p_du , float scale)
{
		if( !p_du )	return;//�f�[�^��������΂Ȃɂ����Ȃ�


	//���݂̕\�����S���W���V�����\�����钆�S���W�ł�����
	double dispCenterX, dispCenterY;
	GetSrcCoor(p_du, FieldSize.cx/2 + FieldCoordinate.x, FieldSize.cy/2 + FieldCoordinate.y, &dispCenterX, &dispCenterY);

	//�V�����\������4�ύ��W
	double newDispLeft = dispCenterX - fabs(dispCenterX - p_du->DataDispOffset.x)*p_du->DispScale /scale ;
	double newDispTop = dispCenterY - fabs(dispCenterY - p_du->DataDispOffset.y)*p_du->DispScale /scale ;
	double newDispRight = dispCenterX + fabs(dispCenterX - p_du->DataDispOffset.x)*p_du->DispScale /scale ;
	double newDispBottom = dispCenterY + fabs(dispCenterY - p_du->DataDispOffset.y)*p_du->DispScale /scale ;

	//�\���I�t�Z�b�g���W�̐ݒ�
	if( newDispLeft>=0 && newDispRight < p_du->DataSize.cx){
		p_du->DataDispOffset.x = newDispLeft;
	}else{
		p_du->DataDispOffset.x = 0;
	}

	if( newDispTop>=0 && newDispBottom < p_du->DataSize.cy){
		p_du->DataDispOffset.y = newDispTop;
	}else{
		p_du->DataDispOffset.y = 0;
	}

	p_du->DispScale = scale;
}

/********************************************************************
�@  �\  ��  �� : �摜�̕\���͈͂��ړ�����
��    ��    �� : ScrollImage
��          �� : CDataUnit		*p_du			(in)�摜������
				 CPoint			poi				(in)�摜�̕\���͈͂̍�����W
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CImageField::ScrollImage(CDataUnit *p_du , CPoint poi)
{
	CSize	src_size;
		if( !p_du )	return;//�f�[�^��������΂Ȃɂ����Ȃ�
		
	GetSrcSize(p_du , &src_size);
		if(poi.x < 0   ||   poi.x > p_du->DataSize.cx - src_size.cx )			return;
		if(poi.y < 0   ||   poi.y > p_du->DataSize.cy - src_size.cy )			return;

	p_du->DataDispOffset = poi;
	//DrawImage(p_du, false);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�\���Ɋւ���f�[�^�擾�̂��߂̊֐�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : �摜�̕\���͈͂̍�����W�𓾂�
��    ��    �� : GetSrcOffset
��          �� : CDataUnit		*p_du			(in)�摜������
				 CPoint			*p_offset		(out)�摜�̕\���͈͂̍�����W
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CImageField::GetSrcOffset(CDataUnit *p_du, CPoint *p_offset)
{
		if( !p_du )	return false;//�f�[�^��������� false ���Ԃ�

	*p_offset = p_du->DataDispOffset;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �摜�̕\���͈͂̃T�C�Y�𓾂�
��    ��    �� : GetSrcSize
��          �� : CDataUnit		*p_du			(in)�摜������
				 CSize			*p_src_size		(out)�摜�̕\���͈͂̃T�C�Y
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CImageField::GetSrcSize(CDataUnit *p_du, CSize *p_src_size)
{
	CSize	result;

		if( !p_du )		return false;//�f�[�^���������false��������

		if( p_du->DataSize.cx * p_du->DispScale  <  FieldSize.cx ){//�g���̉摜�f�[�^�̕���ImageField�̕���菬�������摜�S�̂̕���\���ł���
			result.cx = p_du->DataSize.cx;

		}else{//�g���̃f�[�^�̕����\���̈�̕����傫�����摜�̈ꕔ��\��
			result.cx = (long)(FieldSize.cx / p_du->DispScale);
		}

		if( p_du->DataSize.cy * p_du->DispScale  <  FieldSize.cy ){//�g���̉摜�f�[�^�̍�����ImageField�̍�����菬�������摜�S�̂̍�����\���ł���
			result.cy = p_du->DataSize.cy;

		}else{//�g���̃f�[�^�̍������\���̈�̍������傫�����摜�̈ꕔ��\��
			result.cy = (long)(FieldSize.cy / p_du->DispScale);
		}

	*p_src_size = result;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �摜�̕\���͈͂̋�`�𓾂�
��    ��    �� : GetSrcSize
��          �� : CDataUnit		*p_du			(in)�摜������
				 CRect			*p_src_rect		(out)�摜�̕\���͈͂̋�`
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CImageField::GetSrcRect(CDataUnit *p_du, CRect *p_src_rect)
{
	CRect	result;
	CSize	src_size;
		if( !p_du )		return	false;//�f�[�^���������false��������

	GetSrcSize(p_du, &src_size);
	result.left = p_du->DataDispOffset.x;
	result.top  = p_du->DataDispOffset.y;
	result.right = p_du->DataDispOffset.x + src_size.cx - 1;
	result.bottom = p_du->DataDispOffset.y + src_size.cy - 1;

	*p_src_rect = result;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : ImageField���̉摜�\���̈�̃T�C�Y�𓾂�
��    ��    �� : GetDestSize
��          �� : CDataUnit		*p_du				(in)�摜������
				 CSize			*p_dest_size		(out)ImageField�̉摜�\���̈�̃T�C�Y
��    ��    �l : ����������true
�@          �\ : �u�摜�������̃T�C�Y�~�g�嗦����ImageField�̃T�C�Y�v�̂Ƃ��́AImageField�̃T�C�Y��Ԃ�
				 ����ȊO�́A�u�摜�������̃T�C�Y�~�g�嗦�̃T�C�Y�v��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CImageField::GetDestSize(CDataUnit *p_du, CSize *p_dest_size)
{
	CSize	result, scaled_src_size;

		if( !p_du )		return false;//�f�[�^���������false��������

	scaled_src_size.cx = (long)(p_du->DataSize.cx * p_du->DispScale);
		if(  scaled_src_size.cx <  FieldSize.cx ){//�g���̉摜�f�[�^�̕����\���̈�̕���菬������ImageField�̈ꕔ�ɕ\��
			result.cx = scaled_src_size.cx;

		}else{//�g���̉摜�f�[�^�̕����\���̈�̕����傫����ImageField�̕��S�̂ɕ\��
			result.cx = FieldSize.cx;
		}

	scaled_src_size.cy = (long)(p_du->DataSize.cy * p_du->DispScale);
		if(  scaled_src_size.cy <  FieldSize.cy ){//�g���̉摜�f�[�^�̍������\���̈�̍�����菬������ImageField�̈ꕔ�ɕ\��
			result.cy = scaled_src_size.cy;

		}else{//�g���̉摜�f�[�^�̍������\���̈�̍������傫����ImageField�̍����S�̂ɕ\��
			result.cy = FieldSize.cy;
		}

	*p_dest_size = result;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : ImageField���̉摜�\���̈�̋�`�𓾂�
��    ��    �� : GetDestRect
��          �� : CDataUnit		*p_du				(in)�摜������
				 CRect			*p_dest_rect		(out)ImageField���̉摜�\���̈�̋�`
��    ��    �l : ����������true
�@          �\ :�u �摜�������̃T�C�Y�~�g�嗦����ImageField�̃T�C�Y�v�̂Ƃ��́AImageField�̋�`��Ԃ�
				����ȊO�̂Ƃ��́A������W��ImageField�Ɠ����ŃT�C�Y���u�摜�������̃T�C�Y�~�g�嗦�v�ł����`��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CImageField::GetDestRect(CDataUnit *p_du, CRect *p_dest_rect)
{
	CRect	dest_rect;
	CSize	dest_size;

		if( !p_du )		return	false;//�f�[�^���������false��������

	GetDestSize(p_du, &dest_size);//�摜�]����̋�`�̈�̃T�C�Y���擾����
	dest_rect.left = FieldCoordinate.x;
	dest_rect.top = FieldCoordinate.y;
	dest_rect.right = FieldCoordinate.x + dest_size.cx - 1;
	dest_rect.bottom  = FieldCoordinate.y + dest_size.cy - 1;

	*p_dest_rect = dest_rect;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : ImageField�̋�`�𓾂�
��    ��    �� : GetFieldRect
��          �� : 
��    ��    �l : ImageField�̋�`
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CRect	CImageField::GetFieldRect()
{
	return	CRect(	FieldCoordinate.x ,						FieldCoordinate.y ,
					FieldCoordinate.x + FieldSize.cx -1 ,	FieldCoordinate.y + FieldSize.cy - 1 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�摜�]���O��̍��W�ϊ��Ɋւ���֐�
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : ImageField���̍��W����摜���̍��W�ɕϊ�����
��    ��    �� : GetSrcCoor
��          �� : CDataUnit		*p_du				(in)�摜������
				 double		dest_coor_x, dest_coor_y	(in)���C����ʂ̍��W�n
				 double		*p_src_coor_x, *p_src_coor_y	(out)�摜���W
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CImageField::GetSrcCoor(CDataUnit *p_du,  double dest_coor_x, double dest_coor_y , double *p_src_coor_x, double *p_src_coor_y)
{
		if( !p_du )	return false;//�f�[�^���������false��������

	*p_src_coor_x = (double)( dest_coor_x  -  FieldCoordinate.x ) / p_du->DispScale  + p_du->DataDispOffset.x - 0.5;
	*p_src_coor_y = (double)( dest_coor_y  -  FieldCoordinate.y ) / p_du->DispScale  + p_du->DataDispOffset.y - 0.5;


		//�摜�]�����̗̈���ł͂Ȃ�
		if(*p_src_coor_x<-0.5  ||  *p_src_coor_x>(double)p_du->DataSize.cx-0.5)		return	false;
		if(*p_src_coor_y<-0.5  ||  *p_src_coor_y>(double)p_du->DataSize.cy-0.5)		return	false;
	
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �摜���̍��W����ImageField���̍��W�ɕϊ�����
��    ��    �� : GetDestCoor
��          �� : CDataUnit		*p_du				(in)�摜������
				 double		src_coor_x, src_coor_y		(in)�摜���W
				 double		*p_dest_coor_x, *p_dest_coor_y	(out)���C����ʂ̍��W�n
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CImageField::GetDestCoor(CDataUnit *p_du, double src_coor_x , double src_coor_y , double *p_dest_coor_x, double *p_dest_coor_y)
{
		if( !p_du )	return false;//�f�[�^���������false��������

	*p_dest_coor_x = (double)(src_coor_x + 0.5 - p_du->DataDispOffset.x) * p_du->DispScale  +  FieldCoordinate.x;
	*p_dest_coor_y = (double)(src_coor_y + 0.5 - p_du->DataDispOffset.y) * p_du->DispScale  +  FieldCoordinate.y;

		//�摜�]����̗̈���ł͂Ȃ�
		if(*p_dest_coor_x< (double)FieldCoordinate.x-0.5  ||  *p_dest_coor_x > (double)(FieldCoordinate.x + FieldSize.cx)-0.5 )		return	false;
		if(*p_dest_coor_y< (double)FieldCoordinate.y-0.5  ||  *p_dest_coor_y > (double)(FieldCoordinate.y + FieldSize.cy)-0.5 )		return	false;
	
	return	true;
}

