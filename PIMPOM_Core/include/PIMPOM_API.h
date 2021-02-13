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
//CPimpomAPI
//PIMPOM全体で共通的に使う変数・関数をここで実装する
/*******************************************************************/
#pragma once

//**************************************
//         インクルードファイル         
//**************************************
#include "global_define.h"
#include "global_typedef.h"

#include "resource_core.h"


#include "DataUnit.h"
#include "Calculate.h"
#include "SocketCtrl.h"
#include "PIMPOMDlg.h"
#include "DataDlg.h"
#include "HistgramDlg.h"
#include "PointCloudDlg.h"
#include "ConvertDataDlg.h"
#include "ResizeDataDlg.h"
#include "TransformDlg.h"
#include "ConfirmDlg.h"
#include "CalcDlg1.h"
#include "MarkDlg.h"
#include "IO3DDlg.h"
#include "FilterCtrlDlg.h"
#include "ConvertDataDlg.h"
#include "PointCloudDlg.h"
#include "DataPickDlg.h"
#include "BatchDlg.h"
#include "MultiImageDispDlg.h"
#include "C3DImgDispDlg.h"
#include "WorkAreaDlg.h"
#include "SocketCtrlDlg.h"
#include "BinalizeDlg.h"
#include "CommandPromptDlg.h"
#include "ChartDlg.h"
#include "PythonBatchDlg.h"



///*************************************
//         構造体   定   義         
///*************************************
typedef struct{
	CString			dllname;
	CString			caption;
	HINSTANCE		hInst;
}CUSTOM_FUNC;

typedef struct
{
	BYTE *pBytes;
	int	 width;
	int  height;
}BITMAP_FONT;

///*************************************
//         クラス   定   義         
///*************************************


class AFX_EXT_CLASS CPimpomAPI
{
public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//PIMPOM全体で使える変数
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//標準のモードレスダイアログへのポインタ
	CPIMPOMDlg		*pPIMMOMDlg;//メインウインドウのポインタ
	CDataDlg		*pDataDlg;//データウインドウのポインタ
	CHistgramDlg	*pHistgramDlg;//ヒストグラムウインドウのポインタ
	CFilterCtrlDlg  *pFlterCtrlDlg;//汎用フィルタウィンドウのポインタ
	CCalcDlg		*pCalcDlg;//計算設定用ウインドウのポインタ
	CBinalizeDlg	*pBinalizeDlg;//2値化ウィンドウのポインタ
	CBatchDlg		*pBatchDlg;//バッチ処理ウィンドウのポインタ
	CMultiImageDispDlg	*pMultiImageDispDlg;//複数画像表示用ウィンドウのポインタ
	C3DImgDispDlg	*p3DImgDispDlg;//3D画像の一覧表示ウィンドウのポインタ
	CWorkAreaDlg	*pWorkAreaDlg;//作業領域編集用ウインドウのポインタ
	CMarkDlg		*pMarkDlg;//マーク表示用ウインドウのポインタ
	CDataPickDlg	*pDataPickDlg;//データの直接編集用ウインドウのポインタ
	CSocketCtrlDlg	*pSocketCtrlDlg;//ソケット通信の設定用ウインドウのポインタ
	CChartDlg		*pChartDlg;//グラフ表示用ウィンドウのポインタ
	CCommandPromptDlg	*pCmdPrptDlg;//コマンドプロンプトウィンドウのポインタ
	CPythonBatchDlg *pPythonBatchDlg;//Pythonバッチ処理ウィンドウのポインタ



	//環境設定
	CSize		main_image_size;//メイン画像表示サイズ
	CSize		main_window_size;//メインウィンドウサイズ
	int			data_window_width;//データウィンドウの幅
	int			thumbneil_colum;//データウィンドウに表示するサムネイル１行の画像数（負数またはウィンドウに収まらない場合は強制的に折り返す）
	LOGFONT		default_font;//デフォルトフォント
	COLORREF	default_bg_color;//デフォルト背景色
	COLORREF	default_fg_color;//デフォルト前景色
	int			rgb_gain[3];//カラーをモノクロに変換するときのゲイン×1000
	int			socket_delimiter;//ソケット通信時のデリミタのタイプ(0:CRLF 1:CR 2:LF)
	CSize		small_image_size;//画像メモリ一覧表示ウィンドウに表示する画像サイズ	
	int			invisible;//画面不可視化
	int			batch_execute_rule;//バッチ実行のルール(0:コマンドプロンプト->カスタムファンクション 1：カスタムファンクション->コマンドプロンプト 　0,1以外:コマンドプロンプトのみ)
	int			capture_image_number;//ビデオキャプチャ結果がはいる画像番号
	int			pimpom_id;//PIMPOMのシリアルID(複数起動時)
	CString		python_batch_file;//バッチ実行するpythonのファイルパス

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//このクラスの生成・破棄
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	CPimpomAPI(void);
	~CPimpomAPI(void);
	CString			GetProfile(CString key1, CString key2);//iniデータを読み出す
	void			WriteProfile(CString key1, CString key2, CString str);//iniデータを設定する
	void			InitSettingFromProfile();


	//////////////////////////////////////////////////////////////////////////////
	//画像メモリの操作
	//////////////////////////////////////////////////////////////////////////////

