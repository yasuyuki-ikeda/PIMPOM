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
//CMarkDlg
//マークの設定ウィンドウ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "MarkDlg.h"

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


CMarkDlg::CMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkDlg)
	// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_MARK, m_list_contol);
}


/********************************************************************
機  能  名  称 : リストの選択中の要素番号を返す
関    数    名 : get_selected_list_item
引          数 : 
戻    り    値 : リストの選択中の要素番号
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int	CMarkDlg::get_selected_list_item()
{
	return m_list_contol.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
}

/********************************************************************
機  能  名  称 : リストの要素を選択する
関    数    名 : select_list_item
引          数 : int n (in)リストの要素番号
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::select_list_item(int n)
{
	m_list_contol.SetItemState(n, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_list_contol.SetSelectionMark(n);
	m_list_contol.EnsureVisible(n,true);
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
void CMarkDlg::show_dlg_item()
{
	RECT clientRct={0};//クライアント領域を表す矩形
	int clientWidth, clientHeight;

	::GetClientRect(m_hWnd, &clientRct);//クライアント領域を表す矩形を取得

	clientWidth = (clientRct.right - clientRct.left);
	clientHeight = (clientRct.bottom - clientRct.top);




	//マーク追加
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_MARK_TYPE0), 0, 0, 30, 30, true);//点
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_MARK_TYPE1), 30, 0, 30, 30, true);//線

	//マーク削除
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_MARK_DELETE), 60, 0, 30, 30, true);

	//マーク全削除
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_MARK_ALLDELETE), 90, 0, 30, 30, true);
	
	//マーク番号を画面表示するかどうかを選択するチェックボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_MARK_NUM_DISP), 120, 0, 30, 30, true);

	//マーク編集
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_MARK_EDIT), 150, 0, 30, 30, true);

	//値を表示するリストボックス
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_LIST_MARK), 0, 30, clientWidth, clientHeight - 40, true);
}



/********************************************************************
機  能  名  称 : リスト初期化
関    数    名 : init_list
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::init_list()
{
    LVCOLUMN    lvc;
    char       caption[32];

	m_list_contol.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // 有効フラグ


	//No.
	sprintf(caption, "ID");
    lvc.iSubItem    = 0;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 30;          // 横幅
    m_list_contol.InsertColumn(0, &lvc);


	//Type.
	sprintf(caption, "Type");
    lvc.iSubItem    = 1;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 20;          // 横幅
    m_list_contol.InsertColumn(1, &lvc);

	//X
	sprintf(caption, "x");
    lvc.iSubItem    = 2;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 50;          // 横幅
    m_list_contol.InsertColumn(2, &lvc);
   
	//Y
	sprintf(caption, "y");
    lvc.iSubItem    = 3;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 50;          // 横幅
    m_list_contol.InsertColumn(3, &lvc);

	//X
	sprintf(caption, "x2");
    lvc.iSubItem    = 4;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 50;          // 横幅
    m_list_contol.InsertColumn(4, &lvc);
   
	//Y
	sprintf(caption, "y2");
    lvc.iSubItem    = 5;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 50;          // 横幅
    m_list_contol.InsertColumn(5, &lvc);

	//value
	sprintf(caption, "pt.val");
    lvc.iSubItem    = 6;            // サブアイテム番号
    lvc.pszText     = caption;   // 見出しテキスト
    lvc.cx          = 50;          // 横幅
    m_list_contol.InsertColumn(6, &lvc);

	//attribute
	for(int i=0 ; i<MARK_ATTRIB_NUM ; i++)
	{
		sprintf(caption, "attrib%d",i);
		lvc.iSubItem    = 7+i;            // サブアイテム番号
		lvc.pszText     = caption;   // 見出しテキスト
		lvc.cx          = 50;          // 横幅
		m_list_contol.InsertColumn(7+i, &lvc);
	}
}


/********************************************************************
機  能  名  称 : リスト要素ソートのための比較関数
関    数    名 : compare_func
引          数 : 
戻    り    値 : 
機          能 : 比較対象が両方とも数値ならば、大きさで比較
　　　　　　　　それ以外の場合は文字列の順で比較　　　　　
		 日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CALLBACK CMarkDlg::compare_func(LPARAM param1, LPARAM param2, LPARAM param3)
{
	CMarkDlg* pDlg = (CMarkDlg*)param3;
	int nSubItem  = pDlg->m_iSubItem;
	CString    strItem1 = pDlg->m_list_contol.GetItemText(param1, nSubItem);
	CString    strItem2 = pDlg->m_list_contol.GetItemText(param2, nSubItem);

	double numItem1 = atof(strItem1);
	double numItem2 = atof(strItem2);
	if(numItem1!=0.0 && numItem2!=0.0)//数値どうしの比較
	{
		if(pDlg->m_sort_dir_flg)	
			return (numItem1>numItem2);
		else
			return (numItem1<numItem2);
	}
	else//文字列どうしの比較
	{
		if(pDlg->m_sort_dir_flg)
			return strcmp(strItem1, strItem2);
		else
			return strcmp(strItem2, strItem1);
	}
}


/********************************************************************
機  能  名  称 : マーク追加のタイプ
関    数    名 : GetAddMarkTipe
引          数 : -1:なし　0:点　1:線
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int CMarkDlg::GetMarkEditType()
{
	if( API.GetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0) )
	{
		return MARK_TYPE_POINT;
	}
	else if(API.GetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1))
	{
		return MARK_TYPE_LINE;
	}
	else 
	{
		return -1;
	}
}


/********************************************************************
機  能  名  称 : マーク追加のタイプ
関    数    名 : GetAddMarkTipe
引          数 : -1:なし　0:点　1:線
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::SetMarkEditType(int type)
{
	if (type== MARK_TYPE_POINT)
	{
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0, true);
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1, false);
	}
	else if (type == MARK_TYPE_LINE)
	{
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0, false);
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1, true);
	}
	else
	{
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0, false);
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1, false);
	}
}


/********************************************************************
機  能  名  称 : メイン画面上でマウスクリックされたときに呼ばれる関数
関    数    名 : OnMouseDownOnImage
引          数 : 
戻    り    値 :
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnMouseDownOnImage(float point_x, float point_y)
{
	if (!IsWindowVisible())	return ;//非表示状態では無効


	if (GetMarkEditType()== MARK_TYPE_POINT)
	{
		API.AddMark(CURRENT_IMAGE, point_x, point_y);//点マーク追加
	}
	else if (GetMarkEditType() == MARK_TYPE_LINE)
	{

	}
	else {
		
	}

	SelectMarkFromPoint(CURRENT_IMAGE, point_x, point_y);

}

/********************************************************************
機  能  名  称 : メイン画面上でマウスドラッグされたときに呼ばれる関数
関    数    名 : OnMouseDrugOnImage
引          数 : 
戻    り    値 :
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnMouseDrugOnImage(float point0_x, float point0_y, float point1_x, float point1_y)
{
	if (!IsWindowVisible())	return ;//非表示状態では無効

	if (GetMarkEditType() == MARK_TYPE_POINT)
	{

	}
	else if (GetMarkEditType() == MARK_TYPE_LINE)
	{
		API.DrawImage(CURRENT_IMAGE, false);//再描画
		API.DrawLine(point0_x, point0_y, point1_x, point1_y);
	}
	else {

	}

}


/********************************************************************
機  能  名  称 : メイン画面上でマウスクリックが外されたときに呼ばれる関数
関    数    名 : OnMouseUpOnImage
引          数 : 
戻    り    値 :
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnMouseUpOnImage(float point0_x, float point0_y, float point1_x, float point1_y)
{
	if (!IsWindowVisible())	return ;//非表示状態では無効

	if (GetMarkEditType() == MARK_TYPE_POINT)
	{

	}
	else if (GetMarkEditType() == MARK_TYPE_LINE)
	{
		API.AddLineMark(CURRENT_IMAGE, point0_x, point0_y, point1_x, point1_y);//線マーク追加
	}
	else {

	}
}

/********************************************************************
機  能  名  称 : メイン画面上に描画
関    数    名 : OnDrawOnImage
引          数 : 
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnDrawOnImage()
{
	if (!IsWindowVisible())	return;//非表示状態では無効

	CDataUnit	*p_du = API.GetDataUnit(CURRENT_IMAGE);//現在選択中の画像のDataUnitを取得する
	if (p_du == NULL) {
		return;//データが無ければ何もしない
	}


	for (int n = 0; n<p_du->GetMarkNumber(); n++)
	{
		CMark mark;
		p_du->GetMark(n, &mark);

		if (mark.type == MARK_TYPE_POINT) 
		{//点の場合
			if (n == highlight_mark_id)
			{
				API.DrawCursor(mark.figure.point.x, mark.figure.point.y, 2, RGB(255, 0, 0));//十字カーソルを赤で描画
			}
			else
			{
				API.DrawCursor(mark.figure.point.x, mark.figure.point.y);
			}
		}
		else 
		{//線分の場合
			if (n == highlight_mark_id)
			{
				API.DrawCircle(mark.figure.line.x1, mark.figure.line.y1, 2.0, 1, RGB(255, 0, 0));//始点に円をつける
				API.DrawLine(mark.figure.line.x1, mark.figure.line.y1, mark.figure.line.x2, mark.figure.line.y2, 2, RGB(255, 0, 0));//線を赤で表示
			}
			else
			{
				API.DrawLine(mark.figure.line.x1, mark.figure.line.y1, mark.figure.line.x2, mark.figure.line.y2);
			}
		}

		if (API.GetCheck(m_hWnd, IDC_CHECK_MARK_NUM_DISP))
		{//マークの番号を画面に描画する
				CString		str;
				str.Format("%d", n);
				API.DrawText(mark.figure.point.x, mark.figure.point.y, str, RGB(0, 0, 0));
		}
	}

	highlight_mark_id = -1;//選択を外す
}


/********************************************************************
機  能  名  称 : マークのリスト表示
関    数    名 : ShowMarkVal
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::ShowMarkVal(long image_num)
{
	CDataUnit	*pdu=NULL;
	LVITEM       lvi={0};
	lvi.mask = LVIF_TEXT;
	char        buf[1024];
	

	m_list_contol.DeleteAllItems();//一旦すべてのリスト要素をクリア

	pdu = API.GetDataUnit(image_num);
		if(pdu==NULL)	return;


	for(int i=0 ; i<pdu->GetMarkNumber() ; i++)
	{
		//API.GetMarkVal(image_num, i, &x, &y, &val, attrib);
		CMark mark;
		pdu->GetMark(i, &mark);

		//No.
		lvi.iItem = i;
		lvi.iSubItem = 0;
		sprintf(buf, "%d", i);
		lvi.pszText = buf;
		m_list_contol.InsertItem(&lvi);//リスト挿入


		if(mark.type == MARK_TYPE_POINT)
		{
			//Type.
			lvi.iItem = i;
			lvi.iSubItem = 1;
			sprintf(buf, "P");
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//x
			lvi.iItem = i;
			lvi.iSubItem = 2;
			sprintf(buf, "%0.3f", mark.figure.point.x);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//y
			lvi.iItem = i;
			lvi.iSubItem = 3;
			sprintf(buf, "%0.3f", mark.figure.point.y);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//data value
			lvi.iItem = i;
			lvi.iSubItem = 6;
			sprintf(buf, "%lf", pdu->GetDataValue( mark.figure.point.x, mark.figure.point.y ));
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);
		}
		else
		{
			//Type.
			lvi.iItem = i;
			lvi.iSubItem = 1;
			sprintf(buf, "L");
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//x
			lvi.iItem = i;
			lvi.iSubItem = 2;
			sprintf(buf, "%0.3f", mark.figure.line.x1);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//y
			lvi.iItem = i;
			lvi.iSubItem = 3;
			sprintf(buf, "%0.3f", mark.figure.line.y1);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//x2
			lvi.iItem = i;
			lvi.iSubItem = 4;
			sprintf(buf, "%0.3f", mark.figure.line.x2);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//y2
			lvi.iItem = i;
			lvi.iSubItem = 5;
			sprintf(buf, "%0.3f", mark.figure.line.y2);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//data value
			lvi.iItem = i;
			lvi.iSubItem = 6;
			sprintf(buf, "" );
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

		}

		//attribute
		for(int j=0 ; j<MARK_ATTRIB_NUM ; j++)
		{
			lvi.iItem = i;
			lvi.iSubItem = 7+j;
			sprintf(buf, "%lf", mark.attrib[j]);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);
		}
	}

}


/********************************************************************
機  能  名  称 : マークの選択
関    数    名 : SelectMarkFromPoint
引          数 : 
戻    り    値 : 
機          能 : 指定された点から最も近いマークを探し、選択する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::SelectMarkFromPoint(long image_num, double x, double y)
{
	CDataUnit	*pdu=NULL;
	double		nearestDst=9999;
	int			nearestID=-1;

	pdu = API.GetDataUnit(image_num);
	if(pdu==NULL)	return;


	for(int i=0 ; i<pdu->GetMarkNumber() ; i++)
	{
		CMark mark;
		pdu->GetMark(i,&mark);

		double dist;
		if(mark.type == MARK_TYPE_POINT){//点の場合＝２点間の距離で判断
			//2点間の距離
			dist = sqrt( (mark.figure.point.x - x)*(mark.figure.point.x - x) + (mark.figure.point.y - y)*(mark.figure.point.y - y) );
		}
		else//直線の場合＝始点と終点の位置から判断
		{
			float ip1 = (x - mark.figure.line.x1)*(mark.figure.line.x2 - mark.figure.line.x1) + (y - mark.figure.line.y1)*(mark.figure.line.y2 - mark.figure.line.y1);
			float ip2 = (x - mark.figure.line.x2)*(mark.figure.line.x1 - mark.figure.line.x2) + (y - mark.figure.line.y2)*(mark.figure.line.y1 - mark.figure.line.y2);
			
			if( ip1>0 && ip2>0 ){
				//点(x,y)と直線(x1,y1)-(x2,y2)の距離
				float a = mark.figure.line.y1 - mark.figure.line.y2;
				float b = -mark.figure.line.x1 + mark.figure.line.x2;
				float c = (-b * mark.figure.line.y1) + (-a * mark.figure.line.x1);
				float dim = sqrt(a*a + b*b);
				if(dim!=0){	
					dist = fabs(a*x + b*y + c) / dim;
				}else{
					dist = 9999;
				}
			}else if(ip1>0){
				//点(x,y)と(x2,y2)の距離
				dist = sqrt( (mark.figure.line.x2 - x)*(mark.figure.line.x2 - x) + (mark.figure.line.y2 - y)*(mark.figure.line.y2 - y) );
			}else{
				//点(x,y)と(x1,y1)の距離
				dist = sqrt( (mark.figure.line.x1 - x)*(mark.figure.line.x1 - x) + (mark.figure.line.y1 - y)*(mark.figure.line.y1 - y) );
			}
		}

		if(dist < nearestDst){
			nearestDst = dist;
			nearestID = i;
		}
	}

	if(nearestID>=0)
	{
		highlight_mark_id = nearestID;
		select_list_item(nearestID);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////
//  メッセージ ハンドラ
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMarkDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_MARK_NUM_DISP, OnCheckMarkNumDisp)
	ON_BN_CLICKED(IDC_BUTTON_MARK_ALLDELETE, OnButtonMarkAlldelete)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_MARK, &CMarkDlg::OnNMClickListMark)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_MARK, &CMarkDlg::OnLvnColumnclickListMark)
	ON_BN_CLICKED(IDC_BUTTON_MARK_DELETE, &CMarkDlg::OnBnClickedButtonMarkDelete)
	ON_BN_CLICKED(IDC_BUTTON_MARK_EDIT, &CMarkDlg::OnBnClickedButtonMarkEdit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MARK, &CMarkDlg::OnNMRClickListMark)
	ON_BN_CLICKED(IDC_CHECK_MARK_TYPE0, OnCheckMarkType0)
	ON_BN_CLICKED(IDC_CHECK_MARK_TYPE1, OnCheckMarkType1)
	ON_WM_SIZE()
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
BOOL CMarkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//ビットマップボタン
	delbmp.LoadBitmap(IDB_BUTTON_WORKAREA_DEL);
	editbmp0.LoadBitmap(IDB_BUTTON_MARK_EDIT);
	editbmp1.LoadBitmap(IDB_BUTTON_MARK_EDIT_LINE);
	clearbmp.LoadBitmap(IDB_BUTTON_MARK_ALLDELETE);
	editbmp.LoadBitmap(IDB_BUTTON_EDIT);
	idbmp.LoadBitmap(IDB_BUTTON_MARK_ID);

	((CButton*)GetDlgItem(IDC_BUTTON_MARK_DELETE))->SetBitmap( (HBITMAP)delbmp ); 
	((CButton*)GetDlgItem(IDC_CHECK_MARK_TYPE0))->SetBitmap((HBITMAP)editbmp0);
	((CButton*)GetDlgItem(IDC_CHECK_MARK_TYPE1))->SetBitmap((HBITMAP)editbmp1);
	((CButton*)GetDlgItem(IDC_BUTTON_MARK_ALLDELETE))->SetBitmap((HBITMAP)clearbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_MARK_EDIT))->SetBitmap((HBITMAP)editbmp);
	((CButton*)GetDlgItem(IDC_CHECK_MARK_NUM_DISP))->SetBitmap((HBITMAP)idbmp);

	//ツールチップ
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_MARK_TYPE0), "点マークを追加");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_MARK_TYPE1), "線マークを追加");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_MARK_DELETE), "選択中のマークを削除");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_MARK_ALLDELETE), "すべてのマークを削除");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_MARK_EDIT), "マークを編集");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_MARK_NUM_DISP), "マーク番号を表示");


	show_dlg_item();//ダイアログアイテムの整列

	init_list();//リスト初期化


	//値の初期化
	API.SetEditValue(m_hWnd, IDC_EDIT_MARK_FIND , 0);
	API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0 , true);
	m_sort_dir_flg = true;
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


/********************************************************************
機  能  名  称 :キーボードからの入力
関    数    名 : PreTranslateMessage
引          数 :
戻    り    値 :
機          能 : カーソルキー：カーソルの移動
Delキー：選択中の画像メモリクリア
Enter/Escキー：無効化
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
BOOL CMarkDlg::PreTranslateMessage(MSG* pMsg)
{
	POINT point;

	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_RETURN://Enter
			return  true;

		case VK_ESCAPE://Esc
			return  true;

		case VK_UP://↑
			GetCursorPos(&point);
			SetCursorPos(point.x, point.y - 1);
			return  true;

		case VK_DOWN://↓
			GetCursorPos(&point);
			SetCursorPos(point.x, point.y + 1);
			return  true;

		case VK_LEFT://←
			GetCursorPos(&point);
			SetCursorPos(point.x - 1, point.y);
			return  true;

		case VK_RIGHT://→
			GetCursorPos(&point);
			SetCursorPos(point.x + 1, point.y);
			return true;

		default:
			break;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		//ツールチップ表示
		if (pMsg->hwnd == GetDlgItem(IDC_CHECK_MARK_TYPE0)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_MARK_TYPE1)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_MARK_DELETE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_MARK_ALLDELETE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_MARK_EDIT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_MARK_NUM_DISP)->m_hWnd
			)
		{
			m_toolTip.RelayEvent(pMsg);
		}
		else
		{
			m_toolTip.Pop();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
機  能  名  称 : このウィンドウを表示状態にする
関    数    名 : OnShowWindow
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


}

/********************************************************************
機  能  名  称 : マークの番号画面描画のON／OFF変更
関    数    名 : OnCheckMarkNumDisp
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnCheckMarkNumDisp() 
{
	API.pPIMMOMDlg->DrawImage(true);
}



/********************************************************************
機  能  名  称 : 全消去ボタン押下
関    数    名 : OnButtonMarkAlldelete
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnButtonMarkAlldelete() 
{	
	if(! API.ClearMark(CURRENT_IMAGE) )	return;

	ShowMarkVal(CURRENT_IMAGE);
	API.pPIMMOMDlg->DrawImage(true);
}


/********************************************************************
機  能  名  称 : ウインドウ閉じる
関    数    名 : OnClose
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnClose()
{
	API.SetMarkEditMode(0);

	CDialog::OnClose();
}


/********************************************************************
機  能  名  称 : リスト選択
関    数    名 : OnNMClickListMark
引          数 : 
戻    り    値 : 
機          能 : 選択されたカーソルの色を変えて表示
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnNMClickListMark(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDataUnit	*pdu;
	int selectedid ;

	pdu = API.GetDataUnit(CURRENT_IMAGE);
	if( pdu==NULL )	return;

	if(m_list_contol.GetItemCount()==0)	return;

	selectedid = get_selected_list_item();
	if( 0<=selectedid && selectedid<pdu->GetMarkNumber())
	{
		CMark mark;
		pdu->GetMark(selectedid,&mark);
		highlight_mark_id = selectedid;

		
		if(mark.type == MARK_TYPE_POINT)//点マーク
		{
			API.pPIMMOMDlg->DispDataValue(mark.figure.point.x,mark.figure.point.y);
			API.pPIMMOMDlg->SetClickedCoordinate(CPoint(mark.figure.point.x, mark.figure.point.y));//メインウィンドウのクリック座標を移動
			API.pPIMMOMDlg->DrawImage(true);
		}
		else//線マーク
		{
			API.pPIMMOMDlg->DispDataValue(mark.figure.line.x1, mark.figure.line.y1);
			API.pPIMMOMDlg->SetClickedCoordinate(CPoint(mark.figure.line.x1, mark.figure.line.y1));//メインウィンドウのクリック座標を移動
			API.pPIMMOMDlg->DrawImage(true);
		}
	}else{
		API.pPIMMOMDlg->DrawImage(true);
	}

	*pResult = 0;
}


/********************************************************************
機  能  名  称 : リストの見出し部を押下
関    数    名 : OnLvnColumnclickListBatch
引          数 : 
戻    り    値 : 
機          能 : その項目でリスト要素をソートする
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnLvnColumnclickListMark(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	m_sort_dir_flg = !m_sort_dir_flg;//ソート方向を切り替える
	m_iSubItem = pNMLV->iSubItem;//リストの項目番号
	m_list_contol.SortItems( compare_func, (LPARAM)this );

	for ( int i = 0; i < m_list_contol.GetItemCount(); i++ ){
			m_list_contol.SetItemData( i, (DWORD)i );
	}

	*pResult = 0;
}

/********************************************************************
機  能  名  称 : 「削除」ボタンを押下
関    数    名 : OnBnClickedButtonMarkDelete
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnBnClickedButtonMarkDelete()
{
	CDataUnit	*pdu;
	int selectedid ;

	pdu = API.GetDataUnit(CURRENT_IMAGE);
	if( pdu==NULL )	return;

	if(m_list_contol.GetItemCount()==0)	return;

	selectedid = get_selected_list_item();
	if( 0<=selectedid && selectedid < pdu->GetMarkNumber())
	{
		pdu->DeleteMark(selectedid);

		API.DrawImage();
	}


}

/********************************************************************
機  能  名  称 : 「編集」ボタンを押下
関    数    名 : OnBnClickedButtonMarkEdit
引          数 : 
戻    り    値 : 
機          能 : 編集のためのダイアログを出す。編集後に再表示する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnBnClickedButtonMarkEdit()
{
	CDataUnit	*pdu;
	int selectedid ;

	pdu = API.GetDataUnit(CURRENT_IMAGE);
	if( pdu==NULL )	return;

	if(m_list_contol.GetItemCount()==0)	return;

	selectedid = get_selected_list_item();
	if( 0<=selectedid && selectedid<pdu->GetMarkNumber() )
	{
		CMark mark;
		pdu->GetMark(selectedid, &mark);

		CConfirmDlg cfm;

		if(mark.type == MARK_TYPE_POINT){
			cfm.SetTitle("マーカー編集(点)");
			cfm.SetWidth(50,50);
			cfm.RegistVar("x", &mark.figure.point.x);
			cfm.RegistVar("y", &mark.figure.point.y);
		}else{
			cfm.SetTitle("マーカー編集(線)");
			cfm.SetWidth(50,50);
			cfm.RegistVar("x1", &mark.figure.line.x1);
			cfm.RegistVar("y1", &mark.figure.line.y1);
			cfm.RegistVar("x2", &mark.figure.line.x2);
			cfm.RegistVar("y2", &mark.figure.line.y2);
		}

		for(int i=0 ; i<MARK_ATTRIB_NUM ; i++)
		{
			char caption[32];
			sprintf(caption, "attrib%d",i);
			cfm.RegistVar(caption, &mark.attrib[i]);
		}

		if(cfm.DoModal()==IDOK)
		{
			pdu->SetMark(selectedid, mark);

			API.DrawImage();
			API.DrawCursor(mark.figure.point.x, mark.figure.point.y, 2, RGB(255,0,0));
			select_list_item(selectedid);
		}
	}
}


/********************************************************************
機  能  名  称 : マウス右ボタンを押下
関    数    名 : OnNMRClickListMark
引          数 : 
戻    り    値 : 
機          能 : 編集のためのダイアログを出す。編集後に再表示する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnNMRClickListMark(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	OnBnClickedButtonMarkEdit();
	*pResult = 0;
}


/********************************************************************
機  能  名  称 : ウィンドウのサイズ変更
関    数    名 : OnSize
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CMarkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	show_dlg_item();
}



void CMarkDlg::OnCheckMarkType0()
{
	if (API.GetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0)) {
		API.SetMarkEditMode(2);
	}
}

void CMarkDlg::OnCheckMarkType1()
{
	if (API.GetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1)) {
		API.SetMarkEditMode(3);
	}
}