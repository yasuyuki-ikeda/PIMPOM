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
//�t�@�C��IO
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
#include "PIMPOM_API.h"
#include "atlimage.h"


#include <Shlobj.h>
#pragma comment(lib, "Ole32.lib")

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

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED){
		SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
	}
	return 0;
}

CString BrowseForFolder(CString path, CString root)
{
	BOOL		bRes;
	char		chPutFolder[MAX_PATH];
	LPITEMIDLIST	pidlRetFolder;
	BROWSEINFO	stBInfo;
	CString		cRetStr;
	char		c_path[MAX_PATH];


	if (root != _T("")){
		path = root + path;
	}

	strcpy(c_path, path.GetBuffer());
	path.ReleaseBuffer();

	//�@�\���̂����������܂��B
	stBInfo.pidlRoot = NULL;	//���[�g�t�H���_�ł��B
	stBInfo.hwndOwner = NULL;	//�\������_�C�A���O�̐e�E�B���h�E�̃n���h���ł��B
	stBInfo.pszDisplayName = chPutFolder;	//�I������Ă���t�H���_�����󂯂܂��B
	stBInfo.lpszTitle = "";	//�����̕�����ł��B
	stBInfo.ulFlags = 0;	//�\���t���O�ł��B
	stBInfo.lpfn = &BrowseCallbackProc;	//�_�C�A���O�v���V�[�W���ւ̃|�C���^�ł��B
	stBInfo.lParam = (LPARAM)c_path;	//�v���V�[�W���ɑ���p�����[�^�[�ł��B

	//�@�_�C�A���O�{�b�N�X��\�����܂��B
	pidlRetFolder = ::SHBrowseForFolder(&stBInfo);

	//�@pidlRetFolder�ɂ͂��̃t�H���_��\���A�C�e���h�c���X�g�ւ̃|�C���^��
	//�@�����Ă��܂��BchPutFolder�ɂ͑I�����ꂽ�t�H���_���i��t���p�X�j����
	//�@���������Ă��Ȃ��̂ŁA���̃|�C���^�𗘗p���܂��B

	if (pidlRetFolder != NULL)
	{
		bRes = ::SHGetPathFromIDList(pidlRetFolder, chPutFolder);//�@�t���p�X���擾
		if (bRes != FALSE)
		{
			cRetStr = chPutFolder;

			if (root != ""){//���[�g����̑��΃p�X
				cRetStr = cRetStr.Right(cRetStr.GetLength() - root.GetLength());
			}

		}

		::CoTaskMemFree(pidlRetFolder);
	}

	return cRetStr;
}



