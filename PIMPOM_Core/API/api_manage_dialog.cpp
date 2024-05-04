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
//�_�C�A���O���\�[�X�Ƃ̃f�[�^�̂����
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


///////////////////////////////////////////////////////////////////////////////////
//�_�C�A���O���\�[�X����l�����Ƃ肷��		
///////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�A�C�e���̗̈���擾����
��    ��    �� : GetEditValue
��          �� : HWND			h_wnd		(in)�_�C�A���O�A�C�e�������݂���E�B���h�E�̃n���h��
                 UINT			id			(in)�_�C�A���O�A�C�e���̃��\�[�X�h�c
                 RECT			*pRct		(out)�擾�����̈�i�N���C�A���g���W�j
��    ��    �l : ����������true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetDialogItemRect(HWND h_wnd, UINT id, CRect *pRct)
{
	HWND		h_item;
	CRect		rgn;
	CPoint		pnt;

	h_item = (HWND)::GetDlgItem(h_wnd, id);//�_�C�A���O�A�C�e�����̃|�C���^���擾����
	if (h_item == NULL)		return	false;//�|�C���^�擾�Ɏ��s

	
	::GetWindowRect(h_item, &rgn);

	pnt.x = rgn.left;
	pnt.y = rgn.top;
	::ScreenToClient(h_wnd, &pnt);

	pRct->left = pnt.x;
	pRct->top = pnt.y;
	pRct->right = rgn.right - rgn.left + pnt.x;
	pRct->bottom = rgn.bottom - rgn.top + pnt.y;

	return true;
}


/********************************************************************
�@  �\  ��  �� : �_�C�A���O�A�C�e���̗̈��ݒ肷��
��    ��    �� : GetEditValue
��          �� : HWND			h_wnd		(in)�_�C�A���O�A�C�e�������݂���E�B���h�E�̃n���h��
                 UINT			id			(in)�_�C�A���O�A�C�e���̃��\�[�X�h�c
                 RECT			*pRct		(in)�ݒ肷��̈�i�N���C�A���g���W�j
��    ��    �l : ����������true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SetDialogItemRect(HWND h_wnd, UINT id, CRect Rct)
{
	HWND		h_item;
	CPoint		pnt;

	h_item = (HWND)::GetDlgItem(h_wnd, id);//�_�C�A���O�A�C�e�����̃|�C���^���擾����
	if (h_item == NULL)		return	false;//�|�C���^�擾�Ɏ��s

	pnt.x = Rct.left;
	pnt.y = Rct.top;
	::ClientToScreen(h_wnd, &pnt);

	::MoveWindow(h_item, pnt.x, pnt.y, Rct.Width(), Rct.Height(),true);

	return true;
}



/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g�{�b�N�X���琔�l���擾����(double)
��    ��    �� : GetEditValue
��          �� : HWND			h_wnd		(in)�G�f�B�b�g�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�G�f�B�b�g�{�b�N�X�̃��\�[�X�h�c
				 double			*value		(out)�擾�������l
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , double *value)
{
	char		str[100];
	long		len;
	HWND		h_edit; 
	
	h_edit = (HWND)::GetDlgItem( h_wnd , id );//�G�f�B�b�g�{�b�N�X�̃|�C���^���擾����
		if(h_edit == NULL)		return	false;//�|�C���^�擾�Ɏ��s

	len = ::GetWindowText(h_edit, str, 100);//�G�f�B�b�g�{�b�N�X���̕�������擾����
		if(len == 0)	return	false;//�G�f�B�b�g�{�b�N�X�ɂȂɂ������ĂȂ��ꍇ�͒l���������Ȃ�

	return ToNumber(str, value);

}

/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g�{�b�N�X���琔�l���擾����(float)
��    ��    �� : GetEditValue
��          �� : HWND			h_wnd		(in)�G�f�B�b�g�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�G�f�B�b�g�{�b�N�X�̃��\�[�X�h�c
				 float			*value		(out)�擾�������l
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , float *value)
{
	double  tmp;
		if( !GetEditValue(h_wnd, id, &tmp) )	return false;
	*value = (float)tmp;
	return true;
}

/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g�{�b�N�X���琔�l���擾����(short)
��    ��    �� : GetEditValue
��          �� : HWND			h_wnd		(in)�G�f�B�b�g�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�G�f�B�b�g�{�b�N�X�̃��\�[�X�h�c
				 short			*value		(out)�擾�������l
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , short *value)
{
	double  tmp;
		if( !GetEditValue(h_wnd, id, &tmp) )	return false;
	*value = (short)tmp;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g�{�b�N�X���琔�l���擾����(int)
��    ��    �� : GetEditValue
��          �� : HWND			h_wnd		(in)�G�f�B�b�g�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�G�f�B�b�g�{�b�N�X�̃��\�[�X�h�c
				 int			*value		(out)�擾�������l
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , int *value)
{
	double  tmp;
		if( !GetEditValue(h_wnd, id, &tmp) )	return false;
	*value = (int)tmp;
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g�{�b�N�X����f�[�^(CString)�����o��
��    ��    �� : GetEditValue
��          �� : HWND			h_wnd		(in)�G�f�B�b�g�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�G�f�B�b�g�{�b�N�X�̃��\�[�X�h�c
				 CString		*str		(out)�擾����������
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetEditValue(HWND h_wnd, UINT id , CString *str)
{
	HWND		h_edit; 
	CWnd		*p_wnd;
	
	h_edit = (HWND)::GetDlgItem( h_wnd , id );//�G�f�B�b�g�{�b�N�X�̃|�C���^���擾����
		if(h_edit == NULL)		return	false;//�|�C���^�擾�Ɏ��s

	p_wnd = CWnd::FromHandle( h_edit );
	p_wnd->GetWindowText(*str);//�G�f�B�b�g�{�b�N�X���̕�������擾����
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �`�F�b�N�{�b�N�X����`�F�b�N��Ԃ��擾����
��    ��    �� : GetCheck
��          �� : HWND			h_wnd		(in)�`�F�b�N�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�`�F�b�N�{�b�N�X�̃��\�[�X�h�c
��    ��    �l : �`�F�b�N����Ă�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetCheck(HWND h_wnd, UINT id )
{
	HWND	h_button;
	CButton	*p_button;

	h_button = (HWND)::GetDlgItem( h_wnd, id );//�{�^���̃|�C���^���擾����
		if(h_button == NULL)		return false;

	p_button = (CButton*)CWnd::FromHandle(h_button);
	return	(p_button->GetCheck() == 1) ? true:false;
}

/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g�{�b�N�X�ɐ��l��ݒ肷��
��    ��    �� : SetEditValue
��          �� : HWND			h_wnd		(in)�G�f�B�b�g�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�G�f�B�b�g�{�b�N�X�̃��\�[�X�h�c
				 double			value		(in)�ݒ肷�鐔�l
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SetEditValue(HWND h_wnd, UINT id , double value)
{
	int i, len;
	char		str[100];
	HWND		h_edit;
	
	h_edit = (HWND)::GetDlgItem( h_wnd, id );//�G�f�B�b�g�{�b�N�X�̃|�C���^���擾����
		if(h_edit == NULL)		return false;

	len = sprintf(str, "%f", value);
		for( i = len - 1  ;  i>0 ; i--){//������̉E�[��0������
			if( str[i] != '0' ){
				str[i+1] = '\0';
				break;
			}
		}
	
	::SetWindowText(h_edit, str);//�G�f�B�b�g�{�b�N�X���̕������ݒ肷��
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g�{�b�N�X�Ƀf�[�^(CString)��ݒ肷��
��    ��    �� : SetEditValue
��          �� : HWND			h_wnd		(in)�G�f�B�b�g�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�G�f�B�b�g�{�b�N�X�̃��\�[�X�h�c
				 CString		str			(in)�ݒ肷�镶����
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SetEditValue(HWND h_wnd, UINT id , CString str)
{
	HWND		h_edit;
	
	h_edit = (HWND)::GetDlgItem( h_wnd, id );//�G�f�B�b�g�{�b�N�X�̃|�C���^���擾����
		if(h_edit == NULL)		return false;

	::SetWindowText(h_edit, str);//�G�f�B�b�g�{�b�N�X���̕������ݒ肷��
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �`�F�b�N�{�b�N�X�Ƀ`�F�b�N�̏�Ԃ�ݒ肷��
��    ��    �� : SetCheck
��          �� : HWND			h_wnd		(in)�`�F�b�N�{�b�N�X�����݂���E�B���h�E�̃n���h��
				 UINT			id			(in)�`�F�b�N�{�b�N�X�̃��\�[�X�h�c
				 bool			value		(in)�`�F�b�N�̏�ԁi�`�F�b�N����Ă����true�j
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SetCheck(HWND h_wnd, UINT id ,bool value)
{
	HWND	h_button;
	CButton	*p_button;

	h_button = (HWND)::GetDlgItem( h_wnd, id );//�{�^���̃|�C���^���擾����
		if(h_button == NULL)		return false;

	p_button = (CButton*)CWnd::FromHandle(h_button);
	p_button->SetCheck(value);
	return	true;
}



///////////////////////////////////////////////////////////////////////////////////
//���b�Z�[�W�\���p�̃_�C�A���O���o��
///////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : ���b�Z�[�W�\���p�̃_�C�A���O���o��
��    ��    �� : MessageBox
��          �� : char *format, ...		(in)�\�������t�H�[�}�b�g
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void		CPimpomAPI::MessageBox(char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	AfxMessageBox(CString(buf));
}




///////////////////////////////////////////////////////////////////////////////////
//PIMPOM�W���̃��[�h���X�_�C�A���O������
///////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : �W���̃��[�h���X�_�C�A���O�̐���
��    ��    �� : DialogOpen
��          �� : int	id		(in)�_�C�A���O�̃��\�[�XID
				 bool	show	(in)�\���^��\��
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
			 Y.Ikeda         CMultiImageDispDlg�ǉ�
********************************************************************/
bool	CPimpomAPI::DialogOpen(int id, bool show)
{
	CDialog *pdlg=NULL;

	switch(id){
		case IDD_DATA_DLG:
			if(!pDataDlg){
				pDataDlg = new CDataDlg();
				if(pDataDlg)	pDataDlg->Create(IDD_DATA_DLG, NULL);
			}
			pdlg = (CDialog*)pDataDlg;
			break;

		case IDD_HISTGRAM_DLG:
			if(!pHistgramDlg){
				pHistgramDlg = new CHistgramDlg();
				if(pHistgramDlg)	pHistgramDlg->Create(IDD_HISTGRAM_DLG, NULL);
			}
			pdlg = (CDialog*)pHistgramDlg;
			break;


		case IDD_FILTER_CTRL_DLG:
			if(!pFlterCtrlDlg){
				pFlterCtrlDlg = new CFilterCtrlDlg();
				if(pFlterCtrlDlg)	pFlterCtrlDlg->Create(IDD_FILTER_CTRL_DLG, NULL);
			}
			pdlg = (CDialog*)pFlterCtrlDlg;
			break;

		case IDD_CALC_DLG:
			if(!pCalcDlg){
				pCalcDlg = new CCalcDlg();
				if(pCalcDlg)	pCalcDlg->Create(IDD_CALC_DLG, NULL);
			}
			pdlg = (CDialog*)pCalcDlg;
			break;

		case IDD_BINALIZE_DLG:
			if(!pBinalizeDlg){
				pBinalizeDlg = new CBinalizeDlg();
				if(pBinalizeDlg)	pBinalizeDlg->Create(IDD_BINALIZE_DLG, NULL);
			}
			pdlg = (CDialog*)pBinalizeDlg;
			break;

		case IDD_BATCH_DLG:
			if(!pBatchDlg){
				pBatchDlg = new CBatchDlg();
				if(pBatchDlg)	pBatchDlg->Create(IDD_BATCH_DLG, NULL);
			}
			pdlg = (CDialog*)pBatchDlg;
			break;

		case IDD_MULTI_IMAGE_DISP_DLG:
			if(!pMultiImageDispDlg){
				pMultiImageDispDlg = new CMultiImageDispDlg();
				if(pMultiImageDispDlg)	pMultiImageDispDlg->Create(IDD_MULTI_IMAGE_DISP_DLG, NULL);
			}
			pdlg = (CDialog*)pMultiImageDispDlg;
			break;

		case IDD_3D_IMAGE_DISP_DLG:
			if (!p3DImgDispDlg){
				p3DImgDispDlg = new C3DImgDispDlg();
				if (p3DImgDispDlg)	p3DImgDispDlg->Create(IDD_3D_IMAGE_DISP_DLG, NULL);
			}
			pdlg = (CDialog*)p3DImgDispDlg;
			break;

		case IDD_WORKAREA_DLG:
			if(!pWorkAreaDlg){
				pWorkAreaDlg = new CWorkAreaDlg();
				if(pWorkAreaDlg)	pWorkAreaDlg->Create(IDD_WORKAREA_DLG, NULL);
			}
			pdlg = (CDialog*)pWorkAreaDlg;
			break;

		case IDD_MARK_DLG:
			if(!pMarkDlg){
				pMarkDlg = new CMarkDlg();
				if(pMarkDlg)	pMarkDlg->Create(IDD_MARK_DLG, NULL);
			}
			pdlg = (CDialog*)pMarkDlg;
			break;

		case IDD_DATAPICK_DLG:
			if(!pDataPickDlg){
				pDataPickDlg = new CDataPickDlg();
				if(pDataPickDlg)	pDataPickDlg->Create(IDD_DATAPICK_DLG, NULL);
			}
			pdlg = (CDialog*)pDataPickDlg;
			break;

		case IDD_SOCKET_CTRL_DLG:
			if(!pSocketCtrlDlg){
				pSocketCtrlDlg = new CSocketCtrlDlg();
				if(pSocketCtrlDlg)	pSocketCtrlDlg->Create(IDD_SOCKET_CTRL_DLG, NULL);
			}
			pdlg = (CDialog*)pSocketCtrlDlg;
			break;

		case IDD_CHART_DLG:
			if(!pChartDlg){
				pChartDlg = new CChartDlg();
				if(pChartDlg)	pChartDlg->Create(IDD_CHART_DLG, NULL);
			}
			pdlg = (CDialog*)pChartDlg;
			break;

		case IDD_COMMAND_RPOMPT_DLG:
			if(!pCmdPrptDlg){
				pCmdPrptDlg = new CCommandPromptDlg();
				if(pCmdPrptDlg)	pCmdPrptDlg->Create(IDD_COMMAND_RPOMPT_DLG, NULL);
			}
			pdlg = (CDialog*)pCmdPrptDlg;
			break;

		case IDD_PYTHON_BATCH_DLG:
			if (!pPythonBatchDlg) {
				pPythonBatchDlg = new CPythonBatchDlg();
				if (pPythonBatchDlg)	pPythonBatchDlg->Create(IDD_PYTHON_BATCH_DLG, NULL);
			}
			pdlg = (CDialog*)pPythonBatchDlg;
			break;
	}

	if(pdlg){
		if (show) {
			pdlg->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			
			pdlg->ShowWindow(SW_SHOW);
		}
		else {
			pdlg->ShowWindow(SW_HIDE);
		}

	}else{
		return false;
	}
	
	return true;
}

/********************************************************************
�@  �\  ��  �� : �W���̃��[�h���X�_�C�A���O�̔j��
��    ��    �� : DialogDelete
��          �� : int id		(in)�_�C�A���O�̃��\�[�XID
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DialogDelete(int id)
{
	CDialog *pdlg=NULL;

	switch(id){
		case IDD_DATA_DLG:
			if(pDataDlg){
				DestroyWindow(pDataDlg->m_hWnd);
				delete	pDataDlg;	pDataDlg=NULL;
			}
			break;

		case IDD_HISTGRAM_DLG:
			if(pHistgramDlg){
				DestroyWindow(pHistgramDlg->m_hWnd);
				delete	pHistgramDlg;	pHistgramDlg=NULL;
			}
			break;

		case IDD_FILTER_CTRL_DLG:
			if(pFlterCtrlDlg){
				DestroyWindow(pFlterCtrlDlg->m_hWnd);
				delete	pFlterCtrlDlg;	pFlterCtrlDlg=NULL;
			}
			break;

		case IDD_CALC_DLG:
			if(pCalcDlg){
				DestroyWindow(pCalcDlg->m_hWnd);
				delete	pCalcDlg;	pCalcDlg=NULL;
			}
			break;

		case IDD_BINALIZE_DLG:
			if(pBinalizeDlg){
				DestroyWindow(pBinalizeDlg->m_hWnd);
				delete	pBinalizeDlg;	pBinalizeDlg=NULL;
			}
			break;

		case IDD_BATCH_DLG:
			if(pBatchDlg){
				DestroyWindow(pBatchDlg->m_hWnd);
				delete	pBatchDlg;	pBatchDlg=NULL;
			}
			break;

		case IDD_MULTI_IMAGE_DISP_DLG:
			if(pMultiImageDispDlg){
				DestroyWindow(pMultiImageDispDlg->m_hWnd);
				delete	pMultiImageDispDlg;	pMultiImageDispDlg=NULL;
			}
			break;

		case IDD_3D_IMAGE_DISP_DLG:
			if (p3DImgDispDlg){
				DestroyWindow(p3DImgDispDlg->m_hWnd);
				delete	p3DImgDispDlg;	p3DImgDispDlg = NULL;
			}
			break;

		case IDD_WORKAREA_DLG:
			if(pWorkAreaDlg){
				DestroyWindow(pWorkAreaDlg->m_hWnd);
				delete	pWorkAreaDlg;	pWorkAreaDlg=NULL;
			}
			break;

		case IDD_MARK_DLG:
			if(pMarkDlg){
				DestroyWindow(pMarkDlg->m_hWnd);
				delete	pMarkDlg;	pMarkDlg=NULL;
			}
			break;

		case IDD_DATAPICK_DLG:
			if(pDataPickDlg){
				DestroyWindow(pDataPickDlg->m_hWnd);
				delete	pDataPickDlg;	pDataPickDlg=NULL;
			}
			break;

		case IDD_SOCKET_CTRL_DLG:
			if(pSocketCtrlDlg){
				DestroyWindow(pSocketCtrlDlg->m_hWnd);
				delete	pSocketCtrlDlg;	pSocketCtrlDlg=NULL;
			}
			break;

		case IDD_CHART_DLG:
			if(pChartDlg){
				DestroyWindow(pChartDlg->m_hWnd);
				delete	pChartDlg;	pChartDlg=NULL;
			}
			break;

		case IDD_COMMAND_RPOMPT_DLG:
			if(pCmdPrptDlg){
				DestroyWindow(pCmdPrptDlg->m_hWnd);
				delete	pCmdPrptDlg;	pCmdPrptDlg=NULL;
			}
			break;

		case IDD_PYTHON_BATCH_DLG:
			if (pPythonBatchDlg) {
				DestroyWindow(pPythonBatchDlg->m_hWnd);
				delete	pPythonBatchDlg;	pPythonBatchDlg = NULL;
			}
			break;
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
//���C���E�B���h�E�Ƀ��b�Z�[�W��\������
//////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : ���C���E�B���h�E�Ƀ��b�Z�[�W��\������
��    ��    �� : ShowMessage
��          �� : CString str			(in)�\������
				bool do_clear			(in)�ߋ��̃��b�Z�[�W���N���A���邩�ǂ���
				int num					(in)���b�Z�[�W�̃^�u�ԍ�
				bool force				(in)�����\���i���̃^�u���J���Ă��Ă��؂�ւ���j
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::ShowMessage(bool do_clear, int num, bool force, CString str)
{
	pPIMMOMDlg->ShowMessage( do_clear , num, force, str );
}

/********************************************************************
�@  �\  ��  �� : ���C���E�B���h�E�Ƀ��b�Z�[�W��\������
��    ��    �� : ShowMessage
��          �� : char *format, ...		(in)�\�������t�H�[�}�b�g
				bool do_clear			(in)�ߋ��̃��b�Z�[�W���N���A���邩�ǂ���
				int num					(in)���b�Z�[�W�̃^�u�ԍ�
				bool force				(in)�����\���i���̃^�u���J���Ă��Ă��؂�ւ���j
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::ShowMessage(bool do_clear,int num, bool force, char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	pPIMMOMDlg->ShowMessage( do_clear, num, force ,CString(buf));
}

void	CPimpomAPI::ShowMessage(bool do_clear, char *format, ...)//���b�Z�[�W1�ɋ����\��
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	pPIMMOMDlg->ShowMessage( do_clear, 1,true ,CString(buf));
}

/********************************************************************
�@  �\  ��  �� : �}�[�N�ҏW���[�h�̏�Ԏ擾
��    ��    �� : GetMarkEditMode
��          �� : 
��    ��    �l : 1:�\���̂� 2:�\��+�ҏW(�_) 3:�\��+�ҏW(��) 0:�\���Ȃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CPimpomAPI::GetMarkEditMode()
{
	if (GetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK))
	{
		if (pMarkDlg)
		{
			if (pMarkDlg->GetMarkEditType() == MARK_TYPE_POINT )
			{
				return 2;
			}
			else if (pMarkDlg->GetMarkEditType() == MARK_TYPE_LINE)
			{
				return 3;
			}
			else {
				return 1;
			}
		}
		else {
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

/********************************************************************
�@  �\  ��  �� : �}�[�N�ҏW���[�h�̏�Ԑݒ�
��    ��    �� : SetMarkEditMode
��          �� :int mode  (in)1:�\���̂� 2:�\��+�ҏW(�_) 3:�\��+�ҏW(��) 0:�\���Ȃ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::SetMarkEditMode(int mode)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾
	if(p_du == NULL)		return;//�f�[�^��������΂Ȃɂ����Ȃ�

	if( mode==2 || mode == 3)//�}�[�N�̕ҏW���͂��߂�
	{
		SetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK , true);//�}�[�N�\���{�^�����`�F�b�N



		//�}�[�N�E�B���h�E�\��
		if(pMarkDlg)
		{
			if (mode == 2)
			{
				pMarkDlg->SetMarkEditType(MARK_TYPE_POINT);
			}
			else {
				pMarkDlg->SetMarkEditType(MARK_TYPE_LINE);
			}
			
			pMarkDlg->ShowWindow(SW_SHOW);
			pMarkDlg->ShowMarkVal( CURRENT_IMAGE );//�\���J�[�\����\��
		}

		ShowMessage(false, 0, true, "�N���b�N�Ń}�[�N�ǉ�");//���b�Z�[�W���o��

		//�ق��̕ҏW�{�^���̃`�F�b�N���͂���
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, false);
		if (pWorkAreaDlg) {
			pWorkAreaDlg->SetWorkAreaEditMode(false);
		}
	}
	else if (mode == 1)//�\���̂�
	{
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK, true);//�}�[�N�\���{�^�����`�F�b�N

		//�}�[�N�E�B���h�E��\��
		if (pMarkDlg) {
			pMarkDlg->SetMarkEditType(-1);
			pMarkDlg->ShowWindow(SW_SHOW);
			pMarkDlg->ShowMarkVal(CURRENT_IMAGE);//�\���J�[�\����\��
		}
	}
	else
	{//�}�[�N�\�����Ȃ���Ԃɂ���

		SetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK , false);//�}�[�N�\���{�^���̃`�F�b�N���͂���

		//�}�[�N�E�B���h�E���\��
		if(pMarkDlg){
			pMarkDlg->SetMarkEditType(-1);
			pMarkDlg->ShowWindow(SW_HIDE);
		}
	}

	DrawImage();//�ĕ`��
}



/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�ҏW���[�h�̏�Ԏ擾
��    ��    �� : GetWorkAreaEditMode
��          �� : 
��    ��    �l : ��Ɨ̈�ҏW���[�h�Ȃ��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetWorkAreaEditMode()
{
	if (pWorkAreaDlg)
	{
		if (GetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_WORK_AREA)) {
			return pWorkAreaDlg->GetWorkAreaEditMode();
		}
		else {
			return false;
		}
	}
	else 
	{
		return false;
	}
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�ҏW���[�h�̏�Ԑݒ�
��    ��    �� : SetWorkAreaEditMode
��          �� : bool onoff  (in)true:��Ɨ̈�ҏW���[�h�ɓ���	(in)false:��Ɨ̈�ҏW���[�h�𒆎~
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::SetWorkAreaEditMode(bool onoff)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾
	if(p_du == NULL)		return;//�f�[�^��������΂Ȃɂ����Ȃ�

	if( onoff )
	{//��Ɨ̈�̕ҏW������

		if( ! GetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_WORK_AREA ) )
		{//��Ɨ̈悪�g���Ă��Ȃ��ꍇ�͎g���悤�ɂ���
			SetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_WORK_AREA , true);
			p_du->DoUseWorkArea = true;
		}

		//��Ɨ̈�E�B���h�E��\��
		if(pWorkAreaDlg){
			pWorkAreaDlg->SetWorkAreaEditMode(true);
			pWorkAreaDlg->ShowWindow(SW_SHOW);
		}

		ShowMessage(false, 0, true, "�h���b�O�ō�Ɨ̈�쐬");//���b�Z�[�W���o��

		//�ق��̕ҏW�{�^���̃`�F�b�N���͂���
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, false);
		if (pMarkDlg) 
		{
			if (pMarkDlg->GetMarkEditType() == MARK_TYPE_POINT || pMarkDlg->GetMarkEditType() == MARK_TYPE_LINE) {
				pMarkDlg->SetMarkEditType(-1);
			}
		}
	}
	else {
		if (pWorkAreaDlg) {
			pWorkAreaDlg->SetWorkAreaEditMode(false);
		}
	}

	
	DrawImage();//�ĕ`��
}

/********************************************************************
�@  �\  ��  �� : 2�_�Ԃ̋�������c�[���̏�Ԏ擾
��    ��    �� : GetRulerToolMode
��          �� : 
��    ��    �l : �g�p���Ȃ��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::GetRulerToolMode()
{
	return GetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE);
}

/********************************************************************
�@  �\  ��  �� : 2�_�Ԃ̋�������c�[���̏�Ԑݒ�
��    ��    �� : SetRulerToolMode
��          �� : bool onoff  (in)true:�g�p�J�n	(in)false:�g�p���~
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::SetRulerToolMode(bool onoff)
{
	CDataUnit	*p_du = GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾
	if(p_du == NULL)		return;//�f�[�^��������΂Ȃɂ����Ȃ�

	if( GetCheck( pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE ) ){//�����̌v�Z���ł���悤�ɂ���
		//�ق��̕ҏW�{�^���̃`�F�b�N���͂���
		
		if (pWorkAreaDlg) {
			pWorkAreaDlg->SetWorkAreaEditMode(false);
		}

		if (pMarkDlg) {
			if (pMarkDlg->GetMarkEditType() == MARK_TYPE_POINT || pMarkDlg->GetMarkEditType() == MARK_TYPE_LINE) {
				pMarkDlg->SetMarkEditType(-1);
			}
		}

		ShowMessage(false, 0, true, "�h���b�O�����Q�_�̋����ƒl�̍����v�Z����");//���b�Z�[�W���o��
	}
	DrawImage();//�ĕ`��
}

/********************************************************************
�@  �\  ��  �� : �t���[�n���h�ҏW���[�h�̏�Ԏ擾
��    ��    �� : GetDataPickEditType
��          �� : 
��    ��    �l : 0:��\��  1:�s�b�N  2:�y�C���g 3:�t�B��
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
int		CPimpomAPI::GetDataPickEditType()
{
	if (pDataPickDlg)
	{
		if (pDataPickDlg->IsWindowVisible())
		{
			if (pDataPickDlg->GetDataPickMode()== DATAPICK_PICK) {
				return 1;
			}
			else if (pDataPickDlg->GetDataPickMode() == DATAPICK_PAINT) {
				return 2;
			}
			else {
				return 3;
			}
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

/********************************************************************
�@  �\  ��  �� : �t���[�n���h�ҏW���[�h�̏�Ԑݒ�
��    ��    �� : SetDataPickEditType
��          �� :
��    ��    �l : 0:��\��  1:�s�b�N  2:�y�C���g�@3:�t�B��
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::SetDataPickEditType(int mode)
{
	if (pDataPickDlg)
	{
		if (mode == 0)
		{
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT, false);
			pDataPickDlg->ShowWindow(SW_HIDE);
		}
		else if (mode == 1)
		{
			pDataPickDlg->SetDataPickMode(DATAPICK_PICK);
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT, true);
			pDataPickDlg->ShowWindow(SW_SHOW);
		}
		else if (mode == 2)
		{
			pDataPickDlg->SetDataPickMode(DATAPICK_PAINT);
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT, true);
			pDataPickDlg->ShowWindow(SW_SHOW);
		}
		else {
			pDataPickDlg->SetDataPickMode(DATAPICK_FILL);
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT, true);
			pDataPickDlg->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if (mode == 0)
		{

		}
		else if (mode == 1)
		{
			DialogOpen(IDD_DATAPICK_DLG, true);
			pDataPickDlg->SetDataPickMode(DATAPICK_PICK);
		}
		else if (mode == 2) {
			DialogOpen(IDD_DATAPICK_DLG, true);
			pDataPickDlg->SetDataPickMode(DATAPICK_PAINT);
		}
		else {
			DialogOpen(IDD_DATAPICK_DLG, true);
			pDataPickDlg->SetDataPickMode(DATAPICK_FILL);
		}
	}
}


/********************************************************************
�@  �\  ��  �� : �摜�����E�B���h�E�̏�Ԏ擾
��    ��    �� : GetCalcToolMode
��          �� :
��    ��    �l : true:�\�� false:��\��
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::GetCalcToolMode()
{
	if (pCalcDlg)
	{
		return pCalcDlg->IsWindowVisible();
	}
	else {
		return false;
	}
}

/********************************************************************
�@  �\  ��  �� : �摜�����E�B���h�E�̏�Ԑݒ�
��    ��    �� : GetCalcToolMode
��          �� : true:�\�� false:��\��
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPimpomAPI::SetCalcToolMode(bool onoff)
{
	if (pCalcDlg)
	{
		if (onoff) {
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC, true);
			pCalcDlg->ShowWindow(SW_SHOW);
		}
		else {
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC, false);
			pCalcDlg->ShowWindow(SW_HIDE);
		}
	}
	else {
		if (onoff) {
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC, true);
			DialogOpen(IDD_CALC_DLG, true);
		}
	}
}

/********************************************************************
�@  �\  ��  �� : �q�X�g�O�����E�B���h�E�̂̏�Ԏ擾
��    ��    �� : GetHistgramToolMode
��          �� :
��    ��    �l : true:�\�� false:��\��
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::GetHistgramToolMode()
{
	if (pHistgramDlg)
	{
		return pHistgramDlg->IsWindowVisible();
	}
	else {
		return false;
	}
}

/********************************************************************
�@  �\  ��  �� : �q�X�g�O�����E�B���h�E�̏�Ԑݒ�
��    ��    �� : SetHistgramToolMode
��          �� : true:�\�� false:��\��
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPimpomAPI::SetHistgramToolMode(bool onoff)
{
	if (pHistgramDlg)
	{
		pHistgramDlg->ShowWindow(onoff ? SW_SHOW : SW_HIDE);
	}
	else {
		if (onoff) {
			DialogOpen(IDD_HISTGRAM_DLG, true);
		}
	}
}

/********************************************************************
�@  �\  ��  �� : �r�f�I�L���v�`���̃X���[�\����Ԃ̎擾
��    ��    �� : GetVideoCaptureThroughMode
��          �� : 
��    ��    �l : 1:�X���[�\���� 2:�o�b�`���s���A���B�e�� 0:�X���[�\���Ȃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CPimpomAPI::GetVideoCaptureThroughMode()
{
	if( GetCheck( pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE) ){
		return 1;
	}else if(GetCheck( pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2)){
		return 2;
	}else{
		return 0;
	}
}

/********************************************************************
�@  �\  ��  �� : �r�f�I�L���v�`���̃X���[�\����Ԃ̐ݒ�
��    ��    �� : SetVideoCaptureThroughMode
��          �� : int mode  (in)1:�X���[�\�� 2:�o�b�`���s���A���B�e 0:�X���[�\���Ȃ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::SetVideoCaptureThroughMode(int mode)
{
	if(mode==1)
	{
		if(GetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2)){
			StopBGProc();
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, false);
		}

		SetCheck(pPIMMOMDlg->m_hWnd,IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE,true);
		SetBGProcID(BGPROC_CAPTURING);
	}
	else if(mode==2)
	{
		if(GetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE)){
			StopBGProc();
			SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE,false);
		}

		SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, true);
		SetBGProcID(BGPROC_CAPTURING);
	}
	else
	{
		StopBGProc();
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, false);
		SetCheck(pPIMMOMDlg->m_hWnd, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE,false);
	}
}

/********************************************************************
�@  �\  ��  �� : �\���摜���N���b�v�{�[�h�ɃR�s�[����
��    ��    �� : CopyToClipbord
��          �� : int image_num
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::CopyToClipbord(int image_num)
{
	bool ret=false;
	CDC memDC;
	CBitmap bm;
	int copySizeX, copySizeY;

	CDC* pDC = pPIMMOMDlg->GetDC();//���C���E�B���h�E�̃f�o�C�X�R���e�L�X�g�擾
	if(pDC){

		CDataUnit *pDU = GetDataUnit(image_num);
		if(pDU)
		{
			DrawImage(image_num);

			double imageRight,imageBottom;
			GetDestCoor( (double)pDU->DataSize.cx -0.5 , (double)pDU->DataSize.cy -0.5 , &imageRight, &imageBottom);

			//�\���摜�̉E�[�ŃR�s�[����̈���N���b�v
			if( imageRight > MAIN_WIN_LEFT_MARGIN  &&  imageRight < MAIN_WIN_LEFT_MARGIN + main_image_size.cx){
				copySizeX = (int)imageRight - MAIN_WIN_LEFT_MARGIN;
			}else{
				copySizeX = (int)main_image_size.cx;
			}

			//�\���摜�̉��[�ŃR�s�[����̈���N���b�v
			if( imageBottom > MAIN_WIN_TOP_MARGIN  &&  imageBottom < MAIN_WIN_TOP_MARGIN + main_image_size.cy){
				copySizeY = (int)imageBottom - MAIN_WIN_TOP_MARGIN;
			}else{
				copySizeY = (int)main_image_size.cy;
			}

			if(memDC.CreateCompatibleDC(pDC))//pDC �ƌ݊����̂��郁�����f�o�C�X�R���e�L�X�g���쐬
			{
				if(bm.CreateCompatibleBitmap(pDC, copySizeX, copySizeY))//pDC�ƌ݊����̂���r�b�g�}�b�v���쐬
				{
					CBitmap* pOldBmp = memDC.SelectObject(&bm);//�������f�o�C�X�R���e�L�X�g�Ƀr�b�g�}�b�v��I��
					if( memDC.StretchBlt(MAIN_WIN_LEFT_MARGIN , MAIN_WIN_TOP_MARGIN, copySizeX , copySizeY ,pDC, 0 , 0 , copySizeX , copySizeY ,SRCCOPY) )//��ʃC���[�W���r�b�g�}�b�v�ɓ]��
					{
						if(pPIMMOMDlg->OpenClipboard()){//�N���b�v�{�[�h���J��
							::EmptyClipboard();
							if(::SetClipboardData(CF_BITMAP, bm.GetSafeHandle())!=NULL){//�N���b�v�{�[�h�Ƀr�b�g�}�b�v�\��t��
								ret = true;
							}
							::CloseClipboard();//�N���b�v�{�[�h�����
						}
					}
					bm.Detach();
					memDC.SelectObject(pOldBmp);//�r�b�g�}�b�v���
				}
				memDC.DeleteDC();//�������f�o�C�X�R���e�L�X�g���
			}
		}
		pPIMMOMDlg->ReleaseDC( pDC );//���C���E�B���h�E�̃f�o�C�X�R���e�L�X�g���
	}

	return ret;
}




/********************************************************************
�@  �\  ��  �� : �N���b�v�{�[�h�ɂ���摜���������ɓ����
��    ��    �� : CopyFromClipbord
��          �� : int image_num
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::PasteFromClipbord(int image_num)
{
	bool ret=true;

	if (IsClipboardFormatAvailable(CF_BITMAP)) 
	{
		if( ::OpenClipboard(FindWindow(NULL,NULL)))//�N���b�v�{�[�h���J��
		{
			//DIB�n���h���̎擾 
			HANDLE hBitmap = GetClipboardData(CF_DIB); 
			if(hBitmap == NULL)		return false;

			char* szBuffer = (char *)GlobalLock(hBitmap); //DIB�擪�|�C���^�̎擾
			if(szBuffer)
			{
				BITMAPINFOHEADER bmpInfo;//DIB�w�b�_�̎擾
				memcpy( &bmpInfo, szBuffer, sizeof(BITMAPINFOHEADER) ); 
			


				//�������މ摜�������̊m��
				BYTE *pRGBData = GetRGBMemory(image_num, CSize(bmpInfo.biWidth, bmpInfo.biHeight), true);
				if(pRGBData)
				{
					if(bmpInfo.biBitCount==32)//32�r�b�g
					{
						int nWidth = bmpInfo.biWidth*4;
						unsigned char* pData = (unsigned char*)(szBuffer + sizeof(BITMAPINFOHEADER) + 12);//�摜�f�[�^�擪�|�C���^�擾

						//�f�[�^�R�s�[
						for(int j=0 ; j<bmpInfo.biHeight ; j++){
							for(int i=0 ; i<bmpInfo.biWidth ; i++){
								int addr = i + (bmpInfo.biHeight-1-j)*bmpInfo.biWidth;
								*(pRGBData + addr)										=  *(pData + nWidth*j + i*4 + 2);//R
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight)	=  *(pData + nWidth*j + i*4 + 1);//G
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight*2)	=  *(pData + nWidth*j + i*4 + 0);//B
							}
						}
						

					}
					else if(bmpInfo.biBitCount==24)//24�r�b�g
					{
						unsigned char* pData = (unsigned char*)(szBuffer + sizeof(BITMAPINFOHEADER) );//�摜�f�[�^�擪�|�C���^�擾
						
						//DIB�f�[�^����RGB�f�[�^���̍�
						int nWidth, bmp_width_gap;
						if( (bmpInfo.biWidth*3) % 4 == 0)	bmp_width_gap = 0;	
						else								bmp_width_gap = 4 - (bmpInfo.biWidth*3) % 4;
						nWidth = bmpInfo.biWidth*3 + bmp_width_gap;

						//�f�[�^�R�s�[
						for(int j=0 ; j<bmpInfo.biHeight ; j++){
							for(int i=0 ; i<bmpInfo.biWidth ; i++){
								int addr = i + (bmpInfo.biHeight-1-j)*bmpInfo.biWidth;
								*(pRGBData + addr)										=  *(pData + nWidth*j + i*3 + 2);//R
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight)	=  *(pData + nWidth*j + i*3 + 1);//G
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight*2)	=  *(pData + nWidth*j + i*3 + 0);//B
							}
						}

					}
					else if(bmpInfo.biBitCount==8)//8�r�b�g
					{

						COLORREF	colTbl[256];
						memcpy(colTbl, szBuffer, 1024);//�J���[�p���b�g�擾
						unsigned char* pData = (unsigned char*)(szBuffer + sizeof(BITMAPINFOHEADER) + 1024);//�摜�f�[�^�擪�|�C���^�擾

						//DIB�f�[�^����RGB�f�[�^���̍�
						int nWidth, bmp_width_gap;
						if( (bmpInfo.biWidth) % 4 == 0)	bmp_width_gap = 0;	
						else							bmp_width_gap = 4 - bmpInfo.biWidth % 4;
						nWidth = bmpInfo.biWidth + bmp_width_gap;

						//�f�[�^�R�s�[
						for(int j=0 ; j<bmpInfo.biHeight ; j++){
							for(int i=0 ; i<bmpInfo.biWidth ; i++){
								int addr = i + (bmpInfo.biHeight-1-j)*bmpInfo.biWidth;
								COLORREF rgb = colTbl[*(pData + nWidth*j + i)];
								*(pRGBData + addr)										=  (BYTE)rgb;//R
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight)	=  (BYTE)(((WORD)(rgb)) >> 8);//G
								*(pRGBData + addr + bmpInfo.biWidth*bmpInfo.biHeight*2)	=  (BYTE)((rgb)>>16);//B
							}
						}
					}
					else
					{
						ret = false;
					}
					
					DrawImage(image_num,true,true);//�`��
				}
				GlobalUnlock(hBitmap); 
			}
			::CloseClipboard();//�N���b�v�{�[�h�����
		}
	}

	return ret;
}


////////////////////////////////////////////////////////////////////////////////////////////
//���[�U�_�C�A���O�̎g�p
////////////////////////////////////////////////////////////////////////////////////////////
typedef void (OpenUserDlg) (CPimpomAPI*);
typedef void (DestroyUserDlg) (void);
typedef void (OnDrawMainImage_UserDlg)(int image_number);
typedef void (OnMouseDown_UserDlg) (POINT point);
typedef void (OnMouseDrug_UserDlg) (POINT start_point, POINT currnet_point);
typedef void (OnFinishMouseDrug_UserDlg) (POINT start_point, POINT currnet_point);
typedef void (OnBatchExecute_UserDlg) (BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);
typedef void (OnReceive_UserDlg) (BYTE *data, int *ndata);

/********************************************************************
�@  �\  ��  �� : ���[�U�_�C�A���O�̃L���v�V�������擾
��    ��    �� : GetUserDlgCaptions
��          �� : CString caption[]  (out)�L���v�V����
��    ��    �l : �o�^����Ă��郆�[�U�_�C�A���O�̐�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//���[�U�_�C�A���O�̃L���v�V�������擾
int		CPimpomAPI::GetUserDlgCaptions(CString caption[])
{
	int cnt=0;
	for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){
		if(custom_func[i].hInst != NULL){
			caption[i] = custom_func[i].caption;
			cnt++;
		}else{
			break;
		}
	}

	return cnt;
}

/********************************************************************
�@  �\  ��  �� : ���[�U�_�C�A���O�̋N��
��    ��    �� : UserDialogOpen
��          �� : int n  (in)�_�C�A���O�ԍ�
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogOpen(int n)
{
	OpenUserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OpenUserDlg *)GetProcAddress( custom_func[n].hInst, "OpenUserDlg" );
		if( func != NULL ){
			func(this);
			return true;
		}
	}
	return false;
}

/********************************************************************
�@  �\  ��  �� : ���[�U�_�C�A���O�̔j��
��    ��    �� : UserDialogDelete
��          �� : int n		(in)���[�U�_�C�A���O�̔ԍ�
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogDelete(int n)
{
	DestroyUserDlg		*func;

	if( custom_func[n].hInst == NULL )	return false;

	func = (DestroyUserDlg *)GetProcAddress( custom_func[n].hInst, "DestroyUserDlg" );
	if( func != NULL ){
		func();
	}else{
		return false;
	}
	if(!AfxFreeLibrary( custom_func[n].hInst )){// ���[�h����DLL�����
		return false;
	}
	custom_func[n].hInst=NULL;

	return true;
}


/********************************************************************
�@  �\  ��  �� : ���C����ʕ`��
��    ��    �� : UserDialogOnMouseDown
��          �� : int		n		(in)���[�U�_�C�A���O�̔ԍ�
				 int		image_number	(in)�摜�������ԍ�
��    ��    �l : ������s�ł�����true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogOnDrawMainImage(int n, int image_number)
{
	OnDrawMainImage_UserDlg		*func;

	if (custom_func[n].hInst != NULL){
		func = (OnDrawMainImage_UserDlg *)GetProcAddress(custom_func[n].hInst, "OnDrawMainImage_UserDlg");
		if (func != NULL){
			func(image_number);
			return true;
		}
	}
	return false;
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ł̃}�E�X����
��    ��    �� : UserDialogOnMouseDown
��          �� : int		n		(in)���[�U�_�C�A���O�̔ԍ�
				 POINT		point	(in)�}�E�X�������W�i�摜���W�j
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogOnMouseDown(int n, POINT point)
{
	OnMouseDown_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnMouseDown_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnMouseDown_UserDlg" );
		if( func != NULL ){
			func(point );
			return true;
		}
	}
	return false;
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ł̃}�E�X�E�{�^������
��    ��    �� : UserDialogOnMouseRDown
��          �� : int		n		(in)���[�U�_�C�A���O�̔ԍ�
				 POINT		point	(in)�}�E�X�������W�i�摜���W�j
��    ��    �l : ������s�ł�����true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
			 Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogOnMouseRDown(int n, POINT point)
{
	OnMouseDown_UserDlg* func;

	if (custom_func[n].hInst != NULL) {
		func = (OnMouseDown_UserDlg*)GetProcAddress(custom_func[n].hInst, "OnMouseRDown_UserDlg");
		if (func != NULL) {
			func(point);
			return true;
		}
	}
	return false;
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ł̃}�E�X�h���b�O
��    ��    �� : UserDialogOnMouseDrug
��          �� : int n		(in)���[�U�_�C�A���O�̔ԍ�
				 POINT		start_point	(in)�}�E�X�h���b�O�J�n�i�摜���W�j
				 POINT		currnet_point	(in)���݂̃}�E�X���W�i�摜���W�j
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogOnMouseDrug(int n, POINT start_point, POINT currnet_point)
{
	OnMouseDrug_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnMouseDrug_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnMouseDrug_UserDlg" );
		if( func != NULL ){
			func(start_point,currnet_point );
			return true;
		}
	}
	return false;
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ł̃}�E�X�h���b�O�I��
��    ��    �� : UserDialogOnFinishMouseDrug
��          �� : int n		(in)���[�U�_�C�A���O�̔ԍ�
				 POINT		start_point	(in)�}�E�X�h���b�O�J�n���W�i�摜���W�j
				 POINT		currnet_point	(in)�}�E�X�h���b�O�I�����W�i�摜���W�j
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogOnFinishMouseDrug(int n, POINT start_point, POINT currnet_point)
{
	OnFinishMouseDrug_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnFinishMouseDrug_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnFinishMouseDrug_UserDlg" );
		if( func != NULL ){
			func(start_point,currnet_point );
			return true;
		}
	}
	return false;
}

/********************************************************************
�@  �\  ��  �� : ���[�U�_�C�A���O�̃o�b�`���s
��    ��    �� : UserDialogBatchExecute
��          �� : int	n				(in)���[�U�_�C�A���O�̔ԍ�
				 BATCH_INPUT *in_param	(in)���̓p�����^
				 BATCH_OUTPUT *out_param (out)�o�̓p�����^
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogBatchExecute(int n, BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	OnBatchExecute_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnBatchExecute_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnBatchExecute_UserDlg" );
		if( func != NULL ){
			func(in_param, out_param);
			return true;
		}
	}
	return false;
}

/********************************************************************
�@  �\  ��  �� : �o�b�`���s
��    ��    �� : UserDialogBatchExecute
��          �� : 
			BATCH_INPUT *in_param	(in)���̓p�����^
			BATCH_OUTPUT *out_param (out)�o�̓p�����^
��    ��    �l : ������s�ł�����true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::BatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	//�摜�t�@�C�������[�h
	if (strlen(in_param->imae_file_path) > 0)
	{
		if (SelectAndLoadImageFile(in_param->image_number, CString(in_param->imae_file_path))) {
			DrawImage(in_param->image_number, true, true);
		}
	}
	

	if (batch_execute_rule != 1)
	{
		if (pCmdPrptDlg)//�R�}���h���C���̃o�b�`���s(�J�X�^���t�@���N�V�����̑O)
		{
			if (pCmdPrptDlg->OnBatchExecute(in_param, out_param))//�R�}���h���C���̃o�b�`���s�C�x���g�n���h�����Ăяo��
			{

			}
		}

		if (pPythonBatchDlg)//pyhton�̃o�b�`���s
		{
			if (pPythonBatchDlg->OnBatchExecute())
			{

			}
		}
	}




	//�J�X�^���_�C�A���O�̃o�b�`���s
	if (batch_execute_rule == 0 || batch_execute_rule == 1)
	{
		for (int i = 0; i < CUSTOM_FUNC_MAX; i++)
		{
			BATCH_OUTPUT out_param_tmp = { 0 };

			if (UserDialogBatchExecute(i, in_param, &out_param_tmp))//�J�X�^���_�C�A���O�̃o�b�`���s�C�x���g�n���h�����Ăяo��
			{
				for (int n = 0; n < BATCH_OUT_PARAM_NUM; n++)//�����̃J�X�^���_�C�A���O������ꍇ�́A�o�͒l�𓝍�
				{
					CString reslog = CString(out_param->result[n]) + CString(out_param_tmp.result[n]);
					strcpy(out_param->result[n], reslog.GetBuffer());
					reslog.ReleaseBuffer();
				}
			}
		}
	}


	if (batch_execute_rule == 1)
	{
		if (pCmdPrptDlg)//�R�}���h���C���̃o�b�`���s(�J�X�^���t�@���N�V�����̌�)
		{
			if (pCmdPrptDlg->OnBatchExecute(in_param, out_param))//�R�}���h���C���̃o�b�`���s�C�x���g�n���h�����Ăяo��
			{

			}
		}

		if (pPythonBatchDlg)//pyhton�̃o�b�`���s
		{
			if (pPythonBatchDlg->OnBatchExecute())
			{

			}
		}
	}


	return true;
}

/********************************************************************
�@  �\  ��  �� : ���[�U�_�C�A���O�̒ʐM��M
��    ��    �� : UserDialogBatchExecute
��          �� : int	n		(in)���[�U�_�C�A���O�̔ԍ�
				 BATCH_INPUT *in_param	(in)���̓p�����^
				 BATCH_OUTPUT *out_param (out)�o�̓p�����^
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::UserDialogReceive(int n, BYTE *data, int *ndata)
{
	OnReceive_UserDlg		*func;

	if( custom_func[n].hInst != NULL ){
		func = (OnReceive_UserDlg *)GetProcAddress( custom_func[n].hInst, "OnReceive_UserDlg" );
		if( func != NULL ){
			func(data, ndata);
			return true;
		}
	}
	return false;
}


/********************************************************************
�@  �\  ��  �� : �܂���O���t�ɒl��ǉ����ĕ`�悷��
��    ��    �� : AddDataToChart
��          �� : int	chart_id		(in)�O���t�̔ԍ�
				 float	value			(in)�l
��    ��    �l : ������s�ł�����ǉ������f�[�^�ԍ���Ԃ��B���s�����畉����Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CPimpomAPI::AddDataToChart(int chart_id, float value)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return -1;

	return pChartDlg->AddDataToChart(chart_id, value);
}

/********************************************************************
�@  �\  ��  �� : 2�����U�z�}�ɒl��ǉ����ĕ`�悷��
��    ��    �� : AddDataToChart2
��          �� : int	chart_id		(in)�O���t�̔ԍ�
				 float	valueX			(in)�lX
				 float	valueY			(in)�lY
��    ��    �l : ������s�ł�����ǉ������f�[�^�ԍ���Ԃ��B���s�����畉����Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CPimpomAPI::AddDataToChart2(int chart_id, float valueX, float valueY)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return -1;

	return pChartDlg->AddDataToChart(chart_id, valueX, valueY);
}

/********************************************************************
�@  �\  ��  �� : �܂���O���t�Ƀf�[�^���ǉ����ĕ`�悷��
��    ��    �� : PlotDataToChart
��          �� : int	chart_id		(in)�O���t�̔ԍ�
				 int	data_num		(in)�f�[�^��
				    	*pValArr		(in)�f�[�^��
				 bool	doClear			(in)�v���b�g�O�Ɍ��݂̃O���t���N���A
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::PlotDataToChart(int chart_id, int data_num, float *pValArr, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArr, doClear);
}

bool CPimpomAPI::PlotDataToChart(int chart_id, int data_num, double *pValArr, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArr, doClear);
}

bool CPimpomAPI::PlotDataToChart(int chart_id, int data_num, int *pValArr, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArr, doClear);
}

bool CPimpomAPI::PlotDataToChart(int chart_id, int data_num, BYTE *pValArr, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArr, doClear);
}

/********************************************************************
�@  �\  ��  �� : 2�����U�z�}�Ƀf�[�^���ǉ����ĕ`�悷��
��    ��    �� : PlotDataToChart2
��          �� : int	chart_id		(in)�O���t�̔ԍ�
				 int	data_num		(in)�f�[�^��
				    	*pValArrX		(in)�f�[�^��X
						*pValArrY		(in)�f�[�^��Y
				 bool	doClear			(in)�v���b�g�O�Ɍ��݂̃O���t���N���A
��    ��    �l : ������s�ł�����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CPimpomAPI::PlotDataToChart2(int chart_id, int data_num, float *pValArrX, float *pValArrY, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArrX, pValArrY, doClear);
}

bool CPimpomAPI::PlotDataToChart2(int chart_id, int data_num, double *pValArrX, double *pValArrY, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArrX, pValArrY, doClear);
}

bool CPimpomAPI::PlotDataToChart2(int chart_id, int data_num, int *pValArrX, int *pValArrY, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArrX, pValArrY, doClear);
}

bool CPimpomAPI::PlotDataToChart2(int chart_id, int data_num, BYTE *pValArrX, BYTE *pValArrY, bool doClear)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return false;
	return pChartDlg->PlotDataToChart(chart_id, data_num, pValArrX, pValArrY, doClear);
}

void CPimpomAPI::ClearChart(int chart_id)
{
	if( !DialogOpen(IDD_CHART_DLG, true))	return ;
	pChartDlg->ClearChart(chart_id);
}