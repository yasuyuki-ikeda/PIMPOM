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
//���Z���̉��߂Ǝ��s
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

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

void CCalculate::debug_out(COMMAND	*command_dbg)
{
#ifdef _DEBUG
	char buffer[1024] = { 0 };

	if (command_dbg->type == COM_TYPE_NUMBER)
	{
		sprintf(buffer, "%lf ", command_dbg->number);
	}
	else if (command_dbg->type == COM_TYPE_OPERATOR)
	{
		char op[][5] = { "+", "-", "*", "/", "==", "<", ">", "<=", ">=", "!=", "=", "&&", "||", "+=", "-=", "*=", "/=" };
		sprintf(buffer, "%s ", op[command_dbg->ope]);
	}
	else if (command_dbg->type == COM_TYPE_PARENT)
	{
		char pa[] = { '(', ')' };
		sprintf(buffer, "%c ", pa[command_dbg->pare]);
	}
	else if (command_dbg->type == COM_TYPE_MACRO)
	{
		sprintf(buffer, "%s ", pMacroDefinision[command_dbg->mac].name);
	}
	else if (command_dbg->type == COM_TYPE_COMMA)
	{
		sprintf(buffer, ", ");
	}
	else if (command_dbg->type == COM_TYPE_COOR_X)
	{
		sprintf(buffer, "x ");
	}
	else if (command_dbg->type == COM_TYPE_COOR_Y)
	{
		sprintf(buffer, "y ");
	}
	else if (command_dbg->type == COM_TYPE_STRING)
	{
		sprintf(buffer, "str ");
	}
	else if (command_dbg->type == COM_TYPE_VARIABLE)
	{
		sprintf(buffer, "var ");
	}
	else
	{

	}

	OutputDebugString(buffer);
#endif
}

void CCalculate::debug_out_ln(COMMAND	*start)
{
#ifdef _DEBUG
	//�f�o�b�O�o��
	for (COMMAND *command_dbg = start->next; command_dbg != NULL; command_dbg = command_dbg->next)//�R�}���h���X�g����������
	{
		debug_out(command_dbg);
	}
	OutputDebugString("\n");
#endif
}




CCalculate::CCalculate()
{
	//�}�N����`
	pMacroDefinision = new MACRO_DEF_STRUCT[CALC_MACRO_CNT_MAX];//�������m��
	memset( pMacroDefinision, 0, sizeof(MACRO_DEF_STRUCT)*CALC_MACRO_CNT_MAX );//������������
	define_macros();//�}�N����`�̎��{


	//�R�}���h�擪�|�C���^�}�b�v������
	for(int i=0 ; i<CALC_COMMAND_LINE_MAX ; i++){
		pCalcDataStructMap[i] = NULL;
	}

	//�ϐ��擪�|�C���^�}�b�v������
	for(int i=0 ; i<CALC_VARIABLE_BUFFER_LEN ; i++){
		pCalcVariableMap[i] = NULL;
	}

	//�t�@�C���ꗗ�쐬�̂��߂̃t�@�C���_
	for (int i = 0; i < CALC_FILE_FINDER_MAX; i++){
		pFileFinder[i] = NULL;
	}
}


CCalculate::~CCalculate()
{
	ClearAll();

	delete[]	pMacroDefinision;

	for (int i = 0; i < CALC_FILE_FINDER_MAX; i++){
		if (pFileFinder[i]){
			pFileFinder[i]->Close();
			delete	pFileFinder[i];
		}
	}
}