/********************************************************************
�@  �\  ��  �� : �摜�f�[�^���t�@�C������ǂݍ���
��    ��    �� : template <class DATA>�@load_image_template
��          �� : DATA		*p_data		(out)�摜�f�[�^�̔z��
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
				long		page		(in)�摜�f�[�^�̉��s����
				long		channel		(in)�摜�f�[�^�̃`���l����
��    ��    �l : ��������� true
�@          �\ : �S�Ă̌^�̉摜�ɑΉ����邽�߂̃e���v���[�g
				�t�@�C���̓��e��DATA�Ŏw�肳���^�̃o�C�i���̃x�^�f�[�^

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	load_image_template(DATA *p_data, CString pathname, CSize size, long page, long channel)
{
	CFile		file;
	CString		str;
	int n;
	DATA *addr;

		//�t�@�C�����J��
		if( ! file.Open(pathname ,  CFile::modeRead | CFile::typeBinary ) ){
			str = pathname + _T(" ���J���܂���");
			AfxMessageBox(str);
			return	false;
		}

		//�f�[�^��ǂݍ���
		for(n=0 ; n<page*channel ; n++){
			addr = p_data + size.cx * size.cy * n;
			if( file.Read( addr, size.cx * size.cy * sizeof(DATA) ) != size.cx * size.cy * sizeof(DATA) ){
				AfxMessageBox("�ǂݍ��݂Ɏ��s���܂���");
				file.Close();
				return	false;
			}
		}
	file.Close();//�t�@�C�������
	
	return	true;
}


/********************************************************************
�@  �\  ��  �� : �摜�f�[�^���t�@�C���ɕۑ�����
��    ��    �� : template <class DATA>�@save_image_template
��          �� : DATA		*p_data		(in)�摜�f�[�^�̔z��
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
				long		page		(in)�摜�f�[�^�̉��s����
				long		channel		(in)�摜�f�[�^�̃`���l����
��    ��    �l : ��������� true
�@          �\ : �S�Ă̌^�̉摜�ɑΉ����邽�߂̃e���v���[�g
				�t�@�C���̓��e��DATA�Ŏw�肳���^�̃o�C�i���̃x�^�f�[�^

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
template <class DATA>
bool	save_image_template(DATA *p_data , CString	pathname, CSize size, long page, long channel)
{
	CFile	file;
	CString	str;
	int n;
	DATA *addr;
	long	footer[4]={0};

	if(p_data ==NULL)	return	false;
	if( ! file.Open(pathname , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //�t�@�C�����J��

	//�f�[�^�ۑ�
	for(n=0 ; n<page*channel ; n++){
		addr = p_data + size.cx * size.cy * n;
		file.Write( addr ,  size.cx * size.cy * sizeof(DATA) );
	}

	//�t�b�^����������
	footer[0]=size.cx;
	footer[1]=size.cy;
	footer[2]=page;
	footer[3]=channel;
	file.Write(footer, sizeof(footer) );

	file.Close();//�t�@�C�������

	return	true;
}



static bool	load_image_internal(CPimpomAPI *pAPI, int data_type, long image_number, CString pathname, CSize size, int page)
{
	CDataUnit	*p_du;
	CString		str;
	bool ret;

		if(image_number == CURRENT_IMAGE)		image_number = pAPI->GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	p_du = pAPI->FormatDataUnit( image_number , data_type, size, page, true);//�V�������������擾
		if(p_du==NULL)	return	false;//�������擾�Ɏ��s�����Ƃ�

		switch(data_type){
			case BYTE_FORMAT:
				ret = load_image_template(p_du->pByteData, pathname, size, 1, 1);
				break;

			case THREE_D_FORMAT:
				ret = load_image_template(p_du->pByteData, pathname, size, page, 1);
				break;

			case SHORT16_FORMAT:
				ret = load_image_template(p_du->pShortData, pathname, size, 1, 1);
				break;

			case WORD_FORMAT:
				ret = load_image_template(p_du->pWordData, pathname, size, 1, 1);
				break;

			case LONG32_FORMAT:
				ret = load_image_template(p_du->pLongData, pathname, size, 1, 1);
				break;

			case DWORD_FORMAT:
				ret = load_image_template(p_du->pDwordData, pathname, size, 1, 1);
				break;

			case FLOAT_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, 1, 1);
				break;

			case XY_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, 1, 2);
				break;

			case XYZ_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, 1, 3);
				break;

			case COMPLEX_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, 1, 4);
				break;

			case FLOAT_3D_FORMAT:
				ret = load_image_template(p_du->pFloatData, pathname, size, page, 1);
				break;

			case RGB_FORMAT:
				ret = load_image_template(p_du->pByteData, pathname, size, 1, 3);
				break;

			case RGB_3D_FORMAT:
				ret = load_image_template(p_du->pByteData, pathname, size, page, 3);
				break;
		}

	p_du->DataName = ::PathFindFileNameA(pathname);//���O��t����
	p_du->SrcFilePath = pathname;//�ǂݍ��񂾃t�@�C���̃p�X���c��
	return	ret;
}

/********************************************************************
�@  �\  ��  �� : �t�@�C��IO�̃��O���Ƃ�
��    ��    �� : SetFileIOLog
��          �� : CString		pathname	(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SetFileIOLog(CString pathname)
{
	CString log[FILE_IO_LOG_MAX], key;

	//���݂̃��O��ǂݏo��
	for(int i=0 ; i<FILE_IO_LOG_MAX ; i++){
		key.Format("LOG%d",i);
		log[i] = GetProfile("FILE_LOG", key);
		if(log[i] == pathname){
			return true;
		}
	}

	//���O�ԍ��������߂�
	for(int i=FILE_IO_LOG_MAX-1 ; i>0 ; i--){
		log[i]=log[i-1];
	}
	log[0]=pathname;

	//�t�@�C���ɏ����o��
	for(int i=0 ; i<FILE_IO_LOG_MAX ; i++){
		key.Format("LOG%d",i);
		WriteProfile("FILE_LOG", key, log[i]);
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : �t�@�C��IO�̃��O���擾����
��    ��    �� : GetFileIOLog
��          �� : int	 log_number	(in)���O�ԍ��@�ԍ����������قǐV����
��    ��    �l : �t�@�C���p�X
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CString	CPimpomAPI::GetFileIOLog(int log_number)
{
	//���݂̃��O��ǂݏo��
	CString key;
	key.Format("LOG%d",log_number);
	return GetProfile("FILE_LOG", key);

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//�ǂݍ���
/////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : 8bit�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadByteImage
��          �� : BYTE		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadByteImage(BYTE *p_data, CString pathname, CSize size)
{
	bool ret= load_image_template(p_data, pathname, size, 1,1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : 8bit�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadByteImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadByteImage(long image_number, CString pathname, CSize size)
{
	bool ret = load_image_internal(this, BYTE_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 16bit�������^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadShortImage
��          �� : short		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		filename	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadShortImage(short *p_data, CString pathname, CSize size)
{
	bool ret =  load_image_template(p_data, pathname, size, 1,1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 16bit�����Ȃ��^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadWordImage
��          �� : WORD		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadWordImage(WORD *p_data, CString pathname, CSize size)
{
	bool ret =  load_image_template(p_data, pathname, size, 1,1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : 16bit�������^�f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadShortImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadShortImage(long image_number, CString pathname, CSize size)
{
	bool ret =  load_image_internal(this, SHORT16_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 16bit�����Ȃ��^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadWordImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadWordImage(long image_number, CString pathname, CSize size)
{
	bool ret =  load_image_internal(this, WORD_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : 32bit�����t���^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadLongImage
��          �� : long		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadLongImage(long *p_data, CString pathname, CSize size)
{
	bool ret = load_image_template(p_data, pathname, size, 1, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 32bit�����Ȃ��^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadDWordImage
��          �� : DWORD		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadDWordImage(DWORD *p_data, CString pathname, CSize size)
{
	bool ret = load_image_template(p_data, pathname, size, 1, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 32bit�������^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadLongImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadLongImage(long image_number, CString pathname, CSize size)
{
	bool ret =  load_image_internal(this, LONG32_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 32bit�����Ȃ��^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadDwordImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadDwordImage(long image_number, CString pathname, CSize size)
{
	bool ret =  load_image_internal(this, DWORD_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : float(32���������_)�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadFloatImage
��          �� : float		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadFloatImage(float *p_data, CString pathname, CSize size)
{
	bool ret = load_image_template(p_data, pathname, size, 1,1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : float(32���������_)�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadFloatImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadFloatImage(long image_number, CString	pathname, CSize size)
{
	bool ret = load_image_internal(this, FLOAT_FORMAT, image_number, pathname, size, 1);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : ���f��(32���������_�~4)�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadComplexImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadComplexImage(long image_number, CString	pathname, CSize size)
{
	bool ret = load_image_internal(this, COMPLEX_FORMAT, image_number, pathname, size, 4);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : XY(32���������_�~2)�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadXYImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadXYImage(long image_number, CString	pathname, CSize size)
{
	bool ret = load_image_internal(this, XY_FORMAT, image_number, pathname, size, 2);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : XYZ(32���������_�~3)�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadXYZImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//�摜�������ԍ���CURRENT_IMAGE�ɂ����ꍇ�͌��ݑI�𒆂̉摜�ɑ΂��ď����������Ȃ�
bool	CPimpomAPI::LoadXYZImage(long image_number, CString	pathname, CSize size)
{
	bool ret = load_image_internal(this, XYZ_FORMAT, image_number, pathname, size, 3);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : 3D(8bit�̘A���摜)�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : Load3DImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::Load3DImage(long image_number, CString	pathname, CSize size, long page)
{
	bool ret = load_image_internal(this, THREE_D_FORMAT, image_number, pathname, size, page);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : Float3D(32bit���������_�̘A���摜)�^�̃f�[�^���t�@�C������ǂݍ���
��    ��    �� : LoadF3DImage
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadF3DImage(long image_number, CString	pathname, CSize size, long page)
{
	bool ret = load_image_internal(this, FLOAT_3D_FORMAT, image_number, pathname, size, page);
	if(ret){
		SetFileIOLog(pathname);
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : �摜�t�@�C����ǂݍ���
��    ��    �� : SelectAndLoadImageFile
��          �� : long image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ��������� true
�@          �\ : �ۑ�����t�@�C����I�Ԃ��߂̃t�@�C���_�C�A���O���o���A
				�I�΂ꂽ�t�@�C���̊g���q����^�C�v�𔻒f���ăt�@�C�����J��
				 �Ή����Ă���g���q�͈ȉ��̂Ƃ���
				 *.bmp: 8bit�ɸہA24bit�װ
				 *.raw: 8bit�ɸ�
				 *.jpg: 24bit�װ
				 *.word: 16bit(PIMPOM��p̫�ϯ�)
				 *.long: 32bit(PIMPOM��p̫�ϯ�)
				 *.float: 32bit���������_(PIMPOM��p̫�ϯ�)
				 *.comp: ���f��(PIMPOM��p̫�ϯ�)
				 *.xy: 2�������z(PIMPOM��p̫�ϯ�)
				 *.xyz: 3�������z(PIMPOM��p̫�ϯ�)
				 *.3d: 8bit����(PIMPOM��p̫�ϯ�)
				 *.f3d: 32bit���������_����(PIMPOM��p̫�ϯ�)
				 *.pim: �摜�������̃A�[�J�C�u�f�[�^(PIMPOM��p̫�ϯ�)

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SelectAndLoadImageFile(long image_number)
{
	int i;
	CString		file_type_name[15], all_file_type;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	//���͉\�ȃt�@�C����
	file_type_name[0].LoadString(IDS_FILETYPE_ANY);//*.*
	file_type_name[1].LoadString(IDS_FILETYPE_BITMAP);//*.bmp
	file_type_name[2].LoadString(IDS_FILETYPE_RAW);//*.raw
	file_type_name[3].LoadString(IDS_FILETYPE_JPEG);//*.jpg
	file_type_name[4].LoadString(IDS_FILETYPE_WORD);//*.word
	file_type_name[5].LoadString(IDS_FILETYPE_SHORT);//*.short
	file_type_name[6].LoadString(IDS_FILETYPE_LONG);//*.long
	file_type_name[7].LoadString(IDS_FILETYPE_DWORD);//*.dword
	file_type_name[8].LoadString(IDS_FILETYPE_FLOAT);//*.float
	file_type_name[9].LoadString(IDS_FILETYPE_COMPLEX);//*.comp
	file_type_name[10].LoadString(IDS_FILETYPE_XY);//*.xy
	file_type_name[11].LoadString(IDS_FILETYPE_XYZ);//*.xyz
	file_type_name[12].LoadString(IDS_FILETYPE_3D);//*.3d
	file_type_name[13].LoadString(IDS_FILETYPE_F3D);//*.f3d
	file_type_name[14].LoadString(IDS_FILETYPE_ARCHIVED);//*.pim
	

	all_file_type.Empty();
		for(i=0 ; i<13 ; i++)	all_file_type += file_type_name[i];
	all_file_type += _T("|");

	//���������͂��\�ɂ���
	CFileDialog myDLG(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT , all_file_type );
	char	name[ MULTI_FILE_PATH_BUFFER ];
	name[0] = '\0';
	myDLG.m_ofn.lpstrFile = name;
	myDLG.m_ofn.nMaxFile  = MULTI_FILE_PATH_BUFFER-1;

		if(myDLG.DoModal() != IDOK)  return	false;

	POSITION	pos = myDLG.GetStartPosition();

		//�I�������t�@�C�������Ԃɓǂݍ���
		while( ( pos != NULL )   &&   ( image_number < data_number ) )
		{
			if (SelectAndLoadImageFile(image_number, myDLG.GetNextPathName(pos)))
			{
				DrawImage( image_number, true,true);
			}

			image_number ++;
		}
	return	true;
}



/********************************************************************
�@  �\  ��  �� : �o�C�i���摜�t�@�C���̃t�b�^������摜�̕��E�����E���s�E�`���l����ǂ݂���
��    ��    �� : get_file_footer
��          �� : CString	pathname						(in)�t�@�C���̃t���p�X
				 CString	ext								(in)�t�@�C���̊g���q
				 short		*pwidth,*pheight,*ppage	,*pchannel		(out)�摜�̕�/����/���s/�`���l���i�t�b�^����ǂݎ�������́j�ǂݎ�莸�s����-1
��    ��    �l : ���E�����E���s���摜����ǂݎ����true��Ԃ�
�@          �\ : �o�C�i���摜�t�@�C���̃t�b�^����ǂݏo���A�t�@�C���T�C�Y�Ɣ�r����
				��r���ʂ��Ó��Ȃ�΁A���E�����E���s�E�`���l�����o�͂���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool get_file_footer(CString pathname, CString ext, short *pwidth, short *pheight, short *ppage, short *pchannel)
{
	CFile		file;
	CFileStatus status;
	int			pixsize=1, expected_filesize;
	long	footer[4]={0};


	//�t�@�C�����J��
	if( ! file.Open(pathname ,  CFile::modeRead | CFile::typeBinary ) ){
		return	false;
	}
	
	file.GetStatus(status);//�t�@�C���T�C�Y���擾����
	file.Seek(status.m_size-sizeof(footer), CFile::begin );
	file.Read(footer, sizeof(footer));//�t�b�^�����擾����
	file.Close();

	//�r�b�g�T�C�Y
	ext.MakeUpper();
	if( ext == _T("RAW") )		{	pixsize=1;		}
	else if( ext == _T("WORD") ){	pixsize=2;		}
	else if( ext == _T("SHORT") ){	pixsize=2;		}
	else if( ext == _T("DWORD") ){	pixsize=4;		}
	else if( ext == _T("LONG") ){	pixsize=4;		}
	else if( ext == _T("FLOAT") ){	pixsize=4;		}
	else if( ext == _T("COMP") ){	pixsize=4;		}
	else if( ext == _T("XY") ){		pixsize=4;		}
	else if( ext == _T("XYZ") ){	pixsize=4;		}
	else if( ext == _T("3D") ){		pixsize=1;		}
	else if( ext == _T("F3D") ){	pixsize=4;		}
	else{	
		return false;	
	}

	//�t�b�^����t�@�C���T�C�Y��\��
	expected_filesize = footer[0]*footer[1]*footer[2]*footer[3]*pixsize + sizeof(footer);
	if(expected_filesize==status.m_size){
		*pwidth = (short)footer[0];
		*pheight = (short)footer[1];
		*ppage = (short)footer[2];
		*pchannel = (short)footer[3];
		return true;	
	}else{
		*pwidth = -1;
		*pheight = -1;
		*ppage = -1;
		*pchannel = -1;
		return false;
	}

}

/********************************************************************
�@  �\  ��  �� : �摜�t�@�C����ǂݍ���
��    ��    �� : SelectAndLoadImageFile
��          �� : long		image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CString	pathname			(in)�t�@�C���̃t���p�X
				 short		width				(in)�摜�̕�
				 short		height				(in)�摜�̍���
				 short		page				(in)�摜�̉��s��
��    ��    �l : ��������� true
�@          �\ : �ۑ�����t�@�C����I�Ԃ��߂̃t�@�C���_�C�A���O���o���A
				�I�΂ꂽ�t�@�C���̊g���q����^�C�v�𔻒f���ăt�@�C�����J��

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SelectAndLoadImageFile(long image_number, CString pathname, short width, short height, short page)
{
	long		len, i;
	CString		ext, mes;
	CConfirmDlg	confirm;
	bool		need_confirm=false;
	short		channel;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	//�t�@�C��������g���q�𔲂��o��
	len = pathname.GetLength();
		for(i = len-1 ; i>=0  ; i-- ){
			if( pathname.GetAt(i) == '.' ){
				ext = pathname.Right( len-1-i ); 
				break;
			}
		}

	//�t�b�^��ǂ݂���
	get_file_footer(pathname, ext, &width, &height, &page, &channel);

	
	//�T�C�Y�w�肪�����ꍇ�́A����UI���o��
	if(width<=0 || height<=0 || page<=0){
		need_confirm = true;
		width = 640;
		height = 480;
		page = 1;
		confirm.SetWidth(50, 50);
		confirm.SetTitle("�t�@�C���T�C�Y");
		confirm.RegistVar("��", &width);
		confirm.RegistVar("����", &height);
		confirm.RegistVar("�y�[�W", &page);
	}

	//�g���q�ɂ���ē��͂���^�C�v�𔻒f����
	ext.MakeUpper();
		if( ext == _T("RAW") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadByteImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("BMP") ){
			return	LoadBitmap( image_number, pathname);

		}else if( ext == _T("SHORT") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadShortImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("WORD") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadWordImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("LONG") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadLongImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("DWORD") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadDwordImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("FLOAT") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadFloatImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("COMP") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadComplexImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("XY") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadXYImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("XYZ") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadXYZImage( image_number , pathname , CSize(width, height) );

		}else if( ext == _T("3D") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	Load3DImage( image_number , pathname , CSize(width, height), page );

		}else if( ext == _T("F3D") ){
			if(need_confirm){
				if( confirm.DoModal() != IDOK )	return	false;
			}
			return	LoadF3DImage( image_number , pathname , CSize(width, height), page );

		}else if( ext == _T("PIM") ){
			return	LoadArchivedImage( image_number , pathname);

		}
		else if (ext == _T("IFZ")) {
			return LoadIFZ(image_number, pathname);
		}
		else if (ext == _T("BFZ")) {
			return LoadBFZ(image_number, pathname);
		}else{
			return  LoadAnyImage( image_number, pathname);
		}
}


/********************************************************************
�@  �\  ��  �� : �摜�t�@�C����ǂݍ���
��    ��    �� : LoadAnyImage
��          �� : long		image_number		(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				 CString	pathname			(in)�t�@�C���̃t���p�X
				 short		width				(in)�摜�̕�
				 short		height				(in)�摜�̍���
				 short		page				(in)�摜�̉��s��
��    ��    �l : ��������� true
�@          �\ : �ۑ�����t�@�C����I�Ԃ��߂̃t�@�C���_�C�A���O���o���A
				�I�΂ꂽ�t�@�C���̊g���q����^�C�v�𔻒f���ăt�@�C�����J��

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
			 Y.Ikeda         OpenCV���p�ihttp://opencv.jp/opencv-2svn/cpp/highgui_reading_and_writing_images_and_video.html�j
********************************************************************/
bool CPimpomAPI::LoadAnyImage(long image_number, CString pathname)
{
	cv::Mat img = cv::imread((LPCTSTR)pathname, -1);

	if (!ConvertFromCvImage2(image_number, &img))
	{
		MessageBox("%s �t�@�C�����J���܂���", pathname.GetBuffer()); 
		pathname.ReleaseBuffer();
		return false;
	}


	///�ǂݍ��񂾃t�@�C���̃p�X���c��
	CDataUnit *p_du = GetDataUnit(image_number);
	p_du->DataName = ::PathFindFileNameA(pathname);
	p_du->SrcFilePath = pathname;

	DrawImage();


	SetFileIOLog(pathname);

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�ۑ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �\���摜(8bit)���t�@�C���ɕۑ�����
��    ��    �� : SaveViewAsRaw
��          �� : long		image_number(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveViewAsRaw(long image_number, CString	pathname)
{
	CFile		file;
	CDataUnit	*p_du;
	CSize	size;
	long	footer[4]={0};

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	p_du = GetDataUnit( image_number );//�w�肳�ꂽ�摜���������擾����
		if(p_du == NULL)	return	false;//�w�肳�ꂽ�摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�
		if(p_du->pDispBuffer == NULL)	return	false;//8bit�\���摜���Ȃ���΂Ȃɂ����Ȃ�
	
	size = p_du->DataSize;

		if( ! file.Open(pathname , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //�t�@�C�����J��

	file.Write(p_du->pDispBuffer , p_du->DispBufferRow * size.cy );//�f�[�^����������

	//�t�b�^����������
	footer[0]=p_du->DispBufferRow;
	footer[1]=size.cy;
	footer[2]=1;
	footer[3]=1;
	file.Write(footer, sizeof(footer) );

	file.Close();//�t�@�C�������

	SetFileIOLog(pathname);//�t�@�C�����̃��O���c��

	return	true;
}




/********************************************************************
�@  �\  ��  �� : 8bit�^�̃f�[�^���t�@�C���ɕۑ�����
��    ��    �� : SaveByteImage
��          �� : BYTE		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�̃T�C�Y
				long		page		(in)�摜�̉��s����
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveByteImage(BYTE *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//�t�@�C�����̃��O���c��
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 16bit�����t���^�̃f�[�^���t�@�C���ɕۑ�����
��    ��    �� : SaveShortImage
��          �� : short		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�̃T�C�Y
				long		page		(in)�摜�̉��s����
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveShortImage(short *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//�t�@�C�����̃��O���c��
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 16bit�^�̃f�[�^���t�@�C���ɕۑ�����
��    ��    �� : SaveWordImage
��          �� : WORD		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�̃T�C�Y
				long		page		(in)�摜�̉��s����
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveWordImage(WORD *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//�t�@�C�����̃��O���c��
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 32bit�������^�̃f�[�^���t�@�C���ɕۑ�����
��    ��    �� : SaveLongImage
��          �� : long		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�̃T�C�Y
				long		page		(in)�摜�̉��s����
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveLongImage(long *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//�t�@�C�����̃��O���c��
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : 32bit�^�̃f�[�^���t�@�C���ɕۑ�����
��    ��    �� : SaveDWORDImage
��          �� : DWORD		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�̃T�C�Y
				long		page		(in)�摜�̉��s����
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveDwordImage(DWORD *p_data , CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//�t�@�C�����̃��O���c��
	}
	return ret;
}

/********************************************************************
�@  �\  ��  �� : float(32���������_)�^�̃f�[�^���t�@�C���ɕۑ�����
��    ��    �� : SaveFloatImage
��          �� : float		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CString		pathname	(in)�t�@�C���̃t���p�X
				CSize		size		(in)�摜�̃T�C�Y
				long		page		(in)�摜�̉��s����
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveFloatImage(float *p_data, CString	pathname, CSize size, long page)
{
	bool ret = save_image_template(p_data, pathname, size, page,1);
	if(ret){
		SetFileIOLog(pathname);//�t�@�C�����̃��O���c��
	}
	return ret;
}


/********************************************************************
�@  �\  ��  �� : �摜�f�[�^���t�@�C����ۑ�����
��    ��    �� : SelectAndSaveImageFile
��          �� : long image_number	(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�̃^�C�v�ʂɃt�@�C����ۑ�����
				�ۑ�����t�@�C����I�Ԃ��߂̃t�@�C���_�C�A���O���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SelectAndSaveImageFile(long image_number)
{
	CDataUnit	*p_du;
	CString		filename, fileext, file_type_name;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	p_du = GetDataUnit( image_number );//���ݑI�𒆂̉摜���������擾����
		if(p_du == NULL)	return	false;//���ݑI�𒆂̉摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�

	//�t�@�C���̎�ނ����߂�
		switch( p_du->DataType ){
			case BYTE_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_RAW);//*.raw
				fileext = _T("raw");
				break;

			case FLOAT_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_FLOAT);//*.float
				fileext = _T("float");
				break;

			case COMPLEX_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_COMPLEX);//*.comp
				fileext = _T("comp");
				break;

			case XY_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_XY);//*.xy
				fileext = _T("xy");
				break;

			case XYZ_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_XYZ);//*.xyz
				fileext = _T("xyz");
				break;

			case THREE_D_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_3D);//*.3d
				fileext = _T("3d");
				break;

			case FLOAT_3D_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_F3D);//*.f3d
				fileext = _T("f3d");
				break;

			case RGB_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_RAW);//*.bmp
				fileext = _T("bmp");
				break;

			case SHORT16_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_SHORT);//*.short
				fileext = _T("short");
				break;

			case WORD_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_WORD);//*.word
				fileext = _T("word");
				break;

			case LONG32_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_LONG);//*.long
				fileext = _T("long");
				break;

			case DWORD_FORMAT:
				file_type_name.LoadString(IDS_FILETYPE_DWORD);//*.dword
				fileext = _T("dword");
				break;

			//case RGB_3D_FORMAT:
			//	file_type_name.LoadString(IDS_FILETYPE_BITMAP);//*.rgb3d
			//	fileext = _T("rgb3d");
			//	break;

			default:
				return false;
		}

	CFileDialog myDLG(FALSE, fileext, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, file_type_name);
		if( myDLG.DoModal() != IDOK )	return	false;

	filename = myDLG.GetPathName();
	return	SelectAndSaveImageFile( image_number, filename );
}


/********************************************************************
�@  �\  ��  �� : �摜�f�[�^���t�@�C����ۑ�����
��    ��    �� : SelectAndSaveImageFile
��          �� : long		image_number	(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		filename		(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : �摜�f�[�^�̃^�C�v�ʂɃt�@�C����ۑ�����
				�ۑ�����t�@�C����I�Ԃ��߂̃t�@�C���_�C�A���O���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SelectAndSaveImageFile(long image_number, CString pathname)
{
	CDataUnit	*p_du;
	CSize	size;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	p_du = GetDataUnit( image_number );//�w�肳�ꂽ�摜���������擾����
		if(p_du == NULL)	return	false;//�w�肳�ꂽ�摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�

	if( p_du->DataType!= RGB_FORMAT && p_du->DataType!= RGB_3D_FORMAT )//�O���[
	{
		if( p_du->pByteData ){
				return	SaveByteImage( p_du->pByteData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pShortData ){
				return	SaveShortImage( p_du->pShortData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pWordData ){
				return	SaveWordImage( p_du->pWordData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pLongData ){
				return	SaveLongImage( p_du->pLongData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pDwordData ){
				return	SaveDwordImage( p_du->pDwordData , pathname, p_du->DataSize, p_du->PageNumber );

		}else if( p_du->pFloatData ){
				return	SaveFloatImage( p_du->pFloatData , pathname, p_du->DataSize, p_du->PageNumber );
		}else{
			return false;
		}
	}else{//RGB
		SetDispRange(image_number,0,256);
		return SaveViewAsBitmap(image_number,pathname);
	}
}


//�t�@�C���p�X����f�B���N�g���p�X�𔲂��o��
CString dirpathFromFilepath(CString path)
{
	while (path.GetLength()>0)
	{
		if (PathIsDirectoryA(path))
		{
			return path;
		}
		else
		{
			int i = path.ReverseFind('\\');
			if (i > 0)
			{
				path = path.Left(i);
			}
			else
			{
				return "";
			}
		}
	}

	return "";
}

/********************************************************************
�@  �\  ��  �� : �ǂݍ��ރt�@�C����I�Ԃ��߂̃t�@�C���_�C�A���O���o��
��    ��    �� : OpenFileDialog
��          �� : int		file_number_max	(in)�t�@�C���ő吔
				 CString	file_kinds,		(in)�t�@�C���̎��
				CString		*pfile_path,	(out)�t�@�C���܂ł̃p�X
				CString		pfile_names[],	(out)�t�@�C�����i�����Ń\�[�g�j
				int			*pfile_number	(out)�t�@�C����
				 char		def_path[]		(in)�ŏ��ɊJ���f�B���N�g���p�X�ւ̃|�C���^(NULL�Ȃ�΃J�����g�f�B���N�g��������)
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::OpenFileDialog(int		file_number_max, //�t�@�C���ő吔(IN)
						 CString	file_kinds,		//�t�@�C���̎�ށi�t�@�C���_�C�A���O�Ăяo�����Ɏg�p�j(IN)
						 CString	*pfile_path,	//�p�X(OUT)
						 CString	pfile_names[],	//�t�@�C����(OUT)
						 int		*pfile_number,	//�t�@�C����(OUT)
						 char		def_path[]		//�ŏ��ɊJ���f�B���N�g���p�X
						)
{
	short			n, i;
	char			name[ MULTI_FILE_PATH_BUFFER ];
	POSITION		pos;
	CString			path_name, file_name, path, title, ext, tmp;
	CFileDialog		myDLG(true , NULL , NULL , OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT ,//�t�@�C���̕������I�����\�ɂ���
							file_kinds);	
	TCHAR			t_def_path[MAX_PATH];

		if(file_number_max <=0)		return false;

	//�t�@�C���_�C�A���O��\������
	name[0] = '\0';
	myDLG.m_ofn.lpstrFile = name;
	myDLG.m_ofn.nMaxFile  = MULTI_FILE_PATH_BUFFER-1;

	if(def_path){
		CString dirpath = dirpathFromFilepath(CString(def_path));
		_tcscpy( t_def_path, dirpath);
		myDLG.m_ofn.lpstrInitialDir = t_def_path;
	}

		if( myDLG.DoModal() != IDOK )	return false;	

	//�t�@�C���_�C�A���O����ǂݍ��ރt�@�C���̃f�[�^���擾����
	title = myDLG.GetFileTitle();//�t�@�C���̃^�C�g�����擾����
	
		if( !title.IsEmpty() ){//�P�t�@�C���̂ݑI����
			ext = myDLG.GetFileExt();//�t�@�C���̊g���q���擾����
			path_name = myDLG.GetPathName();
			file_name = path_name.Right(path_name.GetLength() - path_name.ReverseFind('\\') -1);//myDLG.GetFileName();//�t�@�C�������擾����
			path = path_name.Left( path_name.GetLength()  -  file_name.GetLength() );//�t�@�C���̃p�X���擾����

			*pfile_path   = path;
			*pfile_names  = file_name;
			*pfile_number = 1;

		}else{//�����̃t�@�C�����I�����ꂽ�Ƃ��́C�S�Ẵt�@�C������ǂݍ��ރt�@�C���̃��X�g�ɒǉ�����
			pos	= myDLG.GetStartPosition();
			n = 0;

				while( pos != NULL  &&  n < file_number_max ){
					path_name = myDLG.GetNextPathName(pos);
					path	 = myDLG.GetPathName() + _T("\\");
					file_name = path_name.Right( path_name.GetLength() - path.GetLength());

					pfile_names[n] = file_name;

					n ++;

					//�t�@�C�������A���t�@�x�b�g���Ƀ\�[�g
					for(i=n-1 ; i>0 ; i-- ){
						if(  strcmp(pfile_names[i-1], pfile_names[i]) > 0 ){
							tmp = pfile_names[i-1];
							pfile_names[i-1] = pfile_names[i];
							pfile_names[i]=tmp;
						}
					}
				}
			*pfile_path = path;
			*pfile_number = n;
		}

	return true;
}

/********************************************************************
�@  �\  ��  �� : �ǂݍ��ރt�@�C����I�Ԃ��߂̃_�C�A���O���o��
��    ��    �� : OpenFileDialog
��          �� : int		file_number_max	(in)�t�@�C���ő吔
				CString		file_kind_ext	(in)�t�@�C���̊g���q
				CString		file_kind_disp	(in)�t�@�C���̎�ސ���
				CString		*pfile_path,	(out)�t�@�C���܂ł̃p�X
				CString		pfile_names[],	(out)�t�@�C�����i�����Ń\�[�g�j
				int			*pfile_number	(out)�t�@�C����
				 char		def_path[]		(in)�ŏ��ɊJ���f�B���N�g���p�X�ւ̃|�C���^(NULL�Ȃ�΃J�����g�f�B���N�g��������)
��    ��    �l : ��������� true
�@          �\ : �t�@�C���̊g���q�͂P��ނ̂�

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::OpenFileDialog(int		file_number_max,	//�t�@�C���ő吔(IN)
									CString	file_kind_ext,		//�t�@�C���̊g���q(IN)
									CString	file_kind_disp,		//�t�@�C���̎�ސ���(IN)
									CString	*pfile_path,		//�p�X(OUT)
									CString	pfile_names[],		//�t�@�C����(OUT)
									int		*pfile_number,		//�t�@�C����(OUT)
									 char		def_path[]	//�ŏ��ɊJ���f�B���N�g���p�X
									)
{
	CString file_kind;

	file_kind = file_kind_disp + _T("(*.") + file_kind_ext + _T(")|*.") + file_kind_ext.MakeUpper() + _T("|���ׂẴt�@�C��(*.*)|*.*||");

	return OpenFileDialog(file_number_max, file_kind, pfile_path, pfile_names, pfile_number, def_path);
}



/********************************************************************
�@  �\  ��  �� : �������ރt�@�C����I�Ԃ��߂̃t�@�C���_�C�A���O���o��
��    ��    �� : SaveFileDialog
��          �� : 
                 CString	file_kinds    (IN)�t�@�C���̎�ށi�t�@�C���_�C�A���O�Ăяo�����Ɏg�p�j
				 CString	*pfile_path,  (IN/OUT)�p�X
��    ��    �l : ��������� true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveFileDialog(
			CString	file_kinds,		//�t�@�C���̎�ށi�t�@�C���_�C�A���O�Ăяo�����Ɏg�p�j(IN)
			CString	*pfile_path		//�p�X(IN/OUT)
)
{
	CFileDialog myDLG(false, _T("pim"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, file_kinds);
	TCHAR			t_def_path[MAX_PATH];

	if (pfile_path) {
		CString dirpath = dirpathFromFilepath(*pfile_path);
		_tcscpy(t_def_path, dirpath);
		myDLG.m_ofn.lpstrInitialDir = t_def_path;
	}

	if (myDLG.DoModal() != IDOK)  return false;
	*pfile_path = myDLG.GetPathName();

	return true;
}


/********************************************************************
�@  �\  ��  �� : PIMPOM.exe�̃t�@�C���p�X���擾����
��    ��    �� : GetDefaultPath
��          �� : 
��    ��    �l : PIMPOM.exe�̃t�@�C���p�X
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CString CPimpomAPI::GetDefaultPath()
{
	static CString strAppPath;
	if(strAppPath.IsEmpty())
	{
		// �A�v���P�[�V�����p�X���擾
		if(GetModuleFileName(NULL, strAppPath.GetBuffer(_MAX_PATH), _MAX_PATH))
		{
			strAppPath.ReleaseBuffer();
			char szDrive[_MAX_PATH]; // D:
			char szDir[_MAX_PATH]; // \\folders\\ 
			char szFName[_MAX_PATH]; // filename
			char szExt[_MAX_PATH]; // .ext

			// �t���p�X�𕪉�
			_tsplitpath(strAppPath, szDrive, szDir, szFName, szExt);

			// �p�X���쐬
			strAppPath.Format(_T("%s%s"), szDrive, szDir); 
		} 
		else		
		{// �G���[�̏ꍇ�͋��CString��Ԃ�
			strAppPath.ReleaseBuffer();
			strAppPath.Empty();
		}
	}
	return strAppPath;
}


/********************************************************************
�@  �\  ��  �� : �t�H���_��I�ԃ_�C�A���O���o��
��    ��    �� : GetDefaultPath
��          �� : 
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::FolderDialog(CString *pfolder)
{
	*pfolder = BrowseForFolder(*pfolder, "");

	if (*pfolder == ""){
		return false;
	}

	return true;
}

/********************************************************************
�@  �\  ��  �� : �t�H���_�����̃t�@�C���ꗗ���쐬����
��    ��    �� : GetDefaultPath
��          �� :
��    ��    �l : �t�@�C������Ԃ�
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
int	CPimpomAPI::GetFileList(
					CString		folderpath,			//(in)�t�H���_�p�X
					CString		ext,				//(in)�t�@�C���g���q
					int			file_number_max,	//(in)�t�@�C�������
					CString		pfile_names[]		//(out)�t�@�C�����ꗗ
					)
{
	CFileFind    finder;
	BOOL         bWorking = finder.FindFile(folderpath + _T("\\*.") + ext);
	int cnt = 0;

	//find all *.bmp file names and add them to the list
	while (bWorking && cnt<file_number_max)
	{
		bWorking = finder.FindNextFile();//find a file name

		if (finder.IsDots()) continue;
		if (finder.IsDirectory())continue;

		CString filename = finder.GetFileName();

		if (filename.Right(ext.GetLength()) == ext)
		{
			pfile_names[cnt] = filename;
			cnt++;
		}	
	}


	return cnt;
}