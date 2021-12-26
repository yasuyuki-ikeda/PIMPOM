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
//C3DImgDispDlg
//3Dフォーマットデータの表示と編集
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#pragma once
#include "global_define.h"
#include "global_typedef.h"
#include "afxcmn.h"

///*************************************
//         マ  ク  ロ   定   義         
///*************************************
#define  TD_IMAGE_DISP_MAX		(384)	//一度に表示できる最大画像数
#define  TD_IMAGE_DISP_OFFSET_X	(20)
#define  TD_IMAGE_DISP_OFFSET_Y	(50)

// *************************************
//         構  造  体   定   義         
// *************************************

//3D画像データ表示用構造体
typedef struct _TD_IMAGE_DISP_STRUCT
{
	int		pageNumber;//表示する画像データのページ番号
	int		dispOffsetX;//表示位置x
	int		dispOffsetY;//表示位置y
	int		dispWidth;//表示幅
	int		dispHeight;//表示高さ
	BYTE	*pDispBuffers;//表示用メモリ先頭ポインタ
	char	mask;//マスク

}TD_IMAGE_DISP_STRUCT;


///*************************************
//         クラス   定   義         
///*************************************
class C3DImgDispDlg : public CDialog
{
	DECLARE_DYNAMIC(C3DImgDispDlg)

	enum { IDD = IDD_3D_IMAGE_DISP_DLG };// ダイアログ データ

public:
	C3DImgDispDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~C3DImgDispDlg();
	bool DispImage(int image_number=-1);


protected:
	int		imageNumber;//表示するデータ番号
	TD_IMAGE_DISP_STRUCT DispStruct[TD_IMAGE_DISP_MAX];
	BYTE	*pMemories;//表示用メモリ
	CSliderCtrl m_slider;//表示画像選択用スライダー

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	int		arrange_images();
	void	clear_buffer();
	bool	copy_disp_buffers(int image_number, int page_offset = 0, int disp_cnt = TD_IMAGE_DISP_MAX);
	bool	draw_images(int image_number);
	int		set_slider(int image_number);


//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton3dImgDispScale();
	afx_msg void OnBnClickedButton3dImgDispScale2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonDispMinmax();
	afx_msg void OnBnClickedCheckMask();
};
