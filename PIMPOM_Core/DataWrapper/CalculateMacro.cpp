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
//���Z���̃}�N�������̎���
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "Shlwapi.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
//�}�N���R�}���h��ID

//�v�Z��
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



//API�@�摜�f�[�^�A�N�Z�X
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

//API �摜����
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

//�摜�ԉ��Z
#define		CALC_Linear2DataUnit			350
#define		CALC_SumDataUnit				351
#define		CALC_SubtractDataUnit			352
#define		CALC_SubAbsDataUnit				353
#define		CALC_MultiplyDataUnit			354
#define		CALC_DivideDataUnit				355
#define		CALC_LinearDataUnit				356
#define		CALC_AbsDataUnit				357



//API �`��
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


//API ���C���E�B���h�E�@�\�̊��p
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

//API �t�@�C��IO
#define		CALC_SelectAndLoadImageFile		250
#define		CALC_SaveViewAsBitmap			251
#define		CALC_SaveArchivedImage			252
#define		CALC_GetDefaultPath				253
#define		CALC_CheckFileExist				254
#define		CALC_EnumFiles					255
#define		CALC_GetNextFilePath			256
#define		CALC_SelectAndSaveImageFile		257
#define		CALC_LoadBitmap					258

//API �O���t�\��
#define		CALC_AddDataToChart				260
#define		CALC_AddDataToChart2			261
#define		CALC_ClearChart					262

//�}�N�����L
#define		CALC_MCR_RESULT					280
#define		CALC_MCR_CHANGE_RESULT			281
#define		CALC_MCR_CREATE_DIRECTORY		284
#define		CALC_MCR_WRITE_TXT				290
#define		CALC_MCR_APPEND_TXT				291
#define		CALC_MCR_WAIT					292

//�����񑀍�
#define		CALC_STRLEN						300
#define		CALC_SUBSTR						301
#define		CALC_FINDSTR					302
#define		CALC_REPLACESTR					303


// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

// *************************************
//         ��    �I    ��    ��         
// *************************************

// *************************************
//         �O    ��    ��    ��         
// *************************************
extern	CPimpomAPI	API;

// *************************************
//         ��    ��    �Q    ��         
// *************************************




