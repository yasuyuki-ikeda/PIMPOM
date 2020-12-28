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
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#pragma comment(lib, "version.lib")
#define numof(array) (sizeof(array)/sizeof(array[0]))


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
//�����������E�I������		
///////////////////////////////////////////////////////////////////////////////////

//�^�C�g����key���܂ރE�B���h�E�n���h������������
static HWND find_window_from_key(HWND hWndParent, TCHAR key[])
{
	TCHAR szBuff[512];

	SendMessage(hWndParent, WM_GETTEXT, 255, (LPARAM)szBuff);
	if (_tcsstr(szBuff, key)){
		return hWndParent;
	}
	else{
		HWND hWndChild = ::FindWindowEx(hWndParent, NULL, _T("#32770"), NULL);
		if (hWndChild == NULL)	return NULL;

		do {
			HWND wnd = find_window_from_key(hWndChild, key);
			if (wnd)	return wnd;

		} while ((hWndChild = ::FindWindowEx(hWndParent, hWndChild, _T("#32770"), NULL)) != NULL);
	}
	return NULL;
}





CPimpomAPI::CPimpomAPI()
{
	//���[�h���X�_�C�A���O�̏�����******************************************:
	//�W���̃E�C���h�E
	pPIMMOMDlg=NULL;//���C���E�C���h�E�̃|�C���^
	pDataDlg=NULL;//�f�[�^�E�C���h�E�̃|�C���^
	pHistgramDlg=NULL;//�q�X�g�O�����E�C���h�E�̃|�C���^
	pFlterCtrlDlg=NULL;//�ėp�t�B���^�E�B���h�E�̃|�C���^
	pCalcDlg=NULL;//�v�Z�p
	pBinalizeDlg=NULL;//2�l��
	pBatchDlg=NULL;//�o�b�`����
	pWorkAreaDlg=NULL;
	pMarkDlg=NULL;//�}�[�N�\���p
	pDataPickDlg=NULL;//�f�[�^�̃s�b�N�A�b�v
	pSocketCtrlDlg=NULL;//�\�P�b�g�ʐM
	pChartDlg=NULL;//�O���t�\��
	pCmdPrptDlg=NULL;//�R�}���h�v�����v�g
	pPythonBatchDlg = NULL;//Python�o�b�`����


	//���[�U�_�C�A���O
	for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){
		custom_func[i].dllname.Empty();
		custom_func[i].caption.Empty();
		custom_func[i].hInst = NULL;
	}

	//���ݒ��������******************************************:
	data_number = DATA_NUMBER_DEFAULT;

	//�f�t�H���g�t�H���g
	default_font.lfHeight = 11;//����
	default_font.lfWidth = 0;//���ϕ��i�����j
	default_font.lfEscapement =0;//����p�x
	default_font.lfOrientation =0;//�����p�x	
	default_font.lfWeight =FW_THIN;//����
	default_font.lfItalic = false;//�Α�
	default_font.lfUnderline = false;//����
	default_font.lfStrikeOut =false;//�ŏ�����	
	default_font.lfCharSet = SHIFTJIS_CHARSET;//�����Z�b�g	
	default_font.lfOutPrecision = OUT_DEFAULT_PRECIS;//�o�͐��x	
	default_font.lfClipPrecision = CLIP_DEFAULT_PRECIS;//�N���b�s���O���x	
	default_font.lfQuality = DEFAULT_QUALITY;//�o�͕i��	
	default_font.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;//�s�b�`�E�t�@�~��	
	sprintf( default_font.lfFaceName, "�l�r�S�V�b�N");//���̖�	
	
	default_bg_color = RGB(190,190,190);//�W���̔w�i�F
	default_fg_color = RGB(0,255,0);//�W���̃O���t�B�b�N�F
	//Y��0.299�~R+0.587�~G+0.114�~B
	rgb_gain[0] = 299;//299;//R�Q�C��
	rgb_gain[1] = 587;//587;//G�Q�C��
	rgb_gain[2] = 114;//114;//B�Q�C��
	socket_delimiter =0;//�f���~�^

	invisible = false;//��ʕs����

	batch_execute_rule = 0;//�R�}���h�v�����v�g���o�b�`���s����Ƃ��̃��[��(0:�J�X�^���t�@���N�V�����̑O 1�F�J�X�^���t�@���N�V�����̌� 0,1�ȊO:���s���Ȃ�)



	//�摜�������ꗗ�\���E�B���h�E�ɕ\������摜�T�C�Y	
	small_image_size = CSize(SMALL_W, SMALL_H);


	//�摜������******************************************:
	//DataUnit�ւ̃|�C���^�z������
	pp_data_unit = (CDataUnit**)malloc(DATA_NUMBER_MAX * sizeof(CDataUnit**));
	for(int n=0 ; n<DATA_NUMBER_MAX ; n++){
		pp_data_unit[n] = NULL;
	}

	current_image_number = 0;//���ݑI�𒆂̉摜�ԍ���0�ŏ�����
	


	//�o�b�N�O���E���h������������******************
	background_proc_id = BGPROC_NOTHING;//
	

	backup_data = NULL;//�o�b�N�A�b�v�p�摜��������������
	//capture=NULL;
	pSockCtrl = NULL;


	//�����N�����̑Ή�(id�t�^)
	for (int i = 0; i < 10; i++)
	{
		TCHAR key[256];

		if (i == 0){
			_stprintf(key, " - PIMPOM  ");
		}
		else{
			_stprintf(key, " - PIMPOM %d", i);
		}

		HWND wnd = find_window_from_key(::GetDesktopWindow(), key);
		if (wnd == NULL){
			pimpom_id = i;
			break;
		}
	}
}

