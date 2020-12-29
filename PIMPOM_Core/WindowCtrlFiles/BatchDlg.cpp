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
//CBatchDlg
//
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "BatchDlg.h"
#include "pimpom_plot.h"
#include <process.h>

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#define EXEC_MODE_WAIT		(0)
#define EXEC_MODE_AUTO_EXEC		(1)
#define EXEC_MODE_SINGLE_EXEC		(2)

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


unsigned __stdcall BatchThread(void* pArg)
{
	CBatchDlg* pDlg = (CBatchDlg*)pArg;

	pDlg->BackGroundProc();

	return 0;
}



/********************************************************************
�@  �\  ��  �� : ������𕪊�����
��    ��    �� : split
��          �� : CString		str		(in)�����Ώە���
                 CString		delim	(in)�f���~�^
				 CStringArray	&result	(out)�������ʕ����z��
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
static void split(CString str, CString delim, CStringArray &result)
{
  result.RemoveAll();
 
    int indexback = 0;
 
  CString st;
 
  int i;
  for(i = 0; i < str.GetLength(); i++)
  {
    if (str.GetAt(i) == delim.GetAt(0))
    {
      if ((i - indexback) == 0)
      {
        result.Add("");
      }
      else
      {
        st = str.Mid(indexback, i - indexback);
        result.Add(st);
      }
      indexback = i + 1;
    }
  }
 
  if ((i - indexback) != 0)
  {
    st = str.Mid(indexback, i - indexback);
    result.Add(st);
  }
 
}


//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CBatchDlg, CDialog)

CBatchDlg::CBatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchDlg::IDD, pParent)
{
	hThread = NULL;
}

CBatchDlg::~CBatchDlg()
{
	if(hThread)
		TerminateThread(hThread,0);
}

void CBatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BATCH, m_list_contol);
	DDX_Control(pDX, IDC_PROGRESS_BATCH, m_progress);
}




/********************************************************************
�@  �\  ��  �� : �P��o�b�`���s����
��    ��    �� : single_execute
��          �� : bool auto_exec_flg (in)���s��A���X�g���̗v�f��I������悤�ɂ���
��    ��    �l : auto_exec_flg=true�̂Ƃ��́A���X�g�̍Ō�܂œ��B������true��Ԃ�.����ȊO��false��Ԃ�
                 �o�b�`���s�̃X�e�[�^�X�����g���C��v��������false��Ԃ�
				 �o�b�`���s�̃X�e�[�^�X�������I����v��������false��Ԃ�
�@          �\ : ���X�g�Ō��ݑI�𒆂̗v�f�i�摜�A���̓p�����^�j�ɂĂP��o�b�`���s���s��
                 ���X�g�v�f���I������Ă��Ȃ���΍ŏ��̗v�f�Ŏ��s����

				 �o�b�`���s�Ƃ́A�J����Ă���J�X�^���_�C�A���O�̃o�b�`���s�C�x���g�n���h�����Ăяo�����Ƃł���
				 �o�b�`���s�C�x���g�n���h���͏o�͒l����уX�e�[�^�X��Ԃ����Ƃ��ł���B 
				 �����̃J�X�^���_�C�A���O���J����Ă���΁A�e�_�C�A���O�̃o�b�`���s�C�x���g�n���h�����������s����
�@�@�@�@�@�@�@�@ �����̃J�X�^���_�C�A���O����o�Ă����o�͒l�́A�Ȃ����킹�ĕ\�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CBatchDlg::single_execute(bool auto_exec_flg)
{
	int file_no=0;
	bool retryState=false;
	bool stopState=false;
	BATCH_INPUT		in_param = { 0 };//���̓p�����^
	BATCH_OUTPUT	out_param = { 0 };//�o�̓p�����^�F�@�����̃J�X�^���_�C�A���O������ꍇ�́A�o�͒l�𓝍�


	//���̓p�����^���Z�b�g����
	in_param.AutoExecFlg = auto_exec_flg;
	GetBatchInput(&in_param, &file_no);


	//�o�b�`���s
	if (API.BatchExecute(&in_param, &out_param))
	{
		//�o�̓p�����^�����X�g�ɕ\������
		set_list_item(file_no, &in_param, &out_param);

		if (out_param.state == BATCH_EXEC_STATE_STOP)//���f�v��
		{
			return true;
		}
		else if (out_param.state == BATCH_EXEC_STATE_RETRY)//���g���C�v��
		{
			return false;
		}
	}



	if(auto_exec_flg )//��Ď��s��
	{
		int next_num = file_no + 1;
		if(m_list_contol.GetItemCount()<=next_num)//���X�g�̍Ō�܂œ��B����
		{
			select_list_item(0);//���X�g�I��ԍ����ŏ��ɖ߂�
			return true;
		}
		else//���X�g�̍Ō�܂œ��B���Ă��Ȃ�
		{
			select_list_item(next_num);//���X�g�I��ԍ����ЂƂi�߂�
			return false;
		}
	}
	else//�P�����s��
	{
		return true;
	}
}

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�A�C�e���̕\����ԕύX
��    ��    �� : show_dlg_item
��          �� : int executing_mode (in)�A�����s�����ǂ���   0:�ҋ@��� 1:�A�����s��  2:�����s��
��    ��    �l : 
�@          �\ : �A�����s�����ǂ����ŕ\����Ԃ�ς���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CBatchDlg::show_dlg_item(int  executing_mode)
{
	if(executing_mode== EXEC_MODE_AUTO_EXEC){
		API.SetCheck(m_hWnd,IDC_CHECK_BATCH_EXEC,true);//�A�����s�{�^���̃`�F�b�N������
		::SetWindowText(::GetDlgItem(m_hWnd,IDC_CHECK_BATCH_EXEC), CString("���f"));
	}else {
		API.SetCheck(m_hWnd,IDC_CHECK_BATCH_EXEC,false);//�A�����s�{�^���̃`�F�b�N���͂���
		::SetWindowText(::GetDlgItem(m_hWnd,IDC_CHECK_BATCH_EXEC), CString("��Ď��s"));
	}


	::EnableWindow(::GetDlgItem(m_hWnd,IDC_LIST_BATCH), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_BATCH_SRC_IMAGE_NO), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_OPEN), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_DELETE), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_CLEAR), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE_PREV), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE_NEXT), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_OUT), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_FILE_IN), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDCANCEL), !executing_mode);
	::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_BATCH_LOG_FILE_NAME), !executing_mode);

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
void CBatchDlg::init_list()
{
    LVCOLUMN    lvc;
    char       caption[32];

	m_list_contol.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O

	//ID
	sprintf(caption, "ID");
    lvc.iSubItem    = 0;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 40;          // ����
    m_list_contol.InsertColumn(0, &lvc);	

	//�t�@�C����
	sprintf(caption, "�摜�t�@�C��");
    lvc.iSubItem    = 1;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 500;          // ����
    m_list_contol.InsertColumn(1, &lvc);

	//���̓p�����^
    for (int i = 0; i < BATCH_IN_PARAM_NUM ; i++)
    {
		sprintf(caption, "IN%d",i);
        lvc.iSubItem    = i + 2;            // �T�u�A�C�e���ԍ�
        lvc.pszText     = caption;   // ���o���e�L�X�g
        lvc.cx          = 30;          // ����
        m_list_contol.InsertColumn(i + 2, &lvc);
    }

	//�o�̓p�����^
    for (int i = 0; i < BATCH_OUT_PARAM_NUM; i++)
    {
		sprintf(caption, "OUT%d",i);
        lvc.iSubItem    = i + 2 + BATCH_IN_PARAM_NUM;            // �T�u�A�C�e���ԍ�
        lvc.pszText     = caption;   // ���o���e�L�X�g
        lvc.cx          = 30;          // ����
        m_list_contol.InsertColumn(i + 2 + BATCH_IN_PARAM_NUM, &lvc);
    }
}

/********************************************************************
�@  �\  ��  �� : ���X�g�ɗv�f�ǉ�
��    ��    �� : add_list_item
��          �� : BATCH_INPUT *in_param	  (in)�o�b�`���s�̓��̓p�����^
				 BATCH_OUTPUT *out_param  (in)�o�b�`���s�̏o�̓p�����^
                 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::add_list_item(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param )
{
	LVITEM       lvi={0};
	int			index;
	char		str[1024]={0};
	
	lvi.mask = LVIF_TEXT;

	//ID
	lvi.iItem = m_list_contol.GetItemCount();
	lvi.iSubItem = 0;
	sprintf_s(str, 1024, "%05d", lvi.iItem);
	lvi.pszText = str;
	index = m_list_contol.InsertItem(&lvi);
	

	//�摜�t�@�C����
	//lvi.iItem = n;
	lvi.iSubItem = 1;
	lvi.pszText = in_param->imae_file_path;
	m_list_contol.SetItem(&lvi);

	//�p�����^
	set_list_item(index, in_param, out_param);
}

/********************************************************************
�@  �\  ��  �� : ���X�g�̒l�ύX
��    ��    �� : set_list_item
��          �� : int n (in)���X�g�̗v�f�ԍ�
				 BATCH_INPUT *in_param	  (in)�o�b�`���s�̓��̓p�����^
				 BATCH_OUTPUT *out_param  (in)�o�b�`���s�̏o�̓p�����^
                 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::set_list_item(int n, BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	LVITEM       lvi={0};
	lvi.mask = LVIF_TEXT;


	//���̓p�����^
    for (int i = 0; i < BATCH_IN_PARAM_NUM; i++)
    {
		lvi.iItem = n;
		lvi.iSubItem = i+2;
		lvi.pszText = in_param->param[i];
		m_list_contol.SetItem(&lvi);
	}

	//�o�̓p�����^
	if(out_param!=NULL){
		for (int i = 0; i < BATCH_OUT_PARAM_NUM; i++)
		{
			lvi.iItem = n;
			lvi.iSubItem = i+2+BATCH_IN_PARAM_NUM;
			
			lvi.pszText = out_param->result[i];
			m_list_contol.SetItem(&lvi);
		}
	}
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
int	CBatchDlg::get_selected_list_item()
{
	return m_list_contol.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
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
void CBatchDlg::select_list_item(int n)
{
	m_list_contol.SetItemState(n, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_list_contol.SetSelectionMark(n);
	m_list_contol.EnsureVisible(n,true);
}

/********************************************************************
�@  �\  ��  �� : csv�t�@�C��������o�̓p�����^�ꗗ��ǂݍ���
��    ��    �� : load_image_list
��          �� : CString filename (in)csv�t�@�C���̃p�X
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CBatchDlg::load_image_list(CString filename, char splitter)
{
	bool	ret=true;
	FILE *fp = fopen(filename,"rt");
	if(fp){
		for(;;){
			char buffer[5000]={0};
			if(fgets(buffer, 5000, fp)!=NULL)//txt�t�@�C������P�s�ǂݎ��
			{
				BATCH_INPUT in_param={0};
				BATCH_OUTPUT out_param={0};
				CStringArray params;

				if(buffer[strlen(buffer)-1]=='\n'){//fgets�œ��͂��ꂽ���s����������
					buffer[strlen(buffer)-1]=0;
				}

				//csv�t�@�C������ǂݎ�����s�𕪊�
				split(CString(buffer), CString(splitter), params);

				//�摜�t�@�C����
				{
					CString str = params.GetAt(0);
					strcpy(in_param.imae_file_path, str.GetBuffer());
					str.ReleaseBuffer();
				}

				//���̓p�����^
				for(int n=1 ; n<params.GetCount() && n-1<BATCH_IN_PARAM_NUM ; n++){
					CString str = params.GetAt(n);
					strcpy(in_param.param[n-1], str.GetBuffer());
					str.ReleaseBuffer();
				}

				//�o�̓p�����^
				for(int n=1+BATCH_IN_PARAM_NUM ; n<params.GetCount() && n-1-BATCH_IN_PARAM_NUM<BATCH_OUT_PARAM_NUM ; n++){
					CString str = params.GetAt(n);
					strcpy(out_param.result[n-1-BATCH_IN_PARAM_NUM], str.GetBuffer());
					str.ReleaseBuffer();
				}

				add_list_item(&in_param,&out_param);//���X�g�ɒǉ�
			}else{
				ret = false;
				break;
			}
		}
		fclose(fp);
	}	

	return ret;
}

/********************************************************************
�@  �\  ��  �� : ���X�g�v�f�\�[�g�̂��߂̔�r�֐�
��    ��    �� : compare_func
��          �� : 
��    ��    �l : 
�@          �\ : ��r�Ώۂ������Ƃ����l�Ȃ�΁A�傫���Ŕ�r
�@�@�@�@�@�@�@�@����ȊO�̏ꍇ�͕�����̏��Ŕ�r�@�@�@�@�@
		 ���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CALLBACK CBatchDlg::compare_func(LPARAM param1, LPARAM param2, LPARAM param3)
{
	CBatchDlg* pDlg = (CBatchDlg*)param3;
	int nSubItem  = pDlg->m_iSubItem;
	CString    strItem1 = pDlg->m_list_contol.GetItemText(param1, nSubItem);
	CString    strItem2 = pDlg->m_list_contol.GetItemText(param2, nSubItem);

	double numItem1 = atof(strItem1);
	double numItem2 = atof(strItem2);
	if(numItem1!=0.0 && numItem2!=0.0)//���l�ǂ����̔�r
	{
		if(pDlg->m_sort_dir_flg)	
			return (numItem1>numItem2);
		else
			return (numItem1<numItem2);
	}
	else//������ǂ����̔�r
	{
		if(pDlg->m_sort_dir_flg)
			return strcmp(strItem1, strItem2);
		else
			return strcmp(strItem2, strItem1);
	}
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
void CBatchDlg::arrange_dlg_item()
{
	CRect clientRct={0}, listRct = { 0 };

	::GetClientRect(m_hWnd, &clientRct);//�N���C�A���g�̈��\����`���擾

	
	API.GetDialogItemRect(m_hWnd, IDC_LIST_BATCH, &listRct);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_LIST_BATCH)  , listRct.left, listRct.top, clientRct.right-clientRct.left-10, clientRct.bottom-clientRct.top-120, true);//���X�g�R���g���[��
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_STATIC_BATCH),    0, 0, clientRct.right-clientRct.left-2, clientRct.bottom-clientRct.top-60, true);//��ٰ�߃{�b�N�X

	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_STATIC_BATCH_LOGFILEPATH) , 5   , clientRct.bottom-clientRct.top-60 , 70, 20, true);//�u���[�g�p�X�v�L���v�V����
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_EDIT_BATCH_LOG_FILE_NAME) , 90 , clientRct.bottom-clientRct.top-60 , 350, 20, true);//���[�g�p�X�������ި���ޯ��
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_BATCH_ROOT), 440, clientRct.bottom - clientRct.top - 60, 20, 20, true);//���[�g�p�X�u���E�Y�{�^��
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_CHANGE_EXECUTE_RULE), 480, clientRct.bottom - clientRct.top - 60, 60, 20, true);//�u���s���[���v�{�^��
	
	



	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE)     , 5  , clientRct.bottom-clientRct.top-30, 70, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE_PREV), 75 , clientRct.bottom-clientRct.top-30, 30, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_BUTTON_BATCH_EXEC_SINGLE_NEXT), 105, clientRct.bottom-clientRct.top-30, 30, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_CHECK_BATCH_EXEC)             , 135, clientRct.bottom-clientRct.top-30, 70, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_CHECK_BATCH_EXEC)             , 135, clientRct.bottom-clientRct.top-30, 70, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_PROGRESS_BATCH)               , 210, clientRct.bottom-clientRct.top-30, 330, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDC_EDIT_BATCH_PROGRESS)          , 540, clientRct.bottom-clientRct.top-30, 20, 20, true);
	::MoveWindow( ::GetDlgItem(m_hWnd,IDCANCEL)                         , clientRct.right-clientRct.left-80, clientRct.bottom-clientRct.top-30, 70, 20, true);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CBatchDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_OPEN, &CBatchDlg::OnBnClickedButtonBatchFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_DELETE, &CBatchDlg::OnBnClickedButtonBatchFileDelete)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_CLEAR, &CBatchDlg::OnBnClickedButtonBatchFileClear)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_EXEC_SINGLE, &CBatchDlg::OnBnClickedBatchExecSingle)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_BATCH_EXEC_SINGLE_PREV, &CBatchDlg::OnBnClickedBatchExecSinglePrev)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_EXEC_SINGLE_NEXT, &CBatchDlg::OnBnClickedBatchExecSingleNext)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_OUT, &CBatchDlg::OnBnClickedButtonBatchFileOut)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE_IN, &CBatchDlg::OnBnClickedButtonBatchFileIn)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BATCH, &CBatchDlg::OnNMDblclkListBatch)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_BATCH, &CBatchDlg::OnLvnColumnclickListBatch)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BATCH, &CBatchDlg::OnNMRClickListBatch)
	ON_BN_CLICKED(IDC_CHECK_BATCH_EXEC, &CBatchDlg::OnBnClickedCheckBatchExec)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_BATCH_ROOT, &CBatchDlg::OnBnClickedButtonBatchRoot)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_EXECUTE_RULE, &CBatchDlg::OnBnClickedButtonChangeExecuteRule)
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
BOOL CBatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//�h���b�O���h���b�v�Ή�
	DragAcceptFiles();

	//�_�C�A���O�A�C�e���̏�����
    init_list();        // ���X�g�R���g���[��������
	API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_SRC_IMAGE_NO, 0);
	API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_ROOT, "");

	arrange_dlg_item();


	UINT thrID = 0;
	hThread = (HANDLE)::_beginthreadex(NULL, 0, &BatchThread, this, 0, &thrID);


	show_dlg_item(EXEC_MODE_WAIT);//�_�C�A���O�A�C�e���̕\��
	m_sort_dir_flg = true;

	return true;
}

/********************************************************************
�@  �\  ��  �� : ���̃E�B���h�E��\����Ԃɂ���
��    ��    �� : OnShowWindow
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


}

/********************************************************************
�@  �\  ��  �� : �h���b�O�A���h�h���b�v
��    ��    �� : OnDropFiles
��          �� : 
��    ��    �l : 
�@          �\ : �摜�܂���csv���h���b�v���ꂽ��A�N�V����
�@�@�@�@�@�@�@�@�摜�F�摜�t�@�C���̃p�X�����X�g�ɒǉ�
		�@�@�@�@csv�F���̓A�C�e���ꗗ�Ƃ��ēǂݍ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnDropFiles(HDROP hDropInfo)
{
	int		file_num = (int)DragQueryFileA( hDropInfo, -1, NULL, 0);//�h���b�v���ꂽ�t�@�C�������擾

	for(int i=0; i<file_num ; i++)
	{
		UINT size = DragQueryFile( hDropInfo, i, NULL, 0) + 1;//�t�@�C�����̒������擾

		if(size>3)
		{
			CString filename;
			DragQueryFile( hDropInfo, i , filename.GetBuffer(size) ,size );
			filename.ReleaseBuffer();

			if(filename.Right(3).MakeLower()==_T("csv") )//csv�̏ꍇ�̓t�@�C�����ɏ�����Ă���摜�ꗗ��ǂݏo���ă��X�g�ɓ����
			{
				load_image_list(filename);
			}
			else if (filename.Right(3).MakeLower() == _T("txt"))//txt�̏ꍇ�́A��؂蕶����I�����ă��X�g�ɓ����
			{
				CConfirmDlg dlg;
				bool splitterType[] = { true, false , false};
				dlg.SetTitle("��؂蕶��");
				dlg.RegistRadioVar("�J���}(,)", &splitterType[0]);
				dlg.RegistRadioVar("�X�y�[�X", &splitterType[1]);
				dlg.RegistRadioVar("�R����(:)", &splitterType[2]);
				if (dlg.DoModal() == IDOK){
					if (splitterType[0]){
						load_image_list(filename, ',');
					}
					else if (splitterType[1])
					{
						load_image_list(filename, ' ');
					}
					else{
						load_image_list(filename, ':');
					}
				}
			}
			else//����ȊO�̏ꍇ�̓t�@�C���������̂܂܃��X�g�ɓ����
			{
				BATCH_INPUT in_param={0};
				strcpy(in_param.imae_file_path, filename.GetBuffer(size) );
				filename.ReleaseBuffer();
				add_list_item(&in_param);
			}
		}	
	}
	DragFinish(hDropInfo);
}


/********************************************************************
�@  �\  ��  �� : �u��Ď��s�v�{�^������
��    ��    �� : OnBnClickedCheckBatchExec
��          �� : 
��    ��    �l : 
�@          �\ : �A�����s���łȂ���΁A�A�����s���J�n����B
�@�@�@�@�@�@�@�@�@�A�����s���Ȃ�΁A�A�����s�𒆒f����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedCheckBatchExec()
{
	if(m_list_contol.GetItemCount()==0)	return;

	if(API.GetBGProcID()==BGPROC_BATCH_EXC)//��Ď��s���f
	{
		API.StopBGProc();
		show_dlg_item(EXEC_MODE_WAIT);//�_�C�A���O�A�C�e���̕\����ԕύX
		m_progress.SetPos(0);//�v���O���X�o�[���[���ɂ��ǂ�
		API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_PROGRESS,_T(""));
	}
	else if(API.GetBGProcID()==BGPROC_NOTHING)//��Ď��s�J�n
	{
		select_list_item(0);//�ŏ��̉摜�t�@�C����I��
		API.SetBGProcID(BGPROC_BATCH_EXC);
		show_dlg_item(EXEC_MODE_AUTO_EXEC);//�_�C�A���O�A�C�e���̕\����ԕύX
		m_progress.SetRange32(0, m_list_contol.GetItemCount()-1);//�v���O���X�o�[���Z�b�g����
	}
}

/********************************************************************
�@  �\  ��  �� : �u�I�����s�v�{�^������
��    ��    �� : OnBnClickedBatchExecSingle
��          �� : 
��    ��    �l : 
�@          �\ : ���X�g�Ō��ݑI�𒆂̃p�����^�łP��o�b�`���s����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedBatchExecSingle()
{
	if(m_list_contol.GetItemCount()==0)	return;
	
	show_dlg_item(EXEC_MODE_SINGLE_EXEC);
	API.SetBGProcID(BGPROC_BATCH_EXC);
}

/********************************************************************
�@  �\  ��  �� : �u<�v�{�^������
��    ��    �� : OnBnClickedBatchExecSinglePrev
��          �� : 
��    ��    �l : 
�@          �\ : ���X�g�Ō��ݑI�𒆂̗v�f����߂��A���̃p�����^�łP��o�b�`���s����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedBatchExecSinglePrev()
{
	if(m_list_contol.GetItemCount()==0)	return;

	int sel = get_selected_list_item();
	if(sel<=0){
		select_list_item(0);
	}else{
		select_list_item(sel-1);
	}

	show_dlg_item(EXEC_MODE_SINGLE_EXEC);
	API.SetBGProcID(BGPROC_BATCH_EXC);
}

/********************************************************************
�@  �\  ��  �� : �u>�v�{�^������
��    ��    �� : OnBnClickedBatchExecSingleNext
��          �� : 
��    ��    �l : 
�@          �\ : ���X�g�Ō��ݑI�𒆂̗v�f����i�߁A���̃p�����^�łP��o�b�`���s����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedBatchExecSingleNext()
{
	if(m_list_contol.GetItemCount()==0)	return;

	int sel = get_selected_list_item();
	if(sel<0 || sel==m_list_contol.GetItemCount()-1){
		select_list_item(0);
	}else{
		select_list_item(sel+1);
	}

	show_dlg_item(EXEC_MODE_SINGLE_EXEC);
	API.SetBGProcID(BGPROC_BATCH_EXC);
}

/********************************************************************
�@  �\  ��  �� : �u�摜�t�@�C�����J���v�{�^������
��    ��    �� : OnBnClickedButtonBatchFileOpen
��          �� : 
��    ��    �l : 
�@          �\ : �_�C�A���O���J���A�����őI�����ꂽ�摜�t�@�C���̃p�X�����X�g�֒ǉ�����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileOpen()
{
	int			file_number;
	CString		path_name, file_name[10000], path;
	CString		file_type_name[13], all_file_type;


	//���͉\�ȃt�@�C����
	file_type_name[12].LoadString(IDS_FILETYPE_ANY);//*.*
	file_type_name[0].LoadString(IDS_FILETYPE_BITMAP);//*.bmp
	file_type_name[1].LoadString(IDS_FILETYPE_RAW);//*.raw
	file_type_name[2].LoadString(IDS_FILETYPE_JPEG);//*.jpg
	file_type_name[3].LoadString(IDS_FILETYPE_WORD);//*.word
	file_type_name[4].LoadString(IDS_FILETYPE_LONG);//*.long
	file_type_name[5].LoadString(IDS_FILETYPE_FLOAT);//*.float
	file_type_name[6].LoadString(IDS_FILETYPE_COMPLEX);//*.comp
	file_type_name[7].LoadString(IDS_FILETYPE_XY);//*.xy
	file_type_name[8].LoadString(IDS_FILETYPE_XYZ);//*.xyz
	file_type_name[9].LoadString(IDS_FILETYPE_3D);//*.3d
	file_type_name[10].LoadString(IDS_FILETYPE_F3D);//*.f3d
	file_type_name[11].LoadString(IDS_FILETYPE_ARCHIVED);//*.pim
	
	all_file_type.Empty();
	for(int i=0 ; i<13 ; i++){
		all_file_type += file_type_name[i];
	}
	all_file_type += _T("|");

	if( !API.OpenFileDialog(10000,all_file_type, &path, file_name, &file_number) )	return;

	for(int i=0 ; i<file_number ; i++)
	{
		if(file_name[i].GetLength()<3)	return;//�t�@�C�����Z����

		BATCH_INPUT in_param={0};
		CString str = path + file_name[i];
		strcpy(in_param.imae_file_path, str.GetBuffer());
		str.ReleaseBuffer();
		add_list_item(&in_param);
	}
}

/********************************************************************
�@  �\  ��  �� : �u�폜�v�{�^������
��    ��    �� : OnBnClickedButtonBatchFileDelete
��          �� : 
��    ��    �l : 
�@          �\ : ���X�g�Ō��ݑI�𒆂̗v�f���폜����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileDelete()
{
	m_list_contol.DeleteItem( get_selected_list_item());
}

/********************************************************************
�@  �\  ��  �� : �u�N���A�v�{�^������
��    ��    �� : OnBnClickedButtonBatchFileClear
��          �� : 
��    ��    �l : 
�@          �\ : ���X�g�N���A
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileClear()
{
	m_list_contol.DeleteAllItems();
}



/********************************************************************
�@  �\  ��  �� : ���̓p�����^�擾
��    ��    �� : GetBatchInput
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::GetBatchInput(BATCH_INPUT *p_in_param, int *p_file_no)
{
	int file_no = 0;
	int image_num = 0;
	CString path = "";
	CString rootpath = "";

	//���ݑI�𒆂̃t�@�C�����X�g�ԍ����擾
	file_no = get_selected_list_item();
	if (file_no<0) {//�I�����ĂȂ���΍ŏ��̗v�f��I��
		file_no = 0;
		select_list_item(file_no);
	}


	API.GetEditValue(m_hWnd, IDC_EDIT_BATCH_ROOT, &rootpath);

	path = rootpath + m_list_contol.GetItemText(file_no, 1);//�Ώۉ摜�t�@�C��
	API.GetEditValue(m_hWnd, IDC_EDIT_BATCH_SRC_IMAGE_NO, &image_num);//�Ώۉ摜�i�[��̉摜�������ԍ�

	strcpy(p_in_param->imae_file_path, path.GetBuffer());	path.ReleaseBuffer();
	p_in_param->image_number = image_num;

	for (int n = 0; n < BATCH_IN_PARAM_NUM; n++) {
		CString str = m_list_contol.GetItemText(file_no, n + 2);
		strcpy(p_in_param->param[n], str.GetBuffer());	str.ReleaseBuffer();
	}

	if (p_file_no)
	{
		*p_file_no = file_no;
	}
}


/********************************************************************
�@  �\  ��  �� : ���́E�o�̓p�����^�ݒ�
��    ��    �� : SetBatchInOut
��          �� :
��    ��    �l :
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::SetBatchInOut(BATCH_INPUT *p_in_param, BATCH_OUTPUT *p_out_param)
{
	//���ݑI�𒆂̃t�@�C�����X�g�ԍ����擾
	int file_no = get_selected_list_item();
	if (file_no<0) {//�I�����ĂȂ���΍ŏ��̗v�f��I��
		file_no = 0;
		select_list_item(file_no);
	}

	set_list_item(file_no, p_in_param, p_out_param);
}

/********************************************************************
�@  �\  ��  �� : �o�b�N�O���E���h����
��    ��    �� : BackGroundProc
��          �� : 
��    ��    �l : 
�@          �\ : �R�}���h���o�b�N�O���E���h�Ŏ��s
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::BackGroundProc()
{
	for (;;)
	{
		switch (API.GetBGProcID())
		{
		case BGPROC_CAPTURING://�J�����X���[��
			break;

		case BGPROC_MOVIE://����\����
			break;

		case BGPROC_BATCH_EXC://�o�b�`������

			if (API.GetCheck(m_hWnd, IDC_CHECK_BATCH_EXEC))
			{
				//�A�����s
				if (single_execute(true))
				{
					//���X�g�̍Ō�܂œ��B�������A�����s�I��

					API.StopBGProc();
					show_dlg_item(EXEC_MODE_WAIT);//�_�C�A���O�A�C�e���̕\����ԕύX
					m_progress.SetPos(0);//�v���O���X�o�[���[���ɂ��ǂ�
					API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_PROGRESS, _T(""));
				}
				else
				{
					//���X�g�̍Ō�܂œ��B���Ă��Ȃ����A�����s�p��

					m_progress.SetPos(get_selected_list_item());//�v���O���X�o�[�������߂�
					API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_PROGRESS, (get_selected_list_item() + 1) * 100 / m_list_contol.GetItemCount());
				}
			}
			else
			{
				//�P�����s
				if (single_execute(false)) {
					//���g���C�v���Ȃ����I��
					API.StopBGProc();
					show_dlg_item(EXEC_MODE_WAIT);//�_�C�A���O�A�C�e���̕\����ԕύX
				}
			}

			break;
		}

		Sleep(10);
	}

}



/********************************************************************
�@  �\  ��  �� : �ucsv�ɏo�́v�{�^������
��    ��    �� : OnBnClickedButtonBatchFileOut
��          �� : 
��    ��    �l : 
�@          �\ : ���X�g�̓��e�i���o�̓p�����^�ꗗ�j��csv�t�@�C���ɏo�͂���
�@�@�@�@�@�@�@�@�@�o�͐��I�Ԃ��߂̃t�@�C���_�C�A���O���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileOut()
{
	CFileDialog myDLG(false, _T("csv"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
							"�摜�t�@�C�����X�g(*.csv)|*.CSV|�e�L�X�g(*.txt)|*.txt||");
	if( myDLG.DoModal() != IDOK )	return	;

	FILE *fp = fopen(myDLG.GetPathName(),"wt");
	if(fp){
		for(int i=0 ; i<m_list_contol.GetItemCount() ; i++)
		{
			//�摜�t�@�C����
			CString path = m_list_contol.GetItemText(i, 1);
			fprintf(fp, "%s",path.GetBuffer());
			path.ReleaseBuffer();

			//���̓p�����^
			for (int n = 0; n < BATCH_IN_PARAM_NUM; n++){
				CString str = m_list_contol.GetItemText(i, n+2);
				fprintf(fp, ",%s",str.GetBuffer());
				str.ReleaseBuffer();
			}

			//�o�̓p�����^
			for (int n = 0; n < BATCH_OUT_PARAM_NUM; n++){
				CString str = m_list_contol.GetItemText(i, n+2+BATCH_IN_PARAM_NUM);
				fprintf(fp, ",%s",str.GetBuffer());
				str.ReleaseBuffer();
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	}
}

/********************************************************************
�@  �\  ��  �� : �ucsv������́v�{�^������
��    ��    �� : OnBnClickedButtonBatchFileIn
��          �� : 
��    ��    �l : 
�@          �\ : ���X�g�̓��e�i���o�̓p�����^�ꗗ�j��csv�t�@�C��������͂���
�@�@�@�@�@�@�@�@�@�o�͐��I�Ԃ��߂̃t�@�C���_�C�A���O���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchFileIn()
{
	CFileDialog myDLG(true, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
							"�摜�t�@�C�����X�g(*.csv)|*.CSV|�e�L�X�g(*.txt)|*.txt||");
	if( myDLG.DoModal() != IDOK )	return	;

	load_image_list(myDLG.GetPathName());
}

/********************************************************************
�@  �\  ��  �� : ���X�g���ڂ��_�u���N���b�N
��    ��    �� : OnNMDblclkListBatch
��          �� : 
��    ��    �l : 
�@          �\ : ���s
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnNMDblclkListBatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	if(m_list_contol.GetItemCount()==0)	return;

	show_dlg_item(EXEC_MODE_SINGLE_EXEC);
	API.SetBGProcID(BGPROC_BATCH_EXC);


	*pResult = 0;
}

/********************************************************************
�@  �\  ��  �� : ���X�g�̌��o����������
��    ��    �� : OnLvnColumnclickListBatch
��          �� : 
��    ��    �l : 
�@          �\ : ���̍��ڂŃ��X�g�v�f���\�[�g����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnLvnColumnclickListBatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	m_sort_dir_flg = !m_sort_dir_flg;//�\�[�g������؂�ւ���
	m_iSubItem = pNMLV->iSubItem;//���X�g�̍��ڔԍ�
	m_list_contol.SortItems( compare_func, (LPARAM)this );

	for ( int i = 0; i < m_list_contol.GetItemCount(); i++ ){
			m_list_contol.SetItemData( i, (DWORD)i );
	}

	*pResult = 0;
}


/********************************************************************
�@  �\  ��  �� : ���X�g���ڂ��E�N���b�N
��    ��    �� : OnNMRClickListBatch
��          �� : 
��    ��    �l : 
�@          �\ : ���̓p�����^�̕ҏW
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnNMRClickListBatch(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	
	BATCH_INPUT in_param={0};
	CString param_val[BATCH_IN_PARAM_NUM];
	CConfirmDlg dlg;
	int selected_num = get_selected_list_item();
	if(selected_num<0)	return;

	//���̓p�����^��ݒ肷��_�C�A���O���o��
	dlg.SetTitle(_T("���̓p�����^"));
	dlg.SetWidth(50,400);
	for(int i=0 ; i<BATCH_IN_PARAM_NUM ; i++)
	{
		param_val[i] = m_list_contol.GetItemText(selected_num, i+2);	

		CString title;
		title.Format("����%d", i);
		dlg.RegistVar(title, &param_val[i]);
	}

	if(dlg.DoModal()!=IDOK)	return;

	//�p�����^�����X�g�ɐݒ肷��
	for(int i=0 ; i<BATCH_IN_PARAM_NUM ; i++)
	{
		strcpy( in_param.param[i] , param_val[i].GetBuffer() );
		param_val[i].ReleaseBuffer();
	}
	set_list_item(selected_num, &in_param);


	*pResult = 0;
}



/********************************************************************
�@  �\  ��  �� : �E�B���h�E�̃��T�C�Y
��    ��    �� : OnSize
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	arrange_dlg_item();
}



/********************************************************************
�@  �\  ��  �� : ���[�g�p�X�u���E�Y�{�^������
��    ��    �� : OnBnClickedButtonBatchRoot
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedButtonBatchRoot()
{
	CString rootFolder;
	if (API.FolderDialog(&rootFolder)){
		API.SetEditValue(m_hWnd, IDC_EDIT_BATCH_ROOT, rootFolder);
	}
}

/********************************************************************
�@  �\  ��  �� : �u���s���[���v�{�^������
��    ��    �� : OnBnClickedButtonChangeExecuteRule
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CBatchDlg::OnBnClickedButtonChangeExecuteRule()
{
	CConfirmDlg dlg;
	bool rule[3] = {true,false,false};

	for (int i = 0; i < 3; i++){
		rule[i] = (i == API.batch_execute_rule);
	}

	dlg.SetTitle("�o�b�`���s���[��");
	dlg.SetWidth(300, 30);
	dlg.RegistRadioVar("�X�N���v�g -> �J�X�^���t�@���N�V����(0,1,2,,,)", &rule[0]);
	dlg.RegistRadioVar("�J�X�^���t�@���N�V����(0,1,2,,,) -> �X�N���v�g ", &rule[1]);
	dlg.RegistRadioVar("�X�N���v�g �̂�", &rule[2]);

	if (dlg.DoModal() == IDOK){
		for (int i = 0; i < 3; i++){
			if (rule[i]){
				API.batch_execute_rule = i;
				break;
			}
		}
	}
}
