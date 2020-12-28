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
//CCommandPromptDlg
//�X�N���v�g�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#pragma once
#include "afxcmn.h"
#include "CalcDlg1.h"
#include "afxwin.h"



// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#define COMMAND_LINE_MAX	5000
#define CONDITION_STACK_MAX	1000
#define UNDO_MAX	50


// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

//�R�}���h���C���̃C���f�b�N�X
typedef struct COMMAND_LINE_STRUCT_{
	int		lineStartIndex;				//�s�J�n�C���f�b�N�X
	int		lineLength;					//�s�̒���
	int		lineStartIndexMultiChar;	//2�o�C�g������1�����ƃJ�E���g�����ꍇ�̍s�J�n�C���f�b�N�X
	int		lineLengthMultiChar;		//2�o�C�g������1�����ƃJ�E���g�����ꍇ�̍s�̒���
}COMMAND_LINE_STRUCT;


///*************************************
//         �N���X   ��   �`         
///*************************************

class CCommandPromptDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommandPromptDlg)

	enum { IDD = IDD_COMMAND_RPOMPT_DLG };// �_�C�A���O �f�[�^

public:
	CCommandPromptDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CCommandPromptDlg();
	bool OnBatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);

	

protected:
	CRichEditCtrl m_rich_edit;//�X�N���v�g���L������G�f�B�b�g�{�b�N�X
	CRichEditCtrl m_rich_edit2;//���b�Z�[�W��\������G�f�B�b�g�{�b�N�X
	CRichEditCtrl m_rich_edit3;//�v�Z���ʂ�\������G�f�B�b�g�{�b�N�X
	CRichEditCtrl m_rich_edit4;//�s�ԍ���\������G�f�B�b�g�{�b�N�X
	CListBox m_list_macro;
	CListCtrl m_var_list;
	CBitmap m_bmp_find;
	BATCH_INPUT			batchInBuffer;//�o�b�`���s�p�����^�ۑ���(BatchExecute������s����ƁA�㏑�������)
	BATCH_OUTPUT		batchOutBuffer;//�o�b�`���s�p�����^�ۑ���(BatchExecute������s����ƁA�㏑�������)
	CCalculate	m_calclate;//�R�}���h���C�����s�̃N���X
	CString		m_filename;//�X�N���v�g�t�@�C����
	CString		m_macro_description[CALC_MACRO_CNT_MAX];//�}�N���̉����
	bool		rich_edit_change_event_active_flg;//���b�`�G�f�B�b�g�̕ύX�C�x���g���A�N�e�B�u�ɂ��邩�ǂ���
	bool		blockMap[CALC_COMMAND_LINE_MAX]; //blockMap  �ʉߒ��̏����u���b�N�iif�`endif, for�`endfor�j�̏�Ԃ�\���}�b�v�B�@�s�ԍ��ŃA�N�Z�X����bool�^�z��̐擪�|�C���^
												 //			���Y�u���b�N�̒������s�ł���ꍇ��true�B�X�L�b�v����ꍇ��false
	char		*pUndoTbl[UNDO_MAX];//�X�N���v�g�̕ҏW������ޔ�����ꏊ
	int			undoID;//undo��
	bool		brakePoints[CALC_COMMAND_LINE_MAX];//�u���[�N�|�C���g

	virtual  void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	int		 get_command_lines(CString textBuffer, COMMAND_LINE_STRUCT commandLines[]);//�S�̂�\�������񂩂�s���Ƃ̃C���f�b�N�X�𒲂ׂ�
	void	 color_command_lines(bool isFirstRead=false);//�}�N���^������^����R�[�h��F��������
	void	 set_text_color(CRichEditCtrl	*pedit,int startMultiChar, int lenMultiChar, COLORREF color);//m_rich_edit�̃e�L�X�g�F�ݒ�
	void	 show_line_number(int index=-1, bool enhanceColor = false);
	bool	 comp_string_value(CString textBuffer, int index, int *pLength, int *pLengthMultiChar);//textBuffer��"�ň͂܂ꂽ�����񂩂ǂ�������
	bool	 comp_variable_definition(CString textBuffer, int index, int *pLength);
	void	 show_macro_discription(int macroid);//�}�N���̐����\��
	bool	 step_execute(int *p_program_counter, BATCH_INPUT *p_batch_in = NULL, BATCH_OUTPUT *p_batch_out = NULL, bool fast = false, bool *pJumped=NULL);//�X�e�b�v���s
	void	 show_all_result();//�S���ʕ\��
	void	 arrange_dlg_item();
	bool	 load_macro_description();
	bool	 open_script_file(char *folder_path=NULL, char *file_name=NULL);
	void	 init_variable_watch();
	void	 show_variables();
	void	 log_text_data();
	void     change_break_points(char *pPrevTxt=NULL, char *pPrsentTxt=NULL);



//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual  BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonCommandPromptDo();
	afx_msg void OnEnChangeRicheditCommandPrompt();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditCommandPromptProgramCounter();
	afx_msg void OnBnClickedButtonCommandPromptResultClear();
	afx_msg void OnBnClickedCheckCommandPromptDo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCommandPromptMenuNew();
	afx_msg void OnCommandPromptMenuOpen();
	afx_msg void OnCommandPromptMenuSave();
	afx_msg void OnCommandPromptMenuSaveas();
	afx_msg void OnCommandPromptMenuStepExe();
	afx_msg void OnCommandPromptMenuSeqExe();
	afx_msg void OnCommandPromptMenuClear();
	afx_msg void OnLbnSelchangeListCommandPromptMacro();
	afx_msg void OnLbnDblclkListCommandPromptMacro();
	afx_msg void OnCommandPromptMenuSample();
	afx_msg void OnEnUpdateRicheditCommandPrompt();
	afx_msg void OnEnChangeEditCommandPromptMacro();
	afx_msg void OnBnClickedCheckCommandPromptFind();
	afx_msg void OnBnClickedButtonCommandPromptTestDo();
	afx_msg void OnCommandPromptMenuReference();
	afx_msg void OnBnClickedButtonCommandPromptUndo();
	afx_msg void OnBnClickedButtonCommandPromptRedo();
	afx_msg void OnDestroy();
};
