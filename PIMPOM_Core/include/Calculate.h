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
#pragma once
#include "global_define.h"
#include "global_typedef.h"

///*************************************
//         �}  �N  ��   ��   �`         
///*************************************

#define		CALC_MACRO_CNT_MAX			(400)	//�}�N���̍ő吔
#define		CALC_MACRO_NAME_LEN_MAX		(256)	//�}�N�����̍ő咷
#define		CALC_MACRO_ARGS_MAX			(15)	//�}�N�������̍ő吔
#define		CALC_MACRO_OVERLOAD_MAX		(10)		//�}�N���ϐ����I�[�o�[���[�h�ł��鐔
#define		CALC_COMMAND_LIST_MAX		(150)	//�R�}���h���X�g�̍ő�v�f��
#define		CALC_STRING_BUFFER_LEN		(30)	//1�s�̃R�}���h������ň����镶����^�f�[�^�̍ő吔
#define		CALC_STRING_LEN_MAX			(1024)	//������̍ő咷
#define		CALC_COMMAND_LINE_MAX		(5000)	//�R�}���h�s���̍ő�
#define		CALC_VARIABLE_NAME_MAX		(256)	//�ϐ����̍ő吔
#define		CALC_VARIABLE_BUFFER_LEN	(150)	//�ϐ��ő吔
#define		CALC_FILE_FINDER_MAX		(3)	//�t�@�C���񋓂̍ő吔


//�R�}���h���X�g�̗v�f�̎��
#define		COM_TYPE_NUMBER			0	//���l
#define		COM_TYPE_OPERATOR		1	//���Z��
#define		COM_TYPE_PARENT			2	//����
#define		COM_TYPE_MACRO			3	//�}�N��
#define		COM_TYPE_COMMA			4	//�J���}
#define		COM_TYPE_COOR_X			5	//x���W
#define		COM_TYPE_COOR_Y			6	//y���W
#define		COM_TYPE_STRING			7	//������
#define		COM_TYPE_CTRL			8	//�R�}���h���s���䕶
#define		COM_TYPE_VARIABLE		9	//�ϐ�

#define		COM_PARENT_OPEN			0	//(	
#define		COM_PARENT_CLOSE		1	//)	



//�R�}���h���s����R�[�h
#define		CALC_CTRL_BREAK					1000	//break
#define		CALC_CTRL_GOTO					1001	//goto()
#define		CALC_CTRL_IF					1002	//if()
#define		CALC_CTRL_ELSE					1003	//else
#define		CALC_CTRL_ENDIF					1004	//endif
#define		CALC_CTRL_CLEAR					1005	//clear
#define		CALC_CTRL_WHILE					1006	//while()
#define		CALC_CTRL_ENDWHILE				1007	//endwhile
#define		CALC_CTRL_ELSEIF				1008	//elseif
#define		CALC_CTRL_FOR					1009	//for
#define		CALC_CTRL_ENDFOR				1010	//endfor

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

//�}�N����`�̍\����
typedef struct MACRO_DEF_STRUCT_{
	short	overloadCnt;	//�I�[�o�[���[�h��
	char	name[CALC_MACRO_NAME_LEN_MAX];		//�}�N����
	unsigned char	args[CALC_MACRO_OVERLOAD_MAX];								//�����̐�
	unsigned char	arg_type[CALC_MACRO_OVERLOAD_MAX][CALC_MACRO_ARGS_MAX];		//�����̎��
	int		ret_type;//�߂�l�̌^�i�}�N���ɂ���Ă͎��s���ɕύX����ꍇ����j
}MACRO_DEF_STRUCT;


//�}�N���̈���,�߂�l�̍\����
typedef struct MACRO_IO_STRUCT_{
	short	type;	//�����̎�� 0:���l  7:������
	double  number;	//���l����
	short	str_id;	//���������(������o�b�t�@��ID)
}MACRO_IO_STRUCT;




