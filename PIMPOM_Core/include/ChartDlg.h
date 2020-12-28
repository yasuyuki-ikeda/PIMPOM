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
#pragma once
#include "afxwin.h"
#include "afxcmn.h"


///*************************************
//         �}  �N  ��   ��   �`         
///************************************
#define		CHART_TYPE_1D	(0)
#define		CHART_TYPE_2D	(1)

#define		CHART_MODE_DATA	(0)
#define		CHART_MODE_HISTGRAM	(1)
#define		CHART_MODE_SPECTRUM	(2)

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

//�f�[�^�\����
typedef struct CHART_DATA_STRUCT_{
	float	data[CHART_DATA_NUM];//�O���t�f�[�^
	int		dataCount;//�O���t�f�[�^�̐�
	int		dataType;//�f�[�^�̃^�C�v 0:1�������z�̐܂���} 1:2�������z�̎U�z�}
	int		graphMode;//�O���t�̃^�C�v 0:�f�[�^  1:�q�X�g�O����  2:�X�y�N�g���� �i�O���t�̃^�C�v��0�̂Ƃ��̂ݗL���j	
}CHART_DATA_STRUCT;

///*************************************
//         �N���X   ��   �`         
///*************************************

class CChartDlg : public CDialog
{
	DECLARE_DYNAMIC(CChartDlg)

	enum { IDD = IDD_CHART_DLG };// �_�C�A���O �f�[�^

public:
	CChartDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CChartDlg();
	
	//�O���t�f�[�^�ɃA�N�Z�X����
	void	ClearChart(int chart_id);//�O���t���N���A����
	int		GetDataCount(int chart_id);//�O���t�̃f�[�^�����擾����
	float	GetDataFromChart(int chart_id, int data_id);//�O���t����f�[�^���擾����
	int		GetDataType(int chart_id);//�O���t�̃f�[�^�^�C�v���擾����
	int		GetGraphMode(int chart_id);//�O���t�\����Ԃ��擾����
	void	SetGraphMode(int chart_id, int mode);//�O���t�\����Ԃ�ݒ肷��
	bool	SetDataToChart(int chart_id, int data_id, float value);//�O���t�̒l��ݒ肷��
	int		AddDataToChart(int chart_id, float value, bool do_redraw=true);//�O���t�Ƀf�[�^��ǉ��i1�������z�j
	int		AddDataToChart(int chart_id, float valueX, float valueY, bool do_redraw=true);//�O���t�Ƀf�[�^��ǉ��i2�������z�j
	bool	PlotDataToChart(int chart_id, int data_num, float *pValueArr, bool doClear=true);//�f�[�^��̃v���b�g�i1�������z�j
	bool	PlotDataToChart(int chart_id, int data_num, double *pValueArr, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, int *pValueArr, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, BYTE *pValueArr, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, float *pValueArrX, float *pValueArrY, bool doClear=true);//�f�[�^��̃v���b�g�i2�������z�j
	bool	PlotDataToChart(int chart_id, int data_num, double *pValueArrX, double *pValueArrY, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, int *pValueArrX, int *pValueArrY, bool doClear=true);
	bool	PlotDataToChart(int chart_id, int data_num, BYTE *pValueArrX, BYTE *pValueArrY, bool doClear=true);
	int		DrawChart(int chart_id=-1, bool drawList=true, int select_id = -1, CPoint *clicked_point=NULL);//�O���t��`�悷��



protected:
	bool		overlayFlg[CHART_NUM];//�d���\�����������ǂ���
	COLORREF	chartColor[CHART_NUM];//�O���t�f�[�^�̕`��F(�d��)
	CHART_DATA_STRUCT *pChartDataMap[CHART_NUM];
	CListCtrl m_list_ctrl;
	CComboBox m_chart_type;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	void	init_list();//���X�g�r���[������������
	void	show_data_val(long chart_id);//�f�[�^�l�����X�g�r���[�ɂĕ\������
	void	select_list_item(int n);//�w�肳�ꂽ���X�g�r���[�̃A�C�e����I������
	int		get_selected_list_item();//�I�𒆂̃��X�g�r���[�̃A�C�e���ԍ����擾����
	int		calc_overlay_order(int overRayOrder[], int chart_id);//�d���\���̏��Ԃ����߂�


 //���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonChartDlgOutputCsv();
	afx_msg void OnBnClickedButtonChartDlgClear();
	afx_msg void OnBnClickedChkChartAutoMaxY();
	afx_msg void OnBnClickedChkChartAutoMinY();
	afx_msg void OnBnClickedChkChartAutoMaxX();
	afx_msg void OnBnClickedChkChartAutoMinX();
	afx_msg void OnBnClickedButtonChartDlgOverlayDetail();
	afx_msg void OnBnClickedChkChartOverlay();
	afx_msg void OnEnUpdateEditChartDlgHistSplitNum();
	afx_msg void OnBnClickedCheckChartDlgDispData();
	afx_msg void OnNMClickListChart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListChart(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonChartDlgMidValY();
	afx_msg void OnBnClickedButtonChartDlgMidValX();
	afx_msg void OnCbnSelchangeComboChartDlgType();
	afx_msg void OnTcnSelchangeTabChartDlg(NMHDR *pNMHDR, LRESULT *pResult);
};
