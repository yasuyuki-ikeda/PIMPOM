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
//CBinalizeDlg
//�Q�l���ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "C3DImgDispDlg.h"

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
extern	CPimpomAPI	API;

// *************************************
//         ��    ��    �Q    ��         
// *************************************

/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : clear_buffer
��          �� :
��    ��    �l : 
�@          �\ :�@

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void C3DImgDispDlg::clear_buffer()
{
	for (int i = 0; i < TD_IMAGE_DISP_MAX; i++)
	{
		DispStruct[i].pDispBuffers = NULL;
		DispStruct[i].pageNumber = -1;
	}

	delete[]	pMemories;
	pMemories = NULL;
}



/********************************************************************
�@  �\  ��  �� : 3D�摜�f�[�^�ꗗ�\���̈ʒu�^�T�C�Y��ݒ肷��
��    ��    �� : arrange_images
��          �� : 
��    ��    �l : �E�B���h�E���ɕ\���ł���f�[�^��
�@          �\ :�@DispStruct[]�̃����odispOffsetX,dispOffsetY,dispWidth,dispHeight��ݒ肷��
                  �E�B���h�E��ɕ\���ł���ꍇ�́A���ꂼ��ɐ��l������
                  �E�B���h�E��ɕ\���ł��Ȃ��ꍇ�́A���ꂼ��ɕ��l������
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
int C3DImgDispDlg::arrange_images()
{
	RECT windowRect;//���̃E�B���h�E�̃N���C�A���g�̈��\����`
	::GetClientRect(m_hWnd, &windowRect);


	float scale;
	API.GetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, &scale);

	int offsetX = TD_IMAGE_DISP_OFFSET_X;
	int offsetY = TD_IMAGE_DISP_OFFSET_Y;
	int width = (int)( (float)API.main_image_size.cx * scale);
	int height = (int)((float)API.main_image_size.cy * scale);


	//�E�B���h�E���ɓ��������W�z�u����
	
	int bufferCnt = 0;
	for (; bufferCnt < TD_IMAGE_DISP_MAX; bufferCnt++)
	{
		//�܂�Ԃ�
		if (offsetX + width >= windowRect.right - windowRect.left){
			offsetX = TD_IMAGE_DISP_OFFSET_X;
			offsetY += height + SMALL_MARGN_Y;

			if (offsetY + height + SMALL_MARGN_Y> windowRect.bottom - windowRect.top){
				break;
			}
		}

		//�\���ʒu�ƃT�C�Y��ݒ�
		DispStruct[bufferCnt].dispOffsetX = offsetX;
		DispStruct[bufferCnt].dispOffsetY = offsetY;
		DispStruct[bufferCnt].dispWidth = width;
		DispStruct[bufferCnt].dispHeight = height;


		offsetX += width + 5/*SMALL_MARGN_X*/;//�\���ʒu��i�߂�
	}

	//�E�B���h�E���ɓ��肫��Ȃ����͔̂�\���Ƃ���(�T�C�Y��-1�ɂ���)
	for (int n = bufferCnt; n < TD_IMAGE_DISP_MAX; n++)
	{
		DispStruct[bufferCnt].dispOffsetX = -1;
		DispStruct[bufferCnt].dispOffsetY = -1;
		DispStruct[n].dispWidth = -1;
		DispStruct[n].dispHeight = -1;
	}

	return bufferCnt;
}