//�R�}���h���X�g�̍\����
typedef struct COMMAND{
	COMMAND		*prev;//���X�g�̈�O�̃A�h���X
	COMMAND		*next;//���X�g�̈��̃A�h���X

	//�R�}���h�̎��
	short		type;	//0: �萔  1:�I�y���[�^  2:����   3:�}�N��		4:,(�}�N���ƂƂ��Ɏg��)	
						//5:i or x	6:j or y	7:������	8:����\���@9:�ϐ�
	//���g
	double		number;	//type �� 0 �̂Ƃ����l
	short		ope;	//type �� 1 �̂Ƃ�		0:+   1:-   2:*   3:/  4:==  5:<  6:>  7:<= 8:>= 9:!=�@10: = 11:&&  12: ||  13:+=  14:-=  15:*=  16/=
	short		pare;	//type �� 2 �̂Ƃ�     0:(   1:)
	short		mac;	//type �� 3 �̂Ƃ� �}�N��ID
	short		str_id;	//type��7�̂Ƃ�������o�b�t�@��ID
	short		var_id; //type��9�̂Ƃ��̕ϐ��o�b�t�@��ID
}COMMAND;


//������o�b�t�@�̍\����
typedef struct CALC_STRING_BUFFER_STRUCT_{
	char	strbuffer[CALC_STRING_BUFFER_LEN][CALC_STRING_LEN_MAX+1];//CALC_STRING_BUFFER_LEN�̕�����̃o�b�t�@
	short	count;//������o�b�t�@�����܂Ŏg�p���Ă��邩
	short	staticCount;//�Œ蕶����(�R�}���h���C�����""�ň͂܂�����)�����ŕ�����o�b�t�@�����܂Ŏg�p���Ă��邩
}CALC_STRING_BUFFER_STRUCT;



//�R�}���h���C������֘A�̍\����
typedef struct CALC_CTRL_DATA_{
	short				controlCode;						//�R�}���h���C������R�[�h
	short				branchNestCount;					//if�`endif/while�`endwhile�܂ł̃l�X�g
	int					branchToLine[2];					//if/else/while�̏ꍇ�Ɏ��ɃW�����v����s�ԍ�
	int					branchFrmLink;						//else/endif/endwhile����if/while�ւ̋t�����N
}CALC_CTRL_DATA;


//�v�Z�̓��̓p�����^�̍\����
typedef struct CALC_PARAM_STRUCT_{
	int					x;//�摜���Wx
	int					y;//�摜���Wy
	int					rgbType;//RGB�̂ǂ��ɓ���邩
	bool				apiMode;//API�̃}�N�������s���邩
	BATCH_INPUT			*pbatchIn;
	BATCH_OUTPUT		*pbatchOut;
}CALC_PARAM_STRUCT;



//�v�Z���ʂ̍\����
typedef struct CALC_RESULT_STRUCT_{
	short	type;//�^�C�v(���l or ������)
	double	number;//���l
	char	str[CALC_STRING_LEN_MAX];//������
	DWORD	elapsedTime;//�o�ߎ���

}CALC_RESULT_STRUCT;



//�P�s�̃R�}���h���s�ɕK�v�ȑS�f�[�^���܂Ƃ߂��\����
typedef struct CALC_DATA_STRUCT_{
	char						commandString[CALC_STRING_LEN_MAX];	//�R�}���h������
	CALC_STRING_BUFFER_STRUCT	*pStrignBuffer;						//������o�b�t�@�̐擪�|�C���^
	COMMAND						*pCommandList;						//�R�}���h���X�g�̐擪�|�C���^
	CALC_CTRL_DATA				ctrlData;
	CALC_RESULT_STRUCT			result;								//�v������
	char						errorMessage[CALC_STRING_LEN_MAX];	//�G���[���b�Z�[�W
}CALC_DATA_STRUCT;



