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
//CWorkAreaDlg
//��Ɨ̈�ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "WorkAreaDlg.h"


#define MAX(a,b)    ((a)>(b) ? (a):(b))

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
extern CPimpomAPI	API;

// *************************************
//         ��    ��    �Q    ��         
// *************************************

//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CWorkAreaDlg, CDialog)

CWorkAreaDlg::CWorkAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkAreaDlg::IDD, pParent)
{

}

CWorkAreaDlg::~CWorkAreaDlg()
{
	addbmp.DeleteObject(); 
	delbmp.DeleteObject(); 
	fullbmp.DeleteObject();
	editbmp.DeleteObject();
	maskcreatebmp.DeleteObject();
	maskdeletebmp.DeleteObject();
	clipbmp.DeleteObject();
	zoombmp.DeleteObject();
}



void CWorkAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX_FIGURE_UNION, m_figure_list);
}


/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̕ҏW��Ԃ��擾����
��    ��    �� : GetWorkAreaNoFromDlg
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CWorkAreaDlg::GetWorkAreaEditMode()
{
	return API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT);
}


/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̕ҏW��Ԃ�ݒ肷��
��    ��    �� : GetWorkAreaEditMode
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::SetWorkAreaEditMode(bool onoff)
{
	API.SetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT, onoff);
}

