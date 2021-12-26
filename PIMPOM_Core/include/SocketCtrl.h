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
//CSocketCtrl
//�\�P�b�g�ʐM�̃��b�p�[
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#pragma once
#include <afxsock.h>
#include "global_define.h"
#include "global_typedef.h"

///*************************************
//         �N���X   ��   �`         
///*************************************
class PIMPOM_EXT CSocketCtrl : public CAsyncSocket
{


// �A�g���r���[�g
public:
	CSocketCtrl*	pSubSock;
	CString			HostName;//�z�X�g��
	UINT			PortNumber;//�|�[�g�ԍ�
	char			RecvBuffer[RECIVE_BUFFER_SIZE];//��M�o�b�t�@
	int				RecvBufferCounter;//��M�o�b�t�@�̎g�p�T�C�Y
	int				C_S_type;
	
	HWND			h_collback_wind[CALLBACK_FUNC_NUM];//�R�[���o�b�N�E�B���h�E
	void			(*recieve_callback_func[CALLBACK_FUNC_NUM])(HWND, BYTE*, int*);//��M���̃R�[���o�b�N�֐�
	void			(*accept_callback_func[CALLBACK_FUNC_NUM])(HWND, int*);//��M���̃R�[���o�b�N�֐�
	void			(*close_callback_func[CALLBACK_FUNC_NUM])(HWND);//�ؒf���̃R�[���o�b�N�֐�

// �I�y���[�V����
public:
	CSocketCtrl();
	virtual ~CSocketCtrl();
	bool OpenSock(int type, UINT port, CString host, HWND wnd=NULL, void (*rcv_callback)(HWND, BYTE*, int*)=NULL, void (*acc_callback)(HWND, int*)=NULL, void (*cls_callback)(HWND)=NULL);
	void SetAdditionalCallBack(HWND wnd, void (*rcv_callback)(HWND, BYTE*, int*), void (*acc_callback)(HWND, int*), void (*cls_callback)(HWND));
	int SendString(CString message);


// �I�[�o�[���C�h
public:
	void OnClose(int nErrorCode);
	void OnReceive(int nErrorCode);
	void OnAccept(int nErrorCode);
	void OnConnect(int nErrorCode);
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂�
	//{{AFX_VIRTUAL(CSocketCtrl)
	//}}AFX_VIRTUAL

protected:
	void SetDelimiter(char delimiter[]);


	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSocketCtrl)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