/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�\���p�������̐ݒ�
��    ��    �� : copy_disp_buffers
��          �� : int image_number	(in)�Ώۉ摜�������ԍ�(-1�w��̏ꍇ�́A���ݑI�𒆂̉摜������)
				 int page_offset	(in)�\���J�n����y�[�W�ԍ�
				 int disp_cnt		(in)�\������y�[�W�ԍ�
��    ��    �l : ����������true
�@          �\ : pMemories�ɕ\���p���������m�ۂ���
                 DispStruct[]�̃����o pDispBuffers, pageNumber��ݒ肷��
                  
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool C3DImgDispDlg::copy_disp_buffers(int image_number, int page_offset, int disp_cnt)
{
	//�\���f�[�^�̎擾
	CDataUnit *pDU = API.GetDataUnit(image_number);
	if (pDU == NULL){//�\���f�[�^���Ȃ��ꍇ�̓G���[�I��
		return false;
	}

	
	//�o�b�t�@��������N���A
	clear_buffer();

	//�\���o�b�t�@�̃������m��
	int bufferLen = pDU->PageNumber - page_offset;
	if (bufferLen > TD_IMAGE_DISP_MAX){
		bufferLen = TD_IMAGE_DISP_MAX;
	}

	if (pDU->pDispBufferRGB){//RGB
		pMemories = new BYTE[pDU->DispBufferRowRGB * pDU->DataSize.cy * bufferLen];
	}
	else{//RGB�ȊO
		pMemories = new BYTE[pDU->DispBufferRow * pDU->DataSize.cy * bufferLen];
	}

	if (pMemories == NULL){
		return false;//�������m�ێ��s���̓G���[�I��
	}



	int orgDispPage = pDU->DispPage;//���C����ʂɕ\������Ă����y�[�W�ԍ��ޔ�

	//�\���摜�쐬���o�b�t�@�ɃR�s�[
	for (int bufferCnt = 0, dispPage = page_offset; 
		dispPage < pDU->PageNumber && bufferCnt<TD_IMAGE_DISP_MAX && bufferCnt<disp_cnt;
		dispPage++, bufferCnt++)
	{

		if (DispStruct[bufferCnt].dispWidth>0 && DispStruct[bufferCnt].dispHeight>0)
		{//�摜���\���ł���ꍇ�̂�

			//���C����ʕ\���摜�̍쐬
			pDU->DispPage = dispPage;
			pDU->UpdateDispBuffer();

			if (pDU->Average == 0.0 && pDU->Stdiv == 0 && API.GetCheck(m_hWnd, IDC_CHECK_MASK))
			{
				DispStruct[bufferCnt].mask = 1;
			}
			else{
				DispStruct[bufferCnt].mask = 0;
			}

			//�\���摜���o�b�t�@�ɃR�s�[
			DispStruct[bufferCnt].pageNumber = dispPage;
			if (pDU->pDispBufferRGB)
			{//RGB
				DispStruct[bufferCnt].pDispBuffers = &pMemories[pDU->DispBufferRowRGB * pDU->DataSize.cy * bufferCnt];

				memcpy(DispStruct[bufferCnt].pDispBuffers, pDU->pDispBufferRGB, pDU->DispBufferRowRGB * pDU->DataSize.cy);
			}
			else{//RGB�ȊO
				DispStruct[bufferCnt].pDispBuffers = &pMemories[pDU->DispBufferRow * pDU->DataSize.cy * bufferCnt];

				memcpy(DispStruct[bufferCnt].pDispBuffers, pDU->pDispBuffer, pDU->DispBufferRow * pDU->DataSize.cy);
			}
		}
	}

	//���C����ʕ\���y�[�W�����Ƃɂ��ǂ�
	pDU->DispPage = orgDispPage;
	pDU->UpdateDispBuffer();


	return true;
}


/********************************************************************
�@  �\  ��  �� : �摜�f�[�^�ꗗ�`��
��    ��    �� : disp_images
��          �� : int image_number	(in)�Ώۉ摜�������ԍ�(-1�w��̏ꍇ�́A���ݑI�𒆂̉摜������)
��    ��    �l : ����������true
�@          �\ : DispStruct[]�̓��e��`�悷��

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool C3DImgDispDlg::draw_images(int image_number)
{
	//�\���f�[�^�̎擾
	CDataUnit *pDU = API.GetDataUnit(image_number);
	if (pDU == NULL){//�\���f�[�^���Ȃ��ꍇ�̓G���[�I��
		return false;
	}

	//�w�i�`��
	{
		RECT windowRect;
		::GetClientRect(m_hWnd, &windowRect);

		HDC hdc = ::GetDC(m_hWnd);
		HPEN hpen = ::CreatePen(PS_SOLID, 1, API.default_bg_color);
		HBRUSH hbrush = ::CreateSolidBrush(API.default_bg_color);
		::SelectObject(hdc, hpen);
		::SelectObject(hdc, hbrush);
		::Rectangle(hdc, windowRect.left, windowRect.top + 25, windowRect.right, windowRect.bottom);
		::DeleteObject(hbrush);
		::DeleteObject(hpen);
		::ReleaseDC(m_hWnd, hdc);
	}

	//�摜�`��
	for (int bufferCnt = 0; bufferCnt < TD_IMAGE_DISP_MAX; bufferCnt++)
	{
		if (DispStruct[bufferCnt].pDispBuffers != NULL && 
			DispStruct[bufferCnt].pageNumber >= 0 &&
			DispStruct[bufferCnt].dispWidth>0 && DispStruct[bufferCnt].dispHeight>0)
		{
			//�摜��`�悷��
			HDC hdc = ::GetDC(m_hWnd);

			::SetStretchBltMode(hdc, HALFTONE);//���ԐF�ŕ�Ԃ���

			if ((pDU->DataType != RGB_FORMAT && pDU->DataType != RGB_3D_FORMAT))
			{//RGB�ȊO
				::StretchDIBits(hdc,
					DispStruct[bufferCnt].dispOffsetX, DispStruct[bufferCnt].dispOffsetY,
					DispStruct[bufferCnt].dispWidth, DispStruct[bufferCnt].dispHeight,
					0, 0, pDU->DataSize.cx, pDU->DataSize.cy,
					DispStruct[bufferCnt].pDispBuffers,
					pDU->pDispBufferBmpInfo, DIB_RGB_COLORS, SRCCOPY);

			}
			else
			{//RGB
				::StretchDIBits(hdc,
					DispStruct[bufferCnt].dispOffsetX, DispStruct[bufferCnt].dispOffsetY,
					DispStruct[bufferCnt].dispWidth, DispStruct[bufferCnt].dispHeight,
					0, 0, pDU->DataSize.cx, pDU->DataSize.cy,
					DispStruct[bufferCnt].pDispBuffers,
					pDU->pDispBufferBmpInfoRGB, DIB_RGB_COLORS, SRCCOPY);
			}


			//�\���f�[�^�̃y�[�W�ԍ���`��
			HFONT hfont = ::CreateFontIndirect(&API.default_font);
			::SelectObject(hdc, hfont);

			char strbuf[256] = { 0 };
			sprintf(strbuf, "%d", DispStruct[bufferCnt].pageNumber);
			::TextOut(hdc, DispStruct[bufferCnt].dispOffsetX, DispStruct[bufferCnt].dispOffsetY + DispStruct[bufferCnt].dispHeight + 5, strbuf, 256);

			::DeleteObject(hfont);


			//�}�X�N���͐Ԙg
			if (DispStruct[bufferCnt].mask)
			{
				HPEN hpen = ::CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				::SelectObject(hdc, hpen);
				::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
				::Rectangle(hdc, DispStruct[bufferCnt].dispOffsetX - 1,
					DispStruct[bufferCnt].dispOffsetY - 1,
					DispStruct[bufferCnt].dispOffsetX + DispStruct[bufferCnt].dispWidth + 1,
					DispStruct[bufferCnt].dispOffsetY + DispStruct[bufferCnt].dispHeight + 1);
				::DeleteObject(hpen);
			}


			//���C����ʂŕ\�����̃y�[�W�̏ꍇ�͘g��\��
			if (DispStruct[bufferCnt].pageNumber == pDU->DispPage)
			{
				HPEN hpen = ::CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
				::SelectObject(hdc, hpen);
				::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
				::Rectangle(hdc, DispStruct[bufferCnt].dispOffsetX - 4, 
								 DispStruct[bufferCnt].dispOffsetY - 4, 
								 DispStruct[bufferCnt].dispOffsetX + DispStruct[bufferCnt].dispWidth + 4,
								 DispStruct[bufferCnt].dispOffsetY + DispStruct[bufferCnt].dispHeight + 4);
				::DeleteObject(hpen);
			}


			::ReleaseDC(m_hWnd, hdc);
		}
	}

	//�E�B���h�E�^�C�g���Ƀf�[�^���\��
	CString str;
	str.Format(" (%d x %d x %d)", pDU->DataSize.cx, pDU->DataSize.cy, pDU->PageNumber);
	::SetWindowText(m_hWnd, pDU->DataName + str);


	//�\���Z�x	
	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_MIN, pDU->BrightMin);
	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_MAX, pDU->BrightMax);
	

	return true;
}

/********************************************************************
�@  �\  ��  �� : �X���C�_�[��ݒ肷��
��    ��    �� : set_slider
��          �� :
��    ��    �l : �X���C�_�[�̒l�i���\���J�n�y�[�W�ԍ��j
�@          �\ : �E�B���h�E��ɕ\���ł���摜�����A�\���f�[�^�̃y�[�W����菭�Ȃ��ꍇ�A
�@�@�@�@�@�@�@�@�@�X���C�_�[��p���āA�\�������ԁi�\���J�n�y�[�W�ԍ��j�����߂�
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
int C3DImgDispDlg::set_slider(int image_number)
{
	if (pMemories == NULL){//�\���p����������������Ă��Ȃ���Ή������Ȃ�
		return 0;
	}

	//�\���f�[�^�擾
	CDataUnit *pDU = API.GetDataUnit(image_number);
	if (pDU == NULL){
		return 0;
	}

	//�E�B���h�E��ɕ\���ł���摜�����擾
	int dispImgMax = arrange_images();

	int prevPos = m_slider.GetPos();
	int newPos;

	if (pDU->PageNumber > dispImgMax)
	{//�\���ł���摜�����A�\���f�[�^�̃y�[�W����菭�Ȃ��ꍇ�́A�X���C�_�[���g�p���ĕ\���͈͂�I������
		
		m_slider.SetRange(0, pDU->PageNumber - dispImgMax);

		if (prevPos >= 0 && prevPos <= pDU->PageNumber - dispImgMax){
			newPos = prevPos;
		}
		else{
			newPos = 0;
			
		}
		m_slider.SetPos(newPos);
		m_slider.ShowWindow(SW_SHOW);
	}
	else
	{//�\���ł���摜�����A�\���f�[�^�̃y�[�W���ȏ�̏ꍇ�̓X���C�_�[�͎g�p���Ȃ�
		newPos = 0;
		m_slider.SetPos(newPos);
		m_slider.ShowWindow(SW_HIDE);
	}

	return newPos;
}




/********************************************************************
�@  �\  ��  �� : 3D�摜�f�[�^�ꗗ�\��
��    ��    �� : DispImage
��          �� :(in)�Ώۂ̉摜�������ԍ�(-1�̏ꍇ�͌��ݑI�𒆂̉摜�������ԍ�)
��    ��    �l :����������true
�@          �\ :

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool C3DImgDispDlg::DispImage(int image_number)
{
	int pos = set_slider(image_number);

	if (!copy_disp_buffers(image_number, pos))	return false;

	if (!draw_images(image_number))	return false;

	if (image_number < 0)		image_number = API.GetCurrentImageNumber();
	imageNumber = image_number;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(C3DImgDispDlg, CDialog)

C3DImgDispDlg::C3DImgDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3DImgDispDlg::IDD, pParent)
{
	for (int i = 0; i < TD_IMAGE_DISP_MAX; i++)
	{
		memset(&DispStruct[i], 0, sizeof(TD_IMAGE_DISP_STRUCT));
		//DispStruct[i].pDispBuffers = NULL;
		DispStruct[i].pageNumber = -1;
	}
	pMemories = NULL;

	imageNumber = -1;
}

C3DImgDispDlg::~C3DImgDispDlg()
{
	clear_buffer();
}

void C3DImgDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_DISP_PAGE, m_slider);
}


BEGIN_MESSAGE_MAP(C3DImgDispDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_3D_IMG_DISP_SCALE, &C3DImgDispDlg::OnBnClickedButton3dImgDispScale)
	ON_BN_CLICKED(IDC_BUTTON_3D_IMG_DISP_SCALE2, &C3DImgDispDlg::OnBnClickedButton3dImgDispScale2)
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_DISP_MINMAX, &C3DImgDispDlg::OnBnClickedButtonDispMinmax)
	ON_BN_CLICKED(IDC_CHECK_MASK, &C3DImgDispDlg::OnBnClickedCheckMask)
END_MESSAGE_MAP()


// C3DImgDispDlg ���b�Z�[�W �n���h���[

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�̏�����
��    ��    �� : OnInitDialog
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ -------------- - -------------------------------------- -
Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL C3DImgDispDlg::OnInitDialog()
{
	CDialog::OnInitDialog();



	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, (float)API.small_image_size.cx / (float)API.main_image_size.cx);
	imageNumber = -1;

	DispImage(-1);//���ݑI�𒆂̉摜��\���ΏۂƂ���

	return true;
}


/********************************************************************
�@  �\  ��  �� : �E�B���h�E�ĕ`��
��    ��    �� : OnPaint
��          �� :
��    ��    �l :
�@          �\ :

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void C3DImgDispDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CDialog::OnPaint() ���Ăяo���Ȃ��ł��������B

	draw_images(imageNumber);
}


/********************************************************************
�@  �\  ��  �� : �E�B���h�E���T�C�Y
��    ��    �� : DispImage
��          �� :
��    ��    �l :
�@          �\ :

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void C3DImgDispDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	DispImage(imageNumber);
}


/********************************************************************
�@  �\  ��  �� : �\���{���g��{�^������
��    ��    �� : OnBnClickedButton3dImgDispScale
��          �� :
��    ��    �l :
�@          �\ :

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void C3DImgDispDlg::OnBnClickedButton3dImgDispScale()
{
	float scale;
	API.GetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, &scale);
	scale *= 2;
	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, scale);


	DispImage(imageNumber);
}

/********************************************************************
�@  �\  ��  �� : �\���{���k���{�^������
��    ��    �� : OnBnClickedButton3dImgDispScale2
��          �� :
��    ��    �l :
�@          �\ :

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void C3DImgDispDlg::OnBnClickedButton3dImgDispScale2()
{
	float scale;
	API.GetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, &scale);
	scale /= 2;
	API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_SCALE, scale);

	DispImage(imageNumber);
}

/********************************************************************
�@  �\  ��  �� : �X���C�_�[����
��    ��    �� : OnHScroll
��          �� :
��    ��    �l :
�@          �\ :

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void C3DImgDispDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar == (CScrollBar*)&m_slider)//�O���[�o��臒l�ݒ�
	{
		DispImage(imageNumber);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


/********************************************************************
�@  �\  ��  �� : �}�E�X�E�N���b�N
��    ��    �� : OnLButtonDown
��          �� :
��    ��    �l :
�@          �\ :

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void C3DImgDispDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//�\���f�[�^�擾
	CDataUnit *pDU = API.GetDataUnit(imageNumber);
	if (pDU)
	{
		for (int bufferCnt = 0; bufferCnt < TD_IMAGE_DISP_MAX; bufferCnt++)
		{
			if (DispStruct[bufferCnt].pDispBuffers != NULL &&
				DispStruct[bufferCnt].pageNumber >= 0 &&
				DispStruct[bufferCnt].dispWidth>0 && DispStruct[bufferCnt].dispHeight > 0)
			{
				if (DispStruct[bufferCnt].dispOffsetX < point.x &&
					DispStruct[bufferCnt].dispOffsetX + DispStruct[bufferCnt].dispWidth > point.x &&
					DispStruct[bufferCnt].dispOffsetY < point.y &&
					DispStruct[bufferCnt].dispOffsetY + DispStruct[bufferCnt].dispHeight > point.y)
				{
					//�\���y�[�W��ς��čĕ`��
					pDU->DispPage = DispStruct[bufferCnt].pageNumber;
					API.DrawImage(imageNumber);
					DispImage(imageNumber);
					break;
				}
			}
		}
			
	}


	CDialog::OnLButtonDown(nFlags, point);
}




/********************************************************************
�@  �\  ��  �� : min-max�{�^������
��    ��    �� : OnBnClickedButtonDispMinmax
��          �� :
��    ��    �l :
�@          �\ :

���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void C3DImgDispDlg::OnBnClickedButtonDispMinmax()
{
	//�\���f�[�^�擾
	CDataUnit *pDU = API.GetDataUnit(imageNumber);
	if (pDU)
	{
		//�Z�x�㉺���l�̏���
		float maxAll, minAll, min, max;
		pDU->GetDataMinMax(&maxAll, &minAll);

		int orgDispPage = pDU->DispPage;//���C����ʂɕ\������Ă����y�[�W�ԍ��ޔ�
		
		//�S�y�[�W�ŔZ�x�㉺���l��T��
		for (int n = 0; n < pDU->PageNumber; n++)
		{
			pDU->DispPage = n;
			pDU->GetDataMinMax(&min, &max);

			if (minAll > min){
				minAll = min;
			}

			if (maxAll < max){
				maxAll = max;
			}
		}

		pDU->DispPage = orgDispPage;//�\���y�[�W�ԍ����A

		pDU->SetDispRange(minAll, maxAll);//�Z�x�㉺���ݒ�
		API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_MIN, minAll);
		API.SetEditValue(m_hWnd, IDC_EDIT_3D_IMG_DISP_MAX, maxAll);

		DispImage(imageNumber);//�摜�`��

		API.DrawImage();
	}
}



void C3DImgDispDlg::OnBnClickedCheckMask()
{
	DispImage(imageNumber);
}