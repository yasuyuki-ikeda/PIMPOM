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
//CIO3DDlg
//3�����摜�̓��o�̓_�C�A���O
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
#include "PIMPOM_API.h"

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

bool get_file_footer(CString pathname, CString ext, short *pwidth, short *pheight, short *ppage, short *pchannel);

//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////

CIO3DDlg::CIO3DDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIO3DDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIO3DDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CIO3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIO3DDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}

/********************************************************************
�@  �\  ��  �� : �t�@�C���^�C�v�̕ύX
��    ��    �� : change_filetype_radiobutton
��          �� : int type�@(in)
��    ��    �l : 
�@          �\ : ���W�I�{�^���I����Ԃ̕ύX
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CIO3DDlg::change_filetype_radiobutton(int type)
{
	bool	radio_button_check[3];
		for(int i=0 ; i<3 ; i++)	radio_button_check[i] = false;

	radio_button_check[type] = true;

	API.SetCheck( m_hWnd , IDC_RADIO_IO3D_RAW , radio_button_check[FILE_TYPE_RAW] );
	API.SetCheck( m_hWnd , IDC_RADIO_IO3D_BMP , radio_button_check[FILE_TYPE_BMP] );
	API.SetCheck( m_hWnd , IDC_RADIO_IO3D_FLOAT , radio_button_check[FILE_TYPE_FLOAT] );
}

/********************************************************************
�@  �\  ��  �� : ���̃_�C�A���O��ۑ����[�h�ŋN������
��    ��    �� : DoModalStoreType
��          �� : int data_number�@(in)�ۑ����̃f�[�^���������摜�������ԍ�
��    ��    �l : DoModal�Ɠ���
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CIO3DDlg::DoModalStoreType(long data_number)
{
	CDataUnit *p_du;

	do_store = true;//�ۑ��^�C�v�̃t���O�𗧂Ă�
	target_image_number = data_number;//�^�[�Q�b�g�ƂȂ�摜���������w�肷��

	p_du = API.GetDataUnit( target_image_number );
		if( p_du == NULL )	return	IDCANCEL;//�^�[�Q�b�g�̉摜��������΂Ȃɂ����Ȃ�
		if( p_du->PageNumber<=1 )	return	IDCANCEL;//�y�[�W��1�ȉ��̏ꍇ�ɂ͉������Ȃ�

	return	DoModal();//�_�C�A���O�����[�_���Ƃ��ď���������
}

/********************************************************************
�@  �\  ��  �� : ���̃_�C�A���O��ǂݍ��݃��[�h�ŋN������
��    ��    �� : DoModalLoadType
��          �� : int data_number�@(in)�ǂݍ��ݐ�̉摜�������ԍ�
��    ��    �l : DoModal�Ɠ���
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CIO3DDlg::DoModalLoadType(long data_number)
{
	do_store = false;//���̓^�C�v�̃t���O�𗧂Ă�
	target_image_number = data_number;//�^�[�Q�b�g�ƂȂ�摜���������w�肷��

	return	DoModal();//�_�C�A���O�����[�_���Ƃ��ď���������
}

/********************************************************************
�@  �\  ��  �� : 3�����f�[�^�𕡐����̃t�@�C���Ƃ��ĕۑ�
��    ��    �� : store
��          �� : 
��    ��    �l : ����ɕۑ��ł����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CIO3DDlg::store()
{
	CDataUnit	*p_du;
	CListBox	*p_list;
	float		width, height;
	CSize		size;
	CString		file_path, file_name, mes;
	int			n, file_number;
	bool		ret;


	p_du = API.GetDataUnit( target_image_number );//�^�[�Q�b�g�̉摜�f�[�^���擾����
		if( p_du == NULL )	return	false;//�^�[�Q�b�g�̉摜��������΂Ȃɂ����Ȃ�

	//�t�@�C�����̃��X�g�����
	p_list = (CListBox*)GetDlgItem( IDC_LISTBOX_IO3D_FILENAME );//���X�g�{�b�N�X�̃|�C���^���擾����
	
	disp_all_save_files();//���X�g�{�b�N�X�ɑS�Ẵt�@�C������\������

	//�e�l���擾
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_FILE_PATH , &file_path );//�t�@�C���̃p�X
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_WIDTH , &width );//�摜�̕�
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_HEIGHT , &height );//�摜�̍���
	size.cx = (long)width;
	size.cy = (long)height;
	file_number = p_list->GetCount();//�t�@�C���̐�

	//�ۑ�����
	for(n=0  ;  n<file_number  ;  n++)//�S�Ẳ摜�𕪊����ĕۑ�����
	{
		//�ۑ����̉摜��\������
		p_du->SetDispPage(n);//�y�[�W���w��
		API.DrawImage(target_image_number , true);//���ݕۑ����̉摜���X�V
		p_list->GetText(n, file_name);//�t�@�C���������X�g����擾����

			 if( p_du->DataType == THREE_D_FORMAT )//8bit3D�t�@�C���̏ꍇ//////////////////////////
			 {
					if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_RAW) )//RAW�t�@�C���Ƃ��ĕۑ�
					{
						ret = API.SaveByteImage( p_du->pByteData + size.cx * size.cy * n ,
															file_path + file_name , size , 1); 
					}
					else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_BMP) )//�r�b�g�}�b�v�Ƃ��ĕۑ�
					{
						ret = API.SaveByteAsBitmap(p_du->pByteData + size.cx * size.cy * n ,
												 p_du->pDispBufferBmpInfo, size, file_path + file_name );
					}
					else
					{
						AfxMessageBox("���̌`���ł͕ۑ��ł��܂���");
						return	false;
					}

			}
			 else if(p_du->DataType == FLOAT_3D_FORMAT)//32bit3D�t�@�C���̏ꍇ//////////////////////////////
			 {

					if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_RAW) )//RAW�t�@�C���Ƃ��ĕۑ�
					{
						ret = API.SaveViewAsRaw(  target_image_number , file_path + file_name );
					}
					else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_BMP) )//�r�b�g�}�b�v�Ƃ��ĕۑ�
					{
						ret = API.SaveViewAsBitmap( target_image_number , file_path + file_name );

					}
					else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_FLOAT) )//Float�^�Ƃ��ĕۑ�
					{
						ret = API.SaveFloatImage( p_du->pFloatData + size.cx * size.cy * n ,
																file_path + file_name , size, 1);
					}
			}
			else  if( p_du->DataType == RGB_3D_FORMAT )//24bit�J���[3D�t�@�C���̏ꍇ//////////////////////////
			{
					if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_RAW) )//RAW�t�@�C���Ƃ��ĕۑ�
					{
						AfxMessageBox("���̌`���ł͕ۑ��ł��܂���");
						return	false;

					}
					else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_BMP) )//�r�b�g�}�b�v�Ƃ��ĕۑ�
					{
						p_du->SetDispPage(n);
						API.SetDispRange(target_image_number,0,256);//�\���摜�̔Z�x���f�[�^�ɂ��킹��
						ret = API.SaveViewAsBitmap(target_image_number,file_path + file_name);//�\���摜��ۑ�
					}
					else
					{
						AfxMessageBox("���̌`���ł͕ۑ��ł��܂���");
						return	false;
					}
			}else{
				return	false;
			}

			if(ret == false)//�摜�̕ۑ��Ɏ��s������
			{
				mes.Format("%d���ڂ̉摜�ۑ��Ɏ��s");
				AfxMessageBox(mes);
			}

		API.ShowMessage( true, 0, false, "%d/%d�I��" , n+1 , file_number);
	}
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �ۑ����[�h�Łu�Q�Ɓv�{�^���������ꂽ
��    ��    �� : browse_for_store
��          �� : 
��    ��    �l : 
�@          �\ : �P�t�@�C���I���ł���t�@�C���_�C�A���O���o��
				 �I�������t�@�C���̃^�C�g���ɘA���ԍ������āA�ۑ��t�@�C���̃��X�g�ɕ\������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CIO3DDlg::browse_for_store()
{
	CString path_name, file_name, path, title;


	//�t�@�C���_�C�A���O��\��
	CString filestr_any;
	filestr_any.LoadString(IDS_FILETYPE_ANY);
	CFileDialog myDLG(false , NULL , NULL , OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filestr_any + _T("|"));	

		if( myDLG.DoModal() != IDOK )	return;

	//�t�@�C���_�C�A���O����p�X�ƃ^�C�g�����擾����
	path_name = myDLG.GetPathName();
	file_name = myDLG.GetFileName();
	path = path_name.Left( path_name.GetLength()  -  file_name.GetLength() );
	title = myDLG.GetFileTitle();

	//�p�X�ƘA������\��
	API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_FILE_PATH , path );
	API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_SERIAL_NAME , title );
}

/********************************************************************
�@  �\  ��  �� : �r�b�g�}�b�v�t�@�C����BITMAPINFOHEADER��ǂݍ���
��    ��    �� : check_bitmap_header
��          �� : CString			filename	(in)�t�@�C����
				 BITMAPINFOHEADER	&bmp_header	(out)�ǂݍ��܂ꂽBMP�t�@�C����BITMAPINFOHEADER
��    ��    �l : ����ɓǂݍ��߂��true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CIO3DDlg::check_bitmap_header(CString filename, BITMAPINFOHEADER &bmp_header)
{
	CFile  file;
	BITMAPFILEHEADER	bmp_file_header;

		if( ! file.Open(filename,CFile::modeRead|CFile::typeBinary) )	return false;

	//BITMAPFILEHEADER�̓ǂ݂���
	file.Read( &bmp_file_header , sizeof(BITMAPFILEHEADER) );
		if( bmp_file_header.bfType != 0x4d42){	
			AfxMessageBox(_T("���̃t�@�C���̓r�b�g�}�b�v�ł͂���܂���"));
			file.Close();
			return false;
		}

	//BITMAPINFOHEADER�̓ǂ݂���
	file.Read( &bmp_header, sizeof(BITMAPINFOHEADER) );

	file.Close();
	return true;
}

/********************************************************************
�@  �\  ��  �� : 3�����f�[�^���t�@�C������ǂݍ���
��    ��    �� : load
��          �� : 
��    ��    �l : ����ɓǂݍ��߂���true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CIO3DDlg::load()
{
	CListBox	*p_list;
	float		width, height, channel, *p_float_data;
	BYTE		*p_byte_data;
	CSize		size;
	BITMAPINFOHEADER bmp_header = {0};
	CString		file_path, file_name, mes;	
	int			n, file_number;
	bool		ret = true;
	CDataUnit	*pdu;

	p_list = (CListBox*)GetDlgItem( IDC_LISTBOX_IO3D_FILENAME );//���X�g�{�b�N�X�̃|�C���^���擾����
	if( p_list ==NULL )	return false;

	//�e�l���擾
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_FILE_PATH , &file_path );//�t�@�C���̃p�X
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_WIDTH , &width );//�摜�̕�
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_HEIGHT , &height );//�摜�̍���
	API.GetEditValue(m_hWnd, IDC_EDIT_IO3D_CHANNEL, &channel);//�摜�̃`���l��
	size.cx = (long)width;
	size.cy = (long)height;
	file_number = p_list->GetCount();//�t�@�C���̐�

	if( file_number ==0 )	return false;

	//���������擾
	if (API.GetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE))
	{
		if (channel == 3){
			p_byte_data = API.GetRGB3DMemory(target_image_number, size, file_number, true);
		}
		else{
			p_byte_data = API.Get3DMemory(target_image_number, size, file_number, true);
		}
	}
	else{
		p_float_data = API.GetF3DMemory(target_image_number, size, file_number, true);
	}


	pdu = API.GetDataUnit(target_image_number);

	//�t�@�C���̓ǂݍ���
	for(n=0 ; n<file_number ; n++)
	{
		p_list->GetText( n , file_name);
		if (API.GetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE))//RAW
		{
			cv::Mat img = cv::imread((LPCSTR)(file_path + file_name), -1);

			if (img.cols != width || img.rows != height || img.channels() != channel)	return false;

			if (channel==3)
			{
				BYTE *pdstR = p_byte_data + n * size.cx * size.cy * pdu->ChannelNumber;
				BYTE *pdstG = pdstR + size.cx * size.cy;
				BYTE *pdstB = pdstG + size.cx * size.cy;
				for (int j = 0; j <height; j++)
				{
					for (int i = 0; i < width; i++)
					{
						*pdstR = img.at<Vec3b>(j, i)[2];
						*pdstG = img.at<Vec3b>(j, i)[1];
						*pdstB = img.at<Vec3b>(j, i)[0];

						pdstR++;
						pdstG++;
						pdstB++;
					}
				}
			}
			else if (channel == 1)
			{
				BYTE *pdst = p_byte_data + n * size.cx * size.cy * pdu->ChannelNumber;
				BYTE *psrc = img.data;
				for (int j = 0; j <height; j++)
				{
					for (int i = 0; i < width; i++)
					{
						*pdst = *psrc;
						pdst++;
						psrc++;
					}
				}
			}
			else{
				return false;
			}
		}
		else
		{
			ret = API.LoadFloatImage(p_float_data + n * size.cx * size.cy * pdu->ChannelNumber, file_path + file_name, size);
		}

		if(!ret)	return	false;
	}

	API.DrawImage( target_image_number , true);//�ǂݍ��񂾉摜�f�[�^���ĕ`�悷��
	return	true;
}

/********************************************************************
�@  �\  ��  �� : �ǂݍ��݃��[�h�Łu�Q�Ɓv�{�^���������ꂽ
��    ��    �� : browse_for_load
��          �� : 
��    ��    �l : 
�@          �\ : �����t�@�C����I���ł���t�@�C���_�C�A���O���o��
�@�@�@�@�@�@�@�@ �I�����ꂽ�t�@�C�������X�g�ɕ\������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CIO3DDlg::browse_for_load()
{
	int			file_number;
	CString			path_name, file_name[1000], path;

	//�t�@�C���_�C�A���O��\������
	CString		filestr_any, filestr_raw, filestr_bmp, filestr_float;
	filestr_any.LoadString(IDS_FILETYPE_ANY);
	filestr_raw.LoadString(IDS_FILETYPE_RAW);
	filestr_bmp.LoadString(IDS_FILETYPE_BITMAP);
	filestr_float.LoadString(IDS_FILETYPE_FLOAT);
	if (API.OpenFileDialog(1000, _T("���ׂẲ摜�t�@�C��|*.*|") + filestr_bmp + filestr_raw + filestr_float + _T(" | "),
		&path, file_name, &file_number) != IDOK)	return;

	if (file_number > 0){
		setlist_for_load(path, file_name, file_number);
	}

}

void CIO3DDlg::setlist_for_load(CString path, CString file_name[], int file_number)
{
	//�ŏ��̂P���ǂݍ���ŁA���E�����E�`���l�������߂�
	CString fname0 = file_name[0];
	fname0.MakeUpper();
	if(fname0.Right(3) == _T("RAW") )//raw
	{
		API.SetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE, true);

		short w, h, c, p;
		if (get_file_footer(path + file_name[0], _T("RAW"), &w, &h, &p, &c))
		{
			API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_WIDTH, w);//����\������
			API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_HEIGHT, h);//������\������
		}
	}
	else if(fname0.Right(5) == _T("FLOAT") )//float
	{
		API.SetCheck(m_hWnd, IDC_RADIO_IO3D_LOADFLOAT, true);

		short w, h, c, p;
		if (get_file_footer(path + file_name[0], _T("RAW"), &w, &h, &p, &c))
		{
			API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_WIDTH, w);//����\������
			API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_HEIGHT, h);//������\������
		}
	}
	else
	{
		API.SetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE, true);

		cv::Mat img = cv::imread((LPCSTR)(path + file_name[0]), -1);

		if (img.channels() != 1 && img.channels() != 3)	return ;

		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_WIDTH, img.cols);//����\������
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_HEIGHT, img.rows);//������\������
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_CHANNEL, img.channels());//�`���l����\������		
	}


	//���X�g�Ƀt�@�C����������
	CListBox		*p_list = (CListBox*)GetDlgItem(IDC_LISTBOX_IO3D_FILENAME);
	p_list->ResetContent();//���X�g���N���A����

	for(int i=0 ; i<file_number ; i++){
			p_list->AddString( file_name[i] );
	}

	API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_FILE_PATH , path );//�p�X��\������
	API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , file_number );//�t�@�C������\������


}

/********************************************************************
�@  �\  ��  �� : �w�肳�ꂽ�t�@�C�������݂��邩�ǂ����`�F�b�N����
��    ��    �� : DoFileExist
��          �� : CString	file_name	(in)�t�@�C���p�X
��    ��    �l : ���݂����true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CIO3DDlg::DoFileExist(CString file_name)
{
	CFileStatus status;

		if( CFile::GetStatus(file_name , status) )		return  true;
		else											return  false; 
}


//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CIO3DDlg, CDialog)
	//{{AFX_MSG_MAP(CIO3DDlg)
	ON_BN_CLICKED(IDC_BUTTON_IO3D_REFER, OnIo3dRefer)
	//ON_BN_CLICKED(IDC_BUTTON_IO3D_DISP_ALL_FILES, OnIo3dDispAllFiles)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CIO3DDlg::OnBnClickedOk)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_RADIO_IO3D_RAW, &CIO3DDlg::OnBnClickedRadioIo3dRaw)
	ON_BN_CLICKED(IDC_RADIO_IO3D_BMP, &CIO3DDlg::OnBnClickedRadioIo3dBmp)
	ON_BN_CLICKED(IDC_RADIO_IO3D_FLOAT, &CIO3DDlg::OnBnClickedRadioIo3dFloat)
	ON_EN_CHANGE(IDC_EDIT_IO3D_SERIAL_NAME, &CIO3DDlg::OnEnChangeEditIo3dSerialName)
	ON_BN_CLICKED(IDC_IO_3D_DLG_FOLDER_BROWSE, &CIO3DDlg::OnBnClickedIo3dDlgFolderBrowse)
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
BOOL CIO3DDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString		str;
	CSize		init_size;
	CDataUnit	*p_du;




	if( do_store )//�Z�[�u
	{

		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_IO3D_SERIAL_NAME), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_LOADBYTE), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_LOADFLOAT), SW_HIDE);

		p_du = API.GetDataUnit( target_image_number );
		if(p_du == NULL)	return false;
		if(p_du->DataType != THREE_D_FORMAT && p_du->DataType != FLOAT_3D_FORMAT && p_du->DataType != RGB_3D_FORMAT)	return false;

		//�t�@�C���̃T�C�Y
		API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_WIDTH , p_du->DataSize.cx );
		API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_HEIGHT , p_du->DataSize.cy );
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_CHANNEL, p_du->ChannelNumber);
	
		::SetWindowText(m_hWnd, p_du->DataName + "�̕ۑ�");//�ۑ�����摜�f�[�^�̖��O

		//�ۑ�����f�[�^�̌`���ɂ��킹�đI���\�ȃ��W�I�{�^����ς���
		switch( p_du->DataType )
		{
			case THREE_D_FORMAT:
				change_filetype_radiobutton(FILE_TYPE_RAW);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), true);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), true);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), false);
				API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , p_du->PageNumber );
				break;

			case FLOAT_3D_FORMAT:
				change_filetype_radiobutton(FILE_TYPE_FLOAT);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), false);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), false);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), false);
				API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , p_du->PageNumber );
				break;

			case RGB_3D_FORMAT:
				change_filetype_radiobutton(FILE_TYPE_BMP);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), false);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), false);
				::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), false);
				API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , p_du->PageNumber );
				break;
		}

		disp_all_save_files();//�t�@�C���ꗗ
	}
	else//���[�h
	{
		DragAcceptFiles();//�h���b�O&�h���b�v

		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_RAW), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_BMP), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_FLOAT), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_IO3D_SERIAL_NAME), SW_HIDE);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_LOADBYTE), SW_SHOW);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_IO3D_LOADFLOAT), SW_SHOW);

		API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_WIDTH , 640 );
		API.SetEditValue( m_hWnd , IDC_EDIT_IO3D_HEIGHT , 480 );
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_CHANNEL, 1);
		API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , 1 );
		API.SetCheck(m_hWnd, IDC_RADIO_IO3D_LOADBYTE, true);

		::EnableWindow(::GetDlgItem(m_hWnd,IDC_EDIT_IO3D_FILE_PATH),false);
	}


	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/********************************************************************
�@  �\  ��  �� : �ĕ`��
��    ��    �� : OnPaint
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CIO3DDlg::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
		
	// �`��p���b�Z�[�W�Ƃ��� CDialog::OnPaint() ���Ăяo���Ă͂����܂���
}

/********************************************************************
�@  �\  ��  �� : �uOK�v�{�^���������ꂽ
��    ��    �� : OnOK
��          �� : 
��    ��    �l : 
�@          �\ : �ۑ��^�ǂݍ��ݎ��s���ă_�C�A���O�����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CIO3DDlg::OnOK() 
{
	if( do_store ){//�ۑ�
		if( !store() ){
			AfxMessageBox("�ۑ����s");
			return;
		}
	}else{//����
		if( !load() ){
			AfxMessageBox("�ǂݍ��ݎ��s");
			return;
		}
	}
	
	CDialog::OnOK();
}


//�ۑ�����S�Ẵt�@�C����\��
void CIO3DDlg::disp_all_save_files() 
{
	CDataUnit	*p_du;
	CListBox	*p_list;
	int		n;
	float	page_number;
	CString	serial_name, serial_number, file_name, extern_name;
	
	p_du = API.GetDataUnit( target_image_number );//�^�[�Q�b�g�̉摜�f�[�^���擾����
	if( p_du == NULL )	return;//�^�[�Q�b�g�̉摜��������΂Ȃɂ����Ȃ�

	//���X�g�{�b�N�X�̃|�C���^���擾
	p_list = (CListBox*)GetDlgItem( IDC_LISTBOX_IO3D_FILENAME );
	p_list->ResetContent();//���X�g�{�b�N�X�̒��g�����Z�b�g

	//�e�l���擾
	API.GetEditValue( m_hWnd , IDC_EDIT_IO3D_SERIAL_NAME , &serial_name );//�t�@�C���̘A����
	API.GetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , &page_number );//�摜�̉��s��	

	//�w�肵���y�[�W���摜�f�[�^�̉��s�������傫�����͉摜�f�[�^�̉��s���ɂ��킹��
	if(page_number > p_du->PageNumber)
	{
		page_number = (float)p_du->PageNumber;
		API.SetEditValue( m_hWnd , IDC_IO_3D_PAGE_NUMBER , page_number );
	}

	for(n=0 ; n<page_number ; n++)
	{
		serial_number.Format("%03d", n);//�����ԍ������߂�	

		//�g���q�����߂�
		if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_RAW) )				extern_name = _T(".RAW");
		else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_BMP))			extern_name = _T(".BMP");
		else if( API.GetCheck( m_hWnd , IDC_RADIO_IO3D_FLOAT))		extern_name = _T(".FLOAT");

		file_name = serial_name + serial_number + extern_name;//�t�@�C���� = �t�@�C���̘A���� + �����ԍ� + �g���q
			if(!do_store)//�ǂݍ��݂̎�
			{
				if(!DoFileExist(file_name))//�t�@�C�������݂��Ȃ��Ƃ�
				{
					AfxMessageBox( file_name + _T("�͂��̃f�B���N�g���ɂ͑��݂��܂���") );
					break;
				}
			}
		p_list->AddString( file_name );//���X�g�ɒǉ�����
	}	
}

/********************************************************************
�@  �\  ��  �� : �u�Q�Ɓv�{�^���������ꂽ
��    ��    �� : OnIo3dRefer
��          �� : 
��    ��    �l : 
�@          �\ : �ۑ����[�h�F�ۑ������A�̃t�@�C���̃t�@�C�����������t�@�C�������Q�Ƃ��č��
				 �ǂݍ��݃��[�h�F�ǂݍ��ޕ����̉摜�t�@�C����I������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//�u�Q�Ɓv�{�^���������ꂽ
void CIO3DDlg::OnIo3dRefer() 
{
	if( do_store ){//�ۑ�
		browse_for_store();
	}else{//����
		browse_for_load();
	}	
}

//OK�{�^������
void CIO3DDlg::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	OnOK();
}



/********************************************************************
�@  �\  ��  �� : �h���b�O�A���h�h���b�v
��    ��    �� : OnDropFiles
��          �� :
��    ��    �l :
�@          �\ : �摜���h���b�v���ꂽ�烊�X�g�{�b�N�X�ɒǉ�����
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
			Y.Ikeda         �V�K�쐬
********************************************************************/
void CIO3DDlg::OnDropFiles(HDROP hDropInfo)
{
	if (do_store){//�ۑ�
		
	}
	else//����
	{
		int		file_num = (int)DragQueryFileA(hDropInfo, -1, NULL, 0);//�h���b�v���ꂽ�t�@�C�������擾
		CString		file_name[10000], path;

		for (int i = 0; i<file_num; i++)
		{
			UINT size = DragQueryFile(hDropInfo, i, NULL, 0) + 1;//�t�@�C�����̒������擾

			if (size>3)
			{
				CString pathname;
				DragQueryFile(hDropInfo, i, pathname.GetBuffer(size), size);
				pathname.ReleaseBuffer();

				file_name[i] = pathname.Right(pathname.GetLength() - pathname.ReverseFind(_T('\\')) - 1);//�t�@�C��������

				if (i == 0){//�t�@�C���p�X����
					path = pathname.Left(pathname.ReverseFind(_T('\\')) +1);
				}
			}
		}
		DragFinish(hDropInfo);


		setlist_for_load(path, file_name, file_num);

	}
	CDialog::OnDropFiles(hDropInfo);
}






void CIO3DDlg::OnBnClickedRadioIo3dRaw()
{
	if (do_store){//�ۑ�
		disp_all_save_files();
	}
	else
	{//����
	}
}


void CIO3DDlg::OnBnClickedRadioIo3dBmp()
{
	if (do_store){//�ۑ�
		disp_all_save_files();
	}
	else
	{//����
	}
}


void CIO3DDlg::OnBnClickedRadioIo3dFloat()
{
	if (do_store){//�ۑ�
		disp_all_save_files();
	}
	else
	{//����
	}
}


void CIO3DDlg::OnEnChangeEditIo3dSerialName()
{
	if (do_store){//�ۑ�
		disp_all_save_files();
	}
	else
	{//����
	}
}




void CIO3DDlg::OnBnClickedIo3dDlgFolderBrowse()
{
	CString path;
	if (API.FolderDialog(&path))
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_IO3D_FILE_PATH, path + _T("\\"));//�p�X��\������
	}
}