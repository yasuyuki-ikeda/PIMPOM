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
//CDataDlg
//�摜�������ꗗ�\���E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "DataDlg.h"

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

//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}

/********************************************************************
�@  �\  ��  �� : �摜�������Ƀf�[�^�����݂��Ȃ����ǂ������m���߂�
��    ��    �� : DataExistMessage
��          �� : int		image_num	(in)�摜�������ԍ� 
				 CString	mes			(in)image_num�Ƀf�[�^�����݂���ꍇ�ɕ\�����郁�b�Z�[�W
��    ��    �l : ���݂��Ȃ��A�������͏㏑������ꍇ�́Afalse��Ԃ�
�@          �\ : �摜�������Ƀf�[�^���������ޑO�ɓ��e�m�F����Ƃ��Ɏg��
				 image_num�Ƀf�[�^�����݂���ꍇ�́A�㏑���₢���킹�_�C�A���O���o���A������OK��������false��Ԃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CDataDlg::DataExistMessage(CString mes, int image_num)
{
	CDataUnit *pDU;
	CString  str ;

	pDU = API.GetDataUnit(image_num);//���ݑI�𒆂̉摜�f�[�^���擾
	if( !pDU )		return	false;//�w�肵���������Ƀf�[�^�����݂��Ȃ�

	switch( pDU->DataType ){
	case BYTE_FORMAT:
		str.LoadString(IDS_DATATYPE_BYTE);
		break;
	case FLOAT_FORMAT:
		str.LoadString(IDS_DATATYPE_FLOAT);
		break;
	case COMPLEX_FORMAT:
		str.LoadString(IDS_DATATYPE_COMPLEX);
		break;
	case THREE_D_FORMAT:
		str.LoadString(IDS_DATATYPE_3D);
		break;
	case XY_FORMAT:
		str.LoadString(IDS_DATATYPE_XY);
		break;
	case XYZ_FORMAT:
		str.LoadString(IDS_DATATYPE_XYZ);
		break;
	case FLOAT_3D_FORMAT:
		str.LoadString(IDS_DATATYPE_F3D);
		break;
	case SHORT16_FORMAT:
		str.LoadString(IDS_DATATYPE_SHORT);
		break;
	case WORD_FORMAT:
		str.LoadString(IDS_DATATYPE_WORD);
		break;
	case LONG32_FORMAT:
		str.LoadString(IDS_DATATYPE_LONG);
		break;
	case DWORD_FORMAT:
		str.LoadString(IDS_DATATYPE_DWORD);
		break;
	case RGB_FORMAT:
		str.LoadString(IDS_DATATYPE_RGB);
		break;
	case RGB_3D_FORMAT:
		str.LoadString(IDS_DATATYPE_RGB3D);
		break;
	}

	str = "���̉摜�������ɂ�" + str + pDU->DataName + "�������Ă��܂��B\n" + mes;
	if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) == IDOK ){//�㏑����I��
		//backup_stock_image();
		return	false;
	}else{
		return	true;
	}
}

/********************************************************************
�@  �\  ��  �� : ���ݑI�𒆂̉摜�������ԍ���ύX����
��    ��    �� : ChangeCurrentDataNumber
��          �� : long number�@(in)�V���ɑI������摜�������ԍ�
��    ��    �l : �ȑO�ɑI�����Ă����摜�������ԍ���Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
long CDataDlg::ChangeCurrentDataNumber(long number)
{
	long	old_number = API.GetCurrentImageNumber();
	API.SetCurrentImageNumber(number);

		if( old_number != number ){//�I�����Ă���摜�ԍ����ς�����Ƃ��̏���
			if(API.pPIMMOMDlg){
				API.pPIMMOMDlg->StopMovie();//�����~
			}
		}

	draw_current_rect( old_number, API.default_bg_color );//�ȑO�ɑI�����Ă����摜�̎����w�i�F�œh��i��`���㏑������j
	draw_current_rect( API.GetCurrentImageNumber(), RGB(0,0,0) );//�V�����I�������摜�̎���ɋ�`��`��
	
	return	old_number;
}



///////////////////////////////////////////////////////////////////////////
//�T���l�C���`��Ɋւ��郁���o�֐�

/********************************************************************
�@  �\  ��  �� : �w�i�F�ŋ�`�`��
��    ��    �� : draw_background_rect
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::draw_background_rect(CRect rct)
{
	HDC hdc = ::GetDC(m_hWnd);
	HPEN hpen = ::CreatePen(PS_SOLID, 1, API.default_bg_color);
	HBRUSH hbrush = ::CreateSolidBrush(API.default_bg_color);
	::SelectObject(hdc, hpen);
	::SelectObject(hdc, hbrush);
	::Rectangle(hdc, rct.left, rct.top, rct.right, rct.bottom);
	::DeleteObject(hbrush);
	::DeleteObject(hpen);
	::ReleaseDC(m_hWnd, hdc);
}


/********************************************************************
�@  �\  ��  �� : �S�Ẳ摜�f�[�^�̃T���l�C����`�悷��
��    ��    �� : DrawAllImages
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::DrawAllImages()
{
	//�w�i�`��
	RECT clientRct;
	::GetClientRect(m_hWnd, &clientRct);
	draw_background_rect(clientRct);

	for(int n=0 ; n<API.GetDataUnitNumber() ; n++){
		DrawImage(n);
	}
}

/********************************************************************
�@  �\  ��  �� : �w�肵���摜�f�[�^�̃T���l�C����`�悷��
��    ��    �� : DrawImage
��          �� : long number	(in)�摜�������ԍ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::DrawImage(long number)
{
	if (number >= API.GetDataUnitNumber())	return;

	pp_image_disp_small[number]->DrawAdjustedImage( API.GetDataUnit(number) , false);
	draw_image_info( number );
}


/********************************************************************
�@  �\  ��  �� : �������\�������ŃN���b�v����
��    ��    �� : clip_string
��          �� : CDC *pdc       (in)�f�o�C�X�R���e�L�X�g�i�t�H���g�͎��O�ɐݒ肵�Ă������Ɓj
                 CString str	(in)������
				 int max_width  (in)�\������
��    ��    �l : �N���b�v���ꂽ������
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
CString CDataDlg::clip_string(CDC *pdc, CString str, int max_width)
{
	CString clippedStr;

	for (int len = str.GetLength(); len > 0; len--)
	{
		clippedStr = str.Left(len);
		CSize strSize = pdc->GetTextExtent(clippedStr);
		if (strSize.cx < max_width)
		{
			break;
		}
	}

	return clippedStr;
}

/********************************************************************
�@  �\  ��  �� : �w�肵���摜�f�[�^�̏��i�������ԍ��A�f�[�^�`���A�f�[�^���j���T���l�C�����ɕ\������
��    ��    �� : draw_image_info
��          �� : long number	(in)�摜�������ԍ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::draw_image_info(long number)
{	
	CString		type_name, str_image_number;
	CDC			*p_dc;
	CFont		font, *p_old_font;
	CRect		image_area;

	//���C���[�W��ImageField��\����`�̈���擾����
	image_area = pp_image_disp_small[number]->GetFieldRect();
	//�f�o�C�X�R���e�L�X�g�̎擾�ƃt�H���g�̎w�肷��
	p_dc = GetDC();
	font.CreateFontIndirect(&API.default_font);
	p_old_font= (CFont*)p_dc->SelectObject( &font );

	//�摜�ԍ���\��
		if(number<=9)	str_image_number.Format(" %d", number);//1��
		else		str_image_number.Format("%d", number);//2��

	p_dc->TextOut(	image_area.right - 13 , image_area.top , str_image_number );


	//�w�i�F�ŋ�`��`��i�f�[�^���̉��n�j
	CRect name_rct(image_area.left, image_area.bottom + 5, image_area.right, image_area.bottom + 5 + API.default_font.lfHeight);
	draw_background_rect(name_rct);

		if( API.GetDataUnit(number) )
		{//�f�[�^��������΃f�[�^�`��,�f�[�^���͕\���ł��Ȃ�

			//�f�[�^���̕\��
			CString dispName = clip_string(p_dc, API.GetDataUnit(number)->DataName, image_area.Width() );
			p_dc->SetBkColor(API.default_bg_color);//�e�L�X�g�̔w�i�F������
			p_dc->TextOut(image_area.left, image_area.bottom + 5, dispName);//�f�[�^����`��

			

			//�f�[�^�^�C�v���̕\��
				switch( API.GetDataUnit(number)->DataType ){
					case BYTE_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("BYTE");
						break;

					case FLOAT_FORMAT:
						p_dc->SetBkColor( RGB(100,100,255) );
						type_name = _T("FLOAT" );
						break;

					case THREE_D_FORMAT:
						p_dc->SetBkColor( RGB(100,255,100) );
						type_name = _T("3D");
						break;

					case FLOAT_3D_FORMAT:
						p_dc->SetBkColor( RGB(100,255,250) );
						type_name = _T("F3D");
						break;

					case COMPLEX_FORMAT:
						p_dc->SetBkColor( RGB(255,255,100) );
						type_name = _T("COMP" );
						break;

					case XY_FORMAT:
						p_dc->SetBkColor( RGB(255,100,255) );
						type_name = _T("XY");
						break;

					case XYZ_FORMAT:
						p_dc->SetBkColor( RGB(255,255,255) );
						type_name = _T("XYZ");
						break;

					case SHORT16_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("SHORT");
						break;

					case WORD_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("WORD");
						break;

					case LONG32_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("LONG");
						break;

					case DWORD_FORMAT:
						p_dc->SetBkColor( RGB(255,100,100) );
						type_name = _T("DWORD");
						break;

					case RGB_FORMAT:
						p_dc->SetBkColor( RGB(100,100,100) );
						type_name = _T("RGB");
						break;

					case RGB_3D_FORMAT:
						p_dc->SetBkColor( RGB(100,100,100) );
						type_name = _T("RGB3D");
						break;
				}
			p_dc->TextOut(	image_area.left , image_area.top , type_name );//�f�[�^�^�C�v��`��
		}

	//�I�u�W�F�N�g�i�t�H���g�j�̊J���ƃf�o�C�X�R���e�L�X�g�̊J��
	p_dc->SelectObject( p_old_font );
	ReleaseDC( p_dc );
	font.DeleteObject();
}

/********************************************************************
�@  �\  ��  �� : �w�肵���摜�f�[�^�̃T���l�C�����̎���ɋ�`�`��
��    ��    �� : draw_current_rect
��          �� : long		number	(in)�摜�������ԍ�
				 COLORREF	color	(in)�`��F
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::draw_current_rect(long number, COLORREF color)
{
	CDC			*p_dc;
	CPen		pen(PS_SOLID , 2 , color );
	CPen		*p_old_pen;
	CBrush		*p_old_brush;
	CRect		ext_area;
	
	p_dc = GetDC();//�f�o�C�X�R���e�L�X�g���擾����
	if(!p_dc)	return;

	p_old_pen = (CPen*)p_dc->SelectObject( &pen );//�y�����擾����
	p_old_brush = (CBrush*)p_dc->SelectStockObject( NULL_BRUSH );//�u���V���擾����i�k���u���V�j

	ext_area = pp_image_disp_small[number]->GetFieldRect()  +  CRect(3,3,3,3);//��`��`���̈��ݒ肷��
	p_dc->Rectangle( ext_area );//��`��`��

	//�y���ƃu���V���J��
	p_dc->SelectObject( p_old_pen );
	p_dc->SelectObject( p_old_brush );
	pen.DeleteObject();

	ReleaseDC( p_dc );//�f�o�C�X�R���e�L�X�g���J��
}

/********************************************************************
�@  �\  ��  �� : �w�肵���ʒu�ɕ\������Ă���摜�f�[�^�̔ԍ���Ԃ�
��    ��    �� : get_image_number_from_coordinate
��          �� : CPoint		coor	(in)���W
��    ��    �l : �w�肵�����W���ǂ�ImageField�ɂ������Ă��Ȃ��ꍇ��-1��Ԃ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CDataDlg::get_image_number_from_coordinate(CPoint coor)
{
		for(int n=0 ; n<API.GetDataUnitNumber() ; n++){//�\���摜�ԍ��̕ύX
			if( pp_image_disp_small[n]->GetFieldRect().PtInRect(coor) ){
				return	n;
			}
		}
	return	-1;
}



/////////////////////////////////////////////////////////////////////////////
// ���b�Z�[�W �n���h��
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDataDlg, CDialog)
	//{{AFX_MSG_MAP(CDataDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_DATAMENU_RENAME, &CDataDlg::OnDatamenuRename)
	ON_COMMAND(ID_DATAMENU_CONVERT_TYPE, &CDataDlg::OnDatamenuConvertType)
	ON_COMMAND(ID_DATAMENU_RESIZE, &CDataDlg::OnDatamenuResize)
	ON_COMMAND(ID_DATAMENU_DELETE, &CDataDlg::OnDatamenuDelete)
	ON_COMMAND(ID_DATAMENU_SAVEBITMAP, &CDataDlg::OnDatamenuSavebitmap)
	ON_COMMAND(ID_DATAMENU_UNDO, &CDataDlg::OnDatamenuUndo)
	ON_COMMAND(ID_DATAMENU_DISP3D, &CDataDlg::OnDatamenuDisp3d)
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
BOOL CDataDlg::OnInitDialog() 
{
	int n;
	CDialog::OnInitDialog();

	//�h���b�O���h���b�v�Ή�
	DragAcceptFiles();

	//�w�i�F�̃u���V������
	bg_brush = ::CreateSolidBrush(API.default_bg_color);

	//���C���[�W�̔z�������
	pp_image_disp_small = (CImageField**)malloc(API.GetDataUnitNumber() * sizeof(CImageField**));
	if(!pp_image_disp_small)	return FALSE;


	CPoint  origin_coor(5,5);//�ŏ��̉摜�̈�̍��W
	CPoint	disp_small_coor = origin_coor;//�摜�̈�̍��W

	for(n=0 ; n<API.GetDataUnitNumber() ; n++)
	{
		//�摜�̈�̎擾
		pp_image_disp_small[n] = new CImageField( m_hWnd, API.small_image_size, disp_small_coor );
		if(!pp_image_disp_small[n])	return FALSE;

		if(n==API.GetDataUnitNumber()-1)
		{
			//���̃E�B���h�E�̍���������
			RECT wndRct, clientRct;
			GetWindowRect(&wndRct);
			GetClientRect(&clientRct);
			int non_client_height = (wndRct.bottom - wndRct.top) - (clientRct.bottom - clientRct.top);

			WindowHeight = disp_small_coor.y + ( API.small_image_size.cy + SMALL_MARGN_Y) + non_client_height;
		}
		else
		{
			//���̉摜�̈�̍��W�̎Z�o
			disp_small_coor.x += (API.small_image_size.cx + SMALL_MARGN_X);


			//�܂�Ԃ�����
			if (API.thumbneil_colum > 0 &&
				API.thumbneil_colum * (API.small_image_size.cx + SMALL_MARGN_X) < API.data_window_width )
			{//�T���l�C���s�����w�肳��Ă���ꍇ

				if (n%API.thumbneil_colum == API.thumbneil_colum-1) {//����s���Ő܂�Ԃ�
					disp_small_coor.x = origin_coor.x;
					disp_small_coor.y += (API.small_image_size.cy + SMALL_MARGN_Y);
				}
			}
			else
			{//�T���l�C���s�����w�肳��Ă��Ȃ��A�܂��̓E�B���h�E���Ɏ��܂�Ȃ��ꍇ

				if (disp_small_coor.x + API.small_image_size.cx >= API.data_window_width) {//��ʓ��Ɏ��܂�Ȃ��Ȃ邽�тɐ܂�Ԃ�
					disp_small_coor.x = origin_coor.x;
					disp_small_coor.y += (API.small_image_size.cy + SMALL_MARGN_Y);
				}
			}
		}
	}


	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}


/********************************************************************
�@  �\  ��  �� : �h���b�O�A���h�h���b�v
��    ��    �� : OnDropFiles
��          �� : 
��    ��    �l : 
�@          �\ : �T���l�C����ɉ摜�t�@�C���h���b�v���ꂽ��A�h���b�v���ꂽ�摜�������Ƀt�@�C����ǂݍ���
				�@�����̃t�@�C�����h���b�v���ꂽ��A�����̉摜�������ɏ��Ԃɓǂݍ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDropFiles(HDROP hDropInfo)
{
	CString		strFileName;
	int			dropped_image_num;
	int		file_num = (int)DragQueryFileA( hDropInfo, -1, NULL, 0);
	POINT	drop_point;

	if (DragQueryPoint(hDropInfo, &drop_point) == 0){
		return;
	}

	dropped_image_num = get_image_number_from_coordinate(CPoint(drop_point));//�h���b�v���ꂽ�摜�������ԍ�
	if(dropped_image_num==-1){
		return;
	}

	for(int i=0; i+dropped_image_num<API.GetDataUnitNumber() && i<file_num ; i++)
	{
		UINT size = DragQueryFile( hDropInfo, i, NULL, 0) + 1;//�t�@�C�����̒������擾

		DragQueryFile( hDropInfo, i , strFileName.GetBuffer(size),size );
		strFileName.ReleaseBuffer();
			if( DataExistMessage("�㏑�����Ă���낵���ł����H",i + dropped_image_num) )		return;

			API.BackupDataUnit(i + dropped_image_num);//���݂̃f�[�^�o�b�N�A�b�v


		//�t�@�C�������[�h����
		if( !API.SelectAndLoadImageFile( i + dropped_image_num, strFileName) ){
			return;
		}

		API.DrawImage(i + dropped_image_num,true);
	}

	DragFinish(hDropInfo);
}

/********************************************************************
�@  �\  ��  �� :�E�C���h�E�̔j��
��    ��    �� : OnDestroy
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	for(int n=0 ; n<API.GetDataUnitNumber() ; n++){
		delete	pp_image_disp_small[n];
	}
	free( pp_image_disp_small);

	DeleteObject( bg_brush );
}

/********************************************************************
�@  �\  ��  �� :�L�[�{�[�h����̓���
��    ��    �� : PreTranslateMessage
��          �� : 
��    ��    �l : 
�@          �\ : �J�[�\���L�[�F�J�[�\���̈ړ�
				 Del�L�[�F�I�𒆂̉摜�������N���A
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CDataDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN ){
		CPoint	point;
		CString	str;

		switch(pMsg->wParam){
			case VK_RETURN://Enter
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://��
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y - 1);
				break;

			case VK_DOWN://��
				GetCursorPos( &point ) ;
				SetCursorPos(point.x , point.y + 1);
				break;

			case VK_LEFT://��
				GetCursorPos( &point ) ;
				SetCursorPos(point.x - 1 , point.y);
				break;

			case VK_RIGHT://��
				GetCursorPos( &point ) ;
				SetCursorPos(point.x + 1, point.y);
				break;

			case VK_DELETE://Del
				str.Format("�摜������ %d �̃f�[�^���폜���܂��D��낵���ł����H" , API.GetCurrentImageNumber());
				if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) == IDOK ){
					API.DeleteDataUnit(CURRENT_IMAGE);
				}
				break;

			case 'C'://Ctrl + C
				if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) {
					API.CopyToClipbord();
				}
				break;

			case 'V'://Ctrl + V
				if ((GetKeyState(VK_CONTROL) & 0x8000) != 0) 
				{
					if (!DataExistMessage("�㏑�����Ă���낵���ł����H", CURRENT_IMAGE))
					{
						API.BackupDataUnit(CURRENT_IMAGE);//���݂̃f�[�^�o�b�N�A�b�v
						API.PasteFromClipbord(API.GetCurrentImageNumber());//�N���b�v�{�[�h����y�[�X�g
					}
				}
				break;
		}
	}		
	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
�@  �\  ��  �� :�_�C�A���O�̔w�i�F��ݒ�
��    ��    �� : OnCtlColor
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
HBRUSH CDataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);//�_�C�A���O�̔w�i�F��ݒ�
	
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������

	if( nCtlColor == CTLCOLOR_DLG )
		return   bg_brush ;

	return hbr;
}

/********************************************************************
�@  �\  ��  �� :�ĕ`��
��    ��    �� : OnCtlColor
��          �� : 
��    ��    �l : 
�@          �\ : ���ׂẴT���l�C�����ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	DrawAllImages();//�S�Ẳ摜���ĕ`�悷��
	// �`��p���b�Z�[�W�Ƃ��� CDialog::OnPaint() ���Ăяo���Ă͂����܂���
}

/********************************************************************
�@  �\  ��  �� :�}�E�X���{�^���������ꂽ
��    ��    �� : OnLButtonDown
��          �� : 
��    ��    �l : 
�@          �\ : �@�摜�������I��
				 �A�摜�f�[�^�̃������Ԉړ��^�R�s�[�̊J�n
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int n;

	GetCapture();//�}�E�X�L���v�`���[
	is_draging = true;//�h���b�O����\���t���O�𗧂Ă�

		if( (n = get_image_number_from_coordinate(point)) != -1 )//�摜�̏�Ń}�E�X�̍��{�^���������ꂽ
			API.DrawImage(n, false);
	
	CDialog::OnLButtonDown(nFlags, point);
}



/********************************************************************
�@  �\  ��  �� :�}�E�X�J�[�\���̈ړ�
��    ��    �� : OnMouseMove
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
		if(is_draging){//�h���b�O��
			HCURSOR csr = AfxGetApp()->LoadCursor( IDC_POINTER_COPY );//�J�[�\�����h���b�O���̕��ɑւ���
			SetCursor( csr );
		}	
	mouse_point = point;
	CDialog::OnMouseMove(nFlags, point);
}

/********************************************************************
�@  �\  ��  �� :�}�E�X���{�^���������ꂽ
��    ��    �� : OnLButtonUp
��          �� : 
��    ��    �l : 
�@          �\ : �摜�f�[�^�̃������Ԉړ��^�R�s�[�̎��s
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int n;

	ReleaseCapture();//�}�E�X�̃L���v�`���[���J��
	is_draging = false;//�h���b�O����\���t���O��|��

		if( (n = get_image_number_from_coordinate(point)) != -1 ){//�摜�̏�Ń}�E�X�̍��{�^���������ꂽ
			if( n != API.GetCurrentImageNumber() ){//���܂őI�����Ă������̂Ƃ͈قȂ�摜�̏�Ń}�E�X�̍��{�^���������ꂽ
				if( API.CopyDataUnit( API.GetCurrentImageNumber() , n, true) ){//�R�s�[
					/*ChangeCurrentDataNumber(n);*/
					API.DrawImage(n);
				}
			}
		}

	CDialog::OnLButtonUp(nFlags, point);
}




