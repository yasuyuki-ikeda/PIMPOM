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
//CPointCloudDlg
//三次元点群表示ウィンドウ
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include "DataUnit.h"

///*************************************
//         クラス   定   義         
///*************************************
class CPointCloudDlg : public CDialog
{
	enum { IDD = IDD_POINT_CLOUD_DLG };// ダイアログ データ

public:
	CPointCloudDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void RotationTheta(float theta);////X軸周りの回転
	void RotationPhai(float phai);////Y軸周りの回転
	void Rendering(CSize sampling);//点群をスクリーン(ビットマップ)に描く
	int CreatePointCloud(CDataUnit *p_du);


protected:
	CSize	data_size;//画像データのサイズ
	long	bmp_data_width;//画像データの表示バッファのサイズ
	float	*p_transformed_x;//移動後のx座標（配列）
	float	*p_transformed_y;//移動後のy座標（配列）
	float	*p_transformed_z;//移動後のz座標（配列）
	float	*p_original_z;//移動前のz座標（配列）
	BYTE	*p_mask;//マスクの先頭ポインタ
	BYTE	*p_brightness;//データ点の表示濃度（配列）
	float	*pZbuffer;//Zバッファ
	BYTE	*p_pixel_flg;//スクリーンの同じ画素に見えるデータ点があることを表すフラグ（配列）
	BYTE	*p_screen_bmp_data;//スクリーンを表すビットマップのデータ部分の先頭ポインタ
	BITMAPINFO		*p_screen_bmp_info;//スクリーンを表すビットマップのヘッダ部分
	float			scale;//縮尺
	CPoint			translate;//中心からのずれ
	bool			is_druging;//ドラッグ中かどうかを表すフラグ
	bool			is_draging_mbutton;//中央ドラッグ中かどうかを表すフラグ
	CPoint			mouse_down_point;//マウスボタンを押した座標（スクリーン座標）
	CSize			sampling_size;//データ点のサンプリング間隔

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	void set_ctrl_type(short type);//コントロールタイプを決定する(type: 0→回転   1→拡大/縮小   2→平行移動)
	short get_ctrl_type();//現在のコントロールタイプを返す( 0→回転   1→拡大/縮小   2→平行移動)
	void draw_scale();//スケール（縮尺）を描く
	void draw_screen();//スクリーン（ビットマップ）を描く
	void init_bmp();//スクリーンを表すビットマップを初期化する


//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point); 
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPointCloudCtrlRotate();
	afx_msg void OnPointCloudCtrlScale();
	afx_msg void OnPointCloudCtrlTranslate();
};