CPimpomAPI::~CPimpomAPI(void)
{
	//�e��E�B���h�E�̊J��
	DialogDelete(IDD_DATA_DLG);
	DialogDelete(IDD_FILTER_CTRL_DLG);
	DialogDelete(IDD_CALC_DLG);
	DialogDelete(IDD_BINALIZE_DLG);
	DialogDelete(IDD_BATCH_DLG);
	DialogDelete(IDD_MULTI_IMAGE_DISP_DLG); 
	DialogDelete(IDD_3D_IMAGE_DISP_DLG);
	DialogDelete(IDD_WORKAREA_DLG);
	DialogDelete(IDD_MARK_DLG);
	DialogDelete(IDD_HISTGRAM_DLG);
	DialogDelete(IDD_DATAPICK_DLG);
	DialogDelete(IDD_CHART_DLG);
	DialogDelete(IDD_SOCKET_CTRL_DLG);
	DialogDelete(IDD_COMMAND_RPOMPT_DLG);

	
	//���[�U�_�C�A���O�̊J��
	for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){	
		UserDialogDelete(i);
	}


	//�o�b�N�A�b�v�f�[�^�̊J��
	if(backup_data){
		delete	backup_data;
	}

	//�摜�������̊J��
	for(int n=0 ; n<data_number ; n++){
		if(pp_data_unit[n]){
			delete	pp_data_unit[n];
		}
	}
	free(pp_data_unit);

	//�r�f�I�L���v�`���̊J��
	CaptureDeInit();

	//�\�P�b�g�̊J��
	if(pSockCtrl){
		CloseSocket();
		delete pSockCtrl;
	}
}

