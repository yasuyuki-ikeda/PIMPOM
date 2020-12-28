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
//CSocketCtrlDlg
//�\�P�b�g�ʐM�ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "SocketCtrlDlg.h"

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
extern CPimpomAPI	API;

// *************************************
//         ��    ��    �Q    ��         
// *************************************

//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CSocketCtrlDlg, CDialog)

CSocketCtrlDlg::CSocketCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketCtrlDlg::IDD, pParent)
{

}

CSocketCtrlDlg::~CSocketCtrlDlg()
{
}

void CSocketCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SOCKET_SENDSTRING, m_sendString);
}

/********************************************************************
�@  �\  ��  �� : ���M��������o�b�N�A�b�v
��    ��    �� : backup_com_str
��          �� : CComboBox &combo�@(in)�R�}���h�̓������R���{�{�b�N�X���Q��
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::backup_com_str(CComboBox &combo)
{
	CString str,b_str;

	combo.GetWindowText(str);

	for(int i=0 ; i<combo.GetCount() ; i++){//�o�b�N�A�b�v���ꂽ�������Ȃ������ׂ�
		combo.GetLBText(i,b_str);

		if(b_str == str){
			return;//���łɃo�b�N�A�b�v����Ă���Ώ����I��
		}
	}

	combo.AddString(str);//�o�b�N�A�b�v����ĂȂ���΃��X�g�ɒǉ�
}

/********************************************************************
�@  �\  ��  �� : �ʐM�ݒ�����_�C�A���O�ɔ��f����
��    ��    �� : SetOpenInfoToDlg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::SetOpenInfoToDlg(CString host, int port)
{
	API.SetEditValue(m_hWnd, IDC_EDIT_SOKET_HOSTNAME, host);
	API.SetEditValue(m_hWnd, IDC_EDIT_SOCKET_PORTNUM, port);
}


/********************************************************************
�@  �\  ��  �� : ���M������������_�C�A���O�ɔ��f����
��    ��    �� : SetSendStringToDlg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::SetSendStringToDlg(CString message)
{
	m_sendString.SetWindowText(message);

	backup_com_str(m_sendString);

	CString	ret; ret.Format("%c%c", (char)CR , (char)LF);//���s�R�[�h
	CString log_str;
	API.GetEditValue(m_hWnd, IDC_EDIT_SOCKET_RECVSTRING, &log_str);
	API.SetEditValue(m_hWnd, IDC_EDIT_SOCKET_RECVSTRING, "<-" + message + ret + log_str);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSocketCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_SOCKET_OPEN, &CSocketCtrlDlg::OnBnClickedCheckSocketOpen)
	ON_BN_CLICKED(IDC_BUTTON_SOCKET_SEND, &CSocketCtrlDlg::OnBnClickedButtonSend)
	ON_EN_CHANGE(IDC_EDIT_SOCKET_DELIMITER, &CSocketCtrlDlg::OnEnChangeEditSocketDelimiter)
	ON_BN_CLICKED(IDC_BUTTON_SOCKET_RES_CLEAR, &CSocketCtrlDlg::OnBnClickedButtonSocketResClear)
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
BOOL CSocketCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	API.SetCheck(m_hWnd, IDC_RADIO_SOCKET_CLIENT, true);
	API.SetEditValue(m_hWnd,IDC_EDIT_SOKET_HOSTNAME,"localhost");
	API.SetEditValue(m_hWnd,IDC_EDIT_SOCKET_PORTNUM, 9876);

	API.SetEditValue(m_hWnd,IDC_EDIT_SOCKET_DELIMITER, API.socket_delimiter);

	return true;
}

/********************************************************************
�@  �\  ��  �� : ��M���̃R�[���o�b�N�֐�
��    ��    �� : ReceiveCallback
��          �� : HWND	wnd		(in)
				 BYTE	*data	(out)
				 int	*ndata	(out)
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::ReceiveCallback(HWND wnd, BYTE *data, int *ndata)
{
	char rcvdata[RECIVE_BUFFER_SIZE];
	memcpy(rcvdata, data, *ndata);
	rcvdata[*ndata]='\0';

	//��M���ʂ̏o��
	CString	ret; ret.Format("%c%c", (char)CR , (char)LF);//���s�R�[�h

	CString log_str;
	API.GetEditValue(wnd, IDC_EDIT_SOCKET_RECVSTRING, &log_str);
	API.SetEditValue(wnd, IDC_EDIT_SOCKET_RECVSTRING, "->" + CString(rcvdata) + ret + log_str);

	for(int i=0 ; i<CUSTOM_FUNC_MAX ;i++)
	{
		API.UserDialogReceive(i, data, ndata);
	}
}


/********************************************************************
�@  �\  ��  �� : �ڑ������̃R�[���o�b�N�֐�
��    ��    �� : AcceptCallback
��          �� : HWND	wnd		(in)
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::AcceptCallback(HWND wnd, int *errCode)
{
	if(*errCode){
		API.SetCheck(wnd, IDC_CHECK_SOCKET_OPEN, false);
		::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOKET_HOSTNAME), true);
		::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOCKET_PORTNUM), true);
		::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_CLIENT), true);
		::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_SERVER), true);
		API.ShowMessage(false, 0,true, "�ڑ����s");
	}else{
		//�_�C�A���O�̗L���E����
		API.SetCheck(wnd, IDC_CHECK_SOCKET_OPEN, true);
		::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOKET_HOSTNAME), false);
		::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOCKET_PORTNUM), false);
		::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_CLIENT), false);
		::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_SERVER), false);
		API.ShowMessage(false, 0,true, "�ڑ�����");
	}
}

/********************************************************************
�@  �\  ��  �� : �ʐM�Ւf���̃R�[���o�b�N�֐�
��    ��    �� : CloseCallback
��          �� : HWND	wnd		(in)
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::CloseCallback(HWND wnd)
{
	API.CloseSocket();
	API.SetCheck(wnd, IDC_CHECK_SOCKET_OPEN, false);
	::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOKET_HOSTNAME), true);
	::EnableWindow(::GetDlgItem(wnd, IDC_EDIT_SOCKET_PORTNUM), true);
	::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_CLIENT), true);
	::EnableWindow(::GetDlgItem(wnd, IDC_RADIO_SOCKET_SERVER), true);
	API.ShowMessage(false, 0,true, "�ؒf");
}

void CSocketCtrlDlg::SetSocketStatusToDlgItem()
{
	CString hostname;
	UINT portno;
	int cs;

	cs = API.GetSocketStatus(&hostname,&portno);
	if(cs!=-1)
	{
		API.SetCheck(m_hWnd, IDC_CHECK_SOCKET_OPEN, true);
		API.SetEditValue(m_hWnd,IDC_EDIT_SOKET_HOSTNAME,hostname);//�z�X�g��
		API.SetEditValue(m_hWnd,IDC_EDIT_SOCKET_PORTNUM,(int)portno);//�|�[�g�ԍ�

		if(cs==1){
			API.SetCheck(m_hWnd, IDC_RADIO_SOCKET_CLIENT, true);
		}else{
			API.SetCheck(m_hWnd, IDC_RADIO_SOCKET_SERVER, true);
		}

		//�_�C�A���O�̗L���E����
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_SOKET_HOSTNAME), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_SOCKET_PORTNUM), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_SOCKET_CLIENT), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_SOCKET_SERVER), false);

		API.ShowMessage(false, 0,true, "�ڑ�����");
	}
	else
	{
		API.SetCheck(m_hWnd, IDC_CHECK_SOCKET_OPEN, false);

		//�_�C�A���O�̗L���E����
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_SOKET_HOSTNAME), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_SOCKET_PORTNUM), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_SOCKET_CLIENT), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_SOCKET_SERVER), true);

		API.ShowMessage(false, 0,true, "�ؒf");
	}
}


/********************************************************************
�@  �\  ��  �� : �u�|�[�g���J���v�{�^������
��    ��    �� : OnBnClickedCheckSocketOpen
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::OnBnClickedCheckSocketOpen()
{
	CString hostname;
	int portno;
	

	API.GetEditValue(m_hWnd,IDC_EDIT_SOKET_HOSTNAME,&hostname);//�z�X�g���擾
	API.GetEditValue(m_hWnd,IDC_EDIT_SOCKET_PORTNUM,&portno);//�|�[�g�ԍ��擾

	if(API.GetCheck(m_hWnd,IDC_CHECK_SOCKET_OPEN))//�|�[�g���J��
	{
		if(API.GetCheck(m_hWnd, IDC_RADIO_SOCKET_SERVER))//�T�[�o
		{
			API.ShowMessage(true, 0,true, "�ڑ��ҋ@��...");
			API.OpenSocket(SOCKET_SERVER, portno, hostname);
		}
		else//�N���C�A���g
		{
			API.ShowMessage(true, 0,true, "�ڑ��ҋ@��...");
			API.OpenSocket(SOCKET_CLIENT, portno, hostname );
		}
	}else{//�|�[�g�����
		API.CloseSocket();
	}

}

/********************************************************************
�@  �\  ��  �� : �u������𑗂�v�{�^������
��    ��    �� : OnBnClickedButtonSend
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::OnBnClickedButtonSend()
{
	CString  str;

	m_sendString.GetWindowText(str);
	API.SendString(str);
}

/********************************************************************
�@  �\  ��  �� : �f���~�^�ύX
��    ��    �� : OnEnChangeEditSocketDelimiter
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::OnEnChangeEditSocketDelimiter()
{
	// TODO:  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A
	// �܂��ACDialog::OnInitDialog() �֐����I�[�o�[���C�h���āAOR ��Ԃ� ENM_CHANGE
	// �t���O���}�X�N�ɓ���āACRichEditCtrl().SetEventMask() ���Ăяo���Ȃ�����A
	// �R���g���[���́A���̒ʒm�𑗐M���܂���B

	API.GetEditValue(m_hWnd, IDC_EDIT_SOCKET_DELIMITER, &API.socket_delimiter);
}


/********************************************************************
�@  �\  ��  �� : ��M�����N���A
��    ��    �� : OnBnClickedButtonSocketResClear
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrlDlg::OnBnClickedButtonSocketResClear()
{
	API.SetEditValue(m_hWnd, IDC_EDIT_SOCKET_RECVSTRING, "");
}
