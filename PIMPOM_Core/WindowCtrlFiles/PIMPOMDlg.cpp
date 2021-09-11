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
//���C���E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include <vfw.h>
#include "PIMPOMDlg.h"
#include "pimpom_plot.h"
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
extern CPimpomAPI	API;

// *************************************
//         ��    ��    �Q    ��         
// *************************************

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���Ă��� CAboutDlg �_�C�A���O
/////////////////////////////////////////////////////////////////////////////
class CAboutDlg : public CDialog
{
	enum { IDD = IDD_ABOUTBOX };// �_�C�A���O �f�[�^

public:
	CAboutDlg();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{

}

BOOL CAboutDlg::OnInitDialog()
{
	::SetWindowText(::GetDlgItem(m_hWnd,IDC_LABEL_VERSION), API.GetVersion("ProductVersion") );
	::SetWindowText(::GetDlgItem(m_hWnd,IDC_VERSION_COPYRIGHT), API.GetVersion("LegalCopyright") );
	return true;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
		// ���b�Z�[�W �n���h��������܂���B
END_MESSAGE_MAP()




unsigned __stdcall BackGroundThread(void* pArg)
{
	CPIMPOMDlg* pDlg = (CPIMPOMDlg*)pArg;

	pDlg->BackGroundProc();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPIMPOMDlg �_�C�A���O
/////////////////////////////////////////////////////////////////////////////


CPIMPOMDlg::CPIMPOMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPIMPOMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	hThread = NULL;
}

void CPIMPOMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPIMPOMDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOVE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_DROPFILES()
	ON_WM_INITMENU()
	ON_WM_MENUSELECT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_DATA_SELECT0, OnPimponDlgDataSelect0)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_DATA_SELECT1, OnPimponDlgDataSelect1)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_DATA_SELECT2, OnPimponDlgDataSelect2)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_DATA_SELECT3, OnPimponDlgDataSelect3)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_PAINT, OnPimponDlgButtonPaint)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_CALC, OnPimponDlgButtonCalc)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_WORK_AREA, OnPimponDlgButtonWorkArea)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_MARK, OnPimponDlgButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_MASK, OnPimponDlgButtonMask)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE, OnPimponDlgButtonRulerTool)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_SCALE_DBL, OnPimponDlgButtonScaleDbl)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_SCALE_HLF, OnPimponDlgButtonScaleHlf)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED, OnPimponDlgButtonScaleAdjustFiled)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_NO_SCALE, OnPimponDlgButtonNoScale)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE, &CPIMPOMDlg::OnBnClickedCheckVideoCapture)
	ON_BN_CLICKED(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2, &CPIMPOMDlg::OnBnClickedPimponDlgCheckVideoCapture2)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2, &CPIMPOMDlg::OnBnClickedButtonPimponDlgCalcDistance2)
	ON_BN_CLICKED(IDC_BUTTON_PIMPON_DLG_HISTGRAM, &CPIMPOMDlg::OnBnClickedButtonPimponDlgHistgram)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PIMPOM_DLG_MESSAGE, &CPIMPOMDlg::OnTcnSelchangeTabPimpomDlgMessage)
	ON_MESSAGE(WM_EXTERNAL_INPUT, OnMessageExtInput)
	ON_MESSAGE(WM_EXTERNAL_OUTPUT, OnMessageExtOutput)
	ON_MESSAGE(WM_EXTERNAL_PROCESS, OnMessageExtProcess)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_SERIAL, OnFileOpenSerial)
	ON_COMMAND(ID_FILE_SAVE_DATA, OnFileSaveData)
	ON_COMMAND(ID_FILE_SAVE_VIEW, OnFileSaveView)
	ON_COMMAND(ID_FILE_SAVE_IMAGE, OnFileSaveImage)
	ON_COMMAND(ID_FILE_SAVE_SERIAL, OnFileSaveSerial)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_QUIT, OnFileQuit)
	ON_COMMAND(ID_FILE_SAVE_ALLIMAGE, &CPIMPOMDlg::OnFileSaveAllimage)
	ON_COMMAND_RANGE(FILE_IO_LOG_MENU_ID, FILE_IO_LOG_MENU_ID + FILE_IO_LOG_MAX - 1, OnMenuLoadIOLog)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_CONVERT_TYPE, OnEditConvertType)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_COPY_CLIPBORD, OnEditCopyClipbord)
	ON_COMMAND(ID_EDIT_RENAME, &CPIMPOMDlg::OnEditRename)
	ON_COMMAND(ID_EDIT_PASTE_CLIPBORD, &CPIMPOMDlg::OnEditPasteClipbord)
	ON_COMMAND(ID_EDIT_DELETE_ALL, &CPIMPOMDlg::OnEditDeleteAll)
	ON_COMMAND(ID_EDIT_MARK, &CPIMPOMDlg::OnEditMark)
	ON_COMMAND(ID_EDIT_WORKAREA, &CPIMPOMDlg::OnEditWorkarea)
	ON_COMMAND(ID_EDIT_PAINT, &CPIMPOMDlg::OnEditPaint)
	ON_COMMAND(ID_VIEW_COLOR_GRAYSCALE, OnViewColorGrayscale)
	ON_COMMAND(ID_VIEW_COLOR_NEGA, OnViewColorNega)
	ON_COMMAND(ID_VIEW_COLOR_RAINBOW, OnViewColorRainbow)
	ON_COMMAND(ID_VIEW_COLOR_RAINBOW2, OnViewColorRainbow2)
	ON_COMMAND(ID_VIEW_COLOR_COSINE, OnViewColorCosine)
	ON_COMMAND(ID_VIEW_COLOR_YTOB, OnViewColorYtob)
	ON_COMMAND(ID_VIEW_COLOR_GTOR, OnViewColorGtor)
	ON_COMMAND(ID_VIEW_MAKEFONT, OnViewMakefont)
	ON_COMMAND(ID_VIEW_MULTI_IMAGE_DISP, &CPIMPOMDlg::OnViewMultiImageDisp)
	ON_COMMAND(ID_VIEW_COLOR_LABEL, &CPIMPOMDlg::OnViewColorLabel)
	ON_COMMAND(ID_VIEW_MARK, &CPIMPOMDlg::OnViewMark)
	ON_COMMAND(ID_VIEW_WORKAREA, &CPIMPOMDlg::OnViewWorkarea)
	ON_COMMAND(ID_VIEW_SETUP_ENVIRONMENT, &CPIMPOMDlg::OnViewSetupEnvironment)
	ON_COMMAND(ID_VIEW_3D_IMG_DISP, &CPIMPOMDlg::OnView3dImgDisp)
	ON_COMMAND(ID_IMAGE_RESIZE, OnEditResize)
	ON_COMMAND(ID_IMAGE_EQUATION, OnCalcImage)
	ON_COMMAND(ID_IMAGE_FILTER, OnFilter)
	ON_COMMAND(ID_IMAGE_TRANSFORM, &CPIMPOMDlg::OnImageTransform)
	ON_COMMAND(ID_IMAGE_BINALIZE, &CPIMPOMDlg::OnBinalize)
	ON_COMMAND(ID_TOOL_SOCKET, &CPIMPOMDlg::OnToolSocket)
	ON_COMMAND(ID_TOOL_Ruler, &CPIMPOMDlg::OnToolRuler)
	ON_COMMAND(ID_TOOL_PYTHON_BATCH, &CPIMPOMDlg::OnToolPythonBatch)
	ON_COMMAND(ID_TOOL_BATCH, &CPIMPOMDlg::OnToolBatch)
	ON_COMMAND(ID_TOOL_COMMAND_PROMPT, &CPIMPOMDlg::OnToolCommandPrompt)
	ON_COMMAND(ID_TOOL_HISTGRAM, &CPIMPOMDlg::OnToolHistgram)
	ON_COMMAND(ID_TOOL_VIDEOCAPTURE, &CPIMPOMDlg::OnToolVideocapture)
	ON_COMMAND(ID_CUSTOM_ADD, &CPIMPOMDlg::OnCustomAdd)
	ON_COMMAND_RANGE(CUSTOM_MENU_ID, CUSTOM_MENU_ID + CUSTOM_FUNC_MAX - 1, OnMenuCustomFunc)
	ON_COMMAND(ID_HELP_SPECIFICATION_SHEET, &CPIMPOMDlg::OnHelpSpecificationSheet)
	ON_COMMAND(ID_HELP_APIREFERENCE, &CPIMPOMDlg::OnHelpApireference)
	ON_COMMAND(ID_HELP_APIREFERENCE2, &CPIMPOMDlg::OnHelpApireference2)
	ON_COMMAND(ID_HELP_APIREFERENCE3, &CPIMPOMDlg::OnHelpApireference3)
	ON_COMMAND(ID_HELP_VERSION, OnHelpVersion)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�������Ɣj��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
BOOL CPIMPOMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "�o�[�W�������..." ���j���[���ڂ��V�X�e�� ���j���[�֒ǉ����܂��B

	// IDM_ABOUTBOX �̓R�}���h ���j���[�͈̔͂łȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	// ���̃_�C�A���O�p�̃A�C�R����ݒ肵�܂��B�t���[�����[�N�̓A�v���P�[�V�����̃��C��
	// �E�B���h�E���_�C�A���O�łȂ����͎����I�ɐݒ肵�܂���B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R����ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R����ݒ�

	//INI�t�@�C������ݒ��ǂݍ���
	API.InitSettingFromProfile();

	/////////////////////////////////////////////////////////
	//�\���摜�̊m��
	/////////////////////////////////////////////////////////
	p_image_disp_main = new CImageField( m_hWnd, API.main_image_size, CPoint(MAIN_WIN_LEFT_MARGIN, MAIN_WIN_TOP_MARGIN)  );//���C����ʂ�\��ImageFiled�̃C���X�^���X��
	p_image_disp_piece = new CImageField( m_hWnd, CSize(60, 60), CPoint(DATA_VALUE_DISP_FIELD_X, API.main_window_size.cy-140)  );//�w�肵�����W�̎��Ӊ�f��\������ImageField�̃C���X�^���X��

	API.pPIMMOMDlg = this;
	API.DialogOpen(IDD_DATA_DLG,true);
	API.DialogOpen(IDD_HISTGRAM_DLG,false);
	API.DialogOpen(IDD_WORKAREA_DLG,false);
	API.DialogOpen(IDD_MARK_DLG,false);


	/////////////////////////////////////////////////////////
	//�E�C���h�E�̕\���ݒ�
	/////////////////////////////////////////////////////////

	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem(IDC_TAB_PIMPOM_DLG_MESSAGE);
	p_wnd_tab->InsertItem(0, _T("���b�Z�[�W0"));
	p_wnd_tab->InsertItem(1, _T("���b�Z�[�W1"));

	load_bitmaps();//�{�^���ɓ\��t���邽�߂̃r�b�g�}�b�v���\�[�X�̃��[�h
	arrange_buttons();//���C���[�I���{�^���C�f�[�^�I���{�^���̔z�u
	set_bitmap_for_layer_button();//���C���[�I���{�^���Ƀr�b�g�}�b�v���֘A�Â���
	arrange_data_value_disp_field();//�w�肵�����W�̃f�[�^�l��\������̈�̔z�u
	arrange_message_disp_field();//���b�Z�[�W��\�����邽�߂̃_�C�A���O�A�C�e���̔z�u����
	CWnd *p_h_scroll = GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_H );
	CWnd *p_v_scroll = GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_V );
	p_h_scroll->MoveWindow( MAIN_WIN_LEFT_MARGIN , MAIN_WIN_TOP_MARGIN + API.main_image_size.cy , API.main_image_size.cx , 10 );//���C����ʂ̐��������X�N���[���o�[�̔z�u
	p_v_scroll->MoveWindow( MAIN_WIN_LEFT_MARGIN + API.main_image_size.cx , MAIN_WIN_TOP_MARGIN , 10, API.main_image_size.cy );//���C����ʂ̐��������X�N���[���o�[�̔z�u

	DragAcceptFiles();//�h���b�O���h���b�v������

	AVIFileInit();//AVI����

	//�c�[���`�b�v
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_PAINT), "�t���[�n���h�ŉ摜��ҏW");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC), "�����ŉ摜�𐶐�");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_MARK), "�}�[�N");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_WORK_AREA), "��Ɨ̈�");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_MASK), "�}�X�N");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_HISTGRAM), "�q�X�g�O����");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE), "���[���[");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2), "���[���[�ƃ��C���u���C�g");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_DBL), "�\���{���g��");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_HLF), "�\���{���k��");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED), "�E�B���h�E�T�C�Y�ɍ��킹�ĕ\���{���𒲐�");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_PIMPON_DLG_NO_SCALE), "���{�\��");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE), "���C�u�\��");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2), "���C�u���s");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0), "");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1), "");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2), "");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3), "");
	

	//�J�X�^���t�@���N�V���������j���[�ɒǉ�
	CString capts[CUSTOM_FUNC_MAX];
	int custon_func_cnt = API.GetUserDlgCaptions(capts);
	CMenu *menu = this->GetMenu();
	CMenu *pSub =menu->GetSubMenu(5);
	for(int i=0 ; i<custon_func_cnt ; i++){
		CString strcnt;
		strcnt.Format("%d ", i);
		pSub->AppendMenu(MF_STRING,CUSTOM_MENU_ID+i, strcnt + capts[i]);
	}

	/////////////////////////////////////////////////////////
	//�����o������
	////////////////////////////////////////////////////////
	is_draging = false;//�h���b�O����\���t���O��|��
	is_draging_mbutton = false;//�h���b�O����\���t���O��|��
	latest_clicked_coordinate_x = 0;//�ŋ߃N���b�N���ꂽ���W���N���A
	latest_clicked_coordinate_y = 0;
	calc_begin_x = 0;	//�����v�Z
	calc_begin_y = 0;
	calc_end_x = 0;	//�����v�Z
	calc_end_y = 0;

	UINT thrID = 0;
	hThread = (HANDLE)::_beginthreadex(NULL, 0, &BackGroundThread, this, 0, &thrID);


	//�A�C�R����Ƀh���b�O���ꂽ�t�@�C����ǂݍ���
	if(!cmdLine.IsEmpty())
	{
		CString filenames[30];
		int filenamecnt=0;
		bool filenameflg=false;

		//�R�}���h���C������t�@�C�����𕪗�����
		if(cmdLine.Find("\"")!=-1){//�t�@�C������""�ň͂܂�Ă���ꍇ
			for(int i=0 ; i<cmdLine.GetLength() ; i++){
				if(cmdLine[i]=='\"'){
					if(!filenameflg){
						filenames[filenamecnt].Empty();
						filenameflg=true;
					}else{
						filenamecnt++;
						filenameflg=false;
					}
				}else{
					filenames[filenamecnt] += CString(cmdLine[i]);
				}
			}
		}else{//�t�@�C������""�ň͂܂�Ă��Ȃ��ꍇ
			filenames[0] = cmdLine;
			filenamecnt=1;
		}

		//�t�@�C����ǂ݂���
		for(int n=0 ; n<filenamecnt ; n++){
			API.SelectAndLoadImageFile(n,filenames[n]);
			API.DrawImage(n,true,true);
		}
	}

	if (API.invisible)
	{//�E�B���h�E��\�����[�h
		API.pPIMMOMDlg->ShowWindow(SW_HIDE);
		API.pDataDlg->ShowWindow(SW_HIDE);
		API.pHistgramDlg->ShowWindow(SW_HIDE);
	}


	return TRUE;  // TRUE ��Ԃ��ƃR���g���[���ɐݒ肵���t�H�[�J�X�͎����܂���B
}

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�̔j��
��    ��    �� : OnDestroy
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnDestroy() 
{
	KillTimer(1);

	API.CaptureDeInit();//�r�f�I�L���v�`���̊J��

	AVIFileExit();

	CDialog::OnDestroy();
	
	delete		p_image_disp_piece;
	delete		p_image_disp_main;

}

void CPIMPOMDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����_�C�A���O�{�b�N�X�ɍŏ����{�^����ǉ�����Ȃ�΁A�A�C�R����`�悷��
// �R�[�h���ȉ��ɋL�q����K�v������܂��BMFC �A�v���P�[�V������ document/view
// ���f�����g���Ă���̂ŁA���̏����̓t���[�����[�N�ɂ�莩���I�ɏ�������܂��B


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
void CPIMPOMDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// �N���C�A���g�̋�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R����`�悵�܂��B
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{
		DrawImage(true);//���C����ʂ�`��

		CDialog::OnPaint();
	}

	if (API.invisible)//�E�B���h�E��\�����[�h
	{
		ShowWindow(SW_HIDE);
		API.pDataDlg->ShowWindow(SW_HIDE);
		API.pHistgramDlg->ShowWindow(SW_HIDE);
	}
}

// �V�X�e���́A���[�U�[���ŏ����E�B���h�E���h���b�O���Ă���ԁA
// �J�[�\����\�����邽�߂ɂ������Ăяo���܂��B
HCURSOR CPIMPOMDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/********************************************************************
�@  �\  ��  �� :�L�[�{�[�h����̓���
��    ��    �� : PreTranslateMessage
��          �� : 
��    ��    �l : 
�@          �\ : �J�[�\���L�[�F�J�[�\���̈ړ�
				 Del�L�[�F�I�𒆂̉摜�������N���A
				 Enter/Esc�L�[�F������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CPIMPOMDlg::PreTranslateMessage(MSG* pMsg) 
{
	POINT point;
	
	if(pMsg->message == WM_KEYDOWN ){	
		switch(pMsg->wParam){
			case VK_RETURN://Enter
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://��
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y - 1);
				return  true;

			case VK_DOWN://��
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y + 1);
				return  true;

			case VK_LEFT://��
				GetCursorPos( &point ) ;
				SetCursorPos(point.x - 1 , point.y );
				return  true;

			case VK_RIGHT://��
				GetCursorPos( &point ) ;
				SetCursorPos(point.x + 1 , point.y );
				return true;

			case VK_DELETE://Del
				OnEditDelete();
				break;

			case 'C'://Ctrl + C
				if((GetKeyState(VK_CONTROL)&0x8000)!=0){
					API.CopyToClipbord();
				}
				break;

			case 'V'://Ctrl + V
				if((GetKeyState(VK_CONTROL)&0x8000)!=0)
				{
					if (!API.pDataDlg->DataExistMessage("�㏑�����Ă���낵���ł����H", CURRENT_IMAGE))
					{
						API.BackupDataUnit(CURRENT_IMAGE);//���݂̃f�[�^�o�b�N�A�b�v
						API.PasteFromClipbord(API.GetCurrentImageNumber());//�N���b�v�{�[�h����y�[�X�g
					}
				}
				break;


			default:
				break;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		//�c�[���`�b�v�\��
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_PAINT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_MARK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_WORK_AREA)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_MASK)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_HISTGRAM)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_DBL)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_HLF)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_PIMPON_DLG_NO_SCALE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3)->m_hWnd
			)
		{
			m_toolTip.RelayEvent(pMsg);
		}
		else
		{
			m_toolTip.Pop();
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
�@  �\  ��  �� : �E�B���h�E�T�C�Y�ύX
��    ��    �� : OnSize
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnSize(UINT nType, int cx, int cy)
{
	static bool	init_flg = true;

	CDialog::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED)

	if (!init_flg)//��ԍŏ��ɌĂяo���ꂽ�Ƃ������͏������Ȃ�
	{
		//���C���E�B���h�E�ƃ��C���C���[�W�̃T�C�Y�ύX
		API.main_window_size.cx = cx;
		API.main_window_size.cy = cy;
		API.main_image_size.cx = API.main_window_size.cx - MAIN_WIN_LEFT_MARGIN - MAIN_WIN_RIGHT_MARGIN;
		API.main_image_size.cy = API.main_window_size.cy - MAIN_WIN_TOP_MARGIN - MAIN_WIN_BOTTOM_MARGIN;

		arrange_buttons();//���C���[�I���{�^���C�f�[�^�I���{�^���̔z�u
		arrange_data_value_disp_field();//�w�肵�����W�̃f�[�^�l��\������̈�̔z�u
		arrange_message_disp_field();//���b�Z�[�W��\�����邽�߂̃_�C�A���O�A�C�e���̔z�u����
		CWnd *p_h_scroll = GetDlgItem(IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_H);
		CWnd *p_v_scroll = GetDlgItem(IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_V);
		p_h_scroll->MoveWindow(MAIN_WIN_LEFT_MARGIN, MAIN_WIN_TOP_MARGIN + API.main_image_size.cy, API.main_image_size.cx, 10);//���C����ʂ̐��������X�N���[���o�[�̔z�u
		p_v_scroll->MoveWindow(MAIN_WIN_LEFT_MARGIN + API.main_image_size.cx, MAIN_WIN_TOP_MARGIN, 10, API.main_image_size.cy);//���C����ʂ̐��������X�N���[���o�[�̔z�u

		//���C���C���[�W�`��
		CDataUnit *p_du = API.GetDataUnit(CURRENT_IMAGE);
		p_image_disp_main->Resize(API.main_image_size);
		p_image_disp_main->DrawImage(p_du, true);//�`��;
		p_image_disp_piece->Move(CPoint(DATA_VALUE_DISP_FIELD_X, API.main_window_size.cy - 140));

		//���C���摜�O���ĕ`��
		CRect rightToolRegn(API.main_image_size.cx, 0, API.main_window_size.cx, API.main_window_size.cy);
		this->InvalidateRect(rightToolRegn);

		CRect bottomToolRegn(0, API.main_image_size.cy, API.main_image_size.cx, API.main_window_size.cy);
		this->InvalidateRect(bottomToolRegn);
	}

	init_flg = false;
}


/********************************************************************
�@  �\  ��  �� :���C���E�C���h�E���ړ�����
��    ��    �� : OnMove
��          �� : 
��    ��    �l : 
�@          �\ : �f�[�^�E�B���h�E�ƃq�X�g�O�����E�B���h�E�̈ʒu�������Ɉړ�����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnMove(int x, int y) 
{
	static bool	init_flg=true;

	CDialog::OnMove(x, y);
	
	if(API.pDataDlg != NULL)
	{
		RECT  win_rect, client_rect;
		int   mrgn_w, mrgn_h;//�E�B���h�E�̈�ƃN���C�A���g�̈�̃T�C�Y����

		GetWindowRect(&win_rect);//���݂̃E�B���h�E�̈��\����`
		GetClientRect(&client_rect);//���݂̃N���C�A���g�̈��\����`

		mrgn_w = (win_rect.right - win_rect.left) - (client_rect.right - client_rect.left);
		mrgn_h = (win_rect.bottom - win_rect.top) - (client_rect.bottom - client_rect.top);


		if(init_flg){//�ŏ�������ʍ���ɕ\��
			win_rect.top=0;
			win_rect.left=0;
			win_rect.right=API.main_window_size.cx;
			win_rect.bottom=MARK_WIN_H;
			init_flg=false;
		}

		//�f�[�^�E�B���h�E�̈ړ�
		API.pDataDlg->MoveWindow(win_rect.left, win_rect.top + API.main_window_size.cy + mrgn_h, API.data_window_width, API.pDataDlg->WindowHeight);


		//˽ĸ��уE�B���h�E�̈ړ�
		if(API.pHistgramDlg)
		{
			API.pHistgramDlg->MoveWindow(win_rect.left + API.main_window_size.cx + mrgn_w, win_rect.top, HISTGRAM_WIN_W, HISTGRAM_WIN_H);
		}
		
		//�}�[�N�E�B���h�E�̈ړ�
		if(API.pMarkDlg){
			API.pMarkDlg->MoveWindow(win_rect.left + API.main_window_size.cx + mrgn_w, win_rect.top + 20, MARK_WIN_W, MARK_WIN_H);
		}

		//��Ɨ̈�E�B���h�E�̈ړ�
		if(API.pWorkAreaDlg)
		{
			RECT clientRct = { 0 }, winodowRct = { 0 };
			::GetWindowRect(API.pWorkAreaDlg->m_hWnd, &winodowRct);//�E�B���h�E�̈��\����`���擾 
			::GetClientRect(API.pWorkAreaDlg->m_hWnd, &clientRct);//�N���C�A���g�̈��\����`���擾 
			int offset_h = winodowRct.bottom - winodowRct.top - clientRct.bottom;
			int offset_w = winodowRct.right - winodowRct.left - clientRct.right;

			API.pWorkAreaDlg->MoveWindow(win_rect.left + API.main_window_size.cx + mrgn_w, win_rect.top + HISTGRAM_WIN_H + 30, WORKAREA_WIN_W + offset_w, offset_h + WORKAREA_WIN_H);
		}

		//���̃E�C���h�E�̔z�u(�N���C�A���g�̈�Ƃ̍����z�����đ傫������)
		MoveWindow(win_rect.left, win_rect.top, API.main_window_size.cx + mrgn_w, API.main_window_size.cy + mrgn_h);
	}		
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�}�E�X�C�x���g�̂��߂̊֐�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� :�}�E�X�̍��{�^���������ꂽ
��    ��    �� : OnLButtonDown
��          �� : 
��    ��    �l : 
�@          �\ : �@�|�C���g�ʒu�̃f�[�^�l��\��
�@�@�@�@�@�@�@�@ �A�}�E�X�h���b�O�J�n
				 �B�e��N���b�N�C�x���g
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{

	GetCapture();//�}�E�X���L���v�`���[����
	is_draging = true;//�h���b�O����\���t���O�𗧂Ă�
	
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂� DataUnit ���擾
	CPoint		src_coor;
	double		src_coor_subpix_x, src_coor_subpix_y;
	if( p_du == NULL )		return;//�摜�f�[�^��������Ή������Ȃ�

	if( p_image_disp_main->GetFieldRect().PtInRect( point ) ) //���C���摜��ŃN���b�N�����ꍇ
	{
		if( p_image_disp_main->GetSrcCoor(p_du, point.x, point.y, &src_coor_subpix_x, &src_coor_subpix_y) )
		{
			src_coor.x = (int)(src_coor_subpix_x+0.5);
			src_coor.y = (int)(src_coor_subpix_y+0.5);

			if(!API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_SUBPIX) )//�T�u�s�N�Z���\�����Ȃ�
			{
				src_coor_subpix_x = src_coor.x;
				src_coor_subpix_y = src_coor.y;
			}


			//�N���b�N�����E�C���h�E��̍��W�ɑΉ�����摜���̍��W
			latest_clicked_coordinate_x = src_coor_subpix_x;
			latest_clicked_coordinate_y = src_coor_subpix_y;


			//��Ɨ̈�̃}�E�X�h���b�O�J�n�C�x���g
			if (API.pWorkAreaDlg) {
				API.pWorkAreaDlg->OnMouseDownOnImage(src_coor);
			}

			//�|�C���g�ʒu�̃f�[�^�l��\��
			if( API.GetCheck( m_hWnd, IDC_CHECK_PIMPON_DLG_FIX_COOR ) )
				DispDataValue( src_coor_subpix_x, src_coor_subpix_y);//���݃}�E�X�ŃN���b�N�������W(�摜�]���O)�̃f�[�^��\��

			//�}�[�N�̃}�E�X�N���b�N�C�x���g
			if(API.pMarkDlg)
			{
				API.pMarkDlg->OnMouseDownOnImage(src_coor_subpix_x, src_coor_subpix_y);
			}



			//�摜�f�[�^�̃t���[�n���h�ҏW���̃}�E�X�N���b�N�C�x���g
			if(API.pDataPickDlg){
				API.pDataPickDlg->OnMouseDownOnImage(src_coor);//�f�[�^�̃s�b�N/�y�C���g
			}

			//���[�U�_�C�A���O�̃}�E�X�N���b�N�C�x���g
			for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){	
				API.UserDialogOnMouseDown(i,src_coor);
			}

			DrawImage(false);//�ĕ`�悷��

		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�̍��{�^���������ꂽ
��    ��    �� : OnLButtonUp
��          �� : 
��    ��    �l : 
�@          �\ : �}�E�X�h���b�O�I��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();//�}�E�X�L���v�`���[���J������

	if(is_draging)//�}�E�X�h���b�O���I���������̏���
	{
		//�f�[�^�ɑ΂��鏈��
		CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂� DataUnit ���擾
		CPoint		src_coor;
		double		src_coor_subpix_x, src_coor_subpix_y;
		if( p_du )
		{
			if( p_image_disp_main->GetFieldRect().PtInRect( point ) ) //�}�E�X�|�C���^�����C���摜��ɂ���ꍇ
			{
				if( p_image_disp_main->GetSrcCoor(p_du, point.x, point.y, &src_coor_subpix_x, &src_coor_subpix_y) )
				{
					src_coor.x = (int)(src_coor_subpix_x+0.5);
					src_coor.y = (int)(src_coor_subpix_y+0.5);

					if(!API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_SUBPIX) )//�T�u�s�N�Z���\�����Ȃ�
					{
						src_coor_subpix_x = src_coor.x;
						src_coor_subpix_y = src_coor.y;
					}

					//��Ɨ̈�̃}�E�X�h���b�O�I���C�x���g
					if (API.pWorkAreaDlg) {
						API.pWorkAreaDlg->OnMouseUpOnImage(CPoint(latest_clicked_coordinate_x, latest_clicked_coordinate_y), src_coor);
					}


					//�}�[�N�̃}�E�X�h���b�O�I���C�x���g
					if(API.pMarkDlg){
						API.pMarkDlg->OnMouseUpOnImage(latest_clicked_coordinate_x, latest_clicked_coordinate_y, src_coor_subpix_x, src_coor_subpix_y);
					}


					//���[�U�_�C�A���O�̃}�E�X�h���b�O�I���C�x���g
					for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){	
						API.UserDialogOnFinishMouseDrug(i, CPoint(latest_clicked_coordinate_x, latest_clicked_coordinate_y), src_coor);
					}
				}
			}
		}
	}

	is_draging = false;//�}�E�X�h���b�O��\���t���O��|��
	
	CDialog::OnLButtonUp(nFlags, point);
}


/********************************************************************
�@  �\  ��  �� :�}�E�X�̒����{�^���������ꂽ
��    ��    �� : OnMButtonDown
��          �� :
��    ��    �l :
�@          �\ : �摜�\���ʒu�̈ړ�
  ���t         �쐬��          ���e
 ------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	GetCapture();//�}�E�X���L���v�`���[����
	is_draging_mbutton = true;//�h���b�O����\���t���O�𗧂Ă�


	CDataUnit	*p_du = API.GetDataUnit(CURRENT_IMAGE);//���ݑI�𒆂� DataUnit ���擾
	if (p_du == NULL)		return;//�摜�f�[�^��������Ή������Ȃ�

	double		src_coor_subpix_x, src_coor_subpix_y;
	if (p_image_disp_main->GetFieldRect().PtInRect(point)) //���C���摜��ŃN���b�N�����ꍇ
	{
		if (p_image_disp_main->GetSrcCoor(p_du, point.x, point.y, &src_coor_subpix_x, &src_coor_subpix_y))
		{
			//�N���b�N�����E�C���h�E��̍��W�ɑΉ�����摜���̍��W
			latest_mbutton_clicked_coordinate_x = src_coor_subpix_x;
			latest_mbutton_clicked_coordinate_y = src_coor_subpix_y;
		}
	}
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�̒����{�^���������ꂽ
��    ��    �� : OnMButtonUp
��          �� :
��    ��    �l :
�@          �\ : �}�E�X�h���b�O�I��
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();//�}�E�X�L���v�`���[���J������
	is_draging_mbutton = false;
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�̃|�C���^�̒ʉ�
��    ��    �� : OnMouseMove
��          �� : 
��    ��    �l : 
�@          �\ : �@�|�C���g�ʒu�̃f�[�^�l��\��
				 �A�}�E�X�h���b�O��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂� DataUnit ���擾
	CPoint		src_coor;
	double		src_coor_subpix_x, src_coor_subpix_y;

	if( p_du )
	{
		if( p_image_disp_main->GetFieldRect().PtInRect( point ) ) //���C���摜����}�E�X�|�C���^�����؂�ꍇ

			if( p_image_disp_main->GetSrcCoor(p_du, point.x, point.y, &src_coor_subpix_x, &src_coor_subpix_y) )
			{
				src_coor.x = (int)(src_coor_subpix_x+0.5);
				src_coor.y = (int)(src_coor_subpix_y+0.5);

				if(!API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_SUBPIX) )//�T�u�s�N�Z���\�����Ȃ�
				{
					src_coor_subpix_x = src_coor.x;
					src_coor_subpix_y = src_coor.y;
				}

				//�|�C���g�ʒu�̃f�[�^�l��\��
				if( ! API.GetCheck( m_hWnd, IDC_CHECK_PIMPON_DLG_FIX_COOR ) ){
					DispDataValue( src_coor_subpix_x, src_coor_subpix_y);//���݃}�E�X�Ń|�C���g���̉摜�f�[�^��\��
				}

				if(is_draging)//�}�E�X�h���b�O���̏���
				{
					//��Ɨ̈�̃}�E�X�h���b�O�C�x���g
					if (API.pWorkAreaDlg) {
						API.pWorkAreaDlg->OnMouseDrugOnImage(CPoint(latest_clicked_coordinate_x, latest_clicked_coordinate_y), src_coor);
					}


					//�h���b�O�n�_�E���ݓ_�̋����v�Z
					if( API.GetCheck( m_hWnd , IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE ) ){
						calc_begin_x = latest_clicked_coordinate_x;
						calc_begin_y = latest_clicked_coordinate_y;
						calc_end_x = src_coor_subpix_x;
						calc_end_y = src_coor_subpix_y;
						DrawImage(false);//�ĕ`��
					}

					//�t���[�n���h�ҏW�̃}�E�X�h���b�O�C�x���g
					if(API.pDataPickDlg){
						API.pDataPickDlg->OnMouseDrugOnImage(src_coor);//�f�[�^�̃y�C���g
					}


					//�}�[�N�̃}�E�X�h���b�O�C�x���g
					if(API.pMarkDlg){
							API.pMarkDlg->OnMouseDrugOnImage(latest_clicked_coordinate_x, latest_clicked_coordinate_y,src_coor_subpix_x, src_coor_subpix_y);
					}

					//���[�U�_�C�A���O�̃}�E�X�h���b�O�C�x���g
					for(int i=0 ; i<CUSTOM_FUNC_MAX ; i++){	
						API.UserDialogOnMouseDrug(i, CPoint(latest_clicked_coordinate_x, latest_clicked_coordinate_y), src_coor);
					}
				}


				if (is_draging_mbutton)//�}�E�X�����{�^���h���b�O���̏���
				{
					p_image_disp_main->ScrollImage(p_du, CPoint(p_du->DataDispOffset.x - (src_coor_subpix_x - latest_mbutton_clicked_coordinate_x )
															, p_du->DataDispOffset.y - (src_coor_subpix_y - latest_mbutton_clicked_coordinate_y)));//�摜�]�����̋�`�̈���X�N���[��������
					DrawImage(true);//�ĕ`��
				}


				//�摜�f�[�^���ڕҏW���̓J�[�\����ύX����
				if(API.pDataPickDlg){
					API.pDataPickDlg->OnMouseMoveOnImage(src_coor);
				}
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


/********************************************************************
�@  �\  ��  �� :�}�E�X�z�C�[���̉�]
��    ��    �� : OnMouseWheel
��          �� :
��    ��    �l :
�@          �\ : �\���摜�̊g��E�k��
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CPIMPOMDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CDataUnit	*p_du = API.GetDataUnit(CURRENT_IMAGE);
	if (p_du != NULL)
	{
		//�X�P�[����ύX���ĕ`��
		float scale = p_du->DispScale;
		if (zDelta>0){
			scale *= 1.1;//10%�g��
		}
		else{
			scale *= 0.9;//10%�k��
		}

		API.ScaleImage(CURRENT_IMAGE, scale);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʕ\���E�`��
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : ���C����ʕ`��
��    ��    �� : DrawImage
��          �� : bool redraw_all (in)true�ɂ���ƁC�w�i�ƃX�N���[���o�[���ĕ`�悳���
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::DrawImage(bool redraw_all)
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾
	CSize		src_size;
	CScrollBar	*p_h_scroll, *p_v_scroll;//�X�N���[���o�[�̃|�C���^
	int			scroll_condition_h = SW_HIDE;//���������X�N���[���o�[�̉���Ԃ�\���ϐ��i�s����Ԃŏ������j
	int			scroll_condition_v = SW_HIDE;//���������X�N���[���o�[�̉���Ԃ�\���ϐ��i�s����Ԃŏ������j

		if(redraw_all){//�w�i�C�X�N���[���o�[�C�f�[�^�\���{�^�����̍ĕ`��
			p_h_scroll = (CScrollBar*)GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_H );
			p_v_scroll = (CScrollBar*)GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_V );

				if( p_du != NULL ){//�f�[�^������ꍇ�̏���
					//�X�N���[���o�[��ݒ肷��
					p_image_disp_main->GetSrcSize(p_du, &src_size);//�摜�]�����̋�`�̈�̃T�C�Y���擾

						if( src_size.cx < p_du->DataSize.cx ){//���������X�N���[���o�[�������Ƃ����摜�]�����̕����摜�S�̂̕���菬�����ꍇ
							set_scrollbar( *p_h_scroll, src_size.cx, p_du->DataDispOffset.x, 0, p_du->DataSize.cx - 1);
							scroll_condition_h = SW_SHOW;
						}

						if( src_size.cy < p_du->DataSize.cy ){//���������X�N���[���o�[�������Ƃ����摜�]�����̍������摜�S�̂̍�����菬�����ꍇ
							set_scrollbar( *p_v_scroll, src_size.cy, p_du->DataDispOffset.y, 0, p_du->DataSize.cy - 1);
							scroll_condition_v = SW_SHOW;
						}

					API.SetCheck( m_hWnd , IDC_BUTTON_PIMPON_DLG_MASK , p_du->DoUseMask );//�}�X�N�̏�Ԃ�\������
					API.SetEditValue( m_hWnd , IDC_EDIT_PIMPON_DLG_SCALE , p_du->DispScale );//�k�ڂ�\������

						if( API.GetCheck( m_hWnd, IDC_CHECK_PIMPON_DLG_FIX_COOR ) )//�N���b�N���W��\������ꍇ
							DispDataValue( latest_clicked_coordinate_x, latest_clicked_coordinate_y );
				}
					
			p_h_scroll->ShowWindow(scroll_condition_h);//�X�N���[���o�[�̉���Ԑݒ�
			p_v_scroll->ShowWindow(scroll_condition_v);
			
			set_bitmap_for_data_button();//�\���f�[�^�I���{�^��
		}

	message[0].Empty();//���b�Z�[�W�{�b�N�X�N���A
	disp_information_titlebar();//�^�C�g���o�[�ɏ��\��
	p_image_disp_main->DrawImage( p_du , redraw_all);//�`��

	draw_2points_distance();//2�_�Ԃ̋���
	draw_work_area();//��Ɨ̈�
	draw_marks();//�}�[�N

	//���[�U�_�C�A���O�̕`��C�x���g
	for (int i = 0; i<CUSTOM_FUNC_MAX; i++){
		API.UserDialogOnDrawMainImage(i, API.GetCurrentImageNumber());
	}
}

/********************************************************************
�@  �\  ��  �� : ���C����ʂ�ImageField�ւ̃|�C���^��Ԃ�
��    ��    �� : GetMainImage
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CImageField* CPIMPOMDlg::GetMainImage()
{
	return	p_image_disp_main;
}

/********************************************************************
�@  �\  ��  �� : �X�N���[���o�[��ݒ肷��
��    ��    �� : set_scrollbar
��          �� : CScrollBar		&sc		(out)�X�N���[���o�[�̃R���g���[���Q��
				 int			page	(in)�y�[�W��
				 int			pos		(in)�ʒu
				 int			min		(in)�ŏ��ʒu
				 int			max		(in)�ő�ʒu
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::set_scrollbar(CScrollBar &sc, int page, int pos, int min, int max)
{
	SCROLLINFO  scrollinfo;
	sc.GetScrollInfo( &scrollinfo , SIF_ALL );
	scrollinfo.nPage = page;
	scrollinfo.nPos  = pos ;
	scrollinfo.nMin  = min;
	scrollinfo.nMax  = max;
	sc.SetScrollInfo( &scrollinfo , true );	
}

/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : control_main_image_scrollbar
��          �� : UINT			nSBCode		(in)
				 UINT			nPos		(in)
				 CScrollBar		*pScrollBar	(in)�X�N���[���o�[�̃R���g���[���ւ̃|�C���^
��    ��    �l : �����̃X�N���[���o�[�̈ʒu
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
long CPIMPOMDlg::control_main_image_scrollbar(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	int dest_pos = pScrollBar->GetScrollPos();

		switch(nSBCode){
			case SB_LEFT:			//�t�H�[�J�X�������Ă���Ƃ�[Home]���������Ƃ�
				break;

			case SB_RIGHT:			//�t�H�[�J�X�������Ă���Ƃ�[End]���������Ƃ�
				break;

			case SB_LINELEFT:		//�����{�^���N���b�N
				dest_pos -= 10;		break;

			case SB_LINERIGHT:		//�E���{�^���N���b�N
				dest_pos += 10;		break;

			case SB_PAGELEFT:		//���y�[�W�̈�N���b�N
				dest_pos -= 10;		break;

			case SB_PAGERIGHT:		//�E�y�[�W�̈�N���b�N
				dest_pos += 10;		break;

			case SB_THUMBPOSITION:	//�܂݃h���b�N�ŖړI�n�ɒB����
				dest_pos = nPos;	break;

			case SB_THUMBTRACK:		//�܂݃h���b�O���ł���
				dest_pos = nPos;	break;

			case SB_ENDSCROLL:		//�X�N���[���o�[�������I������
				break;
		}
	pScrollBar->SetScrollPos(dest_pos);//�X�N���[���o�[�ĕ\��

	return	dest_pos;
}

/********************************************************************
�@  �\  ��  �� : ���������X�N���[���o�[�����삳�ꂽ
��    ��    �� : OnHScroll
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾
	CPoint		src_offset;
	long		new_pos;

		if( pScrollBar == GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_H ) ){//���C����ʂ̃X�N���[���o�[�ł���

				if(p_du == NULL)	return;//�f�[�^��������Ή������Ȃ�

			p_image_disp_main->GetSrcOffset(p_du, &src_offset);//�摜�]�����̋�`�̈�̃I�t�Z�b�g�i����̍��W�j���擾
			new_pos = control_main_image_scrollbar(nSBCode, nPos, pScrollBar);//�X�N���[���o�[����V�����I�t�Z�b�g���擾
			src_offset.x = new_pos;//�V�����I�t�Z�b�g�ɐݒ�
			p_image_disp_main->ScrollImage( p_du , src_offset);//�摜�]�����̋�`�̈���X�N���[��������
			DrawImage(false);//�ĕ`��
		}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/********************************************************************
�@  �\  ��  �� : ���������X�N���[���o�[�����삳�ꂽ
��    ��    �� : OnVScroll
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾
	CPoint		src_offset;
	long		new_pos;

		if( pScrollBar ==  GetDlgItem( IDC_SCROLL_PIMPOM_DLG_MAIN_IMAGE_V ) ){//���C����ʂ̃X�N���[���o�[�ł���

				if(p_du == NULL)	return;//�f�[�^��������Ή������Ȃ�

			p_image_disp_main->GetSrcOffset(p_du, &src_offset);//�摜�]�����̋�`�̈�̃I�t�Z�b�g�i����̍��W�j���擾
			new_pos = control_main_image_scrollbar(nSBCode, nPos, pScrollBar);//�X�N���[���o�[����V�����I�t�Z�b�g���擾
			src_offset.y = new_pos;//�V�����I�t�Z�b�g�ɐݒ�
			p_image_disp_main->ScrollImage( p_du , src_offset);//�摜�]�����̋�`�̈���X�N���[��������
			DrawImage(false);//�ĕ`��

			
		}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

/********************************************************************
�@  �\  ��  �� : �^�C�g���o�[�ɉ摜�̖��O��f�[�^��ނ�\��
��    ��    �� : disp_information_titlebar
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::disp_information_titlebar()
{
	CString		info, type_name, size, page;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );

		if( p_du != NULL){
			page.Empty();
			switch( p_du->DataType ){
				case BYTE_FORMAT:
					type_name.LoadString(IDS_DATATYPE_BYTE);			break;

				case FLOAT_FORMAT:
					type_name.LoadString(IDS_DATATYPE_FLOAT);		break;

				case THREE_D_FORMAT:
					type_name.LoadString(IDS_DATATYPE_3D);	
					page.Format("  %d/%d ", p_du->DispPage+1, p_du->PageNumber);		break;

				case FLOAT_3D_FORMAT:
					type_name.LoadString(IDS_DATATYPE_F3D);	
					page.Format("  %d/%d ", p_du->DispPage+1, p_du->PageNumber);		break;

				case COMPLEX_FORMAT:
					type_name.LoadString(IDS_DATATYPE_COMPLEX);			break;

				case XY_FORMAT:
					type_name.LoadString(IDS_DATATYPE_XY);		break;

				case XYZ_FORMAT:
					type_name.LoadString(IDS_DATATYPE_XYZ);			break;

				case RGB_FORMAT:
					type_name.LoadString(IDS_DATATYPE_RGB);			break;

				case SHORT16_FORMAT:
					type_name.LoadString(IDS_DATATYPE_SHORT);				break;

				case WORD_FORMAT:
					type_name.LoadString(IDS_DATATYPE_WORD);				break;

				case LONG32_FORMAT:
					type_name.LoadString(IDS_DATATYPE_LONG);				break;

				case DWORD_FORMAT:
					type_name.LoadString(IDS_DATATYPE_DWORD);				break;

				case RGB_3D_FORMAT:
					type_name.LoadString(IDS_DATATYPE_RGB3D);
					page.Format("  %d/%d ", p_du->DispPage+1, p_du->PageNumber);		break;

			}
			size.Format("(%d�~%d)", p_du->DataSize.cx, p_du->DataSize.cy );//�T�C�Y��\������
			info = p_du->DataName + _T("[") + type_name + page + _T("]") + size + _T(" - PIMPOM");

		}else{//�f�[�^���Ȃ���΂��̃\�t�g�̖��O��\������
			info = _T("Platform for IMage Processing Observer and Modifier - PIMPOM");
		}

	CString idstr("  ");//ID�t�^
	if (API.pimpom_id > 0){
		idstr.Format(" %d", API.pimpom_id);
	}

	SetWindowText(info + idstr);//�^�C�g���o�[�ɕ\������
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��ʉE�[�̃{�^���Ɋւ��鏈��
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �{�^���ɕ\������r�b�g�}�b�v��ǂݍ���
��    ��    �� : load_bitmaps
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::load_bitmaps()
{
	//�r�b�g�}�b�v��ǂݍ���
	bmp_button_3d[0].LoadBitmap(IDB_BUTTON_3D_0);
	bmp_button_3d[1].LoadBitmap(IDB_BUTTON_3D_1);
	bmp_button_3d[2].LoadBitmap(IDB_BUTTON_3D_2);
	bmp_button_3d[3].LoadBitmap(IDB_BUTTON_3D_3);

	bmp_button_complex[0].LoadBitmap(IDB_BUTTON_COMPLEX_0);
	bmp_button_complex[1].LoadBitmap(IDB_BUTTON_COMPLEX_1);
	bmp_button_complex[2].LoadBitmap(IDB_BUTTON_COMPLEX_2);
	bmp_button_complex[3].LoadBitmap(IDB_BUTTON_COMPLEX_3);

	bmp_button_xy[0].LoadBitmap(IDB_BUTTON_XY_0);
	bmp_button_xy[1].LoadBitmap(IDB_BUTTON_XY_1);

	bmp_button_xyz[0].LoadBitmap(IDB_BUTTON_XYZ_0);
	bmp_button_xyz[1].LoadBitmap(IDB_BUTTON_XYZ_1);
	bmp_button_xyz[2].LoadBitmap(IDB_BUTTON_XYZ_2);
	bmp_button_xyz[3].LoadBitmap(IDB_BUTTON_XYZ_3);

	bmp_button_rgb[0].LoadBitmap(IDB_BUTTON_RGB_R);
	bmp_button_rgb[1].LoadBitmap(IDB_BUTTON_RGB_G);
	bmp_button_rgb[2].LoadBitmap(IDB_BUTTON_RGB_B);
	bmp_button_rgb[3].LoadBitmap(IDB_BUTTON_RGB);

	bmp_button_paint.LoadBitmap(IDB_BUTTON_PAINT);
	bmp_button_calc.LoadBitmap(IDB_BUTTON_CALC);
	bmp_button_mark.LoadBitmap(IDB_BUTTON_MARK);
	bmp_button_work_area.LoadBitmap(IDB_BUTTON_WORK_AREA);
	bmp_button_mask.LoadBitmap(IDB_BUTTON_MASK);
	bmp_button_histgram.LoadBitmap(IDB_BUTTON_HISTGRAM);

	bmp_button_work_area_edit.LoadBitmap(IDB_BUTTON_WORK_AREA_EDIT);
	bmp_button_mark_edit.LoadBitmap(IDB_BUTTON_MARK_EDIT);
	bmp_button_calc_distance.LoadBitmap(IDB_BUTTON_CALC_DISTANCE);
	bmp_button_calc_distance2.LoadBitmap(IDB_BUTTON_CALC_DISTANCE2);

	bmp_button_scale_adjust_field.LoadBitmap(IDB_BUTTON_SCALE_ADJUST_FIELD);
	bmp_button_no_scale.LoadBitmap(IDB_BUTTON_NO_SCALE);

	bmp_button_video.LoadBitmap(IDB_BUTTON_VIDEO);
	bmp_button_video2.LoadBitmap(IDB_BUTTON_VIDEO2);

	bmp_button_inc.LoadBitmap(IDB_BUTTON_INCLIMENT);
	bmp_button_dec.LoadBitmap(IDB_BUTTON_DECLIMENT);
}

/********************************************************************
�@  �\  ��  �� : �{�^����z�u����
��    ��    �� : arrange_buttons
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::arrange_buttons()
{
	//�\���f�[�^�̎�ޑI���{�^���i�}���`�`���l���摜����ѓ���̂Ƃ��Ɏg���j
	CButton	*p_button_d0 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
	CButton	*p_button_d1 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
	CButton	*p_button_d2 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
	CButton	*p_button_d3 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3);

	p_button_d0->MoveWindow( API.main_window_size.cx-35 , DATA_SELECT_FIELD_Y      , 30 , 30);
	p_button_d1->MoveWindow( API.main_window_size.cx-35 , DATA_SELECT_FIELD_Y + 30 , 30 , 30);
	p_button_d2->MoveWindow( API.main_window_size.cx-35 , DATA_SELECT_FIELD_Y + 60 , 30 , 30);
	p_button_d3->MoveWindow( API.main_window_size.cx-35 , DATA_SELECT_FIELD_Y + 90 , 30 , 30);

	//�e��@�\�̕\���E�ҏW�{�^��
	CButton	*p_button_p0 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_MARK);//�}�[�N�\��
	CButton	*p_button_p1 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_WORK_AREA);//��Ɨ̈�\��
	CButton	*p_button_p2 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_MASK);//�}�X�N�\��
	
	CButton	*p_button_t0 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_HISTGRAM);//�q�X�g�O�����\��
	CButton *p_button_t1 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE);//2�_�Ԃ̋�������c�[��
	CButton *p_button_t2 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2);//2�_�Ԃ̋�������c�[�����O���t�ƘA��������
	CButton	*p_button_t3 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_PAINT);//�t���[�n���h�ŉ摜��ҏW
	CButton	*p_button_t4 = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_CALC);//�����ŉ摜�𐶐�



	p_button_p0->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 0  , 30 , 30);
	p_button_p1->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 30  , 30 , 30);
	p_button_p2->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 60  , 30 , 30);

	p_button_t0->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 100 , 30 , 30);
	p_button_t1->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 130  , 30 , 30);
	p_button_t2->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 160  , 30 , 30);
	p_button_t3->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 190  , 30 , 30);
	p_button_t4->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 220  , 30 , 30);

	//�\���摜�̃X�P�[���ύX�@�\
	CEdit	*p_edit_scale = (CEdit*)GetDlgItem(IDC_EDIT_PIMPON_DLG_SCALE);
	CButton	*p_button_scale_dbl = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_DBL);
	CButton	*p_button_scale_hlf = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_HLF);
	CButton	*p_button_scale_adjust_filed = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED);
	CButton	*p_button_no_scale = (CButton*)GetDlgItem(IDC_BUTTON_PIMPON_DLG_NO_SCALE);

	p_edit_scale->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 340  , 30 , 20);
	p_button_scale_dbl->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 360  , 15 , 15);
	p_button_scale_hlf->MoveWindow( API.main_window_size.cx-35+15 , LAYER_SELECT_FIELD_Y + 360  , 15 , 15);
	p_button_scale_adjust_filed->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 375  , 30 , 20);
	p_button_no_scale->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 395  , 30 , 20);

	//�r�f�I�L���v�`��
	CButton	*p_chk_video = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE);
	p_chk_video->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 430  , 30 , 30);
	

	CButton	*p_chk_video2 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2);
	p_chk_video2->MoveWindow( API.main_window_size.cx-35 , LAYER_SELECT_FIELD_Y + 460  , 30 , 30);

	if (API.IsCaptureOpened() )
	{
		p_chk_video->ShowWindow(SW_SHOW);
		p_chk_video2->ShowWindow(SW_SHOW);
	}
	else {
		p_chk_video->ShowWindow(SW_HIDE);
		p_chk_video2->ShowWindow(SW_HIDE);
	}
	
}

/********************************************************************
�@  �\  ��  �� : ����̃{�^���ƃr�b�g�}�b�v���֘A�Â���
��    ��    �� : set_bitmap_button
��    ��    �l : HBITMAP	h_bmp	(in)�r�b�g�}�b�v�n���h��
				 UINT		id		(in)�{�^���̃_�C�A���OID
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
HBITMAP CPIMPOMDlg::set_bitmap_button(HBITMAP h_bmp, UINT id)
{
	CButton	*p_button = (CButton*)GetDlgItem(id);
	return	p_button->SetBitmap( h_bmp );
}

/********************************************************************
�@  �\  ��  �� : ���ׂẴ{�^���Ƀr�b�g�}�b�v���֘A�Â���
��    ��    �� : set_bitmap_for_layer_button
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::set_bitmap_for_layer_button()
{
	set_bitmap_button( (HBITMAP)bmp_button_paint, IDC_BUTTON_PIMPON_DLG_PAINT);
	set_bitmap_button( (HBITMAP)bmp_button_calc, IDC_BUTTON_PIMPON_DLG_CALC);
	set_bitmap_button( (HBITMAP)bmp_button_mark, IDC_BUTTON_PIMPON_DLG_MARK );
	set_bitmap_button( (HBITMAP)bmp_button_work_area, IDC_BUTTON_PIMPON_DLG_WORK_AREA );
	set_bitmap_button( (HBITMAP)bmp_button_mask, IDC_BUTTON_PIMPON_DLG_MASK );
	set_bitmap_button( (HBITMAP)bmp_button_histgram, IDC_BUTTON_PIMPON_DLG_HISTGRAM );
	set_bitmap_button( (HBITMAP)bmp_button_calc_distance, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE);
	set_bitmap_button( (HBITMAP)bmp_button_calc_distance2, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2);
	set_bitmap_button( (HBITMAP)bmp_button_scale_adjust_field, IDC_BUTTON_PIMPON_DLG_SCALE_ADJUST_FILED);
	set_bitmap_button( (HBITMAP)bmp_button_no_scale, IDC_BUTTON_PIMPON_DLG_NO_SCALE);
	set_bitmap_button( (HBITMAP)bmp_button_video, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE);
	set_bitmap_button( (HBITMAP)bmp_button_video2, IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2); 
	set_bitmap_button((HBITMAP)bmp_button_inc, IDC_BUTTON_PIMPON_DLG_SCALE_DBL);
	set_bitmap_button((HBITMAP)bmp_button_dec, IDC_BUTTON_PIMPON_DLG_SCALE_HLF);
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�ɉ����ă{�^���Ɋ֘A�Â���r�b�g�}�b�v��ς���
��    ��    �� : set_bitmap_for_data_button
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::set_bitmap_for_data_button()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾����
	CButton	*p_button_d0 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
	CButton	*p_button_d1 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
	CButton	*p_button_d2 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
	CButton	*p_button_d3 = (CButton*)GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
	


		if(p_du == NULL){//�f�[�^��������ΑS�Ẵf�[�^�I���{�^�����\���ɂ���
			p_button_d0->ShowWindow( SW_HIDE );
			p_button_d1->ShowWindow( SW_HIDE );
			p_button_d2->ShowWindow( SW_HIDE );
			p_button_d3->ShowWindow( SW_HIDE );

		}else{
			switch( p_du->DataType ){
				case BYTE_FORMAT:
					//not break
				case SHORT16_FORMAT:
					//not break
				case WORD_FORMAT:
					//not break
				case LONG32_FORMAT:
					//not break
				case DWORD_FORMAT:
					//not break
				case FLOAT_FORMAT:
					p_button_d0->ShowWindow( SW_HIDE );
					p_button_d1->ShowWindow( SW_HIDE );
					p_button_d2->ShowWindow( SW_HIDE );
					p_button_d3->ShowWindow( SW_HIDE );
					break;

				case THREE_D_FORMAT:
					//not break
				case FLOAT_3D_FORMAT:
					//not break
				case RGB_3D_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_SHOW );
					p_button_d3->ShowWindow( SW_SHOW );
					set_bitmap_button( (HBITMAP)bmp_button_3d[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_3d[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_bitmap_button( (HBITMAP)bmp_button_3d[2] , IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
					set_bitmap_button( (HBITMAP)bmp_button_3d[3] , IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
					m_toolTip.UpdateTipText( "�A���\��", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0));
					m_toolTip.UpdateTipText("�\���y�[�W����i�߂�", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1) );
					m_toolTip.UpdateTipText("�\���y�[�W����߂�", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2) );
					m_toolTip.UpdateTipText("�A���\�����~�߂�", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3) );
					
						if( API.GetBGProcID()==BGPROC_MOVIE )		set_data_select_button(0);//�r�f�I�Đ���
						else										set_data_select_button(3);//�r�f�I��~��
					break;		

				case COMPLEX_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_SHOW );
					p_button_d3->ShowWindow( SW_SHOW );
					set_bitmap_button( (HBITMAP)bmp_button_complex[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_complex[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_bitmap_button( (HBITMAP)bmp_button_complex[2] , IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
					set_bitmap_button( (HBITMAP)bmp_button_complex[3] , IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
					set_data_select_button( p_du->DispChannel );
					m_toolTip.UpdateTipText("����", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0) );
					m_toolTip.UpdateTipText("����", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1) );
					m_toolTip.UpdateTipText("��Βl", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2) );
					m_toolTip.UpdateTipText("�Ίp", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3));
					break;

				case XY_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_HIDE );
					p_button_d3->ShowWindow( SW_HIDE );
					set_bitmap_button( (HBITMAP)bmp_button_xy[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_xy[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_data_select_button( p_du->DispChannel );
					m_toolTip.UpdateTipText("X����", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0));
					m_toolTip.UpdateTipText("Y����", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1));
					break;
	
				case XYZ_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_SHOW );
					p_button_d3->ShowWindow( SW_SHOW );
					set_bitmap_button( (HBITMAP)bmp_button_xyz[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_xyz[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_bitmap_button( (HBITMAP)bmp_button_xyz[2] , IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
					set_bitmap_button( (HBITMAP)bmp_button_xyz[3] , IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
					set_data_select_button( p_du->DispChannel );
					m_toolTip.UpdateTipText("X����", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0) );
					m_toolTip.UpdateTipText("Y����", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1) );
					m_toolTip.UpdateTipText("Z����", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2) );
					m_toolTip.UpdateTipText("�_�Q", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3) );
					break;

				case RGB_FORMAT:
					p_button_d0->ShowWindow( SW_SHOW );
					p_button_d1->ShowWindow( SW_SHOW );
					p_button_d2->ShowWindow( SW_SHOW );
					p_button_d3->ShowWindow( SW_SHOW );
					set_bitmap_button( (HBITMAP)bmp_button_rgb[0] , IDC_CHECK_PIMPON_DLG_DATA_SELECT0);
					set_bitmap_button( (HBITMAP)bmp_button_rgb[1] , IDC_CHECK_PIMPON_DLG_DATA_SELECT1);
					set_bitmap_button( (HBITMAP)bmp_button_rgb[2] , IDC_CHECK_PIMPON_DLG_DATA_SELECT2);
					set_bitmap_button( (HBITMAP)bmp_button_rgb[3] , IDC_CHECK_PIMPON_DLG_DATA_SELECT3);
					m_toolTip.UpdateTipText("�Ԑ���", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT0) );
					m_toolTip.UpdateTipText("�ΐ���", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT1) );
					m_toolTip.UpdateTipText("����", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT2) );
					m_toolTip.UpdateTipText("RGB�J���[", GetDlgItem(IDC_CHECK_PIMPON_DLG_DATA_SELECT3) );
						if( p_du->DispRGB == false )	set_data_select_button( p_du->DispChannel );//�F�����\��
						else							set_data_select_button( 3 );			//RGB�J���[�\��
					break;
			}
		}
}

/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�̎�ނɉ����ă{�^���̉�����Ԃ�ύX����
��    ��    �� : set_data_select_button
��    ��    �l : long type
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::set_data_select_button(long type)
{
	int i;
	bool	do_disp[4];

		for(i=0 ; i<4 ; i++)	do_disp[i] = false;
	do_disp[ type ] = true;	

	API.SetCheck(m_hWnd, IDC_CHECK_PIMPON_DLG_DATA_SELECT0, do_disp[0] );
	API.SetCheck(m_hWnd, IDC_CHECK_PIMPON_DLG_DATA_SELECT1, do_disp[1] );
	API.SetCheck(m_hWnd, IDC_CHECK_PIMPON_DLG_DATA_SELECT2, do_disp[2] );
	API.SetCheck(m_hWnd, IDC_CHECK_PIMPON_DLG_DATA_SELECT3, do_disp[3] );
}

/********************************************************************
�@  �\  ��  �� : �\���f�[�^�I���{�^���������ꂽ
��    ��    �� : OnPimponDlgDataSelect
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnPimponDlgDataSelect0() 
{
	set_data_select_button(0);//�\���f�[�^�I���{�^���̃`�F�b�N�̏�ԕύX
	change_disp_data_by_button(0);//�\���f�[�^�̕ύX
}

void CPIMPOMDlg::OnPimponDlgDataSelect1() 
{
	set_data_select_button(1);//�\���f�[�^�I���{�^���̃`�F�b�N�̏�ԕύX
	change_disp_data_by_button(1);//�\���f�[�^�̕ύX
}

void CPIMPOMDlg::OnPimponDlgDataSelect2() 
{
	set_data_select_button(2);//�\���f�[�^�I���{�^���̃`�F�b�N�̏�ԕύX
	change_disp_data_by_button(2);//�\���f�[�^�̕ύX
}

void CPIMPOMDlg::OnPimponDlgDataSelect3() 
{
	set_data_select_button(3);//�\���f�[�^�I���{�^���̃`�F�b�N�̏�ԕύX
	change_disp_data_by_button(3);//�\���f�[�^�̕ύX
}

/********************************************************************
�@  �\  ��  �� : �����ꂽ�{�^���ɉ����ĕ\���摜��ύX���čĕ`��
��    ��    �� : change_disp_data_by_button
��    ��    �l : short button_number  (in)�����ꂽ�{�^���̔ԍ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::change_disp_data_by_button(short button_number)
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾����

		if(p_du == NULL)	return;//�f�[�^��������Ή������Ȃ�

		switch( p_du->DataType ){
			case BYTE_FORMAT:
				//not break;
			case SHORT16_FORMAT:
				//not break;
			case WORD_FORMAT:
				//not break;
			case LONG32_FORMAT:
				//not break;
			case DWORD_FORMAT:
				//not break;
			case FLOAT_FORMAT:
				break;

			case THREE_D_FORMAT:
				//not break;
			case FLOAT_3D_FORMAT:
				//not break;
			case RGB_3D_FORMAT:
					switch( button_number ){
						case 0://�A���Đ�
							API.SetBGProcID(BGPROC_MOVIE);
							break;
						case 1://�R�}����
							p_du->MoveDispPage( 1 );
							API.DrawImage(CURRENT_IMAGE, true);
							break;
						case 2://�t�R�}����
							p_du->MoveDispPage( -1 );
							API.DrawImage(CURRENT_IMAGE, true);
							break;
						case 3://�Đ����~
							if(API.GetBGProcID()==BGPROC_MOVIE){
								API.StopBGProc();
							}
							break;
					}
				break;

			case COMPLEX_FORMAT:
				//not break
			case XY_FORMAT:
				p_du->SetDispChannel(button_number);
				API.DrawImage(CURRENT_IMAGE, true);
				break;

			case XYZ_FORMAT:
					if( button_number !=3 ){
						p_du->SetDispChannel(button_number);

					}else{
						CPointCloudDlg	pcd;
						pcd.CreatePointCloud(p_du);//3�������f���\��
					}
				API.DrawImage(CURRENT_IMAGE, true);
				break;

			case RGB_FORMAT:
					if( button_number !=3 ){
						p_du->SetDispChannel(button_number);

					}else{
						p_du->SetDispChannel(-1);
					}
				API.DrawImage(CURRENT_IMAGE, true);
				break;
		}
}

/********************************************************************
�@  �\  ��  �� : �t���[�n���h�ŉ摜��ҏW
��    ��    �� : OnPimponDlgButtonPaint
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonPaint() 
{
	if (API.GetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_PAINT)) {
		API.DialogOpen(IDD_DATAPICK_DLG, true);
	}else {
		API.DialogOpen(IDD_DATAPICK_DLG, false);
	}
}

/********************************************************************
�@  �\  ��  �� : �����ŉ摜���쐬
��    ��    �� : OnPimponDlgButtonCalc
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonCalc() 
{
	if (API.GetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC)) {
		API.DialogOpen(IDD_CALC_DLG, true);
	}
	else {
		API.DialogOpen(IDD_CALC_DLG, false);
	}
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̕\���{�^���������ꂽ
��    ��    �� : OnPimponDlgButtonWorkArea
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonWorkArea() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾

	if(p_du)//�I�𒆂̉摜������������΁C��Ɨ̈�̕\����Ԃ�ύX����
	{
			if( API.GetCheck( m_hWnd, IDC_BUTTON_PIMPON_DLG_WORK_AREA ) )//��Ɨ̈���g�p�ł���悤�ɂ���Ƃ�
			{
				p_du->DoUseWorkArea = true;//
				if(API.pWorkAreaDlg){
					API.pWorkAreaDlg->ShowWindow(SW_SHOW);//��Ɨ̈�E�B���h����\��
				}
			}
			else//��Ɨ̈���g�p�ł��Ȃ��悤�ɂ���Ƃ�
			{
				p_du->DoUseWorkArea = false;//
				if(API.pWorkAreaDlg){
					API.pWorkAreaDlg->ShowWindow(SW_HIDE);//��Ɨ̈�E�B���h�����\��
				}
				API.SetWorkAreaEditMode(0);
			}
		API.DrawImage( CURRENT_IMAGE , true );//��Ɨ̈���̕\���f�[�^���X�V���čĕ`�悷��
	}
	else//�摜��������������Ή������Ȃ�
	{
		if(API.pWorkAreaDlg){
			API.pWorkAreaDlg->ShowWindow(SW_HIDE);//��Ɨ̈�E�B���h�����\��
		}
		return;
	}
}



/********************************************************************
�@  �\  ��  �� : �}�[�N�̕\���{�^���������ꂽ
��    ��    �� : OnPimponDlgButtonMark
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonMark() 
{
	if( API.GetCheck( m_hWnd, IDC_BUTTON_PIMPON_DLG_MARK )){
		API.SetMarkEditMode(1);
	}else{
		API.SetMarkEditMode(0);
	}
}



/********************************************************************
�@  �\  ��  �� : �q�X�g�O�����̕\���{�^���������ꂽ
��    ��    �� : OnBnClickedButtonPimponDlgHistgram
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnBnClickedButtonPimponDlgHistgram()
{
	if(API.GetCheck(m_hWnd,IDC_BUTTON_PIMPON_DLG_HISTGRAM)){
		API.DialogOpen(IDD_HISTGRAM_DLG,true);
	}else{
		API.DialogOpen(IDD_HISTGRAM_DLG,false);
	}
}

/********************************************************************
�@  �\  ��  �� : 2�_�Ԃ̋����v�Z�{�^���������ꂽ
��    ��    �� : OnPimponDlgButtonRulerTool
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonRulerTool() 
{
	API.SetRulerToolMode(API.GetCheck( m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE ));
}

/********************************************************************
�@  �\  ��  �� : 2�_�Ԃ̋����v�Z�ƃO���t��A��������{�^���������ꂽ
��    ��    �� : OnBnClickedButtonPimponDlgCalcDistance2
��    ��    �l : 
�@          �\ : 2�_�Ԃ̋����v�Z��ON�ɂ��A�O���t�E�B���h�E��\������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnBnClickedButtonPimponDlgCalcDistance2()
{
	if(API.GetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2) ){
		API.SetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE,true);
		API.DialogOpen(IDD_CHART_DLG, true);
	}
}

/********************************************************************
�@  �\  ��  �� : �}�X�N�\���{�^���������ꂽ
��    ��    �� : OnPimponDlgButtonMask
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonMask() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂�DataUnit���擾

		if(p_du == NULL)		return;//�f�[�^��������΂Ȃɂ����Ȃ�

	p_du->DoUseMask = API.GetCheck( m_hWnd, IDC_BUTTON_PIMPON_DLG_MASK );//�}�X�N
	API.DrawImage( CURRENT_IMAGE, true);//�ĕ`��
}


/********************************************************************
�@  �\  ��  �� : �\���摜�̔{���~2�{�^���������ꂽ
��    ��    �� : OnPimponDlgButtonScaleDbl
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
			 Y.Ikeda		 ���ݕ\������Ă��钆�S���W��ۂ����܂܊g�嗦�ύX
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonScaleDbl() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;//���ݑI�𒆂̉摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�

	API.ScaleImage(CURRENT_IMAGE, p_du->DispScale*2);
}

/********************************************************************
�@  �\  ��  �� : �\���摜�̔{��/2�{�^���������ꂽ
��    ��    �� : OnPimponDlgButtonScaleHlf
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
			 Y.Ikeda		 ���ݕ\������Ă��钆�S���W��ۂ����܂܊g�嗦�ύX
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonScaleHlf() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
	if(p_du == NULL)	return;//���ݑI�𒆂̉摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�

	API.ScaleImage(CURRENT_IMAGE, p_du->DispScale/2.0);
}

/********************************************************************
�@  �\  ��  �� : �\���摜�����C����ʂ̑傫���ɂ��킷�{�^���������ꂽ
��    ��    �� : OnPimponDlgButtonScaleAdjustFiled
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonScaleAdjustFiled() 
{
	CDataUnit	*p_du;

	p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;//���ݑI�𒆂̉摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�

	p_image_disp_main->DrawAdjustedImage(p_du, true);//�k�ڕύX
	DrawImage(true);//�ĕ`��
}

/********************************************************************
�@  �\  ��  �� : �\���摜���t���X�P�[���\������{�^���������ꂽ
��    ��    �� : OnPimponDlgButtonNoScale
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
			 Y.Ikeda		 ���ݕ\������Ă��钆�S���W��ۂ����܂܊g�嗦�ύX
********************************************************************/
void CPIMPOMDlg::OnPimponDlgButtonNoScale() 
{
	CDataUnit	*p_du;

	p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;//���ݑI�𒆂̉摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�

	//p_du->SetDispScale( 1 );//�k�ڕύX
	p_image_disp_main->ScaleImage(p_du,1);

	DrawImage(true);//�ĕ`��	
}

/********************************************************************
�@  �\  ��  �� : �r�f�I�L���v�`���[�X���[�\���{�^���������ꂽ
��    ��    �� : OnBnClickedCheckVideoCapture
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnBnClickedCheckVideoCapture()
{
	if(API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE)){
		API.SetVideoCaptureThroughMode(1);
	}else{
		API.SetVideoCaptureThroughMode(0);
	}
}

/********************************************************************
�@  �\  ��  �� : �r�f�I�L���v�`���[ +�o�b�`���s�@�{�^���������ꂽ
��    ��    �� : OnBnClickedPimponDlgCheckVideoCapture2
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnBnClickedPimponDlgCheckVideoCapture2()
{
	if(API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2)){
		API.SetVideoCaptureThroughMode(2);
	}else{
		API.SetVideoCaptureThroughMode(0);
	}
}

/********************************************************************
�@  �\  ��  �� : �|�C���g���W�l��\�����邽�߂̃_�C�A���O�A�C�e���̔z�u
��    ��    �� : arrange_data_value_disp_field
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::arrange_data_value_disp_field()
{
	CSize	picture(60,60) , value_disp(60,15) , coor_disp(200, 15); 

	//1�_���ӂ̉摜��\�����邽�߂̃s�N�`���[�{�b�N�X��z�u����
	CWnd	*p_wnd_picture = GetDlgItem( IDC_PICT_PIMPON_DLG_DISP_PIECE );
	p_wnd_picture->MoveWindow( DATA_VALUE_DISP_FIELD_X, API.main_window_size.cy-140, picture.cx, picture.cy );

	//1�_�̉�f�̃f�[�^�l��\�����邽�߂̃G�f�B�b�g�{�b�N�X��z�u����
	CWnd	*p_wnd_value_1_1 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE_1_1 );
	CWnd	*p_wnd_value0_1 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE0_1 );
	CWnd	*p_wnd_value1_1 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE1_1 );
	CWnd	*p_wnd_value_10 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE_10 );
	CWnd	*p_wnd_value00 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE00 );
	CWnd	*p_wnd_value10 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE10 );
	CWnd	*p_wnd_value_11 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE_11 );
	CWnd	*p_wnd_value01 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE01 );
	CWnd	*p_wnd_value11 = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_VALUE11 );

	p_wnd_value_1_1->MoveWindow(DATA_VALUE_DISP_FIELD_X + picture.cx + 10						, API.main_window_size.cy-140 + value_disp.cy	, value_disp.cx , value_disp.cy);
	p_wnd_value0_1->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)	, API.main_window_size.cy-140 + value_disp.cy	, value_disp.cx , value_disp.cy);
	p_wnd_value1_1->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)*2	, API.main_window_size.cy-140 + value_disp.cy	, value_disp.cx , value_disp.cy);
	p_wnd_value_10->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10						, API.main_window_size.cy-140 + value_disp.cy*2	, value_disp.cx , value_disp.cy);
	p_wnd_value00->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)	, API.main_window_size.cy-140 + value_disp.cy*2	, value_disp.cx , value_disp.cy);
	p_wnd_value10->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)*2	, API.main_window_size.cy-140 + value_disp.cy*2	, value_disp.cx , value_disp.cy);
	p_wnd_value_11->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10						, API.main_window_size.cy-140 + value_disp.cy*3	, value_disp.cx , value_disp.cy);
	p_wnd_value01->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)	, API.main_window_size.cy-140 + value_disp.cy*3	, value_disp.cx , value_disp.cy);
	p_wnd_value11->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 + (value_disp.cx+5)*2	, API.main_window_size.cy-140 + value_disp.cy*3	, value_disp.cx , value_disp.cy);

	//1�_�̍��W��\�����邽�߂̃G�f�B�b�g�{�b�N�X��z�u����
	CWnd	*p_wnd_coor_ij = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_COOR_IJ );
	CWnd	*p_wnd_coor_xy = (CWnd*)GetDlgItem( IDC_EDIT_PIMPON_DLG_COOR_XY );
	p_wnd_coor_ij->MoveWindow(	DATA_VALUE_DISP_FIELD_X + picture.cx + 10 , API.main_window_size.cy-140	, coor_disp.cx , coor_disp.cy);

	//�N���b�N���W�Œ�{�^��
	CWnd	*p_wnd_filx_coor = (CWnd*)GetDlgItem( IDC_CHECK_PIMPON_DLG_FIX_COOR );
	p_wnd_filx_coor->MoveWindow( DATA_VALUE_DISP_FIELD_X, API.main_window_size.cy-140 + picture.cy + 5, 150, 15 );
	
	
	//�T�u�s�N�Z�����W�\���{�^��
	CWnd	*p_wnd_subpix_coor = (CWnd*)GetDlgItem( IDC_CHECK_PIMPON_DLG_SUBPIX );
	p_wnd_subpix_coor->MoveWindow( DATA_VALUE_DISP_FIELD_X + 150, API.main_window_size.cy-140 + picture.cy + 5, 120, 15 );
}


/********************************************************************
�@  �\  ��  �� : ���b�Z�[�W��\�����邽�߂̃_�C�A���O�A�C�e���̔z�u
��    ��    �� : arrange_message_disp_field
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::arrange_message_disp_field()
{
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem( IDC_TAB_PIMPOM_DLG_MESSAGE );
	CWnd	*p_wnd_message = (CWnd*)GetDlgItem( IDC_EDIT_PIMPOM_DLG_MESSAGE );

	p_wnd_tab->MoveWindow(MESSAGE_DISP_FIELD_X , API.main_window_size.cy-150, API.main_window_size.cx - 340 , MESSAGE_DISP_FIELD_H);
	p_wnd_message->MoveWindow(MESSAGE_DISP_FIELD_X+5 , API.main_window_size.cy-150+23, API.main_window_size.cx - 340 -10 , MESSAGE_DISP_FIELD_H-25);
}


/********************************************************************
�@  �\  ��  �� : �w����W�܂ގ���9��f�̒l��\������
��    ��    �� : DispDataValue 
��    ��    �l : CPoint src_coor (in)
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::DispDataValue(double point_x, double point_y )
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾����
	long		disp_piece_width = 5;//�w�肵�����W�̎��͂̊g��\������̈�̕�
	int			dlgid[3][3] = { { IDC_EDIT_PIMPON_DLG_VALUE_1_1, IDC_EDIT_PIMPON_DLG_VALUE0_1, IDC_EDIT_PIMPON_DLG_VALUE1_1 },
								{ IDC_EDIT_PIMPON_DLG_VALUE_10, IDC_EDIT_PIMPON_DLG_VALUE00, IDC_EDIT_PIMPON_DLG_VALUE10 },
								{ IDC_EDIT_PIMPON_DLG_VALUE_11, IDC_EDIT_PIMPON_DLG_VALUE01, IDC_EDIT_PIMPON_DLG_VALUE11 }
	};
	CString		coor_xy, coor_ij;
	int			i,j;
	CPoint		src_coor((int)point_x, (int)point_y);

		if(p_du == NULL)
		{//�f�[�^�������Ƃ��͑S�Ă�-��\������
			for (j = 0; j < 3; j++){
				for (i = 0; i < 3; i++){
					API.SetEditValue(m_hWnd, dlgid[j][i], _T("-"));
				}
			}

		}else{
				//�摜�]�����̍��W�ł̕\���\�ȗ̈��\����`�����߂�
				CRect	src_disp_able_rect = CRect(0, 0, p_du->DataSize.cx - 1, p_du->DataSize.cy -1 ) 
					+ CRect( -disp_piece_width/2, -disp_piece_width/2, -disp_piece_width/2, -disp_piece_width/2);


				//9�_�̃f�[�^��\������
				for (j = 0; j < 3; j++){
					for (i = 0; i < 3; i++){
						CPoint point = src_coor + CPoint(i - 1, j - 1);

						if (point.x < 0 || point.x >= p_du->DataSize.cx || point.y < 0 || point.y >= p_du->DataSize.cy){
							API.SetEditValue(m_hWnd, dlgid[j][i], _T("-"));
						}
						else{
							float val = (float)p_du->GetDataValue(point, -1, -1);
							API.SetEditValue(m_hWnd, dlgid[j][i], val);
						}
					}
				}

				//�w�肵�����W�i�摜�]�����j�̎��͂��g��\������
				p_image_disp_piece->DrawPiece(p_du, src_coor - CPoint(disp_piece_width/2, disp_piece_width/2)
					, CSize(disp_piece_width, disp_piece_width) , true , false , false);

				//�w�肵�����W�i�摜�]�����j�̍��W��\������
				if(API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_SUBPIX) )//�T�u�s�N�Z���\��
				{
					coor_ij.Format("(x,y)=(%0.3lf, %0.3lf)", point_x, point_y );//�摜�f�[�^�̍�������_�Ƃ����ꍇ�̍��W�ii,j�j
				}else{
					coor_ij.Format("(x,y)=(%d, %d)", (int)point_x, (int)point_y );//�摜�f�[�^�̍�������_�Ƃ����ꍇ�̍��W�ii,j�j
				}
				API.SetEditValue( m_hWnd, IDC_EDIT_PIMPON_DLG_COOR_IJ , coor_ij );
				API.SetEditValue( m_hWnd, IDC_EDIT_PIMPON_DLG_COOR_XY , coor_xy );
		}

}



/********************************************************************
�@  �\  ��  �� : ��Ɨ̈��`��
��    ��    �� : draw_work_area 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::draw_work_area()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾����
	if(p_du == NULL){
		if(API.pWorkAreaDlg){
			API.pWorkAreaDlg->ShowWindow(SW_HIDE);//��Ɨ̈�E�B���h�����\��
		}
		return;//�f�[�^��������Ή������Ȃ�
	}


	if( p_du->DoUseWorkArea )//��Ɨ̈���g���ꍇ
	{
		API.pWorkAreaDlg->ShowWindow(SW_SHOW);//��Ɨ̈�E�B���h�����\��
		API.pWorkAreaDlg->OnDrawOnImage();
	}
	else//��Ɨ̈���g��Ȃ��ꍇ
	{
		//��Ɨ̈�E�B���h�E���\��
		if(API.pWorkAreaDlg){
			API.pWorkAreaDlg->ShowWindow(SW_HIDE);
		}
	}

	API.SetCheck( m_hWnd , IDC_BUTTON_PIMPON_DLG_WORK_AREA , p_du->DoUseWorkArea );//��Ɨ̈�̎g�p/�s�g�p�����߂�
}



/********************************************************************
�@  �\  ��  �� : �}�[�N�\��
��    ��    �� : draw_marks 
��    ��    �l : 
�@          �\ : �}�[�N��ɏ\���J�[�\����`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::draw_marks()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾����
	CString		str;
	long		mark_length = 20;//�}�[�N�̃��C���̒���


	if(p_du == NULL){//�f�[�^��������Ή������Ȃ�
		if(API.pMarkDlg){//�}�[�N�E�B���h�E��\��
			API.pMarkDlg->ShowWindow(SW_HIDE);
		}
		return;
	}

	if (API.GetMarkEditMode()>0)
	{//�}�[�N�g�p��
		API.pMarkDlg->ShowWindow(SW_SHOW);
		API.pMarkDlg->OnDrawOnImage();
	}
	else 
	{//�}�[�N�s�g�p��
		API.pMarkDlg->ShowWindow(SW_HIDE);
	}
	
}

/********************************************************************
�@  �\  ��  �� : 2�_�̋������v�Z���A�\������
��    ��    �� : draw_2points_distance 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::draw_2points_distance()
{
	float	value1, value2, value_dif, distance, di, dj;
	CDC			*p_dc;
	CRect		dest_rect;
	CString		s_coor_i, s_coor_j, s_value, s_distance, s_index;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̉摜��DataUnit���擾����
	double		dest_point1x, dest_point1y, dest_point2x, dest_point2y;

		if(p_du == NULL)	return	;//�I�𒆂̉摜�������Ƀf�[�^��������΂��������͉������Ȃ�
		if( !API.GetCheck( m_hWnd , IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE) )		return;//�������v�Z����`�F�b�N�{�^����������Ă��邱�Ƃ��m�F

	p_dc = GetDC();//�f�o�C�X�R���e�L�X�g���擾����
	p_dc->SetROP2( R2_NOT );
	p_image_disp_main->GetDestRect(p_du, &dest_rect);//�摜�]����̋�`�̈�

	p_image_disp_main->GetDestCoor( p_du , calc_begin_x, calc_begin_y , &dest_point1x , &dest_point1y );//�摜�]����ł̍��W���擾

	p_image_disp_main->GetDestCoor( p_du , calc_end_x, calc_end_y , &dest_point2x , &dest_point2y );//�摜�]����ł̍��W���擾

	API.DrawClippedSegment(p_dc, dest_rect, dest_point1x, dest_point1y, dest_point2x , dest_point2y);//����`��

	ReleaseDC(p_dc);//�f�o�C�X�R���e�L�X�g���J������

	//���b�Z�[�W�{�b�N�X�ɕ\������
	s_index = _T("     �n�_      �I�_       ��");
	ShowMessage(false, 0, false, s_index);

	di = (float)(calc_end_x - calc_begin_x);
	s_coor_i = _T("x:  ") + API.ToString((float)calc_begin_x,10) + API.ToString((float)calc_end_x,10) + API.ToString(di,10);
	ShowMessage(false, 0, false, s_coor_i);

	dj = (float)(calc_end_y - calc_begin_y);
	s_coor_j = _T("y:  ") + API.ToString((float)calc_begin_y,10) + API.ToString((float)calc_end_y,10) + API.ToString(dj,10);
	ShowMessage(false, 0, false, s_coor_j);

	value1 = (float)p_du->GetDataValue( CPoint(calc_begin_x, calc_begin_y),-1,-1);
	value2 = (float)p_du->GetDataValue( CPoint(calc_end_x, calc_end_y) ,-1,-1);
	value_dif = value2 - value1;
	s_value = _T("�l: ") + API.ToString(value1, 8) + _T("  ") + API.ToString(value2, 8) + _T("  ") + API.ToString(value_dif, 6);
	ShowMessage(false, 0, false, s_value);

	distance = (float)sqrt( (double)(di*di)  +   (double)(dj*dj) );
	s_distance = _T("����: ") + API.ToString(distance,10);
	ShowMessage(false, 0, false, s_distance );



	if( API.GetCheck(m_hWnd, IDC_BUTTON_PIMPON_DLG_CALC_DISTANCE2) ){//�O���t�`���A��������
		if(distance>1){
			int dataCnt=0;
			float dataVal[CHART_DATA_NUM]={0};
			float dx,dy;
			dx = (float)(calc_end_x - calc_begin_x) / distance;
			dy = (float)(calc_end_y - calc_begin_y) / distance;
			for(float pos=0 ; pos<=distance && dataCnt<CHART_DATA_NUM; pos++, dataCnt++)
			{
				float x = calc_begin_x + pos * dx;
				float y = calc_begin_y + pos * dy;

				dataVal[dataCnt] = (float)p_du->GetDataValue((double)x, (double)y ,-1,-1);
			}

			API.PlotDataToChart(CHART_NUM-1, dataCnt, dataVal, true);
		}

	}
}

/********************************************************************
�@  �\  ��  �� : ���b�Z�[�W�\���̈�Ƀ��b�Z�[�W��\������
��    ��    �� : ShowMessage 
��    ��    �l : CString	str			(in)���b�Z�[�W
				 bool		do_clear	(in)true �ɂ��Ă����ƑO�̃��b�Z�[�W������
				 int		num			(in)���b�Z�[�W�E�B���h�E�ԍ�
				 bool		force		(in)�����I�Ƀ^�u��؂�ւ��ă��b�Z�[�W�\������
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::ShowMessage(bool do_clear, int num, bool force, CString str)
{
	CString	ret;
	ret.Format("%c%c", (char)CR , (char)LF);//���s�R�[�h

		if(do_clear){
			message[num] = _T("");
		}

	message[num] += str + ret;


	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem( IDC_TAB_PIMPOM_DLG_MESSAGE );//�I���^�u�ύX

	bool dispflg = false;
	if(force)
	{//�����\��
		p_wnd_tab->SetCurSel(num);
		dispflg = true;
	}
	else
	{//�Y�����b�Z�[�W�ԍ��^�u���I������Ă���Ƃ��̂ݕ\��
		if(num==p_wnd_tab->GetCurSel()){
			dispflg = true;
		}
	}


	if (dispflg)
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_PIMPOM_DLG_MESSAGE, message[num]);

		//�Ō�̕����I��(�X�N���[�������邽��)
		CEdit	*p_wnd_edit = (CEdit*)GetDlgItem(IDC_EDIT_PIMPOM_DLG_MESSAGE);
		int lastidx = message[num].GetLength() - 1;
		p_wnd_edit->SetSel(lastidx, lastidx);
	}
}

/********************************************************************
�@  �\  ��  �� : ���b�Z�[�W�^�u�ύX
��    ��    �� : OnTcnSelchangeTabPimpomDlgMessage 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnTcnSelchangeTabPimpomDlgMessage(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl	*p_wnd_tab = (CTabCtrl*)GetDlgItem( IDC_TAB_PIMPOM_DLG_MESSAGE );

	switch(p_wnd_tab->GetCurSel())
	{
	case 0:
		API.SetEditValue( m_hWnd , IDC_EDIT_PIMPOM_DLG_MESSAGE , message[0] );
		break;
	case 1:
		API.SetEditValue( m_hWnd , IDC_EDIT_PIMPOM_DLG_MESSAGE , message[1] );
		break;
	default:
		break;
	}
	
	*pResult = 0;
}

/********************************************************************
�@  �\  ��  �� : �o�b�N�O���E���h����
��    ��    �� : BackGroundProc 
��    ��    �l : 
�@          �\ : �^�C�}�[�����͓���A���\�y�уX���[�\���ɗp����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::BackGroundProc()
{
	for (;;)
	{
		switch(API.GetBGProcID())
		{
			case BGPROC_CAPTURING://�J�����X���[��
				if(API.GetCheck(m_hWnd,IDC_CHECK_PIMPON_DLG_VIDEO_CAPTURE2))//�X���[�\���{�o�b�`���s
				{
					API.CaptureOne(false);	//�L���v�`��

					BATCH_INPUT in_param = { 0 };
					BATCH_OUTPUT out_param = { 0 };

					API.BatchExecute(&in_param, &out_param);


				}else{//�X���[�\���̂�
					API.CaptureOne(true);
				}
				break;

			case BGPROC_MOVIE://����\����
				movie();
				break;

			case BGPROC_BATCH_EXC://�o�b�`������
				break;
		}

		Sleep(10);
	}
}

/********************************************************************
�@  �\  ��  �� : ����A���\��
��    ��    �� : movie 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::movie()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL){//�f�[�^�������Ƃ�
			if(API.GetBGProcID()==BGPROC_MOVIE){
				API.StopBGProc();//�Đ����~
			}
			return;
		}

		if( p_du->DataType != THREE_D_FORMAT	&&	p_du->DataType != FLOAT_3D_FORMAT &&  p_du->DataType != RGB_3D_FORMAT){//�I�𒆂̃f�[�^������ł͂Ȃ��Ƃ�
			if(API.GetBGProcID()==BGPROC_MOVIE){
				API.StopBGProc();//�Đ����~
			}
			return;
		}

		if( !p_du->MoveDispPage(1) ){//�ŏI�y�[�W�܂ł����Ƃ�
			p_du->SetDispPage(0);//�擪�y�[�W�ɂ��ǂ�
		}

	API.DrawImage( CURRENT_IMAGE, true );//�ĕ`��
}

/********************************************************************
�@  �\  ��  �� : ����A���\�����~
��    ��    �� : StopMovie 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::StopMovie()
{
	if(API.GetBGProcID()==BGPROC_MOVIE){
		API.StopBGProc();//�Đ����~
	}
}

/********************************************************************
�@  �\  ��  �� : �h���b�O�A���h�h���b�v
��    ��    �� : OnDropFiles 
��    ��    �l : 
�@          �\ : ���C����ʏ�ɉ摜�t�@�C���h���b�v���ꂽ��A�I�𒆂̉摜�������Ƀt�@�C����ǂݍ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnDropFiles(HDROP hDropInfo)
{
	CString		strFileName;
	UINT		size = DragQueryFile( hDropInfo, 0, NULL, 0) + 1;

	DragQueryFile( hDropInfo , 0 , strFileName.GetBuffer(size),size );
	strFileName.ReleaseBuffer();
		if( API.pDataDlg->DataExistMessage("�㏑�����Ă���낵���ł����H", CURRENT_IMAGE) )		return;

	API.BackupDataUnit(CURRENT_IMAGE);//���݂̃f�[�^�o�b�N�A�b�v

	//�t�@�C�������[�h����
		if( !API.SelectAndLoadImageFile( CURRENT_IMAGE, strFileName) ){
			return;
		}

	API.DrawImage(CURRENT_IMAGE,true,true,true);
	DragFinish(hDropInfo);
}

/********************************************************************
�@  �\  ��  �� : �o�[�W�������
��    ��    �� : OnHelpVersion 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::OnHelpVersion()
{
	CAboutDlg box;

	box.DoModal();
}


/********************************************************************
�@  �\  ��  �� : �O���v���O��������PIMPOM�ɉ摜�f�[�^��]�����郁�b�Z�[�W��M
��    ��    �� : OnMessageExtInput 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
LRESULT CPIMPOMDlg::OnMessageExtInput(WPARAM wParam, LPARAM lParam)
{
	SHEAREMEM_PLOT_IMAGE	*pShmem;//���L�������̈�
	SHEAREMEM_PLOT_IMAGE	shmem_header={0};
	HANDLE	hShare;//���L�������̃n���h��


	//���L�������̈�̃w�b�_������ǂ�
	pShmem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_PLOT_IMAGE), &hShare, 0);
	if(!pShmem)	return 0;

	memcpy(&shmem_header, pShmem, sizeof(SHEAREMEM_PLOT_IMAGE));
	pimpom_plot_free_shere_mem(pShmem, hShare);//���L���������

	if(shmem_header.image_num<0 || shmem_header.image_num>=API.GetDataUnitNumber() 
		|| shmem_header.width<=0 || shmem_header.height<=0 || shmem_header.size<=0)	return 0;



	//���L�������̈�̑S�̓ǂݒ���
	pShmem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(shmem_header.size, &hShare, 0);
	if(!pShmem)	return 0;


	//�f�[�^�R�s�[���{
	bool doDrawImage = true;//�ĕ`�悷�邩�ǂ���
	switch(pShmem->format)
	{
		case PIMPOM_PLOT_FORMAT_BYTE_IMAGE://BYTE�^������
			{
				BYTE *pDstImg = API.GetByteMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height);	
			}
			break;

		case PIMPOM_PLOT_FORMAT_SHORT_IMAGE://SHORT�^������
			{
				short *pDstImg = API.GetShortMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(short));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_WORD_IMAGE://WORD�^������
			{
				WORD *pDstImg = API.GetWordMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(WORD));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_LONG_IMAGE://LONG�^������
			{
				long *pDstImg = API.GetLongMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(long));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_DWORD_IMAGE://DWORD�^������
			{
				DWORD *pDstImg = API.GetDwordMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(DWORD));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_IMAGE://float�^������
			{
				float *pDstImg = API.GetFloatMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height), true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*sizeof(float));	
			}
			break;

		case PIMPOM_PLOT_FORMAT_3D_IMAGE://3D�^������
			{
				BYTE *pDstImg = API.Get3DMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height),pShmem->page, true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*pShmem->page);	
			}
			break;

		case PIMPOM_PLOT_FORMAT_F3D_IMAGE://F3D�^������
			{
				float *pDstImg = API.GetF3DMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), pShmem->page, true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*pShmem->page*sizeof(float));
			}
			break;

		case PIMPOM_PLOT_FORMAT_RGB_IMAGE://RGB�^������
			{
				BYTE *pDstImg = API.GetRGBMemory(pShmem->image_num, CSize(pShmem->width,pShmem->height),true);
				memcpy(pDstImg, pShmem->pData, pShmem->width*pShmem->height*pShmem->channel);	
			}
			break;

		case PIMPOM_PLOT_FORMAT_MASK_IMAGE://�}�X�N�摜
			{
				BYTE *pMskImg = API.GetMaskMemory(pShmem->image_num);
				if (pMskImg)
				{
					memcpy(pMskImg, pShmem->pData, pShmem->width*pShmem->height*pShmem->channel);
				}
			}
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_1D_CHART://1�����f�[�^(float)
			{
				API.PlotDataToChart(pShmem->image_num, pShmem->width, (float*)pShmem->pData);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_DOUBLE_1D_CHART://1�����f�[�^(double)
			{
				API.PlotDataToChart(pShmem->image_num, pShmem->width, (double*)pShmem->pData);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_INT_1D_CHART://1�����f�[�^(int)
			{
				API.PlotDataToChart(pShmem->image_num, pShmem->width, (int*)pShmem->pData);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_BYTE_1D_CHART://1�����f�[�^(BYTE)
			{
				API.PlotDataToChart(pShmem->image_num, pShmem->width, (BYTE*)pShmem->pData);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_2D_CHART://2�����f�[�^(float)
			{
				API.PlotDataToChart2(pShmem->image_num, pShmem->width, (float*)pShmem->pData, (float*)pShmem->pData + pShmem->width);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_DOUBLE_2D_CHART://2�����f�[�^(double)
			{
				API.PlotDataToChart2(pShmem->image_num, pShmem->width, (double*)pShmem->pData, (double*)pShmem->pData + pShmem->width);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_INT_2D_CHART://2�����f�[�^(int)
			{
				API.PlotDataToChart2(pShmem->image_num, pShmem->width, (int*)pShmem->pData, (int*)pShmem->pData + pShmem->width);
				doDrawImage = false;
			}
			break;

		case PIMPOM_PLOT_FORMAT_BYTE_2D_CHART://2�����f�[�^(BYTE)
			{
				API.PlotDataToChart2(pShmem->image_num, pShmem->width, (BYTE*)pShmem->pData, (BYTE*)pShmem->pData + pShmem->width);
				doDrawImage = false;
			}
			break;
	}

	if (doDrawImage) {
		API.DrawImage(pShmem->image_num, true, true);//�`��
	}

	pimpom_plot_free_shere_mem(pShmem, hShare);//���L���������

	return 0;
}


/********************************************************************
�@  �\  ��  �� : PIMPOM����O���v���O�����ɉ摜�f�[�^��]�����郁�b�Z�[�W��M
��    ��    �� : OnMessageExtInput
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
LRESULT CPIMPOMDlg::OnMessageExtOutput(WPARAM wParam, LPARAM lParam)
{
	SHEAREMEM_PLOT_IMAGE	*pShmem;//���L�������̈�
	SHEAREMEM_PLOT_IMAGE	shmem_header = { 0 };
	HANDLE	hShare;//���L�������̃n���h��


				   //���L�������̈�̃w�b�_������ǂ�
	pShmem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_PLOT_IMAGE), &hShare, 0);
	if (!pShmem)	return 0;

	memcpy(&shmem_header, pShmem, sizeof(SHEAREMEM_PLOT_IMAGE));
	pimpom_plot_free_shere_mem(pShmem, hShare);//���L���������




	if (shmem_header.image_num<0 || shmem_header.image_num >= API.GetDataUnitNumber() ||
		shmem_header.width <= 0 || shmem_header.height <= 0 || shmem_header.size <= 0) {
		return 0;
	}

	//���L�������̈�̑S�̓ǂݒ���
	pShmem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(shmem_header.size, &hShare, 1);
	if (!pShmem)	return 0;


	{
		//�f�[�^�R�s�[���{
		switch (pShmem->format)
		{
			case PIMPOM_PLOT_FORMAT_BYTE_IMAGE://BYTE�^������
			{
				BYTE *pDstImg = API.GetByteMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height);
			}
			break;

			case PIMPOM_PLOT_FORMAT_SHORT_IMAGE://SHORT�^������
			{
				short *pDstImg = API.GetShortMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(short));
			}
			break;

			case PIMPOM_PLOT_FORMAT_WORD_IMAGE://WORD�^������
			{
				WORD *pDstImg = API.GetWordMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(WORD));
			}
			break;

			case PIMPOM_PLOT_FORMAT_LONG_IMAGE://LONG�^������
			{
				long *pDstImg = API.GetLongMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(long));
			}
			break;

			case PIMPOM_PLOT_FORMAT_DWORD_IMAGE://DWORD�^������
			{
				DWORD *pDstImg = API.GetDwordMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(DWORD));
			}
			break;

			case PIMPOM_PLOT_FORMAT_FLOAT_IMAGE://float�^������
			{
				float *pDstImg = API.GetFloatMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height * sizeof(float));
			}
			break;

			case PIMPOM_PLOT_FORMAT_3D_IMAGE://3D�^������
			{
				BYTE *pDstImg = API.Get3DMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), pShmem->page, false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height*pShmem->page);
			}
			break;

			case PIMPOM_PLOT_FORMAT_F3D_IMAGE://F3D�^������
			{
				float *pDstImg = API.GetF3DMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), pShmem->page, false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height*pShmem->page * sizeof(float));
			}
			break;

			case PIMPOM_PLOT_FORMAT_RGB_IMAGE://RGB�^������
			{
				BYTE *pDstImg = API.GetRGBMemory(pShmem->image_num, CSize(pShmem->width, pShmem->height), false);
				memcpy(pShmem->pData, pDstImg, pShmem->width*pShmem->height*pShmem->channel);
			}
			break;

			case PIMPOM_PLOT_FORMAT_MASK_IMAGE://�}�X�N�摜������
			{
				BYTE *pMskImg = API.GetMaskMemory(pShmem->image_num);
				memcpy(pShmem->pData, pMskImg, pShmem->width*pShmem->height*pShmem->channel);
			}
			break;
		}
	}


	pimpom_plot_free_shere_mem(pShmem, hShare);//���L���������


	return 0;
}



/********************************************************************
�@  �\  ��  �� : �O���v���O��������摜�]���𔺂�Ȃ����b�Z�[�W��M
��    ��    �� : OnMessageExtProcess 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
LRESULT CPIMPOMDlg::OnMessageExtProcess(WPARAM wParam, LPARAM lParam)
{
	SHEAREMEM_EXT_PROC	*pShmem;//���L�������̈�
	HANDLE	hShare;//���L�������̃n���h��



	//���L�������̈�̃w�b�_������ǂ�
	pShmem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
	if(!pShmem)	return 0;



	switch(pShmem->type)
	{
		case PIMPOM_PLOT_COMMAND_PREPAIR_DRAWING://�摜��ɕ`�悷�鏀���̂��߂ɁA�w�i�ƂȂ�摜���R�s�[����RGB�t�H�[�}�b�g�ɕϊ�����
			if(pShmem->copy_src>=0 && pShmem->copy_src<API.GetDataUnitNumber()){
				API.CopyDataUnit(pShmem->copy_src, pShmem->image_num);
				CONVERT_DATA_OPTION	option;
				option.type_to = RGB_FORMAT;
				option.rgb_to = RGB_TO_ALL;
				API.ConvertDataUnit(pShmem->image_num,option);
				API.DrawImage(pShmem->image_num);
			}
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_IMAGE://�摜�`��
			{
				int doAdjust;
				char buf[1024]={0};
				sscanf(pShmem->message,"%d,%s\n", &doAdjust, buf);
				if(buf[0]==0){
					API.DrawImage(pShmem->image_num, true, doAdjust);
				}else{
					API.DrawImage(pShmem->image_num, CString(buf), true, doAdjust);
				}
			}
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_POINT_ON_IMAGE://�_�`��
			API.DrawPointOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->size, RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_LINE_ON_IMAGE://�����`��
			API.DrawLineOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->x1, pShmem->y1, pShmem->size, RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_RECT_ON_IMAGE://��`�`��
			API.DrawRectOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->x1, pShmem->y1, RGB(pShmem->red, pShmem->green, pShmem->blue), false, 0, pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_CIRCLE_ON_IMAGE://�~�`�`��
			API.DrawCircleOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->x1, RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->y1, pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_CURSOR_ON_IMAGE://�\���J�[�\���`��
			API.DrawCursorOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->size, RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_DRAW_TEXT_ON_IMAGE://�e�L�X�g�`��
			API.DrawTextOnImage(pShmem->image_num, pShmem->x0, pShmem->y0, pShmem->message,  RGB(pShmem->red, pShmem->green, pShmem->blue), pShmem->copy_src);
			break;

		case PIMPOM_PLOT_COMMAND_SHOW_MESSAGE://���b�Z�[�W�\��
			API.ShowMessage(false,1,true, CString(pShmem->message));
			break;

		case PIMPOM_PLOT_COMMAND_SHOW_NEW_MESSAGE://���b�Z�[�W�\��(�V�K)
			API.ShowMessage(true,1,true, CString(pShmem->message));
			break;

		case PIMPOM_PLOT_COMMAND_ADD_MARK://�}�[�J�[�ǉ�
			{
				float x,y;
				double atrb[3]={0};
				sscanf(pShmem->message,"%f,%f,%lf,%lf,%lf\n", &x, &y, &atrb[0], &atrb[1], &atrb[2]);
				API.AddMark(pShmem->image_num, x, y, atrb); 
			}
			break;

		case PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT://�O���t(�܂��)�ɒl�ǉ�
			{
				float val;
				sscanf(pShmem->message,"%f\n",&val);
				API.AddDataToChart(pShmem->image_num, val);
			}
			break;

		case PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT2://�O���t(2�����U�z�})�ɒl�ǉ�
			{
				float valx, valy;
				sscanf(pShmem->message,"%f,%f\n", &valx, &valy);
				API.AddDataToChart2(pShmem->image_num, valx, valy);
			}
			break;

		case PIMPOM_PLOT_COMMAND_CLEAR_CAHRT://�O���t�N���A
			API.ClearChart(pShmem->image_num);
			break;


		case PIMPOM_PLOT_COMMAND_BATCH_EXECUTE://�o�b�`���s
			{
				pimpom_plot_free_shere_mem(pShmem, hShare);//���L����������������

				pShmem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);//�g���̈��ǂݒ���
				if (!pShmem)	return 0;


				BATCH_INPUT in_param={0};
				BATCH_OUTPUT out_param={0};

				in_param.image_number = pShmem->image_num;//�摜

				//�f�[�^����
				char *ptr = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC);

				sscanf(ptr, "%s", in_param.imae_file_path);
				ptr += 2048;

				for(int i=0 ; i<BATCH_IN_PARAM_NUM ; i++){
					sscanf(ptr, "%s", in_param.param[i]);
					ptr+=1024;
				}

				//�o�b�`���s
				API.BatchExecute(&in_param, &out_param);

				//�f�[�^�o��
				ptr = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE/2;
				for(int i=0 ; i<BATCH_OUT_PARAM_NUM ; i++){
					sprintf(ptr,"%s", out_param.result[i]);
					ptr += 1024;
				}

				*ptr = (char)out_param.state;

			}
			break;

		case 31://�o�b�`���s�V�F�A
			{
					pimpom_plot_free_shere_mem(pShmem, hShare);//���L����������������

					pShmem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC)+SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);//�g���̈��ǂݒ���
					if (!pShmem)	return 0;


					BATCH_INPUT in_param = { 0 };
					BATCH_OUTPUT out_param = { 0 };

					in_param.image_number = pShmem->image_num;//�摜

					//�f�[�^����
					char *ptr = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC);

					sscanf(ptr, "%s", in_param.imae_file_path);
					ptr += 2048;

					for (int i = 0; i < BATCH_IN_PARAM_NUM; i++){
						sscanf(ptr, "%s", in_param.param[i]);
						ptr += 1024;
					}


			}
			break;




		case PIMPOM_PLOT_COMMAND_EXECUTE://�R�}���h������
			{
				pimpom_plot_free_shere_mem(pShmem, hShare);//���L����������������

				pShmem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);//�g���̈��ǂݒ���
				if (!pShmem)	return 0;


				//�f�[�^����
				char *command = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC);


				CString			err_str, com_str;
				double			result_num;
				char			result_str[CALC_STRING_LEN_MAX];
				CCalculate		calc;

				com_str = CString(command);
				bool ret = calc.RegisterCommandString(&com_str, 1, &err_str);
				if (ret)
				{
					BATCH_INPUT in_param = { 0 };
					BATCH_OUTPUT out_param = { 0 };

					if (API.pBatchDlg)
					{
						API.pBatchDlg->GetBatchInput(&in_param);
					}


					ret = calc.ExecuteAPI(0,&err_str, NULL, &in_param, &out_param);
					if (ret)
					{
						if (calc.GetResult(0, &result_num, result_str, &err_str) == 0){
							sprintf(result_str, "%lf", result_num);
						}

						if (API.pBatchDlg)
						{
							API.pBatchDlg->SetBatchInOut(&in_param, &out_param);
						}
					}
				}


				//���ʏo��
				char *result_data = (char*)pShmem + sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE / 2;

				result_data[0] = ret;	//�擪Byte : ���s���� (����: 1   ���s:0)
				strcpy(&result_data[5000], result_str);	//�擪����5000�`10000Byte : ���s����
				strcpy(&result_data[10000], err_str.GetBuffer());//�擪����10000�`15000Byte : �G���[���b�Z�[�W
				err_str.ReleaseBuffer();
			}
			break;
	}

	pimpom_plot_free_shere_mem(pShmem, hShare);//���L���������

	return 0;
}

/********************************************************************
�@  �\  ��  �� : �N���b�N���W��ݒ肷��
��    ��    �� : SetClickedCoordinate 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPIMPOMDlg::SetClickedCoordinate(CPoint point)
{
	latest_clicked_coordinate_x = point.x;
	latest_clicked_coordinate_y = point.y;
}









