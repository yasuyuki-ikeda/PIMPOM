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
#include "stdafx.h"
#include "SocketCtrl.h"
#include "PIMPOM_API.h"
#include <stdio.h>

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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



CSocketCtrl::CSocketCtrl()
{
	pSubSock = NULL;
	memset(RecvBuffer,0,RECIVE_BUFFER_SIZE);
	RecvBufferCounter=0;
	

	for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
	{
		recieve_callback_func[i] = NULL;
		close_callback_func[i] = NULL;
		h_collback_wind[i] = NULL;
	}

}

CSocketCtrl::~CSocketCtrl()
{
	if(pSubSock){
		pSubSock->Close();
		delete	pSubSock;
		pSubSock = NULL;
	}
}


void CSocketCtrl::SetDelimiter(char delimiter[])
{
	if(API.socket_delimiter==1){
		sprintf_s(delimiter, 10, "%c",CR);
	}else if(API.socket_delimiter==2){
		sprintf_s(delimiter, 10, "%c",LF);
	}else{
		sprintf_s(delimiter, 10, "%c%c",CR,LF);
	}
}

/********************************************************************
�@  �\  ��  �� : �\�P�b�g���J��
��    ��    �� : CSocketCtrl
��          �� : int			type		(in)�\�P�b�g�^�C�v 0:�T�[�o 1:�N���C�A���g
				 UINT			port		(in)�|�[�g�ԍ�
				 CString		host		(in)�z�X�g��
				 HWND			wnd			(in)�R�[���o�b�N�֐��ɓn���E�B���h�E�̃n���h��
				 void (*recive_callback)(HWND, BYTE*, int*)		(in)�f�[�^��M���̃R�[���o�b�N�֐��i�֐��|�C���^�j
				 void (*acc_callback)(HWND,int*)				(in)�ڑ��������̃R�[���o�b�N�֐��i�֐��|�C���^�j
				 void (*close_callback)(HWND)					(in)�ʐM�Ւf���̃R�[���o�b�N�֐��i�֐��|�C���^�j
��    ��    �l : ��������� true
�@          �\ : �\�P�b�g���J���ƂƂ��ɁA�ʐM�C�x���g�ɑ΂���R�[���o�b�N�֐���o�^����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CSocketCtrl::OpenSock(int type, UINT port, CString host, HWND wnd, void (*rcv_callback)(HWND, BYTE*, int*), void (*acc_callback)(HWND,int*), void (*cls_callback)(HWND))
{
	C_S_type = type;
	PortNumber = port;
	HostName = host;

		if(C_S_type == SOCKET_SERVER){//Server
			//�R�[���o�b�N�ݒ�
			recieve_callback_func[0] = rcv_callback;
			accept_callback_func[0] = acc_callback;
			close_callback_func[0] = cls_callback;
			h_collback_wind[0] = wnd;

			if( !CAsyncSocket::Create(PortNumber) ){//�\�P�b�g�쐬
				AfxMessageBox( "Create error" );
				return	false;
			}

			if( !CAsyncSocket::Listen() ){//�ڑ��҂�
				AfxMessageBox( "Listen error ");
				Close();
				return	false;
			}

		}else if(C_S_type == SOCKET_CLIENT){//Client

			//�R�[���o�b�N�ݒ�
			recieve_callback_func[0] = rcv_callback;
			accept_callback_func[0] = acc_callback;
			close_callback_func[0] = cls_callback;
			h_collback_wind[0] = wnd;

			if( !CAsyncSocket::Create() ){//�\�P�b�g�쐬
				AfxMessageBox( "Create error");
				return	false;
			}

			int err;
			if( !(err = CAsyncSocket::Connect(HostName, PortNumber) ) ){//�T�[�o�[�ɐڑ�
				if(CAsyncSocket::GetLastError()!=WSAEWOULDBLOCK){
					AfxMessageBox( "Connect error");
					return	false;
				}
			}

		}else{
			//�R�[���o�b�N�ݒ�
			recieve_callback_func[0] = rcv_callback;
			accept_callback_func[0] = acc_callback;
			close_callback_func[0] = cls_callback;
			h_collback_wind[0] = wnd;
		}

	return	true;
}

void CSocketCtrl::SetAdditionalCallBack(HWND wnd, void (*rcv_callback)(HWND, BYTE*, int*), void (*acc_callback)(HWND, int*), void (*cls_callback)(HWND))
{
	recieve_callback_func[1] = rcv_callback;
	accept_callback_func[1] = acc_callback;
	close_callback_func[1] = cls_callback;
	h_collback_wind[1] = wnd;
}

/********************************************************************
�@  �\  ��  �� : �\�P�b�g�����
��    ��    �� : CloseSocket
��          �� : 
��    ��    �l : 
�@          �\ : �\�P�b�g�����R�[���o�b�N�����{
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrl::OnClose(int nErrorCode)
{
	if(nErrorCode){
		Close();
	}

	for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
	{
		if(close_callback_func[i]!=NULL){
			(close_callback_func[i])(h_collback_wind[i]);
			RecvBufferCounter=0;
		}
	}

	CAsyncSocket::OnClose(nErrorCode);
}

/********************************************************************
�@  �\  ��  �� : ������𑗐M����
��    ��    �� : SendString
��          �� : CString		message			(in)���M���镶����
��    ��    �l : ���M���ꂽ�f�[�^��[byte]
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CSocketCtrl::SendString(CString message)
{
	if(C_S_type == SOCKET_SERVER){
		if(pSubSock){
			return  pSubSock->SendString( message );
		}
	}else{
		int len;
		char *buf;
		char Delimiter[10];//�����

		SetDelimiter(Delimiter);
		
		message = message + CString(Delimiter);//���b�Z�[�W�̍Ō�Ƀf���~�^��t������
		len = message.GetLength();
		buf = message.GetBuffer();

		//���b�Z�[�W�T�C�Y�����M���J��Ԃ�
		int sendsum=0;
		do{
			sendsum += Send( &buf[sendsum] , len-sendsum);
		}while(sendsum<len);

		return sendsum;
	}
	return 0;
}


void CSocketCtrl::OnAccept(int nErrorCode)
{
	if(nErrorCode){
		Close();
	}

	if(C_S_type == SOCKET_SERVER){//Server

			if(pSubSock){
				pSubSock->Close();
				delete	pSubSock;
				pSubSock = NULL;
			}
				
			pSubSock = new CSocketCtrl();//�T�u�\�P�b�g����
			pSubSock->OpenSock(SOCKET_SERVER_SUB , PortNumber , HostName );	
			if(!pSubSock)	return ;

			//�T�u�\�P�b�g�ɃR�[���o�b�N���R�s�[
			for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++){
				pSubSock->recieve_callback_func[i] = recieve_callback_func[i];
				pSubSock->accept_callback_func[i] = accept_callback_func[i];
				pSubSock->close_callback_func[i] = close_callback_func[i];
				pSubSock->h_collback_wind[i] = h_collback_wind[i];
			}

			if ( !CAsyncSocket::Accept( *pSubSock )){//�ڑ��󂯓���
				AfxMessageBox( "Accept error");
				return ;
			}
	}


	//�R�[���o�b�N���s
	for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
	{
		if(accept_callback_func[i]!=NULL){
			(accept_callback_func[i])(h_collback_wind[i], &nErrorCode);
			RecvBufferCounter=0;
		}
	}


	CAsyncSocket::OnAccept(nErrorCode);
}


void CSocketCtrl::OnConnect(int nErrorCode)
{
	if(nErrorCode){
		Close();
	}

	//�R�[���o�b�N���s
	for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
	{
		if(accept_callback_func[i]!=NULL){
			(accept_callback_func[i])(h_collback_wind[i], &nErrorCode);
			RecvBufferCounter=0;
		}
	}

	CAsyncSocket::OnConnect(nErrorCode);
}

/********************************************************************
�@  �\  ��  �� : ��������󂯎��
��    ��    �� : OnReceive
��          �� : int nErrorCode
��    ��    �l : 
�@          �\ : �N���C�A���g�\�P�b�g��p�̃R�[���o�b�N�֐�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CSocketCtrl::OnReceive(int nErrorCode)
{
	char	data[1000];
	char Delimiter[10];//�����

	SetDelimiter(Delimiter);


	if ( !nErrorCode){
		int rcvlen;
		if( (rcvlen = Receive( data , 1000)) > 0 )//���b�Z�[�W��M
		{
			for(int i=0 ; i<rcvlen ; i++)//�ꕶ�����`�F�b�N
			{
				//�f���~�^�ł��邩�ǂ���
				bool is_delimiter=true;
				for(int n=0 ; n<(int)strlen(Delimiter) && n+i<rcvlen  ; n++){
					if(data[i+n]!=Delimiter[n]){
						is_delimiter=false;
						break;
					}
				}

				if(!is_delimiter)//�f���~�^�łȂ���΃o�b�t�@�ɓ����
				{
					RecvBuffer[RecvBufferCounter] = data[i];
					RecvBufferCounter++;
					if(RecvBufferCounter>=RECIVE_BUFFER_SIZE){//�o�b�t�@�T�C�Y���z����
						RecvBufferCounter=0;
						AfxMessageBox( "Buffer over run" );
						return;
					}
				}else{//�f���~�^�����������ꍇ�́A�R�[���o�b�N�֐����Ăяo��
					for(int i=0 ; i<CALLBACK_FUNC_NUM ; i++)
					{
						if(recieve_callback_func[i]!=NULL){
							(recieve_callback_func[i])(h_collback_wind[i], (BYTE*)RecvBuffer, &RecvBufferCounter);
							RecvBufferCounter=0;
						}
					}
				}
			}
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}






