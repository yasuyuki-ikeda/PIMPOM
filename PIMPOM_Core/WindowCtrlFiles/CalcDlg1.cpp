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
//CCalcDlg
//���Z���ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "CalcDlg1.h"

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

CCalcDlg::CCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcDlg)

	//}}AFX_DATA_INIT
}


void CCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcDlg)
	DDX_Control(pDX, IDC_COMBO_CALC_COMMAND, m_combo_command);
	DDX_Control(pDX, IDC_PROG_CALC, m_prog);
	//}}AFX_DATA_MAP
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
void CCalcDlg::show_dlg_item()
{
	bool enable_new_image=false, enable_mask_image=false;

	if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_NEW_IMAGE)){//�V�����摜�������擾
		enable_new_image = true;
		enable_mask_image=false;
	}else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_OVERWRITE_IMAGE)){//���݂̃������ɐݒ�
		enable_new_image = false;
		enable_mask_image=false;
	}else{//�}�X�N����
		enable_new_image = false;
		enable_mask_image=true;
	}

	//�V�����摜���������擾���邽�߂̐ݒ�l
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_W), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_H), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_BYTE), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_SHORT), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_WORD), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_LONG), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_DWORD), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_FLOAT), enable_new_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_RGB), enable_new_image);

	//���f���ɏ������ނ��߂̐ݒ�l
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALC_MASK_MIN), enable_mask_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CALC_MASK_MAX), enable_mask_image);
}