//�ϐ��̍\����
typedef struct CALC_VARIABLE_BUFFER_STRUCT_{
	char				name[CALC_VARIABLE_NAME_MAX];//�ϐ���
	CALC_RESULT_STRUCT	data;//�ϐ��̒��g
}CALC_VARIABLE_STRUCT;



///*************************************
//         �N���X   ��   �`         
///*************************************
class PIMPOM_EXT CCalculate
{
public:
	CCalculate();
	virtual ~CCalculate();

	void	ClearAll();//���ׂẴR�}���h���폜����
	bool	RegisterCommandString(CString com_str[], int com_str_cnt, CString *p_err_message);//�R�}���h�����̉��߂Ɠo�^
	bool	ExecuteAPI(int id, CString *p_err_message, int *pNextID = NULL, BATCH_INPUT *p_batch_in = NULL, BATCH_OUTPUT *p_batch_out = NULL, bool autoDrawImage = true, bool *pBlockMap = NULL);//���s
	bool	Execute(int id, int i, int j, CString *p_err_message, int rgb = -1, bool ApiMode = false, int *pNextID = NULL, BATCH_INPUT *p_batch_in = NULL, BATCH_OUTPUT *p_batch_out = NULL, bool autoDrawImage = true, bool *pBlockMap = NULL);//�摜���W���ƂɎ��s
	int		GetResult(int id, double *number, char str[], CString *p_err_message,  DWORD *p_elapsed_time=NULL);//���s���ʎ擾
	
	bool	FindCtrlCode(CString str, int index ,int *p_ctrl_id, int *pIndexIncliment);//�����񂩂�R�}���h���s����\����T��
	bool	IsMacroName(CString str, int index, int *p_macro_id, int *pIndexIncliment);//�����񂪃}�N�������ǂ������f
	bool	IsParenthesis( CString str , int index , int *p_pare);//�����񂪊��ʂ��ǂ������f
	bool	IsOperator(CString comstr, int index, int *p_ope , int *pIndexIncliment);//�����񂪉��Z�q���ǂ������f
	bool	IsNumberValue(CString comstr, int index , double *p_number,int *pIndexIncliment);//�����񂪐��l���ǂ������f
	bool	IsStringValue(CString comstr, int index , CString *p_str ,int *pIndexIncliment);//������""�ň͂܂�Ă��邩�ǂ������f
	bool	IsDefinedVariable(CString comstr, int index ,int *p_var_id, int *pIndexIncliment);//�����񂪐錾�ςݕϐ����ǂ������f
	bool	IsComment(CString	comstr,	int index);//�����񂪃R�����g���ǂ������f
	int		GetCommandLineLen();//�o�^����Ă���R�}���h���C������Ԃ�

	static bool	ExtendSubCommands(int sub_com_num, CString sub_com_str[], CString sub_com_id[], CString *p_com_str, CString *p_err_message, bool *sub_com_enable=NULL);

	
	bool	GetMacroDefinision(int id, MACRO_DEF_STRUCT *pMcr);//�}�N���̒�`��Ԃ�
	int		FindVariableDefinission(CString	comstr,	CString	*p_err_message);//�ϐ��錾��T��
	bool	GetVariable(int id, CALC_RESULT_STRUCT *pvar, char varname[]);//�ϐ����擾����
	bool	SetVariable(int id, CALC_RESULT_STRUCT *pvar);//�ϐ���ݒ肷��



private:
	void debug_out(COMMAND	*start);
	void debug_out_ln(COMMAND	*start);

	//�}�N����`
	bool	define_macro( int ret_type, int id, char name[], 
						  int argtype0=-1, int argtype1=-1, int argtype2=-1, int argtype3=-1, int argtype4=-1, 
						  int argtype5=-1, int argtype6=-1, int argtype7=-1, int argtype8=-1, int argtype9=-1, 
						  int argtype10=-1, int argtype11=-1,int argtype12=-1, int argtype13=-1, int argtype14=-1);
	void	define_macros();

