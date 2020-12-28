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
//ファイルIO(CDataUnitのアーカイブ)
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "atlimage.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************

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

// *************************************
//         関    数    参    照         
// *************************************

/********************************************************************
機  能  名  称 :  画像メモリのアーカイブデータをファイルから読み出す
関    数    名 : LoadArchivedImage
引          数 : long		image_number(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		pathname	(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
 2016.0512   Y.Ikeda         マークの扱い方を変更
********************************************************************/
bool	CPimpomAPI::LoadArchivedImage(long image_number , CString pathname)
{
	CFile		file;
	CDataUnit	*p_du;
	CString		str;
	int	n;

		if(image_number == CURRENT_IMAGE)		image_number =GetCurrentImageNumber();//現在選択中の画像を指定する場合

		if(!file.Open(pathname ,  CFile::modeRead | CFile::typeBinary ) ){//ファイルを開く
			str = pathname + _T(" が開けません");
			AfxMessageBox(str);
			return	false;
		}




//////ヘッダを読み込む
	char	buf[ sizeof(DATA_UNIT_INFO) ];
	DATA_UNIT_INFO	*header;

	file.Read( buf , sizeof(DATA_UNIT_INFO));
	header = (DATA_UNIT_INFO*)buf;

	


///////メモリ取得
	CSize	size( header->DataSizeX , header->DataSizeY );

		switch( header->DataType ){
			case BYTE_FORMAT://BYTE
				p_du = FormatDataUnit( image_number , BYTE_FORMAT, size, 1, true);
				break;

			case THREE_D_FORMAT://3D
				p_du = FormatDataUnit( image_number , THREE_D_FORMAT, size, header->PageNumber, true);
				break;

			case FLOAT_FORMAT://float
				p_du = FormatDataUnit( image_number , FLOAT_FORMAT, size, 1, true);
				break;

			case COMPLEX_FORMAT://complex
				p_du = FormatDataUnit( image_number , COMPLEX_FORMAT, size, 1, true);
				break;

			case XY_FORMAT://XY
				p_du = FormatDataUnit( image_number , XY_FORMAT, size, 1, true);
				break;

			case XYZ_FORMAT://XYZ
				p_du = FormatDataUnit( image_number , XYZ_FORMAT, size, 1, true);
				break;

			case FLOAT_3D_FORMAT://F3D
				p_du = FormatDataUnit( image_number , FLOAT_3D_FORMAT, size, header->PageNumber, true);
				break;

			case RGB_FORMAT://RGB
				p_du = FormatDataUnit( image_number , RGB_FORMAT, size, 1, true);
				break;

			case SHORT16_FORMAT://Short
				p_du = FormatDataUnit( image_number , SHORT16_FORMAT, size, 1, true);
				break;

			case WORD_FORMAT://Word
				p_du = FormatDataUnit( image_number , WORD_FORMAT, size, 1, true);
				break;

			case LONG32_FORMAT://Long
				p_du = FormatDataUnit( image_number , LONG32_FORMAT, size, 1, true);
				break;

			case DWORD_FORMAT://DWord
				p_du = FormatDataUnit( image_number , DWORD_FORMAT, size, 1, true);
				break;

			case RGB_3D_FORMAT://RGB3D
				p_du = FormatDataUnit( image_number , RGB_3D_FORMAT, size, header->PageNumber, true);
				break;

			default:
				//AfxMessageBox("サポートされていないデータタイプです");
				str.Format("%d", header->DataType );
				AfxMessageBox(str);
				file.Close();
				return	false;
		}

		if(p_du == NULL){
			file.Close();
			return	false;//メモリ取得に失敗したとき
		}
	

///////表示データを読み込む
	if(header->DataType != RGB_FORMAT && header->DataType != RGB_3D_FORMAT){//グレー
		file.Read( p_du->pDispBuffer , p_du->DispBufferRow * p_du->DataSize.cy );
		//file.Read( p_du->pDispBufferBmpInfo->bmiColors , 1024 );
	}else{//RGB
		file.Read( p_du->pDispBufferRGB , p_du->DispBufferRowRGB * p_du->DataSize.cy );
	}

///////マスクデータを読み込む
	file.Read( p_du->pMaskData , p_du->DataSize.cx * p_du->DataSize.cy );

////////実データ部分を読み込む
	switch( header->DataType ){
		case BYTE_FORMAT:
			file.Read( p_du->pByteData , p_du->DataSize.cx * p_du->DataSize.cy );
			break;

		case THREE_D_FORMAT://3D
			{
				for(n=0 ; n<p_du->PageNumber ; n++){
					file.Read( p_du->pByteData + p_du->DataSize.cx * p_du->DataSize.cy * n ,  p_du->DataSize.cx * p_du->DataSize.cy );
					str.Format("%d/%d枚読み込み", n+1, p_du->PageNumber);//今まで何枚読み込んだかをメッセ－ジとして表示
				}
				p_du->DispPage = header->DispPage;
				p_du->DispChannel = header->DispChannel;
			}
			break;

		case FLOAT_FORMAT://float
			file.Read( p_du->pFloatData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) );
			break;
		case COMPLEX_FORMAT://complex
			file.Read( p_du->pFloatData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) * 2 );
			p_du->CreateComplexPolarComponents();//パワー、位相を求める
			break;

		case XY_FORMAT://XY
			file.Read( p_du->pFloatData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) * 2 );
			break;

		case XYZ_FORMAT://XYZ
			file.Read( p_du->pFloatData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) * 3 );
			break;

		case FLOAT_3D_FORMAT://F3D
			{
				for(n=0 ; n<p_du->PageNumber ; n++){
					float	*addr = p_du->pFloatData + p_du->DataSize.cx * p_du->DataSize.cy * n;
					file.Read( addr , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) );
					str.Format("%d/%d枚読み込み", n+1, p_du->PageNumber);//今まで何枚読み込んだかをメッセ－ジとして表示
				}
				p_du->DispPage = header->DispPage;
				p_du->DispChannel = header->DispChannel;
			}
			break;

		case RGB_FORMAT://RGB
			file.Read( p_du->pByteData ,  p_du->DataSize.cx * p_du->DataSize.cy * 3);
			p_du->DispRGB = header->DispRGB;
			break;

		case SHORT16_FORMAT://Short
			file.Read( p_du->pShortData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(short) );
			break;

		case WORD_FORMAT://Word
			file.Read( p_du->pWordData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(WORD) );
			break;

		case LONG32_FORMAT://Long
			file.Read( p_du->pLongData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(long) );
			break;

		case DWORD_FORMAT://Dword
			file.Read( p_du->pDwordData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(DWORD) );
			break;

		case RGB_3D_FORMAT://RGB3D
			{
				for(n=0 ; n<p_du->PageNumber ; n++){
					for(int c=0 ; c<p_du->ChannelNumber ; c++){
						int offset = n*p_du->ChannelNumber + c;
						file.Read( p_du->pByteData + p_du->DataSize.cx * p_du->DataSize.cy * offset ,  p_du->DataSize.cx * p_du->DataSize.cy );
					}
					str.Format("%d/%d読み込み", n+1, p_du->PageNumber);//今まで何枚読み込んだかをメッセ－ジとして表示
				}
				p_du->DispPage = header->DispPage;
				p_du->DispChannel = header->DispChannel;
			}
			break;
	}


	//マークを読み込む
	if(header->MarkNum > MARK_NUMBER_MAX_OLD )//新しいフォーマット：マークは可変長データが画像のあとに入っている
	{
		long markNum;
		file.Read( &markNum, sizeof(long));
		for(int i=0 ; i<markNum ; i++)
		{
			CMark mark;
			file.Read( &mark.type, sizeof(short));
			file.Read( &mark.figure, sizeof(FIGURE_UNION));
			p_du->AddMark(mark);
		}
	}
	else//古いフォーマット：マークは点のみの固定長データがヘッダに入っている
	{
		for(int i=0 ; i<header->MarkNum ; i++){
			p_du->AddMark( CMark(header->MarkX[i], header->MarkY[i]));
		}
	}


	////表示設定
	p_du->DataName = CString(header->DataName);

	p_du->BrightMax = header->BrightMax;
	p_du->BrightMin = header->BrightMin;

	p_du->DispScale = header->DispScale;
	p_du->DataDispOffset = CPoint(header->DataDispOffsetX, header->DataDispOffsetY);

	memcpy(p_du->WorkArea.Type, header->WorkAreaType, sizeof(int)*WORKAREA_FIG_NUM);
	memcpy(p_du->WorkArea.OrNot, header->WorkAreaOrNot, sizeof(bool)*WORKAREA_FIG_NUM);
	memcpy(p_du->WorkArea.Figure, header->WorkAreaFig, sizeof(FIGURE_UNION)*WORKAREA_FIG_NUM);

	p_du->DoUseWorkArea = header->DoUseWorkArea;

	p_du->DoUseMask = header->DoUseMask;



	//読み込んだファイルのパスを残す
	p_du->SrcFilePath = pathname;

	file.Close();

	
	SetFileIOLog(pathname);//ファイル名のログを残す


	return		true;
}