/********************************************************************
�@  �\  ��  �� : ����������
��    ��    �� : init_calc_structure
��          �� :
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::init_calc_structure(int id,					//(in)�s�ԍ�
							  CString *p_err_message	//(out)�G���[���b�Z�[�W
							  )
{
	if(!pCalcDataStructMap[id])//�f�[�^���������Ȃ���ΐV�K�m��
	{
		pCalcDataStructMap[id] = new CALC_DATA_STRUCT;
		if(pCalcDataStructMap[id]==NULL){
			p_err_message->Format("Error :lackness of memory (init_calc_structure)");
			return false;
		}
		
		memset(pCalcDataStructMap[id], 0, sizeof(CALC_DATA_STRUCT));//������
	}
	else 
	{	//�f�[�^������������ꍇ

		//���ԃf�[�^�̃��������폜(�v�����ʂƃG���[���b�Z�[�W�͎c��)

		if( pCalcDataStructMap[id]->pCommandList ){//�R�}���h���X�g�N���A
			delete[]	pCalcDataStructMap[id]->pCommandList;
			pCalcDataStructMap[id]->pCommandList = NULL;
		}

		if( pCalcDataStructMap[id]->pStrignBuffer ){//������o�b�t�@�N���A
			delete		pCalcDataStructMap[id]->pStrignBuffer ;
			pCalcDataStructMap[id]->pStrignBuffer = NULL;
		}

		memset(&pCalcDataStructMap[id]->ctrlData, 0, sizeof(CALC_CTRL_DATA));//�R�}���h���C������R�[�h�N���A
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : deinit_calc_structure
��          �� :
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalculate::deinit_calc_structure(int id)
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX){
		return;
	}

	if(pCalcDataStructMap[id])
	{
		if(pCalcDataStructMap[id]->pStrignBuffer)
		{
			delete	pCalcDataStructMap[id]->pStrignBuffer;
			pCalcDataStructMap[id]->pStrignBuffer = NULL;
		}

		if(pCalcDataStructMap[id]->pCommandList)
		{
			delete[]	pCalcDataStructMap[id]->pCommandList;
			pCalcDataStructMap[id]->pCommandList = NULL;
		}
	}

	delete	pCalcDataStructMap[id];
	pCalcDataStructMap[id] = NULL;
}

/********************************************************************
�@  �\  ��  �� : �o�^����Ă���R�}���h���C���̒�����Ԃ�
��    ��    �� : GetCommandLineLen
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCalculate::GetCommandLineLen()
{
	for(int i=0 ; i<CALC_COMMAND_LINE_MAX ; i++)
	{
		if(pCalcDataStructMap[i]==NULL){
			return i;
		}
	}
	return CALC_COMMAND_LINE_MAX-1;
}

/********************************************************************
�@  �\  ��  �� : �R�}���h������^������o�b�t�@�^�R�}���h���X�g�^�v�Z���ʂ����ׂăN���A
��    ��    �� : ClearAll
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalculate::ClearAll()
{
	for(int i=0 ; i<CALC_COMMAND_LINE_MAX ; i++)
	{
		deinit_calc_structure(i);
	}

	clear_variable();
}


/********************************************************************
�@  �\  ��  �� : �V�K�R�}���h���X�g�̑}��
��    ��    �� : set_command_list
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CCalculate::set_command_list(int		id ,				//(in)
									CString	commandStr,				//(in)�R�}���h������
								   COMMAND	*pNewCommandCodeList,	//(in)�}������R�}���h���X�g
								   int		ctrl_code				//(in)�}������R�}���h���C������R�[�h
								   )
{
	if(id<0 || id >= CALC_COMMAND_LINE_MAX)	return false;//id���s��

	if(pCalcDataStructMap[id]==NULL)	return false;//���������������ł��ĂȂ�


	//�w�肳�ꂽ�R�}���h���X�g������΃���������U�폜
	if(pCalcDataStructMap[id]->pCommandList){
		delete[]	pCalcDataStructMap[id]->pCommandList;
		pCalcDataStructMap[id]->pCommandList=NULL;
	}


	int len = get_command_listLen(pNewCommandCodeList);//�V�����R�}���h���X�g�̒�����0�łȂ���΃������m��
	if(len>0){
		pCalcDataStructMap[id]->pCommandList = new COMMAND[len];
		if(pCalcDataStructMap[id]->pCommandList==NULL)	return false;
		
		copy_command_list(pCalcDataStructMap[id]->pCommandList, pNewCommandCodeList, len);//�R�}���h���X�g���R�s�[
	}

	strcpy( pCalcDataStructMap[id]->commandString, commandStr.GetBuffer());//�R�}���h���C����������R�s�[
	commandStr.ReleaseBuffer();

	pCalcDataStructMap[id]->ctrlData.controlCode = ctrl_code;//�R�}���h���C������R�[�h

	return true;
}

/********************************************************************
�@  �\  ��  �� : �R�}���h���X�g�����擾
��    ��    �� : get_command_listLen
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCalculate::get_command_listLen(COMMAND *command)
{
	int cnt=0;
	COMMAND *com = command;

	while(com!=NULL){
		com = com->next;
		cnt++;
	}

	return cnt;
}

/********************************************************************
�@  �\  ��  �� : �R�}���h���X�g�̐擪�|�C���^�擾
��    ��    �� : get_command_list
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
COMMAND *CCalculate::get_command_list(int id, CALC_CTRL_DATA* p_ctrl_data)
{
	if(id<0 || id >= CALC_COMMAND_LINE_MAX)	return NULL;//id���s��

	if(pCalcDataStructMap[id]==NULL)	return NULL;//���������������ł��ĂȂ�


	memcpy( p_ctrl_data,  &pCalcDataStructMap[id]->ctrlData, sizeof(CALC_CTRL_DATA));

	return pCalcDataStructMap[id]->pCommandList;
}

/********************************************************************
�@  �\  ��  �� : �R�}���h���X�g���R�s�[����
��    ��    �� : copy_command_list
��          �� : 
��    ��    �l : ����ɉ��Z�ł����ꍇ�� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CCalculate::copy_command_list(	COMMAND *dst,	//(out)�R�s�[��R�}���h���X�g�擪�|�C���^
										COMMAND *src,	//(in)�R�s�[���R�}���h���X�g�擪�|�C���^
										int		size	//(in)���X�g��
									)
{
	memcpy(dst, src, sizeof(COMMAND)*size);
	for(int i=0 ; i<size ; i++){
		if(src[i].prev!=0){
			int offset_prev = src[i].prev - src;
			dst[i].prev = dst + offset_prev;
		}

		if(src[i].next!=0){
			int offset_next = src[i].next - src;
			dst[i].next = dst + offset_next;
		}
	}
}




/********************************************************************
�@  �\  ��  �� : ������o�b�t�@�֕������V���ɑ}������
��    ��    �� : push_string_to_buffer
��          �� :
��    ��    �l : ���������ꍇ�̓o�b�t�@ID���A��
				���s�����ꍇ�́A-1:ID�ԍ��s��  -2:ID�ԍ��Ŏw�肳�ꂽ�s�������s
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCalculate::push_string_to_buffer(
								 int	 id,
								 CString str,				//(in)�}�����镶����
								 CString *p_err_message,	//(out)�G���[���b�Z�[�W
								 bool	 isStatic			//(in)�R�}���h���C���Ŏw�肳���Œ蕶���񂩂ǂ���
								 )
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX){
		return -1;//ID���s��
	}

	if(pCalcDataStructMap[id]==NULL){
		return -2;//���������������ł��ĂȂ�
	}

	if(str.GetLength() >= CALC_STRING_LEN_MAX )//�}�����镶���񂪒�������
	{
		p_err_message->Format("Error: command string is too long (max %d)", CALC_STRING_LEN_MAX-1);
		set_error_message(id, *p_err_message);
		return -1;
	}

	if(pCalcDataStructMap[id]->pStrignBuffer==NULL)//���������Ȃ���ΐV���ɂƂ�Ȃ���
	{
		pCalcDataStructMap[id]->pStrignBuffer = new CALC_STRING_BUFFER_STRUCT;
		if(!pCalcDataStructMap[id]->pStrignBuffer )
		{
			p_err_message->Format("Error: lackness of mempry (push_string_to_buffer)");
			set_error_message(id, *p_err_message);
			return -1;
		}

		memset(pCalcDataStructMap[id]->pStrignBuffer, 0, sizeof(CALC_STRING_BUFFER_STRUCT));
	}

	if( pCalcDataStructMap[id]->pStrignBuffer->count + 1 >= CALC_STRING_BUFFER_LEN)//������o�b�t�@���ӂ�
	{
		p_err_message->Format("Error: too many string value(max %d)", CALC_STRING_BUFFER_LEN-1);
		set_error_message(id, *p_err_message);
		return -1;
	}


	int newcnt = pCalcDataStructMap[id]->pStrignBuffer->count;

	//������R�s�[
	strcpy( pCalcDataStructMap[id]->pStrignBuffer->strbuffer[newcnt], str.GetBuffer());
	str.ReleaseBuffer();

	pCalcDataStructMap[id]->pStrignBuffer->count++;//������o�b�t�@�J�E���g�i�߂�

	if(isStatic){
		pCalcDataStructMap[id]->pStrignBuffer->staticCount++;
	}

	return newcnt;
}

/********************************************************************
�@  �\  ��  �� : �o�b�t�@���當����擾
��    ��    �� : get_string_from_buffer
��          �� : 
��    ��    �l : ������o�b�t�@�̐擪�A�h���X���A��
				�@���s�����NULL
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
char *CCalculate::get_string_from_buffer(int id, int str_id)
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX)	return NULL;//id���s��

	if(pCalcDataStructMap[id]==NULL)	return NULL;//���������������ł��ĂȂ�

	if(pCalcDataStructMap[id]->pStrignBuffer==NULL)	return NULL;//������o�b�t�@�ɉ����Ȃ�

	if(str_id<0 || str_id>=pCalcDataStructMap[id]->pStrignBuffer->count)	return NULL;//str_id���s��


	return pCalcDataStructMap[id]->pStrignBuffer->strbuffer[str_id];
}


/********************************************************************
�@  �\  ��  �� : ������o�b�t�@����Œ蕶���łȂ����̂��폜
��    ��    �� : delete_non_static_string_buffer
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalculate::delete_non_static_string_buffer(int id)
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX)	return ;//id���s��

	if(pCalcDataStructMap[id]==NULL)	return ;//���������������ł��ĂȂ�

	if(pCalcDataStructMap[id]->pStrignBuffer==NULL)	return ;//������o�b�t�@�ɉ����Ȃ�

	pCalcDataStructMap[id]->pStrignBuffer->count = pCalcDataStructMap[id]->pStrignBuffer->staticCount;
}


/********************************************************************
�@  �\  ��  �� : �ϐ��̃��������m�ۂ���
��    ��    �� : new_variable
��          �� :
��    ��    �l : ���������ꍇ�͕ϐ���ID���A��B���s�����Ƃ��͕������A��
�@          �\ : �ϐ��̃��������m�ۂ���
�@�@�@�@�@�@�@�@�@�����l�͐��l0
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCalculate::new_variable(
						CString var_name,			//(in)�ϐ���
						CString *p_err_message		//(out)�G���[���b�Z�[�W
						)
{
	//�ϐ�������������
	if(var_name.GetLength() > CALC_VARIABLE_NAME_MAX){
		p_err_message->Format("Error: too long variable name(max %d)", CALC_VARIABLE_NAME_MAX-1);
		return -1;
	}

	//�ϐ��o�b�t�@�������ς�
	if(pCalcVariableMap[CALC_VARIABLE_BUFFER_LEN-1] != NULL ){
		p_err_message->Format("Error: too mary varialbe definission (max %d)", CALC_VARIABLE_BUFFER_LEN);
		return -1;
	}

	//�������O�̕ϐ��錾
	int id=0;
	while( pCalcVariableMap[id]!=NULL){
		if( var_name == CString(pCalcVariableMap[id]->name) ){
			//p_err_message->Format("Error: varialbe " + var_name + " is already defined");
			return id;
		}
		id++;
	}

	//�o�b�t�@�ɕϐ��ǉ�
	pCalcVariableMap[id] = new CALC_VARIABLE_STRUCT;
	if(pCalcVariableMap[id]==NULL)	return -1;

	memset(pCalcVariableMap[id], 0, sizeof(CALC_VARIABLE_STRUCT) );

	sprintf( pCalcVariableMap[id]->name, var_name.GetBuffer() );	var_name.ReleaseBuffer();

	return id;
	
}

/********************************************************************
�@  �\  ��  �� : ID�w��ŕϐ����擾����
��    ��    �� : GetVariable
��          �� : 
��    ��    �l : ����������true
				 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::GetVariable(int id,					//(in)�ϐ�ID
							  CALC_RESULT_STRUCT *pvar, //(out)�ϐ��̒l
							  char varname[]			//(out)�ϐ���
								)
{
	if(CALC_VARIABLE_BUFFER_LEN <= id || id<0){//id���s��
		return false;
	}

	if(pCalcVariableMap[id]==NULL){//id�ł��Ă����ꂽ�ϐ����������m�ۂ���Ă��Ȃ�
		return false;
	}

	memcpy(pvar , &pCalcVariableMap[id]->data, sizeof(CALC_RESULT_STRUCT));//�f�[�^�擾
	strcpy(varname, pCalcVariableMap[id]->name);//�ϐ����擾

	return true;
}

/********************************************************************
�@  �\  ��  �� : ID�w��ŕϐ���ݒ肷��
��    ��    �� : SetVariable
��          �� : 
��    ��    �l : ����������true
				 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::SetVariable(int id,					//(in)�ϐ�ID
							  CALC_RESULT_STRUCT *pvar //(in)�ϐ��̒l
								)
{
	if(CALC_VARIABLE_BUFFER_LEN <= id || id<0){//id���s��
		return false;
	}

	if(pCalcVariableMap[id]==NULL){//id�ł��Ă����ꂽ�ϐ����������m�ۂ���Ă��Ȃ�
		return false;
	}

	memcpy( &pCalcVariableMap[id]->data, pvar, sizeof(CALC_RESULT_STRUCT));//�f�[�^�ݒ�
	return true;
}


/********************************************************************
�@  �\  ��  �� : �ϐ��錾�����N���A
��    ��    �� : clear_variable
��          �� : 
��    ��    �l : 
				 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalculate::clear_variable()
{
	for(int i=0 ; i<CALC_VARIABLE_BUFFER_LEN ; i++)
	{
		delete	pCalcVariableMap[i];
		pCalcVariableMap[i] = NULL;
	}

}

/********************************************************************
�@  �\  ��  �� : �v�Z���ʎ擾
��    ��    �� : GetResult
��          �� : 
��    ��    �l : �v�����ʂ̃^�C�v�@0:���l  7:������   -1:ID�ԍ��s��  -2:ID�ԍ��Ŏw�肳�ꂽ�s�������s
				 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCalculate::GetResult(int		id,			//(in)
						  double	*number,	//(out)
						  char		str[],		//(out)
						  CString	*p_err_message,	//(out)
						  DWORD		*p_elapsed_time	//(out)
						)
{
	if(id<0 || id>=GetCommandLineLen())
	{
		p_err_message->Format("Error: Res() argument must be within 0-%d", GetCommandLineLen()-1);
		*number=0;
		str[0]=0;
		return -1;//id���s��
	}

	if(pCalcDataStructMap[id]==NULL)
	{
		p_err_message->Format("---");
		*number=0;
		str[0]=0;
		return -2;//���������������ł��ĂȂ�
	}


	if(pCalcDataStructMap[id]->result.type == COM_TYPE_NUMBER)
	{//���l
		*number = pCalcDataStructMap[id]->result.number;
		str[0]=0;
	}
	else if(pCalcDataStructMap[id]->result.type == COM_TYPE_STRING)
	{//������
		strcpy(str, pCalcDataStructMap[id]->result.str);
		*number=0;
	}


	*p_err_message  = CString(pCalcDataStructMap[id]->errorMessage);//�G���[���b�Z�[�W�o��

	if (p_elapsed_time){
		*p_elapsed_time = pCalcDataStructMap[id]->result.elapsedTime;//�o�ߎ���
	}
	

	return pCalcDataStructMap[id]->result.type;
}

/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : set_result
��          �� : 
��    ��    �l : ���s������false
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CCalculate::set_result(int id, COMMAND *com, DWORD elapsed_time)
{

	if(com->type == COM_TYPE_NUMBER)
	{
		pCalcDataStructMap[id]->result.type = COM_TYPE_NUMBER;
		pCalcDataStructMap[id]->result.number = com->number;
	}
	else if(com->type == COM_TYPE_STRING)
	{
		pCalcDataStructMap[id]->result.type = COM_TYPE_STRING;
		strcpy( pCalcDataStructMap[id]->result.str, get_string_from_buffer(id, com->str_id) );
	}

	pCalcDataStructMap[id]->result.elapsedTime = elapsed_time;


	return true;
}




/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : set_error_message
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CCalculate::set_error_message(int id, CString err )
{
	if(id<0 || id>=CALC_COMMAND_LINE_MAX)	return ;//id���s��

	if(pCalcDataStructMap[id]==NULL)	return ;//���������������ł��ĂȂ�

	if(err.GetLength()>=CALC_STRING_LEN_MAX)	return;//�G���[���b�Z�[�W����������

	strcpy(pCalcDataStructMap[id]->errorMessage, err.GetBuffer() );
	err.ReleaseBuffer();
}


/********************************************************************
�@  �\  ��  �� : �p�����^�Ŋ֘A�Â��������R�}���h�������ÓI�ɓW�J���ĂP�̕�����ɂ܂Ƃ߂�
��    ��    �� : ExtendSubCommands
��          �� : 
��    ��    �l : ����ɓW�J�ł����ꍇ�� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::ExtendSubCommands(int		sub_com_num,		//(in)�T�u�R�}���h������
								   CString	sub_com_str[],		//(in)�T�u�R�}���h������
								   CString	sub_com_id[],		//(in)�T�u�R�}���h��\�����ʕ�����
								   CString	*p_com_str,			//(in/out)���C���R�}���h������
								   CString	*p_err_message,		//(out)�G���[���b�Z�[�W
								   bool		*sub_com_enable		//(in)�T�u�R�}���h�̗L���^�����iNULL�j
								   )
{
	p_err_message->Empty();

	//�p�����^����������ׂēW�J����
	for(int j=0 ; j<sub_com_num ; j++){
		bool sub_com_remain_flg;
		do{
			sub_com_remain_flg=false;
			for(int i=0 ; i<sub_com_num ; i++)
			{
				if( sub_com_str[j].Find(sub_com_id[i]) != -1){//�p�����^������̒��ł���Ƀp�����^���ʕ�������������
					sub_com_remain_flg=true;

					if(sub_com_enable != NULL){
						if(!sub_com_enable[i]){//�����ȃp�����^���g�����ꍇ
							p_err_message->Format("Error: �p�����^" + sub_com_id[i] + "�͗L���ł͂���܂���");
							return false;
						}
					}

					if(i==j){
						p_err_message->Format("Error: �p�����^�̎��ȎQ�Ƃ�����܂�");
						return false;
					}else {
						sub_com_str[j].Replace(sub_com_id[i], CString("(") + sub_com_str[i] +  CString(")"));
					}
				}
			}
		}while(sub_com_remain_flg);//�p�����^�Q�Ƃ��l�X�g���Ă���ꍇ�́A�W�J���J��Ԃ�
	}

	//�R�}���h������ɔ��f
	for(int i=0 ; i<sub_com_num ; i++){
		p_com_str->Replace(sub_com_id[i], CString("(") + sub_com_str[i] +  CString(")"));
	}
	return true;
}




/********************************************************************
�@  �\  ��  �� : ���ׂẴR�}���h�������|�󂵂Ď��s��������
��    ��    �� : RegisterCommandString
��          �� : 
��    ��    �l : ���s�����ꍇ��true��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::RegisterCommandString(	
										CString		com_strs[],		//(in)�R�}���h������
										int			com_str_cnt,	//(in)������
										CString		*p_err_message	//(out)�G���[���b�Z�[�W
										)
{
	for(int id=0 ; id<com_str_cnt ; id++)
	{
		int	com_num=0, ctrl_code=0; 
		COMMAND	CommandCode[CALC_COMMAND_LINE_MAX]={0};


		if(com_strs[id].GetLength() >= CALC_STRING_LEN_MAX){
			p_err_message->Format("Error: command line%d is too long (max=%d)", id, CALC_STRING_LEN_MAX-1);
			return false;//�R�}���h�����񂪒�������
		}


		if(pCalcDataStructMap[id]){
			if(pCalcDataStructMap[id]->commandString == com_strs[id]){//�ߋ��Ɏ��s�����R�}���h������ƕς���Ă��Ȃ��ꍇ�́A�R�}���h���X�g���ė��p����
				continue;
			}
		}


		
		if( init_calc_structure(id, p_err_message ))//�R�}���h��������f�[�^�������ɓo�^
		{
			//�R�}���h������˃R�}���h���X�g�ɖ|��
			com_num = command_string_to_command_list(id, com_strs[id], CommandCode, p_err_message, &ctrl_code);
			if(com_num>0)
			{//����
				if(!set_command_list(id, com_strs[id], CommandCode, ctrl_code) )//�R�}���h���X�g���f�[�^�������ɓo�^
				{
					set_error_message(id, *p_err_message);
					return false;
				}
			}
			else
			{//���s
				set_error_message(id, *p_err_message);
				return false;
			}
		}
		else
		{
			set_error_message(id, *p_err_message);
			return false;
		}
	}

	

	//if-else-endif�̐ݒ�
	if( !set_branch_property(com_str_cnt,p_err_message) )
	{
		return false;
	}

	return true;
}





/********************************************************************
�@  �\  ��  �� : �R�}���h��������s�P�ʂŖ|�󂵂ăR�}���h���X�g�ɕς���
��    ��    �� : command_string_to_command_list
��          �� :
��    ��    �l : ���m�ɃR�}���h��ǂݎ�ꂽ�R�}���h��		�R�}���h�̓ǂݎ��Ɏ��s�����ꍇ�͕���
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCalculate::command_string_to_command_list(
									    int			id,				//(in)�s�ԍ�
										CString		str,			//(in)�R�}���h������
										COMMAND		commands[],		//(out)�R�}���h���X�g
										CString		*p_err_message,	//(out)�G���[���b�Z�[�W
										int			*p_ctrl_code	//(out)
										)
{
	int index_start=0;//�R�}���h�����T���J�n�ʒu
	int	com_cnt=0;//�R�}���h�̐�


	


	//���̍s���ϐ��錾�ł��邩�ǂ�������ׂ�
	int varDefCnt = FindVariableDefinission(str, p_err_message);
	if(  varDefCnt>0 ){
		index_start = str.GetLength();//�ϐ��錾������ꍇ�́A���̍s�ɂ͂��̑��̋L�q�͖����Ƃ���
	}else if(varDefCnt<0){
		set_error_message(id, *p_err_message);//������ϐ��錾����������A�G���[
		return -1;
	}


	//�R�}���h���s���䕶��T��
	int incliment=0;
	if( FindCtrlCode(str, index_start, p_ctrl_code , &incliment) ){
		index_start += incliment;
	}
	


	//�R�}���h�̍ŏ��Ɋ��ʂ�����
	commands[ 0 ].type   = COM_TYPE_PARENT;
	commands[ 0 ].pare   = COM_PARENT_OPEN;
	commands[ 0 ].prev   = NULL;
	commands[ 0 ].next   = commands + 1;
	com_cnt = 1;



	for(int index=index_start ; index<str.GetLength() ; )//�R�}���h�����������ׂ�
	{
		int			temp_int;
		double		temp_double;
		CString		temp_str;
		int			indexIncliment=0;

			if(com_cnt >= CALC_COMMAND_LIST_MAX){//�R�}���h������������
				p_err_message->Format("Error: line%d has too meny commands", id);
				set_error_message(id, *p_err_message);
				return -1;
			}


			if( IsStringValue( str , index , &temp_str, &indexIncliment) )//"�ň͂܂ꂽ�Œ蕶���񂪌�������
			{
				commands[ com_cnt ].type   = COM_TYPE_STRING;
				if( (temp_int = push_string_to_buffer(id, temp_str , p_err_message, true) ) < 0){

					return -1;
				}else{
					commands[ com_cnt ].str_id = temp_int;//�������ꂽ��������o�b�t�@�ɓ����
					commands[ com_cnt ].prev   = commands + com_cnt - 1;
					commands[ com_cnt ].next   = commands + com_cnt + 1;
					com_cnt++;
				}
			}
			else if( IsComment(str, index) )
			{
				//�R�����g������������A����ȏ��͌��Ȃ�
				break;
			}
			else if( IsNumberValue( str , index , &temp_double, &indexIncliment) )//���l����������
			{
				commands[ com_cnt ].type   = COM_TYPE_NUMBER;
				commands[ com_cnt ].number = temp_double;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

			}
			else if( IsOperator( str , index , &temp_int , &indexIncliment) )//���Z�q����������
			{
				commands[ com_cnt ].type   = COM_TYPE_OPERATOR;
				commands[ com_cnt ].ope    = temp_int;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

			}
			else if( IsParenthesis( str , index , &temp_int) )//���ʂ���������
			{
				commands[ com_cnt ].type   = COM_TYPE_PARENT;
				commands[ com_cnt ].pare   = temp_int;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

				indexIncliment=1;

			}
			else if( IsMacroName( str , index , &temp_int, &indexIncliment) )//�}�N������������
			{
				commands[ com_cnt ].type   = COM_TYPE_MACRO;
				commands[ com_cnt ].mac    = temp_int;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;
			}
			else if( IsDefinedVariable( str, index, &temp_int , &indexIncliment) )// �錾�ςݕϐ�����������
			{
				commands[ com_cnt ].type   = COM_TYPE_VARIABLE;
				commands[ com_cnt ].var_id    = temp_int;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;
			}
			else if (str[index] == ',' || str[index] == ';')// �J���}����������
			{
				commands[ com_cnt ].type   = COM_TYPE_COMMA;
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

				indexIncliment=1;

			}
			else if( str[ index ] == 'i' ||  str[ index ] == 'j' || str[ index ] == 'x' || str[ index ] == 'y')// ���W��\�� i, j,x,y ����������
			{
				if( str[ index ] == 'i' || str[ index ] == 'x')		commands[ com_cnt ].type   = COM_TYPE_COOR_X;
				else												commands[ com_cnt ].type   = COM_TYPE_COOR_Y;
					
				commands[ com_cnt ].prev   = commands + com_cnt - 1;
				commands[ com_cnt ].next   = commands + com_cnt + 1;
				com_cnt++;

				indexIncliment=1;
			}
			else if( str[ index ] != ' ' && str[index] != '\t')
			{
				//�s���ȕ������܂܂�Ă���
				p_err_message->Format("Error: line%d has invarid character (%c)", id, str[ index ]);
				return  -1;
			}

			index += indexIncliment;
	}

	//�R�}���h�̊��ʂ����
	commands[ com_cnt ].type = COM_TYPE_PARENT;
	commands[ com_cnt ].pare = COM_PARENT_CLOSE;
	commands[ com_cnt ].prev = commands + com_cnt - 1 ;
	commands[ com_cnt ].next = NULL;
	com_cnt++;


	//�i �Ɓ@�j�̐����������ǂ����`�F�b�N
	if(  !check_parenthesis(commands, p_err_message) )	return -1;


	return	com_cnt;
}


/********************************************************************
�@  �\  ��  �� : �����񂪃R�����g���ǂ���
��    ��    �� : IsComment
��          �� :
��    ��    �l : �R�����g�Ȃ��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::IsComment(
							 CString	comstr,		//(in)�����Ώۂ̕�����
							 int		index		//(in)comstr���̌������J�n����ʒu 
							 )
{
	if( index + 1 >= comstr.GetLength() )	return false;

	if(comstr[index]=='/' && comstr[index+1]=='/')	return true;

	return false;
}


/********************************************************************
�@  �\  ��  �� : �����񂩂當����(""�ň͂܂ꂽ�̈�)����������
��    ��    �� : IsStringValue
��          �� :
��    ��    �l : �����񂪌���������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::IsStringValue(
							 CString	comstr,				//(in)�����Ώۂ̕�����
							 int		index ,				//(in)comstr���̌������J�n����ʒu
							 CString	*p_str,				//(out)�������ꂽ������
							 int		*pindexIncliment	//(out)�C���f�b�N�X�C���N�������g��
							 )
{
	if(comstr[index]!='"')	return	false;//index�Ŏ����������u"�v�ł͂Ȃ��ꍇ�͉������Ȃ�

	CString buffer("");//�`�F�b�N���������������ɂ��߂Ă���
	int temp_index;//���ڕ����C���f�b�N�X

	//������""�ň͂܂�Ă��邩�ǂ��������`�F�b�N
	for(temp_index = index + 1 ; temp_index < comstr.GetLength() ; temp_index++)
	{
		if(comstr[temp_index]=='"')//������I���́u"�v����������
		{
			*p_str = buffer;
			*pindexIncliment = temp_index  - index + 1;
			return true;
		}
		else
		{
			buffer += CString(comstr[temp_index]);
		}
	}

	return false;//������I���́u"�v��������Ȃ�����
}






/********************************************************************
�@  �\  ��  �� :�����񂪐��l�f�[�^���ǂ������f
��    ��    �� : IsNumberValue
��          �� :
��    ��    �l : ���l������������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/

bool CCalculate::IsNumberValue(
							 CString	comstr,				//(in)������
							 int		index,				//(in)comstr���̌������J�n����ʒu 
							 double		*p_number,			//(out)���l
							 int		*pIndexIncliment	//(out)�C���f�b�N�X�C���N�������g��
							 )
{
	if( comstr.GetLength()<=index )	return false;
	if( !isdigit( comstr[index] ) )	return	false;//index�Ŏ������������l�ł͂Ȃ��ꍇ�͉������Ȃ�
	

	CString		num_str("");//���l�Ƃ��ă`�F�b�N���������������ɓ���Ă���
	BOOL		under_1_flg = false;//�����_�����������ǂ����������t���O
	int			temp_index = index;//���ڕ����C���f�b�N�X


	//�����񂪐��l���ǂ��������`�F�b�N
	while( temp_index < comstr.GetLength() ){

			if( isdigit( comstr[temp_index] ) )//���ڕ��������l�̏ꍇ
			{
					num_str += (CString)comstr[ temp_index++ ];
			}
			else if( comstr[temp_index]=='.' )//���ڕ������u.�v �̏ꍇ
			{
					if( under_1_flg == false   &&   !num_str.IsEmpty() )//�u.�v�͏����_�ł���
					{
							num_str += (CString)comstr[ temp_index++ ];
							under_1_flg = true;
					}
					else//�s���ȁu.�v(�����_���Q����)
					{
						return false;
					}

			}else{//���ڕ��������l�łȂ��ꍇ
				break;
			}
	}

	if( !num_str.IsEmpty() ){//���l��\�������񂪌�������
		*p_number = atof( num_str );

		*pIndexIncliment = temp_index - index;
		
		return	true;

	}else{//���l��\�������񂪌�����Ȃ�����
		return false;
	}
}


/********************************************************************
�@  �\  ��  �� : �����񂪉��Z�q���ǂ������f
��    ��    �� : IsOperator
��          �� :
��    ��    �l : ���Z�q������������true
�@          �\ : �l�����Z
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::IsOperator(	CString	comstr,				//(in)������
							   int		index,				//(in)comstr���̌������J�n����ʒu
							   int		*p_ope,				//(out)0:+     1:-      2:*      3:/   
							   int		*pIndexIncliment	//(out)�C���f�b�N�X�C���N�������g��
							   )
{
	if(index + 1 < comstr.GetLength() )//2�������Z�q
	{
		*pIndexIncliment=2;

		if(comstr[index]=='=' && comstr[index+1]=='='){
			*p_ope = 4;
			return true;
		}else if(comstr[index]=='<' && comstr[index+1]=='='){
			*p_ope = 7;
			return true;
		}else if(comstr[index]=='>' && comstr[index+1]=='='){
			*p_ope = 8;
			return true;
		}else if(comstr[index]=='!' && comstr[index+1]=='='){
			*p_ope = 9;
			return true;
		}
		else if (comstr[index] == '&' && comstr[index + 1] == '&') {
			*p_ope = 11;
			return true;
		}
		else if (comstr[index] == '|' && comstr[index + 1] == '|') {
			*p_ope = 12;
			return true;
		}
		else if (comstr[index] == '+' && comstr[index + 1] == '=') {
			*p_ope = 13;
			return true;
		}
		else if (comstr[index] == '-' && comstr[index + 1] == '=') {
			*p_ope = 14;
			return true;
		}
		else if (comstr[index] == '*' && comstr[index + 1] == '=') {
			*p_ope = 15;
			return true;
		}
		else if (comstr[index] == '/' && comstr[index + 1] == '=') {
			*p_ope = 16;
			return true;
		}
	}

	//1�������Z�q
	*pIndexIncliment=1;

	switch( comstr[index] ){
		case '+':
			*p_ope = 0;
			return true;
		case '-':
			*p_ope = 1;
			return true;
		case '*':
			*p_ope = 2;
			return true;
		case '/':
			*p_ope = 3;
			return true;
		case '<':
			*p_ope = 5;
			return true;
		case '>':
			*p_ope = 6;
			return true;
		case '=':
			*p_ope = 10;
			return true;
	}

	return false;
}


/********************************************************************
�@  �\  ��  �� : �����񂪊��ʂ��ǂ������f
��    ��    �� : IsParenthesis
��          �� :
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::IsParenthesis(
								   CString	comstr,		//(in)������
								   int		index,	//(in/out)comstr���̌������J�n����ʒu �@���ʂ����������炻�̕��C���f�b�N�X��i�߂�
								   int		*p_pare		//(out)0:(     1:) 
								   )
{
	if( comstr[ index ] == '(' ){
		*p_pare = 0;
		return true;
	}else if( comstr[ index ] == ')' ){
		*p_pare = 1;
		return true;
	}

	return false;
}


/********************************************************************
�@  �\  ��  �� : �����񂪃}�N�������ǂ������f
��    ��    �� : IsMacroName
��          �� :
��    ��    �l : ����������true
�@          �\ : �}�N��������ђ���́u(�v��T��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::IsMacroName(
							 CString	comstr,				//(in)������
							int			index,				//(in)comstr���̌������J�n����ʒu 
							int			*p_macro_id,		//(out)�}�N���ԍ�
							int			*pIndexIncliment	//(out)�C���f�b�N�X�C���N�������g��
							)
{

	for(int id=0 ; id<CALC_MACRO_CNT_MAX ; id++)//���ׂẴ}�N����������
	{
		if( pMacroDefinision[id].name[0]==0)	continue;//����`

		int		temp_index = index;//���ڕ����C���f�b�N�X

		int mac_nam_len = (int)strlen( pMacroDefinision[id].name );//�}�N������

		//�}�N������comstr��1��������r
		bool isMcrName=true;
		for(int i=0 ; i<mac_nam_len; i++)
		{
			if( comstr[i+temp_index] != pMacroDefinision[id].name[i])
			{
				isMcrName=false;//1�����ł��قȂ��comstr�̓}�N�����ł͂Ȃ�
				break;
			}
		}
		
		if(isMcrName)//���ڕ������}�N�����Ɠ����ł��邽�߁A���̒����(������΁A�}�N���ł���
		{
			if( comstr[temp_index + mac_nam_len] == '(' )//�}�N�����̒���́u ( �v�������������}�N���ł���
			{
				temp_index += mac_nam_len -1;

				*p_macro_id = id;

				*pIndexIncliment = temp_index - index + 1;

				return	true;
			}
		}
	}

	return false;//�}�N����������Ȃ�����
}


/********************************************************************
�@  �\  ��  �� : �����񂪐錾�ςݕϐ������ǂ���
��    ��    �� : IsDefinedVariable
��          �� :
��    ��    �l : �����񂪌���������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::IsDefinedVariable(
							 CString	comstr,		//(in)�����Ώۂ̕�����
							 int		index ,		//(in)comstr���̌������J�n����ʒu
							 int		*p_var_id,	//(out)�ϐ���
							 int		*pIndexIncliment	//(out)�C���f�b�N�X�C���N�������g��
							 )
{
	//�o�^����Ă��邷�ׂĂ̕ϐ����Ɣ�r���A��������T��
	int varID=0;
	int varNameLenMax=0;
	while( pCalcVariableMap[varID]!=NULL )
	{
		CALC_RESULT_STRUCT varres={0};
		char	varname[CALC_VARIABLE_NAME_MAX]={0};

		GetVariable(varID, &varres, varname);//�ϐ������擾

		bool is_var_name=true;

		for(int  idx=0; idx < strlen(varname) ; idx++)//�P��������r
		{
			if(varname[idx] != comstr[idx + index] ){//�قȂ镶����������
				is_var_name=false;
				break;
			}
		}

		if(is_var_name)
		{//���ׂĂ̕�������v����
			if(varNameLenMax < strlen(varname))//�ł������ϐ�����T���i�����I��v������邽�߁j
			{
				varNameLenMax = (int)strlen(varname);
				*p_var_id = varID;
				*pIndexIncliment = (int)strlen(varname);
			}
			//return true;
		}

		varID++;
	}

	if(varNameLenMax>0)
		return true;
	else
		return false;
}



/********************************************************************
�@  �\  ��  �� : �ϐ��錾��T��
��    ��    �� : FindVariableDefinission
��          �� :
��    ��    �l : �ϐ��錾������������ϐ��̐���Ԃ��@�G���[�̂΂�����-1��Ԃ�
�@          �\ : �ϐ��錾��T���āA�ϐ��o�b�t�@���m�ۂ���
				�ϐ��錾�Fvar [�ϐ���1], [�ϐ���2], ...
				�ϐ����ɂ͋L���i+-*=/|&'^~:.@#$!?()[]<>{}"\)����уX�y�[�X�͓���Ă͂����Ȃ�


���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CCalculate::FindVariableDefinission(
							 CString	comstr,		//(in)�����Ώۂ̕�����
							 CString	*p_err_message	//(out)�G���[���b�Z�[�W
							 )
{
	if( comstr.GetLength()<5 ){
		return 0;
	}

	//�ŏ����uvar �v����͂��܂�
	int comstr_def_begin = comstr.Find("var ");
	if(comstr_def_begin<0){
		return 0;
	}

	//�ϐ���1, �ϐ���2, �ϐ���3,,, �ƃJ���}�ŋ�؂�ꂽ�������؂蕪����
	CString substr[CALC_VARIABLE_BUFFER_LEN];

	int var_cand_cnt=0;
	substr[var_cand_cnt].Empty();
	for(int i=comstr_def_begin+4; i<comstr.GetLength() ; i++)
	{
		if(IsComment(comstr,i)){
			break;//�R�����g
		}

		if(comstr[i]==',')
		{
			if(var_cand_cnt+1 >=  CALC_VARIABLE_BUFFER_LEN){
				p_err_message->Format("Error:variable can be defined less than %d", CALC_VARIABLE_BUFFER_LEN);
				return -1;
			}

			var_cand_cnt++;
			substr[var_cand_cnt].Empty();
		}
		else
		{
			substr[var_cand_cnt] += CString(comstr[i]);
		}
	}

	//�ϐ����̑O��̃X�y�[�X��؂�l�߂�
	int var_cnt=0;
	for(int i=0 ; i<=var_cand_cnt ; i++)
	{
		if(substr[i].GetLength()==0)	continue;

		int startPos=0;
		for(int j=0 ; j<substr[i].GetLength()  ; j++){
			if( substr[i][j]==' ' || substr[i][j] == '\t'){
				startPos++;
			}else{
				break;
			}
		}

		int endPos = substr[i].GetLength()-1;
		for(int j=substr[i].GetLength()-1 ; j>=0 ; j++){
			if( substr[i][j]==' ' || substr[i][j] == '\t'){
				endPos--;
			}else{
				break;
			}
		}

		if(startPos<=endPos)
		{
			CString var_str = substr[i].Mid(startPos, endPos-startPos+1);

			if(var_str.FindOneOf("+-*/%=|&'^~:.@#$!?()[]<>{}\"\\ ")<0 && //�֎~�������܂܂�Ă��Ȃ���
				isalpha(var_str[0])  ){//�ŏ��̕����̓A���t�@�x�b�g�ł��邩

				if( new_variable(var_str, p_err_message) < 0 ){
					return -1;
				}

				var_cnt++;
			}else{
				p_err_message->Format("Error:invarild variable difinission");
				return -1;

			}
		}
	}

	return var_cnt;
}


