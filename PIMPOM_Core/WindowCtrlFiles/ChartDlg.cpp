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
//CChartDlg
//�O���t�\���E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "ChartDlg.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************

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


/********************************************************************
�@  �\  ��  �� : �~��`�悷��
��    ��    �� : draw_point
��          �� : CDC *p_dc		(in)�f�o�C�X�R���e�L�X�g
				 int x,y		(in)�~���S���W[pix]
				 int r			(in)���a[pix]
				 COLORREF col	(in)�`��F
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
static void draw_point(CDC *p_dc, int x, int y, int r, COLORREF col)
{
	CPen pen, *poldpen;

	pen.CreatePen(PS_SOLID , 2 , col);
	poldpen = (CPen*)p_dc->SelectObject(&pen);
	p_dc->Ellipse(x-r,y-r,x+r,y+r);
	p_dc->SelectObject(poldpen);
}

/********************************************************************
�@  �\  ��  �� : ����`�悷��
��    ��    �� : draw_point
��          �� : CDC *p_dc		(in)�f�o�C�X�R���e�L�X�g
				 int x0,y0		(in)�n�_���W[pix]
				 int x1,y1		(in)�I�_���W[pix]
				 COLORREF col	(in)�`��F
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
static void draw_line(CDC *p_dc, int x0, int y0, int x1, int y1, COLORREF col)
{
	CPen pen, *poldpen;

	pen.CreatePen(PS_SOLID , 1 , col);
	poldpen = (CPen*)p_dc->SelectObject(&pen);
	p_dc->MoveTo(x0,y0);
	p_dc->LineTo(x1,y1);
	p_dc->SelectObject(poldpen);
}

/********************************************************************
�@  �\  ��  �� : �q�X�g�O�����̃o�[��`�悷��
��    ��    �� : draw_hist_bar
��          �� : CDC *p_dc		(in)�f�o�C�X�R���e�L�X�g
				 int x0,y0		(in)�n�_���W[pix]
				 int x1,y1		(in)�I�_���W[pix]
				 int hist		(in)�x��
				 COLORREF col	(in)�`��F
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
static void draw_hist_bar(CDC *p_dc, int x0, int y0, int x1, int y1, int hist, COLORREF col)
{
	CPen	pen, *poldpen;
	CBrush	burush, *poldbrush;
	CFont	font, *poldfont;
	CString str;

	pen.CreatePen(PS_SOLID, 1, RGB(255,255,255));
	burush.CreateSolidBrush(col);
	poldpen = (CPen*)p_dc->SelectObject(&pen);
	poldbrush = (CBrush*)p_dc->SelectObject(&burush);
	p_dc->Rectangle(x0,y0,x1,y1);
	p_dc->SelectObject(poldbrush);
	p_dc->SelectObject(poldpen);

	if(hist>0){
		font.CreateFontIndirect(&API.default_font);
		poldfont = (CFont*)p_dc->SelectObject(&font);
		str.Format(_T("%d"), hist);
		p_dc->TextOut(x0, y1-9, str);
		p_dc->SelectObject(poldfont);
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �f�[�^�z��i�P�����j���O���t�Ƀv���b�g����
��    ��    �� : template <class DATA> plot_data_to_chart
��          �� : CChartDlg		*pChartDlg		(in)
				 int			chart_id		(in)�O���t�ԍ�
				 int			data_num		(in)�f�[�^��
				 DATA			*pValueArr		(in)�f�[�^�z��
				 bool			doClear			(in)���ɃO���t�Ƀv���b�g����Ă���f�[�^������ꍇ�̋���
													true�F���݂���f�[�^���N���A���Ă���v���b�g
													false:���݂���f�[�^�̌�ɒǉ�����
				 COLORREF col	(in)�`��F
��    ��    �l : �@�����s���̏ꍇ��false���A��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	plot_data_to_chart(CChartDlg *pChartDlg, int chart_id, int data_num, DATA *pValueArr, bool doClear)
{
	if(doClear)	pChartDlg->ClearChart(chart_id);

	for(int i=0 ; i<data_num ; i++){
		if( pChartDlg->AddDataToChart(chart_id, (float)pValueArr[i], false)<0)	break;
	}

	pChartDlg->SetGraphMode(chart_id,false);
	pChartDlg->DrawChart(chart_id);	
	return true;
}

/********************************************************************
�@  �\  ��  �� : �f�[�^�z��i�Q�����j���O���t�Ƀv���b�g����
��    ��    �� : template <class DATA> plot_data_to_chart2
��          �� : CChartDlg		*pChartDlg					(in)
				 int			chart_id					(in)�O���t�ԍ�
				 int			data_num					(in)�f�[�^��
				 DATA			*pValueArrX,*pValueArrY		(in)�f�[�^�z��
				 bool			doClear						(in)���ɃO���t�Ƀv���b�g����Ă���f�[�^������ꍇ�̋���
															true�F���݂���f�[�^���N���A���Ă���v���b�g
															false:���݂���f�[�^�̌�ɒǉ�����
				 COLORREF col	(in)�`��F
��    ��    �l : �@�����s���̏ꍇ��false���A��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	plot_data_to_chart2(CChartDlg *pChartDlg, int chart_id, int data_num, DATA *pValueArrX,  DATA *pValueArrY, bool doClear)
{
	if(doClear)	pChartDlg->ClearChart(chart_id);

	for(int i=0 ; i<data_num ; i++){
		if(pChartDlg->AddDataToChart(chart_id, (float)pValueArrX[i], (float)pValueArrY[i], false)<0)	break;
	}

	pChartDlg->SetGraphMode(chart_id,0);
	pChartDlg->DrawChart(chart_id);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CChartDlg, CDialog)

CChartDlg::CChartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartDlg::IDD, pParent)
{
	for(int i=0 ; i<CHART_NUM ; i++){
		pChartDataMap[i] = NULL;
	}
}

CChartDlg::~CChartDlg()
{
	//�O���t�p�������̈�̉��
	for(int i=0 ; i<CHART_NUM ; i++){
		if(pChartDataMap[i]){
			delete pChartDataMap[i];
			pChartDataMap[i] = NULL;
		}
	}
}

void CChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHART, m_list_ctrl);
	DDX_Control(pDX, IDC_COMBO_CHART_DLG_TYPE, m_chart_type);
}


/********************************************************************
�@  �\  ��  �� : ���݃O���t�Ƀv���b�g����Ă���f�[�^���N���A
��    ��    �� : ClearChart
��          �� : int chart_id (in)�O���t�ԍ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::ClearChart(int chart_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return;

	if(pChartDataMap[chart_id]){
		delete	pChartDataMap[chart_id];
		pChartDataMap[chart_id] = NULL;
	}
}


/********************************************************************
�@  �\  ��  �� : ���݃O���t�Ƀv���b�g����Ă���f�[�^�����擾
��    ��    �� : GetDataNum
��          �� : int chart_id (in)�O���t�ԍ�
��    ��    �l : �f�[�^��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CChartDlg::GetDataCount(int chart_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return 0;

	if(!pChartDataMap[chart_id])	return 0;

	return pChartDataMap[chart_id]->dataCount;
}

/********************************************************************
�@  �\  ��  �� : ���݃O���t�Ƀv���b�g����Ă���f�[�^�̃^�C�v�擾
��    ��    �� : GetDataNum
��          �� : int chart_id (in)�O���t�ԍ�
��    ��    �l : 0:1�����O���t�@1:2�����O���t  �f�[�^�Ȃ��̂Ƃ���0
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CChartDlg::GetDataType(int chart_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return 0;

	if(!pChartDataMap[chart_id])	return 0;

	return pChartDataMap[chart_id]->dataType;
}

/********************************************************************
�@  �\  ��  �� : ���݃O���t�̕\�����[�h�擾
��    ��    �� : GetGraphMode
��          �� : int chart_id (in)�O���t�ԍ�
��    ��    �l : 0:�f�[�^  1:�q�X�g�O����  2:�X�y�N�g����
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CChartDlg::GetGraphMode(int chart_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return false;

	if(!pChartDataMap[chart_id])	return false;

	return pChartDataMap[chart_id]->graphMode;
}


void CChartDlg::SetGraphMode(int chart_id, int mode)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return;

	if(!pChartDataMap[chart_id])	return;

	pChartDataMap[chart_id]->graphMode = mode;
}



/********************************************************************
�@  �\  ��  �� : ���݃O���t�Ƀv���b�g����Ă���f�[�^���擾
��    ��    �� : GetDataNum
��          �� : int chart_id	(in)�O���t�ԍ�
				 int data_id	(in)�f�[�^�ԍ�
��    ��    �l : �f�[�^��Ԃ��B�@�����s���̏ꍇ��0���A��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
float CChartDlg::GetDataFromChart(int chart_id, int data_id)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return 0;

	if(!pChartDataMap[chart_id])	return 0;

	if(data_id<0 || data_id >= pChartDataMap[chart_id]->dataCount)	return 0;

	return  pChartDataMap[chart_id]->data[data_id];
}



/********************************************************************
�@  �\  ��  �� : ���݃O���t�Ƀv���b�g����Ă���f�[�^��ݒ�
��    ��    �� : SetDataToChart
��          �� : int	chart_id	(in)�O���t�ԍ�
				 int	data_id		(in)�f�[�^�ԍ�
				 float	value		(in)�ݒ�l
��    ��    �l : �@�����s���̏ꍇ��false���A��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CChartDlg::SetDataToChart(int chart_id, int data_id, float value)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return false;

	if(!pChartDataMap[chart_id])	return false;

	if(data_id<0 || data_id>=CHART_DATA_NUM)	return false;

	pChartDataMap[chart_id]->data[data_id] = value;

	return true;
}

/********************************************************************
�@  �\  ��  �� : �O���t�Ƀv���b�g����f�[�^��ǉ�����(1����)
��    ��    �� : AddDataToChart
��          �� : int	chart_id	(in)�O���t�ԍ�
				 float	value		(in)�ݒ�l
				 bool	do_redraw	(in)�O���t�ĕ`�悷��ꍇ��true
��    ��    �l : �@������s�ł�����ǉ������f�[�^�ԍ���Ԃ��B���s�����畉����Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int  CChartDlg::AddDataToChart(int chart_id, float value, bool do_redraw)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return -1;

	if( GetDataType(chart_id) != CHART_TYPE_1D  ){//���ł�1D�ȊO�̃�����������΍폜
		ClearChart(chart_id);
	}

	if(!pChartDataMap[chart_id])//���������Ȃ���ΐV�K�쐬
	{
		pChartDataMap[chart_id] = new CHART_DATA_STRUCT;
		if(!pChartDataMap[chart_id])	return -1;

		memset(pChartDataMap[chart_id], 0, sizeof(CHART_DATA_STRUCT) );//������
		pChartDataMap[chart_id]->dataType = CHART_TYPE_1D;
	}
	else
	{
		if( pChartDataMap[chart_id]->dataCount + 1 >= CHART_DATA_NUM)	return -1;//����ȏ�f�[�^�������Ȃ�
	}

	//�f�[�^������
	int idx = pChartDataMap[chart_id]->dataCount;
	if(!SetDataToChart(chart_id, idx, value)) return -1;

	//�f�[�^�����J�E���g�A�b�v
	pChartDataMap[chart_id]->dataCount++;

	//�O���t�`��
	if(do_redraw)  DrawChart(chart_id);

	return idx;
}

/********************************************************************
�@  �\  ��  �� : �O���t�Ƀv���b�g����f�[�^��ǉ�����(2����)
��    ��    �� : AddDataToChart
��          �� : int	chart_id		(in)�O���t�ԍ�
				 float	valueX,	valueY	(in)�ݒ�l
				 bool	do_redraw		(in)�O���t�ĕ`�悷��ꍇ��true
��    ��    �l : �@������s�ł�����ǉ������f�[�^�ԍ���Ԃ��B���s�����畉����Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CChartDlg::AddDataToChart(int chart_id, float valueX, float valueY, bool do_redraw)
{
	if(chart_id<0 || chart_id>=CHART_NUM)	return -1;

	if( GetDataType(chart_id) != CHART_TYPE_2D  ){//���ł�1D�ȊO�̃�����������΍폜
		ClearChart(chart_id);
	}

	if(!pChartDataMap[chart_id])//���������Ȃ���ΐV�K�쐬
	{
		pChartDataMap[chart_id] = new CHART_DATA_STRUCT;
		if(!pChartDataMap[chart_id])	return -1;

		memset(pChartDataMap[chart_id], 0, sizeof(CHART_DATA_STRUCT) );//������
		pChartDataMap[chart_id]->dataType = CHART_TYPE_2D;
	}
	else
	{
		if( pChartDataMap[chart_id]->dataCount + 2 >= CHART_DATA_NUM)	return -1;//����ȏ�f�[�^�������Ȃ�
	}

	//�f�[�^������
	int idx = pChartDataMap[chart_id]->dataCount;
	if(!SetDataToChart(chart_id, idx, valueX)) return -1;
	if(!SetDataToChart(chart_id, idx+1, valueY)) return -1;

	//�f�[�^�����J�E���g�A�b�v
	pChartDataMap[chart_id]->dataCount+=2;

	//�O���t�`��
	if(do_redraw)	DrawChart(chart_id);

	return idx;
}

/********************************************************************
�@  �\  ��  �� : �f�[�^�z��i�P�����j���O���t�Ƀv���b�g����
��    ��    �� : PlotDataToChart
��          �� :  int			chart_id		(in)�O���t�ԍ�
				 int			data_num		(in)�f�[�^��
				 DATA			*pValueArrX		(in)�f�[�^�z��
				 bool			doClear			(in)���ɃO���t�Ƀv���b�g����Ă���f�[�^������ꍇ�̋���
												true�F���݂���f�[�^���N���A���Ă���v���b�g
												false:���݂���f�[�^�̌�ɒǉ�����
��    ��    �l : �@�����s���̏ꍇ��false���A��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CChartDlg::PlotDataToChart(int chart_id, int data_num, float *pValueArr, bool doClear)
{
	return plot_data_to_chart(this, chart_id, data_num, pValueArr, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, double *pValueArr, bool doClear)
{
	return plot_data_to_chart(this, chart_id, data_num, pValueArr, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, int *pValueArr, bool doClear)
{
	return plot_data_to_chart(this, chart_id, data_num, pValueArr, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, BYTE *pValueArr, bool doClear)
{
	return plot_data_to_chart(this, chart_id, data_num, pValueArr, doClear);
}


/********************************************************************
�@  �\  ��  �� : �f�[�^�z��i2�����j���O���t�Ƀv���b�g����
��    ��    �� : PlotDataToChart
��          �� :  int			chart_id					(in)�O���t�ԍ�
				 int			data_num					(in)�f�[�^��
				 DATA			*pValueArrX,*pValueArrY		(in)�f�[�^�z��
				 bool			doClear						(in)���ɃO���t�Ƀv���b�g����Ă���f�[�^������ꍇ�̋���
															true�F���݂���f�[�^���N���A���Ă���v���b�g
															false:���݂���f�[�^�̌�ɒǉ�����
��    ��    �l : �@�����s���̏ꍇ��false���A��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CChartDlg::PlotDataToChart(int chart_id, int data_num, float *pValueArrX, float *pValueArrY, bool doClear)
{
	return plot_data_to_chart2(this, chart_id, data_num, pValueArrX, pValueArrY, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, double *pValueArrX, double *pValueArrY, bool doClear)
{
	return plot_data_to_chart2(this, chart_id, data_num, pValueArrX, pValueArrY, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, int *pValueArrX, int *pValueArrY, bool doClear)
{
	return plot_data_to_chart2(this, chart_id, data_num, pValueArrX, pValueArrY, doClear);
}

bool CChartDlg::PlotDataToChart(int chart_id, int data_num, BYTE *pValueArrX, BYTE *pValueArrY, bool doClear)
{
	return plot_data_to_chart2(this, chart_id, data_num, pValueArrX, pValueArrY, doClear);
}


/********************************************************************
�@  �\  ��  �� : �O���t���d���\�����鏇�Ԃ����߂�
��    ��    �� : calc_overlay_order
��          �� : int overRayOrder[]		(out)�d���\���̏���
				 int chart_id			(in)��ԑO�ɕ\������O���t�ԍ�
��    ��    �l : �d���\������O���t��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CChartDlg::calc_overlay_order(int overRayOrder[], int chart_id)
{
	int overRayCnt=0;

	overRayOrder[overRayCnt++] = chart_id;//chart_id���ŗD��ŕ\������

	if(API.GetCheck(m_hWnd, IDC_CHK_CHART_OVERLAY) && overlayFlg[chart_id] )//chart_id�ȊO�̃O���t���d���\������ꍇ
	{
		for(int n=0 ; n<CHART_NUM ; n++)
		{
			if( GetDataCount(n)>0 && //�f�[�^�����邩�ǂ���
				GetDataType(n) == GetDataType(chart_id) && //�O���t�^�C�v���������ǂ���
				n!=chart_id &&
				overlayFlg[n]	//�d���\���������Ă���O���t���ǂ���
				)
			{
				overRayOrder[overRayCnt++] = n;
			}
		}
	}

	return overRayCnt;
}


/********************************************************************
�@  �\  ��  �� : �f�[�^�̕\��
��    ��    �� : show_data_val
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::show_data_val(long chart_id)
{
	LVITEM       lvi={0};
	lvi.mask = LVIF_TEXT;
	char        buf[1024];
	

	m_list_ctrl.DeleteAllItems();//��U���ׂẴ��X�g�v�f���N���A


	if( GetDataType(chart_id)==CHART_TYPE_1D )//1����
	{
		for(int i=0 ; i<GetDataCount(chart_id) ; i++){

			//No.
			lvi.iItem = i;
			lvi.iSubItem = 0;
			sprintf(buf, "%d", i);
			lvi.pszText = buf;
			m_list_ctrl.InsertItem(&lvi);//���X�g�}��

			//x
			lvi.iItem = i;
			lvi.iSubItem = 1;
			sprintf(buf, "%0.3f", GetDataFromChart(chart_id,i));
			lvi.pszText = buf;
			m_list_ctrl.SetItem(&lvi);
		}
	}
	else if( GetDataType(chart_id)==CHART_TYPE_2D )//2����
	{
		for(int i=0 ; i<GetDataCount(chart_id)/2 ; i++){

			//No.
			lvi.iItem = i;
			lvi.iSubItem = 0;
			sprintf(buf, "%d", i);
			lvi.pszText = buf;
			m_list_ctrl.InsertItem(&lvi);//���X�g�}��

			//x
			lvi.iItem = i;
			lvi.iSubItem = 1;
			sprintf(buf, "%0.3f", GetDataFromChart(chart_id,i*2));
			lvi.pszText = buf;
			m_list_ctrl.SetItem(&lvi);

			//y
			lvi.iItem = i;
			lvi.iSubItem = 2;
			sprintf(buf, "%0.3f", GetDataFromChart(chart_id,i*2+1));
			lvi.pszText = buf;
			m_list_ctrl.SetItem(&lvi);
		}
	}

}

/********************************************************************
�@  �\  ��  �� : ���X�g�̗v�f��I������
��    ��    �� : select_list_item
��          �� : int n (in)���X�g�̗v�f�ԍ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::select_list_item(int n)
{
	m_list_ctrl.SetItemState(n, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_list_ctrl.SetSelectionMark(n);
	m_list_ctrl.EnsureVisible(n,true);
}

/********************************************************************
�@  �\  ��  �� : ���X�g�̑I�𒆂̗v�f�ԍ���Ԃ�
��    ��    �� : get_selected_list_item
��          �� : 
��    ��    �l : ���X�g�̑I�𒆂̗v�f�ԍ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CChartDlg::get_selected_list_item()
{
	return m_list_ctrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
}




/********************************************************************
�@  �\  ��  �� : �O���t��`�悷��
��    ��    �� : DrawChart
��          �� : int	chart_id		(in)�O���t�ԍ� (�����̏ꍇ�͌��ݑI�𒆂̂���)
				 bool	redrawList		(in)���X�g�ĕ`�悷�邩�H�@�iOnPaint�ł͕K��false�ɂ��邱�Ɓj
				 int	select_id		(in)�I���f�[�^�ԍ��w��
				 CPoint *clicked_point	(in)�I���f�[�^�����W�w��i�D��x�͔ԍ��w��̂ق��������j
				 
��    ��    �l : clicked_point�Ŏw�肵�����W�ɍł��߂��ʒu�ɕ`�悳���f�[�^�̔ԍ���Ԃ�
�@          �\ : �E�B���h�E�T�C�Y�ɍ��킹�ăO���t��`�悷��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CChartDlg::DrawChart(int chart_id, bool redrawList, int select_id, CPoint *clicked_point)
{
	CRect client_rect;//�N���C�A���g�̈��\����`
	CRect tab_rect;
	CDC	*pdc;
	int list_w, list_h, list_top, list_left;//�O���t�`��̈�
	int chart_w, chart_h, chart_top, chart_left, chart_h_mrgn;//�O���t�`��̈�
	float valX_max, valY_max, valX_min, valY_min;
	int pointed_data_id=-1, pointed_data_x, pointed_data_y;
	double dist_min=9999;


	if (chart_id < 0) {
		CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
		if(p_wnd_tab==NULL)	return -1;

		chart_id = p_wnd_tab->GetCurSel();
	}

	if (chart_id < 0 || chart_id >= CHART_NUM)
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_DATA_NUM, 0);
		return -1;
	}



	//�O���t�̔z�u�ʒu����
	::GetClientRect(m_hWnd, &client_rect);
	API.GetDialogItemRect(m_hWnd, IDC_TAB_CHART_DLG, &tab_rect);

	if(API.GetCheck(m_hWnd,IDC_CHECK_CHART_DLG_DISP_DATA))//���X�g�\������
	{
		list_left = tab_rect.left + 2;
		list_top = tab_rect.bottom + 2;
		list_w = 120;
		list_h = client_rect.Height() - list_top - 25;

		::ShowWindow(::GetDlgItem(m_hWnd, IDC_LIST_CHART),SW_SHOW);
		::MoveWindow(::GetDlgItem(m_hWnd, IDC_LIST_CHART), list_left,list_top,list_w,list_h,false);

		if(redrawList){
			show_data_val(chart_id);
		}
	}else{//���X�g�\�����Ȃ�
		list_left = tab_rect.left + 2;
		list_top = tab_rect.bottom + 2;
		list_w = 0;
		list_h = 0;
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_LIST_CHART),SW_HIDE);
	}

	chart_left = list_left + list_w + 40;//50;//�O���t�̍���X���W
	chart_top = list_top;//�O���t�̍���Y���W
	chart_w = client_rect.right - client_rect.left - chart_left - 5;//�O���t�̕�
	chart_h = client_rect.bottom - client_rect.top - chart_top - 25;//�O���t�̍���
	chart_h_mrgn = 10;//�O���t�`��̏㉺�}�[�W��



	//�_�C�A���O�A�C�e���̔z�u

	::MoveWindow(::GetDlgItem(m_hWnd, IDC_TAB_CHART_DLG), tab_rect.left, tab_rect.top, client_rect.Width() - 4, tab_rect.Height(), false);//�^�u
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X), chart_left,						chart_top + chart_h, 40,20,false);//X�\���ŏ��l�̃G�f�B�b�g�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_X), chart_left+ chart_w/2,				chart_top + chart_h, 40,20,false);//X�\�����Ԓl�̃G�f�B�b�g�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_X), chart_left+ chart_w/2+60,				chart_top + chart_h, 40,20,false);//X�\�����Ԓl�̃G�f�B�b�g�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_CHART_DLG_MID_X), chart_left+ chart_w/2+40,				chart_top + chart_h, 20,20,false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X), chart_left + chart_w-40,			chart_top + chart_h, 40,20,false);//X�\���ő�l�̃G�f�B�b�g�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y), chart_left-40, chart_top+chart_h-25, 40,20,false);//Y�\���ŏ��l�̃G�f�B�b�g�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_Y), chart_left-40, chart_top+(chart_h-25)/2, 40,20,false);//Y�\�����Ԓl�̃G�f�B�b�g�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_STATIC_CHART_DLG_MID_Y),   chart_left-40, chart_top+(chart_h-25)/2+20, 40,20,false);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_Y), chart_left-40, chart_top+(chart_h-25)/2+40, 40,20,false);//Y�\���l���̃G�f�B�b�g�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y), chart_left-40, chart_top, 40,20,false);//Y�\���ő�l�̃G�f�B�b�g�{�b�N�X

	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CHART_DLG_MID_VAL_X), chart_left+ chart_w/2-30,				chart_top + chart_h, 30,20,false);//X�����l�ݒ�{�^��
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X), chart_left+40,	chart_top + chart_h,	40,20,false);//X�\���ŏ��l�����ݒ�̃`�F�b�N�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X), chart_left + chart_w-80,	chart_top + chart_h,	40,20,false);//X�\���ő�l�����ݒ�̃`�F�b�N�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CHART_DLG_MID_VAL_Y), chart_left-40, chart_top+(chart_h-25)/2-20, 30,20,false);//Y�����l�ݒ�{�^��
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y), chart_left-40,	chart_top+chart_h-25-20, 40,20,false);//Y�\���ŏ��l�����ݒ�̃`�F�b�N�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y), chart_left-40,	chart_top+25,			40,20,false);//Y�\���ő�l�����ݒ�̃`�F�b�N�{�b�N�X


	int type = GetGraphMode(chart_id);//�O���t�^�C�v
	m_chart_type.SetCurSel(type);
	if (type == CHART_MODE_HISTGRAM) 
	{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_SPINT_CHART_DLG_HIST_SPLIT_NUM), SW_SHOW);
	}
	else {
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_SPINT_CHART_DLG_HIST_SPLIT_NUM), SW_HIDE);
	}


	//�w�i�`��
	pdc = GetDC();
	pdc->Rectangle(chart_left, chart_top, chart_left + chart_w, chart_top + chart_h);


	if(GetDataCount(chart_id)>=2)//2�_�ȏ�̃f�[�^������
	{
		if(GetDataType(chart_id) == CHART_TYPE_1D)//1����
		{
			m_chart_type.EnableWindow(1);

			if (GetGraphMode(chart_id)!= CHART_MODE_HISTGRAM)
			{
				//�f�[�^�`��

				int overRayOrder[CHART_NUM] = { 0 };//�d���\������O���t�ԍ��̗D�揇
				int overRayCnt = 0;//�d���\������O���t��
				float spectlum[CHART_DATA_NUM] = { 0 };

				overRayCnt = calc_overlay_order(overRayOrder, chart_id);//�O���t�d���\��������


				//�f�[�^�̍ő�E�ŏ����v�Z����

				valX_min = 0;
				valX_max = (float)GetDataCount(chart_id) - 1;
				valY_min = 999999;
				valY_max = -999999;

				if (GetGraphMode(chart_id) == CHART_MODE_SPECTRUM)
				{//���g���̈�

					for (int i = 0; i < GetDataCount(chart_id) && i < CHART_DATA_NUM; i++)
					{
						spectlum[i] = GetDataFromChart(chart_id, i);
					}

					API.calc_dft(spectlum, GetDataCount(chart_id), spectlum);

					//���g��3�����͐M�p�ł��Ȃ�
					for (int i = 3; i < GetDataCount(chart_id) / 2 && i < CHART_DATA_NUM; i++)
					{
						float val = spectlum[i];
						if (valY_max < val) valY_max = val;
						if (valY_min > val) valY_min = val;
					}
				}
				else{//�f�[�^���̂���
					for (int n = overRayCnt - 1; n >= 0; n--)//�O���t�d���\��(�D�揇�̍������̂قǑO�ʂɕ`��)
					{
						for (int i = 0; i<GetDataCount(overRayOrder[n]) && i<CHART_DATA_NUM; i++)
						{
							float val = GetDataFromChart(overRayOrder[n], i);
							if (valY_max < val) valY_max = val;
							if (valY_min > val) valY_min = val;
						}
					}
				}


				if(valY_min==valY_max){
					valY_max+=1;
					valY_min-=1;
				}


				//�O���t�\���̍ő�E�ŏ��ɌŒ�l or �f�[�^�ő�E�ŏ� �̂ǂ�����g���������߂�

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X) ){//X�����ŏ��ɌŒ�l���g��
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X, &valX_min);
				}

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X) ){//X�����ő�ɌŒ�l���g��
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X, &valX_max);
				}

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y) ){//Y�����ŏ��ɌŒ�l���g��
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y, &valY_min);
				}

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y) ){//Y�����ő�ɌŒ�l���g��
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, &valY_max);
				}



				//�⏕���̕`��

				draw_line(pdc, chart_left, chart_top+chart_h_mrgn, chart_left+chart_w, chart_top+chart_h_mrgn, RGB(128,128,128));//�⏕��
				draw_line(pdc, chart_left, chart_top+chart_h-chart_h_mrgn, chart_left+chart_w, chart_top+chart_h-chart_h_mrgn, RGB(128,128,128));//�⏕��
				draw_line(pdc, chart_left, chart_top+chart_h/2, chart_left+chart_w, chart_top+chart_h/2, RGB(128,128,128));//�⏕��


				//�f�[�^�_�̕`��

				if(valX_max>valX_min && valY_max>valY_min )
				{

					for(int n=overRayCnt-1 ; n>=0 ; n--)//�O���t�d���\��(�D�揇�̍������̂قǑO�ʂɕ`��)
					{
						float x,y;//�f�[�^�_�`����W
						float xo=-1,yo=-1;//��O�̃f�[�^�_�`����W
						int dataCnt = GetDataCount(overRayOrder[n]);
						for (int i = 0; i< dataCnt && i<CHART_DATA_NUM; i++)
						{

							if (GetGraphMode(chart_id) == CHART_MODE_SPECTRUM)
							{//���g���̈�
								if (i > dataCnt / 2)	break;
								x = (i - valX_min) / (valX_max - valX_min)*chart_w + chart_left;
								y = chart_top + chart_h - chart_h_mrgn
									- (spectlum[i] - valY_min) / (valY_max - valY_min)*(chart_h - chart_h_mrgn * 2);
							}
							else
							{	
								x = (i - valX_min) / (valX_max - valX_min)*chart_w + chart_left;
								y = chart_top + chart_h - chart_h_mrgn
									- (GetDataFromChart(overRayOrder[n], i) - valY_min) / (valY_max - valY_min)*(chart_h - chart_h_mrgn * 2);
							}

							if( chart_left <= x && 
								chart_left + chart_w >= x &&
								chart_top <= y &&
								chart_top + chart_h >= y)//�O���t�̈悩��݂̂͂����`�F�b�N
							{
								if(select_id>=0)//�I���f�[�^ID���w�肳��Ă���
								{
									if(select_id==i){
										pointed_data_id = i;
										pointed_data_x=(int)x;
										pointed_data_y=(int)y;
									}
								}
								else if(clicked_point)//�I���f�[�^���W���w�肳��Ă���
								{
									double d = (clicked_point->x - x)*(clicked_point->x - x) + (clicked_point->y - y)*(clicked_point->y - y);
									if(d<dist_min && d<50*50){
										dist_min = d;
										pointed_data_id=i;
										pointed_data_x=(int)x;
										pointed_data_y=(int)y;
									}
								}

								//�f�[�^�_�Ԃ���Ō���
								if(xo>=0 && yo>=0){
									draw_line(pdc,(int)xo,(int)yo,(int)x,(int)y,RGB(0,0,0));
								}
								xo = x;
								yo = y;

								draw_point(pdc, (int)x, (int)y, 2, chartColor[n]);//�f�[�^�_�ɉ~��`��
							}
							else
							{
								xo = -1;
								yo = -1;
							}
						}
					}
				}
			}
			else//�q�X�g�O����
			{
				int histgram[CHART_NUM][CHART_HISTGRAM_SPLIT_MAX]={0};
				int maxHist=0;
				float histStep;
				int hist_split_cnt;

				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM, &hist_split_cnt);
				if(hist_split_cnt>CHART_HISTGRAM_SPLIT_MAX){
					hist_split_cnt = CHART_HISTGRAM_SPLIT_MAX;
				}

				int overRayOrder[CHART_NUM]={0};//�d���\������O���t�ԍ��̗D�揇
				int overRayCnt=0;//�d���\������O���t��

				overRayCnt = calc_overlay_order(overRayOrder, chart_id);//�O���t�d���\��������

				//�f�[�^�̍ő�E�ŏ����v�Z����

				valX_min = 999999;
				valX_max = -999999;
				for(int n=overRayCnt-1 ; n>=0 ; n--)//�O���t�d���\��(�D�揇�̍������̂قǑO�ʂɕ`��)
				{
					for(int i=0 ; i<GetDataCount( overRayOrder[n] ) && i<CHART_DATA_NUM ; i++)
					{
						float val = GetDataFromChart(overRayOrder[n], i);
						if(valX_max < val) valX_max = val;
						if(valX_min > val) valX_min = val;
					}
				}

				if(valX_min==valX_max){
					valX_max+=1;
					valX_min-=1;
				}


				//�x���𐔂���
				histStep = (valX_max - valX_min)/hist_split_cnt;
				for(int n=overRayCnt-1 ; n>=0 ; n--)//�O���t�d���\��(�D�揇�̍������̂قǑO�ʂɕ`��)
				{
					for(int i=0 ; i<GetDataCount( overRayOrder[n] ) && i<CHART_DATA_NUM ; i++)
					{
						for(int j=0 ; j<hist_split_cnt ; j++)
						{
							if( GetDataFromChart(overRayOrder[n], i) - valX_min <= (float)(j+1)*histStep ){
								histgram[n][j]++;
								if(maxHist < histgram[n][j])	maxHist = histgram[n][j];
								break;
							}
						}
					}
				}
				valY_min = 0;
				valY_max = (float)maxHist;


				//�O���t�\���̍ő�E�ŏ��ɌŒ�l or �f�[�^�ő�E�ŏ� �̂ǂ�����g���������߂�

				if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y) ){//Y�����ő�ɌŒ�l���g��
					API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, &valY_max);
				}



				if(valX_max>valX_min && valY_max>valY_min )
				{
					//�`��
					float x0,y0,x1,y1;
					for(int n=overRayCnt-1 ; n>=0 ; n--)//�O���t�d���\��(�D�揇�̍������̂قǑO�ʂɕ`��)
					{
						for(int j=0 ; j<hist_split_cnt ; j++)
						{
							x0 = (float)j*(float)chart_w/(float)hist_split_cnt + chart_left;
							y0 = (float)chart_top + (float)chart_h - (float)chart_h_mrgn;
							x1 = x0 + (float)chart_w/(float)hist_split_cnt;
							y1 = y0 - (float)histgram[n][j] / (float)valY_max * (float)(chart_h - chart_h_mrgn*2);
							if(y1<chart_top)	y1=chart_top;


							draw_hist_bar(pdc, (int)x0, (int)y0, (int)x1, (int)y1, histgram[n][j], chartColor[n]);//�q�X�g�O�����`��

							if(select_id>=0)//�I���f�[�^ID���w�肳��Ă���
							{
		
							}
							else if(clicked_point)//�I���f�[�^���W���w�肳��Ă���
							{
								if(x0<clicked_point->x && x1>clicked_point->x){
									draw_hist_bar(pdc, (int)x0, (int)y0, (int)x1, (int)y1, histgram[n][j], RGB(255,0,0));//�q�X�g�O�����F�ς��`��

									CString str;
									str.Format("freqency=%d (%f�`%f)", histgram[n][j], (float)(j*histStep), (float)(j+1)*histStep);
									API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, str);
								}
							}
						}
					}

					draw_line(pdc, chart_left, chart_top+chart_h_mrgn, chart_left+chart_w, chart_top+chart_h_mrgn, RGB(128,128,128));//�⏕��
					draw_line(pdc, chart_left, chart_top+chart_h-chart_h_mrgn, chart_left+chart_w, chart_top+chart_h-chart_h_mrgn, RGB(128,128,128));//�⏕��
					draw_line(pdc, chart_left, chart_top+chart_h/2, chart_left+chart_w, chart_top+chart_h/2, RGB(128,128,128));//�⏕��
				}
			}

			API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_DATA_NUM, GetDataCount(chart_id));//�f�[�^���\��

		}
		else if(GetDataType(chart_id) == CHART_TYPE_2D)//2�����U�z�}
		{
			m_chart_type.EnableWindow(0);


			int overRayOrder[CHART_NUM]={0};//�d���\������O���t�ԍ��̗D�揇
			int overRayCnt=0;//�d���\������O���t��

			overRayCnt = calc_overlay_order(overRayOrder, chart_id);//�O���t�d���\��������


			//�f�[�^�̍ő�E�ŏ����v�Z����

			valX_min = 999999;
			valX_max = -999999;
			valY_min = 999999;
			valY_max = -999999;
			for(int n=overRayCnt-1 ; n>=0 ; n--)//�O���t�d���\��(�D�揇�̍������̂قǑS�ʂɕ`��)
			{
				for(int i=0 ; i<GetDataCount( overRayOrder[n] ) && i<CHART_DATA_NUM ; i+=2)
				{
					float valx = GetDataFromChart(overRayOrder[n], i);
					if(valX_max < valx) valX_max = valx;
					if(valX_min > valx) valX_min = valx;

					float valy = GetDataFromChart(overRayOrder[n], i+1);
					if(valY_max < valy) valY_max = valy;
					if(valY_min > valy) valY_min = valy;
				}
			}

			if(valX_min==valX_max){
				valX_max+=1;
				valX_min-=1;
			}

			if(valY_min==valY_max){
				valY_max+=1;
				valY_min-=1;
			}


			//�O���t�\���̍ő�E�ŏ��ɌŒ�l or �f�[�^�ő�E�ŏ� �̂ǂ�����g���������߂�

			if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X) ){//X�����ŏ��ɌŒ�l���g��
				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X, &valX_min);
			}

			if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X) ){//X�����ő�ɌŒ�l���g��
				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X, &valX_max);
			}

			if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y) ){//Y�����ŏ��ɌŒ�l���g��
				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y, &valY_min);
			}

			if( !API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y) ){//Y�����ő�ɌŒ�l���g��
				API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, &valY_max);
			}


			//�⏕���̕`��

			draw_line(pdc, chart_left, chart_top+chart_h_mrgn,			chart_left+chart_w, chart_top+chart_h_mrgn,			RGB(128,128,128));//�⏕��
			draw_line(pdc, chart_left, chart_top+chart_h-chart_h_mrgn,	chart_left+chart_w, chart_top+chart_h-chart_h_mrgn, RGB(128,128,128));//�⏕��
			draw_line(pdc, chart_left, chart_top+chart_h/2,				chart_left+chart_w, chart_top+chart_h/2,			RGB(128,128,128));//�⏕��
			draw_line(pdc, chart_left+chart_h_mrgn,				chart_top, chart_left+chart_h_mrgn,				chart_top+chart_h,	RGB(128,128,128));//�⏕��
			draw_line(pdc, chart_left+chart_w-chart_h_mrgn,		chart_top, chart_left+chart_w-chart_h_mrgn,		chart_top+chart_h,	RGB(128,128,128));//�⏕��
			draw_line(pdc, chart_left+chart_w/2,				chart_top, chart_left+chart_w/2,				chart_top+chart_h,	RGB(128,128,128));//�⏕��


			//�f�[�^�_�̕`��

			if(valX_max>valX_min && valY_max>valY_min )
			{
				for(int n=overRayCnt-1 ; n>=0 ; n--)//�O���t�d���\��(�D�揇�̍������̂قǑS�ʂɕ`��)
				{
					float x,y;
					for(int i=0 ; i<GetDataCount( overRayOrder[n] ) && i<CHART_DATA_NUM ; i+=2)
					{
						x = chart_left + chart_h_mrgn
							 + ( GetDataFromChart(overRayOrder[n], i  ) - valX_min) / (valX_max - valX_min)*(chart_w - chart_h_mrgn*2) ;
						y = chart_top + chart_h - chart_h_mrgn
							 - ( GetDataFromChart(overRayOrder[n], i+1) - valY_min) / (valY_max - valY_min)*(chart_h - chart_h_mrgn*2) ;

						if( chart_left <= x && 
							chart_left + chart_w >= x &&
							chart_top <= y &&
							chart_top + chart_h >= y)//�O���t�̈悩��݂̂͂����`�F�b�N
						{
							if(select_id>=0)//�I���f�[�^ID���w�肳��Ă���
							{
								if(select_id==i){
									pointed_data_id=i;
									pointed_data_x=(int)x;
									pointed_data_y=(int)y;
								}
							}
							else if(clicked_point)//�I���f�[�^���W���w�肳��Ă���
							{
								double d = (clicked_point->x - x)*(clicked_point->x - x) + (clicked_point->y - y)*(clicked_point->y - y);
								if(d<dist_min && d<50*50){
									dist_min = d;
									pointed_data_id=i;
									pointed_data_x=(int)x;
									pointed_data_y=(int)y;
								}
							}

							draw_point(pdc, (int)x, (int)y, 2, chartColor[n]);//�f�[�^�_�ɉ~��`��
						}
					}
				}
			}

			API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_DATA_NUM, GetDataCount(chart_id)/2);//�f�[�^���\��
		}
	}
	else
	{
		valX_min = 0;
		valX_max = 0;
		valY_min = 0;
		valY_max = 0;
	}

	
	//��ʏ�N���b�N���ꂽ�f�[�^��`��

	CString str;
	if(GetDataType(chart_id) == CHART_TYPE_1D)//1����
	{
		if(GetGraphMode(chart_id)!= CHART_MODE_HISTGRAM)
		{
			//�܂���O���t

			if(pointed_data_id>=0)
			{
				draw_point(pdc, pointed_data_x, pointed_data_y, 2, RGB(255,0,0));//�f�[�^�_�ɉ~��`��

				float val=GetDataFromChart(chart_id, pointed_data_id);

				if (GetGraphMode(chart_id) == CHART_MODE_SPECTRUM)
				{//���g��
					int dataCnt = GetDataCount(chart_id);
					float period = (float)dataCnt / (float)pointed_data_id;
					str.Format("period=%f", period);
				}
				else {
					str.Format("ID=%d  value=%f", pointed_data_id, val);
				}
				API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, str);

				if(API.GetCheck(m_hWnd,IDC_CHECK_CHART_DLG_DISP_DATA))
				{//���X�g�\������
					select_list_item(pointed_data_id);
				}
			}else{
				API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, CString(""));
			}
		}
	}
	else if(GetDataType(chart_id) == CHART_TYPE_2D)//2�����U�z�}
	{
		if(pointed_data_id>=0)
		{
			draw_point(pdc, pointed_data_x, pointed_data_y, 2, RGB(255,0,0));//�f�[�^�_�ɉ~��`��

			float valx=GetDataFromChart(chart_id, pointed_data_id);
			float valy=GetDataFromChart(chart_id, pointed_data_id+1);
			str.Format("ID=%d  X=%f  Y=%f", pointed_data_id, valx, valy);
			API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, str);


			if(API.GetCheck(m_hWnd,IDC_CHECK_CHART_DLG_DISP_DATA))
			{//���X�g�\������
				select_list_item(pointed_data_id/2);
			}
		}else{
			API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_POINTED_VAL, CString(""));
		}
	}


	//�ő�E�ŏ��E���Ԓl�E���̕\��
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X, valX_max);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_X, (valX_max + valX_min)/2);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_X, (valX_max - valX_min)/2);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X, valX_min);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, valY_max);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_Y, (valY_max + valY_min)/2);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_Y, (valY_max - valY_min)/2);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y, valY_min);


	//���ݑI�𒆂̃O���t�ԍ��ƈقȂ�ԍ����w�肳�ꂽ�ꍇ�͍ēx�`�悷��
	int chart_id_now;
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id_now = p_wnd_tab->GetCurSel();

	if(chart_id_now!=chart_id){
		p_wnd_tab->SetCurSel(chart_id);
	}

	ReleaseDC(pdc);

	return pointed_data_id;
}


/********************************************************************
�@  �\  ��  �� : ���X�g������
��    ��    �� : init_list
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::init_list()
{
    LVCOLUMN    lvc;
    char       caption[32];

	m_list_ctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O


	//No.
	sprintf(caption, "ID");
    lvc.iSubItem    = 0;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 20;          // ����
    m_list_ctrl.InsertColumn(0, &lvc);

	//X
	sprintf(caption, "x");
    lvc.iSubItem    = 1;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 50;          // ����
    m_list_ctrl.InsertColumn(1, &lvc);
   
	//Y
	sprintf(caption, "y");
    lvc.iSubItem    = 2;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 50;          // ����
    m_list_ctrl.InsertColumn(2, &lvc);
}
//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChartDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_OUTPUT_CSV, &CChartDlg::OnBnClickedButtonChartDlgOutputCsv)
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_CLEAR, &CChartDlg::OnBnClickedButtonChartDlgClear)
	ON_BN_CLICKED(IDC_CHK_CHART_AUTO_MAX_Y, &CChartDlg::OnBnClickedChkChartAutoMaxY)
	ON_BN_CLICKED(IDC_CHK_CHART_AUTO_MIN_Y, &CChartDlg::OnBnClickedChkChartAutoMinY)
	ON_BN_CLICKED(IDC_CHK_CHART_AUTO_MAX_X, &CChartDlg::OnBnClickedChkChartAutoMaxX)
	ON_BN_CLICKED(IDC_CHK_CHART_AUTO_MIN_X, &CChartDlg::OnBnClickedChkChartAutoMinX)
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_OVERLAY_DETAIL, &CChartDlg::OnBnClickedButtonChartDlgOverlayDetail)
	ON_BN_CLICKED(IDC_CHK_CHART_OVERLAY, &CChartDlg::OnBnClickedChkChartOverlay)
	ON_EN_UPDATE(IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM, &CChartDlg::OnEnUpdateEditChartDlgHistSplitNum)
	ON_BN_CLICKED(IDC_CHECK_CHART_DLG_DISP_DATA, &CChartDlg::OnBnClickedCheckChartDlgDispData)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CHART, &CChartDlg::OnNMClickListChart)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CHART, &CChartDlg::OnNMRClickListChart)
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_MID_VAL_Y, &CChartDlg::OnBnClickedButtonChartDlgMidValY)
	ON_BN_CLICKED(IDC_BUTTON_CHART_DLG_MID_VAL_X, &CChartDlg::OnBnClickedButtonChartDlgMidValX)
	ON_CBN_SELCHANGE(IDC_COMBO_CHART_DLG_TYPE, &CChartDlg::OnCbnSelchangeComboChartDlgType)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CHART_DLG, &CChartDlg::OnTcnSelchangeTabChartDlg)
END_MESSAGE_MAP()


/********************************************************************
�@  �\  ��  �� : �_�C�A���O�̏�����
��    ��    �� : OnInitDialog
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CChartDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(AfxGetApp()->LoadIcon(IDI_GRAPH), TRUE);			// �A�C�R����ݒ�

	init_list();

	//�O���t�p�������̈�̎擾
	for(int i=0 ; i<CHART_NUM ; i++){
		ClearChart(i);
	}

	//�_�C�A���O�A�C�e���̏�����
	
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	p_wnd_tab->SetCurSel(0);
	for (int n = 0; n < CHART_NUM; n++) 
	{
		CString str;
		str.Format("%d", n);
		p_wnd_tab->InsertItem(n, str);
	}
	
	API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X, true);
	API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X, true);
	API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y, true);
	API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y, true);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_X), false);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_X), false);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_Y), false);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_Y), false);

	CSpinButtonCtrl *spin2  = (CSpinButtonCtrl*)GetDlgItem(IDC_SPINT_CHART_DLG_HIST_SPLIT_NUM);
	spin2->SetRange(10,CHART_HISTGRAM_SPLIT_MAX-1);
	spin2->SetPos(30);
	API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_HIST_SPLIT_NUM, 30);


	overlayFlg[0] = true;
	overlayFlg[1] = true;
	overlayFlg[2] = true;
	overlayFlg[3] = true;
	overlayFlg[4] = true;
	overlayFlg[5] = true;
	overlayFlg[6] = true;
	overlayFlg[7] = true;
	overlayFlg[8] = true;
	overlayFlg[9] = false;

	//�O���t�f�[�^�̕`��F(�d��)
	chartColor[0] = RGB(0,0,128);
	chartColor[1] = RGB(128,0,0);
	chartColor[2] = RGB(0,128,0);
	chartColor[3] = RGB(128,0,128);
	chartColor[4] = RGB(0,128,128);
	chartColor[5] = RGB(128,128,0);
	chartColor[6] = RGB(0,0,255);
	chartColor[7] = RGB(255,0,0);
	chartColor[8] = RGB(0,255,0);
	chartColor[9] = RGB(0,255,0);

	
	//�O���t�^�C�v
	m_chart_type.AddString("�f�[�^");
	m_chart_type.AddString("�q�X�g�O����");
	m_chart_type.AddString("�X�y�N�g����");


	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/********************************************************************
�@  �\  ��  �� : �ĕ`��
��    ��    �� : OnPaint
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	DrawChart(-1, false);
}

/********************************************************************
�@  �\  ��  �� : �\������O���t�ԍ����ύX���ꂽ
��    ��    �� : OnTcnSelchangeTabChartDlg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnTcnSelchangeTabChartDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	this->Invalidate();
	DrawChart();

	*pResult = 0;
}

/********************************************************************
�@  �\  ��  �� :�E�B���h�E�T�C�Y���ύX���ꂽ
��    ��    �� : OnSize
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	this->Invalidate();
	DrawChart();//�ĕ`����s
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�̍��{�^�����������ꂽ
��    ��    �� : OnLButtonDown
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	DrawChart(-1, false, -1, &point);//�����ʒu�̃f�[�^�ԍ����擾

	CDialog::OnLButtonDown(nFlags, point);
}

/********************************************************************
�@  �\  ��  �� :�u�f�[�^��CSV�ɏo�͂���v�{�^�����������ꂽ
��    ��    �� : OnBnClickedButtonChartDlgOutputCsv
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgOutputCsv()
{
	CFileDialog myDLG(false, _T("csv"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
							"�摜�t�@�C�����X�g(*.csv)|*.CSV|�e�L�X�g(*.txt)|*.txt||");
	if( myDLG.DoModal() != IDOK )	return	;


	int chart_id=0;//��ԑO�ɕ\������O���t�ԍ�
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();


	int overRayOrder[CHART_NUM]={0};//�d���\������O���t�ԍ��̗D�揇
	int overRayCnt=0;//�d���\������O���t��
	overRayCnt = calc_overlay_order(overRayOrder, chart_id);//�O���t�d���\��������


	FILE *fp = fopen(myDLG.GetPathName(),"wt");
	if(fp)
	{
		if(GetDataType(chart_id)==CHART_TYPE_1D)//�܂���O���t
		{
			for(int i=0 ; i<GetDataCount(chart_id) ; i++)
			{
				fprintf(fp,"%d",i);
				for(int n=0 ; n<overRayCnt ; n++)
				{
					float val=GetDataFromChart(overRayOrder[n], i);
					fprintf(fp,",%f", val);
				}
				fprintf(fp,"\n");
			}
		}
		else if(GetDataType(chart_id)==CHART_TYPE_2D)//2�����U�z�}
		{
			for(int i=0 ; i<GetDataCount(chart_id) ; i+=2)
			{
				fprintf(fp,"%d",i);
				for(int n=0 ; n<overRayCnt ; n++)
				{
					float valx=GetDataFromChart(chart_id, i);
					float valy=GetDataFromChart(chart_id, i+1);
					fprintf(fp,",%f,%f",valx, valy);
				}
				fprintf(fp,"\n");
			}
		}

		fclose(fp);
	}
}



/********************************************************************
�@  �\  ��  �� : �N���A�{�^������
��    ��    �� : OnBnClickedButtonChartDlgClear
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgClear()
{
	int chart_id;
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();
	ClearChart(chart_id);
	DrawChart(chart_id);
}


/********************************************************************
�@  �\  ��  �� : Y�ő�l�������ݒ肷��`�F�b�N�{�b�N�X�̒l�ύX
��    ��    �� : OnBnClickedChkChartAutoMaxY
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedChkChartAutoMaxY()
{
	if( API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y) ){
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_Y), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_Y), true);
	}
}

/********************************************************************
�@  �\  ��  �� : Y�ŏ��l�������ݒ肷��`�F�b�N�{�b�N�X�̒l�ύX
��    ��    �� : OnBnClickedChkChartAutoMinY
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedChkChartAutoMinY()
{
	if( API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y) ){
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_Y), false);
		
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_Y), true);
	}
}




/********************************************************************
�@  �\  ��  �� : X�ő�l�������ݒ肷��`�F�b�N�{�b�N�X�̒l�ύX
��    ��    �� : OnBnClickedChkChartAutoMaxX
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedChkChartAutoMaxX()
{
	if( API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X) ){
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_X), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MAX_VAL_X), true);
	}
}

/********************************************************************
�@  �\  ��  �� : X�ŏ��l�������ݒ肷��`�F�b�N�{�b�N�X�̒l�ύX
��    ��    �� : OnBnClickedChkChartAutoMinX
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedChkChartAutoMinX()
{
	if( API.GetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X) ){
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_X), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_CHART_DLG_MIN_VAL_X), true);
	}
}


/********************************************************************
�@  �\  ��  �� :�L�[�{�[�h����̓���
��    ��    �� : PreTranslateMessage
��          �� : 
��    ��    �l : 
�@          �\ : ���^�[���F�ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CChartDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN ){
		CPoint	point;
		CString	str;

		switch(pMsg->wParam){
			case VK_RETURN://Enter
				this->Invalidate();
				DrawChart();//�ĕ`����s
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://��
				break;

			case VK_DOWN://��
				break;

			case VK_LEFT://��
				break;

			case VK_RIGHT://��
				break;

			case VK_DELETE://Del
				break;
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
�@  �\  ��  �� :�d���\���ڍאݒ�{�^������
��    ��    �� : OnBnClickedButtonChartDlgOverlayDetail
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgOverlayDetail()
{
	CConfirmDlg cfm;

	cfm.SetTitle("�d���\������O���t");
	cfm.RegistVar("�O���t0", &overlayFlg[0]);
	cfm.RegistVar("�O���t1", &overlayFlg[1]);
	cfm.RegistVar("�O���t2", &overlayFlg[2]);
	cfm.RegistVar("�O���t3", &overlayFlg[3]);
	cfm.RegistVar("�O���t4", &overlayFlg[4]);
	cfm.RegistVar("�O���t5", &overlayFlg[5]);
	cfm.RegistVar("�O���t6", &overlayFlg[6]);
	cfm.RegistVar("�O���t7", &overlayFlg[7]);
	cfm.RegistVar("�O���t8", &overlayFlg[8]);
	cfm.RegistVar("�O���t9", &overlayFlg[9]);

	if( cfm.DoModal()==IDOK){
		this->Invalidate();
		DrawChart();//�ĕ`����s
	}
}

/********************************************************************
�@  �\  ��  �� :�d���\���`�F�b�N�{�b�N�X��ԕω�
��    ��    �� : OnBnClickedChkChartOverlay
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedChkChartOverlay()
{
	this->Invalidate();
	DrawChart();//�ĕ`����s
}


/********************************************************************
�@  �\  ��  �� : �q�X�g�O����
��    ��    �� : OnEnUpdateEditChartDlgHistSplitNum
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnEnUpdateEditChartDlgHistSplitNum()
{
	this->Invalidate();
	DrawChart();//�ĕ`����s
}

/********************************************************************
�@  �\  ��  �� : �O���t���[�h
��    ��    �� : OnCbnSelchangeComboChartDlgType
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnCbnSelchangeComboChartDlgType()
{
	int chart_id;
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();
	SetGraphMode(chart_id, m_chart_type.GetCurSel());
	this->Invalidate();
	DrawChart(chart_id);
}


/********************************************************************
�@  �\  ��  �� :�l�̕\��
��    ��    �� : OnBnClickedCheckChartDlgDispData
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedCheckChartDlgDispData()
{
	this->Invalidate();
	DrawChart();//�ĕ`����s
}




/********************************************************************
�@  �\  ��  �� : ���X�g�I��
��    ��    �� : OnNMClickListChart
��          �� : 
��    ��    �l : 
�@          �\ : �I�����ꂽ�J�[�\���̐F��ς��ĕ\��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnNMClickListChart(NMHDR *pNMHDR, LRESULT *pResult)
{

	int chart_id;
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();

	if( GetDataType(chart_id)==CHART_TYPE_1D )
	{//1����
		DrawChart(chart_id, false, get_selected_list_item());//�O���t�ĕ`��
	}
	else if( GetDataType(chart_id)==CHART_TYPE_2D )
	{//2����
		DrawChart(chart_id, false, get_selected_list_item()*2);//�O���t�ĕ`��
	}

	*pResult = 0;
}



/********************************************************************
�@  �\  ��  �� : �}�E�X�E�{�^��������
��    ��    �� : OnNMRClickListMark
��          �� : 
��    ��    �l : 
�@          �\ : �ҏW�̂��߂̃_�C�A���O���o���B�ҏW��ɍĕ\������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnNMRClickListChart(NMHDR *pNMHDR, LRESULT *pResult)
{
	int chart_id;//�O���t�ԍ�
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_CHART_DLG);
	chart_id = p_wnd_tab->GetCurSel();

	if( GetDataType(chart_id)==CHART_TYPE_1D )
	{//1����
		CConfirmDlg cfm;
		float x = GetDataFromChart(chart_id, get_selected_list_item()) ;

		cfm.SetTitle("�f�[�^�ҏW");
		cfm.SetWidth(50,50);
		cfm.RegistVar("x", &x);

		if(cfm.DoModal()==IDOK)
		{
			SetDataToChart(chart_id, get_selected_list_item(), x);
			DrawChart(chart_id);
		}
	}
	else if( GetDataType(chart_id)==CHART_TYPE_2D )
	{//2����
		CConfirmDlg cfm;
		float x = GetDataFromChart(chart_id, get_selected_list_item()*2 );
		float y = GetDataFromChart(chart_id, get_selected_list_item()*2+1 );

		cfm.SetTitle("�f�[�^�ҏW");
		cfm.SetWidth(50,50);
		cfm.RegistVar("x", &x);
		cfm.RegistVar("y", &y);

		if(cfm.DoModal()==IDOK)
		{
			SetDataToChart(chart_id, get_selected_list_item()*2, x);
			SetDataToChart(chart_id, get_selected_list_item()*2+1, y);
			DrawChart(chart_id);
		}
	}



	*pResult = 0;
}



/********************************************************************
�@  �\  ��  �� : Y�����l�ݒ�{�^������
��    ��    �� : OnBnClickedButtonChartDlgMidValY
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgMidValY()
{
	CConfirmDlg cfm;
	float mid, wid;

	API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_Y, &mid);
	API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_Y, &wid);

	cfm.SetTitle("Y���ݒ�");
	cfm.RegistVar("����", &mid);
	cfm.RegistVar("�}  ", &wid);

	if( cfm.DoModal()==IDOK){
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_Y, mid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_Y, wid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_Y, mid + wid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_Y, mid - wid);
		API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_Y, false);
		API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_Y, false);
		this->Invalidate();
		DrawChart();//�ĕ`����s
	}
}

/********************************************************************
�@  �\  ��  �� : X�����l�ݒ�{�^������
��    ��    �� : OnBnClickedButtonChartDlgMidValX
��          �� : 
��    ��    �l : 
�@          �\ : �ҏW�̂��߂̃_�C�A���O���o���B�ҏW��ɍĕ\������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CChartDlg::OnBnClickedButtonChartDlgMidValX()
{
	CConfirmDlg cfm;
	float mid, wid;

	API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_X, &mid);
	API.GetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_X, &wid);

	cfm.SetTitle("X���ݒ�");
	cfm.RegistVar("����", &mid);
	cfm.RegistVar("�}  ", &wid);

	if( cfm.DoModal()==IDOK){
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MID_VAL_X, mid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_WID_VAL_X, wid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MAX_VAL_X, mid + wid);
		API.SetEditValue(m_hWnd, IDC_EDIT_CHART_DLG_MIN_VAL_X, mid - wid);
		API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MAX_X, false);
		API.SetCheck(m_hWnd, IDC_CHK_CHART_AUTO_MIN_X, false);
		this->Invalidate();
		DrawChart();//�ĕ`����s
	}
}


