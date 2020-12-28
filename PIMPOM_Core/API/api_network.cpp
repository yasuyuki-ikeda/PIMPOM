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
//CPimpomAPI
//�ʐM
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

// *************************************
//         ��    ��    �Q    ��         
// *************************************

/********************************************************************
�@  �\  ��  �� : �\�P�b�g���J��
��    ��    �� : OpenSocket
��          �� : int			type		(in)�\�P�b�g�^�C�v 0:�T�[�o 1:�N���C�A���g
				 UINT			port		(in)�|�[�g�ԍ�
				 CString		host		(in)�z�X�g��
				 HWND			wnd			(in)�R�[���o�b�N�֐��ɓn���E�B���h�E�̃n���h��
				 void (*recive_callback)(HWND, BYTE*, int*)		(in)�f�[�^��M���̃R�[���o�b�N�֐��i�֐��|�C���^�j
				 void (*acc_callback)(HWND,int*)				(in)�ڑ��������̃R�[���o�b�N�֐��i�֐��|�C���^�j
				 void (*close_callback)(HWND)					(in)�ʐM�Ւf���̃R�[���o�b�N�֐��i�֐��|�C���^�j
��    ��    �l : ��������� true
�@          �\ : �\�P�b�g���J���ƂƂ��ɁA�ʐM�C�x���g�ɑ΂���R�[���o�b�N�֐���o�^����
                 ���̊֐����Ă΂��ƁA�\�P�b�g�ʐM�ݒ�E�B���h�E���J�����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::OpenSocket(int type, UINT port, CString host, HWND wnd, void (*recive_callback)(HWND, BYTE*, int*), void (*acc_callback)(HWND,int*), void (*close_callback)(HWND))
{
	if(!pSockCtrl){
		pSockCtrl = new CSocketCtrl();
		if(!pSockCtrl)	return false;
	}

	if(!pSockCtrl->OpenSock(type, port, host, wnd, recive_callback, acc_callback, close_callback)){
		delete	pSockCtrl;
		pSockCtrl = NULL;
		return false;
	}


	//�\�P�b�g�ʐM�ݒ�E�B���h�E���J��
	if(!pSocketCtrlDlg)
	{
		if(!DialogOpen(IDD_SOCKET_CTRL_DLG,true)){
			delete	pSockCtrl;
			pSockCtrl = NULL;
			return false;
		}

		pSocketCtrlDlg->SetOpenInfoToDlg(host, port);
	}

	//�\�P�b�g�ʐM�ݒ�E�B���h�E�̃R�[���o�b�N��ݒ�
	//pSockCtrl->SetAdditionalCallBack(pSocketCtrlDlg->m_hWnd, recive_callback, acc_callback, close_callback);
	pSockCtrl->SetAdditionalCallBack(pSocketCtrlDlg->m_hWnd, CSocketCtrlDlg::ReceiveCallback, CSocketCtrlDlg::AcceptCallback, CSocketCtrlDlg::CloseCallback);


	return true; 
}

/********************************************************************
�@  �\  ��  �� : ������𑗐M����
��    ��    �� : SendString
��          �� : CString		str			(in)���M���镶����
��    ��    �l : ���M���ꂽ�f�[�^��[byte]
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CPimpomAPI::SendString(CString str)
{
	if(!pSockCtrl)	return -1;

	int ret = pSockCtrl->SendString(str);
	if(ret){
		if(pSocketCtrlDlg )
		{
			pSocketCtrlDlg->SetSendStringToDlg(str);
		}
	}

	return ret;
}

/********************************************************************
�@  �\  ��  �� : �\�P�b�g�����
��    ��    �� : CloseSocket
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::CloseSocket()
{
	if(pSockCtrl){
		pSockCtrl->Close();
		delete	pSockCtrl;
		pSockCtrl = NULL;
	}

	if(pSocketCtrlDlg){
		//�\�P�b�g�ʐM�ݒ�E�B���h�E�ɃX�e�[�^�X�\��
		pSocketCtrlDlg->SetSocketStatusToDlgItem();
	}
}

/********************************************************************
�@  �\  ��  �� : �\�P�b�g�̏�Ԏ擾
��    ��    �� : GetSocketStatus
��          �� : CString* pHostName (out)�z�X�g��
				 UINT* pPortNo		(out)�|�[�g�ԍ�
��    ��    �l : 0:�T�[�o 1:�N���C�A���g -1:�J���ꂸ
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CPimpomAPI::GetSocketStatus(CString* pHostName, UINT* pPortNo)
{
	if(pSockCtrl){
		*pHostName = pSockCtrl->HostName;
		*pPortNo = pSockCtrl->PortNumber;
		return pSockCtrl->C_S_type;
	}else{
		return -1;
	}
}