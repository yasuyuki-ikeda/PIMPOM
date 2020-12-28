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
//２値化設定ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "BinalizeDlg.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************

// *************************************
//         外    部    宣    言         
// *************************************

// *************************************
//         構  造  体   定   義         
// *************************************

// *************************************
//         静    的    変    数         
// *************************************

// *************************************
//         外    部    変    数         
// *************************************
extern	CPimpomAPI	API;

// *************************************
//         関    数    参    照         
// *************************************


//////////////////////////////////////////////////////////////////////////////////////////
//  メンバ関数
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
機  能  名  称 : ダイアログアイテムの表示状態変更
関    数    名 : show_dlg_item
引          数 :
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
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
機  能  名  称 : 2値化実行
関    数    名 : execute
引          数 :
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
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
	{//結果をマスクに入れる
		if(!API.Binalize(CURRENT_IMAGE, min_thresh, max_thresh, inverse))	return false;
		API.DrawImage(CURRENT_IMAGE);
	}
	else
	{//結果を別のメモリに入れる
		if(src_num==dst_num){
			API.MessageBox("結果出力先には同じ画像メモリを指定できません");
			return false;
		}

		API.BackupDataUnit(dst_num);//現在のデータバックアップ

		if(API.GetCheck(m_hWnd,IDC_RADIO_BINALIZE0) )//グローバル閾値
		{
			switch(m_combo_binalize_type.GetCurSel())
			{
			case 0://閾値手入力
				API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, &min_thresh);
				API.Binalize(src_num, dst_num, min_thresh, max_thresh, inverse, 255, 0, false, multi);
				break;

			case 1://大津法
				min_thresh = API.BinalizeThresholdOtsu(src_num);
				API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, min_thresh);
				API.Binalize(src_num, dst_num, min_thresh, max_thresh, inverse, 255, 0, false, multi);
				break;

			case 2://Pタイル法
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
		else//ローカル閾値
		{
			switch(m_combo_binalize_type.GetCurSel())
			{
			case 0://ガウシアン自己差分
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

			case 2://NICKの手法
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

		API.DrawImage(dst_num);//描画


		//ラベリング

		if(API.GetCheck(m_hWnd,IDC_CHECK_BINALIZE_LABEL))
		{
			int     label_image_no;
			int		min_area=0;
			int		area[BINALIZE_LABEL_MAX] = { 0 };
			float	gx[BINALIZE_LABEL_MAX] = { 0 };
			float	gy[BINALIZE_LABEL_MAX] = { 0 };

			//ラベル結果画像番号
			API.GetEditValue(m_hWnd, IDC_EDIT_LABEL_INAGE_NO, &label_image_no);

			if(label_image_no == src_num || label_image_no == dst_num)
			{
				API.MessageBox("結果出力先には同じ画像メモリを指定できません");
				return false;
			}
			else
			{

				API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_LABEL_AREA_MIN, &min_area);

				int labelCnt = API.Label(dst_num, label_image_no, true, min_area, area, gx, gy, 255, 0, false, multi);
				if (labelCnt < 0)	return false;

				CDataUnit *pdu;
				if (!(pdu = API.GetDataUnit(label_image_no)))return false;
				pdu->ChangePalette(PALETTE_LABEL);//ラベルがわかりやすいようなカラーパレットに変更

				if (!multi)
				{
					//ラベル位置を表示する
					for (int n = 0; n < labelCnt && n < MARK_NUMBER_MAX; n++)
					{
						double attrib[MARK_ATTRIB_NUM] = { 0 };
						attrib[0] = area[n];

						API.AddMark(label_image_no, gx[n], gy[n], attrib);
					}
				}

				API.DrawImage(label_image_no);//描画
			}
		}


		
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
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
機  能  名  称 : ダイアログの初期化
関    数    名 : OnInitDialog
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
BOOL CBinalizeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//if( API.GetDataUnit(CURRENT_IMAGE) ==NULL)	return false;


	//「結果を別メモリへ入れる」を初期値にする
	API.SetCheck(m_hWnd, IDC_RADIO_BINALIZE_TO_NEW_IMAGE, true);
	show_dlg_item();


	//結果を入れる画像メモリ番号を設定
	int dst_num, label_num, src_num; 

	src_num = API.GetCurrentImageNumber();
	dst_num = src_num + 1;
	label_num = src_num + 2;

	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_FROM_IMAGE_NO, src_num);	
	if(dst_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_TO_IMAGE_NO, dst_num);	
	if(label_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_LABEL_INAGE_NO, label_num);	
	


	//「グローバル2値化」を初期値にする
	API.SetCheck(m_hWnd, IDC_RADIO_BINALIZE0, true);
	OnBnClickedRadioBinalize0();


	//各種パラメタの初期値設定

	//グローバル閾値
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, 128);	
	m_slider_binalize_param0.SetRange(0,255);
	m_slider_binalize_param0.SetPos(128);

	//パーセンタイル
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM1, 50);	
	m_slider_binalize_param1.SetRange(0,100);
	m_slider_binalize_param1.SetPos(50);

	//動的2値化のフィルタサイズ
	for(int i=0, filter_size = 3 ; filter_size <= COMB_FILTER_SIZE_MAX  ; i++, filter_size+=2)
	{
		CString str;	str.Format("%d×%d", filter_size, filter_size);
		m_combo_binalize_dyn_size.AddString( str);		
		m_binalize_dyn_size[i] = filter_size;
	}
	m_combo_binalize_dyn_size.SetCurSel(0);

	//padding
	API.SetCheck(m_hWnd, IDC_RADIO_BINALIZE_PAD0, true);

	//NickのK
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_NICK_K, -0.03);

	//動的2値化のオフセット
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM3, 0);
	m_slider_binalize_param3.SetRange(-128,128);
	m_slider_binalize_param3.SetPos(0);

	//ラベリング最小面積閾値
	API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_LABEL_AREA_MIN, 10);	

	



	return true;
}



