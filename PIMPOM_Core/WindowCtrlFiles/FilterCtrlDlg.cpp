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

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "FilterCtrlDlg.h"


// *************************************
//         マ  ク  ロ   定   義         
// *************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

CFilterCtrlDlg::CFilterCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterCtrlDlg)
	//}}AFX_DATA_INIT
}


void CFilterCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterCtrlDlg)
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_cmbo_filter_type);
	DDX_Control(pDX, IDC_COMBO_FILTER_SIZE, m_cmbo_filter_size);
	//}}AFX_DATA_MAP
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
void CFilterCtrlDlg::show_dlg_item()
{
	if(API.GetCheck(m_hWnd,IDC_RADIO_FILTER_OVERWITE)){
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO), true);
	}
}

/********************************************************************
機  能  名  称 : 選択画像に対してフィルタ実行
関    数    名 : execute
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CFilterCtrlDlg::execute(int src_num, int dst_num)
{
	CDataUnit	*p_du;
	int			filterID;
	CSize		filter_size_c;
	int			filter_size;
	bool		forMulti;
	bool		ret=true;
	int			ctrlID[] = { IDC_EDIT_FILTER_PARAM0 ,IDC_EDIT_FILTER_PARAM1, IDC_EDIT_FILTER_PARAM2, IDC_EDIT_FILTER_PARAM3, IDC_EDIT_FILTER_PARAM4, IDC_EDIT_FILTER_PARAM5, IDC_EDIT_FILTER_PARAM6 };

	p_du = API.GetDataUnit(src_num);
		if(!p_du)	return false;
		
	//フィルタタイプをコンボボックスから取得
	filterID = m_cmbo_filter_type.GetCurSel();


	//フィルタサイズをコンボボックスから取得
	if (m_cmbo_filter_size.GetCount() > 0) {
		m_filter_param[filterID].kernel_size_sel = m_cmbo_filter_size.GetCurSel();
	}
	filter_size = m_filter_param[filterID].kernel_size[m_filter_param[filterID].kernel_size_sel];


	//各種パラメタダイアログアイテムから取得

	//円形
	if (m_filter_param[filterID].circle_shape >= 0) {
		if (API.GetCheck(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE)) {
			m_filter_param[filterID].circle_shape = 1;
		}
		else {
			m_filter_param[filterID].circle_shape = 0;
		}
	}

	//極性
	if (m_filter_param[filterID].direction >= 0)
	{
		if (API.GetCheck(m_hWnd, IDC_RADIO_FILTER_X)) {//横のみ
			m_filter_param[filterID].direction = 1;
			filter_size_c.cx = filter_size;
			filter_size_c.cy = 1;
		}
		else if (API.GetCheck(m_hWnd, IDC_RADIO_FILTER_Y) ) {//縦のみ
			m_filter_param[filterID].direction = 2;
			filter_size_c.cx = 1;
			filter_size_c.cy = filter_size;
		}
		else {//縦横
			m_filter_param[filterID].direction = 0;
			filter_size_c.cx = filter_size;
			filter_size_c.cy = filter_size;
		}
	}
	else {
		filter_size_c.cx = filter_size;
		filter_size_c.cy = filter_size;
	}


	//padding
	if (m_filter_param[filterID].padding_type >= 0)
	{
		if (API.GetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD1)) {//外周延長
			m_filter_param[filterID].padding_type = 1;
		}
		else if (API.GetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD2)) {//折り返し
			m_filter_param[filterID].padding_type = 2;
		}
		else {//縦横
			m_filter_param[filterID].padding_type = 0;
		}
	}

	//すべてのページ／チャネルを対象とする
	forMulti = API.GetCheck(m_hWnd, IDC_CHECK_FILTER_CTRL_FOR_MULTI);


	//その他のパラメタ
	for (int i = 0; i < FILTER_PARAM_MAX; i++) {
		if (m_filter_param[filterID].param_name[0] != 0) {
			API.GetEditValue(m_hWnd, ctrlID[i], &m_filter_param[filterID].param[i]);
		}
	}
	



	//カーネル表示用DU
	if (pKernelDU){
		delete	pKernelDU;
		pKernelDU = NULL;
	}

	if(filter_size_c.cx>0 && filter_size_c.cy>0){
		pKernelDU = new CDataUnit(FLOAT_FORMAT, filter_size_c, 1);
	}

		//フィルタ種類を決定
		switch(filterID)
		{
			case FILTER_MEAN://移動平均
				ret = API.FilterSmooth(src_num, dst_num, filter_size_c, false, forMulti);
				break;

			case FILTER_MEAN_M://最大・最小を除いた移動平均
				ret = API.FilterSmooth(src_num, dst_num, filter_size_c, true, forMulti);
				break;

			case FILTER_GAUSS://ガウス
				ret = API.FilterGauss(src_num, dst_num, filter_size, m_filter_param[filterID].direction, m_filter_param[filterID].padding_type, forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_MEDIAN://メディアン
				ret = API.FilterMedian(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_DILATION://膨張
				ret = API.FilterDilation(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_EROSION://収縮
				ret = API.FilterErosion(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_OPENING://オープニング
				ret = API.FilterOpening(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_CLOSING://クロージング
				ret = API.FilterClosing(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_BILATERAL://バイラテラルフィルタ
				ret = API.FilterBilateral(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], m_filter_param[filterID].param[1], forMulti);
				break;

			case FILTER_SOBEL://エッジ抽出(sobel)
				ret = API.FilterEdgeSobel(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], m_filter_param[filterID].direction, forMulti);
				break;

			case FILTER_LOG://Laplacian of Gaussian
				ret = API.FilterLaplaceGauss(src_num, dst_num, filter_size, m_filter_param[filterID].direction, 
																m_filter_param[filterID].param[0], 
																m_filter_param[filterID].param[1],
																m_filter_param[filterID].padding_type,
																forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_GABOR://Gabor
				ret = API.FilterGabor(src_num, dst_num, filter_size, m_filter_param[filterID].param[3], 
																	m_filter_param[filterID].param[2], 
																	m_filter_param[filterID].param[4], 
																	m_filter_param[filterID].param[5], 
																	m_filter_param[filterID].param[0], 
																	m_filter_param[filterID].param[1], 
																	m_filter_param[filterID].padding_type, 
																	forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_SIGMOID://Sigmoid
				ret = API.FilterEdgeSigmoid(src_num, dst_num, filter_size, m_filter_param[filterID].param[3], 
																		m_filter_param[filterID].param[2], 
																		m_filter_param[filterID].param[0], 
																		m_filter_param[filterID].param[1], 
																		m_filter_param[filterID].padding_type, 
																		forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_LOG_ZEROCLS://エッジ細線化(LoGゼロクロス)
				ret = API.FilterEdgeLog(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], 
																		m_filter_param[filterID].direction, 
																		m_filter_param[filterID].padding_type, forMulti);
				break;

			case FILTER_CANNY://エッジ細線化(Canny)
				ret = API.FilterEdgeCanny(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], m_filter_param[filterID].param[1], forMulti);
				break;

			case FILTER_GAUSS_UNSHADE://シェーディング除去
				ret = API.FilterGaussUnshade(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], 
																			m_filter_param[filterID].param[1], 
																			m_filter_param[filterID].direction,
																			m_filter_param[filterID].padding_type, 
																			forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_NEGA://濃度反転
				ret = API.FilterNega(src_num, dst_num, forMulti);
				break;

			case FILTER_MIRROR_LR://鏡像（左右）
				ret = API.FilterMirror(src_num, dst_num, 0, forMulti);
				break;

			case FILTER_MIRROR_UD://鏡像（上下）
				ret = API.FilterMirror(src_num, dst_num, 1, forMulti);
				break;

			case FILTER_ROT180://180°
				ret = API.FilterMirror(src_num, dst_num, 2, forMulti);
				break;

			case FILTER_CONVOLUTION:
			{
				int kernelSize;
				ret = get_conv_kernel(&kernelSize, pConvolutionKernel);

				if (ret)
				{
					if (kernelSize == filter_size_c.cx)
					{
						ret = API.FilterConvolution(src_num, dst_num, filter_size_c, pConvolutionKernel,
													m_filter_param[filterID].param[0],
													m_filter_param[filterID].param[1],
													m_filter_param[filterID].padding_type, forMulti);

						if (ret){
							memcpy(pKernelDU->pFloatData, pConvolutionKernel, filter_size_c.cx*filter_size_c.cy*sizeof(float));
						}
					}
					else{
						API.MessageBox("kernel value size invalid");
						ret = false;
					}
				}
				else{
					API.MessageBox("kernel value sintax error");
				}
			}
				break;

			case FILTER_CLAHE://CLAHE(Contrast Limited Adaptive Histogram Equalization)
				ret = API.FilterCLAHE(src_num, dst_num, m_filter_param[filterID].param[0], m_filter_param[filterID].param[1], m_filter_param[filterID].param[2], forMulti);
				break;

			case FILTER_LOCAL_CONTRAST://局所コントラスト拡張(sliding windowで 255*( x - ave - s*σ)/(2*σ) )
				ret = API.FilterLocalContrast(src_num, dst_num, m_filter_param[filterID].param[0], m_filter_param[filterID].param[1], forMulti);
				break;

			case FILTER_LOCAL_CONTRAST_P://局所コントラスト拡張(パッチ分割して (x-min)/(max-min))
				ret = API.FilterLocalContrastPatch(src_num, dst_num, m_filter_param[filterID].param[0], forMulti);
				break;

			case FILTER_DFT://離散フーリエ変換
				ret = API.FilterDFT(src_num, dst_num, m_filter_param[filterID].param[0]);
				break;
		}

	//カーネル表示
		if (ret)
		{
			draw_kernel();
		}

	return ret;

}


/********************************************************************
機  能  名  称 : フィルタパラメタの初期化
関    数    名 : init_filter_params
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CFilterCtrlDlg::init_filter_params()
{
	memset(m_filter_param, 0, sizeof(FILTER_CTRL_PARAMS)*FILTER_COUNT);

	strcpy(m_filter_param[FILTER_MEAN].name, "移動平均");
	strcpy(m_filter_param[FILTER_MEAN_M].name, "移動平均(最大/最小除去)");
	strcpy(m_filter_param[FILTER_GAUSS].name, "ガウシアン");
	strcpy(m_filter_param[FILTER_MEDIAN].name, "メディアン");
	strcpy(m_filter_param[FILTER_DILATION].name, "膨張");
	strcpy(m_filter_param[FILTER_EROSION].name, "収縮");
	strcpy(m_filter_param[FILTER_OPENING].name, "オープニング");
	strcpy(m_filter_param[FILTER_CLOSING].name, "クロージング");
	strcpy(m_filter_param[FILTER_BILATERAL].name, "バイラテラル");
	strcpy(m_filter_param[FILTER_SOBEL].name, "エッジ抽出(sobel)");
	strcpy(m_filter_param[FILTER_LOG].name, "Laplacian of Gaussian(LoG)");
	strcpy(m_filter_param[FILTER_GABOR].name, "ガボール");
	strcpy(m_filter_param[FILTER_SIGMOID].name, "エッジ抽出(シグモイド)");
	strcpy(m_filter_param[FILTER_LOG_ZEROCLS].name, "エッジ細線化(LoGゼロクロス)");
	strcpy(m_filter_param[FILTER_CANNY].name, "エッジ細線化(Canny)");
	strcpy(m_filter_param[FILTER_GAUSS_UNSHADE].name, "シェーディング除去(ガウシアン自己差分)");
	strcpy(m_filter_param[FILTER_NEGA].name, "濃度反転");
	strcpy(m_filter_param[FILTER_MIRROR_LR].name, "左右反転");
	strcpy(m_filter_param[FILTER_MIRROR_UD].name, "上下反転");
	strcpy(m_filter_param[FILTER_ROT180].name, "180°反転");
	strcpy(m_filter_param[FILTER_CONVOLUTION].name, "コンボリューション");
	strcpy(m_filter_param[FILTER_CLAHE].name, "CLAHE(ｺﾝﾄﾗｽﾄ制限付き適応型ﾋｽﾄｸﾞﾗﾑ平坦化)");
	strcpy(m_filter_param[FILTER_LOCAL_CONTRAST].name, "局所ｺﾝﾄﾗｽﾄ拡大(255*( x - ave - s*σ)/(2*σ))");
	strcpy(m_filter_param[FILTER_LOCAL_CONTRAST_P].name, "局所ｺﾝﾄﾗｽﾄ拡大(x-min)/(max-min)");
	strcpy(m_filter_param[FILTER_DFT].name, "離散フーリエ変換");

	for (int id = 0; id < FILTER_COUNT; id++)
	{

		switch (id)
		{
		case FILTER_MEAN://平滑化
				 //not break
		case FILTER_MEAN_M://最大・最小を除いた平滑化
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = 0;//極性設定する
			m_filter_param[id].padding_type = -1;//padding設定しない

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_GAUSS://ガウシアン
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = 0;//極性設定する
			m_filter_param[id].padding_type = 0;//padding設定する

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_MEDIAN://メディアン
				   //not break
		case FILTER_DILATION://膨張
					 //not break
		case FILTER_EROSION://収縮
					//not break
		case FILTER_OPENING://オープニング
					//not break
		case FILTER_CLOSING://クロージング
			m_filter_param[id].circle_shape = 0;//円形設定する
			m_filter_param[id].direction = 0;//極性設定する
			m_filter_param[id].padding_type = -1;//padding設定しない

			m_filter_param[id].kernel_size[0] = 2;
			m_filter_param[id].kernel_size_sel = 1;
			for (int i = 1, filter_size = 3; filter_size <= LANK_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}

			break;

		case FILTER_BILATERAL://バイラテラルフィルタ
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = -1;//padding設定しない
			strcpy(m_filter_param[id].param_name[0], "値のσ");		m_filter_param[id].param[0] = 10;
			strcpy(m_filter_param[id].param_name[1], "距離のσ");	m_filter_param[id].param[1] = 10;

			for (int i = 0, filter_size = 3; filter_size <= LANK_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_SOBEL://エッジ抽出(sobel)
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = 0;//極性設定する
			m_filter_param[id].padding_type = -1;//padding設定しない
			strcpy(m_filter_param[id].param_name[0], "濃度閾値");	m_filter_param[id].param[0] = 100;

			for (int i = 0, filter_size = 3; filter_size <= 9; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_LOG://Laplacian of gaussian
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = 0;//極性設定する	
			m_filter_param[id].padding_type = 0;//padding設定する
			strcpy(m_filter_param[id].param_name[0], "オフセット");		m_filter_param[id].param[0] = 128;
			strcpy(m_filter_param[id].param_name[1], "コントラスト");	m_filter_param[id].param[1] = 1;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_GABOR://Gabor
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = 0;//padding設定する
			strcpy(m_filter_param[id].param_name[0], "オフセット");		m_filter_param[id].param[0] = 128;
			strcpy(m_filter_param[id].param_name[1], "コントラスト");	m_filter_param[id].param[1] = 1;
			strcpy(m_filter_param[id].param_name[2], "周波数");			m_filter_param[id].param[2] = 2;
			strcpy(m_filter_param[id].param_name[3], "扁平率");			m_filter_param[id].param[3] = 0.3;
			strcpy(m_filter_param[id].param_name[4], "回転角");			m_filter_param[id].param[4] = 0;
			strcpy(m_filter_param[id].param_name[5], "初期位相");		m_filter_param[id].param[5] = 0;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_SIGMOID://Sigmoid
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = 0;//padding設定する
			strcpy(m_filter_param[id].param_name[0], "オフセット");		m_filter_param[id].param[0] = 128;
			strcpy(m_filter_param[id].param_name[1], "コントラスト");	m_filter_param[id].param[1] = 1;
			strcpy(m_filter_param[id].param_name[2], "回転角");			m_filter_param[id].param[2] = 0;
			strcpy(m_filter_param[id].param_name[3], "k");				m_filter_param[id].param[3] = 1;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_LOG_ZEROCLS://エッジ細線化(LoGゼロクロス)
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = 0;//極性設定する
			m_filter_param[id].padding_type = 0;//padding設定する
			strcpy(m_filter_param[id].param_name[0], "濃度閾値");	m_filter_param[id].param[0] = 100;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_CANNY://エッジ細線化(Canny)
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = -1;//padding設定しない
			strcpy(m_filter_param[id].param_name[0], "濃度閾値(強)");	m_filter_param[id].param[0] = 100;
			strcpy(m_filter_param[id].param_name[1], "濃度閾値(弱)");	m_filter_param[id].param[0] = 30;

			for (int i = 0, filter_size = 3; filter_size <= 7; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_GAUSS_UNSHADE://シェーディング除去
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = 0;//極性設定する
			m_filter_param[id].padding_type = 0;//padding設定する
			strcpy(m_filter_param[id].param_name[0], "オフセット");		m_filter_param[id].param[0] = 0;
			strcpy(m_filter_param[id].param_name[1], "コントラスト");	m_filter_param[id].param[1] = 1;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_NEGA://濃度反転
			//not break;
		case FILTER_MIRROR_LR://鏡像（左右）
			//not break;
		case FILTER_MIRROR_UD://鏡像（上下）
			//not break;
		case FILTER_ROT180://180°
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = -1;//padding設定しない
			break;

		case FILTER_CONVOLUTION:
			m_filter_param[id].padding_type = 0;//padding設定する
			strcpy(m_filter_param[id].param_name[0], "オフセット");		m_filter_param[id].param[0] = 0;
			strcpy(m_filter_param[id].param_name[1], "コントラスト");	m_filter_param[id].param[1] = 1;
			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_CLAHE:
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = -1;//padding設定しない
			strcpy(m_filter_param[id].param_name[0], "度数上限");		m_filter_param[id].param[0] = 40;
			strcpy(m_filter_param[id].param_name[1], "横分割数");		m_filter_param[id].param[1] = 8;
			strcpy(m_filter_param[id].param_name[2], "縦分割数");		m_filter_param[id].param[2] = 8;
			break;

		case FILTER_LOCAL_CONTRAST:
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = -1;//padding設定しない
			strcpy(m_filter_param[id].param_name[0], "局所領域サイズ");		m_filter_param[id].param[0] = 200;
			strcpy(m_filter_param[id].param_name[1], "スケールs");		m_filter_param[id].param[1] = 3;
			break;

		case FILTER_LOCAL_CONTRAST_P:
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = -1;//padding設定しない
			strcpy(m_filter_param[id].param_name[0], "分割数");		m_filter_param[id].param[0] = 8;
			break;

		case FILTER_DFT:
			m_filter_param[id].circle_shape = -1;//円形設定しない
			m_filter_param[id].direction = -1;//極性設定しない
			m_filter_param[id].padding_type = -1;//padding設定しない
			strcpy(m_filter_param[id].param_name[0], "逆変換");		m_filter_param[id].param[0] = 0;
			break;
		}
	}
}


/********************************************************************
機  能  名  称 : カーネルの表示
関    数    名 : draw_kernel
引          数 :
戻    り    値 :
機          能 :　
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CFilterCtrlDlg::draw_kernel()
{
	CDC *pdc = GetDC();
	float kernelMin, kernelMax;
	float kernelCenterLineX[COMB_FILTER_SIZE_MAX];
	float kernelCenterLineY[COMB_FILTER_SIZE_MAX];
	CPoint  orig;
	float  graphH = COMB_FILTER_SIZE_MAX / 3;
	CRect	kernelDispRgn(pKernelIF->FieldCoordinate.x, pKernelIF->FieldCoordinate.y,
		pKernelIF->FieldCoordinate.x + COMB_FILTER_SIZE_MAX + graphH,
		pKernelIF->FieldCoordinate.y + COMB_FILTER_SIZE_MAX + graphH);



	//背景を描画する
	pdc->Rectangle(kernelDispRgn);


	if (pKernelDU)
	{
		//カーネルの画像を表示する
		pKernelDU->GetDataMinMax(&kernelMin, &kernelMax);
		pKernelDU->SetDispRange(kernelMin, kernelMax);
		pKernelDU->UpdateDispBuffer();
		pKernelIF->DrawImage(pKernelDU, false);

		//カーネルの中心ラインの分布を取得する

		for (int i = 0; i < pKernelDU->DataSize.cx; i++) {
			kernelCenterLineX[i] = pKernelDU->pFloatData[i + pKernelDU->DataSize.cy / 2 * pKernelDU->DataSize.cx];
		}

		for (int i = 0; i < pKernelDU->DataSize.cy;i++) {
			kernelCenterLineY[i] = pKernelDU->pFloatData[pKernelDU->DataSize.cx/2 + i * pKernelDU->DataSize.cx];
		}

		//カーネルの中心ラインの分布を表示する
		CPen penCX(PS_SOLID,1,RGB(255,0,0));
		CPen penCY(PS_SOLID, 1,RGB(0, 0, 255));
		CPen penGuide(PS_DASH, 1, RGB(0, 0, 0));
		CPen *poldpen;
		
		//横方向のガイド線
		poldpen = pdc->SelectObject(&penGuide);
		pdc->MoveTo(kernelDispRgn.left, kernelDispRgn.top);
		pdc->LineTo(kernelDispRgn.left, kernelDispRgn.bottom);
		pdc->MoveTo(kernelDispRgn.left + pKernelDU->DataSize.cx, kernelDispRgn.top);
		pdc->LineTo(kernelDispRgn.left + pKernelDU->DataSize.cx, kernelDispRgn.bottom);
		pdc->SelectObject(poldpen);
		
		poldpen = pdc->SelectObject(&penCX);
		pdc->MoveTo(kernelDispRgn.left, kernelDispRgn.top + pKernelDU->DataSize.cy/2);
		pdc->LineTo(kernelDispRgn.left + pKernelDU->DataSize.cx, kernelDispRgn.top + pKernelDU->DataSize.cy / 2);
		pdc->SelectObject(poldpen);

		//横方向のグラフ
		for (int i = 0; i < pKernelDU->DataSize.cx ; i++) 
		{
			float x = kernelDispRgn.left + i;
			float y = kernelDispRgn.bottom - (kernelCenterLineX[i] - kernelMin) / (kernelMax - kernelMin) * graphH;
			pdc->SetPixel(x, y, RGB(255, 0, 0));
			pdc->SetPixel(x, y-1, RGB(255, 0, 0));
		}

		//縦方向のガイド線
		poldpen = pdc->SelectObject(&penGuide);
		pdc->MoveTo(kernelDispRgn.left, kernelDispRgn.top);
		pdc->LineTo(kernelDispRgn.right, kernelDispRgn.top);
		pdc->MoveTo(kernelDispRgn.left, kernelDispRgn.top + pKernelDU->DataSize.cy);
		pdc->LineTo(kernelDispRgn.right, kernelDispRgn.top + pKernelDU->DataSize.cy);
		pdc->SelectObject(poldpen);

		poldpen = pdc->SelectObject(&penCY);
		pdc->MoveTo(kernelDispRgn.left + pKernelDU->DataSize.cx/2, kernelDispRgn.top);
		pdc->LineTo(kernelDispRgn.left + pKernelDU->DataSize.cx / 2, kernelDispRgn.top + pKernelDU->DataSize.cy);
		pdc->SelectObject(poldpen);

		//縦方向のグラフ
		for (int i = 0; i < pKernelDU->DataSize.cy ; i++)
		{
			float x = kernelDispRgn.right - (kernelCenterLineY[i] - kernelMin) / (kernelMax - kernelMin) * graphH;
			float y = kernelDispRgn.top + i;
			pdc->SetPixel(x, y, RGB(0, 0, 255));
			pdc->SetPixel(x-1, y, RGB(0, 0, 255));
		}
	}

	ReleaseDC(pdc);
}


/********************************************************************
機  能  名  称 : コンボリューションカーネルの値をエディットボックスへ表示する
関    数    名 : set_conv_kernel
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CFilterCtrlDlg::set_conv_kernel(int filter_size)
{	
	CString message, tmpstr;
	for (int j = 0; j < filter_size; j++)
	{
		for (int i = 0; i < filter_size; i++){
			tmpstr.Format("%0.5f", pConvolutionKernel[i + j*filter_size]);
			message += tmpstr;

			if (i < filter_size - 1){
				message += CString(", ");
			}
		}

		if (j < filter_size - 1){
			message += CString("\r\n");
		}
	}

	API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_CTRL_CONV_KERNEL, message);

}

/********************************************************************
機  能  名  称 : エディットボックスからコンボリューションカーネルの値を取得する
関    数    名 : get_conv_kernel
引          数 :
戻    り    値 :成功したらtrue
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
bool CFilterCtrlDlg::get_conv_kernel(int *pkernel_size, float *pkernel)
{
	CString message, submes;

	API.GetEditValue(m_hWnd, IDC_EDIT_FILTER_CTRL_CONV_KERNEL, &message);//エディットボックスからすべての文字を取り出す
	
	//行列数を数える
	int  cntH = 0;//行数のカウンタ
	int cntW_perH[COMB_FILTER_SIZE_MAX] = { 0 };//行ごとの列数のカウンタ
	for (int i = 0; i < message.GetLength() ; i++)
	{
		if ( message[i] == '\n' || i == message.GetLength()-2)//改行が見つかったまたは最後に到達した
		{
			cntW_perH[cntH]++;//列数インクリメント
			if (cntW_perH[cntH] > COMB_FILTER_SIZE_MAX){//列数が上限を超えたらアウト
				return false;
			}

			cntH++;//行数インクリメント
			if (cntH > COMB_FILTER_SIZE_MAX){//行数が上限を超えたらアウト
				return false;
			}
		}
		else if (message[i] == ','){//カンマが見つかった
			cntW_perH[cntH]++;//列数インクリメント
			if (cntW_perH[cntH] > COMB_FILTER_SIZE_MAX){//列数が上限を超えたらアウト
				return false;
			}
		}
	}

	if (cntH < 3)	return false;//行数が3未満ならアウト

	for (int i = 0; i < cntH-1; i++)
	{
		if (cntW_perH[i] != cntH){//列数が違ったらアウト
			return false;
		}
	}

	//数値を読み出す
	int cnt=0;
	for (int i = 0; i < message.GetLength() - 1; i++)
	{
		if (message[i] == '\n' || i == message.GetLength() - 2)//改行が見つかったまたは最後に到達した
		{
			pkernel[cnt] = atof(submes.GetBuffer());	submes.ReleaseBuffer();		submes.Empty();
			cnt++;
		}
		else if (message[i] == ','){//カンマが見つかった
			pkernel[cnt] = atof(submes.GetBuffer());	submes.ReleaseBuffer();		submes.Empty();
			cnt++;
		}
		else if (isdigit(message[i]) || isalpha(message[i]) || ispunct(message[i])){
			submes += CString(message[i]);
		}
	}

	*pkernel_size = cntH;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFilterCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterCtrlDlg)
	ON_BN_CLICKED(IDC_BUTTON_FILTER_EXEC, OnFilterExec)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_FILTER_OVERWITE, &CFilterCtrlDlg::OnBnClickedRadioFilterOverwite)
	ON_BN_CLICKED(IDC_RADIO_FILTER_TO_IMAGE, &CFilterCtrlDlg::OnBnClickedRadioFilterToImage)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, &CFilterCtrlDlg::OnCbnSelchangeComboFilterType)
	ON_BN_CLICKED(IDC_BUTTON_FILTER_CTRL_SET_ORG_IMG, &CFilterCtrlDlg::OnBnClickedButtonFilterCtrlSetOrgImg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_SIZE, &CFilterCtrlDlg::OnCbnSelchangeComboFilterSize)
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
BOOL CFilterCtrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//if( API.GetDataUnit(CURRENT_IMAGE) ==NULL)	return false;


	//「結果を別メモリへ入れる」を初期値にする
	API.SetCheck(m_hWnd, IDC_RADIO_FILTER_TO_IMAGE,true);
	show_dlg_item();


	//結果を入れる画像メモリ番号を設定
	int dst_num, src_num; 
	src_num = API.GetCurrentImageNumber();
	dst_num = src_num + 1;

	API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO, src_num);
	if(dst_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO, dst_num);

	//フィルタ初期化
	init_filter_params();

	//フィルタ種類コンボボックスの設定
	m_cmbo_filter_type.DeleteString(0);
	for (int i = 0; i < FILTER_COUNT; i++) {
		if (m_filter_param[i].name[0]!=0) {
			m_cmbo_filter_type.AddString( CString(m_filter_param[i].name) );
		}
	}
	m_cmbo_filter_type.SetCurSel(0);
	

	//フィルタサイズの初期値設定
	OnCbnSelchangeComboFilterType();

	//コンボリューション用カーネル
	pConvolutionKernel = new float[COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX];
	memset(pConvolutionKernel, 0, COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX*sizeof(float));

	//フィルタカーネル表示
	CRect knl_rgn;
	API.GetDialogItemRect(m_hWnd, IDC_STATIC_FILTER_CTRL_KERNEL, &knl_rgn);
	API.SetDialogItemRect(m_hWnd, IDC_STATIC_FILTER_CTRL_KERNEL, CRect(knl_rgn.left, knl_rgn.top, COMB_FILTER_SIZE_MAX, COMB_FILTER_SIZE_MAX));

	pKernelIF = new CImageField(m_hWnd, CSize(COMB_FILTER_SIZE_MAX, COMB_FILTER_SIZE_MAX), CPoint(knl_rgn.left, knl_rgn.top));
	pKernelDU = NULL;
	



	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


void CFilterCtrlDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (pKernelIF) {
		delete	pKernelIF;
	}

	if (pKernelDU) {
		delete	pKernelDU;
	}

	if (pConvolutionKernel){
		delete[]	pConvolutionKernel;
	}
}

/********************************************************************
機  能  名  称 : 実行ボタン押下
関    数    名 : OnFilterExec
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CFilterCtrlDlg::OnFilterExec() 
{

	if(API.GetCheck(m_hWnd, IDC_RADIO_FILTER_OVERWITE))
	{
		API.BackupDataUnit(CURRENT_IMAGE);//現在のデータバックアップ

		if( !execute(CURRENT_IMAGE,CURRENT_IMAGE))	return ;

		API.DrawImage();//描画
	}
	else
	{
		int dst_no=0,src_no=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO, &src_no);
		API.GetEditValue(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO, &dst_no);


		if( !execute(src_no,dst_no))	return ;

		API.DrawImage(dst_no);//描画
	}

}


/********************************************************************
機  能  名  称 : 「現在選択中の画像メモリに上書き」
関    数    名 : OnBnClickedRadioFilterOverwite
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CFilterCtrlDlg::OnBnClickedRadioFilterOverwite()
{
	show_dlg_item();
}


/********************************************************************
機  能  名  称 : 
関    数    名 : OnBnClickedRadioFilterToImage
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CFilterCtrlDlg::OnBnClickedRadioFilterToImage()
{
	show_dlg_item();
}


/********************************************************************
機  能  名  称 : 
関    数    名 : OnCbnSelchangeComboFilterType
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CFilterCtrlDlg::OnCbnSelchangeComboFilterType()
{
	int			filterID = m_cmbo_filter_type.GetCurSel();
	CString		oldSizeStr("");

	//フィルタサイズ初期化
	if(m_cmbo_filter_size.GetCount()>0)
	{
		if(m_cmbo_filter_size.GetCurSel()>=0){
			m_cmbo_filter_size.GetLBText(m_cmbo_filter_size.GetCurSel(), oldSizeStr);//現在選択中の値があれば退避
		}

		while(m_cmbo_filter_size.DeleteString(0));//フィルタサイズコンボボックスの全要素をクリア
	}


	//フィルタサイズを設定
	for (int i = 0;  i<FILTER_SIZE_LIST_MAX; i++)
	{
		int kernel_size = m_filter_param[filterID].kernel_size[i];
		if(kernel_size>0)
		{
			CString str;
			str.Format("%d×%d", kernel_size, kernel_size);
			m_cmbo_filter_size.AddString(str);
		}
	}

	if (m_cmbo_filter_size.GetCount() > 0)
	{
		m_cmbo_filter_size.SetCurSel(m_filter_param[filterID].kernel_size_sel);

		//退避させていた値と同じものがあれば選択する
		for (int i = 0; i < m_cmbo_filter_size.GetCount(); i++){
			CString sizeStr;
			m_cmbo_filter_size.GetLBText(i, sizeStr);//現在選択中の値を保持
			if (sizeStr == oldSizeStr){
				m_cmbo_filter_size.SetCurSel(i);
				m_filter_param[filterID].kernel_size_sel = i;
				break;
			}
		}
	}


	//padding
	int padding_type = m_filter_param[filterID].padding_type;
	if (padding_type >= 0) 
	{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_STATIC_FILTER_CTRL_PAD), true);//「padding」グループボックス
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD1), true);//「ゼロ」ラジオボタン
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD2), true);//「周辺画素」ラジオボタン
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD0), true);//「折り返し」ラジオボタン

		if (padding_type == 1) {
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD1, true);
		}
		else if (padding_type == 2) {
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD2, true);
		}
		else {
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD0, true);
		}
	}
	else {
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_STATIC_FILTER_CTRL_PAD), false);//「padding」グループボックス
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD1), false);//「ゼロ」ラジオボタン
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD2), false);//「周辺画素」ラジオボタン
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD0), false);//「折り返し」ラジオボタン
	}



	//円形
	int circle_shape = m_filter_param[filterID].circle_shape;
	if (circle_shape >= 0) 
	{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE), SW_SHOW);//「円形マスク」チェックボックス

		if (circle_shape == 1) {
			API.SetCheck(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE, true);
		}
		else {
			API.SetCheck(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE, false);
		}
	}
	else {
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE), SW_HIDE);//「円形マスク」チェックボックス
	}


	//極性
	int direction = m_filter_param[filterID].direction;
	if (direction >= 0) 
	{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_XY), SW_SHOW);//「縦横両方」ラジオボタン
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_X), SW_SHOW);//「横方向のみ」ラジオボタン
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_Y), SW_SHOW);//「縦方向のみ」ラジオボタン

		if (direction == 1) {//横のみ
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_X,true);
		}
		else if (direction == 2) {//縦のみ
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_Y, true);
		}
		else {//縦横
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_XY, true);
		}
	}
	else
	{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_XY), SW_HIDE);//「縦横両方」ラジオボタン
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_X), SW_HIDE);//「横方向のみ」ラジオボタン
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_Y), SW_HIDE);//「縦方向のみ」ラジオボタン
	}

	//コンボリューションカーネル
	if (filterID == FILTER_CONVOLUTION)
	{
		set_conv_kernel(m_filter_param[filterID].kernel_size[m_filter_param[filterID].kernel_size_sel]);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_CTRL_CONV_KERNEL), SW_SHOW);
	}
	else{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_CTRL_CONV_KERNEL), SW_HIDE);
	}



	//その他のパラメタ
	int			caption[] = { IDC_STATIC_FILTER_CTRL_PARAM0 ,IDC_STATIC_FILTER_CTRL_PARAM1, IDC_STATIC_FILTER_CTRL_PARAM2, IDC_STATIC_FILTER_CTRL_PARAM3, IDC_STATIC_FILTER_CTRL_PARAM4, IDC_STATIC_FILTER_CTRL_PARAM5, IDC_STATIC_FILTER_CTRL_PARAM6 };
	int			ctrlID[] = { IDC_EDIT_FILTER_PARAM0 ,IDC_EDIT_FILTER_PARAM1, IDC_EDIT_FILTER_PARAM2, IDC_EDIT_FILTER_PARAM3, IDC_EDIT_FILTER_PARAM4, IDC_EDIT_FILTER_PARAM5, IDC_EDIT_FILTER_PARAM6 };

	for (int i = 0; i < FILTER_PARAM_MAX; i++) 
	{
		if (m_filter_param[filterID].param_name[i][0] != 0) 
		{
			::SetWindowText(::GetDlgItem(m_hWnd, caption[i]), CString(m_filter_param[filterID].param_name[i]));
			API.SetEditValue(m_hWnd, ctrlID[i], m_filter_param[filterID].param[i]);

			::ShowWindow(::GetDlgItem(m_hWnd, caption[i]), SW_SHOW);
			::ShowWindow(::GetDlgItem(m_hWnd, ctrlID[i]), SW_SHOW);
		}
		else {
			::ShowWindow(::GetDlgItem(m_hWnd, caption[i]), SW_HIDE);
			::ShowWindow(::GetDlgItem(m_hWnd, ctrlID[i]), SW_HIDE);
		}
	}

			

}



void CFilterCtrlDlg::OnCbnSelchangeComboFilterSize()
{
	int filterID = m_cmbo_filter_type.GetCurSel();

	m_filter_param[filterID].kernel_size_sel = m_cmbo_filter_size.GetCurSel();
	
	set_conv_kernel(m_filter_param[filterID].kernel_size[m_filter_param[filterID].kernel_size_sel]);
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
void CFilterCtrlDlg::OnBnClickedButtonFilterCtrlSetOrgImg()
{
	int orgNo;

	orgNo = API.GetCurrentImageNumber();

	if(orgNo>=0 && orgNo+1<API.GetDataUnitNumber())
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO, orgNo);
		API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO, orgNo+1);
	}

}

/********************************************************************
機  能  名  称 : 
関    数    名 : OnPaint
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CFilterCtrlDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	draw_kernel();
}