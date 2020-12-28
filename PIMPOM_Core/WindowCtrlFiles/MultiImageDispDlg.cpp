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
//MultiImageDispDlg
//�����摜�����\���E�B���h�E
/*******************************************************************/

#include "stdafx.h"
#include "PIMPOM_API.h"
#include "MultiImageDispDlg.h"


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


// CMultiImageDispDlg �_�C�A���O


//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
�@  �\  ��  �� : �\���摜�Ɋ֘A����X�N���[���o�[�̃|�C���^���擾����
��    ��    �� : get_disp_scroll_bar
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::get_disp_scroll_bar(int dispImgNo, CScrollBar **pp_h_scroll, CScrollBar **pp_v_scroll)
{
	int hScrollBarID[]={IDC_SCROLL_MULTI_IMG_DLG_H0, IDC_SCROLL_MULTI_IMG_DLG_H1, IDC_SCROLL_MULTI_IMG_DLG_H2, IDC_SCROLL_MULTI_IMG_DLG_H3,IDC_SCROLL_MULTI_IMG_DLG_H4 ,IDC_SCROLL_MULTI_IMG_DLG_H5 };
	int vScrollBarID[]={IDC_SCROLL_MULTI_IMG_DLG_V0, IDC_SCROLL_MULTI_IMG_DLG_V1, IDC_SCROLL_MULTI_IMG_DLG_V2, IDC_SCROLL_MULTI_IMG_DLG_V3,IDC_SCROLL_MULTI_IMG_DLG_V4 ,IDC_SCROLL_MULTI_IMG_DLG_V5 };

	*pp_h_scroll = (CScrollBar*)GetDlgItem( hScrollBarID[dispImgNo] );
	*pp_v_scroll = (CScrollBar*)GetDlgItem( vScrollBarID[dispImgNo] );
}



/********************************************************************
�@  �\  ��  �� : �\���摜�f�[�^���擾����
��    ��    �� : get_disp_image_data
��          �� : 
��    ��    �l : �\���摜�f�[�^�̐擪�|�C���^
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
CDataUnit *CMultiImageDispDlg::get_disp_image_data(int dispImgNo)
{
	return  API.GetDataUnit(m_disp_image_number[dispImgNo]);
}


/********************************************************************
�@  �\  ��  �� : �X�N���[���o�[��ݒ肷��
��    ��    �� : set_scrollbar
��          �� : CScrollBar		*sc		(out)�X�N���[���o�[�̃R���g���[���Q��
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
void CMultiImageDispDlg::set_scrollbar(CScrollBar *sc, int page, int pos, int min, int max)
{
	SCROLLINFO  scrollinfo;
	sc->GetScrollInfo( &scrollinfo , SIF_ALL );
	scrollinfo.nPage = page;
	scrollinfo.nPos  = pos ;
	scrollinfo.nMin  = min;
	scrollinfo.nMax  = max;
	sc->SetScrollInfo( &scrollinfo , true );	
}



/********************************************************************
�@  �\  ��  �� : �摜�\���̈���Z�o����
��    ��    �� : calc_image_regn
��          �� : CRect imageRect[]   //�摜�\���̈�
��    ��    �l : �摜�\���̈搔
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CMultiImageDispDlg::calc_image_regn(CRect imageRect[])
{
	int imageCnt=0;
	RECT clientRct={0};//�N���C�A���g�̈��\����`
	CRect canvasRect;//�摜�`�悷��̈�

	::GetClientRect(m_hWnd, &clientRct);//���̃E�B���h�E�̃N���C�A���g�̈��\����`���擾

	canvasRect.left =  2;
	canvasRect.right = clientRct.right - clientRct.left - 4;
	canvasRect.top =  40;
	canvasRect.bottom = clientRct.bottom - clientRct.top - 2;

	//������ƕ����ł���悤�ɕ`��̈�̃T�C�Y�������ɂ���
	if( canvasRect.Width()%2 !=0 )	canvasRect.right +=1;
	if( canvasRect.Height()%2 !=0 )	canvasRect.bottom +=1;



	if(m_cmb_disp_num.GetCurSel()==0)//2�摜�\����
	{
		//�摜0 = ��
		imageRect[0].left   = canvasRect.left;
		imageRect[0].top    = canvasRect.top;
		imageRect[0].right  = canvasRect.left + canvasRect.Width()/2 -1;
		imageRect[0].bottom = canvasRect.bottom;

		//�摜1 = �E
		imageRect[1].left   = canvasRect.left + canvasRect.Width()/2;
		imageRect[1].top    = canvasRect.top;
		imageRect[1].right  = canvasRect.right;
		imageRect[1].bottom = canvasRect.bottom;

		imageCnt = 2;
	}
	else if (m_cmb_disp_num.GetCurSel() == 1)
	{//4�摜�\��
		//�摜0 = ����
		imageRect[0].left   = canvasRect.left;
		imageRect[0].top    = canvasRect.top;
		imageRect[0].right  = canvasRect.left + canvasRect.Width()/2 -1;
		imageRect[0].bottom = canvasRect.top + canvasRect.Height()/2 -1;

		//�摜1 = �E��
		imageRect[1].left   = canvasRect.left + canvasRect.Width()/2;
		imageRect[1].top    = canvasRect.top;
		imageRect[1].right  = canvasRect.right;
		imageRect[1].bottom = canvasRect.top + canvasRect.Height()/2 -1;

		//�摜2 = ����
		imageRect[2].left   = canvasRect.left;
		imageRect[2].top    = canvasRect.top + canvasRect.Height()/2;
		imageRect[2].right  = canvasRect.left + canvasRect.Width()/2 -1;
		imageRect[2].bottom = canvasRect.bottom;

		//�摜3 = �E��
		imageRect[3].left   = canvasRect.left + canvasRect.Width()/2;
		imageRect[3].top    = canvasRect.top + canvasRect.Height()/2;
		imageRect[3].right  = canvasRect.right;
		imageRect[3].bottom = canvasRect.bottom;

		imageCnt = 4;
	}
	else
	{//6��ʕ\��
	 //�摜0 = ����
		imageRect[0].left = canvasRect.left;
		imageRect[0].top = canvasRect.top;
		imageRect[0].right = canvasRect.left + canvasRect.Width() / 3 - 1;
		imageRect[0].bottom = canvasRect.top + canvasRect.Height() / 2 - 1;

		//�摜1 = ������
		imageRect[1].left = canvasRect.left + canvasRect.Width() / 3;
		imageRect[1].top = canvasRect.top;
		imageRect[1].right = canvasRect.left + canvasRect.Width() *2 / 3 -1;
		imageRect[1].bottom = canvasRect.top + canvasRect.Height() / 2 - 1;

		//�摜2 = �E��
		imageRect[2].left = canvasRect.left + canvasRect.Width() * 2 / 3;
		imageRect[2].top = canvasRect.top;
		imageRect[2].right = canvasRect.right;
		imageRect[2].bottom = canvasRect.top + canvasRect.Height() / 2 - 1;

		//�摜3 = ����
		imageRect[3].left = canvasRect.left;
		imageRect[3].top = canvasRect.top + canvasRect.Height() / 2;
		imageRect[3].right = canvasRect.left + canvasRect.Width() / 3 - 1;
		imageRect[3].bottom = canvasRect.bottom;

		//�摜4 = ������
		imageRect[4].left = canvasRect.left + canvasRect.Width() / 3;
		imageRect[4].top = canvasRect.top + canvasRect.Height() / 2;
		imageRect[4].right = canvasRect.left + canvasRect.Width() *2/ 3 - 1;
		imageRect[4].bottom = canvasRect.bottom;

		//�摜5 = �E��
		imageRect[5].left = canvasRect.left + canvasRect.Width() *2 / 3;
		imageRect[5].top = canvasRect.top + canvasRect.Height() / 2;
		imageRect[5].right = canvasRect.right;
		imageRect[5].bottom = canvasRect.bottom;

		imageCnt = 6;
	}



	return imageCnt;
}

/********************************************************************
�@  �\  ��  �� : �摜����у_�C�A���O�A�C�e���̔z�u
��    ��    �� : arrange_dlg_item
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::arrange_dlg_item()
{
	CRect imageRegn[6]={0};//�摜�̕\���̈�
	int	imageCnt=0;


	for(int i=0 ; i<6 ; i++){
		if(p_disp_image_field[i]==NULL){//�\���̈�̃��������m�ۂ̏ꍇ�͂Ȃɂ����Ȃ�
			return;
		}
	}


	if( m_cmb_disp_num.GetCurSel()==0)//2�摜�\����
	{
		//�X�N���[���o�[�̕\��
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V5)->ShowWindow(SW_HIDE);
	}
	else if (m_cmb_disp_num.GetCurSel() == 1)
	{//4�摜�\��

		//�X�N���[���o�[�̕\��
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V5)->ShowWindow(SW_HIDE);
	}
	else {
		//6�摜�\��

		//�X�N���[���o�[�̕\��
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_H5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCROLL_MULTI_IMG_DLG_V5)->ShowWindow(SW_SHOW);
	}




	//�摜�\�����Ƃ̐ݒ�

	imageCnt = calc_image_regn(imageRegn);//�̈�v�Z

	for(int i=0 ; i<imageCnt ; i++)
	{
		CRect rct(imageRegn[i]);//ImageField�ŕ`�悷���`�̈�
		rct += CRect(-3,-3,-13,-13);
		
		if(rct.top>rct.bottom || rct.left>rct.right)	continue;


		//�摜�`��̈ʒu�ݒ�
		p_disp_image_field[i]->FieldCoordinate.x = rct.left;
		p_disp_image_field[i]->FieldCoordinate.y = rct.top;
		p_disp_image_field[i]->FieldSize.cx = rct.Width();
		p_disp_image_field[i]->FieldSize.cy = rct.Height();

		p_disp_image_field[i]->DrawImage( get_disp_image_data(i), true);//�摜�`��


		//�X�N���[���o�[�̈ʒu�ݒ�
		CScrollBar *p_h_scroll, *p_v_scroll;
		get_disp_scroll_bar(i, &p_h_scroll, &p_v_scroll);
		p_h_scroll->MoveWindow( rct.left  , rct.bottom+1 , rct.Width() , 10 );//���������X�N���[���o�[�̔z�u
		p_v_scroll->MoveWindow( rct.right+1 , rct.top     , 10, rct.Height() );//���������X�N���[���o�[�̔z�u


		//�X�N���[���o�[�̃X�N���[���ʂ��摜�ɂ��킹�Đݒ�
		update_scrollbar(i);

		//�摜��ɘg�Ɖ摜�ԍ���\��
		update_image_overlap(i);

	}

}






/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : update_scrollbar
��          �� : 
��    ��    �l : �X�N���[���o�[�̃X�N���[���ʂ�ݒ肷��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::update_scrollbar(int dispImgNo)
{
	CScrollBar *p_h_scroll, *p_v_scroll;

	get_disp_scroll_bar(dispImgNo, &p_h_scroll, &p_v_scroll);//�X�N���[���o�[�̃|�C���^���擾

	CDataUnit *pDispImageData = get_disp_image_data(dispImgNo);//�摜�f�[�^�擾

	if(pDispImageData!=NULL)//�f�[�^������ꍇ�̓X�N���[���o�[�T�C�Y���f�[�^�T�C�Y�ɍ��킹��
	{
			CSize src_size;
			p_disp_image_field[dispImgNo]->GetSrcSize(pDispImageData, &src_size);//�摜�]�����̋�`�̈�̃T�C�Y���擾

			if( src_size.cx < pDispImageData->DataSize.cx ){//���������X�N���[���o�[�������Ƃ����摜�]�����̕����摜�S�̂̕���菬�����ꍇ
				set_scrollbar( p_h_scroll, src_size.cx, pDispImageData->DataDispOffset.x, 0, pDispImageData->DataSize.cx - 1);
			}else{
				set_scrollbar( p_h_scroll, pDispImageData->DataSize.cx, 0, 0, 0);
			}

			if( src_size.cy < pDispImageData->DataSize.cy ){//���������X�N���[���o�[�������Ƃ����摜�]�����̍������摜�S�̂̍�����菬�����ꍇ
				set_scrollbar( p_v_scroll, src_size.cy, pDispImageData->DataDispOffset.y, 0, pDispImageData->DataSize.cy - 1);
			}else{
				set_scrollbar( p_v_scroll, pDispImageData->DataSize.cy, 0, 0, 0);
			}
	}
	else//�f�[�^���Ȃ��ꍇ�̓X�N���[���o�[�̓t���T�C�Y
	{
		set_scrollbar( p_h_scroll, 0, 0, 0, 0);
		set_scrollbar( p_v_scroll, 0, 0, 0, 0);
	}
}

/********************************************************************
�@  �\  ��  �� :
��    ��    �� : update_image_overlap
��          �� :
��    ��    �l : �摜��ɘg�E�ԍ���`�悷��
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::update_image_overlap(int dispImgNo)
{
	CRect imageRegn[6];

	calc_image_regn(imageRegn);//�̈�v�Z


	CDC *pdc = GetDC();//
	pdc->SelectStockObject(NULL_BRUSH);

	//�g�̕`��
	if (dispImgNo == m_selected_disp_image_no && API.GetCheck(m_hWnd, IDC_CHECK_MULTI_IMG_DLG_SELECT))
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, API.default_fg_color);
		CPen *poldpen = pdc->SelectObject(&pen);
		pdc->Rectangle(imageRegn[dispImgNo].left + 1, imageRegn[dispImgNo].top + 1, imageRegn[dispImgNo].right - 1, imageRegn[dispImgNo].bottom - 1);
		pdc->SelectObject(poldpen);
		pen.DeleteObject();
	}
	else
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, API.default_bg_color);
		CPen *poldpen = pdc->SelectObject(&pen);
		pdc->Rectangle(imageRegn[dispImgNo].left + 1, imageRegn[dispImgNo].top + 1, imageRegn[dispImgNo].right - 1, imageRegn[dispImgNo].bottom - 1);
		pdc->SelectObject(poldpen);
		pen.DeleteObject();
	}


	//�摜�������ԍ��̕`��
	CFont font;  font.CreateFontIndirect(&API.default_font);
	CFont *poldfont = pdc->SelectObject(&font);
	CString str; str.Format(_T("%d"), m_disp_image_number[dispImgNo]);
	pdc->TextOut(imageRegn[dispImgNo].left+3, imageRegn[dispImgNo].top+3, str);
	pdc->SelectObject(poldfont);
	font.DeleteObject();
	

	ReleaseDC(pdc);
}

/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : synchro_disp_image
��          �� : 
��    ��    �l : �摜�̕\����Ԃ�����̔ԍ��̂��̂ɂ��킹��
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::synchro_disp_image(int dispImageNo)
{
	CDataUnit *pDispImageData = get_disp_image_data(dispImageNo);//����̔ԍ��̕\���摜�f�[�^
	if(pDispImageData==NULL){
		return;
	}


	for(int i=0 ; i<6 ; i++)
	{
		if( m_cmb_disp_num.GetCurSel() == 0 && i >= 2 )	break;//2�摜�\����
		else if( m_cmb_disp_num.GetCurSel() == 1 && i>=4)	break;//4�摜�\����

		CDataUnit *p_du = get_disp_image_data(i);

		if(dispImageNo != i )
		{
			if( p_du != NULL)//�摜�f�[�^����
			{
				if(pDispImageData->DataSize == p_du->DataSize )//�摜�f�[�^�T�C�Y������摜�Ɠ���
				{
					p_du->DataDispOffset = pDispImageData->DataDispOffset;//�\���ʒu�����킹��
					p_du->DispScale = pDispImageData->DispScale ;//�\���{�������킹��

					p_disp_image_field[i]->DrawImage(p_du, true);//�摜�ĕ`��

					update_scrollbar(i);//�X�N���[���o�[�ĕ`��

					update_image_overlap(i);//�g�Ɖ摜�ԍ����ĕ`��
				}
			}
		}

	}

}


/********************************************************************
�@  �\  ��  �� : �摜�������ԍ��ݒ�
��    ��    �� : DispMultiImage
��          �� : 
��    ��    �l : 
�@          �\ : m_selected_disp_image_no �ɕ\������摜��������
�@�@�@�@�@�@�@�@���ݑI�𒆂̉摜�������ԍ��ɐݒ肷��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::DispMultiImage()
{
	if( API.GetCheck(m_hWnd, IDC_CHECK_MULTI_IMG_DLG_SELECT) )
	{
		if(m_selected_disp_image_no>=0)
		{
			m_disp_image_number[m_selected_disp_image_no] = API.GetCurrentImageNumber();
		}
	}

	arrange_dlg_item();
}

/********************************************************************
�@  �\  ��  �� :
��    ��    �� : control_scrollbar
��          �� : UINT			nSBCode		(in)
UINT			nPos		(in)
CScrollBar		*pScrollBar	(in)�X�N���[���o�[�̃R���g���[���ւ̃|�C���^
��    ��    �l : �����̃X�N���[���o�[�̈ʒu
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
long CMultiImageDispDlg::control_scrollbar(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	int dest_pos = pScrollBar->GetScrollPos();

	switch (nSBCode) {
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

//////////////////////////////////////////////////////////////////////////////////////////
//  �C�x���g�n���h��
//////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CMultiImageDispDlg, CDialog)

CMultiImageDispDlg::CMultiImageDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiImageDispDlg::IDD, pParent)
{
	for(int i=0 ; i<6 ; i++){
		p_disp_image_field[i] = NULL;
	}
	m_selected_disp_image_no = -1;
}

CMultiImageDispDlg::~CMultiImageDispDlg()
{
	for(int i=0 ; i<6 ; i++){
		if(p_disp_image_field[i] != NULL){
			delete	p_disp_image_field[i];
			p_disp_image_field[i] = NULL;
		}
	}
}

void CMultiImageDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MULTI_IMAGE_DISP_NUM, m_cmb_disp_num);
}


BEGIN_MESSAGE_MAP(CMultiImageDispDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_MULTI_IMG_DLG_SELECT, &CMultiImageDispDlg::OnBnClickedCheckMultiImgDlgSelect)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_MULTI_IMG_DLG_MAGNIFY0, &CMultiImageDispDlg::OnBnClickedButtonMultiImgDlgMagnify0)
	ON_BN_CLICKED(IDC_BUTTON_MULTI_IMG_DLG_MAGNIFY1, &CMultiImageDispDlg::OnBnClickedButtonMultiImgDlgMagnify1)
	ON_CBN_SELCHANGE(IDC_COMBO_MULTI_IMAGE_DISP_NUM, &CMultiImageDispDlg::OnCbnSelchangeComboMultiImageDispNum)
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
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
BOOL CMultiImageDispDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0 ; i<6 ; i++){
		p_disp_image_field[i] = new CImageField( m_hWnd, API.main_image_size, CPoint(MAIN_WIN_LEFT_MARGIN, MAIN_WIN_TOP_MARGIN)  );//ImageFiled�̃C���X�^���X��
		m_disp_image_number[i] = i;
	}
	

	m_cmb_disp_num.AddString("2�摜"); 
	m_cmb_disp_num.AddString("4�摜");
	m_cmb_disp_num.AddString("6�摜");
	m_cmb_disp_num.SetCurSel(0);

	m_selected_disp_image_no=0;

	arrange_dlg_item();

	m_is_draging = false;
	

	return true;
}


/********************************************************************
�@  �\  ��  �� : �E�B���h�E�̃��T�C�Y
��    ��    �� : OnSize
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	arrange_dlg_item();
}

/********************************************************************
�@  �\  ��  �� : �E�B���h�E�̍ĕ`��
��    ��    �� : OnPaint
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	arrange_dlg_item();
}


/********************************************************************
�@  �\  ��  �� : �\���摜�̃X�N���[��
��    ��    �� : scroll_image
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::scroll_image(int dispImageNo, int newOffsetX, int newOffsetY)
{
	CDataUnit *p_du = get_disp_image_data(dispImageNo);

	if (p_du == NULL)	return;//�f�[�^��������Ή������Ȃ�

	TRACE("%d,%d", newOffsetX, newOffsetY);

	p_disp_image_field[dispImageNo]->ScrollImage(p_du, CPoint(newOffsetX, newOffsetY));//�摜�]�����̋�`�̈���X�N���[��������
	p_disp_image_field[dispImageNo]->DrawImage(p_du, false);
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
void CMultiImageDispDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	for(int i=0 ; i<6 ; i++)//�ǂ̃X�N���[���o�[���������ꂽ�̂��T��
	{
		//�X�N���[���o�[�̃|�C���^���擾
		CScrollBar *p_h_scroll, *p_v_scroll;
		get_disp_scroll_bar(i, &p_h_scroll, &p_v_scroll);

		if( pScrollBar == p_h_scroll )
		{
			CDataUnit *p_du=NULL;//�\���摜�f�[�^
			CPoint		src_offset;
			long		new_pos;

			p_du = get_disp_image_data(i);

				if(p_du == NULL)	return;//�f�[�^��������Ή������Ȃ�

			p_disp_image_field[i]->GetSrcOffset(p_du, &src_offset);//�\�����̉摜�������̌��݂̃I�t�Z�b�g�i����̍��W�j���擾
			new_pos = control_scrollbar(nSBCode, nPos, pScrollBar);//�X�N���[���o�[����V�����I�t�Z�b�g(x)���擾

			scroll_image(i, new_pos, src_offset.y);//�摜�]�����̋�`�̈���X�N���[��������

			update_image_overlap(i);//�g�Ɖ摜�ԍ����ĕ`��

			synchro_disp_image(i);//���̉摜�̕\���ʒu�����̉摜�ɂ��킹��

			break;
		}
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
void CMultiImageDispDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	for(int i=0 ; i<6 ; i++)//�ǂ̃X�N���[���o�[���������ꂽ�̂��T��
	{
		//�X�N���[���o�[�̃|�C���^���擾
		CScrollBar *p_h_scroll, *p_v_scroll;
		get_disp_scroll_bar(i, &p_h_scroll, &p_v_scroll);

		if( pScrollBar == p_v_scroll )
		{
			CDataUnit *p_du=NULL;//�\���摜�f�[�^
			CPoint		src_offset;
			long		new_pos;

			p_du = get_disp_image_data(i);

				if(p_du == NULL)	return;//�f�[�^��������Ή������Ȃ�

			p_disp_image_field[i]->GetSrcOffset(p_du, &src_offset);//�\�����̉摜�������̌��݂̃I�t�Z�b�g�i����̍��W�j���擾
			new_pos = control_scrollbar(nSBCode, nPos, pScrollBar);//�X�N���[���o�[����V�����I�t�Z�b�g(y)���擾

			scroll_image(i, src_offset.x, new_pos);//�摜�]�����̋�`�̈���X�N���[��������

			update_image_overlap(i);//�g�Ɖ摜�ԍ����ĕ`��

			synchro_disp_image(i);//���̉摜�̕\���ʒu�����̉摜�ɂ��킹��

			break;
		}
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}




/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnBnClickedCheckMultiImgDlgSelect
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnBnClickedCheckMultiImgDlgSelect()
{
	arrange_dlg_item();//�ĕ`��
}



/********************************************************************
�@  �\  ��  �� : �}�E�X���{�^������
��    ��    �� : OnLButtonDown
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect imageRect[6];
	int imageCnt=0;

	
	imageCnt = calc_image_regn(imageRect);//�̈�v�Z

	for(int i=0 ; i<imageCnt ; i++)
	{
		if( imageRect[i].PtInRect(point) )
		{
			m_selected_disp_image_no = i;//�I��ԍ��ύX

			arrange_dlg_item();//�ĕ`��


			GetCapture();//�}�E�X���L���v�`���[����
			m_is_draging = true;//�h���b�O����\���t���O�𗧂Ă�
			m_last_clicked_point = point;//�h���b�O�J�n���W��ۑ�
			p_disp_image_field[i]->GetSrcOffset(get_disp_image_data(i), &m_drag_start_offset); //�h���b�O�J�n���̕\�����̉摜�������̃I�t�Z�b�g���擾


			HCURSOR csr = AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE);//�J�[�\�����h���b�O���̕��ɑւ���
			SetCursor(csr);

			break;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}




/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnLButtonUp
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();//�}�E�X�L���v�`���[���J������
	m_is_draging = false;


	CDialog::OnLButtonUp(nFlags, point);
}


/********************************************************************
�@  �\  ��  �� : �}�E�X�����{�^������
��    ��    �� : OnMButtonDown
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	CRect imageRect[6];
	int imageCnt = 0;


	imageCnt = calc_image_regn(imageRect);//�̈�v�Z

	for (int i = 0; i<imageCnt; i++)
	{
		if (imageRect[i].PtInRect(point))
		{
			m_selected_disp_image_no = i;//�I��ԍ��ύX

			arrange_dlg_item();//�ĕ`��


			GetCapture();//�}�E�X���L���v�`���[����
			m_is_draging = true;//�h���b�O����\���t���O�𗧂Ă�
			m_last_clicked_point = point;//�h���b�O�J�n���W��ۑ�
			p_disp_image_field[i]->GetSrcOffset(get_disp_image_data(i), &m_drag_start_offset); //�h���b�O�J�n���̕\�����̉摜�������̃I�t�Z�b�g���擾


			HCURSOR csr = AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE);//�J�[�\�����h���b�O���̕��ɑւ���
			SetCursor(csr);

			break;
		}
	}

	CDialog::OnMButtonDown(nFlags, point);
}

/********************************************************************
�@  �\  ��  �� :
��    ��    �� : OnMButtonUp
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();//�}�E�X�L���v�`���[���J������
	m_is_draging = false;

	CDialog::OnMButtonUp(nFlags, point);
}



/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnMouseMove
��    ��    �l : 
�@          �\ : �}�E�X�|�C���^���E�B���h�E��œ�����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect imageRect[6];
	int imageCnt=0;
	double srcX=-999, srcY=-999;

	//�}�E�X�|�C���^���摜�̈�ɓ����Ă��邩�ǂ���
	imageCnt = calc_image_regn(imageRect);

	for(int i=0 ; i<imageCnt ; i++)
	{
		if( imageRect[i].PtInRect(point) )//�ǂ��̃E�B���h�E��Ƀ}�E�X�J�[�\�������邩�`�F�b�N
		{
			CDataUnit *pdu = get_disp_image_data(i);
			if(get_disp_image_data(i))
			{
				if (m_is_draging) //�h���b�O���Ȃ�΁A�摜���X�N���[������
				{
					HCURSOR csr = AfxGetApp()->LoadCursor(IDC_CURSOR_MOVE);//�J�[�\�����h���b�O���̕��ɑւ���
					SetCursor(csr);

					
					double src0_x, src0_y, src1_x, src1_y;
					p_disp_image_field[i]->GetSrcCoor(pdu, m_last_clicked_point.x, m_last_clicked_point.y, &src0_x, &src0_y);//�h���b�O�J�n�̌��摜���W�擾
					p_disp_image_field[i]->GetSrcCoor(pdu, point.x, point.y, &src1_x, &src1_y);//�h���b�O�I���_�̌��摜���W�擾
					scroll_image(i, m_drag_start_offset.x - (src1_x - src0_x), m_drag_start_offset.y - (src1_y - src0_y));//�摜�]�����̋�`�̈���X�N���[��������

					update_scrollbar(i);//�X�N���[���o�[�ĕ`��

					update_image_overlap(i);//�g�Ɖ摜�ԍ����ĕ`��

					synchro_disp_image(i);//���̉摜�̕\���ʒu�����̉摜�ɂ��킹��

				}

				p_disp_image_field[i]->GetSrcCoor(pdu, point.x , point.y, &srcX, &srcY);//���摜��̍��W�擾

				break;
			}
		}
	}

	//�}�E�X�|�C���^���摜�̈�ɓ����Ă���΁A
	//�摜��̍��W����ъe�摜�̉�f�l��\������
	if(srcX>=0 && srcY>=0)
	{
		CString message,str;

		message.Format("(x,y)=(%d,%d)    ", (int)srcX, (int)srcY );

		for(int i=0 ; i<imageCnt ; i++)
		{
			str.Format("I(%d)=", m_disp_image_number[i]);

			message += str;

			CDataUnit *pdu = get_disp_image_data(i);
			if(pdu){
				str.Format("%lf     ", pdu->GetDataValue( (int)srcX, (int)srcY) );
			}else{
				str.Format("*  ");
			}
			message += str;
		}

		API.SetEditValue(m_hWnd, IDC_MULTI_IMAGE_DISP_OUTPUT, message);
	}

	CDialog::OnMouseMove(nFlags, point);
}


/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnMouseMove
��    ��    �l : 
�@          �\ : �\���摜�̊g��E�k��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CMultiImageDispDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	RECT Rct={0};
	CRect imageRect[6];
	int imageCnt=0;


	::GetWindowRect(m_hWnd, &Rct);//���̃E�B���h�E�̃N���C�A���g�̈��\����`���擾
	CPoint point = pt - CPoint(Rct.left, Rct.top);//�E�B���h�E��̍��W�ɕϊ�

	//�̈�v�Z
	imageCnt = calc_image_regn(imageRect);

	for(int i=0 ; i<imageCnt ; i++)
	{
		if( imageRect[i].PtInRect(point) )//�ǂ��̃E�B���h�E��Ƀ}�E�X�J�[�\�������邩�`�F�b�N
		{
			CDataUnit *pdu = get_disp_image_data(i);
			if(pdu)
			{
				//�X�P�[����ύX���ĕ`��
				float scale = pdu->DispScale;
				if(zDelta<0){
					scale*=0.9;//10%�k��
				}else{
					scale*=1.1;//10%�g��
				}

				p_disp_image_field[i]->ScaleImage(pdu, scale);
				p_disp_image_field[i]->DrawImage(pdu, true);//�摜�ĕ`��

				update_scrollbar(i);//�X�N���[���o�[�ĕ`��

				update_image_overlap(i);//�g�Ɖ摜�ԍ����ĕ`��

				synchro_disp_image(i);//���̉摜�̕\���ʒu�����̉摜�ɂ��킹��
			}
		}
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

/********************************************************************
�@  �\  ��  �� : �\���摜���ύX
��    ��    �� : OnCbnSelchangeComboMultiImageDispNum
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnCbnSelchangeComboMultiImageDispNum()
{
	arrange_dlg_item();//�ĕ`��
}


/********************************************************************
�@  �\  ��  �� : 2�{�g��
��    ��    �� : OnBnClickedButtonMultiImgDlgMagnify0
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnBnClickedButtonMultiImgDlgMagnify0()
{
	if( m_selected_disp_image_no >=0 )
	{
		CDataUnit *pdu = get_disp_image_data(m_selected_disp_image_no);
		if(pdu)
		{
			float scale = pdu->DispScale;

			p_disp_image_field[m_selected_disp_image_no]->ScaleImage(pdu, scale*2.0);
			p_disp_image_field[m_selected_disp_image_no]->DrawImage(pdu, false);//�摜�ĕ`��

			update_scrollbar(m_selected_disp_image_no);//�X�N���[���o�[�ĕ`��

			update_image_overlap(m_selected_disp_image_no);//�g�Ɖ摜�ԍ����ĕ`��

			synchro_disp_image(m_selected_disp_image_no);//���̉摜�̕\���ʒu�����̉摜�ɂ��킹��
		}
	}
}


/********************************************************************
�@  �\  ��  �� : 1/2�{�k��
��    ��    �� : OnBnClickedButtonMultiImgDlgMagnify1
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMultiImageDispDlg::OnBnClickedButtonMultiImgDlgMagnify1()
{
	if( m_selected_disp_image_no >=0 )
	{
		CDataUnit *pdu = get_disp_image_data(m_selected_disp_image_no);
		if(pdu)
		{
			float scale = pdu->DispScale;

			p_disp_image_field[m_selected_disp_image_no]->ScaleImage(pdu, scale/2.0);
			p_disp_image_field[m_selected_disp_image_no]->DrawImage(pdu, false);//�摜�ĕ`��

			update_scrollbar(m_selected_disp_image_no);//�X�N���[���o�[�ĕ`��

			update_image_overlap(m_selected_disp_image_no);//�g�Ɖ摜�ԍ����ĕ`��

			synchro_disp_image(m_selected_disp_image_no);//���̉摜�̕\���ʒu�����̉摜�ɂ��킹��
		}
	}
}


/********************************************************************
�@  �\  ��  �� :�L�[�{�[�h����̓���
��    ��    �� : PreTranslateMessage
��          �� : 
��    ��    �l : 
�@          �\ : ���^�[���F�ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CMultiImageDispDlg::PreTranslateMessage(MSG* pMsg) 
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

			default:
				break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}