/********************************************************************
機  能  名  称 :閉じるボタン押下
関    数    名 : OnBnClickedOk
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnBnClickedOk()
{
	//execute();
	OnOK();
}

/********************************************************************
機  能  名  称 :実行ボタン押下
関    数    名 : OnBnClickedButtonBinalizeApply
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnBnClickedButtonBinalizeApply()
{
	execute();
}

/********************************************************************
機  能  名  称 :「２値画像をマスクへ入れる」ラジオボタン選択
関    数    名 : OnBnClickedRadioBinalizeToMask
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnBnClickedRadioBinalizeToMask()
{
	show_dlg_item();
}

/********************************************************************
機  能  名  称 :「２値画像を別の画像メモリへ入れる」ラジオボタン選択
関    数    名 : OnBnClickedRadioBinalizeToNewImage
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnBnClickedRadioBinalizeToNewImage()
{
	show_dlg_item();
}

/********************************************************************
機  能  名  称 :「ラベリングする」チェックボックス変更
関    数    名 : OnBnClickedCheckBinalizeLabel
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnBnClickedCheckBinalizeLabel()
{
	show_dlg_item();
}





/********************************************************************
機  能  名  称 : グローバル2値化ラジオボタン選択
関    数    名 : OnBnClickedRadioBinalize0
引          数 : 
戻    り    値 : 
機          能 : 固定閾値選択
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnBnClickedRadioBinalize0()
{
	if(m_combo_binalize_type.GetCount()>0 ){
		while(m_combo_binalize_type.DeleteString(0));//アルゴタイプコンボボックスの全要素をクリア
	}

	m_combo_binalize_type.AddString("閾値手入力");				//0
	m_combo_binalize_type.AddString("判別分析(大津法)");		//1
	m_combo_binalize_type.AddString("Pタイル法");				//2
	m_combo_binalize_type.SetCurSel(0);

	OnCbnEditchangeComboBinalizeType();
}

/********************************************************************
機  能  名  称 : 局所2値化ラジオボタン選択
関    数    名 : OnBnClickedRadioBinalize0
引          数 : 
戻    り    値 : 
機          能 : 固定閾値選択
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnBnClickedRadioBinalize1()
{
	if(m_combo_binalize_type.GetCount()>0 ){
		while(m_combo_binalize_type.DeleteString(0));//フィルタサイズコンボボックスの全要素をクリア
	}

	m_combo_binalize_type.AddString("ガウシアン自己差分");	//0
	m_combo_binalize_type.AddString("ラプラシアンオブガウシアン");		//1
	m_combo_binalize_type.AddString("NICK法");				//2
	m_combo_binalize_type.SetCurSel(0);

	OnCbnEditchangeComboBinalizeType();
}


/********************************************************************
機  能  名  称 : 2値化アルゴ選択のコンボボックス変更
関    数    名 : OnCbnEditchangeComboBinalizeType
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnCbnEditchangeComboBinalizeType()
{
	//パラメタ表示UIをいったん非表示
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
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PAD), SW_HIDE);//「padding」グループボックス
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD1), SW_HIDE);//「ゼロ」ラジオボタン
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD2), SW_HIDE);//「周辺画素」ラジオボタン
	::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD0), SW_HIDE);//「折り返し」ラジオボタン


	if(API.GetCheck(m_hWnd,IDC_RADIO_BINALIZE0) )//グローバル閾値
	{
		::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM0), SW_SHOW);
		::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), SW_SHOW);
		
		switch(m_combo_binalize_type.GetCurSel())
		{
		case 0://閾値手入力
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), true);
			m_slider_binalize_param0.ShowWindow(SW_SHOW);
			break;

		case 1://大津法
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), false);
			m_slider_binalize_param0.ShowWindow(SW_HIDE);
			execute();
			break;

		case 2://P-タイル法
			::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM0), false);
			m_slider_binalize_param0.ShowWindow(SW_HIDE);
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM1), SW_SHOW);
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM1), SW_SHOW);
			m_slider_binalize_param1.ShowWindow(SW_SHOW);
			execute();
			break;

		}
	}
	else//局所閾値
	{
		switch(m_combo_binalize_type.GetCurSel())
		{
		case 0://ガウシアン自己差分
			//not break;
		case 1://LOG
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM2), SW_SHOW);
			m_combo_binalize_dyn_size.ShowWindow(SW_SHOW);
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PARAM3), SW_SHOW);
			::ShowWindow( ::GetDlgItem(m_hWnd, IDC_EDIT_BINALIZE_PARAM3), SW_SHOW);	
			m_slider_binalize_param3.ShowWindow(SW_SHOW);
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_STATIC_BINALIZE_PAD), SW_SHOW);//「padding」グループボックス
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD1), SW_SHOW);//「ゼロ」ラジオボタン
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD2), SW_SHOW);//「周辺画素」ラジオボタン
			::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_BINALIZE_PAD0), SW_SHOW);//「折り返し」ラジオボタン
			break;

		case 2://Nickの方法
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
機  能  名  称 : パラメタ0=グローバル2値化の閾値　値変更
関    数    名 : OnEnChangeEditBinalizeParam0
引          数 : 
戻    り    値 : 
機          能 : 固定閾値選択
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnEnChangeEditBinalizeParam0()
{
	int val;
	API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, &val);

	m_slider_binalize_param0.SetPos(val);//スライドバーに同値反映
}

/********************************************************************
機  能  名  称 : パラメタ1=パーセンタイル設定変更
関    数    名 : OnEnChangeEditBinalizeParam1
引          数 : 
戻    り    値 : 
機          能 : 固定閾値選択
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnEnChangeEditBinalizeParam1()
{
	int val;
	API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM1, &val);

	m_slider_binalize_param1.SetPos(val);//スライドバーに同値反映
}

/********************************************************************
機  能  名  称 : パラメタ3=動的2値化オフセット
関    数    名 : OnEnChangeEditBinalizeParam3
引          数 : 
戻    り    値 : 
機          能 : 固定閾値選択
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnEnChangeEditBinalizeParam3()
{
	int val;
	API.GetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM3, &val);

	m_slider_binalize_param3.SetPos(val);//スライドバーに同値反映
}

/********************************************************************
機  能  名  称 : スライドバー動作
関    数    名 : OnHScroll
引          数 : 
戻    り    値 : 
機          能 : 固定閾値選択
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CBinalizeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if(pScrollBar == (CScrollBar*)&m_slider_binalize_param0)//グローバル閾値設定
	{
		int val = m_slider_binalize_param0.GetPos();
		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM0, val);

		execute();//実行
	}
	else if(pScrollBar == (CScrollBar*)&m_slider_binalize_param1)//Pタイル設定
	{
		int val = m_slider_binalize_param1.GetPos();
		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM1, val);

		execute();//実行
	}
	else if(pScrollBar == (CScrollBar*)&m_slider_binalize_param3)//オフセット
	{
		int val = m_slider_binalize_param3.GetPos();
		API.SetEditValue(m_hWnd, IDC_EDIT_BINALIZE_PARAM3, val);

		execute();//実行
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


/********************************************************************
機  能  名  称 : 表示画像を元画像にする
関    数    名 : OnBnClickedButtonBinalizeSetOrgImg
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
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
