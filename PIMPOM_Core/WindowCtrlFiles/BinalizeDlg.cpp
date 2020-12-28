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
//CBinalizeDlg
//�Q�l���ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "BinalizeDlg.h"

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
IMPLEMENT_DYNAMIC(CBinalizeDlg, CDialog)

CBinalizeDlg::CBinalizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinalizeDlg::IDD, pParent)
{

}

CBinalizeDlg::~CBinalizeDlg()
{
}

void CBinalizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_BINALIZE_TYPE, m_combo_binalize_type);
	DDX_Control(pDX, IDC_COMBO_BINALIZE_DYN_SIZE, m_combo_binalize_dyn_size);
	DDX_Control(pDX, IDC_SLIDER_BINALIZE_PARAM0, m_slider_binalize_param0);
	DDX_Control(pDX, IDC_SLIDER_BINALIZE_PARAM1, m_slider_binalize_param1);
	DDX_Control(pDX, IDC_SLIDER_BINALIZE_PARAM2, m_slider_binalize_param3);
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
void CBinalizeDlg::show_dlg_item()
{
	bool to_other_image=false, do_label=false;


	if(API.GetCheck(m_hWnd,IDC_RADIO_BINALIZE_TO_MASK)){
	}else{
		if(API.GetCheck(m_hWnd,IDC_CHECK_BINALIZE_LABEL)){
			do_label = true;
		}
		to_other_image = true;
	}

	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_FROM_IMAGE_NO), to_other_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_TO_IMAGE_NO), to_other_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_CHECK_BINALIZE_LABEL), to_other_image);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_LABEL_AREA_MIN), do_label);
}

