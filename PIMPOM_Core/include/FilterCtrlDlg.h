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
//CFilterCtrlDlg
//フィルタ設定ウィンドウ
/*******************************************************************/
#pragma once


///*************************************
//         マ  ク  ロ   定   義         
///*************************************
#define	FILTER_COUNT			(25)//フィルタ数
#define FILTER_PARAM_MAX		(7)//パラメタ最大数
#define FILTER_SIZE_LIST_MAX	(100)

#define FILTER_MEAN				(0)//移動平均
#define FILTER_MEAN_M			(1)//最大・最小を除いた移動平均
#define FILTER_GAUSS			(2)//ガウス
#define FILTER_MEDIAN			(3)//メディアン
#define FILTER_DILATION			(4)//膨張
#define FILTER_EROSION			(5)//収縮
#define FILTER_OPENING			(6)//オープニング
#define FILTER_CLOSING			(7)//クロージング
#define FILTER_BILATERAL		(8)//バイラテラルフィルタ
#define FILTER_SOBEL			(9)//エッジ抽出(sobel)
#define FILTER_LOG				(10)//Laplacian of Gaussian
#define FILTER_GABOR			(11)//ガボールフィルタ
#define FILTER_SIGMOID			(12)//エッジ抽出(シグモイド)
#define FILTER_LOG_ZEROCLS		(13)//エッジ細線化(LoGゼロクロス)
#define FILTER_CANNY			(14)//エッジ細線化(Canny)
#define FILTER_GAUSS_UNSHADE	(15)//シェーディング除去
#define FILTER_NEGA				(16)//濃度反転
#define FILTER_MIRROR_LR		(17)//鏡像（左右）
#define FILTER_MIRROR_UD		(18)//鏡像（上下）
#define FILTER_ROT180			(19)//180°
#define FILTER_CONVOLUTION		(20)//コンボリューション
#define FILTER_CLAHE			(21)//CLAHE (Contrast Limited Adaptive Histogram Equalization)
#define FILTER_LOCAL_CONTRAST	(22)//局所コントラスト拡張(sliding windowで 255*( x - ave - s*σ)/(2*σ) )
#define FILTER_LOCAL_CONTRAST_P	(23)//局所コントラスト拡張(パッチ分割して (x-min)/(max-min))
#define FILTER_DFT				(24)//離散フーリエ変換


// *************************************
//         構  造  体   定   義         
// *************************************

//フィルタ情報
typedef struct _FILTER_CTRL_PARAMS{
	char	name[100];//フィルタ名
	int		kernel_size[FILTER_SIZE_LIST_MAX];//カーネルサイズリスト
	int		kernel_size_sel;//選択中のカーネルサイズ
	int		padding_type;	//padding  (負数:設定なし  1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
	int		direction;//フィルタ方向   （負数:設定なし  1:横 2:縦 それ以上:縦横）　
	int		circle_shape;//円形のフィルタにするかどうか （負数:設定なし  0: しない それ以上：する) 
	float	param[FILTER_PARAM_MAX];//パラメタの値
	char	param_name[FILTER_PARAM_MAX][100];//パラメタ名称

}FILTER_CTRL_PARAMS;

///*************************************
//         クラス   定   義         
///*************************************


class CFilterCtrlDlg : public CDialog
{
	enum { IDD = IDD_FILTER_CTRL_DLG };// ダイアログ データ

public:
	CFilterCtrlDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ





protected:
	CComboBox	m_cmbo_filter_type;
	CComboBox	m_cmbo_filter_size;
	FILTER_CTRL_PARAMS	m_filter_param[FILTER_COUNT];//フィルタ情報
	CImageField *pKernelIF;
	CDataUnit	*pKernelDU;
	float		*pConvolutionKernel;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	bool	execute(int src_num, int dst_num);
	void	show_dlg_item();
	void	init_filter_params();
	void	draw_kernel();
	void	set_conv_kernel(int kernel_size);
	bool	get_conv_kernel(int *pkernel_size, float *pkernel);

	
//メッセージハンドラ
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnFilterExec();
	afx_msg void OnBnClickedRadioFilterOverwite();
	afx_msg void OnBnClickedRadioFilterToImage();
	afx_msg void OnCbnSelchangeComboFilterType();
	afx_msg void OnBnClickedButtonFilterCtrlSetOrgImg();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeComboFilterSize();
};
