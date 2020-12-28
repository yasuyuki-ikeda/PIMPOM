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
//CommandPromptDlg.cpp
//
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "CommandPromptDlg.h"
#include "shlwapi.h"
#include "Windowsx.h"

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

bool Is2ByteCode(CString str, int index)
{
	if(str.GetLength()<2)	return false;	

	char buf[2];
	buf[0] = str[index];
	buf[1] = str[index+1];

	if( _mbclen( (BYTE*)buf ) ==2){
		return true;
	}
	
	return false;
}

// CCommandPromptDlg �_�C�A���O

//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////

//�o�b�`���s�p
bool CCommandPromptDlg::OnBatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	if( ::IsWindowVisible(m_hWnd) == false)	return false;

	KillTimer(0);//��U�^�C�}�[��؂�

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, 0);//�v���O�����J�E���^������

	m_calclate.ClearAll();//���ʃN���A
	show_all_result();



	memset(blockMap, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//blockMap�̏�����
	for(;;)
	{
		int program_counter=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, &program_counter);//�v���O�����J�E���^�擾
		
		if (step_execute(&program_counter, in_param, out_param, true, blockMap)){//���s
			API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, program_counter);//�v���O�����J�E���^�X�V	
		}else{
			break;
		}
	}



	SetTimer(0,10,NULL);//�^�C�}�[�ĊJ

	return true;
}

