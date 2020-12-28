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
//Cannyﾌｨﾙﾀ
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"

#include "opencv_headers.h"
#include "PIMPOM_API.h"



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
機  能  名  称 : 直線検出
関    数    名 : HoughLines
引          数 : long		image_num			(in)対象エッジ画像メモリ番号(-1にした場合は現在選択中の画像)
				 int		pixSkip				(in)投票空間の距離分解能[pix]
				 int		dirSkip				(in)投票空間の角度分解能[deg]
				 int		voteThresh			(in)投票の閾値パラメータ．十分な票（  >\texttt{threshold} ）を得た直線のみが出力されます．
				 double		minLineLength		(in)最小の線分長．これより短い線分は棄却されます
				 double		maxLineGap			(in)2点が同一線分上にあると見なす場合に許容される最大距離．
戻    り    値 : 抽出した直線数
機          能 : Cannyフィルタなどで抽出したエッジ画像に対し、確率的Hough変換を行う
				　
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
int  CPimpomAPI::HoughLines(long image_num, int pixSkip, int dirSkip, int voteThresh, double minLineLength, double maxLineGap )
{
	CDataUnit	*p_du;


	//引数チェック

	if(image_num == CURRENT_IMAGE){
		image_num = GetCurrentImageNumber();
	}else if(image_num >= GetDataUnitNumber() || image_num<0){
		return 0;
	}


	p_du = GetDataUnit(image_num);
		if(!p_du)	return	0;

	CRect calc_area = p_du->GetRectArea();//処理対象の矩形範囲

		if(p_du->DispRGB == false || (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) )//グレー画像に対する処理
		{
			if(p_du->pByteData){//8bit画像
				
				cv::Mat srcMat, subSrcMat;
				
				if( !ConvertToCvImage2(image_num, (void*)&srcMat, (void*)&subSrcMat) )//画像メモリ⇒cv::Mat
				{
					return false;
				}
			
				std::vector<Vec4i> lines;
				cv::HoughLinesP( subSrcMat, lines, pixSkip, CV_PI/180.0*(double)dirSkip, voteThresh, minLineLength, maxLineGap );//確率的Hough変換実行

				//直線検出結果をマークに反映


				p_du->ClearMark();
				for( size_t i = 0; i < lines.size() && i< MARK_NUMBER_MAX; i++ )
				{ 
					CMark mark( (float)lines[i][0] + calc_area.left, 
								(float)lines[i][1] + calc_area.top, 
								(float)lines[i][2] + calc_area.left, 
								(float)lines[i][3] + calc_area.top);
					p_du->AddMark(mark);
				}
				
				SetMarkEditMode(1);

				return (int)lines.size();
				
			}else if(p_du->pShortData){//16bit画像

				MessageBox("this function is supported only for BYTE format");
				return 0;

			}else if(p_du->pWordData){//16bit画像
				MessageBox("this function is supported only for BYTE format");
				return 0;
			}else if(p_du->pLongData){//32bit画像
				MessageBox("this function is supported only for BYTE format");
				return 0;
			}else if(p_du->pDwordData){//32bit画像
				MessageBox("this function is supported only for BYTE format");
				return 0;
			}else if(p_du->pFloatData){//実数画像
				MessageBox("this function is supported only for BYTE format");
				return 0;
			}
		}
		else//RGB画像に対する処理
		{
			MessageBox("this function is supported only for BYTE format");
			return 0;
	
		}
		return 0;
}


