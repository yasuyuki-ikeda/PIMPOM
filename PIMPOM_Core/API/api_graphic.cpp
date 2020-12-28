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
//���C����ʂւ̕`��
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
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

//�̈�̐��������̕� (���� s1s2) �� ���� p1p2 �̌�_�����߂�
//��_��������΁@false��Ԃ�
static bool	get_closs_secion_with_h_side(double s1x, //(in)�_s1��x���W
										 double s1y, //(in)�_s1��y���W
										double s2x, //(in)�_s2��x���W
										double s2y, //(in)�_s2��y���W
										double p1x, //(in)�_p1��x���W
										double p1y, //(in)�_p1��y���W
										double p2x, //(in)�_p2��x���W
										double p2y, //(in)�_p2��y���W
										double *pCx, //(out)��_��x���W
										double *pCy  //(out)��_��y���W
										)
{
	double	cross_section_xf;//*��_��x���W
	double	h_side_yf;//���������̕� (���� s1s2) ��y���W �� ��_��y���W;

	h_side_yf = s1y;//���������̕� (���� s1s2) ��y���W

	//p1,p2���ӂ��������������ɑ΂��ē������ɂ���@���@���������̕�(���� s1s2)�Ɛ���p1p2�͌����Ȃ�
	if(  (p1y - h_side_yf) * (p2y - h_side_yf) > 0 )
	{//��_�Ȃ�
		return false;
	}
		
	//��_��x���W�����߂�
	cross_section_xf = ( p2x - p1x ) * ( h_side_yf -  p1y ) / ( p2y - p1y ) + p1x;

	//s1,s2��p1p2���������������ɑ΂��A�������ɂ�@���@���������̕�(���� s1s2)�Ɛ���p1p2�͌����Ȃ�
	if( (s1x - cross_section_xf) * (s2x - cross_section_xf) > 0)
	{
		return false;
	}


	*pCx = cross_section_xf;
	*pCy = h_side_yf;

	return true;
}


//�̈�̐��������̕� (���� s1s2) �� ���� p1p2 �̌�_�����߂�
//��_��������΁@false��Ԃ�
static bool	get_closs_secion_with_v_side(double s1x, //(in)�_s1��x���W
										 double s1y, //(in)�_s1��y���W
										double s2x, //(in)�_s2��x���W
										double s2y, //(in)�_s2��y���W
										double p1x, //(in)�_p1��x���W
										double p1y, //(in)�_p1��y���W
										double p2x, //(in)�_p2��x���W
										double p2y, //(in)�_p2��y���W
										double *pCx, //(out)��_��x���W
										double *pCy  //(out)��_��y���W
										)
{
	double	cross_section_yf;//*��_��y���W
	double	h_side_xf;//���������̕� (���� s1s2) ��x���W �� ��_��y���W;

	h_side_xf = s1x;//���������̕� (���� s1s2) ��x���W

	//p1,p2���ӂ��������������ɑ΂��ē������ɂ���@���@���������̕�(���� s1s2)�Ɛ���p1p2�͌����Ȃ�
	if(  (p1x - h_side_xf) * (p2x - h_side_xf) > 0 )
	{
		return false;
	}
		
	//��_��y���W�����߂�
	cross_section_yf = ( p2y - p1y ) * ( h_side_xf -  p1x ) / ( p2x - p1x ) + p1y;

	//s1,s2��p1p2���������������ɑ΂��A�������ɂ�@���@���������̕�(���� s1s2)�Ɛ���p1p2�͌����Ȃ�
	if( (s1y - cross_section_yf) * (s2y - cross_section_yf) > 0)
	{
		return false;
	}

	*pCx = h_side_xf;
	*pCy = cross_section_yf;

	return true;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�摜�\���̍��W�Ɋւ��铮��
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : ���C����ʂ̍��W�n����摜���W�ɕϊ�����
��    ��    �� : GetSrcCoor
��          �� : double		dest_coor_x, dest_coor_y	(in)���C����ʂ̍��W�n
				 double		*p_src_coor_x, *p_src_coor_y	(out)�摜���W
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetSrcCoor( double dest_coor_x, double dest_coor_y , double *p_src_coor_x, double *p_src_coor_y)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );
	CImageField	*p_image = pPIMMOMDlg->GetMainImage();

		if(!p_du)		return	false;
		if(!p_image)	return	false;

	return	p_image->GetSrcCoor( p_du, dest_coor_x, dest_coor_y, p_src_coor_y, p_src_coor_y);
}


