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
//CTransformDlg
//�摜�̊􉽊w�ϊ��ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "TransformDlg.h"

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
IMPLEMENT_DYNAMIC(CTransformDlg, CDialog)

CTransformDlg::CTransformDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransformDlg::IDD, pParent)
{

}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
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
void CTransformDlg::show_dlg_item()
{
	if(API.GetCheck(m_hWnd,IDC_RADIO_TRANSFORM_OVERWITE)){
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO), true);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTransformDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTransformDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_TRANSFORM_OVERWITE, &CTransformDlg::OnBnClickedRadioTransformOverwite)
	ON_BN_CLICKED(IDC_RADIO_TRANSFORM_TO_IMAGE, &CTransformDlg::OnBnClickedRadioTransformToImage)
	ON_BN_CLICKED(IDC_BUTTON_TRASFORM_SET_ORG_IMG, &CTransformDlg::OnBnClickedButtonTrasformSetOrgImg)
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
BOOL CTransformDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	//�u���ʂ�ʃ������֓����v�������l�ɂ���
	API.SetCheck(m_hWnd, IDC_RADIO_TRANSFORM_TO_IMAGE, true);
	show_dlg_item();


	//���ʂ�����摜�������ԍ���ݒ�
	int dst_num, src_num; 
	src_num = API.GetCurrentImageNumber();
	dst_num = src_num + 1;

	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO, src_num);
	if(dst_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO, dst_num);


	//�e��p�����^�̏����l�ݒ�
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TI, 0);
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TJ, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_FL, 16.0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_CCD, 7.4);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_THETA, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_PHAI, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_PSI, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TX, 0);	
	API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TY, 0);	
	


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
void CTransformDlg::OnBnClickedOk()
{
	CDataUnit	*p_du;
	double		ti, tj,tx,ty,theta,phai,psi,fl,ccd;
	int			outRgnMode = 0;

	if( (p_du = API.GetDataUnit(CURRENT_IMAGE) )==NULL)	return;

	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TI, &ti);
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TJ, &tj);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_FL, &fl);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_CCD, &ccd);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_THETA, &theta);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_PHAI, &phai);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_PSI, &psi);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TX, &tx);	
	API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_PERSPECTIVE_TY, &ty);	
	if (API.GetCheck(m_hWnd, IDC_CHECK_TRANSFORM_OUTRGN)) {
		outRgnMode = 1;
	}
	

	if(API.GetCheck(m_hWnd, IDC_RADIO_TRANSFORM_OVERWITE))
	{
		API.BackupDataUnit(CURRENT_IMAGE);//���݂̃f�[�^�o�b�N�A�b�v

		if( !API.PerspectiveTransDataUnit( CURRENT_IMAGE, ti, tj,tx,ty,theta,phai,psi,fl,ccd,true, outRgnMode))	return ;

		API.DrawImage();//�`��
	}
	else
	{
		int src_no=0,dst_no=1;
		API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO, &src_no);
		API.GetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO, &dst_no);

		API.BackupDataUnit(dst_no);//���݂̃f�[�^�o�b�N�A�b�v

		if( !API.PerspectiveTransDataUnit( src_no, dst_no, false, ti, tj,tx,ty,theta,phai,psi,fl,ccd,true,NULL,1, outRgnMode))	return ;
		
		API.DrawImage(dst_no);//�`��
	}
}

void CTransformDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//����p�̃r�b�g�}�b�v��\��t���̈�
	CRect rgn;
	API.GetDialogItemRect(m_hWnd, IDC_STATIC_TRANSFORM_DESCRIPTION, &rgn);

	//����p�̃r�b�g�}�b�v��\��t��
	CBitmap bmp;
	bmp.LoadBitmap(IDB_TRANSFORM);
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
�@  �\  ��  �� : �u���ݑI�𒆂̉摜�������ɏ㏑���v
��    ��    �� : OnBnClickedRadioTransformOverwite
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CTransformDlg::OnBnClickedRadioTransformOverwite()
{
	show_dlg_item();
}


/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnBnClickedRadioTransformToImage
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CTransformDlg::OnBnClickedRadioTransformToImage()
{
	show_dlg_item();
}


/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnBnClickedButtonTrasformSetOrgImg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CTransformDlg::OnBnClickedButtonTrasformSetOrgImg()
{
	int orgNo;

	orgNo = API.GetCurrentImageNumber();

	if(orgNo>=0 && orgNo+1<API.GetDataUnitNumber())
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_FROM_IMAGE_NO, orgNo);
		API.SetEditValue(m_hWnd, IDC_EDIT_TRANSFORM_TO_IMAGE_NO, orgNo+1);
	}
}