/********************************************************************
�@  �\  ��  �� :�}�E�X�E�{�^���������ꂽ
��    ��    �� : OnRButtonDown
��          �� : 
��    ��    �l : 
�@          �\ : ���j���[�\��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int n;
	
	if( (n = get_image_number_from_coordinate(point)) != -1 ){//�摜�̏�Ń}�E�X�̍��{�^���������ꂽ
		if(API.GetDataUnit(n)){
			API.DrawImage(n, false);

			CMenu    *popupMenuP = NULL;
			CMenu    cMenu;
			int      err = 0;
		    
			// ���j���[�����[�h
			if (!err) if (!cMenu.LoadMenu(IDR_DATA_MENU)) err = 1;

			// �T�u���j���[���擾
			if (!err) if ((popupMenuP = cMenu.GetSubMenu(0)) == NULL) err = 1;

			// ���j���[���|�b�v�A�b�v
			if (!err){
				ClientToScreen(&point);
				if (!popupMenuP->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this)) err = 1;
			}
			
			cMenu.DestroyMenu();// ���j���[��j��
		}

	}
		
	CDialog::OnRButtonDown(nFlags, point);
}


/********************************************************************
�@  �\  ��  �� :�}�E�X�E�{�^�����j���[�F�摜�������̖��O�ύX
��    ��    �� :OnDatamenuRename
��          �� : 
��    ��    �l : 
�@          �\ : �摜�������̖��O�ύX
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDatamenuRename()
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
�@  �\  ��  �� :�}�E�X�E�{�^�����j���[�F�摜�������̌^�ϊ�
��    ��    �� :OnDatamenuConvertType
��          �� : 
��    ��    �l : 
�@          �\ : �摜�������̌^�ϊ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDatamenuConvertType()
{
	CConvertDataDlg  cvt;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	cvt.DoModal();
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�E�{�^�����j���[�F�摜�������̃T�C�Y�ϊ�
��    ��    �� :OnDatamenuResize
��          �� : 
��    ��    �l : 
�@          �\ : �摜�������̃T�C�Y�ϊ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDatamenuResize()
{
	CResizeDataDlg  res;

		if(!API.GetDataUnit(CURRENT_IMAGE))	return;
	res.DoModal();
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�E�{�^�����j���[�F�摜�������̍폜
��    ��    �� :OnDatamenuDelete
��          �� : 
��    ��    �l : 
�@          �\ : �摜�������̍폜
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDatamenuDelete()
{
	CString	str;

	str.Format("�摜������ %d �̃f�[�^���폜���܂��D��낵���ł����H" , API.GetCurrentImageNumber());
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	API.BackupDataUnit(CURRENT_IMAGE);
	API.DeleteDataUnit(CURRENT_IMAGE);
}


/********************************************************************
�@  �\  ��  �� :�}�E�X�E�{�^�����j���[�F�\���摜���r�b�g�}�b�v�Ƃ��ĕۑ�
��    ��    �� :OnDatamenuSavebitmap
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDatamenuSavebitmap()
{
	API.SaveViewAsBitmap( API.GetCurrentImageNumber() );
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�E�{�^�����j���[�F�摜�f�[�^�𕜌�
��    ��    �� :OnDatamenuUndo
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDatamenuUndo()
{
	CString	str;

	str.Format("�摜������ %d �Ƀf�[�^�𕜌����܂��D��낵���ł����H" , API.GetCurrentImageNumber());
		if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONQUESTION ) != IDOK )	return;

	API.RecoverDataUnit(CURRENT_IMAGE);
}

/********************************************************************
�@  �\  ��  �� :3�����摜�ꗗ�\��
��    ��    �� :OnDatamenuDisp3d
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CDataDlg::OnDatamenuDisp3d()
{
	API.DialogOpen(IDD_3D_IMAGE_DISP_DLG, true);
}