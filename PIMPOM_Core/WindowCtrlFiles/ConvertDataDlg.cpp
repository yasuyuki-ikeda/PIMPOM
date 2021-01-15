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
//CConvertDataDlg
//�摜�������̌^�ϊ��̃_�C�A���O
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "ConvertDataDlg.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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



//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////

CConvertDataDlg::CConvertDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConvertDataDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CConvertDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConvertDataDlg)
	// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_CONVERT_DLG_DST_TYPE, m_combo_dst_type);
	DDX_Control(pDX, IDC_COMBO_CONVERT_DLG_SRC_TYPE, m_combo_src_type);
	DDX_Control(pDX, IDC_COMBO_CONVERT_DLG_SRC_PROP0, m_combo_src_prop);
	DDX_Control(pDX, IDC_COMBO_CONVERT_DLG_DST_PROP0, m_combo_dst_prop);
	DDX_Control(pDX, IDC_COMBO_CONVERT_DLG_SRC_PROP1, m_combo_src_prop2);
}


/********************************************************************
�@  �\  ��  �� :
��    ��    �� : make_data_type_name
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::make_data_type_name()
{
	
	m_data_type_name[BYTE_FORMAT].LoadString(IDS_DATATYPE_BYTE);			m_data_type_name[BYTE_FORMAT] += "[BYTE]";
	m_data_type_name[WORD_FORMAT].LoadString(IDS_DATATYPE_WORD);			m_data_type_name[WORD_FORMAT] += "[WORD]";
	m_data_type_name[LONG32_FORMAT].LoadString(IDS_DATATYPE_LONG);		m_data_type_name[LONG32_FORMAT] += "[LONG]";
	m_data_type_name[RGB_FORMAT].LoadString(IDS_DATATYPE_RGB);			m_data_type_name[RGB_FORMAT] += "[RGB]";
	m_data_type_name[FLOAT_FORMAT].LoadString(IDS_DATATYPE_FLOAT);		m_data_type_name[FLOAT_FORMAT] += "[FLOAT]";
	m_data_type_name[COMPLEX_FORMAT].LoadString(IDS_DATATYPE_COMPLEX);	m_data_type_name[COMPLEX_FORMAT] += "[COMPLEX]";
	m_data_type_name[XY_FORMAT].LoadString(IDS_DATATYPE_XY);				m_data_type_name[XY_FORMAT] += "[XY]";
	m_data_type_name[XYZ_FORMAT].LoadString(IDS_DATATYPE_XYZ);			m_data_type_name[XYZ_FORMAT] += "[XYZ]";
	m_data_type_name[THREE_D_FORMAT].LoadString(IDS_DATATYPE_3D);			m_data_type_name[THREE_D_FORMAT] += "[3D]";
	m_data_type_name[SHORT16_FORMAT].LoadString(IDS_DATATYPE_SHORT);		m_data_type_name[SHORT16_FORMAT] += "[SHORT]";
	m_data_type_name[DWORD_FORMAT].LoadString(IDS_DATATYPE_DWORD);		m_data_type_name[DWORD_FORMAT] += "[DWORD]";
	m_data_type_name[FLOAT_3D_FORMAT].LoadString(IDS_DATATYPE_F3D);		m_data_type_name[FLOAT_3D_FORMAT] += "[F3D]";
	m_data_type_name[RGB_3D_FORMAT].LoadString(IDS_DATATYPE_RGB3D);		m_data_type_name[RGB_3D_FORMAT] += "[RGB3D]";
}


/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : make_cvt_type_table
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::make_cvt_type_table()
{
	int data_type[] = { BYTE_FORMAT, RGB_FORMAT, SHORT16_FORMAT, WORD_FORMAT, LONG32_FORMAT, DWORD_FORMAT, FLOAT_FORMAT, XY_FORMAT, XYZ_FORMAT, COMPLEX_FORMAT, THREE_D_FORMAT, FLOAT_3D_FORMAT, RGB_3D_FORMAT };

	for (int j = 0; j < 20; j++){
		for (int i = 0; i < 20; i++){
			m_dst_type_table[j][0][i] = -1;
			m_dst_type_table[j][1][i] = -1;
		}
	}


	for (int i = 0; i < sizeof(data_type) / sizeof(int); i++)
	{
		if (data_type[i] == RGB_FORMAT || data_type[i] == RGB_3D_FORMAT){
			m_dst_type_table[data_type[i]][0][0] = RGB_FORMAT;
		}
		else{
			m_dst_type_table[data_type[i]][0][0] = BYTE_FORMAT;
		}

		int cnt = 0;
		if (data_type[i] != BYTE_FORMAT)	m_dst_type_table[data_type[i]][1][cnt++] = BYTE_FORMAT;
		if (data_type[i] != RGB_FORMAT)		m_dst_type_table[data_type[i]][1][cnt++] = RGB_FORMAT;
		if (data_type[i] != SHORT16_FORMAT)	m_dst_type_table[data_type[i]][1][cnt++] = SHORT16_FORMAT;
		if (data_type[i] != WORD_FORMAT)	m_dst_type_table[data_type[i]][1][cnt++] = WORD_FORMAT;
		if (data_type[i] != LONG32_FORMAT)	m_dst_type_table[data_type[i]][1][cnt++] = LONG32_FORMAT;
		if (data_type[i] != DWORD_FORMAT)	m_dst_type_table[data_type[i]][1][cnt++] = DWORD_FORMAT;
		if (data_type[i] != FLOAT_FORMAT)	m_dst_type_table[data_type[i]][1][cnt++] = FLOAT_FORMAT;
		//if (data_type[i] != XY_FORMAT)		m_dst_type_table[data_type[i]][1][cnt++] = XY_FORMAT;
		if (data_type[i] != XYZ_FORMAT)		m_dst_type_table[data_type[i]][1][cnt++] = XYZ_FORMAT;
		if (data_type[i] != COMPLEX_FORMAT)	m_dst_type_table[data_type[i]][1][cnt++] = COMPLEX_FORMAT;
		m_dst_type_table[data_type[i]][1][cnt++] = THREE_D_FORMAT;
		m_dst_type_table[data_type[i]][1][cnt++] = FLOAT_3D_FORMAT;
		//m_dst_type_table[data_type[i]][1][cnt++] = RGB_3D_FORMAT;
	}

}


/********************************************************************
�@  �\  ��  �� :�ϊ��O�̃f�[�^�̃`���l���E�y�[�W��ݒ肷��
��    ��    �� : show_dlg_item_src_props
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CConvertDataDlg::show_dlg_item_src_props(int image_num)
{
	CDataUnit	*p_du;

	if ((p_du = API.GetDataUnit(image_num)) == NULL)		return false;

	if (m_combo_src_prop.GetCount() > 0){
		while (m_combo_src_prop.DeleteString(0));
	}
	m_combo_src_prop.EnableWindow(false);

	if (m_combo_src_prop2.GetCount() > 0){
		while (m_combo_src_prop2.DeleteString(0));
	}
	m_combo_src_prop2.EnableWindow(false);

	if (m_combo_src_type.GetCurSel() == 1)
	{
		switch (p_du->DataType){
		case BYTE_FORMAT:
			break;

		case THREE_D_FORMAT:
			m_combo_src_prop2.EnableWindow(true);
			for (int n = 0; n<p_du->PageNumber; n++){
				CString s;
				s.Format("%d", n);
				m_combo_src_prop2.AddString(s);
			}
			m_combo_src_prop2.SetCurSel(p_du->DispPage);
			break;

		case SHORT16_FORMAT:
			break;

		case WORD_FORMAT:
			break;

		case LONG32_FORMAT:
			break;

		case DWORD_FORMAT:
			break;

		case FLOAT_FORMAT:
			break;

		case COMPLEX_FORMAT:
			m_combo_src_prop.EnableWindow(true);
			m_combo_src_prop.AddString("����");
			m_combo_src_prop.AddString("����");
			m_combo_src_prop.AddString("�p���[");
			m_combo_src_prop.AddString("�ʑ�");
			m_combo_src_prop.SetCurSel(0);
			break;

		case XYZ_FORMAT:
			m_combo_src_prop.EnableWindow(true);
			m_combo_src_prop.AddString("x");
			m_combo_src_prop.AddString("y");
			m_combo_src_prop.AddString("z");
			m_combo_src_prop.SetCurSel(0);
			break;

		case XY_FORMAT:
			m_combo_src_prop.EnableWindow(true);
			m_combo_src_prop.AddString("x");
			m_combo_src_prop.AddString("y");
			m_combo_src_prop.SetCurSel(0);
			break;

		case FLOAT_3D_FORMAT:
			m_combo_src_prop2.EnableWindow(true);
			for (int n = 0; n<p_du->PageNumber; n++){
				CString s;
				s.Format("%d", n);
				m_combo_src_prop2.AddString(s);
			}
			m_combo_src_prop2.SetCurSel(p_du->DispPage);
			break;

		case RGB_FORMAT:
			m_combo_src_prop.EnableWindow(true);
			m_combo_src_prop.AddString("���x");
			m_combo_src_prop.AddString("�ʓx");
			m_combo_src_prop.AddString("�F��");
			m_combo_src_prop.AddString("R");
			m_combo_src_prop.AddString("G");
			m_combo_src_prop.AddString("B");
			m_combo_src_prop.SetCurSel(0);
			break;

		case RGB_3D_FORMAT:
			m_combo_src_prop.EnableWindow(true);
			m_combo_src_prop.AddString("���x");
			m_combo_src_prop.AddString("�ʓx");
			m_combo_src_prop.AddString("�F��");
			m_combo_src_prop.AddString("R");
			m_combo_src_prop.AddString("G");
			m_combo_src_prop.AddString("B");
			m_combo_src_prop.SetCurSel(0);

			m_combo_src_prop2.EnableWindow(true);
			for (int n = 0; n < p_du->PageNumber; n++){
				CString s;
				s.Format("%d", n);
				m_combo_src_prop2.AddString(s);
			}
			m_combo_src_prop2.SetCurSel(p_du->DispPage);
			break;
		}
	}

	return true;
}


/********************************************************************
�@  �\  ��  �� :�ϊ���̃f�[�^�̃^�C�v��ݒ肷��
��    ��    �� : show_dlg_item_dst_type
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CConvertDataDlg::show_dlg_item_dst_type(int image_num)
{
	CDataUnit	*p_du;

	if ((p_du = API.GetDataUnit(image_num)) == NULL)		return false;



	
	if (m_combo_dst_type.GetCount() > 0){
		while (m_combo_dst_type.DeleteString(0));
	}

	if (m_combo_src_type.GetCurSel() == 0)
	{//�\���摜����ϊ�
		if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT){
			m_combo_dst_type.AddString(m_data_type_name[RGB_FORMAT]);
		} else{
			m_combo_dst_type.AddString(m_data_type_name[BYTE_FORMAT]);
		}
	}
	else if (m_combo_src_type.GetCurSel() == 1)
	{//�f�[�^����ϊ�
		for (int i = 0; i < 20; i++)
		{
			int type = m_dst_type_table[p_du->DataType][1][i];
			if (type >= 0)
			{
				m_combo_dst_type.AddString(m_data_type_name[type]);
			}
		}
	}
	else{
		m_combo_dst_type.AddString(m_data_type_name[BYTE_FORMAT]);
	}

	m_combo_dst_type.SetCurSel(0);

	return true;
}


/********************************************************************
�@  �\  ��  �� :�ϊ���̃f�[�^�̃`���l���E�y�[�W��ݒ肷��
��    ��    �� : show_dlg_item_dst_props
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CConvertDataDlg::show_dlg_item_dst_props(int image_num)
{
	CDataUnit	*p_du;

	if ((p_du = API.GetDataUnit(image_num)) == NULL)		return false;

	if (m_combo_dst_prop.GetCount() > 0){
		while (m_combo_dst_prop.DeleteString(0));
	}

	m_combo_dst_prop.EnableWindow(false);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGENUM), false);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGEOFFSET), false);


	if (m_combo_src_type.GetCurSel() == 1)//�f�[�^����ϊ�
	{
		int type = m_dst_type_table[p_du->DataType][1][m_combo_dst_type.GetCurSel()];
		switch (type)
		{
		case BYTE_FORMAT:
			break;

		case THREE_D_FORMAT:
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGENUM), true);
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGEOFFSET), true);
			API.SetEditValue(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGENUM, p_du->PageNumber);
			API.SetEditValue(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGEOFFSET, 0);
			break;

		case SHORT16_FORMAT:
			break;

		case WORD_FORMAT:
			break;

		case LONG32_FORMAT:
			break;

		case DWORD_FORMAT:
			break;

		case FLOAT_FORMAT:
			break;

		case COMPLEX_FORMAT:
			m_combo_dst_prop.EnableWindow(true);
			m_combo_dst_prop.AddString("����");
			m_combo_dst_prop.AddString("����");
			m_combo_dst_prop.SetCurSel(0);
			break;

		case XYZ_FORMAT:
			break;

		case XY_FORMAT:
			break;

		case FLOAT_3D_FORMAT:
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGENUM), true);
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGEOFFSET), true);
			API.SetEditValue(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGENUM, p_du->PageNumber);
			API.SetEditValue(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGEOFFSET, 0);
			break;

		case RGB_FORMAT:
			m_combo_dst_prop.EnableWindow(true);
			m_combo_dst_prop.AddString("BGB���ׂ�");
			m_combo_dst_prop.AddString("R");
			m_combo_dst_prop.AddString("G");
			m_combo_dst_prop.AddString("B");
			m_combo_dst_prop.SetCurSel(0);
			break;

		case RGB_3D_FORMAT:
			break;
		}
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
bool	CConvertDataDlg::show_dlg_item(int image_num)
{
	CDataUnit	*p_du;

	if ((p_du = API.GetDataUnit(image_num)) == NULL)		return false;

	API.SetEditValue(m_hWnd, IDC_EDIT_CONVERT_DLG_SRC_TYPE, m_data_type_name[p_du->DataType]);

	//�ϊ��O�̃f�[�^�^�C�v�ݒ�
	if (m_combo_src_type.GetCount() > 0){
		while (m_combo_src_type.DeleteString(0));
	}
	m_combo_src_type.AddString("�\���摜");
	m_combo_src_type.AddString("�f�[�^");
	m_combo_src_type.AddString("�}�X�N");
	m_combo_src_type.SetCurSel(0);

	//�ϊ��O�̃f�[�^�i�y�[�W�E�`���l���j�ݒ�
	show_dlg_item_src_props(image_num);

	//�ϊ���̃f�[�^�^�C�v�ݒ�
	show_dlg_item_dst_type(image_num);

	//�ϊ���̃f�[�^�i�y�[�W�E�`���l���j�ݒ�
	show_dlg_item_dst_props(image_num);

	return true;
}



/********************************************************************
�@  �\  ��  �� : �ݒ�ɉ������`���l���E�y�[�W��\������
��    ��    �� : set_src_page_channel
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CConvertDataDlg::set_src_page_channel(int image_num)
{
	CDataUnit	*p_du;

	if ((p_du = API.GetDataUnit(image_num)) == NULL)		return false;


	//�ϊ��O�摜�̐ݒ�

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		switch (m_combo_src_prop.GetCurSel())
		{
		case 0://���x
			p_du->DispRGB = true;
			break;
		case 1://�ʓx
			p_du->DispRGB = true;
			break;
		case 2://�F��
			p_du->DispRGB = true;
			break;
		case 3://R
			p_du->DispRGB = false;
			break;
		case 4://G
			p_du->DispRGB = false;
			break;
		case 5://B
			p_du->DispRGB = false;
			break;
		}
	}

	if (p_du->DataType == COMPLEX_FORMAT || p_du->DataType == XY_FORMAT || p_du->DataType == XYZ_FORMAT)
	{
		p_du->DispChannel = m_combo_src_prop.GetCurSel();
	}

	if (p_du->DataType == THREE_D_FORMAT || p_du->DataType == FLOAT_3D_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		p_du->DispPage = m_combo_src_prop2.GetCurSel();
	}

	return true;
}


/********************************************************************
�@  �\  ��  �� : ���ݑI�𒆂̉摜�ɑ΂��A�^�C�v�ϊ������s����
��    ��    �� : execute
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CConvertDataDlg::execute(int image_num) 
{
	CDataUnit	*p_du;
	if ((p_du = API.GetDataUnit(image_num)) == NULL)		return false;

	CONVERT_DATA_OPTION	option = {0};


	//�ϊ��O�摜�̐ݒ�

	set_src_page_channel(image_num);

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		switch (m_combo_src_prop.GetCurSel())
		{
		case 0://���x
			option.color_gray_method = 0;
			break;
		case 1://�ʓx
			option.color_gray_method = 1;
			break;
		case 2://�F��
			option.color_gray_method = 2;
			break;
		}
	}
	

	//�ϊ���摜�̐ݒ�
	int type = m_dst_type_table[p_du->DataType][m_combo_src_type.GetCurSel()][m_combo_dst_type.GetCurSel()];
	option.type_to = type;

	if (m_combo_src_type.GetCurSel() == 0){
		option.byte_from = 1;
	}
	else if (m_combo_src_type.GetCurSel() == 2){
		option.byte_from = 2;
	}
	else{
		if (type == RGB_FORMAT)
		{
			option.rgb_to = m_combo_dst_prop.GetCurSel();
		}
		else if (type == COMPLEX_FORMAT)
		{
			option.comp_to = m_combo_src_prop.GetCurSel();
		}
		else if (type == THREE_D_FORMAT || type == FLOAT_3D_FORMAT)
		{
			API.GetEditValue(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGENUM, &option.threeD_page);
			API.GetEditValue(m_hWnd, IDC_EDIT_CONVERT_DLG_PAGEOFFSET, &option.threeD_offset);
		}
	}

	option.satulate = API.GetCheck(m_hWnd, IDC_CHECK_CONVERT_SATULATE);//�㉺���������Ƃ��O�a�����邩�ǂ���



	API.BackupDataUnit(CURRENT_IMAGE);//���݂̃f�[�^�o�b�N�A�b�v

	//�ϊ����s
	if(!API.ConvertDataUnit(CURRENT_IMAGE, option, true))	return false;

	API.DrawImage();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CConvertDataDlg, CDialog)
	//{{AFX_MSG_MAP(CConvertDataDlg)

	//}}AFX_MSG_MAP

	ON_CBN_SELCHANGE(IDC_COMBO_CONVERT_DLG_SRC_TYPE, &CConvertDataDlg::OnCbnSelchangeComboConvertDlgSrcType)
	ON_CBN_SELCHANGE(IDC_COMBO_CONVERT_DLG_SRC_PROP0, &CConvertDataDlg::OnCbnSelchangeComboConvertDlgSrcProp0)
	ON_CBN_SELENDCANCEL(IDC_COMBO_CONVERT_DLG_SRC_PROP1, &CConvertDataDlg::OnCbnSelendcancelComboConvertDlgSrcProp1)
	ON_CBN_SELCHANGE(IDC_COMBO_CONVERT_DLG_DST_TYPE, &CConvertDataDlg::OnCbnSelchangeComboConvertDlgDstType)
	ON_CBN_SELCHANGE(IDC_COMBO_CONVERT_DLG_DST_PROP0, &CConvertDataDlg::OnCbnSelchangeComboConvertDlgDstProp0)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT_DLG_EXECUTE, &CConvertDataDlg::OnBnClickedButtonConvertDlgExecute)
	ON_BN_CLICKED(IDCANCEL, &CConvertDataDlg::OnBnClickedCancel)
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
BOOL CConvertDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	make_data_type_name();
	make_cvt_type_table();

	show_dlg_item(CURRENT_IMAGE);

	API.SetCheck(m_hWnd, IDC_CHECK_CONVERT_SATULATE,true);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}



/********************************************************************
�@  �\  ��  �� : �ϊ��O�̃^�C�v�I���i�\���摜�^�f�[�^�j�̃R���{�{�b�N�X�̒l���ύX���ꂽ
��    ��    �� : OnCbnSelchangeComboConvertDlgSrcType
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::OnCbnSelchangeComboConvertDlgSrcType()
{
	CDataUnit	*p_du;
	if ((p_du = API.GetDataUnit(CURRENT_IMAGE)) == NULL)		return;

	//�ϊ��O�̃f�[�^�i�y�[�W�E�`���l���j�ݒ�
	show_dlg_item_src_props(CURRENT_IMAGE);

	//�ϊ���̃f�[�^�^�C�v�ݒ�
	show_dlg_item_dst_type( CURRENT_IMAGE);

	//�ϊ���̃f�[�^�i�y�[�W�E�`���l���j�ݒ�
	show_dlg_item_dst_props( CURRENT_IMAGE);

	API.DrawImage(CURRENT_IMAGE);
}

/********************************************************************
�@  �\  ��  �� : �ϊ��O�̃`���l���I���̃R���{�{�b�N�X�̒l���ύX���ꂽ
��    ��    �� : OnCbnSelchangeComboConvertDlgSrcProp0
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::OnCbnSelchangeComboConvertDlgSrcProp0()
{

}

/********************************************************************
�@  �\  ��  �� : �ϊ��O�̃y�[�W�I���̃R���{�{�b�N�X�̒l���ύX���ꂽ
��    ��    �� : OnCbnSelendcancelComboConvertDlgSrcProp1
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::OnCbnSelendcancelComboConvertDlgSrcProp1()
{

}

/********************************************************************
�@  �\  ��  �� : �ϊ���̃^�C�v�I���̃R���{�{�b�N�X�̒l���ύX���ꂽ
��    ��    �� : OnCbnSelchangeComboConvertDlgDstType
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::OnCbnSelchangeComboConvertDlgDstType()
{
	CDataUnit	*p_du;
	if ((p_du = API.GetDataUnit(CURRENT_IMAGE)) == NULL)		return;

	//�ϊ���̃f�[�^�i�y�[�W�E�`���l���j�ݒ�
	show_dlg_item_dst_props( CURRENT_IMAGE);
}

/********************************************************************
�@  �\  ��  �� : �ϊ���̃`���l���I���̃R���{�{�b�N�X�̒l���ύX���ꂽ
��    ��    �� : OnCbnSelchangeComboConvertDlgDstProp0
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::OnCbnSelchangeComboConvertDlgDstProp0()
{

}


/********************************************************************
�@  �\  ��  �� : ���s�{�^�����������ꂽ
��    ��    �� : OnBnClickedButtonConvertDlgExecute
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::OnBnClickedButtonConvertDlgExecute()
{
	execute(CURRENT_IMAGE);

	show_dlg_item(CURRENT_IMAGE);

	CDialog::OnOK();
}

/********************************************************************
�@  �\  ��  �� : �L�����Z���{�^�����������ꂽ
��    ��    �� : OnBnClickedButtonConvertDlgExecute
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CConvertDataDlg::OnBnClickedCancel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CDialog::OnCancel();
}