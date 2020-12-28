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
//CWorkArea
//��Ɨ̈�������N���X
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

/********************************************************************
�@  �\  ��  �� : �}�`�ԍ����w�肵�č�Ɨ̈������������
��    ��    �� : Initialize
��          �� : CSize		image_size				(in)�摜�������T�C�Y
				 int		no						(in)�}�`�ԍ�
				 int		t						(in)�}�`�^�C�v
				 bool		ornot					(in)OR�̈�^NOT�̈�  true:OR  false:NOT
��    ��    �l : 
�@          �\ : ��`�^�C�v�ŏ���������ꍇ�A�T�C�Y�͉摜�������̃T�C�Y�ɂȂ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkArea::Initialize(CSize image_size, int no, int t, bool ornot)
{
	//�����`�F�b�N
	if(image_size.cx<=0 || image_size.cy<=0)	return;
	if(no<0 || no>=WORKAREA_FIG_NUM)return;


	memset(&Figure[no], 0, sizeof(FIGURE_UNION));

	FigureEditMode[no] = 0;
	Type[no] = t;
	OrNot[no] = ornot;
	switch(Type[no])
	{
	case WORK_AREA_TYPE_RECT://��`
		Figure[no].rect.left = 0;
		Figure[no].rect.top = 0;
		Figure[no].rect.right = image_size.cx -1;
		Figure[no].rect.bottom = image_size.cy -1;
		break;

	case WORK_AREA_TYPE_LINE://��������
		memset(&Figure[no].line, 0, sizeof(FIGURE_LINE));
		Figure[no].line.w = 1;
		break;

	case WORK_AREA_TYPE_ELLIPSE://�ȉ~
		memset(&Figure[no].ellipse, 0, sizeof(FIGURE_ELLIPSE));
		break;

	case WORK_AREA_TYPE_CIRCLE://�����~��
		memset(&Figure[no].circle, 0, sizeof(FIGURE_CIRCLE));
		Figure[no].circle.w=1;
		break;

	case WORK_AREA_TYPE_ARC://�����~��
		memset(&Figure[no].arc, 0, sizeof(FIGURE_ARC));
		Figure[no].arc.w=1;
		break;

	case WORK_AREA_TYPE_POLYGON://���p�`
		memset(&Figure[no].polygon, 0, sizeof(FIGURE_POLYGON));
		break;
	}
}

/********************************************************************
�@  �\  ��  �� : �S�Ă̍�Ɨ̈������������
��    ��    �� : Initialize
��          �� : CSize		image_size				(in)�摜�������T�C�Y
��    ��    �l : 
�@          �\ : ��Ɨ̈�0�͉摜�T�C�Y�̋�`�ɏ������B����ȊO�͖���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkArea::Initialize(CSize image_size)
{
	//�����`�F�b�N
	if(image_size.cx<=0 || image_size.cy<=0)	return;


	Initialize(image_size, 0,  WORK_AREA_TYPE_RECT, true);//��Ɨ̈�0�͉摜�T�C�Y�̋�`�ɏ������B����ȊO�͖���
	for(int i=1 ; i<WORKAREA_FIG_NUM ; i++){
		Initialize(CSize(1,1), i,  WORK_AREA_TYPE_NOTHING, true);
	}
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈���R�s�[����
��    ��    �� : Copy
��          �� : CWorkArea		*area		(in)�R�s�[���̍�Ɨ̈�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkArea::Copy(CWorkArea *area)
{
	//�����`�F�b�N
	if(!area)	return;


	memcpy(Type, area->Type, sizeof(int)*WORKAREA_FIG_NUM);
	memcpy(Figure, area->Figure, sizeof(FIGURE_UNION)*WORKAREA_FIG_NUM);
	memcpy(OrNot, area->OrNot, sizeof(bool)*WORKAREA_FIG_NUM);
}

/********************************************************************
�@  �\  ��  �� : �_����Ɨ̈���ɓ����Ă��邩�`�F�b�N����
��    ��    �� : PtInWorkArea
��          �� : CPoint		point		(in)
��    ��    �l : �S�Ă̍�Ɨ̈���`�F�b�N���A
				�ǂꂩ��OR�ɓ����Ă��邩�ǂ�NOT�̈�ɂ������Ă��Ȃ��Ȃ��true��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::PtInWorkArea(CPoint point)
{
	//�ǂꂩ��NOT�}�`���Ȃ�Η̈�O�Ƃ���
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++){
		if(!OrNot[i]){
			if( !PtInWorkArea(point,i) )	return false;
		}
	}

	//���ׂĂ�NOT�}�`�O����
	//�ǂꂩ��OR�}�`���Ȃ�Η̈���Ƃ���
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++){
		if(OrNot[i]){
			if( PtInWorkArea(point,i) )	return true;
		}
	}

	return false;
}

/********************************************************************
�@  �\  ��  �� : �_����Ɨ̈���ɓ����Ă��邩�`�F�b�N����i�}�`�ԍ��w��j
��    ��    �� : PtInWorkArea
��          �� : CPoint		point		(in)
				 int		no			(in)�}�`�ԍ�
��    ��    �l : no�Ŏw�肳�ꂽ�}�`�ԍ���
				 OR�̈�F�@�@�}�`���ɓ����Ă����true
				 NOT�̈�F�@�@�}�`���ɓ����Ă��Ȃ����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::PtInWorkArea(CPoint point, int no)
{
	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)return false;


	bool res=false;

	switch(Type[no])
	{
	case WORK_AREA_TYPE_RECT://��`
		if( Figure[no].rect.left<=point.x && Figure[no].rect.top<=point.y && 
			Figure[no].rect.right>=point.x && Figure[no].rect.bottom>=point.y)
		{
			res = OrNot[no];
		}else{
			res = !OrNot[no];
		}
		break;



	case WORK_AREA_TYPE_LINE://��������
		if(Figure[no].line.x1==Figure[no].line.x2 || Figure[no].line.y1==Figure[no].line.y2 || Figure[no].line.w==0){
			res = !OrNot[no];//�ʐς��Ȃ��Ƃ�
		}else{
			CPoint vertex[4];
			double outer_prod[4];
			GetLineVertxes(Figure[no].line, vertex);

			outer_prod[0] = (vertex[0].x - point.x)*(vertex[1].y - point.y) - (vertex[0].y - point.y)*(vertex[1].x - point.x) ;
			outer_prod[1] = (vertex[1].x - point.x)*(vertex[2].y - point.y) - (vertex[1].y - point.y)*(vertex[2].x - point.x) ;
			outer_prod[2] = (vertex[2].x - point.x)*(vertex[3].y - point.y) - (vertex[2].y - point.y)*(vertex[3].x - point.x) ;
			outer_prod[3] = (vertex[3].x - point.x)*(vertex[0].y - point.y) - (vertex[3].y - point.y)*(vertex[0].x - point.x) ;

			if( outer_prod[0]*outer_prod[1]<0 ||
				outer_prod[1]*outer_prod[2]<0 ||
				outer_prod[2]*outer_prod[3]<0 ||
				outer_prod[3]*outer_prod[0]<0)
			{
				res = !OrNot[no];
			}else{
				res = OrNot[no];
			}
		}
		break;


	case WORK_AREA_TYPE_ELLIPSE://�ȉ~
		if(Figure[no].ellipse.rx==0 || Figure[no].ellipse.ry==0){
			res = !OrNot[no];//�ʐς��Ȃ��Ƃ�
		}else{
			double dis;
			dis = (double)(Figure[no].ellipse.cx - point.x)*(Figure[no].ellipse.cx - point.x)/(double)(Figure[no].ellipse.rx*Figure[no].ellipse.rx)
				+ (double)(Figure[no].ellipse.cy - point.y)*(Figure[no].ellipse.cy - point.y)/(double)(Figure[no].ellipse.ry*Figure[no].ellipse.ry);

			if(dis<=1.0)	res = OrNot[no];
			else		res = !OrNot[no];
		}
		break;

	case WORK_AREA_TYPE_CIRCLE://�����~��
		if(Figure[no].circle.r==0 || Figure[no].circle.w==0){
			res = !OrNot[no];//�ʐς��Ȃ��Ƃ�
		}else{
			double radius2;
			radius2 = (double)(Figure[no].circle.cx - point.x)*(Figure[no].circle.cx - point.x)
				+ (double)(Figure[no].circle.cy - point.y)*(Figure[no].circle.cy - point.y);

			if( radius2> (Figure[no].circle.r - Figure[no].circle.w/2)*(Figure[no].circle.r - Figure[no].circle.w/2) &&
				radius2< (Figure[no].circle.r + Figure[no].circle.w/2)*(Figure[no].circle.r + Figure[no].circle.w/2))
			{
				res = OrNot[no];
			}else{
				res = !OrNot[no];
			}
		}
		break;

	//�����~��
	case WORK_AREA_TYPE_ARC://�����~��
		res = !OrNot[no];
		break;

	//���p�`
	case WORK_AREA_TYPE_POLYGON://���p�`
		res = !OrNot[no];
		break;

	//�̈�Ȃ�
	default:
		res = false;
		break;
	}

	return res;
}

/********************************************************************
�@  �\  ��  �� : ���������̂S���_�����߂�
��    ��    �� : GetLineVertxes
��          �� : FIGURE_LINE		line			(in)��������
				 CPoint				point[4]		(out)���_
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkArea::GetLineVertxes(FIGURE_LINE line, CPoint point[4])
{
		double len, tx, ty;
		CPoint	p1, p2, p3, p4;

		len = sqrt( (double)(line.x1 - line.x2)*(line.x1 - line.x2) + (double)(line.y1 - line.y2)*(line.y1 - line.y2)  );
		if(len==0){
			point[0] = CPoint(line.x1,line.y1);
			point[1] = CPoint(line.x1,line.y1);
			point[2] = CPoint(line.x2,line.y2);
			point[3] = CPoint(line.x2,line.y2);
			return;
		}

		tx =  (double)(line.y1 - line.y2) / len * (double)line.w/2;
		ty = -(double)(line.x1 - line.x2) / len * (double)line.w/2;
		
		point[0].x = line.x1-(long)tx;	point[0].y = line.y1-(long)ty;
		point[1].x = line.x1+(long)tx;	point[1].y = line.y1+(long)ty;
		point[2].x = line.x2+(long)tx;	point[2].y = line.y2+(long)ty;
		point[3].x = line.x2-(long)tx;	point[3].y = line.y2-(long)ty;
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈���擾����
��    ��    �� : GetWorkArea
��          �� : int				no			(in)�}�`�ԍ�
				 FIGURE_UNION	*fig		(out)�}�`
				 bool				*ornot		(out)OR�̈�^NOT�̈�  true:OR  false:NOT
��    ��    �l : ��Ɨ̈�̐}�`�̃^�C�v   (���s������-1��Ԃ�)
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CWorkArea::GetWorkArea(int no, FIGURE_UNION *fig, bool *ornot)
{
	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)	return -1;

	memcpy(fig, &Figure[no], sizeof(FIGURE_UNION));
	*ornot = OrNot[no];
	return Type[no];
}

/********************************************************************
�@  �\  ��  �� : ��`�̍�Ɨ̈��ݒ肷��
��    ��    �� : SetWorkArea
��          �� : int				no			(in)�}�`�ԍ�
				 RECT				rect		(in)��`
				 bool				ornot		(in)OR�̈�^NOT�̈�  true:OR  false:NOT
��    ��    �l : ����������true
�@          �\ : RECT���̏㉺���E�֌W�����]���Ă����ꍇ�́A���K������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::SetWorkArea(int no, RECT rect, bool ornot)
{
	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;


	//RECT���̏㉺���E�֌W�����]���Ă����ꍇ�́A���K������
	int tmp;
	if(rect.left>rect.right){
		tmp = rect.left;
		rect.left = rect.right;
		rect.right = tmp;
	}

	if(rect.top>rect.bottom){
		tmp=rect.top;
		rect.top=rect.bottom;
		rect.bottom = tmp;
	}

	Type[no]= WORK_AREA_TYPE_RECT;
	OrNot[no] = ornot;
	Figure[no].rect.left=rect.left;
	Figure[no].rect.top=rect.top;
	Figure[no].rect.right=rect.right;
	Figure[no].rect.bottom=rect.bottom;

	return true;
}

/********************************************************************
�@  �\  ��  �� : ���������̍�Ɨ̈��ݒ肷��
��    ��    �� : SetWorkArea
��          �� : int				no			(in)�}�`�ԍ�
				 FIGURE_LINE		line		(in)��������
				 bool				ornot		(in)OR�̈�^NOT�̈�  true:OR  false:NOT
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::SetWorkArea(int no, FIGURE_LINE line, bool ornot)
{
	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	Type[no] = WORK_AREA_TYPE_LINE;
	OrNot[no] = ornot;
	Figure[no].line.x1=line.x1;
	Figure[no].line.y1=line.y1;
	Figure[no].line.x2=line.x2;
	Figure[no].line.y2=line.y2;
	Figure[no].line.w=line.w;

	return true;
}

/********************************************************************
�@  �\  ��  �� : �ȉ~�̍�Ɨ̈��ݒ肷��
��    ��    �� : SetWorkArea
��          �� : int				no			(in)�}�`�ԍ�
				 FIGURE_ELLIPSE	ellipse		(in)�ȉ~
				 bool				ornot		(in)OR�̈�^NOT�̈�  true:OR  false:NOT
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::SetWorkArea(int no, FIGURE_ELLIPSE ellipse, bool ornot)
{
	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	Type[no] = WORK_AREA_TYPE_ELLIPSE;
	OrNot[no] = ornot;
	Figure[no].ellipse.cx=ellipse.cx;
	Figure[no].ellipse.cy=ellipse.cy;
	Figure[no].ellipse.rx=ellipse.rx;
	Figure[no].ellipse.ry=ellipse.ry;

	return true;
}

/********************************************************************
�@  �\  ��  �� : �����~���̍�Ɨ̈��ݒ肷��
��    ��    �� : SetWorkArea
��          �� : int				no			(in)�}�`�ԍ�
				 FIGURE_CIRCLE	circle		(in)�����~��
				 bool				ornot		(in)OR�̈�^NOT�̈�  true:OR  false:NOT
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::SetWorkArea(int no, FIGURE_CIRCLE circle, bool ornot)
{
	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	Type[no] = WORK_AREA_TYPE_CIRCLE;
	OrNot[no] = ornot;
	Figure[no].circle.cx=circle.cx;
	Figure[no].circle.cy=circle.cy;
	Figure[no].circle.r=circle.r;
	Figure[no].circle.w=circle.w;

	return true;
}

/********************************************************************
�@  �\  ��  �� : �����~�ʂ̍�Ɨ̈��ݒ肷��
��    ��    �� : SetWorkArea
��          �� : int				no			(in)�}�`�ԍ�
				 FIGURE_ARC		arc			(in)�����~��
				 bool				ornot		(in)OR�̈�^NOT�̈�  true:OR  false:NOT
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::SetWorkArea(int no, FIGURE_ARC arc, bool ornot)
{
	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	Type[no] = WORK_AREA_TYPE_ARC;
	OrNot[no] = ornot;
	Figure[no].arc.cx=arc.cx;
	Figure[no].arc.cy=arc.cy;
	Figure[no].arc.r=arc.r;
	Figure[no].arc.start=arc.start;
	Figure[no].arc.end=arc.end;
	Figure[no].arc.w=arc.w;

	return true;
}

/********************************************************************
�@  �\  ��  �� : ���ׂĂ̐}�`�ɊO�ڂ����`�����߂�
��    ��    �� : GetCircumRect
��          �� : 
��    ��    �l : �O�ڋ�`
�@          �\ : �����Ɏ��s�����ꍇ��(0,0,0,0)���A��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CRect CWorkArea::GetCircumRect()
{
	CRect	rect;
	int		xmin,ymin,xmax,ymax;

	xmin = ymin = 99999;
	xmax = ymax = -99999;
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++)
	{
		if(Type[i] != WORK_AREA_TYPE_NOTHING)
		{
			rect = GetCircumRect(i);
			if(xmin > rect.left){		xmin=rect.left;	}
			if(xmax < rect.right){		xmax=rect.right;}
			if(ymin > rect.top){		ymin=rect.top;	}
			if(ymax < rect.bottom){		ymax=rect.bottom;}
		}
	}

	return CRect(xmin,ymin,xmax,ymax);
}

/********************************************************************
�@  �\  ��  �� : �w�肳�ꂽ�}�`�ɊO�ڂ����`�����߂�
��    ��    �� : GetCircumRect
��          �� : int				no			(in)�}�`�ԍ�
��    ��    �l : �O�ڋ�`
�@          �\ : �����Ɏ��s�����ꍇ��(0,0,0,0)���A��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CRect CWorkArea::GetCircumRect(int no)
{
	CRect	rect(0,0,0,0);

	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)	return rect;

	switch(Type[no])
	{
	case WORK_AREA_TYPE_RECT://��`
		rect.left = Figure[no].rect.left;
		rect.top = Figure[no].rect.top;
		rect.right = Figure[no].rect.right;
		rect.bottom = Figure[no].rect.bottom;
		break;

	case WORK_AREA_TYPE_LINE://��������
		{
			CPoint	vertex[4];
			int		xmin,ymin,xmax,ymax;
			GetLineVertxes(Figure[no].line, vertex);

			xmin = ymin = 99999;
			xmax = ymax = -99999;
			for(int n=0 ; n<4 ; n++)
			{
				if(xmin > vertex[n].x){			xmin=vertex[n].x;	}
				else if(xmax < vertex[n].x){	xmax=vertex[n].x;	}

				if(ymin > vertex[n].y){			ymin=vertex[n].y;	}
				else if(ymax < vertex[n].y){	ymax=vertex[n].y;	}
			}

			rect.left = xmin;
			rect.right = xmax;
			rect.top = ymin;
			rect.bottom = ymax;
		}
		break;


	case WORK_AREA_TYPE_ELLIPSE://�ȉ~
		{
			rect.left = Figure[no].ellipse.cx - Figure[no].ellipse.rx;
			rect.right = Figure[no].ellipse.cx + Figure[no].ellipse.rx;
			rect.top = Figure[no].ellipse.cy - Figure[no].ellipse.ry;
			rect.bottom = Figure[no].ellipse.cy + Figure[no].ellipse.ry;
		}
		break;

	case WORK_AREA_TYPE_CIRCLE://�����~��
		{
			rect.left = Figure[no].circle.cx - (Figure[no].circle.r + Figure[no].circle.w/2) ;
			rect.right = Figure[no].circle.cx + (Figure[no].circle.r + Figure[no].circle.w/2) ;
			rect.top = Figure[no].circle.cy - (Figure[no].circle.r + Figure[no].circle.w/2) ;
			rect.bottom = Figure[no].circle.cy + (Figure[no].circle.r + Figure[no].circle.w/2) ;
		}
		break;

	//�����~��
	case WORK_AREA_TYPE_ARC://�����~��
		break;

	//���p�`
	case WORK_AREA_TYPE_POLYGON://���p�`
		break;

	//�̈�Ȃ�
	default:
		break;
	}

	rect.NormalizeRect();
	return rect;
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�𕽍s�ړ�
��    ��    �� : Move
��          �� : int				dx,dy		(in)�ړ���
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::Move(int dx, int dy)
{
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++)
	{
		if( !Move(i,dx,dy) )	return false;
	}
	return true;
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�𕽍s�ړ�
��    ��    �� : Move
��          �� :int				no			(in)�}�`�ԍ� 
				int				dx,dy		(in)�ړ���
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::Move(int no, int dx, int dy)
{
	//�����`�F�b�N
	if(no<0 || no>=WORKAREA_FIG_NUM)	return false;

	switch(Type[no])
	{
	case WORK_AREA_TYPE_RECT://��`
		Figure[no].rect.left += dx;
		Figure[no].rect.right += dx;
		Figure[no].rect.top += dy;
		Figure[no].rect.bottom += dy;
		break;

	case WORK_AREA_TYPE_LINE://��������
		Figure[no].line.x1 += dx;
		Figure[no].line.x2 += dx;
		Figure[no].line.y1 += dy;
		Figure[no].line.y2 += dy;
		break;


	case WORK_AREA_TYPE_ELLIPSE://�ȉ~
		Figure[no].ellipse.cx += dx;
		Figure[no].ellipse.cy += dy;
		break;

	case WORK_AREA_TYPE_CIRCLE://�����~��
		Figure[no].circle.cx += dx;
		Figure[no].circle.cy += dy;
		break;

	//�����~��
	case WORK_AREA_TYPE_ARC://�����~��
		break;

	//���p�`
	case WORK_AREA_TYPE_POLYGON://���p�`
		break;

	//�̈�Ȃ�
	default:
		return false;
	}

	return true;
}



/********************************************************************
�@  �\  ��  �� : ��Ɨ̈���X�P�[�����O����
��    ��    �� : Scale
��          �� : 
				 double scalex�@�@(in)x�X�P�[��
				 double scaley    (in)y�X�P�[��
				 CPoint offset    (in)x,y�I�t�Z�b�g
��    ��    �l : 
�@          �\ : ���������A�~�ʂ̂Ƃ���x/y�X�P�[���������ꍇ�̂ݎ��s�\
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkArea::Scale(int no, float scalex, float scaley, CPoint offset)
{
	//�����`�F�b�N
	if (no<0 || no >= WORKAREA_FIG_NUM)	return false;

	switch (Type[no])
	{
	case WORK_AREA_TYPE_RECT://��`
		Figure[no].rect.left = scalex * Figure[no].rect.left + offset.x;
		Figure[no].rect.right = scalex * Figure[no].rect.right + offset.x;
		Figure[no].rect.top = scaley * Figure[no].rect.top + offset.y;
		Figure[no].rect.bottom = scaley * Figure[no].rect.bottom + offset.y;
		break;

	case WORK_AREA_TYPE_LINE://��������
		if (scalex == scaley)
		{
			Figure[no].line.x1 = scalex * Figure[no].line.x1 + offset.x;
			Figure[no].line.x2 = scalex * Figure[no].line.x2 + offset.x;
			Figure[no].line.y1 = scaley * Figure[no].line.y1 + offset.y;
			Figure[no].line.y2 = scaley * Figure[no].line.y2 + offset.y;
			Figure[no].line.w = scalex * Figure[no].line.w;
		}
		else {
				return false;
			}
		break;


	case WORK_AREA_TYPE_ELLIPSE://�ȉ~
		Figure[no].ellipse.cx = scalex * Figure[no].ellipse.cx + offset.x;
		Figure[no].ellipse.cy = scaley * Figure[no].ellipse.cy + offset.y;
		Figure[no].ellipse.rx = scalex * Figure[no].ellipse.rx;
		Figure[no].ellipse.ry = scaley * Figure[no].ellipse.ry;
		break;

	case WORK_AREA_TYPE_CIRCLE://�����~��
		if (scalex == scaley)
		{
			Figure[no].circle.cx = scalex * Figure[no].circle.cx + offset.x;
			Figure[no].circle.cy = scalex * Figure[no].circle.cy + offset.y;
			Figure[no].circle.r = scalex * Figure[no].circle.r;
			Figure[no].circle.w = scalex * Figure[no].circle.w;
		}
		else {
			return false;
		}
		break;

		//�����~��
	case WORK_AREA_TYPE_ARC://�����~��
		return false;
		//break;

		//���p�`
	case WORK_AREA_TYPE_POLYGON://���p�`
		break;

		//�̈�Ȃ�
	default:
		return false;
	}

	return true;
}