	//画像メモリへのアクセス
	long			GetDataUnitNumber();//画像メモリの総数
	long			GetCurrentImageNumber();//現在選択中の画像メモリの番号を返す
	bool			SetCurrentImageNumber(long number);//現在選択中の画像メモリの番号を変更する（再描画はしない）
	bool			SetDispPage(long number, long page);
	bool			SetDispChannel(long number, long channel);
	CDataUnit		*GetDataUnit(long number);//DataUnitを返す (画像メモリが無い場合はNULLが返る)
	bool			GetDataTypeSize(long number, int *ptype, int *pwidth, int *pheight, int *ppages);//画像メモリの型・サイズ・奥行等取得
	bool			GetDataMinMax(long number, float *pMin, float *pMax);//データの最大・最小を取得する
	unsigned char	*GetByteMemory(long number, CSize size, bool do_clear_memory);//BYTE型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	float			*GetFloatMemory(long number, CSize size, bool do_clear_memory);//FLOAT型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	unsigned char	*Get3DMemory(long number, CSize size, long page, bool do_clear_memory);//3D型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	float			*GetF3DMemory(long number, CSize size, long page, bool do_clear_memory);//FLOAT3D型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	float			*GetComplexMemory(long number, CSize size, bool do_clear_memory);//複素数型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	float			*GetXYMemory(long number, CSize size, bool do_clear_memory);//XY型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	float			*GetXYZMemory(long number, CSize size, bool do_clear_memory);//XYZ型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	short			*GetShortMemory(long number, CSize size, bool do_clear_memory);//16bit符号付き型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	WORD			*GetWordMemory(long number, CSize size, bool do_clear_memory);//16bit型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	long			*GetLongMemory(long number, CSize size, bool do_clear_memory);//13bit符号付き型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	DWORD			*GetDwordMemory(long number, CSize size, bool do_clear_memory);//13bit型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	unsigned char	*GetRGBMemory(long number, CSize size, bool do_clear_memory);//RGBカラー型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	unsigned char	*GetRGB3DMemory(long number, CSize size, long page, bool do_clear_memory);//RGBカラー動画型メモリの先頭アドレスを取得(データが無い場合は新規生成)
	BYTE			*GetMaskMemory(long number);//マスクメモリの先頭アドレスを取得 (データが無い場合はNULLが返る)
	CString			GetSrcFilePathName(long number);//ファイルから読み込まれた画像メモリに対し、読み込み元のファイルのフルパスを返す
	CString			GetSrcFilePath(long number);
	CString			GetSrcFileName(long number);
	CString			GetSrcFileTitle(long number);
	


	//画像メモリの新規生成・コピー・削除・復元
	CDataUnit		*FormatDataUnit(long number, long type, CSize size, long page, bool do_clear_memory);//指定した形式の画像データのメモリを取得する (メモリ取得が失敗した場合はNULLが返る)
	bool			CopyDataUnit(long src_number, long dest_number, bool do_copy_data, bool do_copy_mask, bool do_copy_workarea, bool do_copy_range, bool do_copy_mark, bool do_copy_pos_scale);//画像メモリをコピーする
	bool			CopyDataUnit(long src_number, long dest_number, bool show_confirm=false);//画像メモリをコピーする
	bool			DeleteDataUnit(long number);//画像メモリを破棄する
	bool			BackupDataUnit(long number);//	画像メモリをバックアップする
	bool			RecoverDataUnit(long number);//バックアップされた画像メモリを復活させる


