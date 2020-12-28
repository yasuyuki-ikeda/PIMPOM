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


#include "stdafx.h"
#include "SerialCtrl.h"
#include "PIMPOM_API.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern	CPimpomAPI	API;
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CSerialCtrl::CSerialCtrl()
{
	hSerialPort = INVALID_HANDLE_VALUE;

}

CSerialCtrl::~CSerialCtrl()
{
	ClosePort();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////シリアルポートを開く
bool CSerialCtrl::OpenPort(CString portname , CWnd*wnd)
{
	COMMTIMEOUTS	CommTimeOuts ;
	COMMCONFIG		CommConfig;

	hSerialPort = CreateFile( portname , GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

		if(hSerialPort == INVALID_HANDLE_VALUE){
			AfxMessageBox("シリアルポートの取得ができません");
			return  false;
		}


///////タイムアウトの設定
    CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.ReadTotalTimeoutConstant = 0 ;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
    CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;
    SetCommTimeouts( hSerialPort, &CommTimeOuts ) ;

/////////通信ポートの設定
	ZeroMemory(&CommConfig , sizeof(COMMCONFIG));
	CommConfig.dwSize = sizeof(COMMCONFIG);
	CommConfig.wVersion = 1;
		if( ::CommConfigDialog( portname , wnd->GetSafeHwnd() , &CommConfig) != IDOK){
			return false;
		}

	SetCommConfig(hSerialPort , &CommConfig , sizeof(COMMCONFIG) );
	return	true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////シリアルポートを閉じる
void CSerialCtrl::ClosePort()
{
	if(hSerialPort == INVALID_HANDLE_VALUE)		return;

	CloseHandle(hSerialPort);
	hSerialPort = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////シリアルポートから命令を受け取る
void CSerialCtrl::CatchCommand(char *command, long *len)
{
	if(hSerialPort == INVALID_HANDLE_VALUE)		return;

	char		temp;
	DWORD		ch;
	long		pos = 0;
	BOOL		lf_wait = false;

	DWORD	start_t = GetTickCount();
	while( ReadFile( hSerialPort ,&temp , 1 , &ch ,NULL) ){	

			if( ch<=0 ){
				if( start_t + 10 < GetTickCount() )	continue;
				else									break;
			}

			if( temp == CR ){		
				lf_wait = true;
			}else if( lf_wait && temp == LF ){
				break;
			}

		command[ pos++ ] = temp;
	}

	PurgeComm( hSerialPort , PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

	command[pos] = '\0';
	*len = pos;	
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////シリアルポートに命令を送る
void CSerialCtrl::SendData(float pitch, float phase, bool direction)
{
	CString		str , cr_lf;
	DWORD		ch;

	str.Format("%f,%f,%d;" , pitch , phase , direction);
	cr_lf.Format("%c%c", (char)CR , (char)LF);
	str = str + cr_lf;

	for (int i = 0 ; i < str.GetLength() ; i++ ) {
		char c = str[i];
		WriteFile(hSerialPort, &c ,1,&ch,NULL);
	}
	return	;
}
