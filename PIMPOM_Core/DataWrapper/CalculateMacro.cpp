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
//CCalculate
//演算式のマクロ部分の実装
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "Shlwapi.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************
//マクロコマンドのID

//計算式
#define		CALC_IMG_DAT	0	
#define		CALC_SIN		1
#define		CALC_COS		2
#define		CALC_TAN		3
#define		CALC_LOG		4
#define		CALC_SQRT		5
#define		CALC_EXP		6
#define		CALC_ABS		7
#define		CALC_RANDOM		8
#define		CALC_RAND		9
#define		CALC_CEIL		10
#define		CALC_FLOOR		11	
#define		CALC_ASIN		12
#define		CALC_ACOS		13
#define		CALC_POW		14
#define		CALC_MOD		15
#define		CALC_ATAN2		16
#define		CALC_ATAN		17
#define		CALC_GAUS		18
#define		CALC_THRESH		19
#define		CALC_DELTA		20
#define		CALC_IMG_DAT2	21
#define		CALC_MASK_DAT	22
#define		CALC_MCR_MIN	23
#define		CALC_MCR_MAX	24
#define		CALC_MCR_PI		25
#define		CALC_MCR_DIST	26
#define		CALC_MCR_SIGMOID	27
#define		CALC_MCR_RELU	28



//API　画像データアクセス
#define		CALC_GetDataUnitNumber			100
#define		CALC_GetCurrentImageNumber		101
#define		CALC_SetCurrentImageNumber		102
#define		CALC_GetDataTypeSize			103
#define		CALC_CopyDataUnit				104
#define		CALC_DeleteDataUnit				105
#define		CALC_BackupDataUnit				106
#define		CALC_RecoverDataUnit			107
#define		CALC_ConvertDataUnit			108
#define		CALC_ConvertDispToByte				109
#define		CALC_GetSrcFilePathName			110
#define		CALC_GetSrcFilePath				111
#define		CALC_GetSrcFileName				112
#define		CALC_GetSrcFileTitle			113
#define		CALC_FormatDataUnit				114
#define     CALC_GetDataMinMax				115
#define		CALC_SetDispPage				116
#define		CALC_SetDispChannel				117
#define		CALC_UseWorkArea				118

#define		CALC_DrawImage					120
#define		CALC_ScaleImage					121
#define		CALC_SetDispRange				122
#define		CALC_SetDispRangeAuto			123
#define		CALC_SetWorkArea				124
#define		CALC_GetWorkAreaVtx				125
#define		CALC_GetWorkAreaOrnot			126
#define		CALC_ChangePalette				127

#define		CALC_AddMark					130
#define		CALC_DeleteMark					131
#define		CALC_GetMarkVal					132
#define		CALC_GetMarkNumber				133
#define		CALC_ClearMark					134
#define		CALC_AddLineMark				135

//API 画像処理
#define		CALC_ResizeDataUnit				150
#define		CALC_ClipDataUnit				151
#define		CALC_PerspectiveTransDataUnit	152
#define		CALC_IsoTransDataUnit			153
#define		CALC_CalcDataValue				154
#define		CALC_CalcMaskValue				155

#define		CALC_FilterSmooth				160
#define		CALC_FilterGauss				161
#define		CALC_FilterGaussUnshade			162
#define		CALC_FilterMedian				163
#define		CALC_FilterErosion				164
#define		CALC_FilterDilation				165
#define		CALC_FilterOpening				166
#define		CALC_FilterClosing				167
#define		CALC_FilterEdgeSobel			168
#define		CALC_FilterLaplaceGauss			169
#define		CALC_FilterEdgeLog				170
#define		CALC_FilterMirror				171
#define		CALC_FilterNega					172
#define		CALC_FilterEdgeCanny			173
#define		CALC_FilterBilateral			174
#define		CALC_FilterGabor				175
#define		CALC_FilterEdgeSigmoid			176
#define		CALC_FilterCLAHE				177
#define		CALC_FilterLocalContrast		178
#define		CALC_FilterLocalContrastPatch	179
#define		CALC_Binalize					180
#define		CALC_BinalizeThresholdOtsu		181
#define		CALC_BinalizeThresholdPTile		182
#define		CALC_BinalizeLocalGauss			183
#define		CALC_BinalizeLocalLaplaceGauss	184
#define		CALC_BinalizeNick				185
#define		CALC_Label						190
#define		CALC_HoughLines					195
#define		CALC_BlendPoisson				196
#define		CALC_DFT						370					

//画像間演算
#define		CALC_Linear2DataUnit			350
#define		CALC_SumDataUnit				351
#define		CALC_SubtractDataUnit			352
#define		CALC_SubAbsDataUnit				353
#define		CALC_MultiplyDataUnit			354
#define		CALC_DivideDataUnit				355
#define		CALC_LinearDataUnit				356
#define		CALC_AbsDataUnit				357



//API 描画
#define		CALC_PrepeareDrawing			200
#define		CALC_DrawLineOnImage			201
#define		CALC_DrawCursorOnImage			202
#define		CALC_DrawCircleOnImage			203
#define		CALC_DrawRectOnImage			204
#define		CALC_DrawPointOnImage			205
#define		CALC_DrawTextOnImage			206
#define		CALC_FloodFillOnImage			207
#define		CALC_FillWorkAreaOnImage		208
#define		CALC_DrawLineOnImageF			209
#define		CALC_DrawImageOnImage			210
#define		CALC_FillWorkAreaOnMask			211

#define		CALC_DrawLine					220
#define		CALC_DrawCursor					221
#define		CALC_DrawRect					222
#define		CALC_DrawCircle					223
#define		CALC_DrawEllipse				224
#define		CALC_DrawText					225
#define		CALC_DrawText					225
#define		CALC_ClipSegment				226
#define		CALC_ClipSegmentF				227


//API メインウィンドウ機能の活用
#define		CALC_ShowMessage				230
#define		CALC_MessageBox					231
#define		CALC_CopyToClipbord				232
#define		CALC_PasteFromClipbord			233
#define		CALC_ToString					234
#define		CALC_ToNumber					235
#define		CALC_GetBatchInput				236
#define		CALC_SetBatchOutput				237
#define		CALC_SetBatchInput				238
#define		CALC_GetBatchOutput				239
#define     CALC_UserDlgBatchExecute		240
#define     CALC_CaptureOne					241
#define     CALC_SendString					242
#define     CALC_OpenFileDialog				243
#define     CALC_FolderDialog				244

//API ファイルIO
#define		CALC_SelectAndLoadImageFile		250
#define		CALC_SaveViewAsBitmap			251
#define		CALC_SaveArchivedImage			252
#define		CALC_GetDefaultPath				253
#define		CALC_CheckFileExist				254
#define		CALC_EnumFiles					255
#define		CALC_GetNextFilePath			256
#define		CALC_SelectAndSaveImageFile		257
#define		CALC_LoadBitmap					258

//API グラフ表示
#define		CALC_AddDataToChart				260
#define		CALC_AddDataToChart2			261
#define		CALC_ClearChart					262

//マクロ特有
#define		CALC_MCR_RESULT					280
#define		CALC_MCR_CHANGE_RESULT			281
#define		CALC_MCR_CREATE_DIRECTORY		284
#define		CALC_MCR_WRITE_TXT				290
#define		CALC_MCR_APPEND_TXT				291
#define		CALC_MCR_WAIT					292

//文字列操作
#define		CALC_STRLEN						300
#define		CALC_SUBSTR						301
#define		CALC_FINDSTR					302
#define		CALC_REPLACESTR					303


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