/********************************************************************
�@  �\  ��  �� :ini�f�[�^��ǂݏo��
��    ��    �� : GetProfile
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
CString		CPimpomAPI::GetProfile(CString key1, CString key2)
{
	CWinApp *pApp=AfxGetApp();
	return pApp->GetProfileString(key1, key2, NULL);
}


/********************************************************************
�@  �\  ��  �� :ini�t�@�C���ɏ�������
��    ��    �� : WriteProfile
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void		CPimpomAPI::WriteProfile(CString key1, CString key2, CString str)
{
	CWinApp *pApp=AfxGetApp();
	pApp->WriteProfileString(key1, key2, str);
}

/********************************************************************
�@  �\  ��  �� :/INI�t�@�C������ݒ��ǂݍ���
��    ��    �� : InitSettingFromProfile
��          �� :
��    ��    �l :
�@          �\ :pimpom.ini�t�@�C������ȉ��̒l��ǂݍ���Őݒ肷��
�@�@�@�@�@�@�@�@ �E�摜��������
		 �@�@�@�@�E�ŏ��ɕ\������摜�������ԍ�
			 �@�@�E���C����ʂ̃T�C�Y�i���^�����j
			�@   �E�f�[�^�E�B���h�E�̕�
			   �@�E�T���l�C���摜�̃T�C�Y�i���^�����j
			�@�@ �E���[�U��`�_�C�A���O
				�i�E�摜�f�[�^�j
				�i�E�T���l�C���摜�̍s���j

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::InitSettingFromProfile()
{	
	CString key_path, key_w, key_h, key_page, buf;
	CString path;
	short	width, height, page;

	data_number =  _ttoi( GetProfile("SETTING", "IMG_NUM"));//�摜�������̐�
	if (data_number < 1) 
	{
		data_number = DATA_NUMBER_DEFAULT;
	}


	//�摜�������Ƀt�@�C�������
	for(int i=0 ; i<GetDataUnitNumber() ; i++ ){
		key_path.Format("IMG%d",i);
		key_w.Format("IMG%dW",i);
		key_h.Format("IMG%dH",i);
		key_page.Format("IMG%dPage",i);

		path = GetProfile("DEFAULT_IMAGE", key_path);
		if(!path.IsEmpty())
		{	
			buf = GetProfile("DEFAULT_IMAGE", key_w);
			if(buf.IsEmpty()){		width = 0;				}
			else{					width = _ttoi( buf );	}

			buf = GetProfile("DEFAULT_IMAGE", key_h);
			if(buf.IsEmpty()){		height = 0;				}
			else{					height = _ttoi( buf );	}

			buf = GetProfile("DEFAULT_IMAGE", key_page);
			if(buf.IsEmpty()){		page = 1;				}
			else{					page = _ttoi( buf );	}

			if( SelectAndLoadImageFile(i,path,width,height,page) )
			{ 
				GetDataUnit(i)->UpdateDispBuffer();
			}
		}
	}

	//�����I���摜�ԍ�
	current_image_number = _ttoi( GetProfile("SETTING", "DEFAULT_CURRENT_IMAGE"));
	if (current_image_number < 0 || current_image_number >= data_number) 
	{
		current_image_number = 0;
	}


	//���C���摜�̃T�C�Y
	main_image_size.cx = _ttoi( GetProfile("SETTING", "MAIN_IMG_W"));
	if (main_image_size.cx < 1) 
	{
		main_image_size.cx = MAIN_W;
	}

	main_image_size.cy = _ttoi( GetProfile("SETTING", "MAIN_IMG_H"));
	if (main_image_size.cy < 1)
	{
		main_image_size.cy = MAIN_H;
	}


	//���C���E�B���h�E�̃T�C�Y
	main_window_size.cx = MAIN_WIN_LEFT_MARGIN + main_image_size.cx + MAIN_WIN_RIGHT_MARGIN;
	main_window_size.cy = MAIN_WIN_TOP_MARGIN + main_image_size.cy + MAIN_WIN_BOTTOM_MARGIN;

	//�f�[�^�E�B���h�E�̕�
	data_window_width = _ttoi( GetProfile("SETTING", "DATA_WIN_W"));
	if (data_window_width < 1)
	{
		data_window_width = DATA_WIN_W;
	}

	//�P�s�ɕ\������T���l�C����
	thumbneil_colum = _ttoi(GetProfile("SETTING", "THUMBNEIL_COLMN"));
	if (thumbneil_colum < 1)
	{
		thumbneil_colum = SMALL_COLUM;
	}


	//�T���l�C���摜�T�C�Y	
	small_image_size.cx = _ttoi( GetProfile("SETTING", "SMALL_IMG_W"));
	if (small_image_size.cx < 1 || small_image_size.cx >= data_window_width) 
	{
		small_image_size.cx = SMALL_W;
	}

	small_image_size.cy = _ttoi( GetProfile("SETTING", "SMALL_IMG_H"));
	if (small_image_size.cy < 1)
	{
		small_image_size.cy = SMALL_H;
	}

	//�t�H���g�T�C�Y
	default_font.lfHeight = _ttoi( GetProfile("SETTING", "FONT_SIZE"));
	if (default_font.lfHeight  < 1)
	{
		default_font.lfHeight = FONT_SIZE;
	}


	CString  defDlg = GetProfile("SETTING", "DEFEULT_USER_DLG");
	invisible = _ttoi(GetProfile("SETTING", "INVISIBLE"));

	//�o�b�`���s����python�̃t�@�C���p�X
	python_batch_file = GetProfile("SETTING", "PYTHON_BATCH_FILE");


	//���[�U�_�C�A���O�̓ǂݍ���
	{
		FILE *fp;
		char buffer[1024];
		CString line, dllname, caption;
		HINSTANCE	hInst=NULL;
		int		count=0;

		//ini�t�@�C������g�p����dll����
		if( (fp = fopen(GetDefaultPath() + "\\custom_func.ini","rt")) != NULL){
			count=0;
			for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++)
			{
				if( fgets(buffer, 1024, fp) != NULL )
				{
					line = CString(buffer);//1���C���ǂݍ���
					int sprit = line.Find("=");
					if(sprit>0){
						dllname = line.Left(sprit) + ".dll";//dll��
						caption = line.Mid(sprit+1, line.GetLength());//�L���v�V����

						hInst = AfxLoadLibrary( dllname );//DLL���[�h����
						if(hInst != NULL){
							custom_func[count].dllname = dllname;
							custom_func[count].caption = caption;
							custom_func[count].hInst = hInst;

							if (defDlg == caption.Left(defDlg.GetLength()) && !defDlg.IsEmpty()){
								UserDialogOpen(count);
							}

							count++;
						}else{
							AfxMessageBox(dllname + _T("��������܂���"));
						}
					}
				}else{
					break;
				}
			}

			fclose(fp);
		}
	}
}




///////////////////////////////////////////////////////////////////////////////////
//�o�b�N�O���E���h����		
///////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �o�b�N�O���E���h������ID��Ԃ�
��    ��    �� : GetBGProcID
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CPimpomAPI::GetBGProcID()
{
	return  background_proc_id;
}

/********************************************************************
�@  �\  ��  �� : �o�b�N�O���E���h������ID��ݒ�
��    ��    �� : SetBGProcID
��          �� : int id(in)
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::SetBGProcID(int id)
{
	if(background_proc_id==BGPROC_NOTHING){
		background_proc_id=id;
	}
}

/********************************************************************
�@  �\  ��  �� : �o�b�N�O���E���h�����̏I��
��    ��    �� : StopBGProc
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::StopBGProc()
{
	background_proc_id=BGPROC_NOTHING;
}



//*********************************************************
// ����:
//   ���̊֐��́A�o�[�W�������\�[�X�̃R�s�[�ւ̃|�C���^�Ԃ��B
//   �o�[�W�������\�[�X�̃R�s�[�́A�g�p��ɉ�����Ȃ��Ă͂Ȃ�Ȃ��B
//
// �߂�l:
//     void * �c�c �֐������������ �o�[�W�������\�[�X�̃R�s�[�ւ̃|�C���^ ��
//            �@�@ �֐������s����� NULL ��Ԃ��B
//
// ����:
//     char   *FileName // �t�@�C����
//     DWORD **dwTrans  // �o�[�W�������\�[�X�̌�����
//*********************************************************
static void *GetVersionInfo( char *FileName, DWORD **dwTrans )
{
	void  *pVersionInfo; // �o�[�W�������\�[�X���w���|�C���^
	UINT   uDumy;        //
	DWORD  dwDumy;       //
	DWORD  dwSize;       // �o�[�W�������\�[�X�̃T�C�Y

	// �o�[�W�������\�[�X�̃T�C�Y���擾
	dwSize = GetFileVersionInfoSize( FileName, &dwDumy );
	if ( dwSize <= 0 )
	{
		// ���s
		return NULL;
	}

	pVersionInfo = malloc( dwSize );
	if ( !pVersionInfo )
	{
		// ���s
		return NULL;
	}

	// �o�[�W�������\�[�X���擾
	if ( GetFileVersionInfo( FileName, 0, dwSize, pVersionInfo ) )
	{
		// �o�[�W�������\�[�X�̌�������擾
		if ( VerQueryValue( pVersionInfo, "\\VarFileInfo\\Translation", (void **)dwTrans, &uDumy ) )
		{
			return pVersionInfo;
		}
	}
	free( pVersionInfo );

	return NULL; // ���s
}//GetVersionInfo

//*********************************************************
// ����:
//   �o�[�W�������\�[�X����A�o�[�W�������̃R�s�[���擾����B
//
// �߂�l:
//     BOOL �c�c �֐������������ TRUE  ���A
//          �@�@ �֐������s����� FALSE ��Ԃ��B
//
// ����:
//           char  *Buffer       �c�c �擾����o�[�W���������󂯎��o�b�t�@
//           void  *pVersionInfo �c�c �֐�GetVersionInfo() �̖߂�l
//           DWORD  dwTrans      �c�c �o�[�W�������\�[�X�̌�����
//     const char  *KeyWord      �c�c �擾�������o�[�W�������̖��O
//       "Comments"         // �R�����g
//       "CompanyName"      // ��Ж�
//       "FileDescription"  // ����
//       "FileVersion"      // �t�@�C���o�[�W����
//       "InternalName"     // ������
//       "LegalCopyright"   // ���쌠
//       "LegalTrademarks"  // ���W
//       "OriginalFilename" // �����t�@�C����
//       "PrivateBuild"     // �v���C�x�[�g�r���h���
//       "ProductName"      // ���i��
//       "ProductVersion"   // ���i�o�[�W����
//       "SpecialBuild"     // �X�y�V�����r���h���
//*********************************************************
static BOOL GetVersionValue( char *Buffer, void *pVersionInfo, DWORD dwTrans, const char *KeyWord )
{
	char *pValue; // �o�[�W�������\�[�X���̃o�[�W���������w���|�C���^
	char  Path[MAX_PATH+1];
	UINT  uDumy;
	WORD  wCodePageID[] = { 0, 932, 949, 950, 1200, 1250, 1251, 1252, 1253, 1254, 1255, 1256 };
	WORD  wLanguageID[] =
		{
			0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408,
			0x0409, 0x040A, 0x040B, 0x040C, 0x040D, 0x040E, 0x040F, 0x0410,
			0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418,
			0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F, 0x0420,
			0x0421, 0x0804, 0x0807, 0x0809, 0x080A, 0x080C, 0x0810,
			0x0813, 0x0814, 0x0816, 0x081A, 0x0C0C, 0x100C
		};

	sprintf( Path, "\\StringFileInfo\\%04x%04x\\%s", LOWORD(dwTrans), HIWORD(dwTrans), KeyWord );
	if ( VerQueryValue( pVersionInfo, Path, (void **)&pValue, &uDumy ) )
	{
		strcpy( Buffer, pValue );
		return TRUE; // ����
	}

	// �o�[�W�������\�[�X�Ɍ����񂪋L�^����Ă��Ȃ��ꍇ
	for( int i = 0; i < ( numof(wCodePageID) ); i++ )
	{
		for( int j = 0; j < numof(wLanguageID); j++ )
		{
			sprintf
			(
				Path,
				"\\StringFileInfo\\%04x%04x\\%s",
				wLanguageID[j],
				wCodePageID[i],
				KeyWord
			);
			if ( VerQueryValue( pVersionInfo, Path, (void **)&pValue, &uDumy ) )
			{
				strcpy( Buffer, pValue );
				return TRUE; // ����
			}
		}
	}

	Buffer[0] = '\0';
	return FALSE; // ���s
}//GetVersionValue

 

/********************************************************************
�@  �\  ��  �� : �o�[�W�������\�[�X���
��    ��    �� : GetVersion
��          �� : 
//     const char  *KeyWord      �c�c �擾�������o�[�W�������̖��O
//       "Comments"         // �R�����g
//       "CompanyName"      // ��Ж�
//       "FileDescription"  // ����
//       "FileVersion"      // �t�@�C���o�[�W����
//       "InternalName"     // ������
//       "LegalCopyright"   // ���쌠
//       "LegalTrademarks"  // ���W
//       "OriginalFilename" // �����t�@�C����
//       "PrivateBuild"     // �v���C�x�[�g�r���h���
//       "ProductName"      // ���i��
//       "ProductVersion"   // ���i�o�[�W����
//       "SpecialBuild"     // �X�y�V�����r���h���
��    ��    �l : �o�[�W�������\�[�X���
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CString CPimpomAPI::GetVersion(const char  *KeyWord)
{
	CString verStr;
	char   strVersion[100];
	void  *pVersionInfo;
	DWORD *dwTrans;
	CString filepath = GetDefaultPath() + "pimpom_core.dll";

	pVersionInfo = GetVersionInfo( filepath.GetBuffer(), &dwTrans );
	if ( pVersionInfo )
	{
		GetVersionValue( strVersion,  pVersionInfo, *dwTrans, KeyWord  );

		verStr.Format("%s", strVersion);

		free( pVersionInfo );
	}

	filepath.ReleaseBuffer();
	
	return verStr; 
}




/********************************************************************
�@  �\  ��  �� : ���l�𕶎���ɕϊ�����
��    ��    �� : ToString
��          �� : float				value				(in)���l
				 int				ordered_length		(in)�����w��(-1�̏ꍇ�͎w�肵�Ȃ�)
				 char				fillchar			(in)�����񒷂���ordered_length�ɖ����Ȃ��ꍇ�ɖ��߂镶��
				 char				filldir				(in)�����񒷂���ordered_length�ɖ����Ȃ��ꍇ�ɂǂ��炩�疄�߂邩 1�ȊO:������ 1:�E����
��    ��    �l : �ϊ����ꂽ������
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CString		CPimpomAPI::ToString(float value, int ordered_length, char fillchar, char filldir)
{
	CString		str;

	str.Format("%f", value);

	long len = str.GetLength();
	for(int i = len - 1  ;  i>0 ; i--){//������̉E�[��0�������������������
		if( str.GetAt(i) != '0' ){
			str = str.Left(i+1);
			break;
		}
	}

	//�ĉE�[��.�Ȃ�΂��������
	if (str.GetAt(str.GetLength() - 1) == '.') {
		str = str.Left(str.GetLength() - 1);
	}



	if(ordered_length>0)//������̒������w�肷��ꍇ
	{
		if( ordered_length > str.GetLength() ){//�����񂪎w�肳�ꂽ�������Z���ꍇ
			for (int i = str.GetLength(); i < ordered_length; i++) {
				if (filldir == 1) {//�E�[���疄�߂�
					str += CString(fillchar);
				}else {//���[���炤�߂�
					str = CString(fillchar) + str;
				}
			}
								
		}else if( ordered_length >0  &&  ordered_length < str.GetLength()  ){//�����񂪎w�肳�ꂽ������蒷���ꍇ
			str = str.Left( ordered_length );
		}
	}

	return	str;
}

/********************************************************************
�@  �\  ��  �� : ������𐔒l�ɕϊ�����
��    ��    �� : ToString
��          �� : CString			str				(in)������
				 double				*presult		(out)�ϊ����ꂽ���l
��    ��    �l : ����������true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::ToNumber(CString str, double *presult)
{
	double result = (float)atof( str );//������𐔒l�ɂ���i�G���[�̏ꍇ atof �́@0.0��Ԃ��j

	if(result == 0.0){//0.0�����l�Ƃ��ĕԂ��ꂽ�ꍇ
		//�l���{����0.0�̂Ƃ��i�G���[�łȂ��ꍇ�j�͕������ '0' , '.' , ' '�݂̂ō\������Ă���
		for(int i=0 ; i<str.GetLength() ; i++){
			if( str[i] != '0'   &&    str[i] != '.'   &&   str[i] != ' ')		return	false;
		}
	}

	*presult = result;

	return true;
}


/********************************************************************
�@  �\  ��  �� : �����x�^�C�}�[�擾
��    ��    �� : Tick
��          �� : 
��    ��    �l : PIMPOM�N��������̌o�ߎ���[ms]
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
DWORD	CPimpomAPI::Tick()
{
	LARGE_INTEGER nFreq, nTimeCnt;

	//�ϐ��̏�����
	memset(&nFreq, 0x00, sizeof nFreq);
	memset(&nTimeCnt, 0x00, sizeof nTimeCnt);

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nTimeCnt);


	return (DWORD)((nTimeCnt.QuadPart) * 1000 / nFreq.QuadPart);
}