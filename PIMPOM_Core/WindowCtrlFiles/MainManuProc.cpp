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
//CPIMPOMDlg
//メインメニューからの処理はここで実装する
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "shlwapi.h"


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

////////////////////////////////////////////////////////////////////////////////////////////
//メニュー追加
////////////////////////////////////////////////////////////////////////////////////////////
void CPIMPOMDlg::OnInitMenu(CMenu* pMenu)
{
	CDialog::OnInitMenu(pMenu);
}

/********************************************************************
機  能  名  称 : メニューが選択された
関    数    名 : OnMenuSelect
引          数 : 
戻    り    値 : 
機          能 : 「最近使ったファイルを開く」メニューに表示する内容の更新
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CDialog::OnMenuSelect(nItemID, nFlags, hSysMenu);

	//「最近使ったファイルを開く」メニューの更新
	if(nItemID<FILE_IO_LOG_MENU_ID || nItemID>=FILE_IO_LOG_MENU_ID + FILE_IO_LOG_MAX )
	{
		CMenu *menu = this->GetMenu();
		if(!menu)	return;

		CMenu *pSub =menu->GetSubMenu(0);
		if(!pSub)	return;

		CMenu *pSub2 =pSub->GetSubMenu(2);
		if(!pSub2)	return;

		for(int i=0 ; i<FILE_IO_LOG_MAX ; i++){
			pSub2->RemoveMenu(MF_BYCOMMAND, FILE_IO_LOG_MENU_ID+i);
		}

		for(int i=0 ; i<FILE_IO_LOG_MAX ; i++){
			CString filename = API.GetFileIOLog(i);
			if(!filename.IsEmpty()){
				pSub2->AppendMenu(MF_STRING,FILE_IO_LOG_MENU_ID+i, filename);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//「ファイル」メニュー
////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 : 画像ファイルを開く
関    数    名 : OnFileOpen
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリにロード
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileOpen() 
{
	if( API.pDataDlg->DataExistMessage("上書きしてもよろしいですか？", CURRENT_IMAGE) )		return;
	API.SelectAndLoadImageFile( API.GetCurrentImageNumber() );
}

/********************************************************************
機  能  名  称 : 複数のファイルを3D/F3Dとして開く
関    数    名 : OnFileOpenSerial
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリにロード
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileOpenSerial() 
{
	CIO3DDlg	iodlg;
	if( API.pDataDlg->DataExistMessage("上書きしてもよろしいですか？", CURRENT_IMAGE) )		return;
	iodlg.DoModalLoadType( API.GetCurrentImageNumber() );
}

/********************************************************************
機  能  名  称 :画像メモリ一括読み込み
関    数    名 : OnFileLoadAllimage
引          数 :
戻    り    値 :
機          能 : [画像メモリ一括保存]にて保存された画像の読み込み


日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileLoadAllimage()
{
	CString dirpath = API.GetDefaultPath();
	dirpath = dirpath.Left(dirpath.GetLength()-1);//\を消す
	
	if (API.FolderDialog(&dirpath))
	{
		//画像メモリを読み込み
		for (int i = 0; i<API.GetDataUnitNumber(); i++)
		{
			CString filename, pathname, key;
			filename.Format("\\bk%03d.pim", i);
			pathname = dirpath + _T("\\") + filename;

			if (PathFileExists(pathname)){
				if (API.LoadArchivedImage(i, pathname)){//画像を読み出す
					API.DrawImage(i);
				}
			}
		}
	}
}

/********************************************************************
機  能  名  称 : 画像データをバイナリで保存する
関    数    名 : OnFileSaveData
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリの内容
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileSaveData() 
{
	API.SelectAndSaveImageFile( API.GetCurrentImageNumber() );	
}

/********************************************************************
機  能  名  称 : 画像データをアーカイブ（*.PIM）として保存する
関    数    名 : OnFileSaveImage
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリの内容
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileSaveImage() 
{
	API.SaveArchivedImage( API.GetCurrentImageNumber() );
}

/********************************************************************
機  能  名  称 : 表示画像をビットマップとして保存する
関    数    名 : OnFileSaveView
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileSaveView() 
{
	API.SaveViewAsBitmap( API.GetCurrentImageNumber() );	
}

/********************************************************************
機  能  名  称 : 3D/F3Dデータを複数の画像ファイルとして保存する
関    数    名 : OnFileSaveSerial
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリの内容
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileSaveSerial() 
{
	CIO3DDlg	iodlg;
	CDataUnit	*p_du;
	long		num;

	num = API.GetCurrentImageNumber();//現在選択中の画像データの番号を取得する
	p_du = API.GetDataUnit( CURRENT_IMAGE );//画像データの先頭ポインタを取得する
		if(p_du == NULL)	return;
		if(p_du->PageNumber <= 1)	return;

	iodlg.DoModalStoreType( num );	//IO3Dダイアログを保存タイプで開く
}


/********************************************************************
機  能  名  称 :画像メモリ一括保存
関    数    名 : OnFileSaveAllimage
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリの内容
                 PIMPOM.iniに書き込む
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileSaveAllimage()
{
	CString str;
	CConfirmDlg dlg;
	CString folderName="images";//保存先フォルダ名
	bool	setDefaultImages=true;//次回起動時の設定に反映するかどうか

	dlg.SetTitle("画像メモリ一括保存");
	dlg.RegistVar("保存フォルダ名", &folderName);
	dlg.RegistVar("次回起動時に反映", &setDefaultImages);


	if (dlg.DoModal() == IDOK)
	{
		CString dirpath = API.GetDefaultPath() + folderName;

		if (PathIsDirectory(dirpath)){
			str = dirpath + _T("はすでに存在します。上書きしますか?");
			if (AfxMessageBox(str, MB_OKCANCEL | MB_ICONQUESTION) != IDOK)	return;
		}
		else{
			if (::CreateDirectory(dirpath, NULL) == 0){
				API.MessageBox("%s フォルダ作成失敗", dirpath.GetBuffer());	dirpath.ReleaseBuffer();
				return;
			}
		}


		if (setDefaultImages){
			str.Format("次回起動時には現在と同じ状態が再現されます" );
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;
		}


		//画像メモリを保存
		for(int i=0 ; i<API.GetDataUnitNumber() ; i++)
		{
			CString filename, pathname, key;
			filename.Format("\\bk%03d.pim", i);
			pathname = dirpath + filename;
			key.Format("IMG%d",i);

			if(API.GetDataUnit(i) )
			{
				API.SaveArchivedImage(i, pathname);//画像を保存する

				if (setDefaultImages){
					API.WriteProfile("DEFAULT_IMAGE", key, pathname);//PIMPOM.iniに書き込む
				}
			}else{
				if (setDefaultImages){
					API.WriteProfile("DEFAULT_IMAGE", key, "");
				}
			}
		}

		if (setDefaultImages){
			//選択中の画像番号を保存
			CString strn;
			strn.Format("%d", API.GetCurrentImageNumber());
			API.WriteProfile("SETTING", "DEFAULT_CURRENT_IMAGE", strn);
		}
	}
}




/********************************************************************
機  能  名  称 : 表示画像を印刷する
関    数    名 : OnFilePrint
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFilePrint() 
{
	CPrintDialog	ptdlg(false);
	HDC				hptdc;
	DOCINFO			docinfo;
	CSize			img_size;
	CDataUnit		*pdu;
	DEVMODE			*pdevmode;
	float			dot_per_mm_dev, dot_per_mm, width_in_dc, height_in_dc;
	CConfirmDlg		confirm;


		if( (pdu = API.GetDataUnit(CURRENT_IMAGE))==NULL ){
			return;
		}

	img_size = pdu->DataSize;

	//解像度指定
	dot_per_mm = 10;
	confirm.SetTitle("印刷の解像度");
	confirm.RegistVar("解像度[pix/mm]", &dot_per_mm);
		if(confirm.DoModal() != IDOK){
			return;
		}

	//印刷ダイアログ指定
	int ret = ptdlg.DoModal();
	if(ret == IDOK){
		hptdc = ptdlg.GetPrinterDC();
		pdevmode = ptdlg.GetDevMode();

		dot_per_mm_dev = (float)((pdevmode->dmPrintQuality)/25.4);//デバイスの解像度[pix/mm]
		width_in_dc = dot_per_mm_dev/dot_per_mm*(float)img_size.cx;
		height_in_dc = dot_per_mm_dev/dot_per_mm*(float)img_size.cy;

		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDatatype = NULL;
		docinfo.lpszDocName = NULL;
		docinfo.lpszOutput = NULL;
		docinfo.fwType = 0;
		::StartDoc( hptdc , &docinfo );//プリントジョブ開始
		::StartPage( hptdc );//ページの開始

		::SetStretchBltMode( hptdc, HALFTONE);//中間色で補間する
		if( (pdu->DataType != RGB_FORMAT && pdu->DataType != RGB_3D_FORMAT) ||  pdu->DispRGB == false){//モノクロ

			::StretchDIBits(hptdc, 0, 0, (int)width_in_dc, (int)height_in_dc,
								0, 0, img_size.cx, img_size.cy, 
								pdu->pDispBuffer,
								pdu->pDispBufferBmpInfo,
								DIB_RGB_COLORS, SRCCOPY);

		}else{//カラー
			::StretchDIBits(hptdc, 0, 0, (int)width_in_dc, (int)height_in_dc,
								0, 0, img_size.cx, img_size.cy, 
								pdu->pDispBufferRGB,
								pdu->pDispBufferBmpInfoRGB,
								DIB_RGB_COLORS, SRCCOPY);

		}
		::TextOut(hptdc, 0, (int)height_in_dc , pdu->DataName ,
							pdu->DataName.GetLength() );

		::EndPage( hptdc );//ページの終了
		::EndDoc( hptdc );//プリントジョブ終了
		::DeleteDC( hptdc );
	}	
}

/********************************************************************
機  能  名  称 : アプリケーションの終了
関    数    名 : OnFileQuit
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFileQuit() 
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE);		
}

/********************************************************************
機  能  名  称 : 最近使ったファイルを開く
関    数    名 : OnMenuLoadIOLog
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリにロード
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnMenuLoadIOLog(unsigned int n)
{
	if( API.pDataDlg->DataExistMessage("上書きしてもよろしいですか？", CURRENT_IMAGE) )		return;

	if( API.SelectAndLoadImageFile(CURRENT_IMAGE, API.GetFileIOLog(n-FILE_IO_LOG_MENU_ID)) ){
		API.DrawImage(CURRENT_IMAGE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//編集メニュー
////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
機  能  名  称 :画像メモリの名前変更
関    数    名 :OnEditRename
引          数 : 
戻    り    値 : 
機          能 : 画像メモリの名前変更
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditRename()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(pdu){
		CConfirmDlg dlg;
		dlg.SetWidth(70,200);
		dlg.RegistVar("画像データ名", &pdu->DataName);
		if(dlg.DoModal()==IDOK){
			API.DrawImage(CURRENT_IMAGE, false);
		}
	}
}

/********************************************************************
機  能  名  称 : 画像データのタイプ変換
関    数    名 : OnEditConvertType
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリの内容
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditConvertType() 
{
	CConvertDataDlg  cvt;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	cvt.DoModal();
}

/********************************************************************
機  能  名  称 : バックアップさせていた画像を復元する
関    数    名 : OnEditUndo
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリに復元
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditUndo() 
{
	CString	str;

	str.Format("画像メモリ %d にデータを復元します．よろしいですか？" , API.GetCurrentImageNumber());
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	API.RecoverDataUnit(CURRENT_IMAGE);
}

/********************************************************************
機  能  名  称 : 画像データ削除
関    数    名 : OnEditDelete
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリをクリア
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditDelete() 
{
	CString	str;

	str.Format("画像メモリ %d のデータを削除します．よろしいですか？" , API.GetCurrentImageNumber());
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	API.BackupDataUnit(CURRENT_IMAGE);
	API.DeleteDataUnit(CURRENT_IMAGE);
}

/********************************************************************
機  能  名  称 : 画像データ全削除
関    数    名 : OnEditDeleteAll
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の全画像メモリをクリア
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditDeleteAll()
{
	CString	str;

	str.Format("すべての画像データを削除します．よろしいですか？");
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	for(int i=0 ; i<API.GetDataUnitNumber() ; i++){
		API.DeleteDataUnit(i);
	}
}

/********************************************************************
機  能  名  称 : マーク編集
関    数    名 : OnEditMark
引          数 :
戻    り    値 :
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditMark()
{
	API.SetMarkEditMode(2);
}

/********************************************************************
機  能  名  称 : 作業領域編集
関    数    名 : OnEditWorkarea
引          数 :
戻    り    値 :
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditWorkarea()
{
	API.SetWorkAreaEditMode(true);
}

/********************************************************************
機  能  名  称 : 表示画像をクリップボードへコピーする
関    数    名 : OnEditCopyClipbord
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditCopyClipbord() 
{
	API.CopyToClipbord();
}

/********************************************************************
機  能  名  称 : 画像をクリップボードからペーストする
関    数    名 : OnEditPasteClipbord
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditPasteClipbord()
{
	if( API.pDataDlg->DataExistMessage("上書きしてもよろしいですか？", CURRENT_IMAGE) )		return;

	API.PasteFromClipbord(CURRENT_IMAGE);
}

/********************************************************************
機  能  名  称 : 画像データの直接編集
関    数    名 : OnEditPaint
引          数 : 
戻    り    値 : 
機          能 : 直接編集するためのウィンドウを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditPaint()
{
	if (API.GetDataPickEditType() == 0) {
		API.SetDataPickEditType(1);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
//「表示」メニュー
////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : カラーパレットの変更
関    数    名 : on_change_color_palette
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::on_change_color_palette(short palette_id)
{
	CDataUnit	*p_du;

	if( (p_du = API.GetDataUnit(CURRENT_IMAGE)) == NULL )		return;
	if( (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)  &&  p_du->DispRGB == true)	return;

	p_du->ChangePalette(palette_id);
	API.DrawImage(CURRENT_IMAGE, false);
}

//カラーパレットをグレースケール(ポジ)に変更
void CPIMPOMDlg::OnViewColorGrayscale()
{
	on_change_color_palette(PALETTE_GRAYSCALE);
}

//カラーパレットをグレースケール(ネガ)に変更
void CPIMPOMDlg::OnViewColorNega() 
{
	on_change_color_palette(PALETTE_NEGATIVE);
}

//カラーパレットを虹色に変更
void CPIMPOMDlg::OnViewColorRainbow()
{
	on_change_color_palette(PALETTE_RAINBOW);
}

//カラーパレットを虹色(0のところは黒)に変更
void CPIMPOMDlg::OnViewColorRainbow2() 
{
	on_change_color_palette(PALETTE_BLACK_RAINBOW);
}

//カラーパレットをコサインに変更
void CPIMPOMDlg::OnViewColorCosine() 
{
	on_change_color_palette(PALETTE_COSINE);
}

//カラーパレットを黄色⇒青に変更
void CPIMPOMDlg::OnViewColorYtob() 
{
	on_change_color_palette(PALETTE_BLUE_YELLOW);
}

//カラーパレットを緑⇒赤に変更
void CPIMPOMDlg::OnViewColorGtor() 
{
	on_change_color_palette(PALETTE_GREEN_RED);
}

void CPIMPOMDlg::OnViewColorLabel()
{
	on_change_color_palette(PALETTE_LABEL);
}


/********************************************************************
機  能  名  称 : 環境設定
関    数    名 : OnViewSetupEnvironment
引          数 :
戻    り    値 :
機          能 : 
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnViewSetupEnvironment()
{
	int data_number, data_window_width, thumbneil_colum;
	int  main_image_w, main_image_h, small_image_w, small_image_h;
	CConfirmDlg dlg;
	

	//iniファイルから読み込み
	data_number = API.GetDataUnitNumber();
	main_image_w = API.main_image_size.cx;
	main_image_h = API.main_image_size.cy;
	data_window_width = API.data_window_width;
	thumbneil_colum = API.thumbneil_colum;
	small_image_w = API.small_image_size.cx;
	small_image_h = API.small_image_size.cy;

	dlg.SetTitle("環境設定");
	dlg.RegistVar("画像メモリ数", &data_number);
	dlg.RegistVar("メイン画像幅", &main_image_w);
	dlg.RegistVar("メイン画像高さ", &main_image_h);
	dlg.RegistVar("データウィンドウ幅", &data_window_width);
	dlg.RegistVar("サムネイル行数", &thumbneil_colum);
	dlg.RegistVar("サムネイル画像幅", &small_image_w);
	dlg.RegistVar("サムネイル画像高さ", &small_image_h);

	if (dlg.DoModal() == IDOK)
	{	//iniファイルに値を書き込む
		CString str;

		str.Format("%d", data_number);
		API.WriteProfile("SETTING", "IMG_NUM", str);

		str.Format("%d", main_image_w);
		API.WriteProfile("SETTING", "MAIN_IMG_W", str);

		str.Format("%d", main_image_h);
		API.WriteProfile("SETTING", "MAIN_IMG_H", str);

		str.Format("%d", data_window_width);
		API.WriteProfile("SETTING", "DATA_WIN_W", str);

		str.Format("%d", small_image_w);
		API.WriteProfile("SETTING", "SMALL_IMG_W", str);

		str.Format("%d", small_image_h);
		API.WriteProfile("SETTING", "SMALL_IMG_H", str);

		API.MessageBox("設定変更を有効にするには、PIMPOMを再起動してください");
	}
}

/********************************************************************
機  能  名  称 : 直接描画用フォント作成
関    数    名 : OnViewMakefont
引          数 : 
戻    り    値 : 
機          能 : 画像データに直接文字描画するためのビットマップフォント情報を生成する
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnViewMakefont()
{

	int		font_size = API.default_font.lfHeight;
	CConfirmDlg	box;

	box.RegistVar("フォントサイズ",&font_size);
	box.SetWidth(80,30);
	if( box.DoModal() != IDOK )	return;

	if (box.DoModal() == IDOK)
	{	//iniファイルに値を書き込む
		CString str;

		str.Format("%d", font_size);
		API.WriteProfile("SETTING", "FONT_SIZE", str);

		API.default_font.lfHeight = font_size;
	}

}


/********************************************************************
機  能  名  称 : マーク表示
関    数    名 : OnViewMark
引          数 :
戻    り    値 :
機          能 : 
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnViewMark()
{
	if (API.GetMarkEditMode() == 0) {
		API.SetMarkEditMode(1);
	}
}

/********************************************************************
機  能  名  称 : 作業領域表示
関    数    名 : OnViewWorkarea
引          数 :
戻    り    値 :
機          能 :
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnViewWorkarea()
{
	API.SetWorkAreaEditMode(true);
}

/********************************************************************
機  能  名  称 : 複数画像の同時表示
関    数    名 : OnViewMultiImageDisp
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnViewMultiImageDisp()
{
	API.DialogOpen(IDD_MULTI_IMAGE_DISP_DLG, true);
}

/********************************************************************
機  能  名  称 : 3次元画像一覧表示
関    数    名 : OnView3dImgDisp
引          数 :
戻    り    値 :
機          能 :
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnView3dImgDisp()
{
	API.DialogOpen(IDD_3D_IMAGE_DISP_DLG, true);
}


////////////////////////////////////////////////////////////////////////////////////////////
//「画像処理」メニュー
////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : 画像データのサイズ変換
関    数    名 : OnEditResize
引          数 : 
戻    り    値 : 
機          能 : 現在選択中の画像メモリの内容
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnEditResize() 
{
	CResizeDataDlg  res;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	res.DoModal();
}

/********************************************************************
機  能  名  称 : 汎用フィルタ
関    数    名 : OnFilter
引          数 : 
戻    り    値 : 
機          能 : フィルタ設定するためのウィンドウを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnFilter() 
{
	API.DialogOpen(IDD_FILTER_CTRL_DLG,true);

}

/********************************************************************
機  能  名  称 : 任意の数式による画像生成
関    数    名 : OnCalcImage
引          数 : 
戻    り    値 : 
機          能 : 数式編集するためのウィンドウを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnCalcImage() 
{
	API.SetCalcToolMode(true);
}

/********************************************************************
機  能  名  称 : 画像データの幾何変換
関    数    名 : OnImageTransform
引          数 : 
戻    り    値 : 
機          能 : 幾何変換するためのウィンドウを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnImageTransform()
{
	CTransformDlg  trn;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	trn.DoModal();
}

/********************************************************************
機  能  名  称 : 画像データの2値化
関    数    名 : OnBinalize
引          数 : 
戻    り    値 : 
機          能 : 2値化するためのウィンドウを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnBinalize()
{
	if(!API.GetDataUnit(CURRENT_IMAGE))	return;

	API.DialogOpen(IDD_BINALIZE_DLG,true);
}


////////////////////////////////////////////////////////////////////////////////////////////
//「ツール」メニュー
////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
機  能  名  称 : ビデオキャプチャ起動
関    数    名 : OnToolVideocapture
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnToolVideocapture()
{
	if( API.CaptureInit(CSize(-1,-1)) ){
		CButton	*p_chk_video = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE);
		p_chk_video->ShowWindow(SW_SHOW);

		CButton	*p_chk_video2 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2);
		p_chk_video2->ShowWindow(SW_SHOW);
	}
}

/********************************************************************
機  能  名  称 : TCP/IP通信設定
関    数    名 : OnToolSocket
引          数 : 
戻    り    値 : 
機          能 : ソケット通信設定するためのウィンドウを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnToolSocket()
{
	API.DialogOpen(IDD_SOCKET_CTRL_DLG,true);
}

/********************************************************************
機  能  名  称 : バッチ実行
関    数    名 : OnToolBatch
引          数 : 
戻    り    値 : 
機          能 : バッチ実行のためのウィンドウを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnToolBatch()
{
	API.DialogOpen(IDD_BATCH_DLG,true);
}


/********************************************************************
機  能  名  称 : 2点間の距離
関    数    名 : OnToolRuler
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnToolRuler()
{
	API.SetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, true);
}


/********************************************************************
機  能  名  称 : ヒストグラム
関    数    名 : OnToolHistgram
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnToolHistgram()
{
	API.DialogOpen(IDD_HISTGRAM_DLG, true);
	API.DrawImage();
}

/********************************************************************
機  能  名  称 : コマンドプロンプト
関    数    名 : OnToolCommandPrompt
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnToolCommandPrompt()
{
	API.DialogOpen(IDD_COMMAND_RPOMPT_DLG,true);
}


/********************************************************************
機  能  名  称 : Pythonバッチ実行設定
関    数    名 : OnToolPythonBatch
引          数 :
戻    り    値 :
機          能 :
日付         作成者          内容
------------ --------------- ---------------------------------------
Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnToolPythonBatch()
{
	API.DialogOpen(IDD_PYTHON_BATCH_DLG, true);
}

////////////////////////////////////////////////////////////////////////////////////////////
//「ヘルプ」メニュー
////////////////////////////////////////////////////////////////////////////////////////////


void CPIMPOMDlg::OnHelpSpecificationSheet()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOMソフトウェア仕様書.pptx", NULL, NULL, SW_SHOWNORMAL);
}

void CPIMPOMDlg::OnHelpApireference()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM_APIリファレンス.docx", NULL, NULL, SW_SHOWNORMAL);
}

void CPIMPOMDlg::OnHelpApireference2()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM_外部プログラム画像データ受け渡しマニュアル.doc", NULL, NULL, SW_SHOWNORMAL);
}

void CPIMPOMDlg::OnHelpApireference3()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM_PIMPOMPYマニュアル.doc", NULL, NULL, SW_SHOWNORMAL);
}

////////////////////////////////////////////////////////////////////////////////////////////
//「カスタムファンクション」メニュー
////////////////////////////////////////////////////////////////////////////////////////////

void CPIMPOMDlg::OnCustomAdd()
{
	ShellExecute(m_hWnd, "open",API.GetDefaultPath() + "CloneUserDlg.exe",NULL,NULL,SW_SHOWNORMAL);
}


/********************************************************************
機  能  名  称 : カスタムファンクション用のウィンドウを出す
関    数    名 : OnMenuCustomFunc
引          数 : 
戻    り    値 : 
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
void CPIMPOMDlg::OnMenuCustomFunc(unsigned int n)
{
	API.UserDialogOpen(n-CUSTOM_MENU_ID);
}