/********************************************************************
機  能  名  称 :  画像メモリのアーカイブデータをファイルに保存する
関    数    名 : SaveArchivedImage
引          数 : long		image_number(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
戻    り    値 : 成功すれば true
機          能 : 保存するファイルを選ぶためのファイルダイアログを出す
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool	CPimpomAPI::SaveArchivedImage(long image_number )
{
	CDataUnit	*p_du;
	CString filename, filestr;
	CFile  file;


		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	p_du = GetDataUnit( image_number );//していされた画像メモリを取得する
		if( p_du == NULL )		return	false;//指定された画像メモリにデータが無ければなにもしない


	//ファイルダイアログ
	filestr.LoadString(IDS_FILETYPE_ARCHIVED);
	CFileDialog myDLG(false , _T("pim") , NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filestr);

		if(myDLG.DoModal() != IDOK)  return false;
	filename = myDLG.GetPathName();

	return	SaveArchivedImage( image_number , filename);
}


/********************************************************************
機  能  名  称 :  画像メモリのアーカイブデータをファイルに保存する
関    数    名 : SaveArchivedImage
引          数 : long		image_number(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		filename	(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
 2016.0512   Y.Ikeda         マークの扱い方を変更
********************************************************************/
bool	CPimpomAPI::SaveArchivedImage(long image_number , CString filename)
{
	CFile		file;
	CDataUnit	*p_du;
	CString		str;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//現在選択中の画像を指定する場合

	p_du = GetDataUnit( image_number );//していされた画像メモリを取得する
		if( p_du == NULL )		return	false;//指定された画像メモリにデータが無ければなにもしない

		if( ! file.Open(filename , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //ファイルを開く

//ヘッダの保存
	DATA_UNIT_INFO	header;
	
	header.DataType = p_du->DataType;
	sprintf(header.DataName, "%s", p_du->DataName.GetBuffer());	p_du->DataName.ReleaseBuffer();
	header.DataSizeX = p_du->DataSize.cx;
	header.DataSizeY = p_du->DataSize.cy;
	header.PageNumber = p_du->PageNumber;
	header.ChannelNumber = p_du->ChannelNumber;
	header.DispPage = p_du->DispPage;
	header.DispChannel = p_du->DispChannel;
	header.DispRGB = p_du->DispRGB;
	header.MarkNum = MARK_NUMBER_MAX;//古いマーク入力領域には、大きい値を入れることで新フォーマットと区別する
	//for(int i=0 ; i< p_du->GetMarkNumber() ; i++)
	//{
	//	CMark mark;
	//	p_du->GetMark(i,&mark);
	//	header.MarkX[i] = mark.figure.point.x;
	//	header.MarkY[i] = mark.figure.point.y;
	//}
	header.BrightMax = p_du->BrightMax;
	header.BrightMin = p_du->BrightMin;
	header.DispScale = p_du->DispScale;
	header.DataDispOffsetX = p_du->DataDispOffset.x;
	header.DataDispOffsetY = p_du->DataDispOffset.y;
	header.DoUseWorkArea = p_du->DoUseWorkArea;


	memcpy(header.WorkAreaType, p_du->WorkArea.Type, sizeof(int)*WORKAREA_FIG_NUM);
	memcpy(header.WorkAreaOrNot, p_du->WorkArea.OrNot, sizeof(bool)*WORKAREA_FIG_NUM);
	memcpy(header.WorkAreaFig, p_du->WorkArea.Figure, sizeof(FIGURE_UNION)*WORKAREA_FIG_NUM);

	file.Write( &header , sizeof(DATA_UNIT_INFO));//ヘッダの保存

////表示画像の保存
	if(p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT){//グレー
		file.Write( p_du->pDispBuffer , p_du->DispBufferRow * p_du->DataSize.cy );
		//file.Write( p_du->pDispBufferBmpInfo->bmiColors, 1024 );
	}else{//RGB
		file.Write( p_du->pDispBufferRGB , p_du->DispBufferRowRGB * p_du->DataSize.cy );
	}

////マスク画像の保存
	file.Write( p_du->pMaskData , p_du->DataSize.cx * p_du->DataSize.cy );
	header.DoUseMask = p_du->DoUseMask;


/////実データの保存
	switch( p_du->DataType ){
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			{
				for(int n=0 ; n<p_du->PageNumber ; n++){
					BYTE	*addr = p_du->pByteData + p_du->DataSize.cx * p_du->DataSize.cy * n;
					file.Write( addr , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(BYTE) );
					str.Format("%d/%d枚読み込み", n+1, p_du->PageNumber);//今まで何枚保存したかをメッセージとして表示する
					ShowMessage( true, 0, false, str );
				}
			}
			break;

		case FLOAT_FORMAT:
			//not break
		case COMPLEX_FORMAT:
			//not break
		case XY_FORMAT:
			//not break
		case XYZ_FORMAT:
			//not break
		case FLOAT_3D_FORMAT:
			{
				for(int n=0 ; n<p_du->PageNumber ; n++){
					float	*addr = p_du->pFloatData + p_du->DataSize.cx * p_du->DataSize.cy * n;
					file.Write( addr , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) );
					str.Format("%d/%d枚読み込み", n+1, p_du->PageNumber);//今まで何枚保存したかをメッセージとして表示する
					ShowMessage( true, 0, false, str );
				}
			}
			break;

		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			{
				for(int n=0 ; n<p_du->PageNumber ; n++){
					BYTE	*addr = p_du->pByteData + p_du->DataSize.cx * p_du->DataSize.cy * p_du->ChannelNumber * n;
					file.Write( addr , p_du->DataSize.cx * p_du->DataSize.cy * p_du->ChannelNumber * sizeof(BYTE) );
					str.Format("%d/%d枚読み込み", n+1, p_du->PageNumber);//今まで何枚保存したかをメッセージとして表示する
					ShowMessage( true, 0, false, str );
				}
			}
			break;

		case SHORT16_FORMAT:
			file.Write( p_du->pShortData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(short) );
			break;

		case WORD_FORMAT:
			file.Write( p_du->pWordData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(WORD) );
			break;

		case LONG32_FORMAT:
			file.Write( p_du->pLongData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(long) );
			break;

		case DWORD_FORMAT:
			file.Write( p_du->pDwordData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(DWORD) );
			break;
	}
	
	//マークの保存:可変長データを画像のあとに入れる
	long markNum = p_du->GetMarkNumber();
	file.Write( &markNum, sizeof(long) );
	
	for(int i=0 ; i<markNum ; i++){
		CMark mark;
		p_du->GetMark(i,&mark);

		file.Write( &mark.type, sizeof(short));
		file.Write( &mark.figure, sizeof(FIGURE_UNION));
	}


	file.Close();

	SetFileIOLog(filename);//ファイル名のログを残す

	return	true;
}