/********************************************************************
�@  �\  ��  �� : �ҏW���̍�Ɨ̈�ԍ����擾����
��    ��    �� : GetWorkAreaNoFromDlg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CWorkAreaDlg::GetWorkAreaNoFromDlg()
{
	return m_figure_list.GetCurSel();
}


static bool PtInEditMargin(CPoint pt, int x, int y)
{
	if (abs(x - pt.x) < WORKAREA_EDIT_MARGIN && abs(y - pt.y) < WORKAREA_EDIT_MARGIN){
		return true;
	}
	else{
		return false;
	}
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ń}�E�X�N���b�N���ꂽ�Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseDownOnImage
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnMouseDownOnImage(CPoint point)
{
	if (!IsWindowVisible())	return;//��\����Ԃł͖���

	if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT))
	{//�ҏW��
		SetWorkAreaBy2Points(point, point);
	}
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ń}�E�X�h���b�O���ꂽ�Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseDrugOnImage
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnMouseDrugOnImage(CPoint point0, CPoint point1)
{
	if (!IsWindowVisible())	return;//��\����Ԃł͖���

	if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT))
	{//�ҏW��
		SetWorkAreaBy2Points(point0, point1);
		API.DrawImage(CURRENT_IMAGE,false);//�ĕ`��
	}
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ń}�E�X�N���b�N���O���ꂽ�Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseUpOnImage
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnMouseUpOnImage(CPoint point0, CPoint point1)
{
	if (!IsWindowVisible())	return;//��\����Ԃł͖���

	if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT))
	{//�ҏW��
		SetWorkAreaBy2Points(point0, point1,true);
		API.SetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT, false);//�ҏW���[�h����߂�
		API.DrawImage(CURRENT_IMAGE, false);//�ĕ`��	
	}
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ɕ`��
��    ��    �� : OnDrawOnImage
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnDrawOnImage()
{
	if (!IsWindowVisible())	return;//��\����Ԃł͖���

	CDataUnit	*p_du = API.GetDataUnit(CURRENT_IMAGE);//���ݑI�𒆂̉摜��DataUnit���擾����
	if (p_du == NULL) {
		return;//�f�[�^��������Ή������Ȃ�
	}

	int selected_fig_id = ShowWorkAreaProps();
	if (selected_fig_id<0)	return;


	API.ShowMessage(true, 1, false, "��Ɨ̈�");

	for (int i = 0; i<WORKAREA_FIG_NUM; i++)
	{
		FIGURE_UNION	fig;
		bool ornot;
		int type = p_du->WorkArea.GetWorkArea(i, &fig, &ornot);
		if (type<0)	continue;

		//�̈�F
		COLORREF col;
		if (ornot) { col = RGB(0, 255, 0); }//OR�}�`�͗�
		else { col = RGB(255, 0, 0); }//NOT�}�`�͐�

									  //�̈摾��
		int pensize = 1;
		if (selected_fig_id == i) { pensize = 2; }

		switch (type)
		{
		case WORK_AREA_TYPE_RECT://��`
			API.DrawRect((double)fig.rect.left - 0.5, (double)fig.rect.top - 0.5, (double)fig.rect.right + 0.5, (double)fig.rect.bottom + 0.5, pensize, col);
			API.ShowMessage(false, 1, false, "%d:��`(%0.2f x %0.2f)", i, fig.rect.right - fig.rect.left, fig.rect.bottom - fig.rect.top);

			break;

		case WORK_AREA_TYPE_LINE://��������
			API.DrawLine(fig.line.x1, fig.line.y1, fig.line.x2, fig.line.y2, pensize, col, fig.line.w);
			API.ShowMessage(false, 1, false, "%d:����(len=%lf wid=%0.2f)", i, API.calc_dist(fig.line.x1, fig.line.y1, fig.line.x2, fig.line.y2), fig.line.w);
			break;

		case WORK_AREA_TYPE_ELLIPSE://�ȉ~
			API.DrawEllipse(fig.ellipse.cx, fig.ellipse.cy, fig.ellipse.rx, fig.ellipse.ry, pensize, col);
			API.ShowMessage(false, 1, false, "%d:�ȉ~(���a%0.2f �Z�a%0.2f)", i, fig.ellipse.rx, fig.ellipse.ry);
			break;

		case WORK_AREA_TYPE_CIRCLE://�����~��
			API.DrawCircle(fig.circle.cx, fig.circle.cy, fig.circle.r, pensize, col, fig.circle.w);
			API.ShowMessage(false, 1, false, "%d:�~��(���a%0.2f ��%0.2f)", i, fig.circle.r, fig.circle.w);
			break;

		case WORK_AREA_TYPE_ARC://�����~��
			API.ShowMessage(false, 1, false, "%d:�~��", i);
			break;

		case WORK_AREA_TYPE_POLYGON://���p�`
			API.ShowMessage(false, 1, false, "%d:���p�`", i);
			break;

		default:
			API.ShowMessage(false, 1, false, "%d:����", i);
			break;
		}



		//��Ɨ̈�ҏW��
		if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT) && selected_fig_id == i)
		{
			//��Ɨ̈�ҏW���ɂ܂�ő��삷��̈�
			switch (type)
			{
			case WORK_AREA_TYPE_RECT://��`
				API.DrawRect((double)fig.rect.left - WORKAREA_EDIT_MARGIN, (double)fig.rect.top - WORKAREA_EDIT_MARGIN, (double)fig.rect.left + WORKAREA_EDIT_MARGIN, (double)fig.rect.top + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.rect.right - WORKAREA_EDIT_MARGIN, (double)fig.rect.bottom - WORKAREA_EDIT_MARGIN, (double)fig.rect.right + WORKAREA_EDIT_MARGIN, (double)fig.rect.bottom + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.rect.left - WORKAREA_EDIT_MARGIN, (double)fig.rect.bottom - WORKAREA_EDIT_MARGIN, (double)fig.rect.left + WORKAREA_EDIT_MARGIN, (double)fig.rect.bottom + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.rect.right - WORKAREA_EDIT_MARGIN, (double)fig.rect.top - WORKAREA_EDIT_MARGIN, (double)fig.rect.right + WORKAREA_EDIT_MARGIN, (double)fig.rect.top + WORKAREA_EDIT_MARGIN, 1, col);
				break;

			case WORK_AREA_TYPE_LINE://��������
				API.DrawRect((double)fig.line.x1 - WORKAREA_EDIT_MARGIN, (double)fig.line.y1 - WORKAREA_EDIT_MARGIN, (double)fig.line.x1 + WORKAREA_EDIT_MARGIN, (double)fig.line.y1 + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.line.x2 - WORKAREA_EDIT_MARGIN, (double)fig.line.y2 - WORKAREA_EDIT_MARGIN, (double)fig.line.x2 + WORKAREA_EDIT_MARGIN, (double)fig.line.y2 + WORKAREA_EDIT_MARGIN, 1, col);
				break;

			case WORK_AREA_TYPE_ELLIPSE://�ȉ~
				API.DrawRect((double)fig.ellipse.cx - fig.ellipse.rx - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cx - fig.ellipse.rx + WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.ellipse.cx + fig.ellipse.rx - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cx + fig.ellipse.rx + WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.ellipse.cx - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy - fig.ellipse.ry - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cx + WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy - fig.ellipse.ry + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.ellipse.cx - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy + fig.ellipse.ry - WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cx + WORKAREA_EDIT_MARGIN, (double)fig.ellipse.cy + fig.ellipse.ry + WORKAREA_EDIT_MARGIN, 1, col);

				break;

			case WORK_AREA_TYPE_CIRCLE://�����~��
				API.DrawRect((double)fig.circle.cx - fig.circle.r - WORKAREA_EDIT_MARGIN, (double)fig.circle.cy - WORKAREA_EDIT_MARGIN, (double)fig.circle.cx - fig.circle.r + WORKAREA_EDIT_MARGIN, (double)fig.circle.cy + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.ellipse.cx + fig.circle.r - WORKAREA_EDIT_MARGIN, (double)fig.circle.cy - WORKAREA_EDIT_MARGIN, (double)fig.circle.cx + fig.circle.r + WORKAREA_EDIT_MARGIN, (double)fig.circle.cy + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.circle.cx - WORKAREA_EDIT_MARGIN, (double)fig.circle.cy - fig.circle.r - WORKAREA_EDIT_MARGIN, (double)fig.circle.cx + WORKAREA_EDIT_MARGIN, (double)fig.circle.cy - fig.circle.r + WORKAREA_EDIT_MARGIN, 1, col);
				API.DrawRect((double)fig.circle.cx - WORKAREA_EDIT_MARGIN, (double)fig.circle.cy + fig.circle.r - WORKAREA_EDIT_MARGIN, (double)fig.circle.cx + WORKAREA_EDIT_MARGIN, (double)fig.circle.cy + fig.circle.r + WORKAREA_EDIT_MARGIN, 1, col);
				break;

			case WORK_AREA_TYPE_ARC://�����~��
				break;

			case WORK_AREA_TYPE_POLYGON://���p�`
				break;

			default:
				break;
			}
		}
	}
}


/********************************************************************
�@  �\  ��  �� : 2�_�̍��W�����Ɨ̈��ݒ肷��
��    ��    �� : SetWorkAreaBy2Points
��          �� : CPoint		src_p1		(in)
				 CPoint		src_p2		(in)
				 bool		postProc	(in)
��    ��    �l : ���ݑI�𒆂̗̈�ԍ���Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CWorkAreaDlg::SetWorkAreaBy2Points(CPoint src_p1, CPoint src_p2, bool postProc)
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return -1;


	int selected_figure_num=GetWorkAreaNoFromDlg();
	if( selected_figure_num<0)	return -1;

	bool	ornot;
	ornot = !API.GetCheck(m_hWnd,IDC_CHECK_WORKAREA_NOT);


	FIGURE_UNION	fig;
	int type = pdu->WorkArea.GetWorkArea(selected_figure_num, &fig, &ornot);//���X�g�őI�𒆂̐}�`���̎擾
	if (type<0)	return -1;

	switch (type)
	{
	case WORK_AREA_TYPE_RECT://��`
		
		if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 0)
		{//�G�f�B�b�g���[�h�E������
			//2�_�@src_p1  src_p2�@��Ίp�����Ƃ����`�����
			RECT rect;
			rect.left = src_p1.x;
			rect.top = src_p1.y;
			rect.right = src_p2.x;
			rect.bottom = src_p2.y;
			pdu->WorkArea.SetWorkArea(selected_figure_num, rect, ornot);
		}
		else
		{
			if (src_p1 == src_p2){//�}�E�X�_�E����
				if (PtInEditMargin(src_p1, fig.rect.right, fig.rect.bottom)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 3;//�G�f�B�b�g���[�h�ύX�E�E�����W�ړ�
				}
				else if (PtInEditMargin(src_p1, fig.rect.left, fig.rect.top)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 2;//�G�f�B�b�g���[�h�ύX�E������W�ړ�
				}
				else if (PtInEditMargin(src_p1, fig.rect.right, fig.rect.top)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 4;//�G�f�B�b�g���[�h�ύX�E�E����W�ړ�
				}
				else if (PtInEditMargin(src_p1, fig.rect.left, fig.rect.bottom)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 5;//�G�f�B�b�g���[�h�ύX�E�������W�ړ�
				}
			}
			else
			{//�}�E�X�ړ���
				RECT rect;

				if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 3)//�G�f�B�b�g���[�h�E�E�����W�ړ�
				{
					rect.left = fig.rect.left;
					rect.top = fig.rect.top;
					rect.right =  src_p2.x;
					rect.bottom = src_p2.y;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 2)//�G�f�B�b�g���[�h�E������W�ړ�
				{
					rect.left = src_p2.x;
					rect.top = src_p2.y;
					rect.right = fig.rect.right;
					rect.bottom = fig.rect.bottom;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 4)//�G�f�B�b�g���[�h�E�E����W�ړ�
				{
					rect.left = fig.rect.left;
					rect.top = src_p2.y;
					rect.right = src_p2.x;
					rect.bottom = fig.rect.bottom;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 5)//�G�f�B�b�g���[�h�E�������W�ړ�
				{
					rect.left = src_p2.x;
					rect.top = fig.rect.top;
					rect.right = fig.rect.right;
					rect.bottom = src_p2.y;
				}
				else
				{//�G�f�B�b�g���[�h�E�S�̈ړ�
					int centerx = (fig.rect.left + fig.rect.right) / 2;
					int centery = (fig.rect.top + fig.rect.bottom) / 2;

					rect.left = fig.rect.left - centerx + src_p2.x;
					rect.top = fig.rect.top - centery + src_p2.y;
					rect.right = fig.rect.right - centerx + src_p2.x;
					rect.bottom = fig.rect.bottom - centery + src_p2.y;
				}

				pdu->WorkArea.SetWorkArea(selected_figure_num, rect, ornot);
			}


		}

		break;

	case WORK_AREA_TYPE_LINE://����
		if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 0)
		{//�G�f�B�b�g���[�h�E������
			FIGURE_LINE line;
			line.x1 = src_p1.x;
			line.y1 = src_p1.y;
			line.x2 = src_p2.x;
			line.y2 = src_p2.y;
			line.w = 1;
			pdu->WorkArea.SetWorkArea(selected_figure_num, line, ornot);
		}
		else
		{
			if (src_p1 == src_p2){//�}�E�X�_�E����
				if (PtInEditMargin(src_p1, fig.line.x2, fig.line.y2)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 3;//�G�f�B�b�g���[�h�ύX�E�I�_���W�ړ�
				}
				else if (PtInEditMargin(src_p1, fig.line.x1, fig.line.y1)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 2;//�G�f�B�b�g���[�h�ύX�E�n�_���W�ړ�
				}
			}
			else
			{//�}�E�X�ړ���

				FIGURE_LINE line;

				if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 3)//�G�f�B�b�g���[�h�E�I�_���W�ړ�
				{
					line.x1 = fig.line.x1;
					line.y1 = fig.line.y1;
					line.x2 = src_p2.x;
					line.y2 = src_p2.y;
					line.w = fig.line.w;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 2)//�G�f�B�b�g���[�h�E�n�_���W�ړ�
				{
					line.x1 = src_p2.x;
					line.y1 = src_p2.y;
					line.x2 = fig.line.x2;
					line.y2 = fig.line.y2;
					line.w = fig.line.w;
				}
				else
				{//�G�f�B�b�g���[�h�E�S�̈ړ�
					int centerx = (fig.line.x1 + fig.line.x2) / 2;
					int centery = (fig.line.y1 + fig.line.y2) / 2;

					line.x1 = fig.line.x1 - centerx + src_p2.x;
					line.y1 = fig.line.y1 - centery + src_p2.y;
					line.x2 = fig.line.x2 - centerx + src_p2.x;
					line.y2 = fig.line.y2 - centery + src_p2.y;
					line.w = fig.line.w;
				}

				pdu->WorkArea.SetWorkArea(selected_figure_num, line, ornot);
			}
		}
		break;


	case WORK_AREA_TYPE_ELLIPSE://�ȉ~

		if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 0)
		{//�G�f�B�b�g���[�h�E������
			FIGURE_ELLIPSE ellipse;
			ellipse.cx = (src_p1.x + src_p2.x)/2;
			ellipse.cy = (src_p1.y + src_p2.y)/2;
			ellipse.rx = abs(src_p1.x - src_p2.x)/2;
			ellipse.ry = abs(src_p1.y - src_p2.y)/2;
			pdu->WorkArea.SetWorkArea(selected_figure_num, ellipse, ornot);
		}
		else
		{
			if (src_p1 == src_p2){//�}�E�X�_�E����
				if (PtInEditMargin(src_p1, fig.ellipse.cx + fig.ellipse.rx, fig.ellipse.cy)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 3;//�G�f�B�b�g���[�h�ύX�E�����ω�
				}
				else if (PtInEditMargin(src_p1, fig.ellipse.cx, fig.ellipse.cy + fig.ellipse.ry)){
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 2;//�G�f�B�b�g���[�h�ύX�E�c���ω�
				}
			}
			else
			{
				FIGURE_ELLIPSE ellipse;

				if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 3)//�G�f�B�b�g���[�h�E�����ω�
				{
					ellipse.cx = fig.ellipse.cx;
					ellipse.cy = fig.ellipse.cy;
					ellipse.rx = fabs((float)(src_p2.x - fig.ellipse.cx));
					ellipse.ry = fig.ellipse.ry;
				}
				else if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 2)//�G�f�B�b�g���[�h�E�c���ω�
				{
					ellipse.cx = fig.ellipse.cx;
					ellipse.cy = fig.ellipse.cy;
					ellipse.rx = fig.ellipse.rx;
					ellipse.ry = fabs((float)(src_p2.y - fig.ellipse.cy));
				}
				else
				{//�G�f�B�b�g���[�h�E�S�̈ړ�
					ellipse.cx = src_p2.x;
					ellipse.cy = src_p2.y;
					ellipse.rx = fig.ellipse.rx;
					ellipse.ry = fig.ellipse.ry;

				}

				pdu->WorkArea.SetWorkArea(selected_figure_num, ellipse, ornot);
			}
			
		}

		break;

	case WORK_AREA_TYPE_CIRCLE://�~

		if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 0)
		{//�G�f�B�b�g���[�h�E������
			FIGURE_CIRCLE circle;
			circle.cx = src_p1.x;
			circle.cy = src_p1.y;
			circle.r = (int)sqrt((double)(src_p1.y - src_p2.y)*(src_p1.y - src_p2.y) + (double)(src_p1.x - src_p2.x)*(src_p1.x - src_p2.x));
			circle.w = 1;
			pdu->WorkArea.SetWorkArea(selected_figure_num, circle, ornot);
		}
		else
		{
			if (src_p1 == src_p2){//�}�E�X�_�E����
				if (PtInEditMargin(src_p1, fig.circle.cx + fig.circle.r, fig.circle.cy) || 
					PtInEditMargin(src_p1, fig.circle.cx - fig.circle.r, fig.circle.cy) || 
					PtInEditMargin(src_p1, fig.circle.cx, fig.circle.cy + fig.circle.r) || 
					PtInEditMargin(src_p1, fig.circle.cx, fig.circle.cy - fig.circle.r) )
				{
					pdu->WorkArea.FigureEditMode[selected_figure_num] = 2;//�G�f�B�b�g���[�h�ύX�E���a�ω�
				}
			}
			else
			{
				FIGURE_CIRCLE circle;

				if (pdu->WorkArea.FigureEditMode[selected_figure_num] == 2)//�G�f�B�b�g���[�h�E���a�ω�
				{
					circle.cx = fig.circle.cx;
					circle.cy = fig.circle.cy;
					circle.r = MAX(fabs(src_p2.x - fig.circle.cx), fabs(src_p2.y - fig.circle.cy));
					circle.w = fig.circle.w;
				}
				else
				{//�G�f�B�b�g���[�h�E�S�̈ړ�
					circle.cx = src_p2.x;
					circle.cy = src_p2.y;
					circle.r = fig.circle.r;
					circle.w = fig.circle.w;
				}

				pdu->WorkArea.SetWorkArea(selected_figure_num, circle, ornot);
			}

		}

		break;

	case WORK_AREA_TYPE_ARC://�~��
		break;

	case WORK_AREA_TYPE_POLYGON://���p�`
		break;

	default ://�ݒ�Ȃ�
		break;
	}

	ShowWorkAreaProps();//�̈���W��\������

	if(postProc)
	{//�}�`�ݒ��̌㏈��
		pdu->WorkArea.FigureEditMode[selected_figure_num] = 1;//�G�f�B�b�g���[�h�ύX�E�S�̈ړ�

		if(API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_MASK)){//�}�X�N����
			API.BackupDataUnit(CURRENT_IMAGE);//���݂̉摜���o�b�N�A�b�v
			pdu->SetMaskByWorkArea();
		}
	}

	return selected_figure_num;
}

/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : figure_type_fo_list_string
��          �� : int type
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CString CWorkAreaDlg::figure_type_fo_list_string(int type)
{
	CString str;
	switch(type){
		case WORK_AREA_TYPE_RECT://��`
			str = "��`";
			break;
		case WORK_AREA_TYPE_LINE://����
			str = "����";
			break;
		case WORK_AREA_TYPE_ELLIPSE://�ȉ~
			str = "�ȉ~";
			break;
		case WORK_AREA_TYPE_CIRCLE://�~
			str = "�~��";
			break;
		case WORK_AREA_TYPE_ARC://�~��
			str = "�~��";
			break;
		case WORK_AREA_TYPE_POLYGON://���p�`
			str = "���p�`";
			break;
		case WORK_AREA_TYPE_NOTHING:
			str = "����";
			break;
	}
	return str;
}

/********************************************************************
�@  �\  ��  �� : �}�`���X�g���X�V����
��    ��    �� : update_figure_list
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::update_figure_list()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	bool selected_flg=false;
	int valid_figure_num=-1;
	int sel = m_figure_list.GetCurSel();//���ݑI�𒆂̃��X�g�ԍ��擾


	//�L���Ȑ}�`�����J�E���g
	valid_figure_num=-1;
	for(int i=0 ; i<WORKAREA_FIG_NUM ; i++){
		if(pdu->WorkArea.Type[i] != WORK_AREA_TYPE_NOTHING){
			valid_figure_num=i;
		}
	}

	m_figure_list.ResetContent();//�}�`���X�g����U���Z�b�g

	//�}�`���X�g���Đݒ�
	for(int i=0 ; i<valid_figure_num+1 ; i++){
		CString fig_num;
		fig_num.Format("%d:", i);
		m_figure_list.AddString(fig_num + figure_type_fo_list_string(pdu->WorkArea.Type[i]));
	}

	if(sel>0 && sel<=valid_figure_num){//���Ƃ��ƑI��ł������X�g�ԍ���I��
		m_figure_list.SetCurSel(sel);
	}else{
		m_figure_list.SetCurSel(0);
	}
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̐ݒ�l���_�C�A���O�ɕ\������
��    ��    �� : ShowWorkAreaProps
��          �� : 
��    ��    �l : ���ݑI�𒆂̗̈�ԍ���Ԃ� ���s������-1��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CWorkAreaDlg::ShowWorkAreaProps()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return -1;

	update_figure_list();//��Ɨ̈�̏�Ԃ����X�g�ɔ��f

	int selected_figure_num=GetWorkAreaNoFromDlg();//���X�g�őI�𒆂̐}�`�ԍ��̎擾
	if(selected_figure_num<0)	return -1;

	FIGURE_UNION	fig;
	bool ornot;
	int type = pdu->WorkArea.GetWorkArea(selected_figure_num,&fig,&ornot);//���X�g�őI�𒆂̐}�`���̎擾
	if(type<0)	return -1;


	//���X�g�őI�𒆂̐}�`�̃f�[�^��\��/////////////////////////////////
	API.SetCheck(m_hWnd, IDC_CHECK_WORKAREA_NOT, !ornot);

	CSpinButtonCtrl *spin1  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM1);
	CSpinButtonCtrl *spin2  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM2);
	CSpinButtonCtrl *spin3  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM3);
	CSpinButtonCtrl *spin4  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM4);
	CSpinButtonCtrl *spin5  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM5);
	CSpinButtonCtrl *spin6  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM6);

	int show_hyde[6];
	switch(type)
	{
	case WORK_AREA_TYPE_RECT://��`

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("�E��X"));
		spin1->SetPos(fig.rect.left);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.rect.left);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("�E��Y"));
		spin2->SetPos(fig.rect.top);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.rect.top);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("����X"));
		spin3->SetPos(fig.rect.right);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.rect.right);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("����Y"));
		spin4->SetPos(fig.rect.bottom);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.rect.bottom);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;

	case WORK_AREA_TYPE_LINE://����

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("�n�_X"));
		spin1->SetPos(fig.line.x1);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.line.x1);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("�n�_Y"));
		spin2->SetPos(fig.line.y1);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.line.y1);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("�I�_X"));
		spin3->SetPos( fig.line.x2);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.line.x2);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("�I�_Y"));
		spin4->SetPos(fig.line.y2);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.line.y2);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM5,_T("��"));
		spin5->SetPos(fig.line.w);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM5, fig.line.w);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_SHOW;
		show_hyde[5]=SW_HIDE;
		break;

	case WORK_AREA_TYPE_ELLIPSE://�ȉ~

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("���S�_X"));
		spin1->SetPos(fig.ellipse.cx);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.ellipse.cx);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("���S�_Y"));
		spin2->SetPos(fig.ellipse.cy);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.ellipse.cy);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("X���a"));
		spin3->SetPos( fig.ellipse.rx);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.ellipse.rx);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("Y���a"));
		spin4->SetPos(fig.ellipse.ry);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.ellipse.ry);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;

	case WORK_AREA_TYPE_CIRCLE://�~

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("���S�_X"));
		spin1->SetPos(fig.circle.cx);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.circle.cx);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("���S�_Y"));
		spin2->SetPos(fig.circle.cy);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.circle.cy);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("���a"));
		spin3->SetPos( fig.circle.r);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.circle.r);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("��"));
		spin4->SetPos(fig.circle.w);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.circle.w);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;


	case WORK_AREA_TYPE_ARC://�~��

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM1,_T("���S�_X"));
		spin1->SetPos(fig.arc.cx);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, fig.arc.cx);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM2,_T("���S�_Y"));
		spin2->SetPos(fig.arc.cy);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, fig.arc.cy);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM3,_T("���a"));
		spin3->SetPos( fig.arc.r);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, fig.arc.r);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM4,_T("�n�_�p�x"));
		spin4->SetPos(fig.arc.start);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, fig.arc.start);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM5,_T("�I�_�p�x"));
		spin5->SetPos(fig.arc.end);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM5, fig.arc.end);

		API.SetEditValue(m_hWnd,IDC_STATIC_WORKAREA_PARAM6,_T("��"));
		spin5->SetPos(fig.arc.w);
		API.SetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM6, fig.arc.w);

		show_hyde[0]=SW_SHOW;
		show_hyde[1]=SW_SHOW;
		show_hyde[2]=SW_SHOW;
		show_hyde[3]=SW_SHOW;
		show_hyde[4]=SW_SHOW;
		show_hyde[5]=SW_SHOW;
		break;

	case WORK_AREA_TYPE_POLYGON://���p�`
		show_hyde[0]=SW_HIDE;
		show_hyde[1]=SW_HIDE;
		show_hyde[2]=SW_HIDE;
		show_hyde[3]=SW_HIDE;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;

	default ://����
		show_hyde[0]=SW_HIDE;
		show_hyde[1]=SW_HIDE;
		show_hyde[2]=SW_HIDE;
		show_hyde[3]=SW_HIDE;
		show_hyde[4]=SW_HIDE;
		show_hyde[5]=SW_HIDE;
		break;
	}

	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM1), show_hyde[0]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM2), show_hyde[1]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM3), show_hyde[2]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM4), show_hyde[3]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM5), show_hyde[4]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM6), show_hyde[5]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM1), show_hyde[0]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM2), show_hyde[1]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM3), show_hyde[2]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM4), show_hyde[3]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM5), show_hyde[4]);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM6), show_hyde[5]);
	spin1->ShowWindow(show_hyde[0]);
	spin2->ShowWindow(show_hyde[1]);
	spin3->ShowWindow(show_hyde[2]);
	spin4->ShowWindow(show_hyde[3]);
	spin5->ShowWindow(show_hyde[4]);
	spin6->ShowWindow(show_hyde[5]);




	return selected_figure_num;
}

/********************************************************************
�@  �\  ��  �� : �ҏW���̍�Ɨ̈�����C����ʂ֔��f����
��    ��    �� : ApplyWorkAreaPropsFromDlg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::ApplyWorkAreaPropsFromDlg()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	int selected_figure_num=GetWorkAreaNoFromDlg();//�_�C�A���O�őI�𒆂̐}�`�ԍ�
	if(selected_figure_num<0)	return;

	bool	ornot;
	int		val1, val2, val3, val4, val5, val6;
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM1, &val1);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM2, &val2);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM3, &val3);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM4, &val4);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM5, &val5);
	API.GetEditValue(m_hWnd,IDC_EDIT_WORKAREA_PARAM6, &val6);
	ornot = !API.GetCheck(m_hWnd,IDC_CHECK_WORKAREA_NOT);

	switch(pdu->WorkArea.Type[selected_figure_num])
	{
	case WORK_AREA_TYPE_RECT://��`
		{
			RECT rect = {val1, val2, val3, val4};
			pdu->WorkArea.SetWorkArea(selected_figure_num, rect, ornot);
		}
		break;

	case WORK_AREA_TYPE_LINE://��������
		{
			FIGURE_LINE line = {(float)val1, (float)val2, (float)val3, (float)val4, (float)val5};
			pdu->WorkArea.SetWorkArea(selected_figure_num, line, ornot);
		}
		break;

	case WORK_AREA_TYPE_ELLIPSE://�ȉ~
		{
			FIGURE_ELLIPSE ellipse = { (float)val1, (float)val2, (float)val3, (float)val4};
			pdu->WorkArea.SetWorkArea(selected_figure_num, ellipse, ornot);
		}
		break;

	case WORK_AREA_TYPE_CIRCLE://�����~��
		{
			FIGURE_CIRCLE circle = { (float)val1, (float)val2, (float)val3, (float)val4};
			pdu->WorkArea.SetWorkArea(selected_figure_num, circle, ornot);
		}
		break;

	case WORK_AREA_TYPE_ARC://�����~��
		{
			FIGURE_ARC arc = { (float)val1, (float)val2, (float)val3, (float)val4, (float)val5, (float)val6};
			pdu->WorkArea.SetWorkArea(selected_figure_num, arc, ornot);
		}
		break;

	case WORK_AREA_TYPE_POLYGON://���p�`
		break;

	default :
		return;
	}

	API.DrawImage();

}


//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWorkAreaDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_APPLY, &CWorkAreaDlg::OnBnClickedButtonWorkareaApply)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM1, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM2, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM3, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM4, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM5, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WORKAREA_PARAM6, &CWorkAreaDlg::OnDeltaposSpinWorkareaParam6)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM1, &CWorkAreaDlg::OnEnChangeEditWorkareaParam1)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM2, &CWorkAreaDlg::OnEnChangeEditWorkareaParam2)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM3, &CWorkAreaDlg::OnEnChangeEditWorkareaParam3)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM4, &CWorkAreaDlg::OnEnChangeEditWorkareaParam4)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM5, &CWorkAreaDlg::OnEnChangeEditWorkareaParam5)
	ON_EN_CHANGE(IDC_EDIT_WORKAREA_PARAM6, &CWorkAreaDlg::OnEnChangeEditWorkareaParam6)
	ON_BN_CLICKED(IDC_CHECK_WORKAREA_MASK, &CWorkAreaDlg::OnBnClickedCheckWorkareaMask)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_ADD, &CWorkAreaDlg::OnBnClickedButtonWorkareaAdd)
	ON_LBN_SELCHANGE(IDC_LISTBOX_FIGURE_UNION, &CWorkAreaDlg::OnLbnSelchangeListWorkareaFigure)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_DELETE, &CWorkAreaDlg::OnBnClickedButtonWorkareaDelete)
	ON_BN_CLICKED(IDC_CHECK_WORKAREA_NOT, &CWorkAreaDlg::OnBnClickedCheckWorkareaNot)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_CLIP, &CWorkAreaDlg::OnBnClickedButtonWorkareaClip)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_CLEAR_MASK, &CWorkAreaDlg::OnBnClickedButtonWorkareaClearMask)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_FULL, &CWorkAreaDlg::OnBnClickedButtonWorkareaFull)
	ON_BN_CLICKED(IDC_BUTTON_WORKAREA_SCALE, &CWorkAreaDlg::OnBnClickedButtonWorkareaScale)
	ON_BN_CLICKED(IDC_CHECK_WORKAREA_EDIT, &CWorkAreaDlg::OnBnClickedWorkareaEdit)
	ON_WM_CLOSE()
	
END_MESSAGE_MAP()


/********************************************************************
�@  �\  ��  �� : �_�C�A���O�̏�����
��    ��    �� : OnInitDialog
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CWorkAreaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//�r�b�g�}�b�v�{�^��
	addbmp.LoadBitmap(IDB_BUTTON_WORKAREA_ADD);
	delbmp.LoadBitmap(IDB_BUTTON_WORKAREA_DEL);
	fullbmp.LoadBitmap(IDB_BUTTON_WORKAREA_FULL);
	editbmp.LoadBitmap(IDB_BUTTON_WORK_AREA_EDIT);
	maskcreatebmp.LoadBitmap(IDB_BUTTON_WORKAREA_MASK);
	maskdeletebmp.LoadBitmap(IDB_BUTTON_CLEAR_MASK);
	clipbmp.LoadBitmap(IDB_BUTTON_WORKAREA_CLIP);
	zoombmp.LoadBitmap(IDB_BUTTON_WORKAREA_ZOOM);

	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_ADD))->SetBitmap((HBITMAP)addbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_DELETE))->SetBitmap((HBITMAP)delbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_FULL))->SetBitmap((HBITMAP)fullbmp);
	((CButton*)GetDlgItem(IDC_CHECK_WORKAREA_EDIT))->SetBitmap((HBITMAP)editbmp);
	((CButton*)GetDlgItem(IDC_CHECK_WORKAREA_MASK))->SetBitmap((HBITMAP)maskcreatebmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_CLEAR_MASK))->SetBitmap((HBITMAP)maskdeletebmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_CLIP))->SetBitmap((HBITMAP)clipbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_WORKAREA_SCALE))->SetBitmap((HBITMAP)zoombmp);

	//�c�[���`�b�v
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_ADD), "��Ɨ̈��ǉ�");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_DELETE), "��Ɨ̈���폜");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_FULL), "��Ɨ̈��S��ʉ�");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_WORKAREA_EDIT), "��Ɨ̈��ҏW");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_WORKAREA_MASK), "��Ɨ̈�O���Ƀ}�X�N��ݒ�");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_CLEAR_MASK), "�}�X�N���N���A");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_CLIP), "��Ɨ̈�ŉ摜���N���b�v");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_WORKAREA_SCALE), "�v���̈���g��\��");

	//�X�s���{�^���͈̔͐ݒ�
	CSpinButtonCtrl *spin1  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM1);
	CSpinButtonCtrl *spin2  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM2);
	CSpinButtonCtrl *spin3  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM3);
	CSpinButtonCtrl *spin4  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM4);
	CSpinButtonCtrl *spin5  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM5);
	CSpinButtonCtrl *spin6  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_WORKAREA_PARAM6);
	spin1->SetRange(0,9999);
	spin2->SetRange(0,9999);
	spin3->SetRange(0,9999);
	spin4->SetRange(0,9999);
	spin5->SetRange(0,9999);
	spin6->SetRange(0,9999);
	
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_WORKAREA_EDIT), 0, 0, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_ADD), 30, 0, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_DELETE), 60, 0, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_FULL), 90, 0, 30, 30, false);

	::MoveWindow(::GetDlgItem(m_hWnd, IDC_LISTBOX_FIGURE_UNION), 0, 30, 100, 210, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_WORKAREA_NOT), 105, 30, 100, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_APPLY), 205, 30, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM1), 105, 60, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM1), 185, 60, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM1), 245, 60, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM2), 105, 90, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM2), 185, 90, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM2), 245, 90, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM3), 105, 120, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM3), 185, 120, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM3), 245, 120, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM4), 105, 150, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM4), 185, 150, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM4), 245, 150, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM5), 105, 180, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM5), 185, 180, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM5), 245, 180, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_WORKAREA_PARAM6), 105, 210, 80, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WORKAREA_PARAM6), 185, 210, 60, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_SPIN_WORKAREA_PARAM6), 245, 210, 30, 30, false);
	
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_WORKAREA_MASK), 110, 240, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_CLEAR_MASK), 140, 240, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_CLIP), 170, 240, 30, 30, false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_WORKAREA_SCALE), 200, 240, 30, 30, false);

	

	ShowWorkAreaProps();


	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/********************************************************************
�@  �\  ��  �� :�L�[�{�[�h����̓���
��    ��    �� : PreTranslateMessage
��          �� :
��    ��    �l :
�@          �\ : �J�[�\���L�[�F�J�[�\���̈ړ�
Del�L�[�F�I�𒆂̉摜�������N���A
Enter/Esc�L�[�F������
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CWorkAreaDlg::PreTranslateMessage(MSG* pMsg)
{
	POINT point;

	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_RETURN://Enter
			return  true;

		case VK_ESCAPE://Esc
			return  true;

		case VK_UP://��
			GetCursorPos(&point);
			SetCursorPos(point.x, point.y - 1);
			return  true;

		case VK_DOWN://��
			GetCursorPos(&point);
			SetCursorPos(point.x, point.y + 1);
			return  true;

		case VK_LEFT://��
			GetCursorPos(&point);
			SetCursorPos(point.x - 1, point.y);
			return  true;

		case VK_RIGHT://��
			GetCursorPos(&point);
			SetCursorPos(point.x + 1, point.y);
			return true;

		default:
			break;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		//�c�[���`�b�v�\��
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_ADD)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_DELETE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_FULL)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_WORKAREA_EDIT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_WORKAREA_MASK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_CLEAR_MASK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_CLIP)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_WORKAREA_SCALE)->m_hWnd
			)
		{
			m_toolTip.RelayEvent(pMsg);
		}
		else
		{
			m_toolTip.Pop();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
�@  �\  ��  �� : ���̃E�B���h�E��\����Ԃɂ���
��    ��    �� : OnShowWindow
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow) {
		ShowWorkAreaProps();
	}
}

/********************************************************************
�@  �\  ��  �� : �ĕ`��
��    ��    �� : OnPaint
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�ŕύX�������e���f�[�^�ɔ��f����
��    ��    �� : ApplyWorkAreaPropsFromDlg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaApply()
{
	ApplyWorkAreaPropsFromDlg();
}

/********************************************************************
�@  �\  ��  �� : �u��Ɨ̈�ǉ���ǉ��v�{�^������
��    ��    �� : OnBnClickedButtonWorkareaAdd
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaAdd()
{
	int addtype = 0;
	int addnumber;
	CConfirmDlg dlg;

	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	update_figure_list();//�}�`���X�g���X�V
	addnumber = m_figure_list.GetCount();//�ǉ�����ԍ�
	if(addnumber>=WORKAREA_FIG_NUM)		return;


	//�}�`��ޑI���_�C�A���O���o��
	bool type[6]={0};
	type[0]=true;
	dlg.SetTitle("��Ɨ̈�}�`");
	dlg.RegistRadioVar(figure_type_fo_list_string(WORK_AREA_TYPE_RECT),&type[0]);
	dlg.RegistRadioVar(figure_type_fo_list_string(WORK_AREA_TYPE_LINE),&type[1]);
	dlg.RegistRadioVar(figure_type_fo_list_string(WORK_AREA_TYPE_ELLIPSE),&type[2]);
	dlg.RegistRadioVar(figure_type_fo_list_string(WORK_AREA_TYPE_CIRCLE),&type[3]);
	if( dlg.DoModal() != IDOK )	return;

	for(int i=0 ; i<6 ; i++){
		if(type[i]){
			addtype=i;
			break;
		}
	}

	pdu->WorkArea.Type[addnumber] = addtype;//��Ɨ̈��L����
	pdu->WorkArea.FigureEditMode[addnumber] = 0;//�����������

	update_figure_list();//�}�`���X�g���čX�V
	m_figure_list.SetCurSel(addnumber);//�ǉ��������X�g�v�f��I��

	API.DrawImage();//���C����ʍĕ`��

	API.SetWorkAreaEditMode(true);//��Ɨ̈�ҏW���[�hON
}

/********************************************************************
�@  �\  ��  �� : �u��Ɨ̈���폜�v�{�^������
��    ��    �� : OnBnClickedButtonWorkareaDelete
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaDelete()
{
	int deletenumber = 0;


	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	update_figure_list();//�}�`���X�g���X�V
	deletenumber = m_figure_list.GetCurSel();//�폜����ԍ�
	if(deletenumber<0)		return;

	pdu->WorkArea.Type[deletenumber] = WORK_AREA_TYPE_NOTHING;//��Ɨ̈�𖳌���

	//�}�`�ԍ�����
	for(int i=deletenumber ; i<WORKAREA_FIG_NUM-1 ; i++){
		pdu->WorkArea.Type[i] = pdu->WorkArea.Type[i+1];
		pdu->WorkArea.OrNot[i] = pdu->WorkArea.OrNot[i+1];
		memcpy(&pdu->WorkArea.Figure[i], &pdu->WorkArea.Figure[i+1], sizeof(FIGURE_UNION)); 
	}

	API.DrawImage();//���C����ʍĕ`��
}

/********************************************************************
�@  �\  ��  �� : �X�s���{�^������
��    ��    �� : OnDeltaposSpinWorkareaParam 1-6
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnDeltaposSpinWorkareaParam1(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam2(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam3(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam4(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam5(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

void CWorkAreaDlg::OnDeltaposSpinWorkareaParam6(NMHDR *pNMHDR, LRESULT *pResult)
{
	edit_from_spin = true;
}

/********************************************************************
�@  �\  ��  �� : �R���g���[���̒l�ϓ�
��    ��    �� : OnEnChangeEditWorkareaParam 1-6
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnEnChangeEditWorkareaParam1()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam2()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam3()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam4()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam5()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

void CWorkAreaDlg::OnEnChangeEditWorkareaParam6()
{
	if (edit_from_spin) {
		edit_from_spin = false;
		ApplyWorkAreaPropsFromDlg();
	}
}

/********************************************************************
�@  �\  ��  �� : �u��Ɨ̈悩��}�X�N�����v�{�^������
��    ��    �� : OnBnClickedCheckWorkareaMask
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedCheckWorkareaMask()
{
	if(API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_MASK)){
		CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
		if(!pdu)	return;

		pdu->SetMaskByWorkArea();//���[�N�G���A����}�X�N����
		API.DrawImage();//���C����ʍĕ`��
	}	
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̐}�`�ύX
��    ��    �� : OnLbnSelchangeListWorkareaFigure
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnLbnSelchangeListWorkareaFigure()
{
	API.DrawImage();//���C����ʍĕ`��
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈��OR/NOT�ݒ�ύX
��    ��    �� : OnBnClickedCheckWorkareaNot
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedCheckWorkareaNot()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	bool	ornot;
	ornot = !API.GetCheck(m_hWnd,IDC_CHECK_WORKAREA_NOT);

	int sel = m_figure_list.GetCurSel();
	if(sel<0)		return;

	pdu->WorkArea.OrNot[sel] = ornot;

	API.DrawImage();//���C����ʍĕ`��
}

/********************************************************************
�@  �\  ��  �� : �u��Ɨ̈�ŉ摜���N���b�s���O�v�{�^������
��    ��    �� : OnBnClickedButtonWorkareaClip
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaClip()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	API.BackupDataUnit(CURRENT_IMAGE);//���݂̉摜���o�b�N�A�b�v

	API.ClipDataUnit(CURRENT_IMAGE, pdu->WorkArea.GetCircumRect(), true);//�N���b�s���O

	API.DrawImage();//���C����ʍĕ`��
}

/********************************************************************
�@  �\  ��  �� : �u�}�X�N�N���A�v�{�^������
��    ��    �� : OnBnClickedButtonWorkareaClearMask
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaClearMask()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	API.BackupDataUnit(CURRENT_IMAGE);//���݂̉摜���o�b�N�A�b�v

	pdu->ClearMask();//�}�X�N�N���A
	API.DrawImage();//���C����ʍĕ`��
}

/********************************************************************
�@  �\  ��  �� : �u�S�̈扻�v�{�^������
��    ��    �� : OnBnClickedButtonWorkareaFull
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaFull()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;

	API.BackupDataUnit(CURRENT_IMAGE);//���݂̉摜���o�b�N�A�b�v

	pdu->InitWorkArea();//
	API.DrawImage();//���C����ʍĕ`��
}


/********************************************************************
�@  �\  ��  �� : �u�g��\���v�{�^������
��    ��    �� : OnBnClickedButtonWorkareaScale
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnBnClickedButtonWorkareaScale()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(!pdu)	return;


	CRect rect = pdu->WorkArea.GetCircumRect(m_figure_list.GetCurSel());
	float scaleX = (float)API.main_image_size.cx / (float)(rect.right - rect.left + 1) ;
	float scaleY = (float)API.main_image_size.cy / (float)(rect.bottom - rect.top + 1);

	pdu->DispScale = (scaleX>scaleY) ? scaleY : scaleX;
	pdu->DataDispOffset = CPoint(rect.left, rect.top);


	API.DrawImage();//���C����ʍĕ`��
}


/********************************************************************
�@  �\  ��  �� : �E�C���h�E����
��    ��    �� : OnClose
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CWorkAreaDlg::OnClose()
{

	CDialog::OnClose();
}


void CWorkAreaDlg::OnBnClickedWorkareaEdit()
{
	if (API.GetCheck(m_hWnd, IDC_CHECK_WORKAREA_EDIT)) 
	{
		API.SetWorkAreaEditMode(true);
	}
	else {
		API.SetWorkAreaEditMode(false);
	}
}