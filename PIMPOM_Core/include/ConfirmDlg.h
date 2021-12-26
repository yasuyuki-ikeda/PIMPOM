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
//CConfirmDlg
//汎用の値設定ダイアログ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "global_define.h"
#include "global_typedef.h"


///*************************************
//         マ  ク  ロ   定   義         
///*************************************
#define		CONFIRM_NUM_MAX	(20)	//このダイアログで使えるコントロールの最大数

#define		CONFIRM_NULL	(-1)
#define		CONFIRM_INT		(0)
#define		CONFIRM_SHORT	(1)
#define		CONFIRM_LONG	(2)
#define		CONFIRM_FLOAT	(3)
#define		CONFIRM_DOUBLE	(4)
#define		CONFIRM_STRING	(5)
#define		CONFIRM_BOOL	(6)
#define		CONFIRM_RADIO_BOOL	(7)


///*************************************
//         クラス   定   義         
///*************************************
class PIMPOM_EXT CConfirmDlg : public CDialog
{
	enum { IDD = IDD_MULTI_CONFIRM_DLG };// ダイアログ データ

public:
	CConfirmDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void SetWidth(int titlesize, int editsize);
	void SetTitle(CString title);
	bool RegistVar(CString title, bool *pval);
	bool RegistVar(CString title, CString *pval);
	bool RegistVar(CString title, double *pval);
	bool RegistVar(CString title, float *pval);
	bool RegistVar(CString title, short *pval);
	bool RegistVar(CString title, int *pval);
	bool RegistRadioVar(CString title, bool *pval);


protected:
	int		title_width;
	int		edit_width;
	int		item_count;
	struct CONFIRM_VALUE{
		short	type;
		CString	title;
		int		*p_var_int;
		short	*p_var_short;
		float	*p_var_float;
		double	*p_var_double;
		CString	*p_var_string;
		bool	*p_var_bool;
	}value[CONFIRM_NUM_MAX];
	int		edit_id[CONFIRM_NUM_MAX], title_id[CONFIRM_NUM_MAX], check_id[CONFIRM_NUM_MAX], radio_id[CONFIRM_NUM_MAX];
	CString window_title;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	void init_setting();


//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedOk();
};