//�}�N�����`
void CCalculate::define_macros()
{
	for(int i=0 ; i<CALC_MACRO_CNT_MAX ; i++){
		memset(&pMacroDefinision[i], 0, sizeof(MACRO_DEF_STRUCT));
	}

	//���l�v�Z(0�`99)
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


	//PIMPOM API�̎��s(100�ȍ~)
	//�摜�f�[�^�A�N�Z�X
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

	//API �摜����
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

	//�摜�ԉ��Z
	define_macro(COM_TYPE_NUMBER, CALC_Linear2DataUnit, "Linear2DataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SumDataUnit, "SumDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SubtractDataUnit, "SubtractDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SubAbsDataUnit, "SubAbsDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MultiplyDataUnit, "MultiplyDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_DivideDataUnit, "DivideDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_LinearDataUnit, "LinearDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_AbsDataUnit, "AbsDataUnit", COM_TYPE_NUMBER, COM_TYPE_NUMBER);

	//API �`��
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


	//API ���C���E�B���h�E�@�\�̊��p
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
	

	//API �t�@�C��IO
	define_macro(COM_TYPE_NUMBER, CALC_SelectAndLoadImageFile, "SelectAndLoadImageFile", COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_SaveViewAsBitmap, "SaveViewAsBitmap", COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_SaveArchivedImage, "SaveArchivedImage", COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_GetDefaultPath, "GetDefaultPath");
	define_macro(COM_TYPE_NUMBER, CALC_CheckFileExist, "CheckFileExist", COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_EnumFiles, "EnumFiles", COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_GetNextFilePath, "GetNextFilePath", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_SelectAndSaveImageFile, "SelectAndSaveImageFile", COM_TYPE_NUMBER, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_LoadBitmap, "LoadBitmap", COM_TYPE_NUMBER, COM_TYPE_STRING, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);

	//API �O���t�\��
	define_macro(COM_TYPE_NUMBER, CALC_AddDataToChart, "AddDataToChart", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_AddDataToChart2, "AddDataToChart2", COM_TYPE_NUMBER, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_ClearChart, "ClearChart", COM_TYPE_NUMBER);

	//�}�N�����L
	define_macro(COM_TYPE_NUMBER, CALC_MCR_RESULT, "Res", COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_CHANGE_RESULT, "ChangeRes", COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_STRING, CALC_MCR_CREATE_DIRECTORY, "CreateDirectory", COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_MCR_WRITE_TXT, "WriteText", COM_TYPE_STRING, COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_MCR_APPEND_TXT, "AppendText", COM_TYPE_STRING, COM_TYPE_STRING);
	define_macro(COM_TYPE_NUMBER, CALC_MCR_WAIT, "Wait", COM_TYPE_NUMBER);

	//�����񑀍�
	define_macro(COM_TYPE_NUMBER, CALC_STRLEN, "StrLen", COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_SUBSTR, "Substr", COM_TYPE_STRING, COM_TYPE_NUMBER, COM_TYPE_NUMBER);
	define_macro(COM_TYPE_NUMBER, CALC_FINDSTR, "FindStr", COM_TYPE_STRING, COM_TYPE_STRING);
	define_macro(COM_TYPE_STRING, CALC_REPLACESTR, "ReplaceStr", COM_TYPE_STRING, COM_TYPE_STRING, COM_TYPE_STRING);
}


//�}�N���������`(�I�[�o�[���C�h�Ή�)
bool CCalculate::define_macro(
	int ret_type,
	int id, char name[],
	int argtype0, int argtype1, int argtype2, int argtype3, int argtype4,
	int argtype5, int argtype6, int argtype7, int argtype8, int argtype9,
	int argtype10, int argtype11, int argtype12, int argtype13, int argtype14)
{
	int overrideId = pMacroDefinision[id].overloadCnt;

	if (overrideId >= CALC_MACRO_OVERLOAD_MAX) {//�I�[�o�[���[�h�\���𒴂�����G���[�I��
		return false;
	}
	else if (overrideId == 0) {//�ŏ��ɒ�`����Ƃ��́A�}�N������ݒ�
		strcpy(pMacroDefinision[id].name, name);
	}
	else {//2��ڈȍ~�̒�`�i�I�[�o�[���[�h�j�̂Ƃ��́A�}�N�������ŏ��̒�`�Ɠ������Ƃ��m�F
		if (strcmp(pMacroDefinision[id].name, name) != 0) {//�قȂ��Ă���΃G���[�I��
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
�@  �\  ��  �� : �}�N���̏���������
��    ��    �� : execute_macro
��          �� :
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::execute_macro(
							  int					id,
							  CALC_PARAM_STRUCT		*p_calc_param,			//(in/out)�v�Z�̓��o�̓p�����^
							  MACRO_IO_STRUCT		*p_result,				//(out)����
							  int					macroID,				//(in)�}�N���ԍ�
							  int					overloadID,			//(in)�I�[�o�[���C�h�ԍ�
							  MACRO_IO_STRUCT		params[],				//(in)�}�N������
							 CString				*p_err_message,			//(out)�G���[���b�Z�[�W
							 bool					autoDrawImage			//(in)������ɉ摜�`������s����
							 )
{
	p_result->type = COM_TYPE_NUMBER;//�o�͂̏�����(���ɋK�肳��Ă��Ȃ��}�N���͐��l0��Ԃ�)
	p_result->number = 0;


	if(!p_calc_param->apiMode && macroID>=100){//�}�N�������s���Ȃ��ꍇ�͏������X�L�b�v����
		return true;
	}

	//�I�[�o�[���[�h�Ή�
	if (overloadID >= CALC_MACRO_OVERLOAD_MAX) {
		return false;
	}




	switch (macroID)
	{
		//macroNo 0�`99 ���l�v�Z����

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



		//macroNo 100�ȍ~�@API����

	case CALC_GetDataUnitNumber://�摜�������̑�����Ԃ�
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.GetDataUnitNumber();
		break;

	case CALC_GetCurrentImageNumber://���ݑI�𒆂̉摜�������̔ԍ���Ԃ�
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.GetCurrentImageNumber();
		break;

	case CALC_SetCurrentImageNumber://���ݑI�𒆂̉摜�������̔ԍ���ύX����
		if (!API.SetCurrentImageNumber((int)params[0].number)) {
			p_err_message->Format("Error: SetCurrentImageNumber()");
			return false;
		}
		break;

	case CALC_GetDataTypeSize://�摜�������̌^�E�T�C�Y�E���s���擾
	{
		int type, width, height, page;
		if (!API.GetDataTypeSize((int)params[0].number, &type, &width, &height, &page))
		{
			p_err_message->Format("Error: GetDataTypeSize()");
			return	false;
		}
		else {
			p_result->type = COM_TYPE_NUMBER;

			if (params[1].number == 0) {//�^�C�v
				p_result->number = type;
			}
			else if (params[1].number == 1) {//��
				p_result->number = width;
			}
			else if (params[1].number == 2) {//����
				p_result->number = height;
			}
			else if (params[1].number == 3) {//���s
				p_result->number = page;
			}
			else {
				p_err_message->Format("Error: 2nd argument must be within 0-3");
				return false;
			}
		}
	}
	break;



	case CALC_CopyDataUnit://�摜���������R�s�[����
		if (!API.CopyDataUnit((int)params[0].number, (int)params[1].number))
		{
			p_err_message->Format("Error: CopyDataUnit()");
			return	false;
		}
		if (autoDrawImage)	API.DrawImage((int)params[1].number);
		break;

	case CALC_DeleteDataUnit://�摜��������j������
		if (!API.DeleteDataUnit((int)params[0].number))
		{
			p_err_message->Format("Error: DeleteDataUnit()");
			return	false;
		}
		break;

	case CALC_BackupDataUnit://�摜���������o�b�N�A�b�v����
		if (!API.BackupDataUnit((int)params[0].number)) {
			p_err_message->Format("Error: BackupDataUnit()");
			return false;
		}
		break;

	case CALC_RecoverDataUnit://�o�b�N�A�b�v���ꂽ�摜�������𕜊�������
		if (!API.RecoverDataUnit((int)params[0].number)) {
			p_err_message->Format("Error: RecoverDataUnit()");
			return false;
		}
		break;

	case CALC_ConvertDataUnit://�摜�������̃^�C�v��ύX
		if (!API.ConvertDataUnit((int)params[0].number,		//�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,	//�ϊ���̉摜�������̌^  (0:BYTE 1:FLOAT 2:���f�� 3:3D 4:F3D 5:XY 6:XYZ 7:RGB 8:WORD 9:LONG32 10:RGB_3D 11:SHORT16 12:DWORD)
			(int)params[2].number,	//RGB�ɕϊ�����ꍇ�̑}���� �i1:R���� 2:G���� 3:B���� 1-3�ȊO:RGB�S�����j
			(int)params[3].number,	//���f���ɕϊ�����ꍇ�̑}����i0:�����@0�ȊO:�����j
			(bool)params[4].number,	//�^���̏㉺���𒴂���l��ϊ�����Ƃ��O�a�����邩�ǂ��� (0:���Ȃ� 0�ȊO:����)
			(int)params[5].number,	//BYTE�ɕϊ�����ꍇ�A�\��������or�}�X�N����ϊ����邩�ǂ��� �i0:�f�[�^���� 1:�\���摜���� 2:�}�X�N����j
			(bool)params[6].number	//RGB����O���[�ɕϊ�������@(1:�ʓx 2:�F��  �����ȊO:���x)
		))
		{
			p_err_message->Format("Error: ConvertDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_ConvertDispToByte://�摜��������BYTE�^�ɃR���o�[�g
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

	case CALC_FormatDataUnit://�摜�������t�H�[�}�b�g
	{
		if (API.FormatDataUnit((int)params[0].number,	//(in)�摜�������ԍ�
			(int)params[1].number,	//(in)�摜�������^�C�v
			CSize((int)params[2].number, (int)params[3].number),//�����E��
			(int)params[4].number//�y�[�W�ԍ�
			, true) == NULL)
		{
			return false;
		}
	}

	if (autoDrawImage)	API.DrawImage((int)params[0].number);

	break;

	case CALC_GetDataMinMax://�摜�������̍ő�E�ŏ��l�擾
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

	case CALC_SetDispPage://�\���y�[�W�ԍ��ݒ�
		if (!API.SetDispPage((int)params[0].number,		//(in)�摜�������ԍ�
			(int)params[1].number		//(in)�y�[�W�ԍ�
		))
		{
			p_err_message->Format("Error: SetDispPage()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_SetDispChannel://�\���`���l���ԍ��ݒ�
		if (!API.SetDispChannel((int)params[0].number,		//(in)�摜�������ԍ�
			(int)params[1].number		//(in)�`���l���ԍ�(RGB�\���̏ꍇ��-1)
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


	case CALC_DrawImage://�摜�`��
		if (overloadID == 0) 
		{
			API.DrawImage((int)params[0].number,					//(in)�摜�������ԍ�
				get_string_from_buffer(id, (int)params[1].str_id),	//(in)���O
				true,
				(bool)params[2].number					//(in)��ʃT�C�Y�ɂ��킹�邩�@(0�ȊO:���킹�� 0:���킹�Ȃ�)
			);
		}
		else if(overloadID==1)
		{
			API.DrawImage((int)params[0].number);					//(in)�摜�������ԍ�
		}
		break;

	case CALC_ScaleImage://�摜�`��
		if (!API.ScaleImage((int)params[0].number,					//(in)�摜�������ԍ�
			(float)params[1].number					//(in)�X�P�[��
		))
		{
			p_err_message->Format("Error: ScaleImage()");
			return false;
		}
		break;

	case CALC_SetDispRange://�\���Z�x�̃����W��ύX���ĕ`�悷��
		if (!API.SetDispRange((int)params[0].number,			//(in)�摜�������ԍ�
			(float)params[1].number,		//(in)�Z�x�ŏ��l
			(float)params[2].number			//(in)�Z�x�ő�l
		))
		{
			p_err_message->Format("Error: SetDispRange()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_SetDispRangeAuto://�\���Z�x�̃����W�������ŕύX���ĕ`�悷��
		if (!API.SetDispRangeAuto((int)params[0].number,		//(in)�摜�������ԍ�
			(int)params[1].number		//(in)�����ݒ���@  �i 0:�ő�/�ŏ� 1:���ϒl�}�W���΍��~2�j
		))
		{
			p_err_message->Format("Error: SetDispRangeAuto()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;

	case CALC_SetWorkArea://��Ɨ̈����`�ɐݒ�
		if (!API.SetWorkArea((int)params[0].number,			//(in)�摜�������ԍ�
			(int)params[1].number,			//(in)��Ɨ̈�̐}�`�ԍ�
			CRect((int)params[2].number,	//(in)��Ɨ̈捶��x
			(int)params[3].number,	//(in)��Ɨ̈捶��y
				(int)params[4].number,	//(in)��Ɨ̈�E��x
				(int)params[5].number),	//(in)��Ɨ̈�E��y
				(bool)params[6].number			//(in)OR/NOT�@�i0�ȊO:OR  0:NOT�j
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

	case CALC_GetWorkAreaVtx://��Ɨ̈�(��`)��4�����W���擾
	{
		FIGURE_UNION fig = { 0 };
		bool	ornot;
		if (API.GetWorkArea((int)params[0].number,			//(in)�摜�������ԍ�
			(int)params[1].number,			//(in)��Ɨ̈�̐}�`�ԍ�
			&fig,
			&ornot
		) != WORK_AREA_TYPE_RECT)
		{
			p_err_message->Format("Error: figure type is not RECT");
			return false;
		}
		else {
			p_result->type = COM_TYPE_NUMBER;

			if (params[2].number == 0) {//����x
				p_result->number = fig.rect.left;
			}
			else if (params[2].number == 1) {//����y
				p_result->number = fig.rect.top;
			}
			else if (params[2].number == 2) {//�E��x
				p_result->number = fig.rect.right;
			}
			else if (params[2].number == 3) {//�E��y
				p_result->number = fig.rect.bottom;
			}
			else {
				p_err_message->Format("Error: 3rd argument must be within 0-3");
				return false;
			}
		}
	}
	break;

	case CALC_GetWorkAreaOrnot://��Ɨ̈�(��`)��OR/NOT���擾
	{
		FIGURE_UNION fig = { 0 };
		bool	ornot;
		if (API.GetWorkArea((int)params[0].number,			//(in)�摜�������ԍ�
			(int)params[1].number,			//(in)��Ɨ̈�̐}�`�ԍ�
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

	case CALC_ChangePalette://�J���[�p���b�g��ύX����
	{
		if (!API.ChangePalette(
			(int)params[0].number,			//(in)�摜�������ԍ�)
			(int)params[1].number) )
		{
			p_err_message->Format("Error: ChangePalette()");
			return false;
		}
	}
	break;

	case CALC_AddMark://�}�[�N��ǉ�����
	{
		int mark_number = API.AddMark(
			(int)params[0].number,			//(in)�摜�������ԍ�
			(float)params[1].number,		//(in)x
			(float)params[2].number			//(in)y
		);
		if (mark_number == -1) {
			p_err_message->Format("Error: AddMark()");
		}
		p_result->number = mark_number;
	}
	break;

	case CALC_DeleteMark://�}�[�N��ǉ�����
		if (!API.DeleteMark((int)params[0].number,		//(in)�摜�������ԍ�
			(int)params[1].number		//(in)�}�[�N�ԍ�
		))
		{
			p_err_message->Format("Error: DeleteMark()");
			return false;
		}
		break;

	case CALC_GetMarkVal://�}�[�N�̒l���擾����
	{
		float x, y;
		double val;
		if (!API.GetMarkVal((int)params[0].number,		//(in)�摜�������ԍ�
			(int)params[1].number,		//(in)�}�[�N�ԍ�
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

	case CALC_GetMarkNumber://�}�[�N�̐����擾����
	{
		int mark_number = API.GetMarkNumber((int)params[0].number);
		if (mark_number == -1) {
			p_err_message->Format("Error: GetMarkNumber()");
			return false;
		}
		p_result->number = mark_number;
	}
	break;


	case CALC_ClearMark://���ׂẴ}�[�N���폜����
	{
		if (!API.ClearMark((int)params[0].number)){
			p_err_message->Format("Error: ClearMark()");
			return false;
		}
	}
	break;


	case CALC_AddLineMark://���}�[�N��ǉ�����
	{
		int mark_number = API.AddLineMark(
			(int)params[0].number,			//(in)�摜�������ԍ�
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


	case CALC_ResizeDataUnit://�摜�̃T�C�Y��ύX
		if (!API.ResizeDataUnit((int)params[0].number,	//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,	//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			true,
			CSize((int)params[2].number,		//(in)�ϊ���̕�
			(int)params[3].number),	//(in)�ϊ���̍���
				(bool)params[4].number,			//(in)�ϊ���̃T�C�Y�ɍ��킹�ĉ摜���g��/�k��(0�ȊO�F����  0:���Ȃ�)
			(int)params[5].number,				//(in)�I�t�Z�b�gx
			(int)params[6].number,				//(in)�I�t�Z�b�gy
			false,
			(int)params[7].number				//(in)�摜�k���^�C�v(0:area integration 1:max 2:min)
		))
		{
			p_err_message->Format("Error: ResizeDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_ClipDataUnit://�摜���N���b�s���O
		if (!API.ClipDataUnit((int)params[0].number,	 //(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,	 //(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			true,
			CRect((int)params[2].number,	//(in)�N���b�v�̈捶�[
			(int)params[3].number,	//(in)�N���b�v�̈��[
				(int)params[4].number,	//(in)�N���b�v�̈�E�[
				(int)params[5].number)	//(in)�N���b�v�̈扺�[
		))
		{
			p_err_message->Format("Error: ClipDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_PerspectiveTransDataUnit://�p�[�X�y�N�e�B�u�ϊ�
		if (!API.PerspectiveTransDataUnit((int)params[0].number,		//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,		//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			true,
			params[2].number,		//(in)�摜�����s�ړ�
			params[3].number,		//(in)�摜�����s�ړ�
			params[4].number,		//(in)�B���ʓ����s�ړ�
			params[5].number,		//(in)�B���ʓ����s�ړ�
			params[6].number,		//(in)�B���ʓ���] [deg]
			params[7].number,		//(in)�J�����������X��[deg]
			params[8].number,		//(in)�J�����c�����X��[deg]
			params[9].number,		//(in)�����Y�œ_����[mm]
			params[10].number,		//(in)ccd1��f�T�C�Y[��m]
			false,
			NULL,
			1,
			(int)params[11].number		//(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
		))
		{
			p_err_message->Format("Error: PerspectiveTransDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_IsoTransDataUnit://���s�ړ��A��]�ړ��A�g��
		if (!API.IsoTransDataUnit((int)params[0].number,		//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,		//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			true,
			params[2].number,		//(in)���s�ړ�tx
			params[3].number,		//(in)���s�ړ�ty
			params[4].number,		//(in)�]�ړ�[deg]
			params[5].number,		//(in)�g�嗦
			false,
			NULL,
			(int)params[6].number		//(in)�摜����͂ݏo���������̏����̕��@(0:���̂܂� 1:�ŋߖT��f�̒l�ŕ��)
		))
		{
			p_err_message->Format("Error: IsoTarnsDataUnit()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_CalcDataValue://�C�ӂ̐����v�Z
		if(overloadID==0)
		{
			if (!API.CalcDataValue( (int)params[0].number,	//(in)�`��Ώۉ摜�������ԍ�
									(int)params[1].number,	//(in)�쐬����摜�̃^�C�v
									CSize( (int)params[2].number,  //(in)�쐬����摜�̕�
									       (int)params[3].number),  //(in)�쐬����摜�̍���
									get_string_from_buffer(id, params[4].str_id),//(in)�e�L�X�g
									(bool)params[5].number,//(in)�O�a���邩�ǂ���
									p_err_message))
			{
				return false;
			}
		}
		else if (overloadID == 1)
		{
			if (!API.CalcDataValue((int)params[0].number,	//(in)�`��Ώۉ摜�������ԍ�
				get_string_from_buffer(id, params[1].str_id),//(in)�e�L�X�g
				(bool)params[2].number,//(in)�O�a���邩�ǂ���
				p_err_message))
			{
				return false;
			}
		}

		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;


	case CALC_CalcMaskValue:
		if (!API.CalcMaskValue((int)params[0].number,	//(in)�`��Ώۉ摜�������ԍ�
			get_string_from_buffer(id, params[1].str_id),//(in)�e�L�X�g
			(bool)params[2].number,//(in)�}�X�NOFF�Ƃ���ő�l
			(bool)params[3].number,//(in)�}�X�NOFF�Ƃ���ŏ��l
			p_err_message))
		{
			return false;
		}
		if (autoDrawImage)	API.DrawImage((int)params[0].number);

		break;


	case CALC_FilterSmooth://�������i�ړ����ρj
		if (!API.FilterSmooth((int)params[0].number,				//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,				//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			CSize((int)params[2].number,		//(in)�t�B���^��
			(int)params[3].number),				//(in)�t�B���^����
			(bool)params[4].number,				//(in)�㉺���l�������i0�ȊO�F���� 0:���Ȃ��j
			(bool)params[5].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterSmooth()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterGauss://�������i�K�E�V�A���j
		if (!API.FilterGauss((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y�i���������j
			(int)params[3].number,			//(in)�t�B���^���� �@�i1:���̂� 2:�c�̂�  ����ȊO:�����j
			(int)params[4].number,			//(in)padding  (1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
			(bool)params[5].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterGauss()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterGaussUnshade://�K�E�V�A�����ȍ����ɂ��V�F�[�f�B���O����
		if (!API.FilterGaussUnshade((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y�i���������j
			(float)params[3].number,		//(in)�I�t�Z�b�g
			(float)params[4].number,		//(in)�R���g���X�g
			(int)params[5].number,			//(in)�t�B���^���� �@�i1:���̂� 2:�c�̂�  ����ȊO:�����j
			(int)params[6].number,			//(in)padding  (1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
			(bool)params[7].number			//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterGaussUnshade()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterMedian://���f�B�A���t�B���^
		if (!API.FilterMedian((int)params[0].number,				//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,				//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			CSize((int)params[2].number,		//(in)�t�B���^��
			(int)params[3].number),				//(in)�t�B���^����
			(bool)params[4].number,				//(in)�~�`�}�X�N(0�ȊO�F�g�� 0:�g��Ȃ�)
			(bool)params[5].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterMedian()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterErosion://���k�t�B���^
		if (!API.FilterErosion((int)params[0].number,				//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,				//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			CSize((int)params[2].number,		//(in)�t�B���^��
			(int)params[3].number),				//(in)�t�B���^����
			(bool)params[4].number,				//(in)�~�`�}�X�N(0�ȊO�F�g�� 0:�g��Ȃ�)
			(bool)params[5].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterErosion()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterDilation://�c���t�B���^
		if (!API.FilterDilation((int)params[0].number,				//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,				//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			CSize((int)params[2].number,		//(in)�t�B���^��
			(int)params[3].number),				//(in)�t�B���^����
			(bool)params[4].number,				//(in)�~�`�}�X�N(0�ȊO�F�g�� 0:�g��Ȃ�)
			(bool)params[5].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterDilation()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterOpening://�I�[�v�j���O�t�B���^
		if (!API.FilterOpening((int)params[0].number,				//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,				//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			CSize((int)params[2].number,		//(in)�t�B���^��
			(int)params[3].number),				//(in)�t�B���^����
			(bool)params[4].number,				//(in)�~�`�}�X�N(0�ȊO�F�g�� 0:�g��Ȃ�)
			(bool)params[5].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterOpening()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterClosing://�N���[�W���O�t�B���^
		if (!API.FilterClosing((int)params[0].number,				//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,				//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			CSize((int)params[2].number,		//(in)�t�B���^��
			(int)params[3].number),				//(in)�t�B���^����
			(bool)params[4].number,				//(in)�~�`�}�X�N(0�ȊO�F�g�� 0:�g��Ȃ�)
			(bool)params[5].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterClosing()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterEdgeSobel://�\�[�x���t�B���^�ɂ��G�b�W���o
		if (!API.FilterEdgeSobel((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y ( 3: 3x3,  5:5x5,  7:7x7,  9:9x9 )
			(float)params[3].number,		//(in)�G�b�W���x�ŏ��l�i���̒l��菬�����ꍇ��0�����I��0�ɂ���j
			(int)params[4].number,			//(in)�t�B���^���� �i0:���̂� 1:�c�̂�  ����ȊO:�����j
			(bool)params[5].number			//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterEdgeSobel()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;


	case CALC_FilterEdgeCanny://Canny�t�B���^�ɂ��G�b�W���o
		if (!API.FilterEdgeCanny(
			(int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y ( 3: 3x3,  5:5x5,  7:7x7)
			(float)params[3].number,		//(in)�G�b�W���x臒l1
			(float)params[4].number,			//(in)�G�b�W���x臒l2
			(bool)params[5].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterEdgeCanny()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterBilateral://�o�C���e�����t�B���^
		if (!API.FilterBilateral(
			(int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y ( 3: 3x3,  5:5x5,  7:7x7)
			(float)params[3].number,		//(in)�l�̃�
			(float)params[4].number,		//(in)�����̃�
			(bool)params[5].number			//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterBilateral()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterLaplaceGauss://���v���V�A���I�u�K�E�V�A���t�B���^
		if (!API.FilterLaplaceGauss((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y
			(int)params[3].number,			//(in)�t�B���^���� �i0:���̂� 1:�c�̂�  ����ȊO:�����j
			(float)params[4].number,		//(in�I�t�Z�b�g
			(float)params[5].number,		//(in)�R���g���X�g
			(int)params[6].number,			//(in)padding  (1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
			(bool)params[7].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterLaplaceGauss()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterGabor://�K�{�[���t�B���^
		if (!API.FilterGabor((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y
			(float)params[3].number,		//(in)���F�J�[�l���G����
			(float)params[4].number,		//(in)�J�[�l�����ł̎���
			(float)params[5].number,		//(in)�ƁF�J�[�l����]�p�x
			(float)params[6].number,		//(in)���F�J�[�l�������ʑ�
			(float)params[7].number,		//(in�I�t�Z�b�g
			(float)params[8].number,		//(in)�R���g���X�g
			(int)params[9].number,			//(in)padding  (1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
			(bool)params[10].number			//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterGabor()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterEdgeSigmoid://sigmoid�t�B���^
		if (!API.FilterEdgeSigmoid((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y
			(float)params[3].number,		//(in)k
			(float)params[4].number,		//(in)�ƁF�J�[�l����]�p�x
			(float)params[5].number,		//(in�I�t�Z�b�g
			(float)params[6].number,		//(in)�R���g���X�g
			(int)params[7].number,			//(in)padding  (1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
			(bool)params[8].number				//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterEdgeSigmoid()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;


	case CALC_FilterEdgeLog://���v���V�A���I�u�K�E�V�A���ɂ��G�b�W���o
		if (!API.FilterEdgeLog((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�t�B���^�T�C�Y
			(float)params[3].number,		//(in)�G�b�W���x�ŏ��l�i���̒l��菬�����ꍇ��0�����I��0�ɂ���j
			(int)params[4].number,			//(in)�t�B���^���� �i0:���̂� 1:�c�̂�  ����ȊO:�����j
			(int)params[5].number,			//(in)padding  (1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
			(bool)params[6].number			//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterEdgeLog()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterMirror://����
		if (!API.FilterMirror((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)�������]�̕��� ( 0:��  1:�c 2:180��)
			(bool)params[3].number			//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterMirror()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterNega://�Z�x���]
		if (!API.FilterNega((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(bool)params[2].number			//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
		))
		{
			p_err_message->Format("Error: FilterNega()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterCLAHE://CLAHE
		if (!API.FilterCLAHE((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,							//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(double)params[2].number,						//(in)�x������l
			(int)params[3].number,							//(in)������������
			(int)params[4].number,							//(in)�c����������
			(bool)params[5].number							//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
			))
		{
			p_err_message->Format("Error: FilterCLAHE()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterLocalContrast:
		if (!API.FilterLocalContrast((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,							//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,							//(in)�t�B���^�T�C�Y
			(float)params[3].number,							//(in)�R���g���X�g�g�嗦
			(bool)params[4].number							//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
			))
		{
			p_err_message->Format("Error: FilterLocalContrast()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_FilterLocalContrastPatch:
		if (!API.FilterLocalContrastPatch((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,							//(in)�i�[��摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,							//(in)�p�b�`������
			(bool)params[4].number							//(in)�����̃y�[�W�^�`���l���ɓ������������邩�ǂ���
			))
		{
			p_err_message->Format("Error: FilterLocalContrastPatch()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_Binalize://�O���[�o��臒l�ɂ��2�l��
		if (!API.Binalize((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)2�l����̉摜�����郁����
			(float)params[2].number,		//(in)�ŏ�臒l
			(float)params[3].number,		//(in)�ő�臒l
			(bool)params[4].number			//(in)�Q�l���](0�ȊO:����  0:���Ȃ�)
		))
		{
			p_err_message->Format("Error: Binalize()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_BinalizeThresholdOtsu://��Â̎�@��2�l����臒l�����߂�
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.BinalizeThresholdOtsu((int)params[0].number);
		break;

	case CALC_BinalizeThresholdPTile://�p�[�Z���^�C���@��臒l�����߂�
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.BinalizeThresholdPTile((int)params[0].number, (int)params[1].number);
		break;

	case CALC_BinalizeLocalGauss://���摜�ƃK�E�V�A���t�B���^��摜�̍����ɂ�郍�[�J��2�l��
		if (!API.BinalizeLocalGauss((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)2�l����̉摜�����郁����
			(int)params[2].number,			//(in)�t�B���^�T�C�Y�i���������j
			(float)params[3].number,		//(in)�I�t�Z�b�g
			(bool)params[4].number,			//(in)�Q�l���](0�ȊO:����  0:���Ȃ�)
			255,
			0,
			(int)params[5].number			//(in)padding  (1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
		))
		{
			p_err_message->Format("Error: BinalizeLocalGauss()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_BinalizeLocalLaplaceGauss://���v���V�A���I�u�K�E�V�A���t�B���^�ɂ��2�l��
		if (!API.BinalizeLocalLaplaceGauss((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)2�l����̉摜�����郁����
			(int)params[2].number,			//(in)�t�B���^�T�C�Y�i���������j
			(float)params[3].number,		//(in)�I�t�Z�b�g
			(bool)params[4].number,			//(in)�Q�l���](0�ȊO:����  0:���Ȃ�)
			255,
			0,
			(int)params[5].number			//(in)padding  (1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
		))
		{
			p_err_message->Format("Error: BinalizeLocalLaplaceGauss()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_BinalizeNick://NICK��2�l��
		if (!API.BinalizeNick((int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)2�l����̉摜�����郁����
			(int)params[2].number,			//(in)�t�B���^�T�C�Y�i���������j
			(float)params[3].number,		//(in)Nick��k
			(bool)params[4].number			//(in)�Q�l���](0�ȊO:����  0:���Ȃ�)
		))
		{
			p_err_message->Format("Error: BinalizeNick()");
			return false;
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;

	case CALC_Label://2�l���摜�����x�����O����
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.Label((int)params[0].number,	//(in)2�l�摜�������ԍ�(���̉摜�̒l�́A0/255�݂̂ō\������Ă���)
			(int)params[1].number,			//(in)���x���摜�����郁����
			(bool)params[2].number,			//(in)�ʐύ~���Ń\�[�g���邩�ǂ���(0�ȊO:����  0:���Ȃ�)
			(int)params[3].number			//(in)�ŏ��ʐ�
		);

		{
			CDataUnit *pdu;
			if ((pdu = API.GetDataUnit((int)params[1].number))) {
				pdu->ChangePalette(PALETTE_LABEL);//���x�����킩��₷���悤�ȃJ���[�p���b�g�ɕύX
			}
		}

		if (autoDrawImage)	API.DrawImage((int)params[1].number);
			
		break;

	case CALC_HoughLines://Hough�ϊ��ɂ�钼�����o
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = API.HoughLines( 
										(int)params[0].number,			//(in)�Ώۉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
										(int)params[1].number,			//(in)���[��Ԃ̋�������\
										(int)params[2].number,			//(in)���[��Ԃ̊p�x����\
										(int)params[3].number,			//(in)���[��臒l�p�����[�^�D�\���ȕ[�𓾂������݂̂��o�͂���܂��D
										params[4].number,				//(in)�ŏ��̐������D������Z�������͊��p����܂�
										params[5].number				//(in)2�_�����������ɂ���ƌ��Ȃ��ꍇ�ɋ��e�����ő勗���D
								 );

			if (autoDrawImage)	API.DrawImage((int)params[1].number);
			
			break;


	case CALC_BlendPoisson://�|���\���摜����
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = API.BlendPoisson(
										(int)params[0].number,			//(in)���摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
										(int)params[1].number,			//(in)�u�����h�摜�������ԍ�
										(int)params[2].number,			//(in)���ʉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
										(int)params[3].number,			//(in)�����J�n�ʒux
										(int)params[4].number,			//(in)�����J�n�ʒuy
										(int)params[5].number			//(in)�������@ 0:FOREGROUND_GRADIENTS  1:AVERAGED_GRADIENTS  2:MIXED_GRADIENTS
				);

			if (autoDrawImage)	API.DrawImage((int)params[2].number);

			break;

	case CALC_DFT:
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = API.FilterDFT(
				(int)params[0].number,			//(in)���摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				(int)params[1].number,			//(in)���ʉ摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				(int)params[2].number			//(in)true�ŋtDFT
				);

			API.DrawImage((int)params[1].number);
			break;

	case CALC_Linear2DataUnit://2�摜�̐��`���Z���s��
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.Linear2DataUnit(
			(int)params[0].number,			//(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[3].number,			//(in)���ʉ摜�̃t�H�[�}�b�g
			(double)params[4].number,		//(in)���摜A�ɂ�����d��
			(double)params[5].number,		//(in)���摜B�ɂ�����d��
			(double)params[6].number,		//(in)�o�C�A�X
			(int)params[7].number			//(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_SumDataUnit://2�摜�̘a
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.SumDataUnit(
			(int)params[0].number,			//(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[3].number,			//(in)���ʉ摜�̃t�H�[�}�b�g
			(int)params[4].number			//(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_SubtractDataUnit://2�摜�̍�
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.SubtractDataUnit(
			(int)params[0].number,			//(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[3].number,			//(in)���ʉ摜�̃t�H�[�}�b�g
			(int)params[4].number			//(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_SubAbsDataUnit://2�摜�̍��̐�Βl
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.SubAbsDataUnit(
			(int)params[0].number,			//(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[3].number,			//(in)���ʉ摜�̃t�H�[�}�b�g
			(int)params[4].number			//(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_MultiplyDataUnit://2�摜�̐�
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.MultiplyDataUnit(
			(int)params[0].number,			//(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[3].number,			//(in)���ʉ摜�̃t�H�[�}�b�g
			(int)params[4].number			//(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_DivideDataUnit://2�摜�̏�
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.DivideDataUnit(
			(int)params[0].number,			//(in)���摜A�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)���摜B�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[3].number,			//(in)���ʉ摜�̃t�H�[�}�b�g
			(int)params[4].number			//(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
			);

		if (autoDrawImage)	API.DrawImage((int)params[2].number);

		break;


	case CALC_LinearDataUnit://1�摜�̐��`���Z
		p_result->type = COM_TYPE_NUMBER;
		p_result->number = API.LinearDataUnit(
			(int)params[0].number,			//(in)���摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[1].number,			//(in)���ʉ摜�̃������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
			(int)params[2].number,			//(in)���ʉ摜�̃t�H�[�}�b�g
			(double)params[3].number,		//(in)���摜�ɑ΂���d��
			(double)params[4].number,		//(in)�o�C�A�X
			(int)params[5].number			//(in)�f�[�^�㉺���ŖO�a�����邩�ǂ���(false�̏ꍇ�̓I�[�o�[�t���[����)
			);

		if (autoDrawImage)	API.DrawImage((int)params[1].number);

		break;


	case CALC_PrepeareDrawing://���m�N���摜���J���[�摜�ɂ���
			if( ! API.PrepeareDrawing((int)params[0].number,		//(in)RGB�摜�̃������ԍ�
									 (int)params[1].number			//(in)�w�i�摜�̉摜�������ԍ�
									) )
			{
				p_err_message->Format("Error: PrepeareDrawing()");
				return false;
			}

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawLineOnImage://�摜��ɒ����`��
			API.DrawLineOnImage(	(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										params[1].number,				//(in)�n�_���Wx
										params[2].number,				//(in)�n�_���Wy
										params[3].number,				//(in)�I�_���Wx
										params[4].number,				//(in)�I�_���Wy
										(int)params[5].number,			//(in)�`�悷��y���̑���
										RGB( (int)params[6].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[7].number,		//(in)�`��F�ΐ���
											(int)params[8].number),		//(in)�`��F����
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);

			break;

		case CALC_DrawLineOnImageF://�摜��ɒ����`��
			API.DrawLineOnImageF(	(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										params[1].number,				//(in)a
										params[2].number,				//(in)b
										params[3].number,				//(in)c
										(int)params[4].number,			//(in)�`�悷��y���̑���
										RGB( (int)params[5].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[6].number,		//(in)�`��F�ΐ���
											(int)params[7].number),		//(in)�`��F����
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawImageOnImage:
			API.DrawImageOnImage(	(int)params[0].number,			//(in)���n�Ƃ���摜�������ԍ�
									(int)params[1].number,			//(in)�`�悷��摜�f�[�^���������摜�������ԍ�
									(int)params[2].number,			//(in)�`��I�t�Z�b�gx���W
									(int)params[3].number,			//(in)�`��I�t�Z�b�gy���W
									(int)params[4].number			//(in)�f�[�^�̍������@ 1:max, 2:min, other:����
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawCursorOnImage://�摜��ɏ\���J�[�\���`��
			API.DrawCursorOnImage(		(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										params[1].number,				//(in)���Wx
										params[2].number,				//(in)���Wy
										(int)params[3].number,			//(in)�`�悷��y���̑���
										RGB( (int)params[4].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[5].number,		//(in)�`��F�ΐ���
											(int)params[6].number),		//(in)�`��F����
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawCircleOnImage://�摜��ɉ~�`��
			API.DrawCircleOnImage(		(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										params[1].number,				//(in)���S���Wx
										params[2].number,				//(in)���S���Wy
										params[3].number,				//(in)���a
										RGB( (int)params[4].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[5].number,		//(in)�`��F�ΐ���
											(int)params[6].number),		//(in)�`��F����
										(int)params[7].number
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawRectOnImage://�摜��ɋ�`�`��
			API.DrawRectOnImage(		(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										params[1].number,				//(in)������Wx
										params[2].number,				//(in)������Wy
										params[3].number,				//(in)�E�����Wx
										params[4].number,				//(in)�E�����Wy
										RGB( (int)params[5].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[6].number,		//(in)�`��F�ΐ���
											(int)params[7].number),		//(in)�`��F����
										(bool)params[8].number,			//(in)����h��Ԃ����ǂ��� (0�ȊO:�h��Ԃ�)
										RGB( (int)params[5].number,		
											(int)params[6].number,		
											(int)params[7].number),
											-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawPointOnImage://�摜��Ƀh�b�g�`��
			API.DrawPointOnImage(		(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										(int)params[1].number,			//(in)���Wx
										(int)params[2].number,			//(in)���Wy
										(int)params[3].number,			//(in)�h�b�g�T�C�Y�i���a�j
										RGB( (int)params[4].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[5].number,		//(in)�`��F�ΐ���
											(int)params[6].number),		//(in)�`��F����
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_DrawTextOnImage://�摜���ASCII�e�L�X�g�`��
			API.DrawTextOnImage(		(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										(int)params[1].number,			//(in)���Wx
										(int)params[2].number,			//(in)���Wy
										get_string_from_buffer(id, params[3].str_id) ,//(in)�e�L�X�g
										RGB( (int)params[4].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[5].number,		//(in)�`��F�ΐ���
											(int)params[6].number),		//(in)�`��F����
										-1
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_FloodFillOnImage://�摜���ʂ�Ԃ�(�w�肳�ꂽ�J�n���W����)
			API.FloodFillOnImage(		(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										(int)params[1].number,			//(in)�h��Ԃ��J�n���Wx
										(int)params[2].number,			//(in)�h��Ԃ��J�n���Wy
										RGB( (int)params[3].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[4].number,		//(in)�`��F�ΐ���
											(int)params[5].number)		//(in)�`��F����
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;

		case CALC_FillWorkAreaOnImage://�摜���ʂ�Ԃ�(��Ɨ̈����)
			API.FillWorkAreaOnImage(	(int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
										RGB( (int)params[1].number,		//(in)�`��F�Ԑ����i�O���[�X�P�[���摜�̏ꍇ�́A���̒l���̗p�����j
											(int)params[2].number,		//(in)�`��F�ΐ���
											(int)params[3].number)		//(in)�`��F����
									) ;

			if (autoDrawImage)	API.DrawImage((int)params[0].number);
			
			break;


		case CALC_FillWorkAreaOnMask://�摜���ʂ�Ԃ�(��Ɨ̈����)
			API.FillWorkAreaOnMask((int)params[0].number,			//(in)�`��Ώۉ摜�������ԍ�
									(int)params[1].number);			//(in)�}�X�N�l

			if (autoDrawImage)	API.DrawImage((int)params[0].number);

			break;

		case CALC_DrawLine://GDI�Œ����`��
			API.DrawLine(				params[0].number,				//(in)�n�_���Wx
										params[1].number,				//(in)�n�_���Wy
										params[2].number,				//(in)�I�_���Wx
										params[3].number,				//(in)�I�_���Wy	
										(int)params[4].number,			//(in)����
										RGB( (int)params[5].number,		//(in)�`��F�Ԑ���
											(int)params[6].number,		//(in)�`��F�ΐ���
											(int)params[7].number)		//(in)�`��F����
									) ;
			break;

		case CALC_DrawCursor://GDI�ŏ\���J�[�\���`��
			API.DrawCursor(				params[0].number,				//(in)���Wx
										params[1].number,				//(in)���Wy
										(int)params[2].number,			//(in)����
										RGB( (int)params[3].number,		//(in)�`��F�Ԑ���
											(int)params[4].number,		//(in)�`��F�ΐ���
											(int)params[5].number)		//(in)�`��F����
									) ;
			break;

		case CALC_DrawRect://GDI�ŋ�`�`��
			API.DrawRect(				params[0].number,				//(in)����x
										params[1].number,				//(in)����y
										params[2].number,				//(in)�E��x
										params[3].number,				//(in)�E��y	
										(int)params[4].number,			//(in)����
										RGB( (int)params[5].number,		//(in)�`��F�Ԑ���
											(int)params[6].number,		//(in)�`��F�ΐ���
											(int)params[7].number)		//(in)�`��F����
									) ;
			break;

		case CALC_DrawCircle://GDI�ŋ�`�`��
			API.DrawCircle(				params[0].number,				//(in)���S���Wx
										params[1].number,				//(in)���S���Wy
										params[2].number,				//(in)���a
										(int)params[3].number,			//(in)����
										RGB( (int)params[4].number,		//(in)�`��F�Ԑ���
											(int)params[5].number,		//(in)�`��F�ΐ���
											(int)params[6].number)		//(in)�`��F����
									) ;
			break;

		case CALC_DrawEllipse://GDI�őȉ~�`��
			API.DrawEllipse(			params[0].number,				//(in)���S���Wx
										params[1].number,				//(in)���S���Wy
										params[2].number,				//(in)x���a
										params[3].number,				//(in)y���a
										(int)params[4].number,			//(in)����
										RGB( (int)params[5].number,		//(in)�`��F�Ԑ���
											(int)params[6].number,		//(in)�`��F�ΐ���
											(int)params[7].number)		//(in)�`��F����
									) ;
			break;

		case CALC_DrawText://GDI�Ńe�L�X�g�`��
			API.DrawText(				params[0].number,					//(in)���Wx
										params[1].number,					//(in)���Wy
										CString( get_string_from_buffer(id, params[2].str_id )),	//(in)�e�L�X�g
										RGB( (int)params[3].number,		//(in)�`��F�Ԑ���
											(int)params[4].number,		//(in)�`��F�ΐ���
											(int)params[5].number),		//(in)�`��F����
										(int)params[6].number			//(in)�T�C�Y
									) ;
			break;


		case CALC_ClipSegment://��`�̈�Ő�����؂���
			{
				double x0 = params[0].number;//�����n�_X
				double y0 = params[1].number;//�����n�_Y
				double x1 = params[2].number;//�����I�_X
				double y1 = params[3].number;//�����I�_Y

				CRect region;
				region.left = (int)params[4].number;//�̈捶��X
				region.top = (int)params[5].number;//�̈捶��Y
				region.right = (int)params[6].number;//�̈�E��X
				region.bottom = (int)params[7].number;//�̈�E��Y

				if (!API.ClipSegment(region, &x0, &y0, &x1, &y1)){
					p_err_message->Format("Error: ClipSegment()");
					return false;
				}

				//�o�͒l
				switch ((int)params[8].number)
				{
					case 0:
						p_result->number = x0;//�n�_X
						break;
					case 1:
						p_result->number = y0;//�n�_Y
						break;
					case 2:
						p_result->number = x1;//�I�_X
						break;
					case 3:
						p_result->number = y1;//�I�_Y
						break;
					default:
						p_err_message->Format("Error: CALC_ClipSegment()");
						return false;
				}
			}
			break;


		case CALC_ClipSegmentF://��`�̈�Ő�����؂���
		{
			CRect region;
			region.left = (int)params[3].number;//�̈捶��X
			region.top = (int)params[4].number;//�̈捶��Y
			region.right = (int)params[5].number;//�̈�E��X
			region.bottom = (int)params[6].number;//�̈�E��Y

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

			//�o�͒l
			switch ((int)params[7].number)
			{
			case 0:
				p_result->number = x0;//�n�_X
				break;
			case 1:
				p_result->number = y0;//�n�_Y
				break;
			case 2:
				p_result->number = x1;//�I�_X
				break;
			case 3:
				p_result->number = y1;//�I�_Y
				break;
			default:
				p_err_message->Format("Error: CALC_ClipSegmentF()");
				return false;
			}
		}
		break;


		case CALC_ShowMessage://���C���E�B���h�E�Ƀ��b�Z�[�W��\������

			API.ShowMessage((bool)params[0].number,				//(in)�ߋ��̃��b�Z�[�W���N���A���邩�ǂ��� (0�ȊO:�N���A 0:�ߋ��̃��b�Z�[�W�̂��Ƃɑ����ĕ\��)
					(int)params[1].number,					//(in)���b�Z�[�W�̃^�u�ԍ�
					true,
					get_string_from_buffer(id, params[2].str_id)		//(in)�\��������
				);

			break;

		case CALC_MessageBox://���b�Z�[�W�{�b�N�X�\��
			API.MessageBox( get_string_from_buffer(id ,params[0].str_id ));
			break;


		case CALC_CopyToClipbord://�\���摜���N���b�v�{�[�h�ɃR�s�[����
			if( !API.CopyToClipbord((int)params[0].number) )
			{
				p_err_message->Format("Error: CopyToClipbord()");
				return false;
			}
			break;

		case CALC_PasteFromClipbord://�N���b�v�{�[�h�ɂ���摜���������ɓ����
			if( !API.PasteFromClipbord( (int)params[0].number) )
			{
				p_err_message->Format("Error: PasteFromClipbord()");
				return false;
			}
			break;

		case CALC_ToString://���l�𕶎���ɂ���
			{
				int str_id = push_string_to_buffer(id, API.ToString( (float)params[0].number, (int)params[1].number, '0',0), p_err_message);
				if(str_id<0){
					return false;
				}
				p_result->type = COM_TYPE_STRING;
				p_result->str_id = str_id;
			}
			break;

		case CALC_ToNumber://������𐔒l�ɂ���
			if(!API.ToNumber( CString( get_string_from_buffer(id, params[0].str_id) ) ,  &p_result->number ) ){
				p_err_message->Format("Error: ToNumber()");
				return false;
			}
			break;

		case CALC_GetBatchInput://�o�b�`���s�̓��̓p�����^���擾����
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

		case CALC_SetBatchInput://�o�b�`���s�̓��̓p�����^��ݒ肷��(���l�Ƃ���)
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

		case CALC_GetBatchOutput://�o�b�`���s�̏o�̓p�����^���擾����
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

		case CALC_SetBatchOutput://�o�b�`���s�̏o�̓p�����^��ݒ肷��(���l�Ƃ���)
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


		case CALC_UserDlgBatchExecute://�J�X�^���t�@���N�V�����̎��s
			if (p_calc_param->pbatchIn != NULL && p_calc_param->pbatchOut != NULL)
			{
				if (params[0].number >= 0 && params[0].number < CUSTOM_FUNC_MAX)
				{
					if (!API.UserDialogBatchExecute((int)params[0].number,//�J�X�^���t�@���N�V�����ԍ�
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

		case CALC_CaptureOne://�摜���L���v�`������
			return API.CaptureOne();
			break;

		case CALC_SendString://TCP/IP����f�[�^�𑗂�
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


		case CALC_OpenFileDialog://�t�@�C�����J���_�C�A���O�{�b�N�X���o��
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


		case CALC_FolderDialog://�t�H���_���w�肷��_�C�A���O�{�b�N�X���o��
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


		case CALC_SelectAndLoadImageFile://�t�@�C�����̊g���q����^�C�v�𔻒f���ăt�@�C����ǂݍ���
			if( !API.SelectAndLoadImageFile( (int)params[0].number,					//(in)�t�@�C����ǂݍ��މ摜�������ԍ�
											get_string_from_buffer(id, params[1].str_id )		//(in)�t�@�C���̃t���p�X
								) )
			{
				p_err_message->Format("Error: SelectAndLoadImageFile()");
				return false;
			}
			API.DrawImage((int)params[0].number);
			break;


		case CALC_SelectAndSaveImageFile://�t�@�C�����̊g���q����^�C�v�𔻒f���ăt�@�C����ǂݍ���
			if (!API.SelectAndSaveImageFile((int)params[0].number,					//(in)�t�@�C����ǂݍ��މ摜�������ԍ�
				get_string_from_buffer(id, params[1].str_id)		//(in)�t�@�C���̃t���p�X
				))
			{
				p_err_message->Format("Error: SelectAndSaveImageFile()");
				return false;
			}
			API.DrawImage((int)params[0].number);
			break;


		case CALC_LoadBitmap://�r�b�g�}�b�v�t�@�C����ǂݍ���
			if (!API.LoadBitmap((int)params[0].number,				//(in)�t�@�C����ǂݍ��މ摜�������ԍ�
								get_string_from_buffer(id, params[1].str_id),		//(in)�t�@�C���̃t���p�X
								(int)params[2].number,				//(in)�����ǂݍ��݊J�n�ʒuX
								(int)params[3].number,				//(in)�����ǂݍ��݊J�n�ʒuY
								(int)params[4].number,				//(in)�����ǂݍ��ݕ�
								(int)params[5].number				//(in)�����ǂݍ��ݍ���
								))
			{
				p_err_message->Format("Error: LoadBitmap()");
				return false;
			}
			API.DrawImage((int)params[0].number);
			break;


		case CALC_SaveViewAsBitmap://�\���f�[�^���r�b�g�}�b�v�Ƃ��ĕۑ�����
			if( !API.SaveViewAsBitmap( (int)params[0].number,					//(in)�ۑ�����摜�������ԍ�
										get_string_from_buffer(id, params[1].str_id )		//(in)�t�@�C���̃t���p�X
								) )
			{
				p_err_message->Format("Error: SaveViewAsBitmap()");
				return false;
			}
			break;

		case CALC_SaveArchivedImage://*pim�^�t�@�C����ۑ�����
			if( !API.SaveArchivedImage( (int)params[0].number,					//(in)�ۑ�����摜�������ԍ�
										get_string_from_buffer( id, params[1].str_id )		//(in)�t�@�C���̃t���p�X
								) )
			{
				p_err_message->Format("Error: SaveArchivedImage()");
				return false;
			}
			break;

		case CALC_GetDefaultPath://�A�v���P�[�V�����̐�΃p�X���擾����
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

		case CALC_CheckFileExist://�t�@�C���̑��݂��m�F����
			{
				p_result->type = COM_TYPE_NUMBER;
				if( PathFileExists( CString(get_string_from_buffer( id, params[0].str_id ))) ){
					p_result->number = 1;
				}else{
					p_result->number = 0;
				}
			}
			break;

		case CALC_EnumFiles://�t�@�C���p�X��񋓂���
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

		case CALC_GetNextFilePath://�񋓂��ꂽ�t�@�C���p�X���ЂƂÂƂ肾��
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

		case CALC_AddDataToChart://�P�����O���t�Ƀf�[�^��ǉ�����
			{
				int idx = API.AddDataToChart( 
									(int)params[0].number,		//(in)�O���t�ԍ�
									(float)params[1].number		//(in)�l
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

		case CALC_AddDataToChart2://�P�����O���t�Ƀf�[�^��ǉ�����
			{
				int idx = API.AddDataToChart2( 
									(int)params[0].number,		//(in)�O���t�ԍ�
									(float)params[1].number,		//(in)�l
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

		case CALC_ClearChart://�O���t���N���A����
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
				{//���l
					p_result->type = COM_TYPE_NUMBER;
					p_result->number = number;
				}
				else if(type == COM_TYPE_STRING)
				{//������
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

		case CALC_MCR_CREATE_DIRECTORY://�f�B���N�g���쐬
			{
				char dirname[CALC_STRING_LEN_MAX]={0};
				strcpy(dirname, get_string_from_buffer(id,params[0].str_id) );

				if( PathIsDirectory(dirname) )
				{
					//�����̃f�B���N�g�������݂���ꍇ�͉������Ȃ�
				}else if( ::CreateDirectory( dirname , NULL) == 0)
				{
					//�f�B���N�g���쐬���s
					p_err_message->Format("Error: CreateDirectory(\"%s\")", dirname);
					return	false;
				}

				p_result->type = COM_TYPE_STRING;
				p_result->str_id = params[0].str_id;
			}
			break;


		case CALC_MCR_WRITE_TXT://�e�L�X�g�t�@�C���o��
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

		case CALC_MCR_APPEND_TXT://�e�L�X�g�t�@�C���o��
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

		case CALC_STRLEN://������̒����擾
		{
			p_result->type = COM_TYPE_NUMBER;
			p_result->number = strlen(get_string_from_buffer(id, params[0].str_id));
		}
		break;

		case CALC_SUBSTR://����������̎擾
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

		case CALC_FINDSTR://������̒T��
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

		case CALC_REPLACESTR://������̒u��
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

		case CALC_MCR_WAIT://������ҋ@
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
�@  �\  ��  �� : 
��    ��    �� : GetMacroDefinision
��          �� :
��    ��    �l : ���������ꍇ��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::GetMacroDefinision(int id, MACRO_DEF_STRUCT *pMcr)
{
	if(id<0 || id>=CALC_MACRO_CNT_MAX)	return false;
	
	memcpy( pMcr, &pMacroDefinision[id] , sizeof(MACRO_DEF_STRUCT));

	return true;
}