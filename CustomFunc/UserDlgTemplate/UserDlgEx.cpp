// UserDlg.cpp : 実装ファイル
//



#include "stdafx.h"
#include "UserDlgTemplate.h"
#include "UserDlgEx.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUserDlgExメンバ関数
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CUserDlgEx, CDialog)


//コンストラクタ
CUserDlgEx::CUserDlgEx(CWnd* pParent /*=NULL*/)
	: CDialog(CUserDlgEx::IDD, pParent)
{

	
}

//デストラクタ
CUserDlgEx::~CUserDlgEx()
{

}

//DDX
void CUserDlgEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// メイン画面操作のメッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//メイン画面描画
void CUserDlgEx::OnDrawMainImage(int image_number)
{

}

//メイン画面上でのマウスボタン押下
void CUserDlgEx::OnMouseDownOnImage(CPoint point)
{

}

//メイン画面上でのマウスドラッグ中
void CUserDlgEx::OnMouseDrugOnImage(CPoint start_point, CPoint currnet_point)
{

}

//メイン画面上でのマウスドラッグ終了
void CUserDlgEx::OnFinishMouseDrugOnImage(CPoint start_point, CPoint currnet_point)
{

}

//TCP/IP通信によるデータ受信
void CUserDlgEx::OnReceive(BYTE *data, int *ndata)
{

}

//バッチ実行用
void CUserDlgEx::OnBatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUserDlgメッセージ ハンドラ
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserDlgEx, CDialog)

END_MESSAGE_MAP()


BOOL CUserDlgEx::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


