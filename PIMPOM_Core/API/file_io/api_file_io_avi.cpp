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
//ファイルIO(AVI)
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include <vfw.h>

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
機  能  名  称 :  AVIファイルを読み込む
関    数    名 : LoadAvi
引          数 : long		image_number(in)データ格納先の画像メモリ番号(-1にした場合は現在選択中の画像)
				CString		filename	(in)ファイルのフルパス
戻    り    値 : 成功すれば true
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         作成途中でうまくうごかない
********************************************************************/
bool	CPimpomAPI::LoadAvi(long image_number, CString	filename)
{
	bool	ret=true;
	PAVIFILE aviFile = NULL;	//AVI ファイルオブジェクト
	PAVISTREAM aviStream = NULL;	//AVI ファイルのストリームオブジェクト
	PGETFRAME aviFrame = NULL;		//AVI ファイルからフレームを解凍するオブジェクト
	int		streamLen;
	BITMAPINFOHEADER *pBmpInfoHeader=NULL;
	CSize bmSize;//ビットマップサイズ
	long bmpWidthGap = 0;//DIBデータ幅とRAWデータ幅の差


    if (AVIFileOpen(&aviFile , filename , OF_READ , NULL)) {//AVIFile を開く
        ret = false;
		goto LoadAvi_Deinit;
	}

    if (AVIFileGetStream(aviFile , &aviStream , streamtypeVIDEO , 0)) {//AVI ストリームを取得する
        ret = false;
		goto LoadAvi_Deinit;
    }

    aviFrame = AVIStreamGetFrameOpen(aviStream , NULL);//フレームを解凍するためのオブジェクトを取得
    if (aviFrame == NULL) {
        ret = false;
		goto LoadAvi_Deinit;
    }

	streamLen = AVIStreamLength(aviStream);//
	if(streamLen<=0){
        ret = false;
		goto LoadAvi_Deinit;
	}

	pBmpInfoHeader = (BITMAPINFOHEADER *)AVIStreamGetFrame(aviFrame,0);//ビットマップの形式確認
	if(pBmpInfoHeader==NULL){
        ret = false;
		goto LoadAvi_Deinit;
	}
	bmSize = CSize(pBmpInfoHeader->biWidth, pBmpInfoHeader->biHeight);
	


	//データ読み出し
	if(pBmpInfoHeader->biBitCount==8)//8bit
	{
		BYTE *ptr = Get3DMemory(image_number, bmSize, streamLen, true);
		if(ptr==NULL){
	        ret = false;
			goto LoadAvi_Deinit;
		}

		//DIBデータ幅とRAWデータ幅の差
		if( bmSize.cx % 4 == 0)	bmpWidthGap = 0;	
		else			bmpWidthGap = 4 - bmSize.cx % 4;

		//カラーパレットの読み込み
		CDataUnit *pdu = GetDataUnit(image_number);
		memcpy( pdu->pDispBufferBmpInfo->bmiColors, pBmpInfoHeader + sizeof(BITMAPINFOHEADER), 1024);


		//データの読み込み
		for(int n=0 ; n<streamLen ; n++)
		{
			pBmpInfoHeader = (BITMAPINFOHEADER *)AVIStreamGetFrame(aviFrame,n);
			if(pBmpInfoHeader==NULL){
				ret = false;
				goto LoadAvi_Deinit;
			}

			//データ読み込み
			for(int j = bmSize.cy-1  ;  j>=0  ;	j-- ){//上下反転
				memcpy( ptr + bmSize.cx*(bmSize.cy-1-j) , 
						pBmpInfoHeader + sizeof(BITMAPINFOHEADER) + 1024 + (bmSize.cx + bmpWidthGap)*j , 
						bmSize.cx  );
			}
		}
	}
	else if(pBmpInfoHeader->biBitCount==24)//RGB
	{
		BYTE *ptr = GetRGB3DMemory(image_number, bmSize, streamLen, true);
		if(ptr==NULL){
	        ret = false;
			goto LoadAvi_Deinit;
		}

		//DIBデータ幅とRAWデータ幅の差
		if( (bmSize.cx*3) % 4 == 0)	bmpWidthGap = 0;	
		else				bmpWidthGap = 4 - (bmSize.cx*3) % 4;

		//データの一時保管場所の作成
		BYTE *ptmp_rgb = (BYTE*)malloc( (bmSize.cx*3)*bmSize.cy );
		if(!ptmp_rgb){
			ret = false;
			goto LoadAvi_Deinit;
		}


		//データの読み込み
		for(int n=0 ; n<streamLen ; n++)
		{
			pBmpInfoHeader = (BITMAPINFOHEADER *)AVIStreamGetFrame(aviFrame,n);
			if(pBmpInfoHeader==NULL){
				free(ptmp_rgb);
				ret = false;
				goto LoadAvi_Deinit;
			}

			//データ読み込み
			for(int j = bmSize.cy-1  ;  j>=0  ;	j-- ){//上下反転
				memcpy( ptmp_rgb + (bmSize.cx*3)*(bmSize.cy-1-j) , 
						pBmpInfoHeader + sizeof(BITMAPINFOHEADER) + 1024 + (bmSize.cx*3 + bmpWidthGap)*j , 
						bmSize.cx*3  );
			}

			//データの変換
			for(int j=0 ; j<bmSize.cy ; j++){
				for(int i=0 ; i<bmSize.cx ; i++){
					*(ptr + i + j*bmSize.cx )					   = *(ptmp_rgb + i*3 + j*(bmSize.cx*3)+2);//r
					*(ptr + i + j*bmSize.cx + bmSize.cx*bmSize.cy)   = *(ptmp_rgb + i*3 + j*(bmSize.cx*3)+1);//g
					*(ptr + i + j*bmSize.cx + bmSize.cx*bmSize.cy*2) = *(ptmp_rgb + i*3 + j*(bmSize.cx*3)+0);//b
				}
			}
			
		}

		free(ptmp_rgb);//データの一時保管場所の開放

	}






LoadAvi_Deinit:
    if (aviFrame) AVIStreamGetFrameClose(aviFrame);
    if (aviStream) AVIStreamRelease(aviStream);
    if (aviFile) AVIFileRelease(aviFile);

	return ret;
}