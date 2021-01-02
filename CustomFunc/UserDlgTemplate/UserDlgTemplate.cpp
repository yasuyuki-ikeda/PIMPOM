// UserDlgTemplate.cpp : DLL の初期化ルーチンです。
//

#include "stdafx.h"
#include "UserDlgTemplate.h"
#include "UserDlgEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ***************************************************************************
//　カスタムダイアログのオブジェクト
// ***************************************************************************
CUserDlgEx		*p_user_dlg = NULL;
CPimpomAPI		*p_api =NULL;

// ***************************************************************************
//  PimomCoreから見える関数実装
// ***************************************************************************

//ユーザダイアログの起動
USER	int		OpenUserDlg(CPimpomAPI *pAPI)
{
	p_api = pAPI;

	if(p_api){
		if(!p_user_dlg){
			p_user_dlg = new  CUserDlgEx();
			if(p_user_dlg){
				p_user_dlg->Create(IDD_USER_DLG_EX);	
			}
		}
		p_user_dlg->ShowWindow(SW_SHOW);
	}

	return 0;
}

//ユーザダイアログの破壊
USER	void		DestroyUserDlg()
{
	if(p_user_dlg){
		delete	p_user_dlg;
		p_user_dlg = NULL;
		p_api = NULL;
	}
}


//PIMPOM_APIへのポインタ参照
USER	CPimpomAPI*		GetAPI()
{
	return p_api;
}


//メイン画面描画
USER	void	OnDrawMainImage_UserDlg(int image_number)
{
	if (p_user_dlg){
		if (p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnDrawMainImage(image_number);
		}
	}
}

//メイン画面上でのマウスボタン押下
USER	void	OnMouseDown_UserDlg(POINT point)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnMouseDownOnImage(point);
		}
	}
}

//メイン画面上でのマウスドラッグ
USER	void	OnMouseDrug_UserDlg(POINT start_point, POINT currnet_point)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnMouseDrugOnImage(start_point, currnet_point);
		}
	}
}

//メイン画面上でのマウスドラッグ終了
USER	void	OnFinishMouseDrug_UserDlg(POINT start_point, POINT currnet_point)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnFinishMouseDrugOnImage(start_point, currnet_point);
		}
	}
}

//バッチ実行用
USER	void	OnBatchExecute_UserDlg(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnBatchExecute(in_param,out_param);
		}
	}
}

//TCP/IP通信受信
USER	void	OnReceive_UserDlg(BYTE *data, int *ndata)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnReceive(data, ndata);
		}
	}
}