/********************************************************************
�@  �\  ��  �� : �X�N���v�g�̕ҏW������ޔ�����
��    ��    �� : log_text_data
��          �� : 
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::log_text_data()
{
	CString str;
	m_rich_edit.GetWindowText(str);

	if (pUndoTbl[UNDO_MAX - 1] != NULL){
		delete[] pUndoTbl[UNDO_MAX - 1];
	}

	for (int n = UNDO_MAX-1; n > 0; n--)
	{
		pUndoTbl[n] = pUndoTbl[n - 1];
	}

	pUndoTbl[0] = new char[str.GetLength()+1 ];
	if (pUndoTbl[0])
	{
		strcpy(pUndoTbl[0], str.GetBuffer());
		str.ReleaseBuffer();
	}

	undoID = 0;
}


/********************************************************************
�@  �\  ��  �� :�����Ԃ̃e�L�X�g�̐F����
��    ��    �� : SetTextColor
��          �� : �����C���f�b�N�X�^������2�o�C�g������1�����ƃJ�E���g�����Ƃ��̏����Őݒ肷�邱��
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::set_text_color(CRichEditCtrl	*pedit,
									 int			startMultiChar,		//(in)�F���߂����Ԃ̊J�n�����C���f�b�N�X
									 int			lenMultiChar,		//(in)�F���߂����Ԃ̒���
									 COLORREF		color				//(in)�����F
									 )
{
	//return;

	long old_sel_start, old_sel_end ;
	pedit->GetSel(old_sel_start,old_sel_end);//���݂̑I��͈͂��擾����


	//�F�ݒ�
	CHARFORMAT2 cf;

	pedit->SetSel( startMultiChar, startMultiChar + lenMultiChar);//�I��͈͂�ς���

	pedit->GetDefaultCharFormat(cf);

	cf.crTextColor = color;//�F�w��

	pedit->SetSelectionCharFormat(cf);

	pedit->SetSel(old_sel_start,old_sel_end);//�I��͈͂����Ƃɂ��ǂ�

}

/********************************************************************
�@  �\  ��  �� : �s�ԍ��\���̐ݒ�
��    ��    �� : set_line_index
��          �� : �s�ԍ� -1�̏ꍇ�͂��ׂ�
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void	 CCommandPromptDlg::show_line_number(int index, bool enhanceColor)
{
	if (index == -1)
	{
		//���ׂ�
		CString strbuf(_T("")), str;
		for (int i = 0; i<CALC_COMMAND_LINE_MAX; i++) 
		{
			if (brakePoints[i])
			{
				str.Format("%04d@\r\n", i);
			}
			else {
				str.Format("%04d \r\n", i);
			}
			strbuf += str;
		}
		m_rich_edit4.SetWindowText(strbuf);
	}
	else if(0<= index && index<CALC_COMMAND_LINE_MAX)
	{
		//�w�肵���ԍ��̂�
		CString strbuf(_T(""));
		m_rich_edit4.GetWindowText(strbuf);

		if (brakePoints[index])
		{
			strbuf.SetAt(index * 7 + 4, '@');
		}
		else {
			strbuf.SetAt(index * 7 + 4, ' ');
		}
		m_rich_edit4.SetWindowText(strbuf);

		if (enhanceColor) {
			set_text_color(&m_rich_edit4, index * 6, 6, RGB(255, 0, 0));//�F�t��
		}
	}

}

/********************************************************************
�@  �\  ��  �� : �}�N���̐����\��
��    ��    �� : show_macro_discription
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::show_macro_discription(int macroid)
{
	if(macroid<0 || macroid>= CALC_MACRO_CNT_MAX)	return ;

	MACRO_DEF_STRUCT mcr_def={0};

	if( !m_calclate.GetMacroDefinision(macroid, &mcr_def))	return ;//�}�N����`���擾

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_MACRO_DISCRIPTION, m_macro_description[macroid]);//�}�N���̐�����\������


	//�}�N�����X�g���瓖�Y�}�N����I������
	for(int i=0 ; i<m_list_macro.GetCount() ; i++){
		CString str;
		m_list_macro.GetText(i,str);
		if(str==CString(mcr_def.name)){
			m_list_macro.SetCurSel(i);
			break;
		}
	}
}



/********************************************************************
�@  �\  ��  �� : �ϐ��錾
��    ��    �� : comp_variable_definition
��          �� : 
��    ��    �l : �����񂪌��������ꍇ��true
�@          �\ : "var "��T��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCommandPromptDlg::comp_variable_definition(CString  textBuffer,		//(in)��r�Ώە�����
												 int	  index,			//(in)������̃C���f�b�N�X(�����񂪌��������ꍇ�͂��̕��i�߂�)
												 int	  *pLength			//(out)
													)
{
	CString target=_T("var ");//�ϐ��錾�̃L�[���[�h


	for(int i=0 ; i<target.GetLength() ; i++){
		if( target[i] != textBuffer[index+i] ){
			return false;
		}
	}

	if(index>0){//�ϐ��錾�̑O�ɂ͂Ȃɂ����������Ă͂����Ȃ�
		if( isalnum((int)textBuffer[index-1]) ){
			return false;
		}
	}

	*pLength = target.GetLength();
	return true;
}


/********************************************************************
�@  �\  ��  �� : ""�ň͂܂�镶���񂩂ǂ�������ׂ�
��    ��    �� : comp_string_value
��          �� : 
��    ��    �l : �����񂪌��������ꍇ��true
�@          �\ : textBuffer��""�ň͂܂�镶���񂩂ǂ�������ׂ�
                 �ŏ��̕�����"�Ȃ�΁A����������s�܂ł͈̔͂𕶎���Ƃ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCommandPromptDlg::comp_string_value(	CString textBuffer,			//(in)��r�Ώە�����
											int		intex,				//(in)������̃C���f�b�N�X
											int		*pLength,			//(out)������̒���
											int		*pLengthMultiChar	//(out)2�o�C�g������1�����ƃJ�E���g�����Ƃ��̕�����̒���
									   )
{
	//1�����ڂ𒲂ׂ�
	if(textBuffer[intex]!='"'){
		return false;//�ŏ��̕�����"�łȂ���Ε�����ł͂Ȃ�
	}
	

	//2�����ڈȍ~�𒲂ׂ�
	int tmp_index = intex + 1;
	int count_multi_char = 1;

	while(textBuffer[tmp_index]!=0 && textBuffer[tmp_index]!='\r')//���s���I�[������܂Œ��ׂ�
	{
		if(textBuffer[tmp_index]=='"'){//"����������
			tmp_index++;
			count_multi_char++;
			break;
		}else{
			if(Is2ByteCode(textBuffer,tmp_index )){//2�o�C�g����
				tmp_index+=2;
				count_multi_char++;
			}else{//1�o�C�g����
				tmp_index++;
				count_multi_char++;
			}
		}
	}

	*pLength = tmp_index - intex;
	*pLengthMultiChar = count_multi_char;
	return true;
}


/********************************************************************
�@  �\  ��  �� : �s���Ƃ̃C���f�b�N�X�𒲂ׂ�
��    ��    �� : get_command_lines
��          �� : 
��    ��    �l : �s��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCommandPromptDlg::get_command_lines(CString				textString,			//(in)�G�f�B�b�g�R���g���[������擾�����e�L�X�g�S��
									   COMMAND_LINE_STRUCT	commandLines[])		//(out)�R�}���h���C��
{
	int commandLineCount=0;//�s��
	
	
	//�o�͈���������
	memset(commandLines, 0, sizeof(COMMAND_LINE_STRUCT)*COMMAND_LINE_MAX );


	//�s������ъe�s������̕������𐔂���

	int index=0;//textString�̃C���f�b�N�X
	int indexMultiChar=0;//2�o�C�g������1�����ƃJ�E���g�����ꍇ��textString�̃C���f�b�N�X

	while( index < textString.GetLength())
	{
		if(index+1 <textString.GetLength() && textString[index]=='\r' && textString[index+1]=='\n')//���s�R�[�h
		{
			if(commandLineCount+1 > COMMAND_LINE_MAX)
			{//�s������������
				API.MessageBox("too match lines (%d)", COMMAND_LINE_MAX);
				return -1;
			}

			commandLineCount++;//�R�}���h���C���i�߂�

			//�s�̊J�n�ʒu��ݒ�
			commandLines[ commandLineCount ].lineStartIndex = index+2;
			commandLines[ commandLineCount ].lineStartIndexMultiChar = indexMultiChar+1;

			//�s�̒�����������
			commandLines[ commandLineCount ].lineLength =0;
			commandLines[ commandLineCount ].lineLengthMultiChar = 0;

			index+=2;
			indexMultiChar++;
		}
		else{
			
			if( Is2ByteCode(textString,index ) )
			{//2�o�C�g����
				commandLines[ commandLineCount ].lineLength +=2;
				commandLines[ commandLineCount ].lineLengthMultiChar ++;

				index+=2;
				indexMultiChar++;
				
			}else{//1�o�C�g����
				commandLines[ commandLineCount ].lineLength ++;
				commandLines[ commandLineCount ].lineLengthMultiChar ++;

				index++;
				indexMultiChar++;
			}
		}
	}

	commandLineCount++;//�Ō��1�s�i�߂�


	return commandLineCount;
}

/********************************************************************
�@  �\  ��  �� : �R�����g�^�}�N�����^�e�L�X�g�ɉ����ĐF������
��    ��    �� : color_command_lines
��          �� : bool isFirstRead      (in)���߂ēǂݍ��ރe�L�X�g���ǂ���
��    ��    �l : �s��
�@          �\ : �EisFirstRead��true�̂Ƃ��́A���ׂĂ̍s�ɂ��ĐF�t������
�@�@�@�@�@�@�@�@ �EisFirstRead��false�̂Ƃ��A�O�񂱂̊֐����Ă΂ꂽ���ƃR�}���h�s����2�ȏ�ω����Ă���΁A���ׂĂ̍s�ɂ��ĐF�t������
		 �@�@�@�@�E��L�ȊO�̏ꍇ�́A�J�[�\��������s�̂ݐF�t������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::color_command_lines(bool isFirstRead)	
{
	COMMAND_LINE_STRUCT commandLines[COMMAND_LINE_MAX]={0};//�R�}���h���C���̃C���f�b�N�X
	int commandLineCount=0;//�s��
	static int prevCmmandLineCount=0;//�O�񂱂̊֐����Ă΂ꂽ�Ƃ��̍s��
	CString commandString;
	long CurrentSelEnd, CurrentSelStart;



	m_rich_edit.GetSel(CurrentSelStart, CurrentSelEnd);//���݃G�f�B�b�g�R���g���[���őI�𒆂̈ʒu�擾

	m_rich_edit.GetWindowText(commandString);//�G�f�B�b�g�R���g���[������e�L�X�g�擾

	commandLineCount = get_command_lines(commandString, commandLines);//�s�ԍ����擾

	if(isFirstRead){
		set_text_color(&m_rich_edit, 0, m_rich_edit.GetTextLength() , RGB(0,0,0));//�ŏ��̓ǂݍ��ݎ��͑S�s����U���ɂ���
	}


	for(int i=0 ; i<commandLineCount ; i++)//���ׂĂ̍s�ɂ��Ď��{
	{
		if(commandLines[i].lineLength==0)
		{
			//�󔒍s
		}
		else
		{//�󔒂łȂ��s

			if(!isFirstRead){//�ŏ��̓ǂݍ��ݎ��ȊO�ŁA�s���ω��s����1�s�ȉ��̏ꍇ�A�J�[�\���̂���s�ł̂ݐF�t������������
				if(abs(prevCmmandLineCount - commandLineCount)<=1){
					if(CurrentSelEnd < commandLines[i].lineStartIndexMultiChar-1   || CurrentSelStart >  commandLines[i].lineStartIndexMultiChar + commandLines[i].lineLengthMultiChar + 1)	continue;
				}

				set_text_color(&m_rich_edit, commandLines[i].lineStartIndexMultiChar, commandLines[i].lineLengthMultiChar, RGB(0,0,0));//�s�����ɂ���
			}

			CString commandStr = commandString.Mid(commandLines[i].lineStartIndex, commandLines[i].lineLength);//���ڍs

				//�R�}���h���C�����䕶�����T��
				int ctrlID, ctrlCodeLen=0;
				if( m_calclate.FindCtrlCode( commandStr, 0, &ctrlID ,&ctrlCodeLen) )
				{
					set_text_color(&m_rich_edit, commandLines[i].lineStartIndexMultiChar, ctrlCodeLen, RGB(0,0,255));//�R�}���h���C�����䕶�����ɂ���
				}
				

				//�}�N���A�������T��

				int idx = commandLines[i].lineStartIndex;
				int idxMultiChar  =commandLines[i].lineStartIndexMultiChar; 

				while( idx < commandLines[i].lineStartIndex + commandLines[i].lineLength )
				{
					int macroId;
					int len, multiLen;

					if( comp_variable_definition(commandString, idx, &len) )//�ϐ��錾���ǂ����`�F�b�N
					{
						set_text_color(&m_rich_edit, idxMultiChar, 3, RGB(0,0,255));//�ɂ���

						idx += len;
						idxMultiChar += len;
					}
					else 
					if( comp_string_value(commandString, idx, &len, &multiLen)  )//������������ǂ����`�F�b�N
					{	
						set_text_color(&m_rich_edit, idxMultiChar, multiLen, RGB(128,0,0));//����������C�����b�h�ɂ���

						idx += len;
						idxMultiChar += multiLen;
					}
					else if(m_calclate.IsComment(commandString,idx))//�R�����g���ǂ����`�F�b�N
					{
						int commentLenMultiChar = commandLines[i].lineLengthMultiChar - (idxMultiChar - commandLines[i].lineStartIndexMultiChar );
						
						set_text_color(&m_rich_edit, idxMultiChar, commentLenMultiChar, RGB(0,200,0));//�s��΂ɂ���

						break;//�R�����g�Ȃ�΃u���[�N
					}
					else 
					if(m_calclate.IsMacroName( commandString, idx, &macroId, &len) )//�}�N�����ǂ����`�F�b�N
					{
						set_text_color(&m_rich_edit, idxMultiChar, len, RGB(255,100,100));//�}�N����Ԃɂ���

						if(idxMultiChar <= CurrentSelEnd && idxMultiChar+len+1 >= CurrentSelEnd)
						{//�J�[�\�����}�N���ʒu�ɂ���΁A�������o��
							show_macro_discription(macroId);
						}

						idx += len;
						idxMultiChar += len;
					}
				    else
					{
						if(Is2ByteCode(commandString,idx ))
						{//2�o�C�g����
							idx+=2;
							idxMultiChar++;
						}else{//1�o�C�g����
							idx++;
							idxMultiChar++;
						}
					}
						
			}
		}
	}

	prevCmmandLineCount = commandLineCount;
}




/********************************************************************
�@  �\  ��  �� : �X�e�b�v���s
��    ��    �� : OnBnClickedButtonCommandPromptDo
��          �� : 
��    ��    �l : ������s�I����A���ɐi�ނ��Ƃ���� true
				 �G���[���̌����ŏ������~�܂�ꍇ��false
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCommandPromptDlg::step_execute(int			*p_program_counter,			//(in/out)�v���O�����J�E���^
									BATCH_INPUT		*p_batch_in,		//(in)
									BATCH_OUTPUT	*p_batch_out,		//(out)
									bool			fast,
									bool			*pJumped				//(in/out)
									)
{
	CString				comStrBuffer(""), tmpErr("");
	COMMAND_LINE_STRUCT commandLines[COMMAND_LINE_MAX]={0};//�R�}���h���C���̃C���f�b�N�X
	int					commandLineCount=0;	//�s��
	bool				ret=true;

	
	m_rich_edit.GetWindowText(comStrBuffer);//�G�f�B�b�g�R���g���[������S�e�L�X�g���擾

	commandLineCount = get_command_lines(comStrBuffer, commandLines);//�S�R�}���h�s�擾

	//�v���O�����J�E���^�̍s�ԍ���ԐF�A����ȊO�̍s�ԍ����O���[�ɂ���
	show_line_number();
	show_line_number(*p_program_counter, true);

	//���ׂẴR�}���h�s��o�^
	CString comStr[COMMAND_LINE_MAX];
	for(int i=0 ; i<commandLineCount ; i++){
		comStr[i] = comStrBuffer.Mid(commandLines[i].lineStartIndex, commandLines[i].lineLength);
	}

	if( ret = m_calclate.RegisterCommandString(comStr, commandLineCount, &tmpErr) )//�R�}���h���
	{
		ret = m_calclate.ExecuteAPI(*p_program_counter , &tmpErr, p_program_counter, p_batch_in, p_batch_out, 
			API.GetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_AUTO_DRAWIMAGE),
			pJumped
			) ;//1�s���s
	}else{
		//�R�}���h��͎��̃G���[���b�Z�[�W�͕\������
		if(!tmpErr.IsEmpty()){
			p_batch_out->state = BATCH_EXEC_STATE_STOP;
			AfxMessageBox(tmpErr);
		}
	}


	//�o�b�`���s�̃p�����^��ۑ����Ă���
	if (p_batch_in != &batchInBuffer){
		memcpy(&batchInBuffer, p_batch_in, sizeof(BATCH_INPUT));
	}
	if (p_batch_out != &batchOutBuffer){
		memcpy(&batchOutBuffer, p_batch_out, sizeof(BATCH_OUTPUT));
	}

	if (!fast)
	{
		//�S���ʕ\��
		show_all_result();

		//�ϐ��ꗗ�\��
		show_variables();
	}

	return ret;

}

/********************************************************************
�@  �\  ��  �� : �S���ʕ\��
��    ��    �� : show_all_result
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::show_all_result()
{
	CString	 comStrBuffer(""), resultStrBuffer(""), errStrBuffer(""), tmpStr, tmpErr;

	//�S���ʕ\��
	for(int i=0 ; i<m_calclate.GetCommandLineLen() ; i++)
	{
		double		number;
		char		strbuf[CALC_STRING_LEN_MAX];
		DWORD		elapsed_time = 0;
		int			result_type = m_calclate.GetResult(i, &number, strbuf, &tmpErr, &elapsed_time);//���ʎ擾	
		
		//�G���[���b�Z�[�W����ʕ\���ɔ��f
		errStrBuffer += tmpErr + CString("\r\n");
		
		//���ʂ���ʕ\���ɔ��f
		if(result_type>=0){//���ʎ擾����
			if (result_type == COM_TYPE_NUMBER)		tmpStr.Format("Res(%d)=%lf (%d[ms])", i, number, elapsed_time);
			else									tmpStr.Format("Res(%d)=\"%s\" (%d[ms])", i, strbuf, elapsed_time);
		}else{//���ʎ擾���s
			tmpStr.Empty();
		}
		resultStrBuffer += tmpStr + CString("\r\n");

	}

	m_rich_edit2.SetWindowText(errStrBuffer);//�G���[���b�Z�[�W�ꗗ
	m_rich_edit3.SetWindowText(resultStrBuffer);//���ʈꗗ
}

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�A�C�e���̈ړ�
��    ��    �� : arrange_dlg_item
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::arrange_dlg_item()
{
	RECT clientRct={0};//�N���C�A���g�̈��\����`
	int clientWidth, clientHeight, offsetx, offsety;

	::GetClientRect(m_hWnd, &clientRct);//�N���C�A���g�̈��\����`���擾

	clientWidth = (clientRct.right - clientRct.left);
	clientHeight = (clientRct.bottom - clientRct.top);
	offsetx = 2;
	offsety = 0;



	//�_�C�A���O�A�C�e��
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT),					offsetx + clientWidth*5/100,	offsety,					clientWidth*55/100-2, clientHeight*70/100-4, true);//�R�}���h�v�����v�g
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT2),				offsetx + clientWidth*60/100,	offsety,					clientWidth*20/100-2, clientHeight*70/100-4, true);//�G���[���b�Z�[�W
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT3),				offsetx + clientWidth*80/100,	offsety,					clientWidth*20/100-2, clientHeight*70/100-4, true);//���ʂ̒l
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT4),				offsetx,					offsety,					clientWidth*5/100-2,  clientHeight*70/100-4, true);//�s�ԍ�
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_COMMAND_PROMPT_FIND),				offsetx,					offsety+clientHeight*70/100,   20, 20, true);//�}�N������
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_COMMAND_PROMPT_MACRO),				offsetx+20,					offsety+clientHeight*70/100,   clientWidth*30/100-2-20, 20, true);//�}�N������
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_LIST_COMMAND_PROMPT_MACRO),				offsetx,					offsety+clientHeight*70/100+20,	clientWidth*30/100-2, clientHeight*25/100-20, true);//�}�N���ꗗ
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_EDIT_COMMAND_PROMPT_MACRO_DISCRIPTION),	offsetx + clientWidth*30/100,	offsety+clientHeight*70/100,	clientWidth*50/100-2, clientHeight*25/100, true);//�}�N������
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_LIST_VARIABLE),							offsetx + clientWidth*80/100,	offsety+clientHeight*70/100,	clientWidth*20/100-2, clientHeight*25/100, true);//�ϐ��E�H�b�`


	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_STATIC_COMMAND_PROMPT_PROGRAM_COUNTER),	offsetx,	offsety+clientHeight*95/100, 80, 20, true);//�v���O�����J�E���^�̃L���v�V����
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER),	offsetx + 80,   offsety+clientHeight*95/100, 40, 20, true);//�v���O�����J�E���^�̒l
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_DO),	offsetx + 120,   offsety+clientHeight*95/100, 80, 20, true);//�X�e�b�v���s�v�{�^��
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO),	offsetx + 200,   offsety+clientHeight*95/100, 80, 20, true);//�u�A�����s�v�`�F�b�N�{�b�N�X
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_RESULT_CLEAR),	offsetx + 280,   offsety+clientHeight*95/100, 80, 20, true);//�u���ʃN���A�v�{�^��
	//::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_COMMAND_PROMPT_AUTO_DRAWIMAGE), offsetx + 360, offsety + clientHeight * 95 / 100, 200, 20, true);//�����ĕ`��{�^��
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_COMMAND_PROMPT_UNDO), offsetx + 360, offsety + clientHeight * 95 / 100, 60, 20, true);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_COMMAND_PROMPT_REDO), offsetx + 360 + 60, offsety + clientHeight * 95 / 100, 60, 20, true);
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_COMMAND_PROMPT_TEST_DO), offsetx + 560, offsety + clientHeight * 95 / 100, 60, 20, true);//�e�X�g���s�{�^��
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_EDIT_COMMAND_PROMPT_TEST), offsetx + 620, offsety + clientHeight * 95 / 100, 500, 20, true);//�e�X�g���s�p�R�}���h�v�����v�g

	if(API.GetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO))
	{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT),false);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_DO),false);//�u�X�e�b�v���s�v�{�^���������Ȃ�����
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_RESULT_CLEAR),false);//�u���ʃN���A�v�{�^���������Ȃ�����
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER),false);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_LIST_COMMAND_PROMPT_MACRO),false);//�}�N���ꗗ���X�g���g���Ȃ�����
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_RICHEDIT_COMMAND_PROMPT),true);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_DO),true);//�u�X�e�b�v���s�v�{�^�����g�p�\�ɂ���
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_COMMAND_PROMPT_RESULT_CLEAR),true);//�u���ʃN���A�v�{�^�����g�p�\�ɂ���
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER),true);
		::EnableWindow(::GetDlgItem(m_hWnd,IDC_LIST_COMMAND_PROMPT_MACRO),true);//�}�N���ꗗ���X�g���g����悤�ɂ���
	}
}


/********************************************************************
�@  �\  ��  �� : �ϐ��E�H�b�`���X�g
��    ��    �� : init_variable_watch
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::init_variable_watch()
{
    LVCOLUMN    lvc;
    char       caption[32];

	m_var_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O


	//�ϐ���
	sprintf(caption, "�ϐ�");
    lvc.iSubItem    = 0;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 80;          // ����
    m_var_list.InsertColumn(0, &lvc);

	//�l
	sprintf(caption, "�l");
    lvc.iSubItem    = 1;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 80;          // ����
    m_var_list.InsertColumn(1, &lvc);
   
}



/********************************************************************
�@  �\  ��  �� : �ϐ��̃��X�g�\��
��    ��    �� : show_variables
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::show_variables()
{
	LVITEM       lvi={0};
	lvi.mask = LVIF_TEXT;
	char        buf[1024];
	

	m_var_list.DeleteAllItems();//��U���ׂẴ��X�g�v�f���N���A


	CALC_RESULT_STRUCT varres={0};
	char	varname[CALC_VARIABLE_NAME_MAX]={0};

	int varID=0;
	while( m_calclate.GetVariable(varID,&varres,varname) )
	{
			//�ϐ���
			lvi.iItem = varID;
			lvi.iSubItem = 0;
			sprintf(buf, "%s", varname);
			lvi.pszText = buf;
			m_var_list.InsertItem(&lvi);//���X�g�}��

			//�l
			lvi.iItem = varID;
			lvi.iSubItem = 1;
			if(varres.type == COM_TYPE_NUMBER){
				sprintf(buf, "%lf", varres.number);
			}else if(varres.type = COM_TYPE_STRING){
				sprintf(buf, "%s", varres.str);
			}
			lvi.pszText = buf;
			m_var_list.SetItem(&lvi);

		varID++;
	}

	for (int i = 0; i < BATCH_IN_PARAM_NUM; i++)
	{
		//�ϐ���
		lvi.iItem = varID;
		lvi.iSubItem = 0;
		sprintf(buf, "BATCH_IN[%d]", i);
		lvi.pszText = buf;
		m_var_list.InsertItem(&lvi);//���X�g�}��

		//�l
		lvi.iItem = varID;
		lvi.iSubItem = 1;
		sprintf(buf, "%s", batchInBuffer.param[i]);
		lvi.pszText = buf;
		m_var_list.SetItem(&lvi);

		varID++;
	}

	for (int i = 0; i < BATCH_OUT_PARAM_NUM; i++)
	{
		//�ϐ���
		lvi.iItem = varID;
		lvi.iSubItem = 0;
		sprintf(buf, "BATCH_OUT[%d]", i);
		lvi.pszText = buf;
		m_var_list.InsertItem(&lvi);//���X�g�}��

		//�l
		lvi.iItem = varID;
		lvi.iSubItem = 1;
		sprintf(buf, "%s", batchOutBuffer.result[i]);
		lvi.pszText = buf;
		m_var_list.SetItem(&lvi);

		varID++;
	}
}


/********************************************************************
�@  �\  ��  �� : �}�N���̉��������ǂݍ���
��    ��    �� : load_macro_description
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCommandPromptDlg::load_macro_description()
{
	FILE *fp = fopen(API.GetDefaultPath() + "..\\..\\..\\Doc\\macro_description.txt", "rt");//�t�@�C�����J��
	if(fp)
	{
		char tmp[CALC_STRING_LEN_MAX]={0};
		CString lineBuffer[CALC_COMMAND_LINE_MAX];
		int line_number=0;

		//�t�@�C������S�s�ǂݍ���
		for(line_number=0; line_number<CALC_COMMAND_LINE_MAX; line_number++){
			if(fgets(tmp, CALC_STRING_LEN_MAX, fp) == NULL){
				break;
			}else{
				lineBuffer[line_number].Format("%s\r\n", tmp);
			}
		}

		fclose(fp);



		//�}�N���v�f���Ƃɐ�������T��
		for(int i=0 ; i<CALC_MACRO_CNT_MAX ; i++)
		{
			m_macro_description[i].Empty();

			MACRO_DEF_STRUCT mac={0};
			m_calclate.GetMacroDefinision(i, &mac);//�}�N�����擾

			if(strlen(mac.name)<=0)	continue;

			int start_line=line_number;
			for(int j=0 ; j<line_number ; j++){
				if( lineBuffer[j].Find( CString(mac.name) + CString("("), 0) >=0 ){//�}�N�����������ꂽ�s��T��
					start_line = j;
					break;
				}
			}

			for(int j=start_line ; j<line_number ; j++){
				if( lineBuffer[j].Find( CString("======"), 0) >=0  ){//�}�N�������̏I�[��T��
					break;
				}else{
					m_macro_description[i] += lineBuffer[j];
				}
			}
		}

		return true;
	}else{
		return false;
	}
}

/********************************************************************
�@  �\  ��  �� : �X�N���v�g�t�@�C�����J��
��    ��    �� : open_script_file
��          �� : (in)folder_path
                 (in)file_name
��    ��    �l : ���������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCommandPromptDlg::open_script_file(char folder_path[], char file_name[])
{
	int	file_number;
	CString filetype[2], path_str, file_name_str;
	FILE *fp;

	if (file_name == NULL)
	{
		filetype[0].LoadString(IDS_FILETYPE_TXT);
		filetype[1].LoadString(IDS_FILETYPE_ANY);
		if (!API.OpenFileDialog(1, filetype[0] + filetype[1] + _T("|"), &path_str, &file_name_str, &file_number, folder_path))	return false;//�t�@�C���_�C�A���O���J���ăt�@�C���p�X���擾
	}
	else
	{
		file_name_str = CString(file_name);
		path_str = CString(folder_path);
	}


	fp = fopen(path_str + file_name_str, "rt");//�t�@�C�����J��
	if(fp)
	{
		CString allStr("");
		char tmp[CALC_STRING_LEN_MAX]={0};

		for(int i=0 ; i<CALC_COMMAND_LINE_MAX ; i++)
		{
			if( fgets(tmp, CALC_STRING_LEN_MAX-1, fp) != NULL ){//�e�L�X�g��1�s�ǂݍ���
				allStr += CString(tmp);//
			}
			else
			{
				rich_edit_change_event_active_flg = false;//���b�`�G�f�B�b�g�̕ҏW�C�x���g��؂�

				OnBnClickedButtonCommandPromptResultClear();//���ʃN���A

				m_rich_edit.SetWindowText(allStr);//�ǂݍ��񂾃e�L�X�g���G�f�B�b�g�ɔ��f

				log_text_data();

				color_command_lines(true);//�����F��

				rich_edit_change_event_active_flg=true;//���b�`�G�f�B�b�g�̕ҏW�C�x���g���A�N�e�B�x�[�g

				m_filename = path_str + file_name_str;//�ǂݍ��񂾃t�@�C�������c��

				::SetWindowText(m_hWnd, m_filename + CString(" -�X�N���v�g"));//�t�@�C�������_�C�A���O�̃L���v�V�����ɔ��f����
				break;
			}
		}

		fclose(fp);

		return true;
	}else{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
//�C�x���g�n���h��
//////////////////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(CCommandPromptDlg, CDialog)

CCommandPromptDlg::CCommandPromptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommandPromptDlg::IDD, pParent)
{
	m_filename = _T("");
	AfxInitRichEdit2();
}

CCommandPromptDlg::~CCommandPromptDlg()
{
	//���������
	for (int i = 0; i < UNDO_MAX; i++){
		if (pUndoTbl[i] != NULL){
			delete[] pUndoTbl[i];
		}
	}

	m_bmp_find.DeleteObject();
}

void CCommandPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_COMMAND_PROMPT, m_rich_edit);
	DDX_Control(pDX, IDC_RICHEDIT_COMMAND_PROMPT2, m_rich_edit2);
	DDX_Control(pDX, IDC_RICHEDIT_COMMAND_PROMPT3, m_rich_edit3);
	DDX_Control(pDX, IDC_RICHEDIT_COMMAND_PROMPT4, m_rich_edit4);
	DDX_Control(pDX, IDC_LIST_COMMAND_PROMPT_MACRO, m_list_macro);
	DDX_Control(pDX, IDC_LIST_VARIABLE, m_var_list);
}


BEGIN_MESSAGE_MAP(CCommandPromptDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_DO, &CCommandPromptDlg::OnBnClickedButtonCommandPromptDo)
	ON_EN_CHANGE(IDC_RICHEDIT_COMMAND_PROMPT, &CCommandPromptDlg::OnEnChangeRicheditCommandPrompt)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, &CCommandPromptDlg::OnEnChangeEditCommandPromptProgramCounter)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_RESULT_CLEAR, &CCommandPromptDlg::OnBnClickedButtonCommandPromptResultClear)
	ON_BN_CLICKED(IDC_CHECK_COMMAND_PROMPT_DO, &CCommandPromptDlg::OnBnClickedCheckCommandPromptDo)
	ON_WM_SIZE()
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_NEW, &CCommandPromptDlg::OnCommandPromptMenuNew)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_OPEN, &CCommandPromptDlg::OnCommandPromptMenuOpen)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_SAVE, &CCommandPromptDlg::OnCommandPromptMenuSave)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_SAVEAS, &CCommandPromptDlg::OnCommandPromptMenuSaveas)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_STEP_EXE, &CCommandPromptDlg::OnCommandPromptMenuStepExe)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_SEQ_EXE, &CCommandPromptDlg::OnCommandPromptMenuSeqExe)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_CLEAR, &CCommandPromptDlg::OnCommandPromptMenuClear)
	ON_LBN_SELCHANGE(IDC_LIST_COMMAND_PROMPT_MACRO, &CCommandPromptDlg::OnLbnSelchangeListCommandPromptMacro)
	ON_LBN_DBLCLK(IDC_LIST_COMMAND_PROMPT_MACRO, &CCommandPromptDlg::OnLbnDblclkListCommandPromptMacro)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_SAMPLE, &CCommandPromptDlg::OnCommandPromptMenuSample)
	ON_EN_UPDATE(IDC_RICHEDIT_COMMAND_PROMPT, &CCommandPromptDlg::OnEnUpdateRicheditCommandPrompt)
	ON_EN_CHANGE(IDC_EDIT_COMMAND_PROMPT_MACRO, &CCommandPromptDlg::OnEnChangeEditCommandPromptMacro)
	ON_BN_CLICKED(IDC_CHECK_COMMAND_PROMPT_FIND, &CCommandPromptDlg::OnBnClickedCheckCommandPromptFind)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_TEST_DO, &CCommandPromptDlg::OnBnClickedButtonCommandPromptTestDo)
	ON_COMMAND(ID_COMMAND_PROMPT_MENU_REFERENCE, &CCommandPromptDlg::OnCommandPromptMenuReference)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_UNDO, &CCommandPromptDlg::OnBnClickedButtonCommandPromptUndo)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PROMPT_REDO, &CCommandPromptDlg::OnBnClickedButtonCommandPromptRedo)
	ON_WM_DESTROY()
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
BOOL CCommandPromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(AfxGetApp()->LoadIcon(IDI_RUN), TRUE);			// �A�C�R����ݒ�
	
	m_bmp_find.LoadBitmap(IDB_FIND);
	CButton	*p_button = (CButton*)GetDlgItem(IDC_CHECK_COMMAND_PROMPT_FIND);
	p_button->SetBitmap((HBITMAP)m_bmp_find);
	

	arrange_dlg_item();

	init_variable_watch();

	load_macro_description();

	//�C�x���g�}�X�N�ݒ�
	m_rich_edit.SetEventMask(ENM_CHANGE);

	//�f�t�H���g�t�H�[�}�b�g�ݒ�
	CHARFORMAT2 cf;
	m_rich_edit.GetDefaultCharFormat(cf);
	cf.dwMask = CFM_COLOR | CFM_SIZE | CFM_FACE |CFM_CHARSET ;
	cf.yHeight = 15 * 12;//�����T�C�Y
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.crTextColor = RGB(0,0,0);//�����F(��)
	cf.bCharSet = SHIFTJIS_CHARSET;//�����R�[�h�Z�b�g
	lstrcpy(cf.szFaceName, TEXT("�l�r �S�V�b�N"));//�t�H���g

	m_rich_edit.SetDefaultCharFormat(cf);
	m_rich_edit2.SetDefaultCharFormat(cf);
	m_rich_edit3.SetDefaultCharFormat(cf);

	cf.crTextColor = RGB(128,128,128);//�O���[
	m_rich_edit4.SetDefaultCharFormat(cf);



	//�}�N���ꗗ������
	for(int i=0 ; i<CALC_MACRO_CNT_MAX ; i++){
		MACRO_DEF_STRUCT mac;
		m_calclate.GetMacroDefinision(i, &mac);
		if(strlen(mac.name)>0){
			m_list_macro.AddString( CString(mac.name));
		}
	}

	memset(&batchInBuffer, 0, sizeof(BATCH_INPUT));
	memset(&batchOutBuffer, 0, sizeof(BATCH_OUTPUT));

	m_calclate.ClearAll();//�R�}���h������

	memset(blockMap, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//PassedIfIDMap�̏�����

	memset(brakePoints, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//brakePoints�̏�����


	//�X�N���v�g�̕ҏW������ޔ�����ꏊ�̊m��
	for (int i = 0; i < UNDO_MAX; i++){
		pUndoTbl[i] = NULL;
	}
	undoID = 0;
	log_text_data();

	API.SetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_AUTO_DRAWIMAGE, true);//�����ĕ`��ON

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, 0);//�v���O�����J�E���^������

	show_line_number();//�s�ԍ��\��������

	rich_edit_change_event_active_flg=true;//���b�`�G�f�B�b�g�̕ҏW���A�N�e�B�x�[��

	SetTimer(0,10,NULL);//�^�C�}�[�J�n
	
						
	//�o�b�N�A�b�v�t�@�C����ǂݍ���
	CString path = API.GetDefaultPath();
	open_script_file(path.GetBuffer(), "script_backup.txt");
	path.ReleaseBuffer();
	m_rich_edit.SetSel(0, 0);



	return true;
}


/********************************************************************
�@  �\  ��  �� : �_�C�A���O�̔j��
��    ��    �� : OnDestroy
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnDestroy()
{
	//�e�L�X�g�o�b�N�A�b�v
	CString str;
	m_rich_edit.GetWindowText(str);
	FILE *fp = fopen(API.GetDefaultPath() + "\\script_backup.txt", "wt");
	if (fp) {
		fwrite(str.GetBuffer(), 1, str.GetLength(), fp);
		str.ReleaseBuffer();
		fclose(fp);
	}

	CDialog::OnDestroy();
}

/********************************************************************
�@  �\  ��  �� :�L�[�{�[�h����̓���
��    ��    �� : PreTranslateMessage
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CCommandPromptDlg::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message == WM_KEYDOWN )
	{//�L�[����

		switch(pMsg->wParam){
			case VK_ESCAPE://Esc
				return  true;

			case VK_TAB://tab
				if (pMsg->hwnd == ::GetDlgItem(m_hWnd, IDC_RICHEDIT_COMMAND_PROMPT))
				{
					//�X�N���v�g���L������G�f�B�b�g�{�b�N�X��tab�������ꂽ�ꍇ
					//�_�C�A���O�A�C�e���I����ς���̂ł͂Ȃ��A�G�f�B�b�g�{�b�N�X��tab��}������
					m_rich_edit.ReplaceSel(_T("\t"));
					return true;
				}
				break;
			default:
				break;
		}
	}

	else if (pMsg->message == WM_LBUTTONDOWN)
	{//�}�E�X���{�^���N���b�N
		if (pMsg->hwnd == ::GetDlgItem(m_hWnd, IDC_RICHEDIT_COMMAND_PROMPT4))
		{//�s�ԍ�
			CString comStrBuffer;
			RECT win_rct;

			m_rich_edit4.GetWindowText(comStrBuffer);//�G�f�B�b�g�R���g���[������S�e�L�X�g���擾
			m_rich_edit4.GetWindowRect(&win_rct);//�G�f�B�b�g�R���g���[���̍��W�擾
		
			int charStart = 0, lineCnt=0;
			int prevCharPos = m_rich_edit4.GetCharPos(0).y + win_rct.top;
			for (int n = 1; n < comStrBuffer.GetLength()  ; n++)
			{
				int charPos = m_rich_edit4.GetCharPos(n).y + win_rct.top;

				if (pMsg->pt.y < charPos)
				{//�s�ʒu���N���b�N�ʒu�𒴂����ꍇ

					brakePoints[lineCnt] = !brakePoints[lineCnt];//�u���[�N�|�C���g���]
					show_line_number(lineCnt);//�s�ԍ��\���ɔ��f
					break;
				}
				else if (prevCharPos < charPos)
				{//���s��
					charStart = n;
					lineCnt++;
				}

				prevCharPos = charPos;
			}
		}
	}





	return CDialog::PreTranslateMessage(pMsg);
}


/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g���e�ύX�ɔ����u���[�N�|�C���g�ύX
��    ��    �� : change_break_points
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::change_break_points(char *pPrevStr, char *pPresentStr)
{
	bool		tmpbrakePoints[CALC_COMMAND_LINE_MAX] = {0};

	CString str;
	m_rich_edit.GetWindowText(str);
	char *pBuffer = str.GetBuffer();


	if (pPrevStr == NULL)
	{
		pPrevStr = pUndoTbl[0];
	}

	if (pPresentStr == NULL)
	{
		pPresentStr = pBuffer;
	}

	if (pUndoTbl[0])
	{//�ЂƂO�̗����Ɣ�r

		int prevStrLen = (int)strlen(pPrevStr);
		int presentStrLen = (int)strlen(pPresentStr);

		//�O����v��r
		int lineCnt = 0;
		for (int i = 0; i<prevStrLen && i<presentStrLen; i++)
		{
			if (pPrevStr[i] != pPresentStr[i])
			{
				break;
			}
			else if(pPresentStr[i] =='\n')
			{//���s
				tmpbrakePoints[lineCnt] = brakePoints[lineCnt];//���̍s�̃u���[�N�|�C���g���R�s�[
				lineCnt++;//�s�ԍ���i�߂�
			}
		}

		if (strcmp(pPrevStr, pPresentStr) != 0 && presentStrLen>0)
		{
			//�s�ԍ��𐔂���
			int prevLines = 0;
			int presentLines = 0;
			for (int i = 0; i < prevStrLen ; i++)
			{
				if (pPrevStr[i] == '\n') { prevLines++; }
			}

			
			for (int i = 0; i < presentStrLen; i++)
			{
				if (pPresentStr[i] == '\n') { presentLines++; }
			}

			//�����v��r
			int prevLineCnt = prevLines-1;
			int presentLineCnt = presentLines-1;
			for (int i = prevStrLen - 1, j = presentStrLen - 1; i>=0 && j>=0; i--, j--)
			{
				if (pPrevStr[i] != pPresentStr[j])
				{
					break;
				}
				else if (pPresentStr[j] == '\n')
				{//���s
					tmpbrakePoints[presentLineCnt] = brakePoints[prevLineCnt];//���̍s�̃u���[�N�|�C���g���R�s�[
					prevLineCnt--;//�s�ԍ���i�߂�
					presentLineCnt--;
				}
			}
		}

		memcpy(brakePoints, tmpbrakePoints, sizeof(bool)*CALC_COMMAND_LINE_MAX);
		show_line_number();//�s�ԍ��\��

	}

	str.ReleaseBuffer();
}



/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g���e�ύX
��    ��    �� : OnEnChangeRicheditCommandPrompt
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnEnChangeRicheditCommandPrompt()
{
	if(rich_edit_change_event_active_flg){
		color_command_lines();	//�R�}���h���C���̐F����
		//API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, 0);//�v���O�����J�E���^������

		change_break_points();//�u���[�N�|�C���g�̕ύX

		log_text_data();
	}
}





/********************************************************************
�@  �\  ��  �� :
��    ��    �� : OnTimer
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/

void CCommandPromptDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);//��U�^�C�}�[��؂�
	
	//�A�����s
	if(API.GetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO))
	{
		int program_counter=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, &program_counter);//�v���O�����J�E���^�擾

		if (brakePoints[program_counter])
		{//�u���[�N�|�C���g
			API.SetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_DO, false);
			arrange_dlg_item();

			//�v���O�����J�E���^�̍s�ԍ���ԐF�A����ȊO�̍s�ԍ����O���[�ɂ���
			show_line_number();
			show_line_number(program_counter, true);

			API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, program_counter+1);//�v���O�����J�E���^���ЂƂi�߂Ă���	
			return;
		}

		if (step_execute(&program_counter, &batchInBuffer, &batchOutBuffer, false, blockMap))
		{//���s
			API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, program_counter);//�v���O�����J�E���^�X�V	
		}else{
			API.SetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO,false);
			arrange_dlg_item();
		}
	}
	else
	{
		//�e�G�f�B�b�g�̃X�N���[���ʂ����낦��
		int line = m_rich_edit.GetFirstVisibleLine();
		int line2 = m_rich_edit2.GetFirstVisibleLine();
		int line3 = m_rich_edit3.GetFirstVisibleLine();
		int line4 = m_rich_edit4.GetFirstVisibleLine();
		
		m_rich_edit2.LineScroll(line-line2);
		m_rich_edit3.LineScroll(line-line3);
		m_rich_edit4.LineScroll(line-line4);
	}

	SetTimer(0,10,NULL);//�^�C�}�[�ĊJ

	CDialog::OnTimer(nIDEvent);
}



void CCommandPromptDlg::OnEnChangeEditCommandPromptProgramCounter()
{
	// TODO:  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A
	// �܂��ACDialog::OnInitDialog() �֐����I�[�o�[���C�h���āAOR ��Ԃ� ENM_CHANGE
	// �t���O���}�X�N�ɓ���āACRichEditCtrl().SetEventMask() ���Ăяo���Ȃ�����A
	// �R���g���[���́A���̒ʒm�𑗐M���܂���B

	// TODO:  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B
}


/********************************************************************
�@  �\  ��  �� : �u�X�e�b�v���s�v�{�^������
��    ��    �� : OnBnClickedButtonCommandPromptDo
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptDo()
{
	int	program_counter=0;//�v���O�����J�E���^

	API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, &program_counter);//�v���O�����J�E���^�擾

	step_execute(&program_counter, &batchInBuffer, &batchOutBuffer, false, blockMap);//�X�e�b�v���s

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, program_counter);//�v���O�����J�E���^�X�V

}

/********************************************************************
�@  �\  ��  �� : �u���ʃN���A�v�{�^������
��    ��    �� : OnBnClickedButtonCommandPromptResultClear
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptResultClear()
{
	m_calclate.ClearAll();

	memset(&batchInBuffer, 0, sizeof(BATCH_INPUT));
	memset(&batchOutBuffer, 0, sizeof(BATCH_OUTPUT));

	show_all_result();

	//�ϐ��ꗗ�\��
	show_variables();

	API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_PROGRAM_COUNTER, 0);//�v���O�����J�E���^������

	set_text_color(&m_rich_edit4, 0 , m_rich_edit4.GetTextLength(), RGB(128,128,128));//�s�ԍ������ׂăO���[�ɂ���

	memset(blockMap, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//PassedIfIDMap�̏�����
}

/********************************************************************
�@  �\  ��  �� : �u�A�����s�v�`�F�b�N�{�b�N�X����
��    ��    �� : OnBnClickedButtonCommandPromptResultClear
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnBnClickedCheckCommandPromptDo()
{
	if(API.GetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO))
	{
		arrange_dlg_item();

		KillTimer(0);//��U�^�C�}�[��؂�

		//OnBnClickedButtonCommandPromptResultClear();//���ʃN���A

		SetTimer(0,10,NULL);//�^�C�}�[�ĊJ
	}else{
		arrange_dlg_item();
	}
}


/********************************************************************
�@  �\  ��  �� : �u�e�X�g���s�v�{�^������
��    ��    �� : OnBnClickedButtonCommandPromptTestDo
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptTestDo()
{
	CString command_str, err_str;
	CCalculate		calc;


	API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_TEST, &command_str);//�R�}���h�擾

	if (calc.RegisterCommandString(&command_str, 1, &err_str)){
		calc.ExecuteAPI(0, &err_str, NULL, NULL, NULL,
			API.GetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_AUTO_DRAWIMAGE)
			);//1�s���s
	}
}


/********************************************************************
�@  �\  ��  �� : �E�B���h�E�T�C�Y�ύX
��    ��    �� : OnSize
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	arrange_dlg_item();
}


/********************************************************************
�@  �\  ��  �� : �u�t�@�C���^�V�K�쐬�v���j���[
��    ��    �� : OnCommandPromptMenuNew
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuNew()
{
	OnBnClickedButtonCommandPromptResultClear();

	

	CString str(" ");
	m_rich_edit.SetWindowText(str);//�G�f�B�b�g�N���A

	memset(brakePoints, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//brakePoints�̏�����
	show_line_number();//�s�ԍ��\��
}


/********************************************************************
�@  �\  ��  �� : �u�t�@�C���^�J���v���j���[
��    ��    �� : OnCommandPromptMenuNew
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuOpen()
{
	if (open_script_file())
	{
		memset(brakePoints, 0, sizeof(bool)*CALC_COMMAND_LINE_MAX);//brakePoints�̏�����
		show_line_number();//�s�ԍ��\��
	}
}


/********************************************************************
�@  �\  ��  �� : �u�t�@�C���^�㏑���ۑ��v���j���[
��    ��    �� : OnCommandPromptMenuSave
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuSave()
{
	if(!m_filename.IsEmpty())
	{	
		//�ߋ��ɊJ����or�ۑ������t�@�C����������
		if( PathFileExists( m_filename) )
		{
			//�t�@�C�������݂���
			FILE *fp = fopen(m_filename, "wb");//�t�@�C�����J��
			if(fp)
			{
				CString allStr("");
				
				m_rich_edit.GetWindowText(allStr);//�R�}���h���C������������ׂĎ擾

				if( fwrite(allStr.GetBuffer(), allStr.GetLength(), 1, fp) != 1){ //�t�@�C����ۑ�
					API.MessageBox("%s file saving failed", m_filename);
				}

				fclose(fp);
			}
		}else{//�t�@�C�������݂��Ȃ���΁A�t�@�C���������ĕۑ����Ȃ���
			OnCommandPromptMenuSaveas();
		}
	}else{//���߂ĂȂ�΁A�t�@�C���������ĕۑ�
		OnCommandPromptMenuSaveas();
	}
}

/********************************************************************
�@  �\  ��  �� : �u�t�@�C���^���O�����ĕۑ��v���j���[
��    ��    �� : OnCommandPromptMenuSaveas
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuSaveas()
{
	CString filetype, pathname;
	FILE *fp;

	filetype.LoadString(IDS_FILETYPE_TXT);
	CFileDialog myDLG(false , "*.txt", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,  filetype);

	if(myDLG.DoModal() != IDOK)  return ;
	pathname = myDLG.GetPathName();

	fp = fopen(pathname, "wb");//�t�@�C�����J��
	if(fp)
	{
		CString allStr("");
		
		m_rich_edit.GetWindowText(allStr);//�R�}���h���C������������ׂĎ擾

		if( fwrite(allStr.GetBuffer(), allStr.GetLength(), 1, fp) != 1){ //�t�@�C����ۑ�
			API.MessageBox("%s file saving failed", pathname);
		}else{
			m_filename = pathname;//�ۑ������t�@�C�������c��
			::SetWindowText(m_hWnd, m_filename + CString(" -�X�N���v�g"));//�t�@�C�������_�C�A���O�̃L���v�V�����ɔ��f����
		}

		fclose(fp);
	}
	else{
		API.MessageBox("%s file opening failed", pathname);
	}
}

/********************************************************************
�@  �\  ��  �� : �u���s�^�X�e�b�v���s�v���j���[
��    ��    �� : OnCommandPromptMenuStepExe
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuStepExe()
{
	OnBnClickedButtonCommandPromptDo();
}

/********************************************************************
�@  �\  ��  �� : �u���s�^�A�����s�v���j���[
��    ��    �� : OnCommandPromptMenuSeqExe
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuSeqExe()
{
	if(!API.GetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO)){
		API.SetCheck(m_hWnd,IDC_CHECK_COMMAND_PROMPT_DO,true);
	}
}

/********************************************************************
�@  �\  ��  �� : �u���s�^���ʃN���A�v���j���[
��    ��    �� : OnCommandPromptMenuClear
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuClear()
{
	OnBnClickedButtonCommandPromptResultClear();
}


/********************************************************************
�@  �\  ��  �� : �u�w���v�^�R�}���h�v�����v�g���t�@�����X�v���j���[
��    ��    �� : OnCommandPromptMenuClear
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuReference()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM_�X�N���v�g�}�j���A��.doc", NULL, NULL, SW_SHOWNORMAL);
}



/********************************************************************
�@  �\  ��  �� : �}�N�������X�g�I��ύX
��    ��    �� : OnLbnSelchangeListCommandPromptMacro
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnLbnSelchangeListCommandPromptMacro()
{
	CString macro_name;
	int idx = m_list_macro.GetCurSel();
	m_list_macro.GetText(idx,macro_name);

	for(int i=0 ; i<CALC_MACRO_CNT_MAX ; i++)
	{
		MACRO_DEF_STRUCT mcr_def={0};
		if( m_calclate.GetMacroDefinision(i, &mcr_def))//�}�N����`���擾
		{
			if( macro_name == CString(mcr_def.name))
			{
				API.SetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_MACRO_DISCRIPTION, m_macro_description[i]);//�}�N���̐�����\������
				break;
			}
		}
	}
}

/********************************************************************
�@  �\  ��  �� : �}�N�������X�g�_�u���N���b�N
��    ��    �� : OnLbnSelchangeListCommandPromptMacro
��          �� : 
��    ��    �l : 
�@          �\ : �R�}���h�v�����v�g�ɑI���}�N�����}�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnLbnDblclkListCommandPromptMacro()
{
	CString macro_name;
	int idx = m_list_macro.GetCurSel();
	m_list_macro.GetText(idx,macro_name);

	m_rich_edit.ReplaceSel(macro_name + CString("(") );
}

/********************************************************************
�@  �\  ��  �� : �T���v���X�N���v�g���J��
��    ��    �� : OnCommandPromptMenuSample
��          �� :
��    ��    �l :
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnCommandPromptMenuSample()
{
	CString path = API.GetDefaultPath() + CString("..\\..\\..\\Doc\\ScriptSpl");
	char fillpathbuf[1024];
	_fullpath( fillpathbuf, path.GetBuffer() , 1024);
	path.ReleaseBuffer();
	
	open_script_file(fillpathbuf);
}


void CCommandPromptDlg::OnEnUpdateRicheditCommandPrompt()
{
	// TODO:  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A
	// �܂��ACDialog::OnInitDialog() �֐����I�[�o�[���C�h���āAOR ��Ԃ� ENM_CHANGE
	// OR ��Ԃ� ENM_SCROLL �t���O�� IParam �}�X�N�ɓ���āA
	// OR ��Ԃ� ENM_UPDATE �t���O�� lParam �}�X�N�ɓ���āA

	// TODO:  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B

	
}

/********************************************************************
�@  �\  ��  �� : �}�N���������G�f�B�b�g�{�b�N�X�̒l���ύX���ꂽ
��    ��    �� : OnEnChangeEditCommandPromptMacro
��          �� :
��    ��    �l :
�@          �\ :�@�}�N���������G�f�B�b�g�{�b�N�X�̒l���܂ރ}�N���݂̂�\��
�@�@�@�@�@�@�@�@�@(�}�N���������`�F�b�N�{�b�N�XON�̊Ԃ̂�)
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnEnChangeEditCommandPromptMacro()
{
	CString targetStr;//�L�[���[�h

	API.GetEditValue(m_hWnd, IDC_EDIT_COMMAND_PROMPT_MACRO, &targetStr);

	ListBox_ResetContent(::GetDlgItem(m_hWnd, IDC_LIST_COMMAND_PROMPT_MACRO));//���X�g�{�b�N�X�N���A


	//�}�N���ꗗ�̂����L�[���[�h�ƓK��������̂̂݃��X�g�ɓ����

		for (int i = 0; i < CALC_MACRO_CNT_MAX; i++)
		{
			MACRO_DEF_STRUCT mac;
			m_calclate.GetMacroDefinision(i, &mac);

			if (strlen(mac.name) > 0) {
				CString str = CString(mac.name);

				if (str.MakeUpper().Find(targetStr.MakeUpper()) >= 0) {//�啶���E�������s��
					m_list_macro.AddString(CString(mac.name));
				}
			}
		}



	API.SetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_FIND, true);
}


/********************************************************************
�@  �\  ��  �� : �}�N���������`�F�b�N�{�b�N�X�̏�ԕύX
��    ��    �� : OnEnChangeEditCommandPromptMacro
��          �� :
��    ��    �l :
�@          �\ :�@�}�N�������X�g�{�b�N�X������L�[���[�h����
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnBnClickedCheckCommandPromptFind()
{
	if (API.GetCheck(m_hWnd, IDC_CHECK_COMMAND_PROMPT_FIND))
	{
		OnEnChangeEditCommandPromptMacro();
	}
	else
	{
		ListBox_ResetContent(::GetDlgItem(m_hWnd, IDC_LIST_COMMAND_PROMPT_MACRO));//���X�g�{�b�N�X�N���A

		//�}�N���ꗗ����蒼��
		for (int i = 0; i<CALC_MACRO_CNT_MAX; i++)
		{
			MACRO_DEF_STRUCT mac;
			m_calclate.GetMacroDefinision(i, &mac);

			if (strlen(mac.name)>0) {
				m_list_macro.AddString(CString(mac.name));
			}
		}
	}
}


/********************************************************************
�@  �\  ��  �� : Undo
��    ��    �� : OnBnClickedButtonCommandPromptUndo
��          �� :
��    ��    �l :
�@          �\ :�@
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptUndo()
{
	rich_edit_change_event_active_flg = false;

	if (undoID + 1 > 0 && undoID + 1 < UNDO_MAX)
	{
		if (pUndoTbl[undoID + 1])
		{
			m_rich_edit.SetWindowText(CString(pUndoTbl[undoID + 1]));
			color_command_lines(true);//�����F��
			change_break_points(pUndoTbl[undoID], pUndoTbl[undoID + 1]);//�u���[�N�|�C���g�̕ύX
			undoID++;
		}
	}


	rich_edit_change_event_active_flg = true;

	
}

/********************************************************************
�@  �\  ��  �� : Redo
��    ��    �� : OnBnClickedButtonCommandPromptRedo
��          �� :
��    ��    �l :
�@          �\ :�@
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CCommandPromptDlg::OnBnClickedButtonCommandPromptRedo()
{
	rich_edit_change_event_active_flg = false;

	if (undoID - 1 >= 0 && undoID - 1 < UNDO_MAX)
	{
		if (pUndoTbl[undoID -1])
		{
			m_rich_edit.SetWindowText(CString(pUndoTbl[undoID- 1]));
			color_command_lines(true);//�����F��
			change_break_points(pUndoTbl[undoID], pUndoTbl[undoID-1]);//�u���[�N�|�C���g�̕ύX
			undoID--;
		}
	}


	rich_edit_change_event_active_flg = true;
}