/********************************************************************
�@  �\  ��  �� : �_�C�A���O�A�C�e���őI������Ă���f�[�^�^�C�v���擾
��    ��    �� : get_data_type_from_dlg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCalcDlg::get_data_type_from_dlg()
{
	if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_BYTE)){		return BYTE_FORMAT;		}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_SHORT)){	return SHORT16_FORMAT;	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_WORD)){	return WORD_FORMAT;		}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_LONG)){	return LONG32_FORMAT;	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_DWORD)){	return DWORD_FORMAT;	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_FLOAT)){	return FLOAT_FORMAT;	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_RGB)){	return RGB_FORMAT;		}

	return BYTE_FORMAT;
}


/********************************************************************
�@  �\  ��  �� : �R�}���h��������o�b�N�A�b�v
��    ��    �� : backup_com_str
��          �� : CComboBox &combo�@(in)�R�}���h������̓������R���{�{�b�N�X���Q��
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalcDlg::backup_com_str(CComboBox &combo)
{
	CString str,b_str;

	combo.GetWindowText(str);

	for(int i=0 ; i<combo.GetCount() ; i++){//�o�b�N�A�b�v���ꂽ�������Ȃ������ׂ�
		combo.GetLBText(i,b_str);

		if(b_str == str){
			return;//���łɃo�b�N�A�b�v����Ă���Ώ����I��
		}
	}

	combo.InsertString(0,str);//�o�b�N�A�b�v����ĂȂ���΃��X�g�ɒǉ�
}

/********************************************************************
�@  �\  ��  �� : �S�ẴR�}���h��������o�b�N�A�b�v
��    ��    �� : backup_all_com_str
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalcDlg::backup_all_com_str()
{
	backup_com_str(m_combo_command);
}

/********************************************************************
�@  �\  ��  �� : �R�}���h������̗�����ini�t�@�C�����烍�[�h
��    ��    �� : load_com_str
��          �� : CComboBox &combo�@ (in)�R�}���h������̓������R���{�{�b�N�X���Q��
                 CString param_name (in)ini�t�@�C���̃p�X
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalcDlg::load_com_str(CComboBox &combo, CString param_name)
{
	CString buf, key, b_str;
	bool	do_insert;

	for(int i=0 ; i<10 ; i++){
		key.Format("_history%d",i);
		key = param_name + key;
		buf = API.GetProfile("CALCLATION", key);
		if(!buf.IsEmpty())
		{
			do_insert = true;

			for(int j=0 ; j<combo.GetCount() ; j++){//�o�b�N�A�b�v���ꂽ�������Ȃ������ׂ�
				b_str.Empty();
				combo.GetLBText(j,b_str);
				if(b_str == buf){
					do_insert = false;
					break;
				}
			}

			if(do_insert){
				combo.AddString(buf);
			}
		}
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : �R�}���h������̗�����ini�t�@�C���փZ�[�u
��    ��    �� : save_com_str
��          �� : CComboBox &combo�@ (in)�R�}���h������̓������R���{�{�b�N�X���Q��
                 CString param_name (in)ini�t�@�C���̃p�X
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalcDlg::save_com_str(CComboBox &combo, CString param_name)
{
	CString buf, key;

	for(int i=0 ; i<combo.GetCount() && i<10 ; i++){
		combo.GetLBText(i,buf);
		key.Format("_history%d",i);
		key = param_name + key;
		API.WriteProfile("CALCLATION", key, buf);
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : ���Z�������s���A���ݑI�𒆂̉摜�������ɓ����
��    ��    �� : execute
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalcDlg::execute()
{
	CString			com_str, err_str;
	bool			ret;

	m_combo_command.GetWindowText(com_str);//�R�}���h������;

	//���݂̃f�[�^�o�b�N�A�b�v
	if( API.GetDataUnit(CURRENT_IMAGE) != NULL ){
		API.BackupDataUnit(CURRENT_IMAGE);
	}


	//���Z���s
	if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_NEW_IMAGE))//�V�����摜�������擾
	{
		int size_w=0, size_h=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_W, &size_w);
		API.GetEditValue(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_H, &size_h);
		ret = API.CalcDataValue(CURRENT_IMAGE, get_data_type_from_dlg(), CSize(size_w, size_h), com_str, 
			API.GetCheck(m_hWnd,IDC_CHECK_CALC_SATULATE) , &err_str, &m_prog);
	}
	else if(API.GetCheck(m_hWnd, IDC_RADIO_CALC_OVERWRITE_IMAGE))//���݂̃������ɏ㏑��
	{
		ret = API.CalcDataValue(CURRENT_IMAGE, com_str, API.GetCheck(m_hWnd,IDC_CHECK_CALC_SATULATE) ,&err_str, &m_prog);
	}
	else
	{//�}�X�N����
		double	mask_max=256, mask_min=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_CALC_MASK_MIN, &mask_min);
		API.GetEditValue(m_hWnd, IDC_EDIT_CALC_MASK_MAX, &mask_max);

		ret = API.CalcMaskValue(CURRENT_IMAGE, com_str, mask_max, mask_min, &err_str, &m_prog);
	}

	
	if(ret){//���Z�����܂��������猋�ʉ摜��\��
		API.DrawImage();
	}else{//���s������G���[���b�Z�[�W�\��
		AfxMessageBox(err_str);
		return false;
	}

	backup_all_com_str();//�R�}���h������̃o�b�N�A�b�v	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCalcDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CALC_EXEC, OnCalcExec)
	ON_BN_CLICKED(IDC_BUTTON_CALC_SHOW_MACRO_HELP, &CCalcDlg::OnBnClickedShowMacroHelp)
	ON_BN_CLICKED(IDC_RADIO_CALC_NEW_IMAGE, &CCalcDlg::OnBnClickedRadioNew)
	ON_BN_CLICKED(IDC_RADIO_CALC_OVERWRITE_IMAGE, &CCalcDlg::OnBnClickedRadioOverwrite)
	ON_BN_CLICKED(IDC_RADIO_CALC_OVERWRITE_IMAGE2, &CCalcDlg::OnBnClickedRadioCalcOverwriteImage2)
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
BOOL CCalcDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	

	//�_�C�A���O�A�C�e���̏�����
	API.SetEditValue(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_W, API.main_image_size.cx);
	API.SetEditValue(m_hWnd, IDC_EDIT_CALC_IMAGE_SIZE_H, API.main_image_size.cy);
	API.SetCheck(m_hWnd, IDC_RADIO_CALC_NEW_IMAGE, true);
	API.SetCheck(m_hWnd, IDC_RADIO_CALC_OVERWRITE_IMAGE, false);
	API.SetCheck(m_hWnd, IDC_RADIO_CALC_DATA_TYPE_BYTE, true);
	API.SetEditValue(m_hWnd, IDC_EDIT_CALC_MASK_MIN, 0);
	API.SetEditValue(m_hWnd, IDC_EDIT_CALC_MASK_MAX, 256);
	API.SetCheck(m_hWnd, IDC_CHECK_CALC_SATULATE,true);

	//�r�b�g�}�b�v�{�^��
	execbmp.LoadBitmap(IDB_BUTTON_3D_0);
	((CButton*)GetDlgItem(IDC_BUTTON_CALC_EXEC))->SetBitmap((HBITMAP)execbmp);

	//���Z���̗�����ǂݍ���
	load_com_str(m_combo_command, CString("value"));

	show_dlg_item();//���͉ۏ�Ԃ̐ݒ�

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�̔j��
��    ��    �� : OnDestroy
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalcDlg::OnDestroy() 
{
	save_com_str(m_combo_command, CString("value"));

	CDialog::OnDestroy();
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
void CCalcDlg::OnClose()
{
	CDialog::OnClose();

	API.SetCalcToolMode(false);
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
void CCalcDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

/********************************************************************
�@  �\  ��  �� : ���Z���̎��s�{�^������
��    ��    �� : OnCalcExec
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalcDlg::OnCalcExec() 
{
	execute();
}

/********************************************************************
�@  �\  ��  �� : �}�N���ꗗ�\���{�^������
��    ��    �� : OnBnClickedShowMacroHelp
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalcDlg::OnBnClickedShowMacroHelp()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\macro_help.txt", NULL, NULL, SW_SHOWNORMAL);
}

/********************************************************************
�@  �\  ��  �� : �u�V�����摜���������擾����v�`�F�b�N�{�b�N�X�̒l��ύX
��    ��    �� : OnBnClickedRadioNew
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalcDlg::OnBnClickedRadioNew()
{
	show_dlg_item();
}

/********************************************************************
�@  �\  ��  �� : �u���݂̉摜�������ɏ������ށv�`�F�b�N�{�b�N�X�̒l��ύX
��    ��    �� : OnBnClickedRadioOverwrite
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalcDlg::OnBnClickedRadioOverwrite()
{
	show_dlg_item();
}

/********************************************************************
�@  �\  ��  �� : �u���Z���ʂ����Ɍ��ݑI�𒆂̉摜�̃}�X�N�𐶐�����v�`�F�b�N�{�b�N�X�̒l��ύX
��    ��    �� : OnBnClickedRadioCalcOverwriteImage2
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalcDlg::OnBnClickedRadioCalcOverwriteImage2()
{
	show_dlg_item();
}