	//�R�}���h������̉��
	int		command_string_to_command_list(int id, CString  comstr, COMMAND commands[], CString *p_err_message, int *p_ctrl_code);//�R�}���h���������͂��ăR�}���h���X�g�����							 
	bool	check_parenthesis(COMMAND *com, CString *p_err_message);
	bool	set_branch_property(int com_str_cnt, CString *p_err_message)	;


	//�R�}���h���X�g�̎��s
	bool	calculate_for_command_list(int id, int mode, CALC_PARAM_STRUCT	*p_param, COMMAND  *com, COMMAND *additonal_com, int *additional_com_id, CString *p_err_message, bool autoDrawImage);//for���̎��s
	bool	calculate_command_list(int id,  CALC_PARAM_STRUCT	*p_param, COMMAND  *com, COMMAND *additonal_com,int *additional_com_id, CString *p_err_message, bool autoDrawImage);//�R�}���h���X�g�̎��s
	bool	calculate_command_list_with_macro(int id, CALC_PARAM_STRUCT	*p_param, COMMAND *com, COMMAND *additonal_com,int *additional_com_id, CString *p_err_message, bool autoDrawImage);//�}�N���֘A�̃R�}���h���X�g�̎��s
	bool	refer_image(double *p_result, int image_no, double x, double y, int page, int channel, CString *p_err_message, bool to_mask=false);//�摜�Q��
	bool	execute_macro(int id, CALC_PARAM_STRUCT	*p_calc_param, MACRO_IO_STRUCT *p_result, int macro_no, int overwrideID, MACRO_IO_STRUCT params[], CString *p_err_message, bool autoDrawImage);//�}�N���̎��s�i������CalculateMacro.cpp�j
	bool	set_next_id(int id, CALC_CTRL_DATA ctrlData, COMMAND *com, CString *p_err_message, int *pNextID, bool *blockMap);


	//�f�[�^�Ǘ�
	bool	init_calc_structure(int id, CString *p_err_message);//�f�[�^������������
	void	deinit_calc_structure(int id);//���������
	int		push_string_to_buffer(int id, CString str, CString *p_err_message, bool isStatic=false);//������o�b�t�@�ɕ�������P�}��
	char	*get_string_from_buffer(int id, int str_id);//������o�b�t�@���當������擾
	void	delete_non_static_string_buffer(int id);//������o�b�t�@����Œ蕶���łȂ����̂��폜
	bool	set_command_list(int id, CString commandStr, COMMAND *pNewCommandCodeList, int ctrl_code);//�R�}���h���X�g��ݒ�
	COMMAND *get_command_list(int id, CALC_CTRL_DATA* p_ctrl_code);//�R�}���h���X�g�̐擪�|�C���^�擾
	int		get_command_listLen(COMMAND *command);//�R�}���h���X�g�̒����擾
	void    copy_command_list(COMMAND *dst, COMMAND *src, int size);//�R�}���h���X�g���R�s�[
	bool	set_result(int id, COMMAND *com, DWORD elapsed_time);//�v�����ʂ�ݒ�
	void	set_error_message(int id, CString err );
	int		new_variable( CString str, CString *p_err_message);//�V�ϐ�
	void	clear_variable();//�ϐ��錾���ׂč폜



	MACRO_DEF_STRUCT		*pMacroDefinision;//�}�N���̒�`�̔z��擪�|�C���^
	
	CALC_DATA_STRUCT		*pCalcDataStructMap[CALC_COMMAND_LINE_MAX];//�R�}���h���s�ɕK�v�ȃf�[�^�̐擪�|�C���^�}�b�v

	CALC_VARIABLE_STRUCT	*pCalcVariableMap[CALC_VARIABLE_BUFFER_LEN];//�ϐ��̐擪�|�C���^�}�b�v

	CFileFind	*pFileFinder[CALC_FILE_FINDER_MAX];//�t�@�C���ꗗ�쐬�̂��߂̃t�@�C���_
};