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
//CResizeDataDlg
//�摜�T�C�Y�ύX�ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "ResizeDataDlg.h"

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
IMPLEMENT_DYNAMIC(CResizeDataDlg, CDialog)

CResizeDataDlg::CResizeDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResizeDataDlg::IDD, pParent)
{

}

CResizeDataDlg::~CResizeDataDlg()
{
}

void CResizeDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
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
void CResizeDataDlg::show_dlg_item()
{
	if(API.GetCheck(m_hWnd,IDC_RADIO_RESIZE_OVERWITE)){
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO), true);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CResizeDataDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CResizeDataDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_RESIZE_HALF, &CResizeDataDlg::OnBnClickedButtonResizeHalf)
	ON_BN_CLICKED(IDC_BUTTON_RESIZE_DOUBLE, &CResizeDataDlg::OnBnClickedButtonResizeDouble)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_RESIZE_STRETCH_DATA, &CResizeDataDlg::OnBnClickedCheckResizeStretchData)
	ON_BN_CLICKED(IDC_RADIO_RESIZE_OVERWITE, &CResizeDataDlg::OnBnClickedRadioResizeOverwite)
	ON_BN_CLICKED(IDC_RADIO_RESIZE_TO_IMAGE, &CResizeDataDlg::OnBnClickedRadioResizeToImage)
	ON_BN_CLICKED(IDC_BUTTON_RESIZE_SET_ORG_IMG, &CResizeDataDlg::OnBnClickedButtonResizeSetOrgImg)
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
BOOL CResizeDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDataUnit *p_du=NULL;
	if( (p_du = API.GetDataUnit(CURRENT_IMAGE)) ==NULL)	return false;


	//�u���ʂ�ʃ������֓����v���f�t�H���g�ɂ���
	API.SetCheck(m_hWnd, IDC_RADIO_RESIZE_TO_IMAGE, true);
	show_dlg_item();


	//���ʂ�����摜�������ԍ���ݒ�
	int dst_num, src_num; 
	src_num = API.GetCurrentImageNumber();
	dst_num = src_num + 1;

	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO, src_num);
	if(dst_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO, dst_num);


	//�e��p�����^�̏����l�ݒ�
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, p_du->DataSize.cx);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, p_du->DataSize.cy);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_OFFSET_X, 0);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_OFFSET_Y, 0);
	API.SetCheck(m_hWnd,IDC_RADIO_RESIZE_INTP0,true);


	return true;
}

/********************************************************************
�@  �\  ��  �� : �u���s�v�{�^������
��    ��    �� : OnBnClickedOk
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CResizeDataDlg::OnBnClickedOk()
{
	CDataUnit	*p_du;
	int			width, height, offset_x, offset_y, interp_type;

	if( (p_du = API.GetDataUnit(CURRENT_IMAGE) )==NULL)	return;

	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, &width);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, &height);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_OFFSET_X, &offset_x);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_OFFSET_Y, &offset_y);

	if(API.GetCheck(m_hWnd,IDC_RADIO_RESIZE_INTP1)){
		interp_type=1;
	}else if(API.GetCheck(m_hWnd,IDC_RADIO_RESIZE_INTP2)){
		interp_type=2;
	}else{
		interp_type=0;
	}
	

	if(API.GetCheck(m_hWnd, IDC_RADIO_RESIZE_OVERWITE))
	{
		API.BackupDataUnit(CURRENT_IMAGE);//���݂̃f�[�^�o�b�N�A�b�v

		if( !API.ResizeDataUnit( CURRENT_IMAGE, CSize(width,height), API.GetCheck(m_hWnd, IDC_CHECK_RESIZE_STRETCH_DATA),offset_x,offset_y,true, interp_type))	return ;//���T�C�Y���s

		API.DrawImage();//�`��
	}
	else
	{
		int src_no=0,dst_no=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO, &src_no);
		API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO, &dst_no);

		API.BackupDataUnit(dst_no);//���݂̃f�[�^�o�b�N�A�b�v

		if( !API.ResizeDataUnit( src_no, dst_no, false, CSize(width,height), API.GetCheck(m_hWnd, IDC_CHECK_RESIZE_STRETCH_DATA),offset_x,offset_y,true, interp_type))	return ;//���T�C�Y���s
		
		API.DrawImage(dst_no);//�`��
	}
}

/********************************************************************
�@  �\  ��  �� : �u1/2�v�{�^������
��    ��    �� : OnBnClickedButtonResizeHalf
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CResizeDataDlg::OnBnClickedButtonResizeHalf()
{
	int			width, height;	

	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, &width);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, &height);

	if(width<10 || height<10)	return;
	width /=2;
	height /=2;

	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, width);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, height);
}

/********************************************************************
�@  �\  ��  �� : �ux2�v�{�^������
��    ��    �� : OnBnClickedButtonResizeDouble
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CResizeDataDlg::OnBnClickedButtonResizeDouble()
{
	int			width, height;	

	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, &width);
	API.GetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, &height);

	width *=2;
	height *=2;

	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_W, width);
	API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_H, height);
}


/********************************************************************
�@  �\  ��  �� : �`��
��    ��    �� : OnPaint
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CResizeDataDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//����p�̃r�b�g�}�b�v��\��t���̈�
	CRect rgn;
	API.GetDialogItemRect(m_hWnd, IDC_STATIC_RESIZE_RESCRIPTION, &rgn);
	

	//����p�̃r�b�g�}�b�v��\��t��
	CBitmap bmp;
	if(API.GetCheck(m_hWnd,IDC_CHECK_RESIZE_STRETCH_DATA)){
		bmp.LoadBitmap(IDB_RESIZE2);
	}else{
		bmp.LoadBitmap(IDB_RESIZE1);
	}
	CDC* pDC=GetDC();
	CDC bmpDC;
	bmpDC.CreateCompatibleDC(pDC);
	CBitmap* oldbmp=bmpDC.SelectObject(&bmp);
	pDC->BitBlt(rgn.left, rgn.top, rgn.Width(), rgn.Height(),&bmpDC,0,0,SRCCOPY);
	bmpDC.SelectObject(oldbmp);
	bmp.DeleteObject();
	ReleaseDC(pDC);
}

/********************************************************************
�@  �\  ��  �� : �u�T�C�Y�ɍ��킹�ĉ摜�f�[�^��L�k����v
��    ��    �� : OnBnClickedCheckResizeStretchData
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CResizeDataDlg::OnBnClickedCheckResizeStretchData()
{
	Invalidate();
}


/********************************************************************
�@  �\  ��  �� : �u���ݑI�𒆂̉摜�������ɏ㏑���v
��    ��    �� : OnBnClickedRadioResizeOverwite
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CResizeDataDlg::OnBnClickedRadioResizeOverwite()
{
	show_dlg_item();
}

/********************************************************************
�@  �\  ��  �� :
��    ��    �� : OnBnClickedRadioResizeOverwite
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CResizeDataDlg::OnBnClickedRadioResizeToImage()
{
	show_dlg_item();
}


/********************************************************************
�@  �\  ��  �� :
��    ��    �� : OnBnClickedButtonResizeSetOrgImg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CResizeDataDlg::OnBnClickedButtonResizeSetOrgImg()
{
	int orgNo;

	orgNo = API.GetCurrentImageNumber();

	if(orgNo>=0 && orgNo+1<API.GetDataUnitNumber())
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_FROM_IMAGE_NO, orgNo);
		API.SetEditValue(m_hWnd, IDC_EDIT_RESIZE_TO_IMAGE_NO, orgNo+1);
	}
}
