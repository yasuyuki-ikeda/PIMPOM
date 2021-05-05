#pragma once

#include "resource.h"		// メイン シンボル
#include "opencv_headers.h"
#include "PIMPOM_API.h"


// ***************************************************************************
//　関数定義
// ***************************************************************************
#define USER		__declspec(dllexport)		/* User Functions/Variables			*/




extern "C" {
USER	int				OpenUserDlg(CPimpomAPI *pAPI);//ユーザダイアログの起動
USER	void			DestroyUserDlg();//ユーザダイアログの破壊
USER	CPimpomAPI*		GetAPI();
USER	void			OnDrawMainImage_UserDlg(int image_number);//メイン画面描画
USER	void			OnMouseDown_UserDlg(POINT point);//メイン画面上でのマウスボタン押下
USER	void			OnMouseDrug_UserDlg(POINT start_point, POINT currnet_point);//メイン画面上でのマウスドラッグ
USER	void			OnFinishMouseDrug_UserDlg(POINT start_point, POINT currnet_point);//メイン画面上でのマウスドラッグ終了
USER	void			OnBatchExecute_UserDlg(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);//バッチ実行
USER	void			OnReceive_UserDlg(BYTE *data, int *ndata);//TCP/IP通信受信
}

#define API		(*GetAPI())


