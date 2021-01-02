#pragma once

#include "resource.h"		// メイン シンボル


#include	"afxcmn.h"


// CUserDlg ダイアログ

class AFX_EXT_CLASS CUserDlgEx : public CDialog
{
	DECLARE_DYNAMIC(CUserDlgEx)

public:
	CUserDlgEx(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CUserDlgEx();
	void		OnDrawMainImage(int image_number);//メイン画面描画
	void		OnMouseDownOnImage(CPoint point);//メイン画面上でのマウスボタン押下
	void		OnMouseDrugOnImage(CPoint start_point, CPoint currnet_point);//メイン画面上でのマウスドラッグ中
	void		OnFinishMouseDrugOnImage(CPoint start_point, CPoint currnet_point);//メイン画面上でのマウスドラッグ終了
	void		OnBatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);//バッチ実行用
	void		OnReceive(BYTE *data, int *ndata);//TCP/IP通信を受信した

	// ダイアログ データ
	enum { IDD = IDD_USER_DLG_EX };

	


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()


};