//マクロを定義
void CCalculate::define_macros()
{
	for(int i=0 ; i<CALC_MACRO_CNT_MAX ; i++){
		memset(&pMacroDefinision[i], 0, sizeof(MACRO_DEF_STRUCT));
	}

	//数値計算(0～99)
	define_macro(COM_TYPE_NUMBER, CALC_IMG_DAT, "I", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_IMG_DAT, "I", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_IMG_DAT, "I", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_IMG_DAT, "I", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SIN, "sin", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_COS, "cos", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_TAN, "tan", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_LOG, "log", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SQRT, "sqrt", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_EXP, "exp", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ABS, "abs", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_RANDOM, "random");
	define_macro(COM_TYPE_NUMBER, CALC_RAND, "rand");
	define_macro(COM_TYPE_NUMBER, CALC_CEIL, "ceil", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FLOOR, "floor", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ASIN, "asin", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ACOS, "acos", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_POW, "pow", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MOD, "mod", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ATAN2, "atan2", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ATAN, "atan", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_GAUS, "gaus");
	define_macro(COM_TYPE_NUMBER, CALC_THRESH, "thresh", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DELTA, "delta", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_IMG_DAT2, "A", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MASK_DAT, "M", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MIN, "min", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_MAX, "max", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_PI, "PI");
	define_macro(COM_TYPE_NUMBER, CALC_MCR_DIST, "dist", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_SIGMOID, "sigmoid", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_RELU, "ReLU", COM_TYPE_NUMBER);


	//PIMPOM APIの実行(100以降)
	//画像データアクセス
	define_macro(COM_TYPE_NUMBER, CALC_GetDataUnitNumber, "GetDataUnitNumber");
	define_macro(COM_TYPE_NUMBER, CALC_GetCurrentImageNumber, "GetCurrentImageNumber");
	define_macro(COM_TYPE_NUMBER, CALC_SetCurrentImageNumber, "SetCurrentImageNumber", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_GetDataTypeSize, "GetDataTypeSize", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_CopyDataUnit, "CopyDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DeleteDataUnit, "DeleteDataUnit", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_BackupDataUnit, "BackupDataUnit", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_RecoverDataUnit, "RecoverDataUnit", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ConvertDataUnit, "ConvertDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ConvertDispToByte, "ConvertDispToByte", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_STRING, CALC_GetSrcFilePathName, "GetSrcFilePathName", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_STRING, CALC_GetSrcFilePath, "GetSrcFilePath", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_STRING, CALC_GetSrcFileName, "GetSrcFileName", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_STRING, CALC_GetSrcFileTitle, "GetSrcFileTitle", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FormatDataUnit, "FormatDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_GetDataMinMax, "GetDataMinMax", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SetDispPage, "SetDispPage", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SetDispChannel, "SetDispChannel", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_UseWorkArea, "UseWorkArea", COM_TYPE_NUMBER, COM_TYPE_NUMBER);

	define_macro(COM_TYPE_NUMBER, CALC_DrawImage, "DrawImage", COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawImage, "DrawImage", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ScaleImage, "ScaleImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SetDispRange, "SetDispRange", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SetDispRangeAuto, "SetDispRangeAuto", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SetWorkArea, "SetWorkArea", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_GetWorkAreaVtx, "GetWorkAreaVtx", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_GetWorkAreaOrnot, "GetWorkAreaOrnot", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ChangePalette, "ChangePalette", COM_TYPE_NUMBER, COM_TYPE_NUMBER);

	define_macro(COM_TYPE_NUMBER, CALC_AddMark, "AddMark", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DeleteMark, "DeleteMark", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_GetMarkVal, "GetMarkVal", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_GetMarkNumber, "GetMarkNumber", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ClearMark, "ClearMark", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_AddLineMark, "AddLineMark", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);

	//API 画像処理
	define_macro(COM_TYPE_NUMBER, CALC_ResizeDataUnit, "ResizeDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ClipDataUnit, "ClipDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_PerspectiveTransDataUnit, "PerspectiveTransDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER,
																			COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_IsoTransDataUnit, "IsoTransDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_CalcDataValue, "CalcDataValue", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_CalcDataValue, "CalcDataValue", COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_CalcMaskValue, "CalcMaskValue", COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterSmooth, "FilterSmooth", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterGauss, "FilterGauss", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterGaussUnshade, "FilterGaussUnshade", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterMedian, "FilterMedian", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterErosion, "FilterErosion", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterDilation, "FilterDilation", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterOpening, "FilterOpening", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterClosing, "FilterClosing", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterEdgeSobel, "FilterEdgeSobel", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterLaplaceGauss, "FilterLaplaceGauss", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterEdgeLog, "FilterEdgeLog", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterEdgeCanny, "FilterEdgeCanny", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterBilateral, "FilterBilateral", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterMirror, "FilterMirror", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterGabor, "FilterGabor", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterEdgeSigmoid, "FilterEdgeSigmoid", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterNega, "FilterNega", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterCLAHE, "FilterCLAHE", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterLocalContrast, "FilterLocalContrast", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FilterLocalContrastPatch, "FilterLocalContrastPatch", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_Binalize, "Binalize", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_BinalizeThresholdOtsu, "BinalizeThresholdOtsu", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_BinalizeThresholdPTile, "BinalizeThresholdPTile", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_BinalizeLocalGauss, "BinalizeLocalGauss", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_BinalizeLocalLaplaceGauss, "BinalizeLocalLaplaceGauss", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_BinalizeNick, "BinalizeNick", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_Label, "Label", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_HoughLines, "HoughLines", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_BlendPoisson, "BlendPoisson", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER); 
	define_macro(COM_TYPE_NUMBER, CALC_DFT, "FilterDFT", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);

	//画像間演算
	define_macro(COM_TYPE_NUMBER, CALC_Linear2DataUnit, "Linear2DataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SumDataUnit, "SumDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SubtractDataUnit, "SubtractDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SubAbsDataUnit, "SubAbsDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MultiplyDataUnit, "MultiplyDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DivideDataUnit, "DivideDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_LinearDataUnit, "LinearDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_AbsDataUnit, "AbsDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER);

	//API 描画
	define_macro(COM_TYPE_NUMBER, CALC_PrepeareDrawing, "PrepeareDrawing", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawLineOnImage, "DrawLineOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawCursorOnImage, "DrawCursorOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawCircleOnImage, "DrawCircleOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawRectOnImage, "DrawRectOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawPointOnImage, "DrawPointOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawTextOnImage, "DrawTextOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FloodFillOnImage, "FloodFillOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FillWorkAreaOnImage, "FillWorkAreaOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawLineOnImageF, "DrawLineOnImageF", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawImageOnImage, "DrawImageOnImage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FillWorkAreaOnMask, "FillWorkAreaOnMask", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	

	define_macro(COM_TYPE_NUMBER, CALC_DrawLine, "DrawLine", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawCursor, "DrawCursor", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawRect, "DrawRect", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawCircle, "DrawCircle", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawEllipse, "DrawEllipse", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DrawText, "DrawText", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ClipSegment, "ClipSegment", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ClipSegmentF, "ClipSegmentF", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);


	//API メインウィンドウ機能の活用
	define_macro(COM_TYPE_NUMBER, CALC_ShowMessage, "ShowMessage", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_MessageBox, "MessageBox", COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_CopyToClipbord, "CopyToClipbord", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_PasteFromClipbord, "PasteFromClipbord", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_STRING, CALC_ToString, "ToString", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ToNumber, "ToNumber", COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_GetBatchInput, "GetBatchInput", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SetBatchOutput, "SetBatchOutput", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SetBatchInput, "SetBatchInput", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_GetBatchOutput, "GetBatchOutput", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_UserDlgBatchExecute, "UserDlgBatchExecute", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_CaptureOne, "CaptureOne");
	define_macro(COM_TYPE_NUMBER, CALC_SendString, "SendString", COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_OpenFileDialog, "OpenFileDialog", COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_FolderDialog, "FolderDialog");
	

	//API ファイルIO
	define_macro(COM_TYPE_NUMBER, CALC_SelectAndLoadImageFile, "SelectAndLoadImageFile", COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_SaveViewAsBitmap, "SaveViewAsBitmap", COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_SaveArchivedImage, "SaveArchivedImage", COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_GetDefaultPath, "GetDefaultPath");
	define_macro(COM_TYPE_NUMBER, CALC_CheckFileExist, "CheckFileExist", COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_EnumFiles, "EnumFiles", COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_GetNextFilePath, "GetNextFilePath", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SelectAndSaveImageFile, "SelectAndSaveImageFile", COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_LoadBitmap, "LoadBitmap", COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);

	//API グラフ表示
	define_macro(COM_TYPE_NUMBER, CALC_AddDataToChart, "AddDataToChart", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_AddDataToChart2, "AddDataToChart2", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ClearChart, "ClearChart", COM_TYPE_NUMBER);

	//マクロ特有
	define_macro(COM_TYPE_NUMBER, CALC_MCR_RESULT, "Res", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_CHANGE_RESULT, "ChangeRes", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_STRING, CALC_MCR_CREATE_DIRECTORY, "CreateDirectory", COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_MCR_WRITE_TXT, "WriteText", COM_TYPE_STRING, COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_MCR_APPEND_TXT, "AppendText", COM_TYPE_STRING, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_WAIT, "Wait", COM_TYPE_NUMBER);

	//文字列操作
	define_macro(COM_TYPE_NUMBER, CALC_STRLEN, "StrLen", COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_SUBSTR, "Substr", COM_TYPE_STRING, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FINDSTR, "FindStr", COM_TYPE_STRING, COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_REPLACESTR, "ReplaceStr", COM_TYPE_STRING, COM_TYPE_STRING, COM_TYPE_STRING);
}


//マクロ引数を定義(オーバーライド対応)
bool CCalculate::define_macro(
	int ret_type,
	int id, char name[],
	int argtype0, int argtype1, int argtype2, int argtype3, int argtype4,
	int argtype5, int argtype6, int argtype7, int argtype8, int argtype9,
	int argtype10, int argtype11, int argtype12, int argtype13, int argtype14)
{
	int overrideId = pMacroDefinision[id].overloadCnt;

	if (overrideId >= CALC_MACRO_OVERLOAD_MAX) {//オーバーロード可能数を超えたらエラー終了
		return false;
	}
	else if (overrideId == 0) {//最初に定義するときは、マクロ名を設定
		strcpy(pMacroDefinision[id].name, name);
	}
	else {//2回目以降の定義（オーバーロード）のときは、マクロ名が最初の定義と同じことを確認
		if (strcmp(pMacroDefinision[id].name, name) != 0) {//異なっていればエラー終了
			return false;
		}
	}


	pMacroDefinision[id].ret_type = ret_type;
	
	
	int args=0;

	if(argtype0>=0){	pMacroDefinision[id].arg_type[overrideId][0] = argtype0;	args++;	}
	if(argtype1>=0){	pMacroDefinision[id].arg_type[overrideId][1] = argtype1;	args++;	}
	if(argtype2>=0){	pMacroDefinision[id].arg_type[overrideId][2] = argtype2;	args++;	}
	if(argtype3>=0){	pMacroDefinision[id].arg_type[overrideId][3] = argtype3;	args++;	}
	if(argtype4>=0){	pMacroDefinision[id].arg_type[overrideId][4] = argtype4;	args++;	}
	if(argtype5>=0){	pMacroDefinision[id].arg_type[overrideId][5] = argtype5;	args++;	}
	if(argtype6>=0){	pMacroDefinision[id].arg_type[overrideId][6] = argtype6;	args++;	}
	if(argtype7>=0){	pMacroDefinision[id].arg_type[overrideId][7] = argtype7;	args++;	}
	if(argtype8>=0){	pMacroDefinision[id].arg_type[overrideId][8] = argtype8;	args++;	}
	if(argtype9>=0){	pMacroDefinision[id].arg_type[overrideId][9] = argtype9;	args++;	}
	if(argtype10>=0){	pMacroDefinision[id].arg_type[overrideId][10] = argtype10;	args++;	}
	if(argtype11>=0){	pMacroDefinision[id].arg_type[overrideId][11] = argtype11;	args++;	}
	if(argtype12>=0){	pMacroDefinision[id].arg_type[overrideId][12] = argtype12;	args++;	}
	if(argtype13>=0){	pMacroDefinision[id].arg_type[overrideId][13] = argtype13;	args++;	}
	if(argtype14>=0){	pMacroDefinision[id].arg_type[overrideId][14] = argtype14;	args++;	}

	pMacroDefinision[id].args[overrideId] = args;

	pMacroDefinision[id].overloadCnt++;

	return  true;
}





/********************************************************************
機  能  名  称 : マクロの処理をする
関    数    名 : execute_macro
引          数 :
戻    り    値 : 成功したらtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::execute_macro(
							  int					id,
							  CALC_PARAM_STRUCT		*p_calc_param,			//(in/out)計算の入出力パラメタ
							  MACRO_IO_STRUCT		*p_result,				//(out)結果
							  int					macroID,				//(in)マクロ番号
							  int					overloadID,			//(in)オーバーライド番号
							  MACRO_IO_STRUCT		params[],				//(in)マクロ引数
							 CString				*p_err_message,			//(out)エラーメッセージ
							 bool					autoDrawImage			//(in)処理後に画像描画を実行する
							 )
{
	p_result->type = COM_TYPE_NUMBER;//出力の初期化(特に規定されていないマクロは数値0を返す)
	p_result->number = 0;


	if(!p_calc_param->apiMode && macroID>=100){//マクロを実行しない場合は処理をスキップする
		return true;
	}

	//オーバーロード対応
	if (overloadID >= CALC_MACRO_OVERLOAD_MAX) {
		return false;
	}




	switch (macroID)
	{
		//macroNo 0～99 数値計算部分

	case CALC_RANDOM:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = rand() / 32767.0;
		break;
	case CALC_RAND:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = rand();
		break;
	case CALC_GAUS:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.calc_gaus();
		break;
	case CALC_MCR_PI:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = PI;
		break;
	case CALC_IMG_DAT:
		if(overloadID==0){
			p_result->type = COM_TYPE_NUMBER;
			if (!refer_image(&p_result->number, params[0].number, (double)p_calc_param->x, (double)p_calc_param->y, -1, p_calc_param->rgbType, p_err_message))	return	false;
		}
		else if (overloadID == 1){
			p_result->type = COM_TYPE_NUMBER;
			if (!refer_image(&p_result->number, params[0].number, (double)p_calc_param->x, (double)p_calc_param->y, params[1].number, p_calc_param->rgbType, p_err_message))	return	false;
		}
		else if(overloadID == 2){
			p_result->type = COM_TYPE_NUMBER;
			if (!refer_image(&p_result->number, params[0].number, (double)params[1].number, (double)params[2].number, - 1, p_calc_param->rgbType, p_err_message))	return	false;
		}
		else{
			p_result->type = COM_TYPE_NUMBER;
			if (!refer_image(&p_result->number, params[0].number, (double)params[1].number, (double)params[2].number, (int)params[3].number, (int)params[4].number, p_err_message))	return	false;
		}
		break;
	case CALC_MASK_DAT:
		p_result->type = COM_TYPE_NUMBER;
		if (!refer_image(&p_result->number, params[0].number, (double)p_calc_param->x, (double)p_calc_param->y, -1,-1, p_err_message, true))	return	false;
		break;
	case CALC_SIN:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = sin(params[0].number);
		break;
	case CALC_COS:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = cos(params[0].number);
		break;
	case CALC_TAN:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = tan(params[0].number);
		break;
	case CALC_LOG:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = log(params[0].number);
		break;
	case CALC_SQRT:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = sqrt(params[0].number);
		break;
	case CALC_EXP:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = exp(params[0].number);
		break;
	case CALC_ABS:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = fabs(params[0].number);
		break;
	case CALC_CEIL:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = ceil(params[0].number);
		break;
	case CALC_FLOOR:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = floor(params[0].number);
		break;
	case CALC_ASIN:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = asin(params[0].number);
		break;
	case CALC_ACOS:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = acos(params[0].number);
		break;
	case CALC_ATAN:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = atan(params[0].number);
		break;
	case CALC_POW:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = pow(params[0].number, params[1].number);
		break;
	case CALC_MOD:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = fmod(params[0].number, params[1].number);
		break;
	case CALC_ATAN2:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = atan2(params[0].number, params[1].number);
		break;
	case CALC_THRESH:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = (params[0].number > params[1].number) ? 1 : 0;
		break;
	case CALC_DELTA:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = (params[0].number == params[1].number) ? 1 : 0;
		break;
	case CALC_MCR_MIN:
		{
			p_result->type = COM_TYPE_NUMBER;

			float mindata = params[0].number;
			for(int i=1 ; i<overloadID+2 ;  i++ ){
				if(mindata > params[i].number){
					mindata = params[i].number;
				}
			}
			
			p_result->number = mindata;
		}
		break;
	case CALC_MCR_MAX:
		{
			p_result->type = COM_TYPE_NUMBER;

			float maxdata = params[0].number;
			for(int i=1 ; i<overloadID+2 ;  i++ ){
				if(maxdata < params[i].number){
					maxdata = params[i].number;
				}
			}
			
			p_result->number = maxdata;
		}
		break;
	case CALC_IMG_DAT2:
		p_result->type = COM_TYPE_NUMBER;
		if (!refer_image(&p_result->number, (int)params[0].number, (double)params[1].number, (double)params[2].number, (int)params[3].number, (int)params[4].number, p_err_message))	return	false;
		break;
	case CALC_MCR_DIST:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.calc_dist(params[0].number, params[1].number, params[2].number, params[3].number);
		break;
	case CALC_MCR_SIGMOID:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = 1.0 / ( 1.0 + exp( -params[0].number * params[1].number ));
		break;
	case CALC_MCR_RELU:
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = (params[0].number>0) ? params[0].number : 0 ;
		break;



		//macroNo 100以降　API部分

	case CALC_GetDataUnitNumber://画像メモリの総数を返す
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.GetDataUnitNumber();
		break;

	case CALC_GetCurrentImageNumber://現在選択中の画像メモリの番号を返す
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.GetCurrentImageNumber();
		break;

	case CALC_SetCurrentImageNumber://現在選択中の画像メモリの番号を変更する
		if (!API.SetCurrentImageNumber((int)params[0].number)) {
			p_err_message->Format("Error: SetCurrentImageNumber()");
			return false;
		}
		break;

	case CALC_GetDataTypeSize://画像メモリの型・サイズ・奥行等取得
	{
		int type, width, height, page;
		if (!API.GetDataTypeSize((int)params[0].number, &type, &width, &height, &page))
		{
			p_err_message->Format("Error: GetDataTypeSize()");
			return	false;
		}
		else {
			p_result->type = COM_TYPE_NUMBER;

			if (params[1].number == 0) {//タイプ
				p_result->number = type;
			}
			else if (params[1].number == 1) {//幅
				p_result->number = width;
			}
			else if (params[1].number == 2) {//高さ
				p_result->number = height;
			}
			else if (params[1].number == 3) {//奥行
				p_result->number = page;
			}
			else {
				p_err_message->Format("Error: 2nd argument must be within 0-3");
				return false;
			}
		}
	}
	break;



	case CALC_CopyDataUnit://画像メモリをコピーする
		if (!API.CopyDataUnit((int)params[0].number, (int)params[1].number))
		{
			p_err_message->Format("Error: CopyDataUnit()");
			return	false;
		}
		if (autoDrawImage)	API.DrawImage((int)params[1].number);
		break;

	case CALC_DeleteDataUnit://画像メモリを破棄する
		if (!API.DeleteDataUnit((int)params[0].number))
		{
			p_err_message->Format("Error: DeleteDataUnit()");
			return	false;
		}
		break;

	case CALC_BackupDataUnit://画像メモリをバックアップする
		if (!API.BackupDataUnit((int)params[0].number)) {
			p_err_message->Format("Error: BackupDataUnit()");
			return false;
		}
		break;

	case CALC_RecoverDataUnit://バックアップされた画像メモリを復活させる
		if (!API.RecoverDataUnit((int)params[0].number)) {
			p_err_message->Format("Error: RecoverDataUnit()");
			return false;
		}
		break;

	case CALC_ConvertDataUnit://画像メモリのタイプを変更
		if (!API.ConvertDataUnit((int)params[0].number,		//対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,	//変換後の画像メモリの型  (0:BYTE 1:FLOAT 2:複素数 3:3D 4:F3D 5:XY 6:XYZ 7:RGB 8:WORD 9:LONG32 10:RGB_3D 11:SHORT16 12:DWORD)
			(int)params[2].number,	//RGBに変換する場合の挿入先 （1:R成分 2:G成分 3:B成分 1-3以外:RGB全成分）
			(int)params[3].number,	//複素数に変換する場合の挿入先（0:実部　0以外:虚部）
			(bool)params[4].number,	//型式の上下限を超える値を変換するとき飽和させるかどうか (0:しない 0以外:する)
			(int)params[5].number,	//BYTEに変換する場合、表示メモリorマスクから変換するかどうか （0:データから 1:表示画像から 2:マスクから）
			(bool)params[6].number	//RGBからグレーに変換する方法(1:彩度 2:色相  これら以外:明度)
		))
		{
			p_err_message->Format("Error: ConvertDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_ConvertDispToByte://画像メモリをBYTE型にコンバート
		if (!API.ConvertDispToByte((int)params[0].number)) {
			p_err_message->Format("Error: ConvertDispToByte()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_GetSrcFilePathName://
	{
		int str_id = push_string_to_buffer(id, API.GetSrcFilePathName((int)params[0].number), p_err_message);
		if (str_id < 0) {
			return false;
		}

		p_result->type = COM_TYPE_STRING;
		p_result->str_id = str_id;
	}
	break;

	case CALC_GetSrcFilePath://
	{
		int str_id = push_string_to_buffer(id, API.GetSrcFilePath((int)params[0].number), p_err_message);
		if (str_id < 0) {
			return false;
		}

		p_result->type = COM_TYPE_STRING;
		p_result->str_id = str_id;
	}
	break;

	case CALC_GetSrcFileName://
	{
		int str_id = push_string_to_buffer(id, API.GetSrcFileName((int)params[0].number), p_err_message);
		if (str_id < 0) {
			return false;
		}

		p_result->type = COM_TYPE_STRING;
		p_result->str_id = str_id;
	}
	break;

	case CALC_GetSrcFileTitle://
	{
		int str_id = push_string_to_buffer(id, API.GetSrcFileTitle((int)params[0].number), p_err_message);
		if (str_id < 0) {
			return false;
		}

		p_result->type = COM_TYPE_STRING;
		p_result->str_id = str_id;
	}
	break;

	case CALC_FormatDataUnit://画像メモリフォーマット
	{
		if (API.FormatDataUnit((int)params[0].number,	//(in)画像メモリ番号
			(int)params[1].number,	//(in)画像メモリタイプ
			CSize((int)params[2].number, (int)params[3].number),//高さ・幅
			(int)params[4].number//ページ番号
			, true) == NULL)
		{
			return false;
		}
	}

	if (autoDrawImage)	API.DrawImage((int)params[0].number);

	break;

	case CALC_GetDataMinMax://画像メモリの最大・最小値取得
	{
		float max, min;
		if (!API.GetDataMinMax((int)params[0].number, &min, &max))
		{
			p_err_message->Format("Error: GetDataMinMax()");
			return	false;
		}
		else {
			p_result->type = COM_TYPE_NUMBER;

			if (params[1].number == 0) {//min
				p_result->number = min;
			}
			else if (params[1].number == 1) {//max
				p_result->number = max;
			}
			else {
				p_err_message->Format("Error: 2nd argument must be within 0-1");
				return false;
			}
		}
	}
	break;

	case CALC_SetDispPage://表示ページ番号設定
		if (!API.SetDispPage((int)params[0].number,		//(in)画像メモリ番号
			(int)params[1].number		//(in)ページ番号
		))
		{
			p_err_message->Format("Error: SetDispPage()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_SetDispChannel://表示チャネル番号設定
		if (!API.SetDispChannel((int)params[0].number,		//(in)画像メモリ番号
			(int)params[1].number		//(in)チャネル番号(RGB表示の場合は-1)
		))
		{
			p_err_message->Format("Error: SetDispChannel()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_UseWorkArea:
		{
			CDataUnit *pdu = API.GetDataUnit((int)params[0].number);
			if (pdu == NULL){
				p_err_message->Format("Error: UseWorkArea()");
				return false;
			}

			pdu->DoUseWorkArea = (bool)params[1].number;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
		}
		break;


	case CALC_DrawImage://画像描画
		if (overloadID == 0) 
		{
			API.DrawImage((int)params[0].number,					//(in)画像メモリ番号
				get_string_from_buffer(id, (int)params[1].str_id),	//(in)名前
				true,
				(bool)params[2].number					//(in)画面サイズにあわせるか　(0以外:あわせる 0:あわせない)
			);
		}
		else if(overloadID==1)
		{
			API.DrawImage((int)params[0].number);					//(in)画像メモリ番号
		}
		break;

	case CALC_ScaleImage://画像描画
		if (!API.ScaleImage((int)params[0].number,					//(in)画像メモリ番号
			(float)params[1].number					//(in)スケール
		))
		{
			p_err_message->Format("Error: ScaleImage()");
			return false;
		}
		break;

	case CALC_SetDispRange://表示濃度のレンジを変更して描画する
		if (!API.SetDispRange((int)params[0].number,			//(in)画像メモリ番号
			(float)params[1].number,		//(in)濃度最小値
			(float)params[2].number			//(in)濃度最大値
		))
		{
			p_err_message->Format("Error: SetDispRange()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_SetDispRangeAuto://表示濃度のレンジを自動で変更して描画する
		if (!API.SetDispRangeAuto((int)params[0].number,		//(in)画像メモリ番号
			(int)params[1].number		//(in)自動設定方法  （ 0:最大/最小 1:平均値±標準偏差×2）
		))
		{
			p_err_message->Format("Error: SetDispRangeAuto()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_SetWorkArea://作業領域を矩形に設定
		if (!API.SetWorkArea((int)params[0].number,			//(in)画像メモリ番号
			(int)params[1].number,			//(in)作業領域の図形番号
			CRect((int)params[2].number,	//(in)作業領域左上x
			(int)params[3].number,	//(in)作業領域左上y
				(int)params[4].number,	//(in)作業領域右下x
				(int)params[5].number),	//(in)作業領域右下y
				(bool)params[6].number			//(in)OR/NOT　（0以外:OR  0:NOT）
		))
		{
			p_err_message->Format("Error: SetWorkArea()");
			return false;
		}

		if (autoDrawImage) {
			CDataUnit *pdu = API.GetDataUnit((int)params[0].number);
			if (pdu) {
				pdu->DoUseWorkArea = true;
			}


			API.DrawImage((int)params[0].number);
		}

		break;

	case CALC_GetWorkAreaVtx://作業領域(矩形)の4隅座標を取得
	{
		FIGURE_UNION fig = { 0 };
		bool	ornot;
		if (API.GetWorkArea((int)params[0].number,			//(in)画像メモリ番号
			(int)params[1].number,			//(in)作業領域の図形番号
			&fig,
			&ornot
		) != WORK_AREA_TYPE_RECT)
		{
			p_err_message->Format("Error: figure type is not RECT");
			return false;
		}
		else {
			p_result->type = COM_TYPE_NUMBER;

			if (params[2].number == 0) {//左上x
				p_result->number = fig.rect.left;
			}
			else if (params[2].number == 1) {//左上y
				p_result->number = fig.rect.top;
			}
			else if (params[2].number == 2) {//右下x
				p_result->number = fig.rect.right;
			}
			else if (params[2].number == 3) {//右下y
				p_result->number = fig.rect.bottom;
			}
			else {
				p_err_message->Format("Error: 3rd argument must be within 0-3");
				return false;
			}
		}
	}
	break;

	case CALC_GetWorkAreaOrnot://作業領域(矩形)のOR/NOTを取得
	{
		FIGURE_UNION fig = { 0 };
		bool	ornot;
		if (API.GetWorkArea((int)params[0].number,			//(in)画像メモリ番号
			(int)params[1].number,			//(in)作業領域の図形番号
			&fig,
			&ornot
		) == -1)
		{
			p_err_message->Format("Error: GetWorkArea()");
			return false;
		}
		else
		{
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = ornot ? 1 : 0;
		}
	}
	break;

	case CALC_ChangePalette://カラーパレットを変更する
	{
		if (!API.ChangePalette(
			(int)params[0].number,			//(in)画像メモリ番号)
			(int)params[1].number) )
		{
			p_err_message->Format("Error: ChangePalette()");
			return false;
		}
	}
	break;

	case CALC_AddMark://マークを追加する
	{
		int mark_number = API.AddMark(
			(int)params[0].number,			//(in)画像メモリ番号
			(float)params[1].number,		//(in)x
			(float)params[2].number			//(in)y
		);
		if (mark_number == -1) {
			p_err_message->Format("Error: AddMark()");
		}
		p_result->number = mark_number;
	}
	break;

	case CALC_DeleteMark://マークを追加する
		if (!API.DeleteMark((int)params[0].number,		//(in)画像メモリ番号
			(int)params[1].number		//(in)マーク番号
		))
		{
			p_err_message->Format("Error: DeleteMark()");
			return false;
		}
		break;

	case CALC_GetMarkVal://マークの値を取得する
	{
		float x, y;
		double val;
		if (!API.GetMarkVal((int)params[0].number,		//(in)画像メモリ番号
			(int)params[1].number,		//(in)マーク番号
			&x, &y, &val
		))
		{
			p_err_message->Format("Error: DeleteMark()");
			return false;
		}
		else {
			p_result->type = COM_TYPE_NUMBER;

			if (params[2].number == 0) {
				p_result->number = x;
			}
			else if (params[2].number == 1) {
				p_result->number = y;
			}
			else if (params[2].number == 2) {
				p_result->number = val;
			}
			else {
				p_err_message->Format("Error: 3rd argument must be within 0-2");
				return false;
			}
		}
	}
	break;

	case CALC_GetMarkNumber://マークの数を取得する
	{
		int mark_number = API.GetMarkNumber((int)params[0].number);
		if (mark_number == -1) {
			p_err_message->Format("Error: GetMarkNumber()");
			return false;
		}
		p_result->number = mark_number;
	}
	break;


	case CALC_ClearMark://すべてのマークを削除する
	{
		if (!API.ClearMark((int)params[0].number)){
			p_err_message->Format("Error: ClearMark()");
			return false;
		}
	}
	break;


	case CALC_AddLineMark://線マークを追加する
	{
		int mark_number = API.AddLineMark(
			(int)params[0].number,			//(in)画像メモリ番号
			(float)params[1].number,		//(in)x0
			(float)params[2].number,		//(in)y0
			(float)params[3].number,		//(in)x1
			(float)params[4].number			//(in)y1
			);
		if (mark_number == -1) {
			p_err_message->Format("Error: AddLineMark()");
		}
		p_result->number = mark_number;
	}
	break;


	case CALC_ResizeDataUnit://画像のサイズを変更
		if (!API.ResizeDataUnit((int)params[0].number,	//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,	//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			true,
			CSize((int)params[2].number,		//(in)変換後の幅
			(int)params[3].number),	//(in)変換後の高さ
				(bool)params[4].number,			//(in)変換後のサイズに合わせて画像を拡大/縮小(0以外：する  0:しない)
			(int)params[5].number,				//(in)オフセットx
			(int)params[6].number,				//(in)オフセットy
			false,
			(int)params[7].number				//(in)画像縮小タイプ(0:area integration 1:max 2:min)
		))
		{
			p_err_message->Format("Error: ResizeDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_ClipDataUnit://画像をクリッピング
		if (!API.ClipDataUnit((int)params[0].number,	 //(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,	 //(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			true,
			CRect((int)params[2].number,	//(in)クリップ領域左端
			(int)params[3].number,	//(in)クリップ領域上端
				(int)params[4].number,	//(in)クリップ領域右端
				(int)params[5].number)	//(in)クリップ領域下端
		))
		{
			p_err_message->Format("Error: ClipDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_PerspectiveTransDataUnit://パースペクティブ変換
		if (!API.PerspectiveTransDataUnit((int)params[0].number,		//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,		//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			true,
			params[2].number,		//(in)画像内平行移動
			params[3].number,		//(in)画像内平行移動
			params[4].number,		//(in)撮像面内平行移動
			params[5].number,		//(in)撮像面内平行移動
			params[6].number,		//(in)撮像面内回転 [deg]
			params[7].number,		//(in)カメラ横方向傾斜[deg]
			params[8].number,		//(in)カメラ縦方向傾斜[deg]
			params[9].number,		//(in)レンズ焦点距離[mm]
			params[10].number,		//(in)ccd1画素サイズ[μm]
			false,
			NULL,
			1,
			(int)params[11].number		//(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
		))
		{
			p_err_message->Format("Error: PerspectiveTransDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_IsoTransDataUnit://平行移動、回転移動、拡大
		if (!API.IsoTransDataUnit((int)params[0].number,		//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,		//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			true,
			params[2].number,		//(in)平行移動tx
			params[3].number,		//(in)平行移動ty
			params[4].number,		//(in)転移動[deg]
			params[5].number,		//(in)拡大率
			false,
			NULL,
			(int)params[6].number		//(in)画像からはみ出した部分の処理の方法(0:そのまま 1:最近傍画素の値で補間)
		))
		{
			p_err_message->Format("Error: IsoTarnsDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_CalcDataValue://任意の数式計算
		if(overloadID==0)
		{
			if (!API.CalcDataValue( (int)params[0].number,	//(in)描画対象画像メモリ番号
									(int)params[1].number,	//(in)作成する画像のタイプ
									CSize( (int)params[2].number,  //(in)作成する画像の幅
									       (int)params[3].number),  //(in)作成する画像の高さ
									get_string_from_buffer(id, params[4].str_id),//(in)テキスト
									(bool)params[5].number,//(in)飽和するかどうか
									p_err_message))
			{
				return false;
			}
		}
		else if (overloadID == 1)
		{
			if (!API.CalcDataValue((int)params[0].number,	//(in)描画対象画像メモリ番号
				get_string_from_buffer(id, params[1].str_id),//(in)テキスト
				(bool)params[2].number,//(in)飽和するかどうか
				p_err_message))
			{
				return false;
			}
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;


	case CALC_CalcMaskValue:
		if (!API.CalcMaskValue((int)params[0].number,	//(in)描画対象画像メモリ番号
			get_string_from_buffer(id, params[1].str_id),//(in)テキスト
			(bool)params[2].number,//(in)マスクOFFとする最大値
			(bool)params[3].number,//(in)マスクOFFとする最小値
			p_err_message))
		{
			return false;
		}
		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;


	case CALC_FilterSmooth://平滑化（移動平均）
		if (!API.FilterSmooth((int)params[0].number,				//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,				//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			CSize((int)params[2].number,		//(in)フィルタ幅
			(int)params[3].number),				//(in)フィルタ高さ
			(bool)params[4].number,				//(in)上下限値を除去（0以外：する 0:しない）
			(bool)params[5].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterSmooth()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterGauss://平滑化（ガウシアン）
		if (!API.FilterGauss((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ（高さ＆幅）
			(int)params[3].number,			//(in)フィルタ方向 　（1:横のみ 2:縦のみ  それ以外:両方）
			(int)params[4].number,			//(in)padding  (1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
			(bool)params[5].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterGauss()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterGaussUnshade://ガウシアン自己差分によるシェーディング除去
		if (!API.FilterGaussUnshade((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ（高さ＆幅）
			(float)params[3].number,		//(in)オフセット
			(float)params[4].number,		//(in)コントラスト
			(int)params[5].number,			//(in)フィルタ方向 　（1:横のみ 2:縦のみ  それ以外:両方）
			(int)params[6].number,			//(in)padding  (1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
			(bool)params[7].number			//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterGaussUnshade()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterMedian://メディアンフィルタ
		if (!API.FilterMedian((int)params[0].number,				//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,				//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			CSize((int)params[2].number,		//(in)フィルタ幅
			(int)params[3].number),				//(in)フィルタ高さ
			(bool)params[4].number,				//(in)円形マスク(0以外：使う 0:使わない)
			(bool)params[5].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterMedian()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterErosion://収縮フィルタ
		if (!API.FilterErosion((int)params[0].number,				//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,				//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			CSize((int)params[2].number,		//(in)フィルタ幅
			(int)params[3].number),				//(in)フィルタ高さ
			(bool)params[4].number,				//(in)円形マスク(0以外：使う 0:使わない)
			(bool)params[5].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterErosion()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterDilation://膨張フィルタ
		if (!API.FilterDilation((int)params[0].number,				//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,				//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			CSize((int)params[2].number,		//(in)フィルタ幅
			(int)params[3].number),				//(in)フィルタ高さ
			(bool)params[4].number,				//(in)円形マスク(0以外：使う 0:使わない)
			(bool)params[5].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterDilation()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterOpening://オープニングフィルタ
		if (!API.FilterOpening((int)params[0].number,				//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,				//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			CSize((int)params[2].number,		//(in)フィルタ幅
			(int)params[3].number),				//(in)フィルタ高さ
			(bool)params[4].number,				//(in)円形マスク(0以外：使う 0:使わない)
			(bool)params[5].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterOpening()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterClosing://クロージングフィルタ
		if (!API.FilterClosing((int)params[0].number,				//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,				//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			CSize((int)params[2].number,		//(in)フィルタ幅
			(int)params[3].number),				//(in)フィルタ高さ
			(bool)params[4].number,				//(in)円形マスク(0以外：使う 0:使わない)
			(bool)params[5].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterClosing()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterEdgeSobel://ソーベルフィルタによるエッジ抽出
		if (!API.FilterEdgeSobel((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ ( 3: 3x3,  5:5x5,  7:7x7,  9:9x9 )
			(float)params[3].number,		//(in)エッジ強度最小値（この値より小さい場合は0強制的に0にする）
			(int)params[4].number,			//(in)フィルタ方向 （0:横のみ 1:縦のみ  それ以外:両方）
			(bool)params[5].number			//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterEdgeSobel()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;


	case CALC_FilterEdgeCanny://Cannyフィルタによるエッジ抽出
		if (!API.FilterEdgeCanny(
			(int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ ( 3: 3x3,  5:5x5,  7:7x7)
			(float)params[3].number,		//(in)エッジ強度閾値1
			(float)params[4].number,			//(in)エッジ強度閾値2
			(bool)params[5].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterEdgeCanny()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterBilateral://バイラテラルフィルタ
		if (!API.FilterBilateral(
			(int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ ( 3: 3x3,  5:5x5,  7:7x7)
			(float)params[3].number,		//(in)値のσ
			(float)params[4].number,		//(in)距離のσ
			(bool)params[5].number			//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterBilateral()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterLaplaceGauss://ラプラシアンオブガウシアンフィルタ
		if (!API.FilterLaplaceGauss((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ
			(int)params[3].number,			//(in)フィルタ方向 （0:横のみ 1:縦のみ  それ以外:両方）
			(float)params[4].number,		//(inオフセット
			(float)params[5].number,		//(in)コントラスト
			(int)params[6].number,			//(in)padding  (1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
			(bool)params[7].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterLaplaceGauss()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterGabor://ガボールフィルタ
		if (!API.FilterGabor((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ
			(float)params[3].number,		//(in)γ：カーネル扁平率
			(float)params[4].number,		//(in)カーネル内での周期
			(float)params[5].number,		//(in)θ：カーネル回転角度
			(float)params[6].number,		//(in)Ψ：カーネル初期位相
			(float)params[7].number,		//(inオフセット
			(float)params[8].number,		//(in)コントラスト
			(int)params[9].number,			//(in)padding  (1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
			(bool)params[10].number			//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterGabor()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterEdgeSigmoid://sigmoidフィルタ
		if (!API.FilterEdgeSigmoid((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ
			(float)params[3].number,		//(in)k
			(float)params[4].number,		//(in)θ：カーネル回転角度
			(float)params[5].number,		//(inオフセット
			(float)params[6].number,		//(in)コントラスト
			(int)params[7].number,			//(in)padding  (1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
			(bool)params[8].number				//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterEdgeSigmoid()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;


	case CALC_FilterEdgeLog://ラプラシアンオブガウシアンによるエッジ抽出
		if (!API.FilterEdgeLog((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)フィルタサイズ
			(float)params[3].number,		//(in)エッジ強度最小値（この値より小さい場合は0強制的に0にする）
			(int)params[4].number,			//(in)フィルタ方向 （0:横のみ 1:縦のみ  それ以外:両方）
			(int)params[5].number,			//(in)padding  (1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
			(bool)params[6].number			//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterEdgeLog()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterMirror://鏡像
		if (!API.FilterMirror((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)鏡像反転の方向 ( 0:横  1:縦 2:180°)
			(bool)params[3].number			//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterMirror()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterNega://濃度反転
		if (!API.FilterNega((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(bool)params[2].number			//(in)複数のページ／チャネルに同じ処理をするかどうか
		))
		{
			p_err_message->Format("Error: FilterNega()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterCLAHE://CLAHE
		if (!API.FilterCLAHE((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,							//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(double)params[2].number,						//(in)度数上限値
			(int)params[3].number,							//(in)横方向分割数
			(int)params[4].number,							//(in)縦方向分割数
			(bool)params[5].number							//(in)複数のページ／チャネルに同じ処理をするかどうか
			))
		{
			p_err_message->Format("Error: FilterCLAHE()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterLocalContrast:
		if (!API.FilterLocalContrast((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,							//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,							//(in)フィルタサイズ
			(float)params[3].number,							//(in)コントラスト拡大率
			(bool)params[4].number							//(in)複数のページ／チャネルに同じ処理をするかどうか
			))
		{
			p_err_message->Format("Error: FilterLocalContrast()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterLocalContrastPatch:
		if (!API.FilterLocalContrastPatch((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,							//(in)格納先画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,							//(in)パッチ分割数
			(bool)params[4].number							//(in)複数のページ／チャネルに同じ処理をするかどうか
			))
		{
			p_err_message->Format("Error: FilterLocalContrastPatch()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_Binalize://グローバル閾値による2値化
		if (!API.Binalize((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)2値化後の画像を入れるメモリ
			(float)params[2].number,		//(in)最小閾値
			(float)params[3].number,		//(in)最大閾値
			(bool)params[4].number			//(in)２値反転(0以外:する  0:しない)
		))
		{
			p_err_message->Format("Error: Binalize()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_BinalizeThresholdOtsu://大津の手法で2値化の閾値を求める
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.BinalizeThresholdOtsu((int)params[0].number);
		break;

	case CALC_BinalizeThresholdPTile://パーセンタイル法で閾値を求める
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.BinalizeThresholdPTile((int)params[0].number, (int)params[1].number);
		break;

	case CALC_BinalizeLocalGauss://原画像とガウシアンフィルタ後画像の差分によるローカル2値化
		if (!API.BinalizeLocalGauss((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)2値化後の画像を入れるメモリ
			(int)params[2].number,			//(in)フィルタサイズ（高さ＆幅）
			(float)params[3].number,		//(in)オフセット
			(bool)params[4].number,			//(in)２値反転(0以外:する  0:しない)
			255,
			0,
			(int)params[5].number			//(in)padding  (1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
		))
		{
			p_err_message->Format("Error: BinalizeLocalGauss()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_BinalizeLocalLaplaceGauss://ラプラシアンオブガウシアンフィルタによる2値化
		if (!API.BinalizeLocalLaplaceGauss((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)2値化後の画像を入れるメモリ
			(int)params[2].number,			//(in)フィルタサイズ（高さ＆幅）
			(float)params[3].number,		//(in)オフセット
			(bool)params[4].number,			//(in)２値反転(0以外:する  0:しない)
			255,
			0,
			(int)params[5].number			//(in)padding  (1:外周の画素を延長  2:外周で折り返し   それ以外:ゼロ)
		))
		{
			p_err_message->Format("Error: BinalizeLocalLaplaceGauss()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_BinalizeNick://NICKの2値化
		if (!API.BinalizeNick((int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)2値化後の画像を入れるメモリ
			(int)params[2].number,			//(in)フィルタサイズ（高さ＆幅）
			(float)params[3].number,		//(in)Nickのk
			(bool)params[4].number			//(in)２値反転(0以外:する  0:しない)
		))
		{
			p_err_message->Format("Error: BinalizeNick()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_Label://2値化画像をラベリングする
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.Label((int)params[0].number,	//(in)2値画像メモリ番号(この画像の値は、0/255のみで構成されている)
			(int)params[1].number,			//(in)ラベル画像を入れるメモリ
			(bool)params[2].number,			//(in)面積降順でソートするかどうか(0以外:する  0:しない)
			(int)params[3].number			//(in)最小面積
		);

		{
			CDataUnit *pdu;
			if ((pdu = API.GetDataUnit((int)params[1].number))) {
				pdu->ChangePalette(PALETTE_LABEL);//ラベルがわかりやすいようなカラーパレットに変更
			}
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);
			
		break;

	case CALC_HoughLines://Hough変換による直線抽出
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = API.HoughLines( 
										(int)params[0].number,			//(in)対象画像メモリ番号(-1にした場合は現在選択中の画像)
										(int)params[1].number,			//(in)投票空間の距離分解能
										(int)params[2].number,			//(in)投票空間の角度分解能
										(int)params[3].number,			//(in)投票の閾値パラメータ．十分な票を得た直線のみが出力されます．
										params[4].number,				//(in)最小の線分長．これより短い線分は棄却されます
										params[5].number				//(in)2点が同一線分上にあると見なす場合に許容される最大距離．
								 );

			if (autoDrawImage)	API.DrawImage((int)params[1].number);
			
			break;


	case CALC_BlendPoisson://ポワソン画像合成
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = API.BlendPoisson(
										(int)params[0].number,			//(in)元画像メモリ番号(-1にした場合は現在選択中の画像)
										(int)params[1].number,			//(in)ブレンド画像メモリ番号
										(int)params[2].number,			//(in)結果画像メモリ番号(-1にした場合は現在選択中の画像)
										(int)params[3].number,			//(in)合成開始位置x
										(int)params[4].number,			//(in)合成開始位置y
										(int)params[5].number			//(in)合成方法 0:FOREGROUND_GRADIENTS  1:AVERAGED_GRADIENTS  2:MIXED_GRADIENTS
				);

			if (autoDrawImage)	API.DrawImage((int)params[2].number);

			break;

	case CALC_DFT:
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = API.FilterDFT(
				(int)params[0].number,			//(in)元画像メモリ番号(-1にした場合は現在選択中の画像)
				(int)params[1].number,			//(in)結果画像メモリ番号(-1にした場合は現在選択中の画像)
				(int)params[2].number			//(in)trueで逆DFT
				);

			API.DrawImage((int)params[1].number);
			break;

	case CALC_Linear2DataUnit://2画像の線形演算を行う
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.Linear2DataUnit(
			(int)params[0].number,			//(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[3].number,			//(in)結果画像のフォーマット
			(double)params[4].number,		//(in)元画像Aにかける重み
			(double)params[5].number,		//(in)元画像Bにかける重み
			(double)params[6].number,		//(in)バイアス
			(int)params[7].number			//(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_SumDataUnit://2画像の和
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.SumDataUnit(
			(int)params[0].number,			//(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[3].number,			//(in)結果画像のフォーマット
			(int)params[4].number			//(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_SubtractDataUnit://2画像の差
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.SubtractDataUnit(
			(int)params[0].number,			//(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[3].number,			//(in)結果画像のフォーマット
			(int)params[4].number			//(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_SubAbsDataUnit://2画像の差の絶対値
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.SubAbsDataUnit(
			(int)params[0].number,			//(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[3].number,			//(in)結果画像のフォーマット
			(int)params[4].number			//(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_MultiplyDataUnit://2画像の積
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.MultiplyDataUnit(
			(int)params[0].number,			//(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[3].number,			//(in)結果画像のフォーマット
			(int)params[4].number			//(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_DivideDataUnit://2画像の商
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.DivideDataUnit(
			(int)params[0].number,			//(in)元画像Aのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)元画像Bのメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[3].number,			//(in)結果画像のフォーマット
			(int)params[4].number			//(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_LinearDataUnit://1画像の線形演算
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.LinearDataUnit(
			(int)params[0].number,			//(in)元画像のメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[1].number,			//(in)結果画像のメモリ番号(-1にした場合は現在選択中の画像)
			(int)params[2].number,			//(in)結果画像のフォーマット
			(double)params[3].number,		//(in)元画像に対する重み
			(double)params[4].number,		//(in)バイアス
			(int)params[5].number			//(in)データ上下限で飽和させるかどうか(falseの場合はオーバーフローする)
			);

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;


	case CALC_PrepeareDrawing://モノクロ画像をカラー画像にする
			if( ! API.PrepeareDrawing((int)params[0].number,		//(in)RGB画像のメモリ番号
									 (int)params[1].number			//(in)背景画像の画像メモリ番号
									) )
			{
				p_err_message->Format("Error: PrepeareDrawing()");
				return false;
			}

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawLineOnImage://画像上に直線描画
			API.DrawLineOnImage(	(int)params[0].number,			//(in)描画対象画像メモリ番号
										params[1].number,				//(in)始点座標x
										params[2].number,				//(in)始点座標y
										params[3].number,				//(in)終点座標x
										params[4].number,				//(in)終点座標y
										(int)params[5].number,			//(in)描画するペンの太さ
										RGB( (int)params[6].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[7].number,		//(in)描画色緑成分
											(int)params[8].number),		//(in)描画色青成分
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);

			break;

		case CALC_DrawLineOnImageF://画像上に直線描画
			API.DrawLineOnImageF(	(int)params[0].number,			//(in)描画対象画像メモリ番号
										params[1].number,				//(in)a
										params[2].number,				//(in)b
										params[3].number,				//(in)c
										(int)params[4].number,			//(in)描画するペンの太さ
										RGB( (int)params[5].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[6].number,		//(in)描画色緑成分
											(int)params[7].number),		//(in)描画色青成分
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawImageOnImage:
			API.DrawImageOnImage(	(int)params[0].number,			//(in)下地とする画像メモリ番号
									(int)params[1].number,			//(in)描画する画像データが入った画像メモリ番号
									(int)params[2].number,			//(in)描画オフセットx座標
									(int)params[3].number,			//(in)描画オフセットy座標
									(int)params[4].number			//(in)データの合成方法 1:max, 2:min, other:平均
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawCursorOnImage://画像上に十字カーソル描画
			API.DrawCursorOnImage(		(int)params[0].number,			//(in)描画対象画像メモリ番号
										params[1].number,				//(in)座標x
										params[2].number,				//(in)座標y
										(int)params[3].number,			//(in)描画するペンの太さ
										RGB( (int)params[4].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[5].number,		//(in)描画色緑成分
											(int)params[6].number),		//(in)描画色青成分
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawCircleOnImage://画像上に円描画
			API.DrawCircleOnImage(		(int)params[0].number,			//(in)描画対象画像メモリ番号
										params[1].number,				//(in)中心座標x
										params[2].number,				//(in)中心座標y
										params[3].number,				//(in)半径
										RGB( (int)params[4].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[5].number,		//(in)描画色緑成分
											(int)params[6].number),		//(in)描画色青成分
										(int)params[7].number
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawRectOnImage://画像上に矩形描画
			API.DrawRectOnImage(		(int)params[0].number,			//(in)描画対象画像メモリ番号
										params[1].number,				//(in)左上座標x
										params[2].number,				//(in)左上座標y
										params[3].number,				//(in)右下座標x
										params[4].number,				//(in)右下座標y
										RGB( (int)params[5].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[6].number,		//(in)描画色緑成分
											(int)params[7].number),		//(in)描画色青成分
										(bool)params[8].number,			//(in)中を塗りつぶすかどうか (0以外:塗りつぶす)
										RGB( (int)params[5].number,		
											(int)params[6].number,		
											(int)params[7].number),
											-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawPointOnImage://画像上にドット描画
			API.DrawPointOnImage(		(int)params[0].number,			//(in)描画対象画像メモリ番号
										(int)params[1].number,			//(in)座標x
										(int)params[2].number,			//(in)座標y
										(int)params[3].number,			//(in)ドットサイズ（直径）
										RGB( (int)params[4].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[5].number,		//(in)描画色緑成分
											(int)params[6].number),		//(in)描画色青成分
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawTextOnImage://画像上にASCIIテキスト描画
			API.DrawTextOnImage(		(int)params[0].number,			//(in)描画対象画像メモリ番号
										(int)params[1].number,			//(in)座標x
										(int)params[2].number,			//(in)座標y
										get_string_from_buffer(id, params[3].str_id) ,//(in)テキスト
										RGB( (int)params[4].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[5].number,		//(in)描画色緑成分
											(int)params[6].number),		//(in)描画色青成分
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_FloodFillOnImage://画像をぬりつぶす(指定された開始座標から)
			API.FloodFillOnImage(		(int)params[0].number,			//(in)描画対象画像メモリ番号
										(int)params[1].number,			//(in)塗りつぶし開始座標x
										(int)params[2].number,			//(in)塗りつぶし開始座標y
										RGB( (int)params[3].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[4].number,		//(in)描画色緑成分
											(int)params[5].number)		//(in)描画色青成分
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_FillWorkAreaOnImage://画像をぬりつぶす(作業領域内部)
			API.FillWorkAreaOnImage(	(int)params[0].number,			//(in)描画対象画像メモリ番号
										RGB( (int)params[1].number,		//(in)描画色赤成分（グレースケール画像の場合は、この値が採用される）
											(int)params[2].number,		//(in)描画色緑成分
											(int)params[3].number)		//(in)描画色青成分
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;


		case CALC_FillWorkAreaOnMask://画像をぬりつぶす(作業領域内部)
			API.FillWorkAreaOnMask((int)params[0].number,			//(in)描画対象画像メモリ番号
									(int)params[1].number);			//(in)マスク値

			if (autoDrawImage)	API.DrawImage((int)params[0].number);

			break;

		case CALC_DrawLine://GDIで直線描画
			API.DrawLine(				params[0].number,				//(in)始点座標x
										params[1].number,				//(in)始点座標y
										params[2].number,				//(in)終点座標x
										params[3].number,				//(in)終点座標y	
										(int)params[4].number,			//(in)太さ
										RGB( (int)params[5].number,		//(in)描画色赤成分
											(int)params[6].number,		//(in)描画色緑成分
											(int)params[7].number)		//(in)描画色青成分
									) ;
			break;

		case CALC_DrawCursor://GDIで十字カーソル描画
			API.DrawCursor(				params[0].number,				//(in)座標x
										params[1].number,				//(in)座標y
										(int)params[2].number,			//(in)太さ
										RGB( (int)params[3].number,		//(in)描画色赤成分
											(int)params[4].number,		//(in)描画色緑成分
											(int)params[5].number)		//(in)描画色青成分
									) ;
			break;

		case CALC_DrawRect://GDIで矩形描画
			API.DrawRect(				params[0].number,				//(in)左上x
										params[1].number,				//(in)左上y
										params[2].number,				//(in)右下x
										params[3].number,				//(in)右下y	
										(int)params[4].number,			//(in)太さ
										RGB( (int)params[5].number,		//(in)描画色赤成分
											(int)params[6].number,		//(in)描画色緑成分
											(int)params[7].number)		//(in)描画色青成分
									) ;
			break;

		case CALC_DrawCircle://GDIで矩形描画
			API.DrawCircle(				params[0].number,				//(in)中心座標x
										params[1].number,				//(in)中心座標y
										params[2].number,				//(in)半径
										(int)params[3].number,			//(in)太さ
										RGB( (int)params[4].number,		//(in)描画色赤成分
											(int)params[5].number,		//(in)描画色緑成分
											(int)params[6].number)		//(in)描画色青成分
									) ;
			break;

		case CALC_DrawEllipse://GDIで楕円描画
			API.DrawEllipse(			params[0].number,				//(in)中心座標x
										params[1].number,				//(in)中心座標y
										params[2].number,				//(in)x半径
										params[3].number,				//(in)y半径
										(int)params[4].number,			//(in)太さ
										RGB( (int)params[5].number,		//(in)描画色赤成分
											(int)params[6].number,		//(in)描画色緑成分
											(int)params[7].number)		//(in)描画色青成分
									) ;
			break;

		case CALC_DrawText://GDIでテキスト描画
			API.DrawText(				params[0].number,					//(in)座標x
										params[1].number,					//(in)座標y
										CString( get_string_from_buffer(id, params[2].str_id )),	//(in)テキスト
										RGB( (int)params[3].number,		//(in)描画色赤成分
											(int)params[4].number,		//(in)描画色緑成分
											(int)params[5].number),		//(in)描画色青成分
										(int)params[6].number			//(in)サイズ
									) ;
			break;


		case CALC_ClipSegment://矩形領域で線分を切り取る
			{
				double x0 = params[0].number;//線分始点X
				double y0 = params[1].number;//線分始点Y
				double x1 = params[2].number;//線分終点X
				double y1 = params[3].number;//線分終点Y

				CRect region;
				region.left = (int)params[4].number;//領域左上X
				region.top = (int)params[5].number;//領域左上Y
				region.right = (int)params[6].number;//領域右下X
				region.bottom = (int)params[7].number;//領域右下Y

				if (!API.ClipSegment(region, &x0, &y0, &x1, &y1)){
					p_err_message->Format("Error: ClipSegment()");
					return false;
				}

				//出力値
				switch ((int)params[8].number)
				{
					case 0:
						p_result->number = x0;//始点X
						break;
					case 1:
						p_result->number = y0;//始点Y
						break;
					case 2:
						p_result->number = x1;//終点X
						break;
					case 3:
						p_result->number = y1;//終点Y
						break;
					default:
						p_err_message->Format("Error: CALC_ClipSegment()");
						return false;
				}
			}
			break;


		case CALC_ClipSegmentF://矩形領域で線分を切り取る
		{
			CRect region;
			region.left = (int)params[3].number;//領域左上X
			region.top = (int)params[4].number;//領域左上Y
			region.right = (int)params[5].number;//領域右下X
			region.bottom = (int)params[6].number;//領域右下Y

			double x0, y0,x1,y1 ;
			if (!API.ClipSegmentF(region,
							params[0].number,	//a
							params[1].number,	//b
							params[2].number,	//c
							&x0, &y0, &x1, &y1))
			{
				p_err_message->Format("Error: ClipSegment()");
				return false;
			}

			//出力値
			switch ((int)params[7].number)
			{
			case 0:
				p_result->number = x0;//始点X
				break;
			case 1:
				p_result->number = y0;//始点Y
				break;
			case 2:
				p_result->number = x1;//終点X
				break;
			case 3:
				p_result->number = y1;//終点Y
				break;
			default:
				p_err_message->Format("Error: CALC_ClipSegmentF()");
				return false;
			}
		}
		break;


		case CALC_ShowMessage://メインウィンドウにメッセージを表示する

			API.ShowMessage((bool)params[0].number,				//(in)過去のメッセージをクリアするかどうか (0以外:クリア 0:過去のメッセージのあとに続けて表示)
					(int)params[1].number,					//(in)メッセージのタブ番号
					true,
					get_string_from_buffer(id, params[2].str_id)		//(in)表示文字列
				);

			break;

		case CALC_MessageBox://メッセージボックス表示
			API.MessageBox( get_string_from_buffer(id ,params[0].str_id ));
			break;


		case CALC_CopyToClipbord://表示画像をクリップボードにコピーする
			if( !API.CopyToClipbord((int)params[0].number) )
			{
				p_err_message->Format("Error: CopyToClipbord()");
				return false;
			}
			break;

		case CALC_PasteFromClipbord://クリップボードにある画像をメモリに入れる
			if( !API.PasteFromClipbord( (int)params[0].number) )
			{
				p_err_message->Format("Error: PasteFromClipbord()");
				return false;
			}
			break;

		case CALC_ToString://数値を文字列にする
			{
				int str_id = push_string_to_buffer(id, API.ToString( (float)params[0].number, (int)params[1].number, '0',0), p_err_message);
				if(str_id<0){
					return false;
				}
				p_result->type = COM_TYPE_STRING;
				p_result->str_id = str_id;
			}
			break;

		case CALC_ToNumber://文字列を数値にする
			if(!API.ToNumber( CString( get_string_from_buffer(id, params[0].str_id) ) ,  &p_result->number ) ){
				p_err_message->Format("Error: ToNumber()");
				return false;
			}
			break;

		case CALC_GetBatchInput://バッチ実行の入力パラメタを取得する
			if( p_calc_param->pbatchIn != NULL)
			{
				if(params[0].number>=0 && params[0].number<BATCH_IN_PARAM_NUM)
				{
					p_result->type = COM_TYPE_NUMBER;
					CString str = CString( p_calc_param->pbatchIn->param[(int)params[0].number] );
					API.ToNumber(str, &p_result->number);
				}else{
					p_err_message->Format("Error: 1st argument must be within 0-%d", BATCH_IN_PARAM_NUM-1);
					return false;
				}
			}else{
				return true;
			}
			break;

		case CALC_SetBatchInput://バッチ実行の入力パラメタを設定する(数値として)
			if (p_calc_param->pbatchIn != NULL)
			{
				if (params[0].number >= 0 && params[0].number<BATCH_IN_PARAM_NUM)
				{
					CString str = API.ToString(params[1].number);
					strcpy(p_calc_param->pbatchIn->param[(int)params[0].number], str.GetBuffer());
					str.ReleaseBuffer();
				}
				else{
					p_err_message->Format("Error: 1st argument must be within 0-%d", BATCH_IN_PARAM_NUM - 1);
					return false;
				}
			}
			else{
				return true;
			}
			break;

		case CALC_GetBatchOutput://バッチ実行の出力パラメタを取得する
			if (p_calc_param->pbatchOut != NULL)
			{
				if (params[0].number >= 0 && params[0].number<BATCH_OUT_PARAM_NUM)
				{
					p_result->type = COM_TYPE_NUMBER;
					CString str = CString(p_calc_param->pbatchOut->result[(int)params[0].number]);
					API.ToNumber(str, &p_result->number);
				}
				else{
					p_err_message->Format("Error: 1st argument must be within 0-%d", BATCH_OUT_PARAM_NUM - 1);
					return false;
				}
			}
			else{
				return true;
			}
			break;

		case CALC_SetBatchOutput://バッチ実行の出力パラメタを設定する(数値として)
			if( p_calc_param->pbatchOut != NULL)
			{
				if(params[0].number>=0 && params[0].number<BATCH_OUT_PARAM_NUM)
				{
					CString str = API.ToString(params[1].number);
					strcpy( p_calc_param->pbatchOut->result[(int)params[0].number] , str.GetBuffer());
					str.ReleaseBuffer();
				}else{
					p_err_message->Format("Error: 1st argument must be within 0-%d", BATCH_OUT_PARAM_NUM-1);
					return false;
				}
			}else{
				return true;
			}
			break;


		case CALC_UserDlgBatchExecute://カスタムファンクションの実行
			if (p_calc_param->pbatchIn != NULL && p_calc_param->pbatchOut != NULL)
			{
				if (params[0].number >= 0 && params[0].number < CUSTOM_FUNC_MAX)
				{
					if (!API.UserDialogBatchExecute((int)params[0].number,//カスタムファンクション番号
						p_calc_param->pbatchIn, p_calc_param->pbatchOut)
						)
					{
						p_err_message->Format("Error: UserDialogBatchExecute(%d)", (int)params[0].number);
						return false;
					}
				}
				else{
					p_err_message->Format("Error: 1st argument must be within 0-%d", CUSTOM_FUNC_MAX - 1);
					return false;
				}
			}
			else{
				return true;
			}
			break;

		case CALC_CaptureOne://画像をキャプチャする
			return API.CaptureOne();
			break;

		case CALC_SendString://TCP/IPからデータを送る
			{
				int ret = API.SendString(get_string_from_buffer(id, params[0].str_id));
				if (ret == 0){
					p_err_message->Format("Error: SendString()");
					return false;
				}
				else{
					p_result->number = ret;
				}
			}
			break;


		case CALC_OpenFileDialog://ファイルを開くダイアログボックスを出す
		{
			int fileNumber;
			CString filePath, fileName, ext;

			ext.Format("%s||", get_string_from_buffer(id, params[0].str_id));
			if (!API.OpenFileDialog(1,
									ext,
									&filePath, &fileName, &fileNumber
									))
			{
				p_err_message->Format("Error: OpenFileDialog()");
				return false;
			}
			else
			{
				int str_id = push_string_to_buffer(id, filePath + fileName, p_err_message);
				if (str_id<0){
					return false;
				}
				p_result->type = COM_TYPE_STRING;
				p_result->str_id = str_id;
			}
		}
		break;


		case CALC_FolderDialog://フォルダを指定するダイアログボックスを出す
		{
			CString folderPath;
			if (!API.FolderDialog(&folderPath))
			{
				p_err_message->Format("Error: FolderDialog()");
				return false;
			}
			else
			{
				int str_id = push_string_to_buffer(id, folderPath, p_err_message);
				if (str_id<0){
					return false;
				}
				p_result->type = COM_TYPE_STRING;
				p_result->str_id = str_id;
			}
		}
		break;


		case CALC_SelectAndLoadImageFile://ファイル名の拡張子からタイプを判断してファイルを読み込む
			if( !API.SelectAndLoadImageFile( (int)params[0].number,					//(in)ファイルを読み込む画像メモリ番号
											get_string_from_buffer(id, params[1].str_id )		//(in)ファイルのフルパス
								) )
			{
				p_err_message->Format("Error: SelectAndLoadImageFile()");
				return false;
			}
			API.DrawImage((int)params[0].number);
			break;


		case CALC_SelectAndSaveImageFile://ファイル名の拡張子からタイプを判断してファイルを読み込む
			if (!API.SelectAndSaveImageFile((int)params[0].number,					//(in)ファイルを読み込む画像メモリ番号
				get_string_from_buffer(id, params[1].str_id)		//(in)ファイルのフルパス
				))
			{
				p_err_message->Format("Error: SelectAndSaveImageFile()");
				return false;
			}
			API.DrawImage((int)params[0].number);
			break;


		case CALC_LoadBitmap://ビットマップファイルを読み込む
			if (!API.LoadBitmap((int)params[0].number,				//(in)ファイルを読み込む画像メモリ番号
								get_string_from_buffer(id, params[1].str_id),		//(in)ファイルのフルパス
								(int)params[2].number,				//(in)部分読み込み開始位置X
								(int)params[3].number,				//(in)部分読み込み開始位置Y
								(int)params[4].number,				//(in)部分読み込み幅
								(int)params[5].number				//(in)部分読み込み高さ
								))
			{
				p_err_message->Format("Error: LoadBitmap()");
				return false;
			}
			API.DrawImage((int)params[0].number);
			break;


		case CALC_SaveViewAsBitmap://表示データをビットマップとして保存する
			if( !API.SaveViewAsBitmap( (int)params[0].number,					//(in)保存する画像メモリ番号
										get_string_from_buffer(id, params[1].str_id )		//(in)ファイルのフルパス
								) )
			{
				p_err_message->Format("Error: SaveViewAsBitmap()");
				return false;
			}
			break;

		case CALC_SaveArchivedImage://*pim型ファイルを保存する
			if( !API.SaveArchivedImage( (int)params[0].number,					//(in)保存する画像メモリ番号
										get_string_from_buffer( id, params[1].str_id )		//(in)ファイルのフルパス
								) )
			{
				p_err_message->Format("Error: SaveArchivedImage()");
				return false;
			}
			break;

		case CALC_GetDefaultPath://アプリケーションの絶対パスを取得する
			{
				int str_id = push_string_to_buffer(id, API.GetDefaultPath(), p_err_message);
				if(str_id<0 )
				{
					return false;
				}
				p_result->type = COM_TYPE_STRING;
				p_result->str_id = str_id;
			}
			break;

		case CALC_CheckFileExist://ファイルの存在を確認する
			{
				p_result->type = COM_TYPE_NUMBER;
				if( PathFileExists( CString(get_string_from_buffer( id, params[0].str_id ))) ){
					p_result->number = 1;
				}else{
					p_result->number = 0;
				}
			}
			break;

		case CALC_EnumFiles://ファイルパスを列挙する
		{
			if (params[0].number < 0 || params[0].number >= CALC_FILE_FINDER_MAX){
				return false;
			}

			CString dirPath(get_string_from_buffer(id, params[1].str_id));
			CString fileExt(get_string_from_buffer(id, params[2].str_id));

			if (pFileFinder[(int)params[0].number] != NULL)
			{
				pFileFinder[(int)params[0].number]->Close();
				delete  pFileFinder[(int)params[0].number];
			}
			pFileFinder[(int)params[0].number] = new CFileFind();
			
			bool ret = pFileFinder[(int)params[0].number]->FindFile(dirPath + CString("\\*.") + fileExt);

			p_result->type = COM_TYPE_NUMBER;
			p_result->number = ret;
		}
		break;

		case CALC_GetNextFilePath://列挙されたファイルパスをひとつづつとりだす
		{
			if (params[0].number < 0 || params[0].number >= CALC_FILE_FINDER_MAX){
				return false;
			}

			if (pFileFinder[(int)params[0].number] == NULL){
				return false;
			}

			CString filepath = "";
			BOOL ff_ret = true;
			while (ff_ret)
			{
				ff_ret = pFileFinder[(int)params[0].number]->FindNextFile();

				if (pFileFinder[(int)params[0].number]->IsDots())	continue;
				if (pFileFinder[(int)params[0].number]->IsDirectory())	continue;
				
				filepath = pFileFinder[(int)params[0].number]->GetFilePath();
				break;
			}

			if (!ff_ret){
				pFileFinder[(int)params[0].number]->Close();
				delete pFileFinder[(int)params[0].number];
				pFileFinder[(int)params[0].number] = NULL;
			}


			int str_id = push_string_to_buffer(id, filepath, p_err_message);
			if (str_id<0)
			{
				return false;
			}
			p_result->type = COM_TYPE_STRING;
			p_result->str_id = str_id;
		}
		break;

		case CALC_AddDataToChart://１次元グラフにデータを追加する
			{
				int idx = API.AddDataToChart( 
									(int)params[0].number,		//(in)グラフ番号
									(float)params[1].number		//(in)値
								);
				if(idx<0)
				{
					p_err_message->Format("Error: AddDataToChart()");
					return false;
				}
				p_result->type = COM_TYPE_NUMBER;
				p_result->number = idx;
			}
			break;

		case CALC_AddDataToChart2://１次元グラフにデータを追加する
			{
				int idx = API.AddDataToChart2( 
									(int)params[0].number,		//(in)グラフ番号
									(float)params[1].number,		//(in)値
									(float)params[2].number
								);
				if(idx<0)
				{
					p_err_message->Format("Error: AddDataToChart2()");
					return false;
				}
				p_result->type = COM_TYPE_NUMBER;
				p_result->number = idx;
			}
			break;

		case CALC_ClearChart://グラフをクリアする
			API.ClearChart( (int)params[0].number );
			break;



		case CALC_MCR_RESULT:
			if( params[0].number >= 0 && params[0].number < GetCommandLineLen() )
			{
				int type;
				double number;
				char strbuffer[CALC_STRING_LEN_MAX];

				type = GetResult((int)params[0].number, &number, strbuffer, p_err_message);

				if(type == COM_TYPE_NUMBER)
				{//数値
					p_result->type = COM_TYPE_NUMBER;
					p_result->number = number;
				}
				else if(type == COM_TYPE_STRING)
				{//文字列
					int str_id = push_string_to_buffer(id, CString(strbuffer) , p_err_message);
					if( str_id<0){
						return false;
					}
					p_result->type = COM_TYPE_STRING;
					p_result->str_id = str_id;

				}else{
					p_err_message->Format("Error: line%d has not excuted yet", (int)params[0].number);
					return false;
				}
			}else{
				p_err_message->Format("Error: Res() argument must be within 0-%d", GetCommandLineLen()-1);
				return	false;
			}
			break;

		case CALC_MCR_CHANGE_RESULT:
			if( params[0].number >= 0 && params[0].number < GetCommandLineLen() )
			{
				if(pCalcDataStructMap[(int)params[0].number]==NULL)
				{
					p_err_message->Format("Error: line%d has not excuted yet", (int)params[0].number);
					return false;
				}
				else
				{
					pCalcDataStructMap[(int)params[0].number]->result.type = COM_TYPE_NUMBER;
					pCalcDataStructMap[(int)params[0].number]->result.number = params[1].number;
				}
			}else{
				p_err_message->Format("Error: Res() argument must be within 0-%d", GetCommandLineLen()-1);
				return	false;
			}
			break;

		case CALC_MCR_CREATE_DIRECTORY://ディレクトリ作成
			{
				char dirname[CALC_STRING_LEN_MAX]={0};
				strcpy(dirname, get_string_from_buffer(id,params[0].str_id) );

				if( PathIsDirectory(dirname) )
				{
					//同名のディレクトリが存在する場合は何もしない
				}else if( ::CreateDirectory( dirname , NULL) == 0)
				{
					//ディレクトリ作成失敗
					p_err_message->Format("Error: CreateDirectory(\"%s\")", dirname);
					return	false;
				}

				p_result->type = COM_TYPE_STRING;
				p_result->str_id = params[0].str_id;
			}
			break;


		case CALC_MCR_WRITE_TXT://テキストファイル出力
			{
				char filepath[CALC_STRING_LEN_MAX]={0};
				strcpy(filepath, get_string_from_buffer(id,params[0].str_id) );

				FILE *fp = fopen(filepath, "wt");
				if(!fp){
					p_err_message->Format("Error: WriteText(\"%s\")", filepath);
					return false;
				}
					
				fprintf( fp, "%s\n", get_string_from_buffer(id,params[1].str_id) );

				fclose(fp);

				p_result->type = COM_TYPE_STRING;
				p_result->str_id = params[1].str_id;
			}
			break;

		case CALC_MCR_APPEND_TXT://テキストファイル出力
			{
				char filepath[CALC_STRING_LEN_MAX]={0};
				strcpy(filepath, get_string_from_buffer(id,params[0].str_id) );

				FILE *fp = fopen(filepath, "at");
				if(!fp){
					p_err_message->Format("Error: AppendText(\"%s\")", filepath);
					return false;
				}
					
				fprintf( fp, "%s\n", get_string_from_buffer(id,params[1].str_id) );

				fclose(fp);

				p_result->type = COM_TYPE_STRING;
				p_result->str_id = params[1].str_id;
			}
			break;

		case CALC_STRLEN://文字列の長さ取得
		{
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = strlen(get_string_from_buffer(id, params[0].str_id));
		}
		break;

		case CALC_SUBSTR://部分文字列の取得
		{
			char *pStr = get_string_from_buffer(id, params[0].str_id);
			int start = params[1].number;
			int sublen = params[2].number;
			int len = (int)strlen(pStr);

			if (start < 0 || start >= len || sublen<1){
				return false;
			}

			if (start + sublen > len){
				sublen = len - start;
			}


			int str_id = push_string_to_buffer(id, CString(pStr).Mid(start, sublen), p_err_message);
			if (str_id<0){
				return false;
			}
			p_result->type = COM_TYPE_STRING;
			p_result->str_id = str_id;
		}
		break;

		case CALC_FINDSTR://文字列の探索
		{
			char *pStr = get_string_from_buffer(id, params[0].str_id);
			char *pTargetStr = get_string_from_buffer(id, params[1].str_id);

			if (strlen(pStr) < strlen(pTargetStr)){
				return false;
			}

			p_result->type = COM_TYPE_NUMBER;
			p_result->number = CString(pStr).Find(CString(pTargetStr));
		}
		break;

		case CALC_REPLACESTR://文字列の置換
		{
			char *pStr = get_string_from_buffer(id, params[0].str_id);
			char *pOldStr = get_string_from_buffer(id, params[1].str_id);
			char *pNewStr = get_string_from_buffer(id, params[2].str_id);

			if (strlen(pStr) < strlen(pOldStr) || strlen(pStr) < strlen(pNewStr)){
				return false;
			}

			CString repstr(pStr);
			repstr.Replace(CString(pOldStr), CString(pNewStr));

			int str_id = push_string_to_buffer(id, repstr, p_err_message);
			if (str_id<0){
				return false;
			}
			p_result->type = COM_TYPE_STRING;
			p_result->str_id = str_id;
		}
		break;

		case CALC_MCR_WAIT://処理を待機
		{
			Sleep((DWORD)params[0].number);
		}
		break;


		default:
			return false;
	}

	return true;
}




/********************************************************************
機  能  名  称 : 
関    数    名 : GetMacroDefinision
引          数 :
戻    り    値 : 成功した場合はtrue
機          能 : 
日付         作成者          内容
------------ --------------- --------------------------------------- 
             Y.Ikeda         新規作成
********************************************************************/
bool CCalculate::GetMacroDefinision(int id, MACRO_DEF_STRUCT *pMcr)
{
	if(id<0 || id>=CALC_MACRO_CNT_MAX)	return false;
	
	memcpy( pMcr, &pMacroDefinision[id] , sizeof(MACRO_DEF_STRUCT));

	return true;
}