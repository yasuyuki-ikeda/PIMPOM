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

//PIMPOM�̃A�v�����s
/*******************************************************************/

#include "stdafx.h"
#include "PIMPOM.h"

#include "resource_core.h"
#include "PIMPOM_API.h"
#include "PIMPOMDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CPIMPOM_EXEApp

BEGIN_MESSAGE_MAP(CPIMPOM_EXEApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPIMPOM_EXEApp �R���X�g���N�V����

CPIMPOM_EXEApp::CPIMPOM_EXEApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� CPIMPOM_EXEApp �I�u�W�F�N�g�ł��B

CPIMPOM_EXEApp theApp;


// CPIMPOM_EXEApp ������

BOOL CPIMPOM_EXEApp::InitInstance()
{
	CWinApp::InitInstance();

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	//SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));



	// INI�t�@�C���̃p�X����ύX����
	if( m_pszProfileName ) {
		delete ((void*)m_pszProfileName);
		m_pszProfileName = new char[MAX_PATH];
		if( !m_pszProfileName ) {
			AfxMessageBox("�������s���G���[�ł��B");
			return FALSE;
		}

		CString comm = CString( theApp.m_lpCmdLine );
		if(!comm.IsEmpty()){
			strcpy((LPTSTR)m_pszProfileName, comm);//�R�}���h���C����������INI�t�@�C�����w�肷��
		}else{
			strcpy((LPTSTR)m_pszProfileName, CPimpomAPI::GetDefaultPath());
			strcat((LPTSTR)m_pszProfileName, "\\pimpom.ini");
		}
	}


	//���C���E�B���h�E�̋N��
	CPIMPOMDlg	dlg;
	m_pMainWnd = &dlg;
	dlg.cmdLine = m_lpCmdLine;
	dlg.DoModal();


	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	//  �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}


int CPIMPOM_EXEApp::ExitInstance()
{
	if( m_pszProfileName ) {
		delete ((void*)m_pszProfileName);
		m_pszProfileName = NULL; 
	}

	return CWinApp::ExitInstance();
}

