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
//CDataPickDlg
//�摜�������t���[�n���h�ҏW�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "DataPickDlg.h"

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
extern	CPimpomAPI	API;

// *************************************
//         ��    ��    �Q    ��         
// *************************************

//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDataPickDlg, CDialog)

CDataPickDlg::CDataPickDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataPickDlg::IDD, pParent)
{

}

CDataPickDlg::~CDataPickDlg()
{
	paintbmp.DeleteObject(); 
	pickbmp.DeleteObject();
	fillbm.DeleteObject();
	undobmp.DeleteObject();
}

void CDataPickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

/********************************************************************
�@  �\  ��  �� : �f�[�^�s�b�N���[�h�Ȃ̂��A�y�C���g���[�h�Ȃ̂��擾
��    ��    �� : GetDataPickMode
��          �� : 
��    ��    �l :  DATAPICK_PICK:�s�b�N  DATAPICK_PAINT:�y�C���g DATAPICK_FILL:�t�B��
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
int CDataPickDlg::GetDataPickMode()
{
	if (API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{
		return DATAPICK_PICK;
	}
	else if (API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))
	{
		return DATAPICK_PAINT;
	}
	else 
	{
		return DATAPICK_FILL;
	}
}

/********************************************************************
�@  �\  ��  �� : �f�[�^�s�b�N���[�h�Ȃ̂��A�y�C���g���[�h�Ȃ̂��ݒ�
��    ��    �� : SetDataPickMode
��          �� : DATAPICK_PICK:�s�b�N  DATAPICK_PAINT:�y�C���g DATAPICK_FILL:�t�B��
��    ��    �l : 
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::SetDataPickMode(int mode)
{
	if (mode== DATAPICK_PICK)
	{
		API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK, true);
	}
	else if (mode == DATAPICK_PAINT)
	{
		API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT, true);
		show_dlg_item();
	}
	else
	{
		API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_FILL, true);
		show_dlg_item();
	}
	
}

/********************************************************************
�@  �\  ��  �� : ����̍��W�̉�f���s�b�N����
��    ��    �� : DataPick
��          �� : CPoint point	(in)�s�b�N������W
��    ��    �l : ����ɏI��������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataPickDlg::DataPick(CPoint point)
{
	if( !API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK) )		return false;

	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾

		if(p_du == NULL)		return false;//�f�[�^��������΂Ȃɂ����Ȃ�
		if(point.x<0 || point.y<0 || point.x >= p_du->DataSize.cx || point.y >= p_du->DataSize.cy)	return false;

		//�f�[�^�s�b�N
		if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB==false )
		{//���m�N��
			double bri;
			bri = p_du->GetDataValue(point,-1,-1);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,bri);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,bri);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,bri);
		}
		else
		{//RGB
			double r,g,b;
			r = p_du->GetDataValue(point, -1,0);
			g = p_du->GetDataValue(point, -1,1);
			b = p_du->GetDataValue(point, -1,2);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,r);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,g);
			API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,b);

		}

		//�}�X�N�s�b�N
		if(p_du->GetMaskValue(point)==MASKED_PIXEL){
			API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON, true);
			API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_OFF, false);
		}else{
			API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON, false);
			API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_OFF, true);
		}

	return true;
}





/********************************************************************
�@  �\  ��  �� : ����̍��W�̉�f�Ƀh�b�g��`��
��    ��    �� : PaintDot
��          �� : CPoint point	(in)�`�悷����W
��    ��    �l : ����ɏI��������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataPickDlg::PaintDot(CPoint point)
{
	if( API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK) || !API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))		return false;

	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾
	if(p_du == NULL)		return false;//�f�[�^��������΂Ȃɂ����Ȃ�

	int dot_size;
	double r,g,b;
	API.GetEditValue(m_hWnd, IDC_EDIT_DATAPICK_PAINT_DOT_SIZE, &dot_size);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,&r);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,&g);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,&b);


	if(!API.GetCheck(m_hWnd,IDC_CHECK_DATAPICK_MASK))
	{//�f�[�^�ɕ`��
		if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
		{//RGB
			API.DrawPointOnImage(CURRENT_IMAGE, point.x, point.y, dot_size, RGB((BYTE)r,(BYTE)g,(BYTE)b));
		}
		else
		{//���m�N��
			API.DrawPointOnImage(CURRENT_IMAGE, point.x, point.y, dot_size, r);
		}
	}
	else
	{//�}�X�N�ɕ`��
		int mask_val=UNMASKED_PIXEL;
		if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON))	mask_val=MASKED_PIXEL;
		API.DrawPointOnMask(CURRENT_IMAGE, point.x, point.y, dot_size, mask_val);
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : ����̍��W�̉�f���N�_�Ƃ��āA�P��F�œh��Ԃ�
��    ��    �� : PaintFloodFill
��          �� : CPoint point	(in)�N�_�Ƃ�����W
��    ��    �l : ����ɏI��������true
�@          �\ : �܂��摜�S�̂Ń��׃����O���s���A�N�_�ʒu�Ɠ����x���ɂȂ�����f�����ׂēh��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataPickDlg::PaintFloodFill(CPoint point)
{
	if( API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK) || API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))		return false;

	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾
	if(p_du == NULL)		return false;//�f�[�^��������΂Ȃɂ����Ȃ�

	double r,g,b;
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,&r);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,&g);
	API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,&b);

	if(!API.GetCheck(m_hWnd,IDC_CHECK_DATAPICK_MASK))
	{//�f�[�^�ɕ`��
		if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
		{//RGB
			API.FloodFillOnImage(CURRENT_IMAGE, point.x, point.y, RGB((BYTE)r,(BYTE)g,(BYTE)b));
		}
		else
		{//���m�N��
			API.FloodFillOnImage(CURRENT_IMAGE, point.x, point.y, r);
		}
	}
	else
	{//�}�X�N�ɕ`��
		int mask_val=UNMASKED_PIXEL;
		if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON))	mask_val=MASKED_PIXEL;
		API.FloodFillOnMask(CURRENT_IMAGE, point.x, point.y, mask_val);
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ń}�E�X�N���b�N���ꂽ�Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseDownOnImage
��          �� : CPoint point	(in)�摜���W
��    ��    �l : 
�@          �\ : �_�C�A���O�̏�Ԃɍ��킹�A�s�b�N�^�y�C���g�^�h��Ԃ����s��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataPickDlg::OnMouseDownOnImage(CPoint point)
{
	if(!IsWindowVisible())	return true;//��\����Ԃł͖���

	bool ret=false;
	if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{//�s�b�N
		ret = DataPick(point);
	}
	else
	{//�y�C���g
		HCURSOR csr = AfxGetApp()->LoadCursor( IDC_CURSOR_PEN );//�J�[�\�����y���ɑւ���
		SetCursor( csr );

		if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))
		{//�h�b�g
			API.BackupDataUnit(CURRENT_IMAGE);
			ret = PaintDot(point);
		}
		else
		{//�h��Ԃ�
			API.BackupDataUnit(CURRENT_IMAGE);
			ret = PaintFloodFill(point);
		}

		API.DrawImage();
	}

	return ret;
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ń}�E�X�h���b�O���ꂽ�Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseDrugOnImage
��          �� : CPoint point	(in)�摜���W
��    ��    �l : 
�@          �\ : �y�C���g���[�h�̂Ƃ��́A�h���b�O�ʒu�Ƀy�C���g���s��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataPickDlg::OnMouseDrugOnImage(CPoint point)
{
	if(!IsWindowVisible())	return true;//��\����Ԃł͖���

	bool ret=false;
	if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{//�s�b�N

	}
	else
	{//�y�C���g
		if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))
		{//�h�b�g
			ret = PaintDot(point);
		}
		else
		{//�h��Ԃ�
			ret = PaintFloodFill(point);
		}
		API.DrawImage();
	}

	return ret;
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ���}�E�X���ʉ݂����Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseMoveOnImage
��          �� : CPoint point	(in)�摜���W
��    ��    �l : 
�@          �\ : �y�C���g���[�h�̂Ƃ��̓A�C�R����ύX����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataPickDlg::OnMouseMoveOnImage(CPoint point)
{
	if(!IsWindowVisible())	return true;//��\����Ԃł͖���

	if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{//�s�b�N

	}
	else
	{//�y�C���g
		int cur_id;
		if (API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT))
		{//�h�b�g
			cur_id = IDC_CURSOR_PEN;
		}
		else 
		{//�h��Ԃ�
			cur_id = IDC_CURSOR_FLOOD;
		}
		HCURSOR csr = AfxGetApp()->LoadCursor(cur_id);//�J�[�\�����y���ɑւ���
		SetCursor( csr );
	}
	return true;
}

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�A�C�e���̕\����ԕύX
��    ��    �� : show_dlg_item
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::show_dlg_item()
{
	int pickflg, maskflg;

	//�s�b�N�E�y�C���g�ɘA�����ĕς�����

	if( API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK) )	pickflg = FALSE;
	else	pickflg = TRUE;

	::EnableWindow(::GetDlgItem(m_hWnd, IDC_GROUP_DATAPICK_PAINT_DETAIL), pickflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_DATAPICK_PAINT_FILL_WORKAREA), pickflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_LABEL_DATAPICK_PAINT_DOT), pickflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_DATAPICK_PAINT_DOT_SIZE), pickflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_DATAPICK_UNDO), pickflg);

	//�}�X�NONOFF�ɘA�����ĕς�����

	if( API.GetCheck(m_hWnd, IDC_CHECK_DATAPICK_MASK) )	maskflg = TRUE;
	else	maskflg = FALSE;

	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_DATAPICK_DATA0), !maskflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_DATAPICK_DATA1), !maskflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_DATAPICK_DATA2), !maskflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON), maskflg);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_DATAPICK_MASK_OFF), maskflg);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDataPickDlg, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_DATAPICK_PAINT_FILL_WORKAREA, &CDataPickDlg::OnBnClickedButtonFillWorkarea)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_DATAPICK, &CDataPickDlg::OnBnClickedRadioDatapick)
	ON_BN_CLICKED(IDC_RADIO_DATAPICK_PAINT_DOT, &CDataPickDlg::OnBnClickedRadioDatapaint)
	ON_BN_CLICKED(IDC_RADIO_DATAPICK_PAINT_FILL, &CDataPickDlg::OnBnClickedRadioDatafill)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_DATAPICK_MASK, &CDataPickDlg::OnBnClickedCheckDatapickMask)
	ON_BN_CLICKED(IDC_BUTTON_DATAPICK_UNDO, &CDataPickDlg::OnBnClickedUndo)
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
BOOL CDataPickDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�r�b�g�}�b�v�{�^��
	pickbmp.LoadBitmap(IDB_BUTTON_PICK);
	paintbmp.LoadBitmap(IDB_BUTTON_PAINT);
	fillbm.LoadBitmap(IDB_BUTTON_FILL);
	undobmp.LoadBitmap(IDB_BUTTON_UNDO);
	((CButton*)GetDlgItem(IDC_RADIO_DATAPICK))->SetBitmap((HBITMAP)pickbmp);
	((CButton*)GetDlgItem(IDC_RADIO_DATAPICK_PAINT_DOT))->SetBitmap((HBITMAP)paintbmp);
	((CButton*)GetDlgItem(IDC_RADIO_DATAPICK_PAINT_FILL))->SetBitmap((HBITMAP)fillbm);
	((CButton*)GetDlgItem(IDC_BUTTON_DATAPICK_UNDO))->SetBitmap((HBITMAP)undobmp);

	//�c�[���`�b�v
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_RADIO_DATAPICK), "�s�b�N");
	m_toolTip.AddTool(GetDlgItem(IDC_RADIO_DATAPICK_PAINT_DOT), "�y�C���g");
	m_toolTip.AddTool(GetDlgItem(IDC_RADIO_DATAPICK_PAINT_FILL), "�h��Ԃ�");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_DATAPICK_UNDO), "���ɖ߂�");

	//�l�̏�����
	API.SetCheck(m_hWnd,IDC_CHECK_DATAPICK_MASK,false);
	API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK, true);
	API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_DOT, false);
	API.SetCheck(m_hWnd, IDC_RADIO_DATAPICK_PAINT_FILL, false);
	API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,0);
	API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,0);
	API.SetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,0);
	API.SetEditValue(m_hWnd, IDC_EDIT_DATAPICK_PAINT_DOT_SIZE, 1);
	CSpinButtonCtrl *spin1  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PAINT_DOT_SIZE);
	spin1->SetRange(1,100);	

	show_dlg_item();
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/********************************************************************
�@  �\  ��  �� : ���̃E�B���h�E���������ꂽ
��    ��    �� : OnShowWindow
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (bShow) {
		show_dlg_item();
	}
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
BOOL CDataPickDlg::PreTranslateMessage(MSG* pMsg)
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
		if (pMsg->hwnd == GetDlgItem(IDC_RADIO_DATAPICK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_RADIO_DATAPICK_PAINT_DOT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_DATAPICK_UNDO)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_RADIO_DATAPICK_PAINT_FILL)->m_hWnd
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
�@  �\  ��  �� : ��Ɨ̈������h��Ԃ�
��    ��    �� : OnBnClickedButtonFillWorkarea
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::OnBnClickedButtonFillWorkarea()
{
	if(!API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK))
	{
		CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾
		if(p_du == NULL)		return ;//�f�[�^��������΂Ȃɂ����Ȃ�


		API.BackupDataUnit(CURRENT_IMAGE);//�f�[�^�̃o�b�N�A�b�v

		if(!API.GetCheck(m_hWnd,IDC_CHECK_DATAPICK_MASK))
		{//�f�[�^�ɕ`��
			if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
			{//RGB
				double r=0,g=0,b=0;
				API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,&r);
				API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA1,&g);
				API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA2,&b);

				API.FillWorkAreaOnImage(CURRENT_IMAGE, RGB(r,g,b));
			}
			else
			{//���m�N��
				double bri=0;
				API.GetEditValue(m_hWnd,IDC_EDIT_DATAPICK_DATA0,&bri);
				
				API.FillWorkAreaOnImage(CURRENT_IMAGE, bri);
				API.DrawImage();
			}
		}else{//�}�X�N�ɕ`��
			int mask_val=UNMASKED_PIXEL;
			if(API.GetCheck(m_hWnd, IDC_RADIO_DATAPICK_MASK_ON))	mask_val=MASKED_PIXEL;

			API.FillWorkAreaOnMask(CURRENT_IMAGE, mask_val);
		}
	}
	API.DrawImage();
}




/********************************************************************
�@  �\  ��  �� : �E�B���h�E�����
��    ��    �� : OnClose
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::OnClose()
{
	CDialog::OnClose();

	API.SetDataPickEditType(0);
}

/********************************************************************
�@  �\  ��  �� : �s�b�N���[�h�I��
��    ��    �� : OnBnClickedRadioDatapick
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::OnBnClickedRadioDatapick()
{
	show_dlg_item();
}

/********************************************************************
�@  �\  ��  �� : �y�C���g���[�h�I��
��    ��    �� : OnBnClickedRadioDatapaint
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::OnBnClickedRadioDatapaint()
{
	show_dlg_item();
}

/********************************************************************
�@  �\  ��  �� : �t�B�����[�h�I��
��    ��    �� : OnBnClickedRadioDatafill
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::OnBnClickedRadioDatafill()
{
	show_dlg_item();
}

/********************************************************************
�@  �\  ��  �� : �}�X�NON/OFF�I��
��    ��    �� : OnBnClickedCheckDatapickMask
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::OnBnClickedCheckDatapickMask()
{
	show_dlg_item();
}


/********************************************************************
�@  �\  ��  �� : Undo
��    ��    �� : OnBnClickedUndo
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataPickDlg::OnBnClickedUndo()
{
	API.RecoverDataUnit(CURRENT_IMAGE);
}