/********************************************************************
�@  �\  ��  �� : �����񂩂�R�}���h���s����\������������
��    ��    �� : FindCtrlCode
��          �� :
��    ��    �l : ����������true
�@          �\ : 
				break/goto()/if()/else/endif
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::FindCtrlCode(
							 CString	comstr,				//(in)�����Ώۂ̕�����
							 int		start_index,		//(in)comstr���̌������J�n����ʒu
							 int		*p_ctrl_id,			//(out)����\���ԍ�
							 int		*pIndexIncliment	//(out)�C���f�b�N�X�C���N�������g��
							)
{

	*p_ctrl_id = 0;

	comstr.MakeUpper();

	for(int index=start_index ; index<comstr.GetLength() ; index++)
	{
		if(comstr[index]==' ' || comstr[index] == '\t'){
			continue;//����\���̑O�̃X�y�[�X�͖�������
		}
		else
		{
			if( comstr.Find("BREAK",index) == index )
			{	
				int next_index = index + CString("BREAK").GetLength();
				if( comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//break�̒���̓X�y�[�X�����I�[���s�̂�
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_BREAK;
				return true;
			}
			else if( comstr.Find("CLEAR",index) == index  )
			{
				int next_index = index + CString("CLEAR").GetLength();
				if( comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//clear�̒���̓X�y�[�X�����s���I�[�̂�
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_CLEAR;
				return true;
			}
			else if( comstr.Find("GOTO(",index) == index  )
			{
				int next_index = index + CString("GOTO").GetLength();

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_GOTO;
				return true;
			}
			else if( comstr.Find("IF(",index) == index  )
			{
				int next_index = index + CString("IF").GetLength();

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_IF;
				return true;
			}
			else if (comstr.Find("ELSEIF", index) == index)
			{
				int next_index = index + CString("ELSEIF").GetLength();

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ELSEIF;
				return true;
			}
			else if( comstr.Find("ELSE",index) == index  )
			{
				int next_index = index + CString("ELSE").GetLength();
				if( comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//else�̒���̓X�y�[�X�����s���I�[�̂�
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ELSE;
				return true;
			}
			else if( comstr.Find("ENDIF",index) == index  )
			{
				int next_index = index + CString("ENDIF").GetLength();
				if( comstr[next_index] != ' '  && comstr[next_index] != '\t'  &&comstr[next_index] != '\r' && comstr[next_index] != '\0'){//endif�̒���̓X�y�[�X�����s���I�[�̂�
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ENDIF;
				return true;
			}
			else if( comstr.Find("WHILE(",index) == index  )
			{
				int next_index = index + CString("WHILE").GetLength();

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_WHILE;
				return true;
			}
			else if (comstr.Find("ENDWHILE", index) == index)
			{
				int next_index = index + CString("WNDWHILE").GetLength();
				if (comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//endwhile�̒���̓X�y�[�X�����s���I�[�̂�
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ENDWHILE;
				return true;
			}
			else if (comstr.Find("FOR(", index) == index)
			{
					int next_index = index + CString("FOR").GetLength();

					*pIndexIncliment = next_index - start_index;
					*p_ctrl_id = CALC_CTRL_FOR;
					return true;
			}
			else if (comstr.Find("ENDFOR", index) == index)
			{
				int next_index = index + CString("ENDFOR").GetLength();
				if (comstr[next_index] != ' '  && comstr[next_index] != '\t'  && comstr[next_index] != '\r' && comstr[next_index] != '\0'){//endfor�̒���̓X�y�[�X�����s���I�[�̂�
					return false;
				}

				*pIndexIncliment = next_index - start_index;
				*p_ctrl_id = CALC_CTRL_ENDFOR;
				return true;
			}
			else{
				return false;
			}
		}
	}


	return false;
}



/********************************************************************
�@  �\  ��  �� : //�R�}���h���C������R�[�h�Ə�������̃W�����v��𒲂ׂ�
��    ��    �� : set_branch_property
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::set_branch_property(int com_str_cnt, CString *p_err_message)	
{
	for(int id=0 ; id<com_str_cnt ; id++){
		pCalcDataStructMap[id]->ctrlData.branchNestCount=0;//�l�X�g�̐[��������
		pCalcDataStructMap[id]->ctrlData.branchToLine[0]=0;//�W�����v�揉����
		pCalcDataStructMap[id]->ctrlData.branchToLine[1]=0;
		pCalcDataStructMap[id]->ctrlData.branchFrmLink = -1;//else/endif��if endwhile��while�t�����N
	}


	//if�`endif�̃l�X�g�̏�Ԓ���

	int nestCnt=0;
	for(int id=0 ; id<com_str_cnt ; id++)
	{
		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_IF){//if�Ȃ�΂�������l�X�g����[���Ȃ�
			nestCnt++;
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_WHILE){//while�Ȃ�΂�������l�X�g����[���Ȃ�
			nestCnt++;
		}

		if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_FOR){//for�Ȃ�΂�������l�X�g����[���Ȃ�
			nestCnt++;
		}

		pCalcDataStructMap[id]->ctrlData.branchNestCount = nestCnt;

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDIF){//endif�Ȃ�Ύ�����l�X�g����󂭂Ȃ�
			nestCnt--;
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDWHILE){//endwhile�Ȃ�Ύ�����l�X�g����󂭂Ȃ�
			nestCnt--;
		}

		if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDFOR){//endfor�Ȃ�Ύ�����l�X�g����󂭂Ȃ�
			nestCnt--;
		}
	}



	//if,elseif,else,while,for�̃W�����v�撲��
	for(int id=0 ; id<com_str_cnt ; id++)
	{
		if( pCalcDataStructMap[id]->ctrlData.controlCode  == CALC_CTRL_IF)//���ڂ���if
		{
			pCalcDataStructMap[id]->ctrlData.branchToLine[0] = id + 1;//�����ielseif(true)�̂Ƃ��j����̃W�����v��->���̒���̍s��


			//���ڂ���if�ƑΉ�����else/elseif/endif��T���āA�W�����v���E�W�����v���ݒ肷��

			bool endifFoundFlg = false;//endif�������������Ƃ�\���t���O
			int  previfid = id;//�ЂƂO��if�܂���elseif��ID
			int  elseID = -1;//�ЂƂO��else��ID
			for(int j=id+1 ; j<com_str_cnt ; j++)
			{	
				if (pCalcDataStructMap[previfid]->ctrlData.branchNestCount == pCalcDataStructMap[j]->ctrlData.branchNestCount)
				{
					 if (pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ELSEIF)//elseif����������
					{
						if (elseID >= 0){
							//elseif���else����Ɍ������Ă��܂��Ă�����G���[
							p_err_message->Format("invarid IF/ENDIF block");
							return false;
						}

						//�Ή�����if
						pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;

						//�W�����v��( if(false) / elseif(false) )����̃W�����v->���̍s��
						pCalcDataStructMap[previfid]->ctrlData.branchToLine[1] = j;

						//�����ielseif(true)�̂Ƃ��j����̃W�����v��->���̒���̍s��
						pCalcDataStructMap[j]->ctrlData.branchToLine[0] = j + 1;


						previfid = j;
					}
					else if( pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ELSE )//else����������
					{
						//�Ή�����if
						pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;

						//�W�����v��( if(false) / elseif(false) )����̃W�����v->���̍s��
						pCalcDataStructMap[previfid]->ctrlData.branchToLine[1] = j;

						//��������̃W�����v��(if�`endif���ɂ܂�True�̏������Ȃ��ꍇ)->���̒���̍s��
						pCalcDataStructMap[j]->ctrlData.branchToLine[0] = j + 1;


						elseID = j;
					}
					else if(pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ENDIF)//endif����������
					{
						//�Ή�����if
						pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;

						if(elseID>=0)//�W�����v����else
						{
							//�W�����v��( else )����̃W�����v->���̍s��
							pCalcDataStructMap[elseID]->ctrlData.branchToLine[1] = j;
						}
						else//�W�����v����if�Eelseif
						{
							//�W�����v��( if(false) / elseif(false) )����̃W�����v��->���̍s��
							pCalcDataStructMap[previfid]->ctrlData.branchToLine[1] = j;
						}

						endifFoundFlg = true;
						break;
					}

				}
				else if(pCalcDataStructMap[id]->ctrlData.branchNestCount > pCalcDataStructMap[j]->ctrlData.branchNestCount)
				{
					//���ڂ���if���l�X�g���󂭂Ȃ�����T���I��
					break;
				}
			}

			if(!endifFoundFlg){
				//�Ή�����endif��������Ȃ�������G���[
				p_err_message->Format("invarid IF/ENDIF block");
				return false;
			}
		}
		else
		if( pCalcDataStructMap[id]->ctrlData.controlCode  == CALC_CTRL_WHILE)//���ڂ���while
		{
			pCalcDataStructMap[id]->ctrlData.branchToLine[0] = id + 1;//while(true)�̃W�����v��Fwhile�̒���̍s

			//���ڂ���while�ƑΉ�����endwhile��T��
			bool endwhileFoundFlg=false;
			for(int j=id+1 ; j<com_str_cnt ; j++)
			{	
				if(pCalcDataStructMap[id]->ctrlData.branchNestCount== pCalcDataStructMap[j]->ctrlData.branchNestCount)
				{
					if(pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ENDWHILE)
					{
						//�Ή�����endwhile����������
						endwhileFoundFlg = true;
						pCalcDataStructMap[id]->ctrlData.branchToLine[1] = j+1;//while(false)�̃W�����v��F�����l�X�g��endwhile�̒���̍s
						pCalcDataStructMap[j]->ctrlData.branchToLine[0] = id;//endwhile�̃W�����v��F ���ڂ���while()�̍s
						pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;
						break;
					}
				}
				else if(pCalcDataStructMap[id]->ctrlData.branchNestCount > pCalcDataStructMap[j]->ctrlData.branchNestCount)
				{
					//���ڂ���while���l�X�g���󂭂Ȃ�����T���I��
					break;
				}
			}

			if(!endwhileFoundFlg){
				//�Ή�����endif��������Ȃ�������G���[
				p_err_message->Format("invarid WHILE/ENDWHILE block");
				return false;
			}
		}
		else
			if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_FOR)//���ڂ���for
			{
				pCalcDataStructMap[id]->ctrlData.branchToLine[0] = id + 1;//for(,true,)�̃W�����v��Fwhile�̒���̍s

				//���ڂ���for�ƑΉ�����endfor��T��
				bool endforFoundFlg = false;
				for (int j = id + 1; j<com_str_cnt; j++)
				{
					if (pCalcDataStructMap[id]->ctrlData.branchNestCount == pCalcDataStructMap[j]->ctrlData.branchNestCount)
					{
						if (pCalcDataStructMap[j]->ctrlData.controlCode == CALC_CTRL_ENDFOR)
						{
							//�Ή�����endfor����������
							endforFoundFlg = true;
							pCalcDataStructMap[id]->ctrlData.branchToLine[1] = j + 1;//for(false)�̃W�����v��F�����l�X�g��endfor�̒���̍s
							pCalcDataStructMap[j]->ctrlData.branchToLine[0] = id;//endfor�̃W�����v��F ���ڂ���for()�̍s
							pCalcDataStructMap[j]->ctrlData.branchFrmLink = id;
							break;
						}
					}
					else if (pCalcDataStructMap[id]->ctrlData.branchNestCount > pCalcDataStructMap[j]->ctrlData.branchNestCount)
					{
						//���ڂ���for���l�X�g���󂭂Ȃ�����T���I��
						break;
					}
				}

				if (!endforFoundFlg){
					//�Ή�����endif��������Ȃ�������G���[
					p_err_message->Format("invarid FOR/ENDFOR block");
					return false;
				}
			}
	}



	//�ǂ��Ƃ��Ή����Ȃ�elseif,else,endif�����endwhile�����for���Ȃ����`�F�b�N

	for(int id=0 ; id<com_str_cnt ; id++)
	{
		if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ELSEIF){
			if (pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid if/elseif/else/endif block");
				return false;
			}
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ELSE){
			if(pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid if/elseif/else/endif block");
				return false;
			}
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDIF){
			if(pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid if/endif block");
				return false;
			}
		}

		if(pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDWHILE){
			if(pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid while/endwhile block");
				return false;
			}
		}

		if (pCalcDataStructMap[id]->ctrlData.controlCode == CALC_CTRL_ENDFOR){
			if (pCalcDataStructMap[id]->ctrlData.branchFrmLink<0){
				p_err_message->Format("invarid for/endfor block");
				return false;
			}
		}
	}




	return true;
}


/********************************************************************
�@  �\  ��  �� : �R�}���h�����s����
��    ��    �� : ExecuteAPI
��          �� :
	pBlockMap  �ʉߒ��̏����u���b�N�iif�`endif, for�`endfor�j�̏�Ԃ�\���}�b�v�B�@�s�ԍ��ŃA�N�Z�X����bool�^�z��̐擪�|�C���^
	�@�@�@�@�@���Y�u���b�N�̒������s����ꍇ��true�B�X�L�b�v����ꍇ��false
��    ��    �l : �G���[�܂��́A���ɐi�߂Ȃ������Ȃ��false
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CCalculate::ExecuteAPI(
						    int			id,					//(in)���s����s�ԍ�
							CString		*p_err_message,		//(out)�G���[���b�Z�[�W
							int			*pNextID,			//(out)���Ɏ��s����s�ԍ�(NULL��ݒ肷�邱�Ƃ���)
							BATCH_INPUT *p_batch_in,		//(in)
							BATCH_OUTPUT *p_batch_out,		//(out)
							bool		autoDrawImage,		//(in)������ɉ摜�`������s����
							bool		*pBlockMap		//(in/out)
							)
{
	return Execute(id, 1, 1, p_err_message, -1, true, pNextID, p_batch_in, p_batch_out, autoDrawImage, pBlockMap);
}


/********************************************************************
�@  �\  ��  �� : ��f�P�ʂŃR�}���h�����s����
��    ��    �� : Execute
��          �� :
	pBlockMap  �ʉߒ��̏����u���b�N�iif�`endif, for�`endfor�j�̏�Ԃ�\���}�b�v�B�@�s�ԍ��ŃA�N�Z�X����bool�^�z��̐擪�|�C���^
�@�@�@�@	�@���Y�u���b�N�̒������s����ꍇ��true�B�X�L�b�v����ꍇ��false
��    ��    �l : �G���[�܂��́A���ɐi�߂Ȃ������Ȃ��false
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CCalculate::Execute(
	int			id,					//(in)���s����s�ԍ�
	int			x,					//(in)�摜��̍��Wx
	int			y,					//(in)�摜��̍��Wy
	CString		*p_err_message,		//(out)�G���[���b�Z�[�W
	int			rgb,				//(in)
	bool		ApiMode,			//(in)PIMPOM API�̃R�}���h�����s���郂�[�h
	int			*pNextID,			//(out)���Ɏ��s����s�ԍ�(NULL��ݒ肷�邱�Ƃ���)
	BATCH_INPUT *p_batch_in,		//(in)
	BATCH_OUTPUT *p_batch_out,		//(out)
	bool		autoDrawImage,		//(in)������ɉ摜�`������s����
	bool		*pBlockMap			//(in/out)
	)
{
	//�����`�F�b�N
	if (id < 0 || id >= GetCommandLineLen()){
		//p_err_message->Format("Error: line number must be within 0-%d", GetCommandLineLen()-1);
		//set_error_message(id, *p_err_message);
		return false;
	}



	COMMAND		CommandMemoryTmp[CALC_COMMAND_LIST_MAX] = { 0 };//��Ɨp
	COMMAND		AdditionalCommandMemory[CALC_COMMAND_LIST_MAX] = { 0 };//��Ɨp
	int			AdditionalCommandMemoryIndex = 0;//��Ɨp


	CALC_CTRL_DATA	ctrlData = { 0 };
	COMMAND		*pCommandList = get_command_list(id, &ctrlData);//�R�}���h���X�g�擾
	int			commandListLen = get_command_listLen(pCommandList);//�R�}���h���X�g�̒���

	if (commandListLen <= 2 && ctrlData.controlCode == 0)//�R�}���h�����Ȃ��A �R�}���h���C������R�[�h���Ȃ�
	{
		//���̍s�͖������čs�ԍ���i�߂�
		p_err_message->Format("---");
		set_error_message(id, *p_err_message);

		if (pNextID){
			*pNextID = id + 1;
		}
	}
	else
	{
		if(ctrlData.controlCode==CALC_CTRL_CLEAR){//�R�}���h���C������R�[�h���N���A����
			ClearAll();
			*pNextID = 0;
			return false;
		}

		copy_command_list(CommandMemoryTmp, pCommandList, commandListLen);//�R�}���h���X�g�̃R�s�[���Ƃ�i���s����Ɠ��e���ς�邽�߁j


		//���̓p�����^�ݒ�
		CALC_PARAM_STRUCT param={0};
		param.x = x;
		param.y = y;
		param.rgbType = rgb;
		param.apiMode = ApiMode;
		param.pbatchIn = p_batch_in;
		param.pbatchOut = p_batch_out;

		DWORD start_tick = API.Tick(); //�����J�n����

		bool ret;
		if (ctrlData.controlCode == CALC_CTRL_FOR)//for��
		{
			bool mode;
			if (pBlockMap[id] == false){//for���̒��ɏ��߂Ă͂���
				mode = 0;//������->���[�v�����v�Z
			}
			else{//for���̃��[�v���ł���
				mode = 1;//�J�E���^�X�V->���[�v�����v�Z
			}
			ret = calculate_for_command_list(id, mode, &param, CommandMemoryTmp, AdditionalCommandMemory, &AdditionalCommandMemoryIndex, p_err_message, autoDrawImage);//�v�Z���s
		}
		else//for���ȊO
		{
			ret = calculate_command_list(id, &param, CommandMemoryTmp, AdditionalCommandMemory, &AdditionalCommandMemoryIndex, p_err_message, autoDrawImage);//�v�Z���s
		}

		if( ret )//�v�Z���s
		{
			set_result(id, CommandMemoryTmp, API.Tick() - start_tick );//�v�Z���ʊi�[


			if(pNextID){
				//�R�}���h���C������R�[�h���ƂɁA���Ɏ��s����s�ԍ������߂�
				if (!set_next_id(id, ctrlData, CommandMemoryTmp, p_err_message, pNextID, pBlockMap))
				{
					set_error_message(id, *p_err_message);
					delete_non_static_string_buffer(id);//�R�}���h���s�Ŏg����������o�b�t�@���N���A
					return false;
				}
			}
		}
		else
		{
			set_error_message(id, *p_err_message);
			delete_non_static_string_buffer(id);//�R�}���h���s�Ŏg����������o�b�t�@���N���A
			return false;
		}

		p_err_message->Format("OK");
		set_error_message(id, *p_err_message);
		delete_non_static_string_buffer(id);//�R�}���h���s�Ŏg����������o�b�t�@���N���A
	
	}

	return true;
}


/********************************************************************
�@  �\  ��  �� : �R�}���h���C������R�[�h�ƌv�Z���ʂ̐������̃`�F�b�N
��    ��    �� : CheckCtrlCode
��          �� : 
		pBlockMap  �ʉߒ��̏����u���b�N�iif�`endif, for�`endfor�j�̏�Ԃ�\���}�b�v�B�@�R�}���h�Ɠ������т�bool�^�z��̐擪�|�C���^
�@�@�@�@�@���Y�u���b�N�̒������s����ꍇ��true�B�X�L�b�v����ꍇ��false
��    ��    �l : �G���[�܂��́A���ɐi�߂Ȃ������Ȃ��false
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/

bool	CCalculate::set_next_id(int id, CALC_CTRL_DATA ctrlData, COMMAND *com, CString *p_err_message, int *pNextID, bool *pBlockMap)
{
	if(ctrlData.controlCode==CALC_CTRL_BREAK)//break
	{
		//��ɂ����܂Ȃ�
		*pNextID = id;
		return false;
	}

	if(ctrlData.controlCode == CALC_CTRL_GOTO)//goto
	{
		//���s�s�ԍ������]�̈ʒu�Ɉړ�������
		if(com->type==COM_TYPE_NUMBER){
			if(com->number>=0 && com->number<GetCommandLineLen())
			{	
				*pNextID = (int)com->number;

			}else{
				//�ړ��ł��Ȃ��ꏊ���w���Ă���
				p_err_message->Format("Error: goto() argument must be within 0-%d", GetCommandLineLen()-1);
				return false;				
			}
		}else{
			//goto()�̈��������l�łȂ�
			p_err_message->Format("Error: goto() argument must be number");
			return false;
		}
	}
	else if(ctrlData.controlCode == CALC_CTRL_IF)//if
	{
		//���s�s�ԍ������]�̈ʒu�Ɉړ�������
		if(com->type==COM_TYPE_NUMBER)
		{
			if(com->number!=0)
			{//if(true)
				*pNextID = ctrlData.branchToLine[0];//����if�̒������s

				if (pBlockMap){
					pBlockMap[id] = true;
				}
			}
			else
			{//if(false)
				*pNextID = ctrlData.branchToLine[1];//����if�̒��̓X�L�b�v
			}
		}
		else
		{
			//if()�̈��������l�łȂ�
			p_err_message->Format("Error: if() argument must be number");
			return false;
		}
	}
	else if (ctrlData.controlCode == CALC_CTRL_ELSEIF)//elseif
	{
		if (com->type == COM_TYPE_NUMBER)
		{
			bool passed_block_flg = true;
			if (pBlockMap){
				passed_block_flg = pBlockMap[ctrlData.branchFrmLink];
			}

	
			if (com->number != 0 && passed_block_flg==false)
			{	//elseif(true)�ł���
				//�����YIf�`endif�u���b�N��ʉߒ��ł܂�true�̏������������Ă��Ȃ�

				*pNextID = ctrlData.branchToLine[0];//����elseif�̒������s

				if (pBlockMap){
					pBlockMap[ctrlData.branchFrmLink] = true;
				}
			}
			else
			{//elseif(false)�܂��͓��YIf�`endif�u���b�N�ł��ł�true�̏������������Ă���

				*pNextID = ctrlData.branchToLine[1];//����elseif�̒��̓X�L�b�v
			}
		}
		else
		{
			//if()�̈��������l�łȂ�
			p_err_message->Format("Error: elseif() argument must be number");
			return false;
		}
	}
	else if(ctrlData.controlCode == CALC_CTRL_ELSE)//else
	{
		bool passed_block_flg = true;
		if (pBlockMap){
			passed_block_flg = pBlockMap[ctrlData.branchFrmLink];
		}


		if (passed_block_flg==false)//���YIf�`endif�u���b�N��ʉߒ��ł܂�true�̏������������Ă��Ȃ�
		{
			*pNextID = ctrlData.branchToLine[0];//else�������s

			if (pBlockMap){
				pBlockMap[ctrlData.branchFrmLink] = true;
			}
		}
		else{
			*pNextID = ctrlData.branchToLine[1];//endif�܂ŃX�L�b�v
		}


	}
	else if (ctrlData.controlCode == CALC_CTRL_ENDIF)//endif
	{
		*pNextID = id + 1;

		if (pBlockMap){
			pBlockMap[ctrlData.branchFrmLink] = false;
		}
	}
	else if(ctrlData.controlCode == CALC_CTRL_WHILE)//while
	{
		//���s�s�ԍ������]�̈ʒu�Ɉړ�������
		if(com->type==COM_TYPE_NUMBER)
		{
			if(com->number!=0)
			{//while(true)
				*pNextID = ctrlData.branchToLine[0];//���̃u���b�N�������s

				if (pBlockMap){
					pBlockMap[id] = true;
				}
			}
			else{//while(false)
				*pNextID = ctrlData.branchToLine[1];//���̃u���b�N�����X�L�b�v


				if (pBlockMap){
					pBlockMap[id] = false;
				}
			}
		}
		else
		{
			//if()�̈��������l�łȂ�
			p_err_message->Format("Error: white() argument must be number");
			return false;
		}
	}
	else if(ctrlData.controlCode == CALC_CTRL_ENDWHILE)//endwhile
	{
		//���s�s�ԍ������]�̈ʒu�Ɉړ�������
		*pNextID  = ctrlData.branchToLine[0];
	}
	else if (ctrlData.controlCode == CALC_CTRL_FOR)//for
	{
		//���s�s�ԍ������]�̈ʒu�Ɉړ�������
		if (com->type == COM_TYPE_NUMBER)
		{
			if (com->number != 0)
			{	//if(true)����
				*pNextID = ctrlData.branchToLine[0];//���̃u���b�N�������s

				if (pBlockMap){
					pBlockMap[id] = true;
				}
			}
			else{//if(false)
				*pNextID = ctrlData.branchToLine[1];//���̃u���b�N�����X�L�b�v

				if (pBlockMap){
					pBlockMap[id] = false;
				}
			}
		}
		else
		{
			//if()�̈��������l�łȂ�
			p_err_message->Format("Error: for() argument must be number");
			return false;
		}
	}
	else if (ctrlData.controlCode == CALC_CTRL_ENDFOR)//endfor
	{
		//���s�s�ԍ������]�̈ʒu�Ɉړ�������
		*pNextID = ctrlData.branchToLine[0];
	}
	else
	{
		*pNextID = id + 1;
	}

	return true;
}


/********************************************************************
�@  �\  ��  �� : �R�}���h���X�g���������s����
��    ��    �� : Calclate
��          �� :
��    ��    �l : ����Ɍv�Z���ꂽ��true
�@          �\ : 
			�R�}���h���X�g���ɂ����āA�u(�v  ����  �u)�v �܂ł̋�Ԃ��v�Z���Đ��l�܂��͕�����ɂ���������B	
			�l�X�g���ꂽ ( ������΍ċN�Ăяo��������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::calculate_command_list( 
						  int				id,
						 CALC_PARAM_STRUCT	*p_calc_param,			//(in/out)�v�Z�̓��o�̓p�����^
						 COMMAND			*start ,				//(in/out)�v�Z���J�n����A�R�}���h���X�g���̃|�C���^�B���@�֐��I����ɂ͐��lor�����񂪓����Ă���
						 COMMAND			*additonal_com,			//(work)
						 int				*additional_com_id,		//(work)
						 CString			*p_err_message,			//(out)�G���[���b�Z�[�W
						 bool				autoDrawImage			//(in)������ɉ摜�`������s����
						 )
{
	COMMAND		*command ;//���ڂ��鉉�Z�q


	if(start == NULL){//�ŏ��̃R�}���h���Ȃ�
		p_err_message->Format("Syntex Error (Calclate0)");
		return	false;
	}

	if(start->type != COM_TYPE_PARENT  || start->pare != COM_PARENT_OPEN){//�ŏ��̃R�}���h���u(�v�łȂ�
		p_err_message->Format("Syntex Error (Calclate1)");
		return	false;
	}

	


	//�}�N�������()�̃l�X�g������

	for( command = start->next ; command != NULL ; command = command->next )//�R�}���h���X�g����������
	{
		if( command->type == COM_TYPE_MACRO )
		{//�}�N������������
			if( !calculate_command_list_with_macro(id, p_calc_param, command , additonal_com, additional_com_id, p_err_message, autoDrawImage) )	return false;
		}
		else if( command->type == COM_TYPE_PARENT )
		{
			if( command->pare == COM_PARENT_OPEN)
			{// �l�X�g���ꂽ()������������
				if( !calculate_command_list(id, p_calc_param, command , additonal_com, additional_com_id, p_err_message,autoDrawImage) ) return  false;
			}else{
				break;// �u ) �v �������������ߌ����I��
			}
		}
		else if( command->type ==  COM_TYPE_COMMA)
		{
			break;// �u , �v �������������ߌ����I��
		}

	}


	

	//�ϐ���l�i���l�^������j�ɒu��������(�������A������Z�q�̒��O�̕ϐ��͒u�������Ȃ�)
	for( command = start->next ; command != NULL ; command = command->next)//�R�}���h���X�g����������
	{
		if(command->type == COM_TYPE_PARENT && command->pare == COM_PARENT_CLOSE){// �u ) �v �������������ߌ����I��
			
			
			break;
		}

		if( command->type == COM_TYPE_VARIABLE )
		{
			if (command->next->type == COM_TYPE_OPERATOR &&
				(command->next->ope == 10 || command->next->ope == 13 || command->next->ope == 14 || command->next->ope == 15 || command->next->ope == 16))
			{
				continue;	//������Z�q�̒��O�̕ϐ��̏ꍇ�͏������X�L�b�v
			}

			CALC_RESULT_STRUCT varres={0};
			char	varname[CALC_VARIABLE_NAME_MAX]={0};

			GetVariable(command->var_id, &varres, varname);//�ϐ����擾

			if( varres.type == COM_TYPE_NUMBER )//�ϐ��̒��g�����l�̏ꍇ
			{
				command->type = COM_TYPE_NUMBER;
				command->number = varres.number;
			}
			else if( varres.type == COM_TYPE_STRING )//�ϐ��̒��g��������̏ꍇ
			{
				int ret = push_string_to_buffer(id, CString(varres.str), p_err_message) ;
				if(ret>=0){
					command->type = COM_TYPE_STRING;
					command->str_id = ret;
				}else{
					return false;
				}
			}
		}
	}


	//i,j,x,y�𐔒l�ɒu��������
	for( command = start->next ; command != NULL ; command = command->next)//�R�}���h���X�g����������
	{
		if(command->type == COM_TYPE_PARENT && command->pare == COM_PARENT_CLOSE){// �u ) �v �������������ߌ����I��
			break;
		}

		if( command->type == COM_TYPE_COOR_X ){//i,x
			command->type = COM_TYPE_NUMBER;
			command->number = (double)p_calc_param->x;
		}else if( command->type == COM_TYPE_COOR_Y ){//j,y
			command->type = COM_TYPE_NUMBER;
			command->number = (double)p_calc_param->y;
		}
	}


	//���̒i�K�Ŗ��߂�����ȏꍇ�� [(][���l][���Z�q][���l][���Z�q]...[���l][)] �̏��ŕ���ł���B

	//��O:���̕��������l�̑Ή��P
	if( start->next->type == COM_TYPE_OPERATOR && start->next->ope == 1)//�u ( �v�̒��オ�u-�v�ł���
	{
			//[(][-][���l]��[(][0][-][���l]
			additonal_com[ *additional_com_id ].type = COM_TYPE_NUMBER;
			additonal_com[ *additional_com_id ].number = 0.0;
			additonal_com[ *additional_com_id ].prev = start;
			additonal_com[ *additional_com_id ].next = start->next;
			start->next->prev = additonal_com + *additional_com_id;
			start->next = additonal_com + *additional_com_id;
			(*additional_com_id) ++;
	}

	//��O:���̕��������l�̑Ή��Q
	for( command = start->next ; command != NULL ; command = command->next)
	{
		if(command->type==COM_TYPE_OPERATOR && command->ope == 1 &&
			command->prev->type==COM_TYPE_OPERATOR && 
			(command->prev->ope == 10 || command->prev->ope == 13 || command->prev->ope == 14 || command->prev->ope == 15 || command->prev->ope == 16))//������Z�q�̒��オ�u-�v�ł���
		{
			//[=][-][���l]��[=][0][-][���l]
			additonal_com[ *additional_com_id ].type = COM_TYPE_NUMBER;
			additonal_com[ *additional_com_id ].number = 0.0;
			additonal_com[ *additional_com_id ].prev = command->prev;
			additonal_com[ *additional_com_id ].next = command;
			command->prev->next = additonal_com + *additional_com_id;
			command->prev = additonal_com + *additional_com_id;
			(*additional_com_id) ++;
		}
	}



	//��]�Z����������
	for( command = start->next ; command != NULL ; command = command->next)//�R�}���h���X�g����������
	{
		if( command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE){// �u ) �v �������������ߌ����I��
			break;
		}

		if( command->type == COM_TYPE_OPERATOR )//���Z�q
		{		
			if( command->ope == 2  ||  command->ope == 3 )	//��]�Z
			{		

				if( command->prev->type != COM_TYPE_NUMBER  ||  command->next->type != COM_TYPE_NUMBER){//���Z�q�̑O�オ���l�ł͂Ȃ�
					p_err_message->Format("Syntax Error (Calclate2)");
					return false;
				}


				double	 result;
					if( command->ope == 2 ){	//�|���Z
						result = command->prev->number * command->next->number;
					}else{						//����Z
						result = command->prev->number / command->next->number;
					}

				//�g�������߂����X�g����폜���đ���Ɍv�Z���ʂ�����
				COMMAND	*temp_next = command->next;
				command = command->prev;
				command->type = COM_TYPE_NUMBER;
				command->number = result;
				command->next = temp_next->next;
				temp_next->next->prev = command;
			}
		}
	}


	//�����Z����������

	for( command = start->next ; command != NULL ; command = command->next)//�R�}���h���X�g����������
	{
		if( command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE){// �u ) �v �������������ߌ����I��
			break;
		}

		if( command->type == COM_TYPE_OPERATOR )//���Z�q
		{		
			if( command->ope == 0  ||  command->ope == 1 )	//�����Z
			{		
				if( command->prev->type == COM_TYPE_NUMBER  &&  command->next->type == COM_TYPE_NUMBER)//���Z�q�̑O�オ���l�ł���
				{
					double	 result;
						if( command->ope == 0 ){	//�����Z
							result = command->prev->number + command->next->number;
						}else{						//�����Z
							result = command->prev->number - command->next->number;
						}

					//�����Z�Ɏg�������߂����X�g����폜���đ���Ɍv�Z���ʂ�����
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = result;
					command->next = temp_next->next;
					temp_next->next->prev = command;


				}
				else if( command->ope == 0 && (command->prev->type == COM_TYPE_STRING &&  command->next->type == COM_TYPE_STRING ) )//+�̗�����������ł���
				{
					//�����񍇐�
					CString str = CString( get_string_from_buffer(id, command->prev->str_id) ) + CString( get_string_from_buffer(id, command->next->str_id) );
				
					int str_id = push_string_to_buffer(id, str, p_err_message);
					if(str_id<0){
						return false;
					}
					
					//�g�������߂����X�g����폜���đ���Ɍv�Z���ʂ�����
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_STRING;
					command->str_id = str_id;
					command->next = temp_next->next;
					temp_next->next->prev = command;
				}
				else
				{
					p_err_message->Format("Syntax Error (Calclate3)");
					return false;
				}



			}
		}
	}




	//��r���Z�q����������

	for( command = start->next ; command != NULL ; command = command->next)//�R�}���h���X�g����������
	{
		if( command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE){// �u ) �v �������������ߌ����I��
			break;
		}

		if( command->type == COM_TYPE_OPERATOR )//���Z�q
		{		
			if( command->ope == 4  ||  command->ope == 5 || command->ope == 6 ||command->ope == 7 || command->ope == 8 || command->ope == 9 )	//��r���Z�q
			{		
				if( command->prev->type == COM_TYPE_NUMBER  &&  command->next->type == COM_TYPE_NUMBER)//���Z�q�̑O�オ���l�ł���
				{
					double	 result;
						if( command->ope == 4 ){	//==
							result = (command->prev->number == command->next->number ) ? 1 :0 ;
						}else if( command->ope == 5 ){	//<
							result = (command->prev->number < command->next->number ) ? 1 : 0;
						}else if( command->ope == 6 ){	//>
							result = (command->prev->number > command->next->number ) ? 1 : 0;
						}else if( command->ope == 7 ){	//<=
							result = (command->prev->number <= command->next->number ) ? 1 : 0;
						}else if( command->ope == 8 ){	//>=
							result = (command->prev->number >= command->next->number ) ? 1 : 0;
						}else if( command->ope == 9 ){	//!=
							result = (command->prev->number != command->next->number ) ? 1 : 0;
						}
			

					//�g�������߂����X�g����폜���đ���Ɍv�Z���ʂ�����
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = result;
					command->next = temp_next->next;
					temp_next->next->prev = command;
				}
				else if( (command->ope == 4 || command->ope == 9) && command->prev->type == COM_TYPE_STRING  &&  command->next->type == COM_TYPE_STRING)//���Z�q(==,!=)�̑O�オ������ł��� �ˁ@�������r���A�����Ȃ��0,�Ⴆ��1
				{
					int result=0; 
					if( strcmp( get_string_from_buffer(id, command->prev->str_id), get_string_from_buffer(id, command->next->str_id) )==0 ){//�����񂪓���
						if( command->ope == 4 ){//==
							result=1;
						}else{//!=
							result=0;
						}
					}else{
						if( command->ope == 4 ){//==
							result=0;
						}else{//!=
							result=1;
						}
					}

					

					//�g�������߂����X�g����폜���đ���Ɍv�Z���ʂ�����
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = result;
					command->next = temp_next->next;
					temp_next->next->prev = command;
				}else{
					p_err_message->Format("Syntax Error (Calclate4)");
					return false;
				}
			}
		}
	}

	//�_����r���Z�q����������

	for (command = start->next; command != NULL; command = command->next)//�R�}���h���X�g����������
	{
		if (command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE) {// �u ) �v �������������ߌ����I��
			break;
		}

		if (command->type == COM_TYPE_OPERATOR)//���Z�q
		{
			if ( command->ope == 11 || command->ope == 12)	//��r���Z�q
			{
				if (command->prev->type == COM_TYPE_NUMBER  &&  command->next->type == COM_TYPE_NUMBER)//���Z�q�̑O�オ���l�ł���
				{
					double	 result;
					 if (command->ope == 11) {	//&&
						result = (command->prev->number != 0) && (command->next->number != 0);
					}
					else if (command->ope == 12) {	//||
						result = (command->prev->number != 0) || (command->next->number != 0);
					}


					//�g�������߂����X�g����폜���đ���Ɍv�Z���ʂ�����
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = result;
					command->next = temp_next->next;
					temp_next->next->prev = command;
				}
				else {
					p_err_message->Format("Syntax Error (Calclate4)");
					return false;
				}
			}
		}
	}


	//������Z�q����������

	for( command = start->next ; command != NULL ; command = command->next)//�R�}���h���X�g����������
	{
		if( command->type == COM_TYPE_PARENT  && command->pare == COM_PARENT_CLOSE){// �u ) �v �������������ߌ����I��
			break;
		}

		if( command->type == COM_TYPE_OPERATOR )//���Z�q
		{		
			if (command->ope == 10 || command->ope == 13 || command->ope == 14 || command->ope == 15 || command->ope == 16)
			{
				if (command->prev->type == COM_TYPE_VARIABLE  &&  command->next->type == COM_TYPE_NUMBER)//���Z�q�̑O���ϐ��A�オ���l�ł���
				{

					//�ϐ��ɐ��l��������
					CALC_RESULT_STRUCT varres = { 0 };
					char varname[CALC_VARIABLE_NAME_MAX];

					GetVariable(command->prev->var_id, &varres, varname);

					if (command->ope == 10){ // =
						varres.type = COM_TYPE_NUMBER;
						varres.number = command->next->number;
					}
					else if (varres.type == COM_TYPE_NUMBER)
					{
						if (command->ope == 13){// += 
							varres.number += command->next->number;
						}
						else if (command->ope == 14){// -= 
							varres.number -= command->next->number;
						}
						else if (command->ope == 15){// *= 
							varres.number *= command->next->number;
						}
						else if (command->ope == 16){// /= 
							varres.number /= command->next->number;
						}
						else{
							p_err_message->Format("Syntax Error (Calclate4)");
							return false;
						}
					}
					else{
						p_err_message->Format("Syntax Error (Calclate4)");
						return false;
					}

					SetVariable(command->prev->var_id, &varres);

					//�g�������߂����X�g����폜���đ���Ɍv�Z����(���l)������
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_NUMBER;
					command->number = varres.number;
					command->next = temp_next->next;
					temp_next->next->prev = command;

				}
				else if (command->ope == 10 && command->prev->type == COM_TYPE_VARIABLE  &&  command->next->type == COM_TYPE_STRING)//���Z�q�̑O���ϐ��A�オ������ł���
				{

					//�ϐ��ɕ������������
					CALC_RESULT_STRUCT varres = { 0 };
					char varname[CALC_VARIABLE_NAME_MAX];

					GetVariable(command->prev->var_id, &varres, varname);

					if (command->ope == 10){ // =
						varres.type = COM_TYPE_STRING;
						strcpy(varres.str, get_string_from_buffer(id, command->next->str_id));
					}
					else if (varres.type == COM_TYPE_STRING)
					{
						if (command->ope == 13)//+=
						{
							if (strlen(varres.str) + strlen(get_string_from_buffer(id, command->next->str_id)) >= CALC_STRING_LEN_MAX)
							{
								p_err_message->Format("too much long strings!");
								return false;
							}

							strcat(varres.str, get_string_from_buffer(id, command->next->str_id));
						}
						else{
							p_err_message->Format("Syntax Error (Calclate4)");
							return false;
						}
					}
					else{
						p_err_message->Format("Syntax Error (Calclate4)");
						return false;
					}

					SetVariable(command->prev->var_id, &varres);

					//�g�������߂����X�g����폜���đ���Ɍv�Z���ʂ�����
					COMMAND	*temp_next = command->next;
					command = command->prev;
					command->type = COM_TYPE_STRING;
					command->str_id = temp_next->str_id;
					command->next = temp_next->next;
					temp_next->next->prev = command;

				}
				else{
					p_err_message->Format("Syntax Error (Calclate4)");
					return false;
				}
			}
		}
	}




	//�㏈��

	//���̒i�K�ł� () �܂��� ( ���l or������ )�̏��ɕ���ł���
	//�܂��Acommand�͍Ō��)��\���Ă���

	if(start->next->type == COM_TYPE_NUMBER)//(���l)
	{
		start->type = COM_TYPE_NUMBER;
		start->number = start->next->number;
		start->next = command->next;
			if( start->next != NULL ){//���̖��ߌꂪ����ꍇ�̓A�h���X��n��
				start->next->prev = start;
			}
	}
	else if(start->next->type == COM_TYPE_STRING)//(������)
	{
		start->type = COM_TYPE_STRING;
		start->str_id = start->next->str_id;
		start->next = command->next;
			if( start->next != NULL ){//���̖��ߌꂪ����ꍇ�̓A�h���X��n��
				start->next->prev = start;
			}
	}else{//()
		start->type = COM_TYPE_NUMBER;
		start->number = 0;
		start->next = command->next;
			if( start->next != NULL ){//���̖��ߌꂪ����ꍇ�̓A�h���X��n��
				start->next->prev = start;
			}
	}


	return true;
}


/********************************************************************
�@  �\  ��  �� : for�������s����
��    ��    �� : calculate_command_list_with_macro
��          �� :
��    ��    �l : ����Ɍv�Z���ꂽ��true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::calculate_for_command_list(
	int				id,						//(in)
	int				mode,					//(in)0:������->���[�v�p�������v�Z �@1:�J�E���^�X�V->���[�v�p�������v�Z
	CALC_PARAM_STRUCT	*p_calc_param,		//(in/out)�v�Z�̓��o�̓p�����^
	COMMAND			*start,					//(in/out)�R�}���h���X�g����for���ւ̃|�C���^�B���@�֐��I����ɂ͐��lor�����񂪓����Ă���
	COMMAND			*additonal_com,			//(work)
	int				*additional_com_id,		//(work)
	CString			*p_err_message,			//(out)�G���[���b�Z�[�W
	bool			autoDrawImage		//(in)������ɉ摜�`������s����
	)
{
	COMMAND		*command;
	MACRO_IO_STRUCT	arg_val[CALC_MACRO_ARGS_MAX] = { 0 };//�}�N������
	MACRO_IO_STRUCT	result_val = { 0 };//�}�N�����s����
	int			overloadID = 0;//�}�N�������̃I�[�o�[���C�h�ԍ�
	


	//���͎��_�ł̓R�}���h���X�g�͈ȉ��̕��тɂȂ��Ă���͂��ł���
	//[for][(].....[,].....[,].....[)]


	if (start->next == NULL){//for�̎��̃R�}���h���Ȃ�
		p_err_message->Format("Syntex Error (calculate_for_command_list)");
		return	false;
	}
	if (start->next->type != COM_TYPE_PARENT || start->next->pare != 0){//for�̎��̃R�}���h���u(�v�łȂ�
		p_err_message->Format("Syntex Error (calculate_for_command_list)");
		return	false;
	}

	if (start->next->next == NULL){//for�̎��̎��̃R�}���h���Ȃ�
		p_err_message->Format("Syntex Error (calculate_for_command_list)");
		return	false;
	}




		int			arg_counter = 0;//�u,�v�łȂ������\���̐�

		//������,���[�v�p�������v�Z,�J�E���^�X�V�̍\����T��

		COMMAND	*p_arg_com[3] = { 0 };//���ߌ�̐擪�A�h���X�̊i�[��

		p_arg_com[arg_counter++] = start->next;//�������ƂȂ閽�ߌ�̐擪�A�h���X

		for (command = start->next; command != NULL; command = command->next)
		{
			if (command->type == COM_TYPE_COMMA)
			{//�u,�v����������

				if (arg_counter == 3){
					p_err_message->Format("Syntax Error (calculate_for_command_list)");
					return false;
				}

				//�u,�v���R�}���h����폜���Ă��̈ʒu�Ɂu ) �v�Ɓu ( �v��}������
				// for( a , b , c) �� for( a )( b )( c )
				command->type = COM_TYPE_PARENT;
				command->pare = COM_PARENT_CLOSE;
				additonal_com[*additional_com_id].type = COM_TYPE_PARENT;
				additonal_com[*additional_com_id].pare = COM_PARENT_OPEN;
				additonal_com[*additional_com_id].prev = command;
				additonal_com[*additional_com_id].next = command->next;
				command->next = &additonal_com[*additional_com_id];
				(*additional_com_id)++;

				p_arg_com[arg_counter++] = command->next;//�����ƂȂ閽�ߌ�̐擪�A�h���X
			}
			else if (command->type == COM_TYPE_PARENT && command->pare == COM_PARENT_CLOSE)
			{
				//�u)�v���݂�����������T���I��
				break;
			}
		}

		if (arg_counter != 3)
		{
			p_err_message->Format("Syntax Error (calculate_for_command_list)");
			return false;
		}


		//���ߌ�����s����

		if (mode == 0)
		{
			if (!calculate_command_list(id, p_calc_param, p_arg_com[0], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
			if (!calculate_command_list(id, p_calc_param, p_arg_com[1], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
		}
		else{
			if (!calculate_command_list(id, p_calc_param, p_arg_com[2], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
			if (!calculate_command_list(id, p_calc_param, p_arg_com[1], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
		}


		//���̒i�K�ł�p_arg_com[1]�̈ʒu�ɂ͕����񂩐��l�����Ă���͂��ł���

		if (p_arg_com[1]->type == COM_TYPE_NUMBER)
		{
			//���̍s�̌��ʂ����ւ���
			start->type = p_arg_com[1]->type;
			start->number = p_arg_com[1]->number;
		}
		else
		{
			p_err_message->Format("Error: 2nd argument of for() must be number");
			return false;
		}


	return true;
}



/********************************************************************
�@  �\  ��  �� : �R�}���h���X�g�̃}�N���������������s����
��    ��    �� : calculate_command_list_with_macro
��          �� :
��    ��    �l : ����Ɍv�Z���ꂽ��true
�@          �\ : 
			�R�}���h���X�g���ɂ����āA�}�N����Ԃ��v�Z���Đ��l�܂��͕�����ɂ���������B	
			�l�X�g���ꂽ ( ������΍ċN�Ăяo��������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
2020.04.29			 �u()�v�̃l�X�g�ɑΉ�
********************************************************************/
bool CCalculate::calculate_command_list_with_macro(
						     int				id,						//(in)
							 CALC_PARAM_STRUCT	*p_calc_param,			//(in/out)�v�Z�̓��o�̓p�����^
							 COMMAND			*start,					//(in/out)�R�}���h���X�g���̃}�N���ւ̃|�C���^�B���@�֐��I����ɂ͐��lor�����񂪓����Ă���
							 COMMAND			*additonal_com,			//(work)
							 int				*additional_com_id,		//(work)
							 CString			*p_err_message,			//(out)�G���[���b�Z�[�W
							 bool				autoDrawImage			//(in)������ɉ摜�`������s����
							 )
{
	COMMAND		*command;
	MACRO_IO_STRUCT	arg_val[CALC_MACRO_ARGS_MAX]={0};//�}�N������
	MACRO_IO_STRUCT	result_val={0};//�}�N�����s����
	int			overloadID = 0;//�}�N�������̃I�[�o�[���C�h�ԍ�
	int			arg_counter = 0;//�����̐�




	//���͎��_�ł̓R�}���h���X�g�͈ȉ��̕��тɂȂ��Ă���͂��ł���
	//[�}�N��][(].....[,].....[)]


	if(start->next == NULL){//�}�N���̎��̃R�}���h���Ȃ�
		p_err_message->Format("Syntex Error (calculate_command_list_with_macro0)");
		return	false;
	}
	if(start->next->type != COM_TYPE_PARENT  || start->next->pare != 0){//�}�N���̎��̃R�}���h���u(�v�łȂ�
		p_err_message->Format("Syntex Error (calculate_command_list_with_macro1)");
		return	false;
	}

	if(start->next->next==NULL ){//�}�N���̎��̎��̃R�}���h���Ȃ�
		p_err_message->Format("Syntex Error (calculate_command_list_with_macro2)");
		return	false;
	}


			

	if (start->next->next->type == COM_TYPE_PARENT &&	//�}�N���̎��̎��̃R�}���h���u)�v�ł���
		start->next->next->pare == COM_PARENT_CLOSE)
	{
		//�}�N���������Ȃ��ꍇ

		//�}�N����`�Ɉ����Ȃ��̂��̂����邩�T��
		bool varidArgsFlg = false;
		for (overloadID = 0; overloadID < pMacroDefinision[start->mac].overloadCnt; overloadID++) {
			if (pMacroDefinision[start->mac].args[overloadID] == 0) {
				varidArgsFlg = true;
				break;
			}
		}
		if (!varidArgsFlg) {//�Ȃ���΃G���[�I��
			p_err_message->Format("Error: invarid argument count for %s", pMacroDefinision[start->mac].name);
			return false;
		}

		//�}�N�����s
		if( !execute_macro(id, p_calc_param, &result_val, start->mac, overloadID, arg_val,  p_err_message, autoDrawImage) ){
			return false;
		}

		//�㏈��
		//�}�N���̌v�Z�ɗp�������ߌ���폜���Čv�Z���ʂ�}������
		start->type = result_val.type;
		if(result_val.type == COM_TYPE_NUMBER){
			start->number = result_val.number;
		}else{
			start->str_id = result_val.str_id;
		}

		start->next = start->next->next->next;
		if (start->next != NULL) {//���̖��ߌꂪ����΃A�h���X��n��
			start->next->prev = start;
		}
	}
	else
	{
		//�}�N������������ꍇ
		

		//�����ƂȂ閽�ߌ��T��

		COMMAND	*p_arg_com[CALC_MACRO_ARGS_MAX] = { 0 };//���ߌ�̐擪�A�h���X�̊i�[��

		p_arg_com[arg_counter++] = start->next;//�ŏ��̈����ƂȂ閽�ߌ�̐擪�A�h���X


		int pearNestCnt = 0;//�u()�v�̃l�X�g��

		//2�Ԗڈȍ~�̈�����T��
		for (command = start->next; command != NULL; command = command->next)
		{

			if (command->type == COM_TYPE_COMMA)
			{//�u,�v����������

				if (arg_counter + 1 >= CALC_MACRO_ARGS_MAX) {//�����̐�����������
					p_err_message->Format("Syntax Error (calculate_command_list_with_macro3)");
					return false;
				}

				//if (pearNestCnt>1)
				//{//�u()�v�̃l�X�g�ʒu����������
				//	p_err_message->Format("Syntax Error () nest?");
				//	return false;
				//}

				//�u,�v���R�}���h����폜���Ă��̈ʒu�Ɂu ) �v�Ɓu ( �v��}������
				// macro( a , b ) �� macro( a )( b )
				command->type = COM_TYPE_PARENT;
				command->pare = COM_PARENT_CLOSE;
				additonal_com[*additional_com_id].type = COM_TYPE_PARENT;
				additonal_com[*additional_com_id].pare = COM_PARENT_OPEN;
				additonal_com[*additional_com_id].prev = command;
				additonal_com[*additional_com_id].next = command->next;
				command->next = &additonal_com[*additional_com_id];
				(*additional_com_id)++;

				p_arg_com[arg_counter++] = command->next;//�����ƂȂ閽�ߌ�̐擪�A�h���X
					
				if (pearNestCnt > 0) {//���̖��ߌ�A�h���X�́A���̏ꏊ�ɐV�����������u(�v�Ȃ̂ŁA�l�X�g�̐��𒲐�����
					pearNestCnt--;
				}
			}
			else if (command->type == COM_TYPE_MACRO)
			{//�}�N���̈������ɂ���Ƀ}�N��������ꍇ�͐[���D��ŏ�������

				if (!calculate_command_list_with_macro(id, p_calc_param, command, additonal_com, additional_com_id, p_err_message, autoDrawImage))	return false;
			}
			else if (command->type == COM_TYPE_PARENT )
			{
				if (command->pare == COM_PARENT_OPEN)
				{
					pearNestCnt++;//�u()�v�̃l�X�g�񐔂�1���₷
				}
				else{
					if (pearNestCnt > 1){//�u()�v�̃l�X�g���[���ꍇ
						pearNestCnt--;//�u()�v�̃l�X�g�񐔂�1���炷
					}
					else{
						//���̃}�N���ƑΉ�����u)�v���݂�����������T���I��
						break;
					}

				}
			}
		}


		//�}�N���̈����ƂȂ閽�ߌ���ɂ��ׂĎ��s����

		for (int c = 0; c < arg_counter; c++) {
			if (!calculate_command_list(id, p_calc_param, p_arg_com[c], additonal_com, additional_com_id, p_err_message, autoDrawImage))	return	false;
		}


		//���̒i�K�ł́@[�}�N��][���l/������][���l/������]...�̏��@�ɕ���ł���͂��ł���


		//�����̕��т����������`�F�b�N����
		bool varidArgsFlg = false;
		COMMAND *pLastCommand;

		for (overloadID = 0; overloadID < pMacroDefinision[start->mac].overloadCnt; overloadID++)//���ׂẴI�[�o�[���[�h�ɑ΂��ă`�F�b�N
		{
			if (pMacroDefinision[start->mac].args[overloadID] == arg_counter)
			{
				bool invaridArgCorderFlg = true;

				COMMAND *com = start->next;
				for (int c = 0; c < arg_counter; c++, com = com->next)
				{
					if (com->type == pMacroDefinision[start->mac].arg_type[overloadID][c])//�����̎�ށi���l/������j����������
					{
						if (com->type == COM_TYPE_NUMBER)
						{//���l����
							arg_val[c].type = COM_TYPE_NUMBER;
							arg_val[c].number = com->number;
						}
						else {//���������
							arg_val[c].type = COM_TYPE_STRING;
							arg_val[c].str_id = com->str_id;
						}
					}
					else
					{
						invaridArgCorderFlg = false;
						break;
					}
				}

				if (invaridArgCorderFlg) //������
				{
					pLastCommand = com;
					varidArgsFlg = true;
					break;
				}
			}
		}

		if (!varidArgsFlg) {//�Ȃ���΃G���[�I��
			p_err_message->Format("Error: invarid argument for %s", pMacroDefinision[start->mac].name);
			return false;
		}

		//�}�N�����s
		if (!execute_macro(id, p_calc_param, &result_val, start->mac, overloadID, arg_val, p_err_message, autoDrawImage))	return false;


		//�㏈��
		//�}�N���̌v�Z�ɗp�������ߌ���폜���Čv�Z���ʂ�}������
		start->type = result_val.type;
		if (result_val.type == COM_TYPE_NUMBER) {
			start->number = result_val.number;
		}
		else {
			start->str_id = result_val.str_id;
		}

		start->next = pLastCommand;
		if (start->next != NULL) {//���̖��ߌꂪ����΃A�h���X��n��
			start->next->prev = start;
		}
	}

	return true;
}