	//画像メモリの変換
	bool			ConvertDataUnit(long number, CONVERT_DATA_OPTION option, bool show_confirm=false);//画像メモリのタイプを変更
	bool			ConvertDataUnit(long number, int type_to, int rgb_to, int comp_to, bool satulate, int byte_from, int color_gray_method, bool show_message=false);
	bool			ConvertDispToByte(long number);//画像メモリをBYTE型にコンバート
	bool			ConvertMaskToByte(long number);//マスクをBYTE型にコンバート
	bool			ClipDataUnit(long number, CRect clip_rect, bool show_message=false);//画像をクリッピング
	bool			ClipDataUnit(long number, long dst_data_number, bool do_clear_dst, CRect clip_rect, bool show_message=false);//画像をクリッピング
	bool			ResizeDataUnit(long number, CSize new_size, bool do_stretch, 
									int offset_x=0, int offset_y=0, bool show_confirm=false, int reduction_type=0);//画像のサイズを変更
	bool			ResizeDataUnit(long number, long dst_data_number, bool do_clear_dst, 
									CSize new_size, bool do_stretch, int offset_x, int offset_y, bool show_message=false, int reduction_type=0);//画像のサイズを変更
	bool			PerspectiveTransDataUnit(long number, double p_param[8], bool show_message=false, int outRgnMode=0);//パースペクティブ変換
	bool			PerspectiveTransDataUnit(long number, long dst_data_number, bool do_clear_dst, double p_param[8], bool show_message=false, int outRgnMode = 0);//パースペクティブ変換
	bool			PerspectiveTransDataUnit(long number, 
											double ti, double tj, double tX, double tY, 
											double theta, double phai, double psi, 
											double fl_phisical, double ccd_pix_size, bool show_message=false, double *perspective_param=NULL,
											double magnify=1, int outRgnMode = 0);//撮像条件を想定したパースペクティブ変換
	bool			PerspectiveTransDataUnit(long number,long dst_data_number, bool do_clear_dst, 
											 double ti, double tj, double tX, double tY, 
											 double theta, double phai, double psi, 
											 double fl_phisical, double ccd_pix_size, bool show_message=false, double *perspective_param=NULL,
											 double magnify=1, int outRgnMode = 0);//撮像条件を想定したパースペクティブ変換
	bool			IsoTransDataUnit(long number, double ti, double tj, double theta, double magnify, bool show_message=false, double *perspective_param=NULL, int outRgnMode = 0);//平行移動、回転移動、拡大
	bool			IsoTransDataUnit(long number, long dst_data_number, bool do_clear_dst, 
									 double ti, double tj, double theta, double magnify, bool show_message=false, double *perspective_param=NULL, int outRgnMode = 0);//平行移動、回転移動、拡大
	bool			PerspectiveTransCoor(double p_param[8], double srcX, double srcY, double *pDstX, double *pDstY);//パースペクティブ座標変換
	bool			CalcDataValue(long number, long type, CSize size, CString com_str, bool satulate, CString *pErr_str=NULL, CProgressCtrl *pProg=NULL);//数式で画素値をきめる
	bool			CalcDataValue(long number, CString com_str, bool satulate, CString *pErr_str=NULL, CProgressCtrl *pProg=NULL);//数式で画素値をきめる
	bool			CalcMaskValue(long number, CString com_str, double mask_max, double mask_min, CString *pErr_str=NULL, CProgressCtrl *pProg=NULL);//数式でマスク値をきめる
	bool			FilterSmooth(long number, long dst_data_number, CSize filter_size, bool rem_min_max, bool toMultiImage=false);//平滑化（移動平均）
	bool			FilterGauss(long number, long dst_data_number, int filter_size, int direction = 0, int pad_type = 0, bool toMultiImage = false, float *pKernel = NULL);//平滑化（ガウシアン）
	bool			FilterGabor(long image_num, long dst_data_number, int filter_size, float gamma, float freq, float theta, float	psi, float offset = 0, float contrast = 1, int pad_type = 0, bool toMultiImage = false, float *pKernel = NULL);//ガボールフィルタ
	bool			FilterGaussUnshade(long image_num, long dst_data_number, int filter_size, float offset, float contrast, int direction = 0, int pad_type = 0, bool toMultiImage = false, float *pKernel = NULL);//ガウシアン自己差分によるシェーディング除去
	bool			FilterMedian(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false);//メディアンフィルタ
	bool			FilterBilateral(long image_num, long dst_data_number, int filter_size, float sigmaVal, float sigmaDist, bool toMultiImage=false);//バイラテラルフィルタ
	bool			FilterErosion(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false, bool doErosion = true);//収縮
	bool			FilterDilation(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false);//膨張
	bool			FilterOpening(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false);//オープニング
	bool			FilterClosing(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false);//クロージング
	bool			FilterEdgeSobel(long number, long dst_data_number, int filter_size, float min_thresh, int direction = 0, bool toMultiImage = false);//ソーベルによるエッジ抽出
	bool			FilterLaplaceGauss(long image_num, long dst_data_number, int filter_size, int direction = 0, float offset = 0, float contrast = 1, int pad_type = 0, bool toMultiImage = false, float *pKernel = NULL);//ラプラシアンオブガウシアンフィルタ
	bool			FilterEdgeLog(long number, long dst_data_number, int filter_size, double min_thresh, int direction = 0, int pad_type = 0, bool toMultiImage = false);//LoGによる細線エッジ抽出
	bool			FilterEdgeCanny(long image_num, long dst_data_number, int filter_size, float thresh, float thresh_weak,  bool toMultiImage=false);//cannyによる細線エッジ抽出
	bool			FilterEdgeSigmoid(long image_num, long dst_data_number, int filter_size, float k, float theta, float offset = 0, float contrast = 1, int pad_type = 0, bool toMultiImage = false, float *pOutKernel = NULL);
	bool			FilterMirror(long image_num, long dst_data_number, int dir, bool toMultiImage = false);//鏡像反転
	bool			FilterNega(long image_num, long dst_data_number, bool toMultiImage = false);//濃度反転
	bool			FilterConvolution(long image_num, long dst_data_number, CSize kernel_size, float *pKernel, float offset = 0, float contrast = 1, int pad_type = 0, bool toMultiImage=false);//コンボリューション
	bool			FilterCLAHE(long image_num, long dst_data_number, double clipLimit, int tileW, int tileH, bool toMultiImage = false);//CLAHE (Contrast Limited Adaptive Histogram Equalization)
	bool			FilterLocalContrast(long image_num, long dst_data_number, int filter_size, float contrast, bool toMultiImage);//局所コントラスト拡張(sliding windowで 255*( x - ave - s*σ)/(2*σ) )
	bool			FilterLocalContrastPatch(long image_num, long dst_data_number, int patch_cnt, bool toMultiImage);//局所コントラスト拡張(パッチ分割して (x-min)/(max-min))
	bool			FilterDFT(long image_num, long dst_data_number, bool invert);//DFT
	bool			Binalize(long src_number, long dst_number, double min_thresh, double max_thresh, bool inverse, BYTE fg_val = 255, BYTE bg_val = 0, bool show_message = false ,bool toMultiImage = false);//２値化
	bool			Binalize(long src_number, double min_thresh, double max_thresh, bool inverse);//２値化（結果をマスクに入れる）
	double			BinalizeThresholdOtsu(long src_number);//大津の手法で2値化閾値を求める
	double			BinalizeThresholdPTile(long src_number, int param);//Pタイルで2値化閾値を求める
	bool			BinalizeLocalGauss(long src_number, long dst_number, int filter_size, int offset, bool inverse, BYTE fg_val = 255, BYTE bg_val = 0, int pad_type = 0, bool show_message = false, bool toMultiImage = false);//ガウシアン自己差分による2値化
	bool			BinalizeLocalLaplaceGauss(long src_number, long dst_number, int filter_size, int offset, bool inverse, BYTE fg_val = 255, BYTE bg_val = 0, int pad_type = 0, bool show_message = false, bool toMultiImage = false);//LOGガウシアンによる2値化
	bool			BinalizeNick(long src_number, long dst_number, int windowSize, float k, bool inverse, BYTE fg_val = 255, BYTE bg_val = 0, bool show_message = false, bool toMultiImage = false);//NICKの2値化
	int				Label(long src_number, long dst_number, bool sort_label, int min_area, int *area = NULL, float *gx = NULL, float *gy = NULL, BYTE fg_val = 255, BYTE bg_val = 0, bool show_message = false, bool toMultiImage = false);//ラベリング
	int				HoughLines(long image_num, int pixSkip, int dirSkip, int voteThresh, double minLineLength, double maxLineGap );//Cannyフィルタなどで抽出したエッジ画像に対し、確率的Hough変換を行う
	bool			BlendPoisson(long image_num, long blend_data_number, long dst_data_number, int offsetx, int offsety, int mode=2);//ポワソン画像合成