/********************************************************************
�@  �\  ��  �� : �摜���W���烁�C����ʂ̍��W�n�ɕϊ�����
��    ��    �� : GetDestCoor
��          �� : double		src_coor_x, src_coor_y		(in)�摜���W
				 double		*p_dest_coor_x, *p_dest_coor_y	(out)���C����ʂ̍��W�n
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//�摜�]���O�̍��W�i�摜���j����摜�]����i�E�C���h�E���j�̍��W�ɕϊ�����
bool	CPimpomAPI::GetDestCoor(double src_coor_x , double src_coor_y , double *p_dest_coor_x, double *p_dest_coor_y)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );
	CImageField	*p_image = pPIMMOMDlg->GetMainImage();

		if(!p_du)		return	false;
		if(!p_image)	return	false;

	return	p_image->GetDestCoor( p_du, src_coor_x, src_coor_y, p_dest_coor_x, p_dest_coor_y);
}


///////////////////////////////////////////////////////////////////////////////////////
//�摜�\���֘A
///////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : �\���摜�̔Z�x�̃����W��ݒ肵�Ă���摜�\��
��    ��    �� : SetDispRange
��          �� : long		number		(in)�摜�������ԍ�
				float		min			(in)�Z�x�ŏ��l
				float		max			(in)�Z�x�ő�l
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool		CPimpomAPI::SetDispRange(long number, float min , float max)
{
		if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	CDataUnit	*p_du = GetDataUnit(number);
		if(p_du == NULL)	return	false;//�f�[�^�������Ƃ��͂Ȃɂ����Ȃ�

		if( !p_du->SetDispRange( min, max) )	return	false;//�\�������W�̕ύX�Ɏ��s

	DrawImage( number, true);
	return	true;
}


/********************************************************************
�@  �\  ��  �� : �\���摜�̔Z�x�̃����W�������Őݒ肵�Ă���摜�\��
��    ��    �� : SetDispRangeAuto
��          �� : long		number		(in)�摜�������ԍ�
				int			type		(in)�����ݒ�^�C�v   0:�ő�/�ŏ� 1:���ϒl�}�W���΍��~2
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool		CPimpomAPI::SetDispRangeAuto(long number, int type)
{
		if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	CDataUnit	*p_du = GetDataUnit(number);
		if(p_du == NULL)	return	false;//�f�[�^�������Ƃ��͂Ȃɂ����Ȃ�

	float min, max;
	if(type==0){
		p_du->GetDataMinMax(&min,&max);
	}else{
		if(!pHistgramDlg->set_min_max_from_mid_width( p_du->Average , p_du->Stdiv * 2 , &min, &max) )	return false;
	}
		if( !p_du->SetDispRange( min, max) )	return	false;//�\�������W�̕ύX�Ɏ��s

	DrawImage( number, true);
	return	true;
}


/********************************************************************
�@  �\  ��  �� : �摜�\��
��    ��    �� : DrawImage
��          �� : long		number		(in)�摜�������ԍ�
				bool		do_update	(in)�\���f�[�^�̍X�V���邩
				bool		do_adjust	(in)�\���T�C�Y�����C����ʃT�C�Y�ɂ��킹�邩
				bool redraw_background  (in)�w�i�ĕ`�悷�邩
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawImage(long number, bool do_update , bool do_adjust, bool redraw_background)
{
		if(number == CURRENT_IMAGE)		number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ
	pDataDlg->ChangeCurrentDataNumber( number );//�I�𒆂̉摜��ύX���� 

	CDataUnit	*p_du = GetDataUnit(number);
	if(p_du)
	{
		if(do_update){	
			p_du->UpdateDispBuffer();//�\���摜�̍X�V�������Ȃ�
		}

		if( do_adjust){
			pPIMMOMDlg->p_image_disp_main->DrawAdjustedImage(p_du,do_adjust);//�k�ڕύX���ĕ`�悷��
		}
	}
	
	pPIMMOMDlg->DrawImage(redraw_background);
	pDataDlg->DrawImage(number);
	pHistgramDlg->DrawHistgram();//�q�X�g�O������`�悷��
	pMarkDlg->ShowMarkVal( number );//�}�[�N��\������

	if(pMultiImageDispDlg){//�����摜�\��
		pMultiImageDispDlg->DispMultiImage();
	}

	if (p3DImgDispDlg){//3D�摜�ꗗ�\��
		p3DImgDispDlg->DispImage();
	}
}

void	CPimpomAPI::DrawImage(long number, CString name, bool do_update, bool do_adjust)
{
	CDataUnit	*p_du = GetDataUnit(number);
	if(p_du)	p_du->DataName = name;

	DrawImage(number, do_update, do_adjust);
}

/********************************************************************
�@  �\  ��  �� : ���C����ʕ\���摜�X�P�[���ύX
��    ��    �� : ScaleImage
��          �� : long		number		(in)�摜�������ԍ�
				�@float scale			(in)�X�P�[���@�i1/100�`100�͈̔́j
��    ��    �l : ��������� true
�@          �\ : �X�P�[���ύX�̂̂��ĕ`�悷��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ScaleImage(long number, float scale)
{
	CDataUnit *p_du = GetDataUnit( number );
	if(p_du == NULL)	return false;//���ݑI�𒆂̉摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�

	if(scale < 0.01 || scale > 100)	return false;//�{����1/100�`100

	pPIMMOMDlg->p_image_disp_main->ScaleImage(p_du,scale);

	DrawImage(number);//�ĕ`��

	return true;
}

/********************************************************************
�@  �\  ��  �� : �摜�\���i���C����ʂ̂݁j
��    ��    �� : DrawCurrentImageFast
��          �� : bool		do_update	(in)�\���f�[�^�̍X�V���邩
��    ��    �l : ��������� true
�@          �\ : ���ݑI�𒆂̉摜��\������B���C����ʂ݂̂ł��邽��,���������ɂȂ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void		CPimpomAPI::DrawCurrentImageFast(bool do_update )
{
	long	number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

		if(do_update){	//�\���摜�̍X�V�������Ȃ��ꍇ
			CDataUnit	*p_du = GetDataUnit(number);
				if(p_du)	p_du->UpdateDispBuffer();//�f�[�^�������Ƃ��͍X�V���Ȃ�
		}
	pPIMMOMDlg->DrawImage(false);//���C����ʂ�`�悷��
}


/********************************************************************
�@  �\  ��  �� : �\���摜�̃J���[�p���b�g��ύX����
��    ��    �� : ChangePalette
��          �� : long		number		(in)�摜�������ԍ�
				short			palette_id		(in)�J���[�p���b�g��ID
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
bool		CPimpomAPI::ChangePalette(int number, short palette_id)
{
	CDataUnit *pdu = GetDataUnit(number);

	if (!pdu)	return false;

	pdu->ChangePalette(palette_id);

	return true;
}


///////////////////////////////////////////////////////////////////////////////////
//�}�`�`��Ɋւ��鏈��
/////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : ����ax + by + c=0����`�̈�ŃN���b�v����
��    ��    �� : ClipSegmentF
��          �� : CRect		region				(in)��`�̈�
				 double		a,b,c				(in)�����p�����^
				 double		p0x, p0y, p1x, p1y	(out)���������n�_
��    ��    �l : ��������� true
�@          �\ : �����������̈���Ɋ܂܂�Ȃ��ꍇ��false ��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ClipSegmentF( CRect region , double a, double b, double c, double *p0x, double *p0y, double *p1x, double *p1y)
{
	//�̈���ɒ����������Ă��邩�ǂ����`�F�b�N
	POINT ptLU, ptRU, ptRB, ptLB;
	ptLU.x = region.left;	ptLU.y = region.top;//�̈捶��
	ptRU.x = region.right;	ptRU.y = region.top;//�̈�E��
	ptRB.x = region.right;	ptRB.y = region.bottom;//�̈�E��
	ptLB.x = region.left;	ptLB.y = region.bottom;//�̈捶��


	double clossPointX[2], clossPointY[2];
	int crossCnt=0;//��_��

	if( (a * (double)ptLU.x + b * (double)ptLU.y + c) * (a * (double)ptRU.x + b * (double)ptRU.y + c) < 0 )		//�������̈�[ ��[�i����-�E��j�ƌ����
	{
		clossPointY[crossCnt] = ptLU.y;
		clossPointX[crossCnt] = (-c - b*(double)ptLU.y )/a;
		crossCnt++;
	}

	if( (a * (double)ptRB.x + b * (double)ptRB.y + c) * (a * (double)ptLB.x + b * (double)ptLB.y + c) < 0 )		//�������̈�[ ����-�E���ƌ����
	{
		clossPointY[crossCnt] = ptLB.y;
		clossPointX[crossCnt] = (-c - b*(double)ptLB.y )/a;
		crossCnt++;
	}

	if( (a * (double)ptLU.x + b * (double)ptLU.y + c) * (a * (double)ptLB.x + b * (double)ptLB.y + c) < 0 )		//�������̈�[ ����-�����ƌ����
	{
		clossPointX[crossCnt] = ptLU.x;
		clossPointY[crossCnt] = (-c - a*(double)ptLU.x )/b;
		crossCnt++;
	}

	if( (a * (double)ptRU.x + b * (double)ptRU.y + c) * (a * (double)ptRB.x + b * (double)ptRB.y + c) < 0 )		//�������̈�[ �E��-�E���ƌ����
	{
		clossPointX[crossCnt] = ptRB.x;
		clossPointY[crossCnt] = (-c - a*(double)ptRB.x )/b;
		crossCnt++;
	}

	if(crossCnt!=2)	return false;

	*p0x = clossPointX[0];
	*p0y = clossPointY[0];
	*p1x = clossPointX[1];
	*p1y = clossPointY[1];

	return true;
}




/********************************************************************
�@  �\  ��  �� : ������������`�̈�ŃN���b�s���O
��    ��    �� : ClipSegment
��          �� : CRect		region		(in)��`�̈�
				 double		p0x, p0y	(in/out)���������n�_
				 CPoint		p1x, p1y	(in/out)���������I�_
��    ��    �l : ��������� true
�@          �\ : �����������̈���Ɋ܂܂�Ȃ��ꍇ��false ��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ClipSegment( CRect region , double *p0x, double *p0y, double *p1x, double *p1y )
{
	double  cross_section_x, cross_section_y, vertex1x, vertex1y, vertex2x, vertex2y, vertex3x, vertex3y, vertex4x, vertex4y;//��`�̈�̊e���_
	double  clipped_vertex_x[2], clipped_vertex_y[2];//������`�悷�钸�_

	int		vertex_counter = 0;//���肵�����_�̐��i2�ɂȂ�����`��J�n�j

		if(*p0x == *p1x && *p0y == *p1y)		return	false;//2�_���������W�̏ꍇ�͐؂���������Ȃ�Ȃ�

	//�̈���ɓ����Ă���_��T��
		if(	*p0x > region.left  &&		
			*p0x < region.right  &&
			*p0y > region.top  &&
			*p0y < region.bottom  )//p0���̈���ł���
		{
			clipped_vertex_x[vertex_counter] = *p0x;
			clipped_vertex_y[vertex_counter] = *p0y;
			vertex_counter++;
		}

		if(	*p1x > region.left  &&		
			*p1x < region.right  &&
			*p1y > region.top  &&
			*p1y < region.bottom  )//p1���̈���ł���
		{
			clipped_vertex_x[vertex_counter] = *p1x;
			clipped_vertex_y[vertex_counter] = *p1y;
			vertex_counter++;
		}

				if(vertex_counter==2)	goto	L_DET_VERT;//�`�悷�������2���_�����܂���


	//�����؂���
	vertex1x = region.left ;
	vertex2x = region.right;
	vertex3x = region.right;
	vertex4x = region.left ;
	vertex1y = region.top ;
	vertex2y = region.top ;
	vertex3y = region.bottom ;
	vertex4y = region.bottom ;

	//��̕ӂ�2�_�����Ԓ����Ƃ̌�_�𒲂ׂ�
	if( get_closs_secion_with_h_side( vertex1x, vertex1y, vertex2x, vertex2y, *p0x, *p0y, *p1x, *p1y, &cross_section_x, &cross_section_y)  ){
		clipped_vertex_x[ vertex_counter ] = cross_section_x;
		clipped_vertex_y[ vertex_counter ] = cross_section_y;
		vertex_counter++;
				
		if(vertex_counter==2)	goto	L_DET_VERT;//�`�悷�������2���_�����܂���
	}

	//�E�̕ӂ�2�_�����Ԓ����Ƃ̌�_�𒲂ׂ�
	if( get_closs_secion_with_v_side( vertex2x, vertex2y, vertex3x, vertex3y, *p0x, *p0y, *p1x, *p1y, &cross_section_x, &cross_section_y)  ){
		clipped_vertex_x[ vertex_counter ] = cross_section_x;
		clipped_vertex_y[ vertex_counter ] = cross_section_y;
		vertex_counter++;
				
		if(vertex_counter==2)	goto	L_DET_VERT;//�`�悷�������2���_�����܂���
	}

	//���̕ӂ�2�_�����Ԓ����Ƃ̌�_�𒲂ׂ�
	if( get_closs_secion_with_h_side( vertex3x, vertex3y, vertex4x, vertex4y, *p0x, *p0y, *p1x, *p1y, &cross_section_x, &cross_section_y)  ){
		clipped_vertex_x[ vertex_counter ] = cross_section_x;
		clipped_vertex_y[ vertex_counter ] = cross_section_y;
		vertex_counter++;
				
		if(vertex_counter==2)	goto	L_DET_VERT;//�`�悷�������2���_�����܂���
	}

	//���̕ӂ�2�_�����Ԓ����Ƃ̌�_�����߂�
	if( get_closs_secion_with_v_side( vertex4x, vertex4y, vertex1x, vertex1y, *p0x, *p0y, *p1x, *p1y, &cross_section_x, &cross_section_y)  ){
		clipped_vertex_x[ vertex_counter ] = cross_section_x;
		clipped_vertex_y[ vertex_counter ] = cross_section_y;
		vertex_counter++;
				
		if(vertex_counter==2)	goto	L_DET_VERT;//�`�悷�������2���_�����܂���
	}


	//�Ō�܂�2�̒��_�����܂�Ȃ������ꍇ
	if( vertex_counter < 2 )	return	false;//��������`�̈���Ɋ܂܂�Ȃ�

	//2���_�����肵���ꍇ
L_DET_VERT:
		*p0x = clipped_vertex_x[ 0 ];
		*p0y = clipped_vertex_y[ 0 ];
		*p1x = clipped_vertex_x[ 1 ];
		*p1y = clipped_vertex_y[ 1 ];

		return	true;
}


/********************************************************************
�@  �\  ��  �� : ��`�̈���ɒ���������`�悷��i�̈�O�̓N���b�s���O�j
��    ��    �� : DrawClippedSegment
��          �� : CDC		*p_dc		(in)
				 CRect		region		(in)��`�̈�
				 double		p0x,p0y		(in)���������n�_
				 double		p1x,p1y		(in)���������I�_
				 int		size		(in)����
				 COLORREF	col			(in)�`��F
��    ��    �l : ��������� true
�@          �\ : �����������̈���Ɋ܂܂�Ȃ��ꍇ��false ��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::DrawClippedSegment(CDC *p_dc, CRect region , double p0x, double p0y, double p1x, double p1y, int size, COLORREF col)
{
	CPen	pen, *poldpen;
		if( !ClipSegment(region, &p0x, &p0y, &p1x, &p1y) )		return	false;//������������`�̈���Ɋ܂܂�Ȃ�

	pen.CreatePen(PS_SOLID , size , col );
	poldpen = p_dc->SelectObject(&pen);
	p_dc->MoveTo( (int)(p0x+0.5), (int)(p0y+0.5) );
	p_dc->LineTo( (int)(p1x+0.5), (int)(p1y+0.5) );
	p_dc->SelectObject(poldpen);
	pen.DeleteObject();

	return	true;
}

bool	CPimpomAPI::DrawClippedSegment(CDC *p_dc, CRect region , double p0x, double p0y, double p1x, double p1y)
{
	return DrawClippedSegment(p_dc, region ,p0x, p0y, p1x ,p1y, 1, default_fg_color);//�W���F�ŕ`�悷��
}





/********************************************************************
�@  �\  ��  �� : ��`�̈���ɋ�`��`�悷��i�̈�O�̓N���b�s���O�j
��    ��    �� : DrawClippedRect
��          �� : CDC		*p_dc		(in)
				 CRect		region		(in)��`�̈�
				 CRect		draw_rect	(in)�`�悷���`
				 int		size		(in)����
				 COLORREF	col			(in)�`��F
��    ��    �l : ��������� true
�@          �\ : �`���`���̈���Ɋ܂܂�Ȃ��ꍇ��false ��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::DrawClippedRect(CDC *p_dc, CRect region , CRect draw_rect, int size, COLORREF col)
{
	bool	result = false;
	CPoint	cross_section, vertex1, vertex2, vertex3, vertex4;//��`�̈�̊e���_

	vertex1 = CPoint( draw_rect.left , draw_rect.top );
	vertex2 = CPoint( draw_rect.right, draw_rect.top );
	vertex3 = CPoint( draw_rect.right, draw_rect.bottom );
	vertex4 = CPoint( draw_rect.left , draw_rect.bottom );

		if(DrawClippedSegment(p_dc, region, vertex1.x , vertex1.y , vertex2.x , vertex2.y ,size, col) )	result = true;
		if(DrawClippedSegment(p_dc, region, vertex2.x , vertex2.y , vertex3.x , vertex3.y ,size, col) )	result = true;
		if(DrawClippedSegment(p_dc, region, vertex3.x , vertex3.y , vertex4.x , vertex4.y ,size, col) )	result = true;
		if(DrawClippedSegment(p_dc, region, vertex4.x , vertex4.y , vertex1.x , vertex1.y ,size, col) )	result = true;

	return	result;
}

bool	CPimpomAPI::DrawClippedRect(CDC *p_dc, CRect region , CRect draw_rect)
{
	return DrawClippedRect(p_dc, region , draw_rect, 1, default_fg_color);//�W���F�ŕ`�悷��
}


/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ɒ����`��
��    ��    �� : DrawLine
��          �� : double		x0,y0		(in)�n�_���W�i�摜���W�Ŏw��j
				 double		x1,y1		(in)�I�_���W�i�摜���W�Ŏw��j
				 int		pen_size	(in)�`�悷��y���T�C�Y
				 COLORREF	col			(in)�`��F
				 int		width		(in)����
��    ��    �l : 
�@          �\ : ������1���傫���l���w�肷��ƁA���������̈�ɂȂ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawLine(double x0, double y0, double x1, double y1, int pen_size, COLORREF col, int width)
{
	CImageField  *p_if;
	CDC			 *p_dc;
	CDataUnit	 *p_du;
	CRect		dest_rect;
	double		dest_point1x, dest_point1y, dest_point2x, dest_point2y;

	if( (p_du = GetDataUnit(CURRENT_IMAGE))==NULL)	return;//�I�𒆂̉摜�������Ƀf�[�^��������΂��������͉������Ȃ�
	p_dc = pPIMMOMDlg->GetDC();//���C���E�C���h�E�̃f�o�C�X�R���e�L�X�g�擾
	p_if = pPIMMOMDlg->GetMainImage();//���C����ʂ̉摜�\�������擾
	p_if->GetDestRect(p_du, &dest_rect);//���C����ʂ̉摜�\����`�̈���擾
	p_if->GetDestCoor( p_du , x0, y0 , &dest_point1x, &dest_point1y );//���C����ʂł̍��W���擾
	p_if->GetDestCoor( p_du , x1, y1 , &dest_point2x, &dest_point2y );//���C����ʂł̍��W���擾
	DrawClippedSegment(p_dc, dest_rect, dest_point1x, dest_point1y, dest_point2x, dest_point2y, pen_size, col);//����`��
	pPIMMOMDlg->ReleaseDC(p_dc);

	if(width>1){//����
		CPoint point[4];
		FIGURE_LINE line;

		line.x1=(int)(x0+0.5);
		line.y1=(int)(y0+0.5);
		line.x2=(int)(x1+0.5);
		line.y2=(int)(y1+0.5);
		line.w = width;
		CWorkArea::GetLineVertxes(line, point);
		
		DrawLine(point[0].x, point[0].y, point[1].x, point[1].y, pen_size, col, 1);
		DrawLine(point[1].x, point[1].y, point[2].x, point[2].y, pen_size, col, 1);
		DrawLine(point[2].x, point[2].y, point[3].x, point[3].y, pen_size, col, 1);
		DrawLine(point[3].x, point[3].y, point[0].x, point[0].y, pen_size, col, 1);
	}
}


void	CPimpomAPI::DrawLine(double x0, double y0, double x1, double y1, int width)
{
	DrawLine(x0, y0, x1, y1, 1, default_fg_color, width);
}





/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ɏ\���J�[�\���`��
��    ��    �� : DrawCursor
��          �� : double		x,y			(in)���W�i�摜���W�Ŏw��j
				 int		pen_size	(in)�`�悷��y���T�C�Y
				 COLORREF	col			(in)�`��F
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawCursor(double x, double y, int pen_size, COLORREF col)
{
	long		mark_length = 20;//�}�[�N�̃��C���̒���

	DrawLine((double)(x - mark_length/2) , y , (double)(x + mark_length/2) , y, pen_size, col);
	DrawLine(x , (double)(y - mark_length/2) , x , (double)(y + mark_length/2), pen_size, col);
}

void	CPimpomAPI::DrawCursor(double x, double y)
{
	DrawCursor(x, y, 1, default_fg_color);
}


/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ɋ�`�`��
��    ��    �� : DrawRect
��          �� : double		left		(in)���Ӎ��W�i�摜���W�Ŏw��j
				double			top			(in)�E�Ӎ��W�i�摜���W�Ŏw��j
				double			right		(in)��Ӎ��W�i�摜���W�Ŏw��j
				double			bottom		(in)���Ӎ��W�i�摜���W�Ŏw��j
				 int		pen_size	(in)�`�悷��y���T�C�Y
				 COLORREF	col			(in)�`��F
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawRect(double left, double top, double right, double bottom, int pen_size, COLORREF col)
{
	DrawLine(left, top, right, top, pen_size, col);
	DrawLine(left, bottom, right, bottom, pen_size, col);
	DrawLine(left, top, left, bottom, pen_size, col);	
	DrawLine(right, top, right, bottom, pen_size, col);
}

void	CPimpomAPI::DrawRect(double left, double top, double right, double bottom)
{
	DrawRect(left, top, right, bottom, 1, default_fg_color);
}


/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ɉ~�`��
��    ��    �� : DrawCircle
��          �� : double		x,y			(in)���S���W�i�摜���W�Ŏw��j
				double		r			(in)���a�i�摜���W�Ŏw��j
				 int		pen_size	(in)�`�悷��y���T�C�Y
				 COLORREF	col			(in)�`��F
				 int		width		(in)����
��    ��    �l : 
�@          �\ : ������1���傫���l���w�肷��ƁA�����~���̈�ɂȂ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawCircle(double x, double y, double r, int pen_size, COLORREF col, int width)
{
	DrawEllipse(x,y,r,r,pen_size,col,width);
}

void	CPimpomAPI::DrawCircle(double x, double y, double r, int width)
{
	DrawCircle( x, y, r, 1, default_fg_color, width);
}


/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ɑȉ~�`��
��    ��    �� : DrawEllipse
��          �� : double		x,y			(in)���S���W�i�摜���W�Ŏw��j
				double		rx,yx		(in)X,Y�����̔��a�i�摜���W�Ŏw��j
				 int		pen_size	(in)�`�悷��y���T�C�Y
				 COLORREF	col			(in)�`��F
				 int		width		(in)����
��    ��    �l : 
�@          �\ : ������1���傫���l���w�肷��ƁA�����ȉ~���̈�ɂȂ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawEllipse(double x, double y, double rx, double ry, int pen_size, COLORREF col, int width)
{
	CImageField  *p_if;
	CDataUnit	 *p_du;
	CDC			 *p_dc;
	double		dest_rx, dest_ry;
	CRect		dest_rect;
	double		dest_point_x, dest_point_y;
	CPen	pen, *poldpen;
	CBrush	 *poldbrush;
			
			if( (p_du = GetDataUnit(CURRENT_IMAGE))==NULL)	return;//�I�𒆂̉摜�������Ƀf�[�^��������΂��������͉������Ȃ�

	p_dc = pPIMMOMDlg->GetDC();//���C���E�C���h�E�̃f�o�C�X�R���e�L�X�g�擾
	p_if = pPIMMOMDlg->GetMainImage();//���C����ʂ̉摜�\�������擾
	pen.CreatePen(PS_SOLID , pen_size , col);
	poldpen = (CPen*)p_dc->SelectObject(&pen);
	poldbrush = (CBrush*)p_dc->SelectStockObject(NULL_BRUSH);

	p_if->GetDestRect(p_du, &dest_rect);//���C����ʂ̉摜�\����`�̈���擾
	p_if->GetDestCoor( p_du , x, y , &dest_point_x, &dest_point_y );//���C����ʂł̍��W���擾
	dest_rx = p_du->DispScale * rx;
	dest_ry = p_du->DispScale * ry;

	p_dc->Ellipse((int)(dest_point_x-dest_rx+0.5), (int)(dest_point_y-dest_ry+0.5), (int)(dest_point_x+dest_rx+0.5), (int)(dest_point_y+dest_ry+0.5));
	p_dc->SelectObject(poldpen);
	p_dc->SelectObject(poldbrush);
	pPIMMOMDlg->ReleaseDC(p_dc);
	pen.DeleteObject();

	if(width>1){
		if( ((double)width/2.0 < rx)  &&  ((double)width/2.0 < ry)  )
		{
			DrawEllipse(x, y, rx-(double)width/2.0, ry-(double)width/2.0);
		}
	
		DrawEllipse(x, y, rx+(double)width/2.0, ry+(double)width/2.0);
	}
}

void	CPimpomAPI::DrawEllipse(double x, double y, double rx, double ry)
{
	DrawEllipse( x, y, rx, ry, 1, default_fg_color);
}


/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ƀe�L�X�g�`��
��    ��    �� : DrawText
��          �� : double		x,y			(in)�`����W�i�摜���W�Ŏw��j
				 CString	str,		(in)�e�L�X�g
				 COLORREF	col			(in)�`��F
				 int		size		(in)�����T�C�Y
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawText(double x, double y, CString str, COLORREF col, int size)
{
	CImageField  *p_if;
	CDataUnit	 *p_du;
	CRect		dest_rect;
	double		dest_point_x, dest_point_y;
	CDC			*p_dc;
	CFont		font, *poldfont;
	LOGFONT		lf;
	COLORREF	oldcol;

			if( (p_du = GetDataUnit(CURRENT_IMAGE))==NULL)	return;//�I�𒆂̉摜�������Ƀf�[�^��������΂��������͉������Ȃ�

	memcpy(&lf,&default_font,sizeof(LOGFONT));
	if(size>0){		lf.lfHeight = size;	}/*lf.lfHeight=80;*/
	font.CreateFontIndirect(&lf);

	p_dc = pPIMMOMDlg->GetDC();//���C���E�C���h�E�̃f�o�C�X�R���e�L�X�g�擾
	p_if = pPIMMOMDlg->GetMainImage();//���C����ʂ̉摜�\�������擾
	oldcol = p_dc->SetTextColor(col);
	poldfont = (CFont*)p_dc->SelectObject(&font);
	p_if->GetDestRect(p_du, &dest_rect);//���C����ʂ̉摜�\����`�̈���擾
	p_if->GetDestCoor( p_du , x, y , &dest_point_x, &dest_point_y );//���C����ʂł̍��W���擾
	
	//���C����ʂ���͂ݏo���Ȃ��悤�ɂ���
	if(	(int)(dest_point_x+0.5) >= MAIN_WIN_LEFT_MARGIN && 
		(int)(dest_point_x+0.5) + lf.lfHeight < MAIN_WIN_LEFT_MARGIN + main_image_size.cx && 
		(int)(dest_point_y+0.5) >= MAIN_WIN_TOP_MARGIN && 
		(int)(dest_point_y+0.5) + lf.lfWidth < MAIN_WIN_TOP_MARGIN + main_image_size.cy)
	{
		p_dc->TextOut((int)(dest_point_x+0.5), (int)(dest_point_y+0.5), str);
	}
	
	p_dc->SetTextColor(oldcol);
	p_dc->SelectObject(poldfont);
	pPIMMOMDlg->ReleaseDC(p_dc);//�f�o�C�X�R���e�L�X�g���J������
	font.DeleteObject();
}

void	CPimpomAPI::DrawText(double x, double y, CString str)
{
	DrawText(x, y, str, default_fg_color, NULL);
}


/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ƀe�L�X�g�`��i�t�H�[�}�b�g�w��j
��    ��    �� : DrawText
��          �� : int		x,y			(in)�`����W�i�摜���W�Ŏw��j
				 COLORREF	col			(in)�`��F
				 int		size		(in)�����T�C�Y
				 char		*format, ...(in)�e�L�X�g
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPimpomAPI::DrawText(double x, double y, COLORREF col, int size, char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	DrawText(x, y, CString(buf), col, size);
}