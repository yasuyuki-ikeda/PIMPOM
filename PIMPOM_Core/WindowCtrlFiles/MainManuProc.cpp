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
//CPIMPOMDlg
//���C�����j���[����̏����͂����Ŏ�������
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "shlwapi.h"


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

////////////////////////////////////////////////////////////////////////////////////////////
//���j���[�ǉ�
////////////////////////////////////////////////////////////////////////////////////////////
void CPIMPOMDlg::OnInitMenu(CMenu* pMenu)
{
	CDialog::OnInitMenu(pMenu);
}

/********************************************************************
�@  �\  ��  �� : ���j���[���I�����ꂽ
��    ��    �� : OnMenuSelect
��          �� : 
��    ��    �l : 
�@          �\ : �u�ŋߎg�����t�@�C�����J���v���j���[�ɕ\��������e�̍X�V
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CDialog::OnMenuSelect(nItemID, nFlags, hSysMenu);

	//�u�ŋߎg�����t�@�C�����J���v���j���[�̍X�V
	if(nItemID<FILE_IO_LOG_MENU_ID || nItemID>=FILE_IO_LOG_MENU_ID + FILE_IO_LOG_MAX )
	{
		CMenu *menu = this->GetMenu();
		if(!menu)	return;

		CMenu *pSub =menu->GetSubMenu(0);
		if(!pSub)	return;

		CMenu *pSub2 =pSub->GetSubMenu(2);
		if(!pSub2)	return;

		for(int i=0 ; i<FILE_IO_LOG_MAX ; i++){
			pSub2->RemoveMenu(MF_BYCOMMAND, FILE_IO_LOG_MENU_ID+i);
		}

		for(int i=0 ; i<FILE_IO_LOG_MAX ; i++){
			CString filename = API.GetFileIOLog(i);
			if(!filename.IsEmpty()){
				pSub2->AppendMenu(MF_STRING,FILE_IO_LOG_MENU_ID+i, filename);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//�u�t�@�C���v���j���[
////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� : �摜�t�@�C�����J��
��    ��    �� : OnFileOpen
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������Ƀ��[�h
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileOpen() 
{
	if( API.pDataDlg->DataExistMessage("�㏑�����Ă���낵���ł����H", CURRENT_IMAGE) )		return;
	API.SelectAndLoadImageFile( API.GetCurrentImageNumber() );
}

/********************************************************************
�@  �\  ��  �� : �����̃t�@�C����3D/F3D�Ƃ��ĊJ��
��    ��    �� : OnFileOpenSerial
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������Ƀ��[�h
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileOpenSerial() 
{
	CIO3DDlg	iodlg;
	if( API.pDataDlg->DataExistMessage("�㏑�����Ă���낵���ł����H", CURRENT_IMAGE) )		return;
	iodlg.DoModalLoadType( API.GetCurrentImageNumber() );
}

/********************************************************************
�@  �\  ��  �� :�摜�������ꊇ�ǂݍ���
��    ��    �� : OnFileLoadAllimage
��          �� :
��    ��    �l :
�@          �\ : [�摜�������ꊇ�ۑ�]�ɂĕۑ����ꂽ�摜�̓ǂݍ���


���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileLoadAllimage()
{
	CString dirpath = API.GetDefaultPath();
	dirpath = dirpath.Left(dirpath.GetLength()-1);//\������
	
	if (API.FolderDialog(&dirpath))
	{
		//�摜��������ǂݍ���
		for (int i = 0; i<API.GetDataUnitNumber(); i++)
		{
			CString filename, pathname, key;
			filename.Format("\\bk%03d.pim", i);
			pathname = dirpath + _T("\\") + filename;

			if (PathFileExists(pathname)){
				if (API.LoadArchivedImage(i, pathname)){//�摜��ǂݏo��
					API.DrawImage(i);
				}
			}
		}
	}
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^���o�C�i���ŕۑ�����
��    ��    �� : OnFileSaveData
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������̓��e
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileSaveData() 
{
	API.SelectAndSaveImageFile( API.GetCurrentImageNumber() );	
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^���A�[�J�C�u�i*.PIM�j�Ƃ��ĕۑ�����
��    ��    �� : OnFileSaveImage
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������̓��e
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileSaveImage() 
{
	API.SaveArchivedImage( API.GetCurrentImageNumber() );
}

/********************************************************************
�@  �\  ��  �� : �\���摜���r�b�g�}�b�v�Ƃ��ĕۑ�����
��    ��    �� : OnFileSaveView
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileSaveView() 
{
	API.SaveViewAsBitmap( API.GetCurrentImageNumber() );	
}

/********************************************************************
�@  �\  ��  �� : 3D/F3D�f�[�^�𕡐��̉摜�t�@�C���Ƃ��ĕۑ�����
��    ��    �� : OnFileSaveSerial
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������̓��e
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileSaveSerial() 
{
	CIO3DDlg	iodlg;
	CDataUnit	*p_du;
	long		num;

	num = API.GetCurrentImageNumber();//���ݑI�𒆂̉摜�f�[�^�̔ԍ����擾����
	p_du = API.GetDataUnit( CURRENT_IMAGE );//�摜�f�[�^�̐擪�|�C���^���擾����
		if(p_du == NULL)	return;
		if(p_du->PageNumber <= 1)	return;

	iodlg.DoModalStoreType( num );	//IO3D�_�C�A���O��ۑ��^�C�v�ŊJ��
}


/********************************************************************
�@  �\  ��  �� :�摜�������ꊇ�ۑ�
��    ��    �� : OnFileSaveAllimage
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������̓��e
                 PIMPOM.ini�ɏ�������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileSaveAllimage()
{
	CString str;
	CConfirmDlg dlg;
	CString folderName="images";//�ۑ���t�H���_��
	bool	setDefaultImages=true;//����N�����̐ݒ�ɔ��f���邩�ǂ���

	dlg.SetTitle("�摜�������ꊇ�ۑ�");
	dlg.RegistVar("�ۑ��t�H���_��", &folderName);
	dlg.RegistVar("����N�����ɔ��f", &setDefaultImages);


	if (dlg.DoModal() == IDOK)
	{
		CString dirpath = API.GetDefaultPath() + folderName;

		if (PathIsDirectory(dirpath)){
			str = dirpath + _T("�͂��łɑ��݂��܂��B�㏑�����܂���?");
			if (AfxMessageBox(str, MB_OKCANCEL | MB_ICONQUESTION) != IDOK)	return;
		}
		else{
			if (::CreateDirectory(dirpath, NULL) == 0){
				API.MessageBox("%s �t�H���_�쐬���s", dirpath.GetBuffer());	dirpath.ReleaseBuffer();
				return;
			}
		}


		if (setDefaultImages){
			str.Format("����N�����ɂ͌��݂Ɠ�����Ԃ��Č�����܂�" );
			if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;
		}


		//�摜��������ۑ�
		for(int i=0 ; i<API.GetDataUnitNumber() ; i++)
		{
			CString filename, pathname, key;
			filename.Format("\\bk%03d.pim", i);
			pathname = dirpath + filename;
			key.Format("IMG%d",i);

			if(API.GetDataUnit(i) )
			{
				API.SaveArchivedImage(i, pathname);//�摜��ۑ�����

				if (setDefaultImages){
					API.WriteProfile("DEFAULT_IMAGE", key, pathname);//PIMPOM.ini�ɏ�������
				}
			}else{
				if (setDefaultImages){
					API.WriteProfile("DEFAULT_IMAGE", key, "");
				}
			}
		}

		if (setDefaultImages){
			//�I�𒆂̉摜�ԍ���ۑ�
			CString strn;
			strn.Format("%d", API.GetCurrentImageNumber());
			API.WriteProfile("SETTING", "DEFAULT_CURRENT_IMAGE", strn);
		}
	}
}




/********************************************************************
�@  �\  ��  �� : �\���摜���������
��    ��    �� : OnFilePrint
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFilePrint() 
{
	CPrintDialog	ptdlg(false);
	HDC				hptdc;
	DOCINFO			docinfo;
	CSize			img_size;
	CDataUnit		*pdu;
	DEVMODE			*pdevmode;
	float			dot_per_mm_dev, dot_per_mm, width_in_dc, height_in_dc;
	CConfirmDlg		confirm;


		if( (pdu = API.GetDataUnit(CURRENT_IMAGE))==NULL ){
			return;
		}

	img_size = pdu->DataSize;

	//�𑜓x�w��
	dot_per_mm = 10;
	confirm.SetTitle("����̉𑜓x");
	confirm.RegistVar("�𑜓x[pix/mm]", &dot_per_mm);
		if(confirm.DoModal() != IDOK){
			return;
		}

	//����_�C�A���O�w��
	int ret = ptdlg.DoModal();
	if(ret == IDOK){
		hptdc = ptdlg.GetPrinterDC();
		pdevmode = ptdlg.GetDevMode();

		dot_per_mm_dev = (float)((pdevmode->dmPrintQuality)/25.4);//�f�o�C�X�̉𑜓x[pix/mm]
		width_in_dc = dot_per_mm_dev/dot_per_mm*(float)img_size.cx;
		height_in_dc = dot_per_mm_dev/dot_per_mm*(float)img_size.cy;

		docinfo.cbSize = sizeof(docinfo);
		docinfo.lpszDatatype = NULL;
		docinfo.lpszDocName = NULL;
		docinfo.lpszOutput = NULL;
		docinfo.fwType = 0;
		::StartDoc( hptdc , &docinfo );//�v�����g�W���u�J�n
		::StartPage( hptdc );//�y�[�W�̊J�n

		::SetStretchBltMode( hptdc, HALFTONE);//���ԐF�ŕ�Ԃ���
		if( (pdu->DataType != RGB_FORMAT && pdu->DataType != RGB_3D_FORMAT) ||  pdu->DispRGB == false){//���m�N��

			::StretchDIBits(hptdc, 0, 0, (int)width_in_dc, (int)height_in_dc,
								0, 0, img_size.cx, img_size.cy, 
								pdu->pDispBuffer,
								pdu->pDispBufferBmpInfo,
								DIB_RGB_COLORS, SRCCOPY);

		}else{//�J���[
			::StretchDIBits(hptdc, 0, 0, (int)width_in_dc, (int)height_in_dc,
								0, 0, img_size.cx, img_size.cy, 
								pdu->pDispBufferRGB,
								pdu->pDispBufferBmpInfoRGB,
								DIB_RGB_COLORS, SRCCOPY);

		}
		::TextOut(hptdc, 0, (int)height_in_dc , pdu->DataName ,
							pdu->DataName.GetLength() );

		::EndPage( hptdc );//�y�[�W�̏I��
		::EndDoc( hptdc );//�v�����g�W���u�I��
		::DeleteDC( hptdc );
	}	
}

/********************************************************************
�@  �\  ��  �� : �A�v���P�[�V�����̏I��
��    ��    �� : OnFileQuit
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFileQuit() 
{
	AfxGetMainWnd()->PostMessage(WM_CLOSE);		
}

/********************************************************************
�@  �\  ��  �� : �ŋߎg�����t�@�C�����J��
��    ��    �� : OnMenuLoadIOLog
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������Ƀ��[�h
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnMenuLoadIOLog(unsigned int n)
{
	if( API.pDataDlg->DataExistMessage("�㏑�����Ă���낵���ł����H", CURRENT_IMAGE) )		return;

	if( API.SelectAndLoadImageFile(CURRENT_IMAGE, API.GetFileIOLog(n-FILE_IO_LOG_MENU_ID)) ){
		API.DrawImage(CURRENT_IMAGE);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
//�ҏW���j���[
////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
�@  �\  ��  �� :�摜�������̖��O�ύX
��    ��    �� :OnEditRename
��          �� : 
��    ��    �l : 
�@          �\ : �摜�������̖��O�ύX
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditRename()
{
	CDataUnit *pdu = API.GetDataUnit(CURRENT_IMAGE);
	if(pdu){
		CConfirmDlg dlg;
		dlg.SetWidth(70,200);
		dlg.RegistVar("�摜�f�[�^��", &pdu->DataName);
		if(dlg.DoModal()==IDOK){
			API.DrawImage(CURRENT_IMAGE, false);
		}
	}
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃^�C�v�ϊ�
��    ��    �� : OnEditConvertType
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������̓��e
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditConvertType() 
{
	CConvertDataDlg  cvt;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	cvt.DoModal();
}

/********************************************************************
�@  �\  ��  �� : �o�b�N�A�b�v�����Ă����摜�𕜌�����
��    ��    �� : OnEditUndo
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������ɕ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditUndo() 
{
	CString	str;

	str.Format("�摜������ %d �Ƀf�[�^�𕜌����܂��D��낵���ł����H" , API.GetCurrentImageNumber());
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	API.RecoverDataUnit(CURRENT_IMAGE);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�폜
��    ��    �� : OnEditDelete
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜���������N���A
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditDelete() 
{
	CString	str;

	str.Format("�摜������ %d �̃f�[�^���폜���܂��D��낵���ł����H" , API.GetCurrentImageNumber());
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	API.BackupDataUnit(CURRENT_IMAGE);
	API.DeleteDataUnit(CURRENT_IMAGE);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�S�폜
��    ��    �� : OnEditDeleteAll
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̑S�摜���������N���A
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditDeleteAll()
{
	CString	str;

	str.Format("���ׂẲ摜�f�[�^���폜���܂��D��낵���ł����H");
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	for(int i=0 ; i<API.GetDataUnitNumber() ; i++){
		API.DeleteDataUnit(i);
	}
}

/********************************************************************
�@  �\  ��  �� : �}�[�N�ҏW
��    ��    �� : OnEditMark
��          �� :
��    ��    �l :
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditMark()
{
	API.SetMarkEditMode(2);
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�ҏW
��    ��    �� : OnEditWorkarea
��          �� :
��    ��    �l :
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditWorkarea()
{
	API.SetWorkAreaEditMode(true);
}

/********************************************************************
�@  �\  ��  �� : �\���摜���N���b�v�{�[�h�փR�s�[����
��    ��    �� : OnEditCopyClipbord
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditCopyClipbord() 
{
	API.CopyToClipbord();
}

/********************************************************************
�@  �\  ��  �� : �摜���N���b�v�{�[�h����y�[�X�g����
��    ��    �� : OnEditPasteClipbord
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditPasteClipbord()
{
	if( API.pDataDlg->DataExistMessage("�㏑�����Ă���낵���ł����H", CURRENT_IMAGE) )		return;

	API.PasteFromClipbord(CURRENT_IMAGE);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̒��ڕҏW
��    ��    �� : OnEditPaint
��          �� : 
��    ��    �l : 
�@          �\ : ���ڕҏW���邽�߂̃E�B���h�E���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditPaint()
{
	if (API.GetDataPickEditType() == 0) {
		API.SetDataPickEditType(1);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
//�u�\���v���j���[
////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �J���[�p���b�g�̕ύX
��    ��    �� : on_change_color_palette
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::on_change_color_palette(short palette_id)
{
	CDataUnit	*p_du;

	if( (p_du = API.GetDataUnit(CURRENT_IMAGE)) == NULL )		return;
	if( (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)  &&  p_du->DispRGB == true)	return;

	p_du->ChangePalette(palette_id);
	API.DrawImage(CURRENT_IMAGE, false);
}

//�J���[�p���b�g���O���[�X�P�[��(�|�W)�ɕύX
void CPIMPOMDlg::OnViewColorGrayscale()
{
	on_change_color_palette(PALETTE_GRAYSCALE);
}

//�J���[�p���b�g���O���[�X�P�[��(�l�K)�ɕύX
void CPIMPOMDlg::OnViewColorNega() 
{
	on_change_color_palette(PALETTE_NEGATIVE);
}

//�J���[�p���b�g����F�ɕύX
void CPIMPOMDlg::OnViewColorRainbow()
{
	on_change_color_palette(PALETTE_RAINBOW);
}

//�J���[�p���b�g����F(0�̂Ƃ���͍�)�ɕύX
void CPIMPOMDlg::OnViewColorRainbow2() 
{
	on_change_color_palette(PALETTE_BLACK_RAINBOW);
}

//�J���[�p���b�g���R�T�C���ɕύX
void CPIMPOMDlg::OnViewColorCosine() 
{
	on_change_color_palette(PALETTE_COSINE);
}

//�J���[�p���b�g�����F�ːɕύX
void CPIMPOMDlg::OnViewColorYtob() 
{
	on_change_color_palette(PALETTE_BLUE_YELLOW);
}

//�J���[�p���b�g��΁ːԂɕύX
void CPIMPOMDlg::OnViewColorGtor() 
{
	on_change_color_palette(PALETTE_GREEN_RED);
}

void CPIMPOMDlg::OnViewColorLabel()
{
	on_change_color_palette(PALETTE_LABEL);
}


/********************************************************************
�@  �\  ��  �� : ���ݒ�
��    ��    �� : OnViewSetupEnvironment
��          �� :
��    ��    �l :
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnViewSetupEnvironment()
{
	int data_number, data_window_width, thumbneil_colum;
	int  main_image_w, main_image_h, small_image_w, small_image_h;
	CConfirmDlg dlg;
	

	//ini�t�@�C������ǂݍ���
	data_number = API.GetDataUnitNumber();
	main_image_w = API.main_image_size.cx;
	main_image_h = API.main_image_size.cy;
	data_window_width = API.data_window_width;
	thumbneil_colum = API.thumbneil_colum;
	small_image_w = API.small_image_size.cx;
	small_image_h = API.small_image_size.cy;

	dlg.SetTitle("���ݒ�");
	dlg.RegistVar("�摜��������", &data_number);
	dlg.RegistVar("���C���摜��", &main_image_w);
	dlg.RegistVar("���C���摜����", &main_image_h);
	dlg.RegistVar("�f�[�^�E�B���h�E��", &data_window_width);
	dlg.RegistVar("�T���l�C���s��", &thumbneil_colum);
	dlg.RegistVar("�T���l�C���摜��", &small_image_w);
	dlg.RegistVar("�T���l�C���摜����", &small_image_h);

	if (dlg.DoModal() == IDOK)
	{	//ini�t�@�C���ɒl����������
		CString str;

		str.Format("%d", data_number);
		API.WriteProfile("SETTING", "IMG_NUM", str);

		str.Format("%d", main_image_w);
		API.WriteProfile("SETTING", "MAIN_IMG_W", str);

		str.Format("%d", main_image_h);
		API.WriteProfile("SETTING", "MAIN_IMG_H", str);

		str.Format("%d", data_window_width);
		API.WriteProfile("SETTING", "DATA_WIN_W", str);

		str.Format("%d", small_image_w);
		API.WriteProfile("SETTING", "SMALL_IMG_W", str);

		str.Format("%d", small_image_h);
		API.WriteProfile("SETTING", "SMALL_IMG_H", str);

		API.MessageBox("�ݒ�ύX��L���ɂ���ɂ́APIMPOM���ċN�����Ă�������");
	}
}

/********************************************************************
�@  �\  ��  �� : ���ڕ`��p�t�H���g�쐬
��    ��    �� : OnViewMakefont
��          �� : 
��    ��    �l : 
�@          �\ : �摜�f�[�^�ɒ��ڕ����`�悷�邽�߂̃r�b�g�}�b�v�t�H���g���𐶐�����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnViewMakefont()
{

	int		font_size = API.default_font.lfHeight;
	CConfirmDlg	box;

	box.RegistVar("�t�H���g�T�C�Y",&font_size);
	box.SetWidth(80,30);
	if( box.DoModal() != IDOK )	return;

	if (box.DoModal() == IDOK)
	{	//ini�t�@�C���ɒl����������
		CString str;

		str.Format("%d", font_size);
		API.WriteProfile("SETTING", "FONT_SIZE", str);

		API.default_font.lfHeight = font_size;
	}

}


/********************************************************************
�@  �\  ��  �� : �}�[�N�\��
��    ��    �� : OnViewMark
��          �� :
��    ��    �l :
�@          �\ : 
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnViewMark()
{
	if (API.GetMarkEditMode() == 0) {
		API.SetMarkEditMode(1);
	}
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�\��
��    ��    �� : OnViewWorkarea
��          �� :
��    ��    �l :
�@          �\ :
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnViewWorkarea()
{
	API.SetWorkAreaEditMode(true);
}

/********************************************************************
�@  �\  ��  �� : �����摜�̓����\��
��    ��    �� : OnViewMultiImageDisp
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnViewMultiImageDisp()
{
	API.DialogOpen(IDD_MULTI_IMAGE_DISP_DLG, true);
}

/********************************************************************
�@  �\  ��  �� : 3�����摜�ꗗ�\��
��    ��    �� : OnView3dImgDisp
��          �� :
��    ��    �l :
�@          �\ :
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnView3dImgDisp()
{
	API.DialogOpen(IDD_3D_IMAGE_DISP_DLG, true);
}


////////////////////////////////////////////////////////////////////////////////////////////
//�u�摜�����v���j���[
////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̃T�C�Y�ϊ�
��    ��    �� : OnEditResize
��          �� : 
��    ��    �l : 
�@          �\ : ���ݑI�𒆂̉摜�������̓��e
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnEditResize() 
{
	CResizeDataDlg  res;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	res.DoModal();
}

/********************************************************************
�@  �\  ��  �� : �ėp�t�B���^
��    ��    �� : OnFilter
��          �� : 
��    ��    �l : 
�@          �\ : �t�B���^�ݒ肷�邽�߂̃E�B���h�E���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnFilter() 
{
	API.DialogOpen(IDD_FILTER_CTRL_DLG,true);

}

/********************************************************************
�@  �\  ��  �� : �C�ӂ̐����ɂ��摜����
��    ��    �� : OnCalcImage
��          �� : 
��    ��    �l : 
�@          �\ : �����ҏW���邽�߂̃E�B���h�E���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnCalcImage() 
{
	API.SetCalcToolMode(true);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̊􉽕ϊ�
��    ��    �� : OnImageTransform
��          �� : 
��    ��    �l : 
�@          �\ : �􉽕ϊ����邽�߂̃E�B���h�E���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnImageTransform()
{
	CTransformDlg  trn;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	trn.DoModal();
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^��2�l��
��    ��    �� : OnBinalize
��          �� : 
��    ��    �l : 
�@          �\ : 2�l�����邽�߂̃E�B���h�E���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnBinalize()
{
	if(!API.GetDataUnit(CURRENT_IMAGE))	return;

	API.DialogOpen(IDD_BINALIZE_DLG,true);
}


////////////////////////////////////////////////////////////////////////////////////////////
//�u�c�[���v���j���[
////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �r�f�I�L���v�`���N��
��    ��    �� : OnToolVideocapture
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnToolVideocapture()
{
	if( API.CaptureInit(CSize(-1,-1)) ){
		CButton	*p_chk_video = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE);
		p_chk_video->ShowWindow(SW_SHOW);

		CButton	*p_chk_video2 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2);
		p_chk_video2->ShowWindow(SW_SHOW);
	}
}

/********************************************************************
�@  �\  ��  �� : TCP/IP�ʐM�ݒ�
��    ��    �� : OnToolSocket
��          �� : 
��    ��    �l : 
�@          �\ : �\�P�b�g�ʐM�ݒ肷�邽�߂̃E�B���h�E���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnToolSocket()
{
	API.DialogOpen(IDD_SOCKET_CTRL_DLG,true);
}

/********************************************************************
�@  �\  ��  �� : �o�b�`���s
��    ��    �� : OnToolBatch
��          �� : 
��    ��    �l : 
�@          �\ : �o�b�`���s�̂��߂̃E�B���h�E���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnToolBatch()
{
	API.DialogOpen(IDD_BATCH_DLG,true);
}


/********************************************************************
�@  �\  ��  �� : 2�_�Ԃ̋���
��    ��    �� : OnToolRuler
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnToolRuler()
{
	API.SetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, true);
}


/********************************************************************
�@  �\  ��  �� : �q�X�g�O����
��    ��    �� : OnToolHistgram
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnToolHistgram()
{
	API.DialogOpen(IDD_HISTGRAM_DLG, true);
	API.DrawImage();
}

/********************************************************************
�@  �\  ��  �� : �R�}���h�v�����v�g
��    ��    �� : OnToolCommandPrompt
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnToolCommandPrompt()
{
	API.DialogOpen(IDD_COMMAND_RPOMPT_DLG,true);
}


/********************************************************************
�@  �\  ��  �� : Python�o�b�`���s�ݒ�
��    ��    �� : OnToolPythonBatch
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnToolPythonBatch()
{
	API.DialogOpen(IDD_PYTHON_BATCH_DLG, true);
}

////////////////////////////////////////////////////////////////////////////////////////////
//�u�w���v�v���j���[
////////////////////////////////////////////////////////////////////////////////////////////


void CPIMPOMDlg::OnHelpSpecificationSheet()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM�\�t�g�E�F�A�d�l��.pptx", NULL, NULL, SW_SHOWNORMAL);
}

void CPIMPOMDlg::OnHelpApireference()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM_API���t�@�����X.docx", NULL, NULL, SW_SHOWNORMAL);
}

void CPIMPOMDlg::OnHelpApireference2()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM_�O���v���O�����摜�f�[�^�󂯓n���}�j���A��.doc", NULL, NULL, SW_SHOWNORMAL);
}

void CPIMPOMDlg::OnHelpApireference3()
{
	ShellExecute(NULL, "open", API.GetDefaultPath() + "..\\..\\..\\Doc\\PIMPOM_PIMPOMPY�}�j���A��.doc", NULL, NULL, SW_SHOWNORMAL);
}

////////////////////////////////////////////////////////////////////////////////////////////
//�u�J�X�^���t�@���N�V�����v���j���[
////////////////////////////////////////////////////////////////////////////////////////////

void CPIMPOMDlg::OnCustomAdd()
{
	ShellExecute(m_hWnd, "open",API.GetDefaultPath() + "CloneUserDlg.exe",NULL,NULL,SW_SHOWNORMAL);
}


/********************************************************************
�@  �\  ��  �� : �J�X�^���t�@���N�V�����p�̃E�B���h�E���o��
��    ��    �� : OnMenuCustomFunc
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnMenuCustomFunc(unsigned int n)
{
	API.UserDialogOpen(n-CUSTOM_MENU_ID);
}