/********************************************************************
�@  �\  ��  �� : �摜�������̎Q��
��    ��    �� : refer_image
��          �� :
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::refer_image( 
						   double	*p_result,		//(out)�Q�ƌ���
						   int		image_no,		//(in)�摜�������ԍ�
						   double	x,				//(in)�摜���W
						   double	y,				//(in)�摜���W
						   int		page,			//(int)
						   int		channel,		//(int)
						   CString	*p_err_message,	//(out)�G���[���b�Z�[�W
						   bool		to_mask			//(in)�}�X�N�Q�Ƃ��邩�ǂ���
						   )
{
	CDataUnit		*pDU;
	CString			str;
	CSize			size;
							

	if( 0<=image_no  &&  image_no < API.GetDataUnitNumber() )
	{
		if( API.GetDataUnit( (int)image_no ) == NULL){
			p_err_message->Format("Error: image%d do not have data",image_no);
			return false;
		}
	}
	else
	{
		p_err_message->Format("Error: image id must be within 0 �` %d ", API.GetDataUnitNumber() -1);
		return false;
	}


	pDU = API.GetDataUnit( image_no);
	size = pDU->DataSize;

	if(to_mask){
		*p_result = (double)*(pDU->pMaskData + (int)x + (int)y * pDU->DataSize.cx);
	}else{
		*p_result  = pDU->GetDataValue( x, y, page ,channel);
	}

	return	true;
}


/********************************************************************
�@  �\  ��  �� : �R�}���h���X�g�̃`�F�b�N
��    ��    �� : refer_image
��          �� :
��    ��    �l : �`�F�b�N���ʂ��true
�@          �\ : (��)�̐����������`�F�b�N
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CCalculate::check_parenthesis(
								COMMAND		*com,			//(in)�R�}���h���X�g�擪�|�C���^
								CString		*p_err_message	//(out)�G���[���b�Z�[�W
								)
{
	COMMAND		*command;
	int			start_num, end_num;
	start_num = 0;	// �u ( �v �̐�
	end_num = 0;	// �u ) �v �̐�

	for(command = com ; command != NULL ; command = command->next){
		if( command->type == 2 ){
			if( command->pare == 0 )	start_num++;
			else						end_num++;
		}
	}

	if( start_num != end_num ){
		p_err_message->Format("Error: count ( and ) must be same");
		return false;

	}else{
		return true;
	}
}