/********************************************************************
�@  �\  ��  �� : 2�l�����s
��    ��    �� : execute
��          �� :
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CBinalizeDlg::execute()
{
	int		src_num=0,dst_num=1;
	int     pad_type;
	double	min_thresh=0, max_thresh=999999;
	bool	inverse=false;
	bool	multi = false;


	API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_FROM_IMAGE_NO, &src_num);	
	API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_TO_IMAGE_NO, &dst_num);	
	inverse=API.GetCheck(m_hWnd, IDC_CHECK_BINALIZE_INVERSE);
	multi = API.GetCheck(m_hWnd, IDC_CHECK_BINALIZE_FOR_MULTI);
	
	if (API.GetCheck(m_hWnd, IDC_RADIO_BINALIZE_PAD1)) {
		pad_type = 1;
	}
	else if (API.GetCheck(m_hWnd, IDC_RADIO_BINALIZE_PAD2)) {
		pad_type = 2;
	}
	else {
		pad_type = 0;
	}


	

	if(API.GetCheck(m_hWnd,IDC_RADIO_BINALIZE_TO_MASK))
	{//���ʂ��}�X�N�ɓ����
		if(!API.Binalize(CURRENT_IMAGE, min_thresh, max_thresh, inverse))	return false;
		API.DrawImage(CURRENT_IMAGE);
	}
	else
	{//���ʂ�ʂ̃������ɓ����
		if(src_num==dst_num){
			API.MessageBox("���ʏo�͐�ɂ͓����摜���������w��ł��܂���");
			return false;
		}

		API.BackupDataUnit(dst_num);//���݂̃f�[�^�o�b�N�A�b�v

		if(API.GetCheck(m_hWnd,IDC_RADIO_BINALIZE0) )//�O���[�o��臒l
		{
			switch(m_combo_binalize_type.GetCurSel())
			{
			case 0://臒l�����
				API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, &min_thresh);
				API.Binalize(src_num, dst_num, min_thresh, max_thresh, inverse, 255, 0, false, multi);
				break;

			case 1://��Ö@
				min_thresh = API.BinalizeThresholdOtsu(src_num);
				API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, min_thresh);
				API.Binalize(src_num, dst_num, min_thresh, max_thresh, inverse, 255, 0, false, multi);
				break;

			case 2://P�^�C���@
				{
					double ptile;
					API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM1, &ptile);
					min_thresh = API.BinalizeThresholdPTile(src_num, ptile);
				}
				API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, min_thresh);
				API.Binalize(src_num, dst_num, min_thresh, max_thresh, inverse, 255, 0, false, multi);
				break;
			}
		}
		else//���[�J��臒l
		{
			switch(m_combo_binalize_type.GetCurSel())
			{
			case 0://�K�E�V�A�����ȍ���
				{
					int filter_size, offset;
					API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM3, &offset);
					filter_size = m_binalize_dyn_size[ m_combo_binalize_dyn_size.GetCurSel()];

					API.BinalizeLocalGauss(src_num, dst_num, filter_size, offset, inverse, 255, 0, pad_type, false, multi);
				}
				break;

			case 1://LOG
				{
					int filter_size, offset;
					API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM3, &offset);
					filter_size = m_binalize_dyn_size[ m_combo_binalize_dyn_size.GetCurSel()];

					API.BinalizeLocalLaplaceGauss(src_num, dst_num, filter_size, offset, inverse, 255, 0, pad_type, false, multi);
				}
				break;

			case 2://NICK�̎�@
			{
				int filter_size;
				float k;
				API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_NICK_K, &k);
				filter_size = m_binalize_dyn_size[m_combo_binalize_dyn_size.GetCurSel()];

				API.BinalizeNick(src_num, dst_num, filter_size, k, inverse, 255, 0, false, multi);
			}
				break;
			}
		}

		API.DrawImage(dst_num);//�`��


		//���x�����O

		if(API.GetCheck(m_hWnd,IDC_CHECK_BINALIZE_LABEL))
		{
			int     label_image_no;
			int		min_area=0;
			int		area[BINALIZE_LABEL_MAX] = { 0 };
			float	gx[BINALIZE_LABEL_MAX] = { 0 };
			float	gy[BINALIZE_LABEL_MAX] = { 0 };

			//���x�����ʉ摜�ԍ�
			API.GetEditValue(m_hWnd, IDC_EDIT_LABEL_INAGE_NO, &label_image_no);

			if(label_image_no == src_num || label_image_no == dst_num)
			{
				API.MessageBox("���ʏo�͐�ɂ͓����摜���������w��ł��܂���");
				return false;
			}
			else
			{

				API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_LABEL_AREA_MIN, &min_area);

				int labelCnt = API.Label(dst_num, label_image_no, true, min_area, area, gx, gy, 255, 0, false, multi);
				if (labelCnt < 0)	return false;

				CDataUnit *pdu;
				if (!(pdu = API.GetDataUnit(label_image_no)))return false;
				pdu->ChangePalette(PALETTE_LABEL);//���x�����킩��₷���悤�ȃJ���[�p���b�g�ɕύX

				if (!multi)
				{
					//���x���ʒu��\������
					for (int n = 0; n < labelCnt && n < MARK_NUMBER_MAX; n++)
					{
						double attrib[MARK_ATTRIB_NUM] = { 0 };
						attrib[0] = area[n];

						API.AddMark(label_image_no, gx[n], gy[n], attrib);
					}
				}

				API.DrawImage(label_image_no);//�`��
			}
		}


		
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBinalizeDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBinalizeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BINALIZE_APPLY, &CBinalizeDlg::OnBnClickedButtonBinalizeApply)
	ON_BN_CLICKED(IDC_RADIO_BINALIZE_TO_MASK, &CBinalizeDlg::OnBnClickedRadioBinalizeToMask)
	ON_BN_CLICKED(IDC_RADIO_BINALIZE_TO_NEW_IMAGE, &CBinalizeDlg::OnBnClickedRadioBinalizeToNewImage)
	ON_BN_CLICKED(IDC_CHECK_BINALIZE_LABEL, &CBinalizeDlg::OnBnClickedCheckBinalizeLabel)
	ON_CBN_EDITCHANGE(IDC_COMBO_BINALIZE_TYPE, &CBinalizeDlg::OnCbnEditchangeComboBinalizeType)
	ON_BN_CLICKED(IDC_RADIO_BINALIZE0, &CBinalizeDlg::OnBnClickedRadioBinalize0)
	ON_BN_CLICKED(IDC_RADIO_BINALIZE1, &CBinalizeDlg::OnBnClickedRadioBinalize1)
	ON_EN_CHANGE(IDC_EDIT_BINALIZE_PARAM0, &CBinalizeDlg::OnEnChangeEditBinalizeParam0)
	ON_CBN_SELCHANGE(IDC_COMBO_BINALIZE_TYPE, &CBinalizeDlg::OnCbnEditchangeComboBinalizeType)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_BINALIZE_PARAM1, &CBinalizeDlg::OnEnChangeEditBinalizeParam1)
	ON_BN_CLICKED(IDC_BUTTON_BINALIZE_SET_ORG_IMG, &CBinalizeDlg::OnBnClickedButtonBinalizeSetOrgImg)
	ON_EN_CHANGE(IDC_EDIT_BINALIZE_PARAM3, &CBinalizeDlg::OnEnChangeEditBinalizeParam3)
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
BOOL CBinalizeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//if( API.GetDataUnit(CURRENT_IMAGE) ==NULL)	return false;


	//�u���ʂ�ʃ������֓����v�������l�ɂ���
	API.SetCheck(m_hWnd, IDC_RADIO_BINALIZE_TO_NEW_IMAGE, true);
	show_dlg_item();


	//���ʂ�����摜�������ԍ���ݒ�
	int dst_num, label_num, src_num; 

	src_num = API.GetCurrentImageNumber();
	dst_num = src_num + 1;
	label_num = src_num + 2;

	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_FROM_IMAGE_NO, src_num);	
	if(dst_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_TO_IMAGE_NO, dst_num);	
	if(label_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_LABEL_INAGE_NO, label_num);	
	


	//�u�O���[�o��2�l���v�������l�ɂ���
	API.SetCheck(m_hWnd, IDC_RADIO_BINALIZE0, true);
	OnBnClickedRadioBinalize0();


	//�e��p�����^�̏����l�ݒ�

	//�O���[�o��臒l
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, 128);	
	m_slider_binalize_param0.SetRange(0,255);
	m_slider_binalize_param0.SetPos(128);

	//�p�[�Z���^�C��
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM1, 50);	
	m_slider_binalize_param1.SetRange(0,100);
	m_slider_binalize_param1.SetPos(50);

	//���I2�l���̃t�B���^�T�C�Y
	for(int i=0, filter_size = 3 ; filter_size <= COMB_FILTER_SIZE_MAX  ; i++, filter_size+=2)
	{
		CString str;	str.Format("%d�~%d", filter_size, filter_size);
		m_combo_binalize_dyn_size.AddString( str);		
		m_binalize_dyn_size[i] = filter_size;
	}
	m_combo_binalize_dyn_size.SetCurSel(0);

	//padding
	API.SetCheck(m_hWnd, IDC_RADIO_BINALIZE_PAD0, true);

	//Nick��K
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_NICK_K, -0.03);

	//���I2�l���̃I�t�Z�b�g
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM3, 0);
	m_slider_binalize_param3.SetRange(-128,128);
	m_slider_binalize_param3.SetPos(0);

	//���x�����O�ŏ��ʐ�臒l
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_LABEL_AREA_MIN, 10);	

	



	return true;
}



/********************************************************************
�@  �\  ��  �� :����{�^������
��    ��    �� : OnBnClickedOk
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnBnClickedOk()
{
	//execute();
	OnOK();
}

/********************************************************************
�@  �\  ��  �� :���s�{�^������
��    ��    �� : OnBnClickedButtonBinalizeApply
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnBnClickedButtonBinalizeApply()
{
	execute();
}

/********************************************************************
�@  �\  ��  �� :�u�Q�l�摜���}�X�N�֓����v���W�I�{�^���I��
��    ��    �� : OnBnClickedRadioBinalizeToMask
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnBnClickedRadioBinalizeToMask()
{
	show_dlg_item();
}

/********************************************************************
�@  �\  ��  �� :�u�Q�l�摜��ʂ̉摜�������֓����v���W�I�{�^���I��
��    ��    �� : OnBnClickedRadioBinalizeToNewImage
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnBnClickedRadioBinalizeToNewImage()
{
	show_dlg_item();
}

/********************************************************************
�@  �\  ��  �� :�u���x�����O����v�`�F�b�N�{�b�N�X�ύX
��    ��    �� : OnBnClickedCheckBinalizeLabel
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnBnClickedCheckBinalizeLabel()
{
	show_dlg_item();
}





/********************************************************************
�@  �\  ��  �� : �O���[�o��2�l�����W�I�{�^���I��
��    ��    �� : OnBnClickedRadioBinalize0
��          �� : 
��    ��    �l : 
�@          �\ : �Œ�臒l�I��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnBnClickedRadioBinalize0()
{
	if(m_combo_binalize_type.GetCount()>0 ){
		while(m_combo_binalize_type.DeleteString(0));//�A���S�^�C�v�R���{�{�b�N�X�̑S�v�f���N���A
	}

	m_combo_binalize_type.AddString("臒l�����");				//0
	m_combo_binalize_type.AddString("���ʕ���(��Ö@)");		//1
	m_combo_binalize_type.AddString("P�^�C���@");				//2
	m_combo_binalize_type.SetCurSel(0);

	OnCbnEditchangeComboBinalizeType();
}

/********************************************************************
�@  �\  ��  �� : �Ǐ�2�l�����W�I�{�^���I��
��    ��    �� : OnBnClickedRadioBinalize0
��          �� : 
��    ��    �l : 
�@          �\ : �Œ�臒l�I��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnBnClickedRadioBinalize1()
{
	if(m_combo_binalize_type.GetCount()>0 ){
		while(m_combo_binalize_type.DeleteString(0));//�t�B���^�T�C�Y�R���{�{�b�N�X�̑S�v�f���N���A
	}

	m_combo_binalize_type.AddString("�K�E�V�A�����ȍ���");	//0
	m_combo_binalize_type.AddString("���v���V�A���I�u�K�E�V�A��");		//1
	m_combo_binalize_type.AddString("NICK�@");				//2
	m_combo_binalize_type.SetCurSel(0);

	OnCbnEditchangeComboBinalizeType();
}


/********************************************************************
�@  �\  ��  �� : 2�l���A���S�I���̃R���{�{�b�N�X�ύX
��    ��    �� : OnCbnEditchangeComboBinalizeType
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnCbnEditchangeComboBinalizeType()
{
	//�p�����^�\��UI�����������\��
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM0), SW_HIDE);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), SW_HIDE);
	m_slider_binalize_param0.ShowWindow(SW_HIDE);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM1), SW_HIDE);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM1), SW_HIDE);
	m_slider_binalize_param1.ShowWindow(SW_HIDE);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM2), SW_HIDE);
	m_combo_binalize_dyn_size.ShowWindow(SW_HIDE);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM3), SW_HIDE);
	::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM3), SW_HIDE);	
	m_slider_binalize_param3.ShowWindow(SW_HIDE);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_NICK_K), SW_HIDE);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_NICK_K), SW_HIDE);
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PAD), SW_HIDE);//�upadding�v�O���[�v�{�b�N�X
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD1), SW_HIDE);//�u�[���v���W�I�{�^��
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD2), SW_HIDE);//�u���Ӊ�f�v���W�I�{�^��
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD0), SW_HIDE);//�u�܂�Ԃ��v���W�I�{�^��


	if(API.GetCheck(m_hWnd,IDC_RADIO_BINALIZE0) )//�O���[�o��臒l
	{
		::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM0), SW_SHOW);
		::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), SW_SHOW);
		
		switch(m_combo_binalize_type.GetCurSel())
		{
		case 0://臒l�����
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), true);
			m_slider_binalize_param0.ShowWindow(SW_SHOW);
			break;

		case 1://��Ö@
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), false);
			m_slider_binalize_param0.ShowWindow(SW_HIDE);
			execute();
			break;

		case 2://P-�^�C���@
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), false);
			m_slider_binalize_param0.ShowWindow(SW_HIDE);
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM1), SW_SHOW);
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM1), SW_SHOW);
			m_slider_binalize_param1.ShowWindow(SW_SHOW);
			execute();
			break;

		}
	}
	else//�Ǐ�臒l
	{
		switch(m_combo_binalize_type.GetCurSel())
		{
		case 0://�K�E�V�A�����ȍ���
			//not break;
		case 1://LOG
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM2), SW_SHOW);
			m_combo_binalize_dyn_size.ShowWindow(SW_SHOW);
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM3), SW_SHOW);
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM3), SW_SHOW);	
			m_slider_binalize_param3.ShowWindow(SW_SHOW);
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PAD), SW_SHOW);//�upadding�v�O���[�v�{�b�N�X
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD1), SW_SHOW);//�u�[���v���W�I�{�^��
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD2), SW_SHOW);//�u���Ӊ�f�v���W�I�{�^��
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD0), SW_SHOW);//�u�܂�Ԃ��v���W�I�{�^��
			break;

		case 2://Nick�̕��@
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM2), SW_SHOW);
			m_combo_binalize_dyn_size.ShowWindow(SW_SHOW);
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_NICK_K), SW_SHOW);
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_NICK_K), SW_SHOW);
			break;
		default:
			break;
		}
	}
}

/********************************************************************
�@  �\  ��  �� : �p�����^0=�O���[�o��2�l����臒l�@�l�ύX
��    ��    �� : OnEnChangeEditBinalizeParam0
��          �� : 
��    ��    �l : 
�@          �\ : �Œ�臒l�I��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnEnChangeEditBinalizeParam0()
{
	int val;
	API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, &val);

	m_slider_binalize_param0.SetPos(val);//�X���C�h�o�[�ɓ��l���f
}

/********************************************************************
�@  �\  ��  �� : �p�����^1=�p�[�Z���^�C���ݒ�ύX
��    ��    �� : OnEnChangeEditBinalizeParam1
��          �� : 
��    ��    �l : 
�@          �\ : �Œ�臒l�I��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnEnChangeEditBinalizeParam1()
{
	int val;
	API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM1, &val);

	m_slider_binalize_param1.SetPos(val);//�X���C�h�o�[�ɓ��l���f
}

/********************************************************************
�@  �\  ��  �� : �p�����^3=���I2�l���I�t�Z�b�g
��    ��    �� : OnEnChangeEditBinalizeParam3
��          �� : 
��    ��    �l : 
�@          �\ : �Œ�臒l�I��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnEnChangeEditBinalizeParam3()
{
	int val;
	API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM3, &val);

	m_slider_binalize_param3.SetPos(val);//�X���C�h�o�[�ɓ��l���f
}

/********************************************************************
�@  �\  ��  �� : �X���C�h�o�[����
��    ��    �� : OnHScroll
��          �� : 
��    ��    �l : 
�@          �\ : �Œ�臒l�I��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(pScrollBar == (CScrollBar*)&m_slider_binalize_param0)//�O���[�o��臒l�ݒ�
	{
		int val = m_slider_binalize_param0.GetPos();
		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, val);

		execute();//���s
	}
	else if(pScrollBar == (CScrollBar*)&m_slider_binalize_param1)//P�^�C���ݒ�
	{
		int val = m_slider_binalize_param1.GetPos();
		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM1, val);

		execute();//���s
	}
	else if(pScrollBar == (CScrollBar*)&m_slider_binalize_param3)//�I�t�Z�b�g
	{
		int val = m_slider_binalize_param3.GetPos();
		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM3, val);

		execute();//���s
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


/********************************************************************
�@  �\  ��  �� : �\���摜�����摜�ɂ���
��    ��    �� : OnBnClickedButtonBinalizeSetOrgImg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBinalizeDlg::OnBnClickedButtonBinalizeSetOrgImg()
{
	int orgNo;

	orgNo = API.GetCurrentImageNumber();

	if(orgNo>=0 && orgNo+2<API.GetDataUnitNumber())
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_FROM_IMAGE_NO, orgNo);
		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_TO_IMAGE_NO, orgNo+1);
		API.SetEditValue(m_hWnd, IDC_EDIT_LABEL_INAGE_NO, orgNo+2);
	}
}