	//2画像間演算
	bool			Calc2DataUnit(int calc_type, int src_imageA, int src_imageB, int dst_image, int dst_type, double weightA = 1, double weightB = 1, double bias = 0, bool satulate = true);
	bool			Linear2DataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, double weightA = 1, double weightB = 1, double bias = 0, bool satulate = true);//2画像の値の線形演算
	bool			SumDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate = true);//2画像の和
	bool			SubtractDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate = true);//2画像の差
	bool			SubAbsDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate); //2画像の差の絶対値
	bool			MultiplyDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate = true);//2画像の積
	bool			DivideDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate = true);//2画像の商
	bool			LinearDataUnit(int src_image, int dst_image, int dst_type, double weight, double bias, bool satulate = true);//1画像の値の線形演算
	bool			AbsDataUnit(int src_image, int dst_image);//画像の値の絶対値をとる


	//画像メモリへの描画
	bool			PrepeareDrawing(int num, int copy_src_num);//モノクロ画像をカラー画像にする
	void			DrawLineOnImage(long number, double x0, double y0, double x1, double y1, int pen_size, COLORREF col, int copy_image_num=-1);//直線描画（RGB）
	void			DrawLineOnImage(long number, double x0, double y0, double x1, double y1, int pen_size, double val);//直線描画（グレースケール）
	void			DrawLineOnImageF(long number, double a, double b, double c, int pen_size, COLORREF col, int copy_image_num=-1);//方程式から直線描画（RGB）
	void			DrawLineOnImageF(long number, double a, double b, double c, int pen_size, double val);//方程式から直線描画（グレースケール）
	void			DrawCursorOnImage(long number, double x, double y, int pen_size, COLORREF col, int copy_image_num=-1);//十字カーソル描画（RGB）
	void			DrawCursorOnImage(long number, double x, double y, int pen_size, double val);//十字カーソル描画（グレースケール）
	void			DrawCircleOnImage(long number, double cx, double cy, double rad, COLORREF col, bool fill, int copy_image_num);//円描画（RGB）
	void			DrawCircleOnImage(long number, double cx, double cy, double rad, double val, bool fill);//円描画（グレースケール）
	void			DrawRectOnImage(long number, double x0, double y0, double x1, double y1, COLORREF col, bool fill=false, COLORREF fill_col=0, int copy_image_num=-1);//矩形描画（RGB）
	void			DrawRectOnImage(long number, double x0, double y0, double x1, double y1, double val, bool fill=false, double fill_val=0);//矩形描画
	void			DrawTextOnImage(long number, int x, int y, char str[], COLORREF col, int src_image_num=-1);//テキスト描画（RGB）
	void			DrawTextOnImage(long number, int x, int y, char str[], double val);//テキスト描画（グレースケール）
	void			DrawTextOnImage(long number, int x, int y, COLORREF col, char *format, ...);//フォーマット指定でテキスト描画（RGB）
	void			DrawTextOnImage(long number, int x, int y, double val, char *format, ...);//フォーマット指定でテキスト描画（グレースケール）
	void			DrawPointOnImage(long number, int x, int y, int dot_size, COLORREF col, int copy_image_num=-1);//ドット描画（RGB）
	void			DrawPointOnImage(long number, int x, int y, int dot_size, double val);//ドット描画（グレースケール）
	void			DrawPointOnMask(long number, int x, int y, int dot_size, BYTE val);//ドット描画(マスク)
	void			FloodFillOnImage(long number, int x, int y, COLORREF col);//領域塗りつぶし（RGB）
	void			FloodFillOnImage(long number, int x, int y, double val);//領域塗りつぶし（グレースケール）
	void			FloodFillOnMask(long number, int x, int y, BYTE val);//領域塗りつぶし(マスク)
	void			FillWorkAreaOnImage(long number, COLORREF col);//作業領域内塗りつぶし（RGB）
	void			FillWorkAreaOnImage(long number, double val);//作業領域内塗りつぶし（グレースケール）
	void			FillWorkAreaOnMask(long number, BYTE val);//作業領域内塗りつぶし(マスク)
	void			DrawImageOnImage(long number, long src_image_num, int offsetX, int offsetY, int mixType);

	//画像メモリの表示
	void			DrawImage(long number = CURRENT_IMAGE, bool do_update = true, bool do_adjust = false, bool redraw_background=true);//画像を表示する
	void			DrawImage(long number, CString name, bool do_update, bool do_adjust);//画像を表示する
	bool			ScaleImage(long number, float scale);//表示画像スケール変更
	void			DrawCurrentImageFast(bool do_update = true);//現在選択中の画像を表示する
	bool			SetDispRange(long number = CURRENT_IMAGE, float min=0 , float max=256);//表示濃度のレンジを変更して描画する
	bool			SetDispRangeAuto(long number = CURRENT_IMAGE, int type=0);//表示濃度のレンジを変更して描画する
	bool			ChangePalette(int number, short palette_id);//カラーパレットを変更する

	//作業領域
	bool			SetWorkArea(long number, long figure_number, RECT rect, bool ornot );//作業領域を矩形に設定（矩形は正規化されていなくてもよい）
	bool			SetWorkArea(long number, long figure_number, FIGURE_LINE line, bool ornot);//作業領域を幅つき直線に設定
	bool			SetWorkArea(long number, long figure_number, FIGURE_ELLIPSE ellipse, bool ornot);//作業領域を楕円に設定
	bool			SetWorkArea(long number, long figure_number, FIGURE_CIRCLE circle, bool ornot);//作業領域を幅つき円周に設定
	bool			SetWorkArea(long number, long figure_number, FIGURE_ARC arc, bool ornot);//作業領域を幅つき円弧に設定
	int				GetWorkArea(long number, long figure_number, FIGURE_UNION *area, bool *ornot);//作業領域を取得（戻り値は作業領域の種類。失敗したら-1）


	//マーク
	long			AddMark(long number, float x, float y, double *p_attrib=NULL);//点マークを追加する
	long			AddLineMark(long number, float x, float y, float x2, float y2, double *p_attrib=NULL);//線マークを追加する
	long			AddMark(long number, CMark src_mark);//マークを追加する
	bool			DeleteMark(long number, long mark_number);//マークを削除する
	bool			ClearMark(long number);//すべてのマークを削除する
	bool			GetMarkVal(long number, long mark_number, float *px, float *py, double *p_val, double *p_attrib=NULL);//マークの座標を取得する
	long			GetMarkNumber(long number);//マークの数を取得する


	//////////////////////////////////////////////////////////////////////////////////
	//メイン画面への表示関する処理
	//////////////////////////////////////////////////////////////////////////////////
	//GDIで描画
	bool			ClipSegment( CRect region , double *p0x, double *p0y, double *p1x, double *p2y);//領域で線分を切り取る
	bool			ClipSegmentF( CRect region , double a, double b, double c, double *p0x, double *p0y, double *p1x, double *p1y);//領域で直線(ax+by+c=0)を切り取る
	bool			DrawClippedSegment(CDC *p_dc, CRect region , double p0x, double p0y, double p1x, double p1y, int size, COLORREF col);///領域で切り取った線分を描画する．
	bool			DrawClippedSegment(CDC *p_dc, CRect region , double p0x, double p0y, double p1x, double p1y);///領域で切り取った線分を描画する．
	bool			DrawClippedRect(CDC *p_dc, CRect region , CRect draw_rect, int size, COLORREF col);//領域で切り取った矩形を描画する
	bool			DrawClippedRect(CDC *p_dc, CRect region , CRect draw_rect);//領域で切り取った矩形を描画する
	void			DrawLine(double x0, double y0, double x1, double y1, int pen_size, COLORREF col, int width=1);//線分を描画
	void			DrawLine(double x0, double y0, double x1, double y1, int width=1);//線分を描画
	void			DrawCursor(double x, double y, int pen_size, COLORREF col);//十字カーソルを描画
	void			DrawCursor(double x, double y);//十字カーソルを描画
	void			DrawRect(double left, double top, double right, double bottom, int pen_size, COLORREF col);//矩形描画
	void			DrawRect(double left, double top, double right, double bottom);//矩形描画
	void			DrawCircle(double x, double y, double r, int pen_size, COLORREF col, int width=1);//円を描く
	void			DrawCircle(double x, double y, double r, int width=1);//円を描く
	void			DrawEllipse(double x, double y, double rx, double ry, int pen_size, COLORREF col, int width=1);//楕円を描画
	void			DrawEllipse(double x, double y, double rx, double ry);//楕円を描く
	void			DrawText(double x, double y, CString str, COLORREF col, int size=-1);//テキスト描画
	void			DrawText(double x, double y, CString str);//テキスト描画
	void			DrawText(double x, double y, COLORREF col, int size, char *format, ...);//フォーマット指定でテキスト描画

	//メイン画面上の座標と画像座標との対応をとる
	bool			GetSrcCoor( double dest_coor_x, double dest_coor_y , double *p_src_coor_x, double *p_src_coor_y);//メイン画面の座標から画像内の座標に変換する
	bool			GetDestCoor( double src_coor_x , double src_coor_y , double *p_dest_coor_x, double *p_dest_coor_y);//画像内の座標からメイン画面内の座標に変換する


	///////////////////////////////////////////////////////////////////////////////////
	//ダイアログリソースとのデータのやり取り		
	///////////////////////////////////////////////////////////////////////////////////
	bool			GetDialogItemRect(HWND h_wnd, UINT id, CRect *pRct);//ダイアログアイテムの領域を取得する
	bool			SetDialogItemRect(HWND h_wnd, UINT id, CRect Rct);//ダイアログアイテムの領域を設定する
	bool			GetEditValue(HWND h_wnd, UINT id , int *value);//エディットボックスからデータ(int)を取り出す
	bool			GetEditValue(HWND h_wnd, UINT id , short *value);//エディットボックスからデータ(short)を取り出す
	bool			GetEditValue(HWND h_wnd, UINT id , float *value);//エディットボックスからデータ(float)を取り出す
	bool			GetEditValue(HWND h_wnd, UINT id , double *value);//エディットボックスからデータ(double)を取り出す
	bool			SetEditValue(HWND h_wnd, UINT id , double value);//エディットボックスにデータ(double)を設定する
	bool			GetEditValue(HWND h_wnd, UINT id , CString *str);//エディットボックスからデータ(CString)を取り出す
	bool			SetEditValue(HWND h_wnd, UINT id , CString str);//エディットボックスにデータ(CString)を設定する
	bool			GetCheck(HWND h_wnd, UINT id );//ボタンからチェックの状態を取得する
	bool			SetCheck(HWND h_wnd, UINT id ,bool value);//ボタンにチェックの状態を設定する
	CString			ToString(float value, int ordered_length=-1, char fillchar=' ', char filldir=1);//数値を文字列に変換する
	bool			ToNumber(CString str, double *presult);//文字列を数値に変換する
	void			ShowMessage(bool do_clear, int num, bool force, CString str);//メッセージを表示する（do_clearをtrueにすると前のメッセージを消す）
	void			ShowMessage(bool do_clear, int num, bool force, char *format, ...);//フォーマット指定でメッセージを表示する
	void			ShowMessage(bool do_clear, char *format, ...);//フォーマット指定でメッセージを表示する（メッセージ１に強制表示）
	void			MessageBox(char *format, ...);//フォーマット指定でメッセージボックスを表示する
	bool			DialogOpen(int id, bool show);
	void			DialogDelete(int id);
	bool			UserDialogOpen(int n);
	bool			UserDialogDelete(int n);
	bool			UserDialogOnDrawMainImage(int n, int image_number);
	bool			UserDialogOnMouseDown(int n, POINT point);
	bool			UserDialogOnMouseDrug(int n, POINT start_point, POINT currnet_point);
	bool			UserDialogOnFinishMouseDrug(int n, POINT start_point, POINT currnet_point);
	bool			UserDialogBatchExecute(int n, BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);
	bool			UserDialogReceive(int n, BYTE *data, int *ndata);
	bool			BatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);
	int				GetUserDlgCaptions(CString caption[]);//ユーザダイアログのキャプションを取得

	//メインウィンドウ付属の各種機能状態取得・変更
	int				GetMarkEditMode();//マーク編集モードの状態取得
	void			SetMarkEditMode(int mode);//マーク編集モードの状態設定
	bool			GetWorkAreaEditMode();//作業領域編集モードの状態取得
	void			SetWorkAreaEditMode(bool onoff);//作業領域編集モードの状態設定
	bool			GetRulerToolMode();//2点間の距離測定ツールの状態取得
	void			SetRulerToolMode(bool onoff);//2点間の距離測定ツールの状態設定
	int				GetDataPickEditType();//フリーハンド編集モードの状態取得
	void			SetDataPickEditType(int mode);//フリーハンド編集モードの状態設定
	bool			GetCalcToolMode();//画像生成の状態取得
	void			SetCalcToolMode(bool onoff);//画像生成の状態取得
	bool			GetHistgramToolMode();//ヒストグラムの状態取得
	void			SetHistgramToolMode(bool onoff);//ヒストグラムの状態取得
	int				GetVideoCaptureThroughMode();//ビデオキャプチャのスルー表示状態の取得
	void			SetVideoCaptureThroughMode(int mode);//ビデオキャプチャのスルー表示状態の設定
	bool			CopyToClipbord(int image_num=CURRENT_IMAGE);//表示画像をクリップボードにコピーする
	bool			PasteFromClipbord(int image_num);//クリップボードにある画像をメモリに入れる（ＲＧＢのみ）

	////////////////////////////////////////////////////////////////////////////////
	//ファイルIO
	////////////////////////////////////////////////////////////////////////////////
	bool			SetFileIOLog(CString filename);//ファイルIOのログをとる
	CString			GetFileIOLog(int no);//ファイルIOのログを返す
	///ロード
	bool			LoadByteImage(long number, CString	filename, CSize size);//8bit型のデータを読み込む
	bool			LoadByteImage(BYTE *p_data , CString	filename, CSize size);//8bit型のデータを読み込む
	bool			LoadShortImage(long number, CString filename, CSize size);//16bit符号つき型のデータを読み込む
	bool			LoadShortImage(short *p_data, CString filename, CSize size);//16bit符号つき型のデータを読み込む
	bool			LoadWordImage(long number, CString	filename, CSize size);//16bit符号なし型のデータを読み込む
	bool			LoadWordImage(WORD *p_data, CString	filename, CSize size);//16bit符号なし型のデータを読み込む
	bool			LoadLongImage(long number, CString	filename, CSize size);//32bit符号つき型のデータを読み込む
	bool			LoadLongImage(long *p_data, CString	filename, CSize size);//32bit型のデータを読み込む
	bool			LoadDwordImage(long image_number, CString filename, CSize size);//32bit符号なし型のデータを読み込む
	bool			LoadDWordImage(DWORD *p_data, CString filename, CSize size);//32bit符号なし型のデータを読み込む
	bool			LoadBitmap(long number, CString	filename, int partX=-1, int partY=-1, int partW=-1, int partH=-1);//ビットマップを読み込む
	bool			LoadSizedBitmap(BYTE *p_data , CSize size, bool is_rgb, CString	filename);//大きさ指定でビットマップを開く
	bool			LoadAvi(long number, CString	filename);//AVIを読み込む
	bool			LoadFloatImage(long number, CString	filename, CSize size);//float(32浮動小数点)型のデータを読み込む
	bool			LoadFloatImage(float *p_data, CString	filename, CSize size);
	bool			LoadComplexImage(long number, CString	filename, CSize size);//複素数型のデータを読み込む
	bool			LoadXYImage(long number, CString	filename, CSize size);//XY型のデータを読み込む
	bool			LoadXYZImage(long number, CString	filename, CSize size);//XYZ型のデータを読み込む
	bool			Load3DImage(long number, CString	filename, CSize size, long page);//3D型（8bit動画）データを読み込む
	bool			LoadF3DImage(long number, CString	filename, CSize size, long page);//Float3D（32bit動画）データを読み込む
	bool			SelectAndLoadImageFile(long number);//拡張子からタイプを判断してファイルを開く
	bool			SelectAndLoadImageFile(long number, CString filename, short width=0, short height=0, short page=1);//拡張子からタイプを判断してファイルを読み込む
	bool			LoadArchivedImage(long number , CString filename);//IMAGE型ファイルを開く
	bool			LoadAnyImage(long number, CString filename);//Bitmap以外の画像を開く
	bool			LoadIFZ(long number, CString filename);
	bool			LoadBFZ(long number, CString filename);

	///セーブ
	bool			SaveViewAsRaw(long number, CString	pathname);//表示データをRAWファイルとして保存する
	bool			SaveViewAsBitmap(long number, CString	pathname);//表示データをビットマップとして保存する
	bool			SaveViewAsBitmap(long number);//表示データをビットマップとして保存する
	bool			SaveByteImage(BYTE *p_data , CString	pathname, CSize size, long page);//8bit型のデータを保存する
	bool			SaveShortImage(short *p_data , CString	pathname, CSize size, long page);//16bit符号付き型のデータを保存する
	bool			SaveWordImage(WORD *p_data , CString	pathname, CSize size, long page);//16bit型のデータを保存する
	bool			SaveDwordImage(DWORD *p_data , CString	pathname, CSize size, long page);//32bit符号付き型のデータを保存する
	bool			SaveLongImage(long *p_data , CString	pathname, CSize size, long page);//32bit型のデータを保存する
	bool			SaveByteAsBitmap(BYTE *p_data , BITMAPINFO *pBmpInfo, CSize	size, CString pathname);
	bool			SaveFloatImage(float *p_data, CString	pathname, CSize size, long page);//float(32浮動小数点)型のデータを保存する
	bool			SelectAndSaveImageFile(long number);//タイプ別にファイルを保存する
	bool			SelectAndSaveImageFile(long number, CString pathname);//タイプ別にファイルを保存する
	bool			SaveArchivedImage(long number , CString pathname);//*pim型ファイルを保存する
	bool			SaveArchivedImage(long number );//*pim型ファイルを保存する

	//ファイルダイアログを表示する
	bool			OpenFileDialog(int file_number_max, CString file_kinds,	
							 CString *pfile_path, CString pfile_names[], int *pfile_number,  char *def_path=NULL);
	bool			OpenFileDialog(int file_number_max, CString file_kind_ext, CString file_kind_disp,
							CString *pfile_path, CString	pfile_names[], int *pfile_number, char *def_path=NULL);
	bool			SaveFileDialog(CString	file_kinds, CString	*pfile_path);
	bool			FolderDialog(CString *pfolder);
	int				GetFileList(CString folderpath, CString file_kind_ext, int file_number_max, CString	pfile_names[]);

	static CString GetDefaultPath();//アプリケーションの絶対パスを取得する
	static CString GetVersion(const char  *KeyWord);//アプリケーションのバージョン情報を取得する



	////////////////////////////////////////////////////////////////////////////////
	//数学計算
	////////////////////////////////////////////////////////////////////////////////
	double			calc_mod( double x , double a);//剰余
	float			calc_float_max( float *pf , long len);//配列の最大値
	float			calc_float_min( float *pf , long len);//配列の最小値
	double			calc_random();//一様分布の乱数発生（0-1）
	double			calc_gaus();//ガウス分布の乱数発生（標準偏差1）
	double			calc_dist(double x0, double y0, double x1, double y1);//ユークリッド距離
	bool			calc_lsm(float **ppA, float *pb, float *pX, int spl_num, int param_num);//最小二乗法
	bool			calc_dft(float *pSrc, int length, float *pPower, float *pPhase=NULL);//離散フーリエ変換

	///////////////////////////////////////////////////////////////////////////////////
	//グラフ表示		
	///////////////////////////////////////////////////////////////////////////////////
	int				AddDataToChart(int chart_id, float value);//折れ線グラフに値を追加して描画する
	bool			PlotDataToChart(int chart_id, int data_num, float *pValueArr, bool doClear=true);
	bool			PlotDataToChart(int chart_id, int data_num, double *pValueArr, bool doClear=true);
	bool			PlotDataToChart(int chart_id, int data_num, int *pValueArr, bool doClear=true);
	bool			PlotDataToChart(int chart_id, int data_num, BYTE *pValueArr, bool doClear=true);
	int				AddDataToChart2(int chart_id, float valueX, float valueY);// 2次元散布図に値を追加して描画する
	bool			PlotDataToChart2(int chart_id, int data_num, float *pValueArrX, float *pValueArrY, bool doClear=true);
	bool			PlotDataToChart2(int chart_id, int data_num, double *pValueArrX, double *pValueArrY, bool doClear=true);
	bool			PlotDataToChart2(int chart_id, int data_num, int *pValueArrX, int *pValueArrY, bool doClear=true);
	bool			PlotDataToChart2(int chart_id, int data_num, BYTE *pValueArrX, BYTE *pValueArrY, bool doClear=true);
	void			ClearChart(int chart_id);


	////////////////////////////////////////////////////////////////////////////////
	//OpenCVサポート
	////////////////////////////////////////////////////////////////////////////////
	void*			ConvertToCvImage(long number);//画像データをIplImage型に変換する
	bool			ConvertToCvImage2(long image_num, void *pSrcMat, void *pClippedMat=NULL);//画像データをcv::Matに変換する
	bool			Img2Mat(long image_num, void *pSrcMat, void *pClippedMat = NULL);//画像データをcv::Matに変換する
	bool			ConvertFromCvImage(long number, void *pMat, long src_image_num=-1);//IplImage型を画像データに変換する
	bool			ConvertFromCvImage2(long image_num, void *pMat, long src_image_num=-1);//cv::Matを画像データに展開する
	bool			Mat2Img(long image_num, void *pMat, long src_image_num = -1);//cv::Matを画像データに展開する

	////////////////////////////////////////////////////////////////////////////////
	//ビデオキャプチャ
	////////////////////////////////////////////////////////////////////////////////
	bool			CaptureInit(CSize size);//ビデオキャプチャドライバを開く
	void			CaptureDeInit();//キャプチャドライバを閉じる
	bool			CaptureOne(bool doDrawImage=true);//画像１まい取り込み
	bool			IsCaptureOpened();//ビデオキャプチャが開かれているかどうか

	////////////////////////////////////////////////////////////////////////////////
	//通信
	////////////////////////////////////////////////////////////////////////////////
	//TCP/IP
	bool			OpenSocket(int type, UINT port, CString host, HWND wnd=NULL, void (*recive_callback)(HWND, BYTE*, int*)=NULL, void (*acc_callback)(HWND, int*)=NULL, void (*close_callback)(HWND)=NULL);//ソケットを開く
	int				SendString(CString str);//文字列を送信する
	void			CloseSocket();//ソケットを閉じる
	int				GetSocketStatus(CString* pHostName, UINT* pPortNo);//ソケットの状態取得 0:サーバ 1:クライアント -1:開かれず

	////////////////////////////////////////////////////////////////////////////////
	//バックグラウンド処理
	////////////////////////////////////////////////////////////////////////////////
	int				GetBGProcID();//ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞ処理のID取得
	void			SetBGProcID(int id);//ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞ処理のID設定
	void			StopBGProc();//ﾊﾞｯｸｸﾞﾗｳﾝﾄﾞ処理終了

	unsigned long	Tick();


private:
	long			data_number;//画像メモリ数
	CDataUnit		**pp_data_unit;//画像メモリ
	long			current_image_number;//現在選択中の画像番号
	CDataUnit		*backup_data;//バックアップ用画像メモリ
	int				background_proc_id;//バックグラウンド処理（別スレッド）のID
	CSocketCtrl		*pSockCtrl;//ソケット通信
	CUSTOM_FUNC		custom_func[CUSTOM_FUNC_MAX];//カスタムファンクション
};


