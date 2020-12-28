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
//CHistgramDlg
//�q�X�g�O�����E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "HistgramDlg.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define		COLOR_BAR_W		(256)//�J���[�o�[�̕�
#define		COLOR_BAR_H		(10)//�J���[�o�[�̍���
#define		DATA_FIELD_X	(0)//�q�X�g�O�����f�[�^�\���̈�̍����X���W
#define		DATA_FIELD_Y	(195)//�q�X�g�O�����f�[�^�\���̈�̍����Y���W
#define		DATA_FIELD_W	(256)//�q�X�g�O�����f�[�^�\���̈�̕�
#define		DATA_FIELD_H	(60)//�q�X�g�O�����f�[�^�\���̈�̍���

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

CHistgramDlg::CHistgramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistgramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistgramDlg)
	//}}AFX_DATA_INIT
}


void CHistgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistgramDlg)
	//}}AFX_DATA_MAP
}


//�_�C�A���O�A�C�e���̔z�u
void CHistgramDlg::show_dlg_item()
{
	//�{�^���Ɋ֘A�t����r�b�g�}�b�v�̃��[�h
	bmp_button_inc.LoadBitmap( IDB_BUTTON_INCLIMENT );
	bmp_button_dec.LoadBitmap( IDB_BUTTON_DECLIMENT );

	//�q�X�g�O�����̈�
	CWnd	*p_histgram_field = (CWnd*)GetDlgItem( IDC_HISTGRAM_FIELD  );
	p_histgram_field->MoveWindow( 0, 0, HISTGRAM_W , HISTGRAM_H );

	//�q�X�g�O�����f�[�^�̕\���̈�
	CWnd	*p_histgram_data_field = (CWnd*)GetDlgItem( IDC_HISTGRAM_DATA_FIELD  );
	p_histgram_data_field->MoveWindow( DATA_FIELD_X , DATA_FIELD_Y , DATA_FIELD_W , DATA_FIELD_H );

	//�ݒ�{�^��
	CWnd	*p_button_set = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET  );
	p_button_set->MoveWindow( HISTGRAM_W, HISTGRAM_H + COLOR_BAR_H - 25, 35, 25);

	//�x���\���̍ő�l��\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_histgram_max = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_MAX  );
	p_edit_histgram_max->MoveWindow( HISTGRAM_W, 0, 40, 15); 

	CWnd	*p_button_histgram_max_inc = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_MAX_INC  );//�l��1�����₷�{�^��
	p_button_histgram_max_inc->MoveWindow( HISTGRAM_W + 40, 0, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_HISTGRAM_MAX_INC ); 

	CWnd	*p_button_histgram_max_dec = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_MAX_DEC  );//�l��1�����炷�{�^��
	p_button_histgram_max_dec->MoveWindow( HISTGRAM_W + 40, 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_HISTGRAM_MAX_DEC ); 

	CWnd	*p_button_histgram_max_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_MAX_HLF  );//�l�𔼕��ɂ���{�^��
	p_button_histgram_max_hlf->MoveWindow( HISTGRAM_W, 15, 20, 15);
	
	CWnd	*p_button_histgram_max_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_MAX_DBL  );//�l��2�{�ɂ���{�^��
	p_button_histgram_max_dbl->MoveWindow( HISTGRAM_W + 20, 15, 20, 15);

	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , 3000);//�����l��ݒ�

	//�\�������W�̍ŏ��l��\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_range_min = (CWnd*)GetDlgItem( IDC_EIDT_HISTGRAM_RANGE_MIN  );
	p_edit_range_min->MoveWindow( 0 , HISTGRAM_H + COLOR_BAR_H , 40 , 15); 

	CWnd	*p_button_range_min_inc = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MIN_INC  );//�l��1�����₷�{�^��
	p_button_range_min_inc->MoveWindow( 40, HISTGRAM_H + COLOR_BAR_H, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_RANGE_MIN_INC ); 

	CWnd	*p_button_range_min_dec = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MIN_DEC  );//�l��1�����炷�{�^��
	p_button_range_min_dec->MoveWindow( 40, HISTGRAM_H + COLOR_BAR_H + 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_RANGE_MIN_DEC );

	CWnd	*p_button_range_min_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MIN_HLF  );//�l�𔼕��ɂ���{�^��
	p_button_range_min_hlf->MoveWindow( 0, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);
	
	CWnd	*p_button_range_min_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MIN_DBL  );//�l��2�{�ɂ���{�^��
	p_button_range_min_dbl->MoveWindow( 20, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);


	//�\�������W�̍ő�l��\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_range_max = (CWnd*)GetDlgItem( IDC_EIDT_HISTGRAM_RANGE_MAX  );
	p_edit_range_max->MoveWindow( HISTGRAM_W - 40 , HISTGRAM_H + COLOR_BAR_H , 40, 15); 

	CWnd	*p_button_range_max_inc = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MAX_INC  );//�l��1�����₷�{�^��
	p_button_range_max_inc->MoveWindow( HISTGRAM_W , HISTGRAM_H + COLOR_BAR_H, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_RANGE_MAX_INC ); 

	CWnd	*p_button_range_max_dec = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MAX_DEC  );//�l��1�����炷�{�^��
	p_button_range_max_dec->MoveWindow( HISTGRAM_W , HISTGRAM_H + COLOR_BAR_H + 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_RANGE_MAX_DEC );

	CWnd	*p_button_range_max_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MAX_HLF  );//�l�𔼕��ɂ���{�^��
	p_button_range_max_hlf->MoveWindow( HISTGRAM_W - 40, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);
	
	CWnd	*p_button_range_max_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MAX_DBL  );//�l��2�{�ɂ���{�^��
	p_button_range_max_dbl->MoveWindow( HISTGRAM_W - 20, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);


	//�\�������W�̒��Ԓl��\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_range_mid = (CWnd*)GetDlgItem( IDC_EIDT_HISTGRAM_RANGE_MID  );
	p_edit_range_mid->MoveWindow(80 , HISTGRAM_H + COLOR_BAR_H , 40, 15); 

	CWnd	*p_edit_static_range_mid = (CWnd*)GetDlgItem( IDC_HISTGRAM_DLG_STATIC_RANGE_MID  );//�G�f�B�b�g�{�b�N�X�̐����̕����istatic�j
	p_edit_static_range_mid->MoveWindow(60 , HISTGRAM_H + COLOR_BAR_H + 2, 20, 15); 

	CWnd	*p_button_range_mid_inc = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MID_INC  );//�l��1�����₷�{�^��
	p_button_range_mid_inc->MoveWindow( 120 , HISTGRAM_H + COLOR_BAR_H, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_RANGE_MID_INC ); 

	CWnd	*p_button_range_mid_dec = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MID_DEC  );//�l��1�����炷�{�^��
	p_button_range_mid_dec->MoveWindow( 120 , HISTGRAM_H + COLOR_BAR_H + 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_RANGE_MID_DEC );

	CWnd	*p_button_range_mid_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MID_HLF  );//�l�𔼕��ɂ���{�^��
	p_button_range_mid_hlf->MoveWindow( 80, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);
	
	CWnd	*p_button_range_mid_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_MID_DBL  );//�l��2�{�ɂ���{�^��
	p_button_range_mid_dbl->MoveWindow( 100, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);


	//�\�������W�̕���\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_range_width = (CWnd*)GetDlgItem( IDC_EIDT_HISTGRAM_RANGE_WIDTH  );
	p_edit_range_width->MoveWindow( 150 , HISTGRAM_H + COLOR_BAR_H , 40, 15); 

	CWnd	*p_edit_static_range_width = (CWnd*)GetDlgItem( IDC_HISTGRAM_DLG_STATIC_RANGE_WIDTH  );//�G�f�B�b�g�{�b�N�X�̐����̕����istatic�j
	p_edit_static_range_width->MoveWindow(140 , HISTGRAM_H + COLOR_BAR_H + 2, 10, 15); 

	CWnd	*p_button_range_width_inc = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_WIDTH_INC  );//�l��1�����₷�{�^��
	p_button_range_width_inc->MoveWindow( 190 , HISTGRAM_H + COLOR_BAR_H, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_inc, IDC_BUTTON_RANGE_WIDTH_INC ); 

	CWnd	*p_button_range_width_dec = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_WIDTH_DEC  );//�l��1�����炷�{�^��
	p_button_range_width_dec->MoveWindow( 190 , HISTGRAM_H + COLOR_BAR_H + 12, 12, 12);
	set_bitmap_button( (HBITMAP)bmp_button_dec, IDC_BUTTON_RANGE_WIDTH_DEC );

	CWnd	*p_button_range_width_hlf = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_WIDTH_HLF  );//�l�𔼕��ɂ���{�^��
	p_button_range_width_hlf->MoveWindow( 150, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);
	
	CWnd	*p_button_range_width_dbl = (CWnd*)GetDlgItem( IDC_BUTTON_RANGE_WIDTH_DBL  );//�l��2�{�ɂ���{�^��
	p_button_range_width_dbl->MoveWindow( 170, HISTGRAM_H + COLOR_BAR_H + 15, 20, 15);


	//���ϒl��\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_average = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_AVERAGE  );
	p_edit_average->MoveWindow( DATA_FIELD_X + 35 , DATA_FIELD_Y + 5 , 50, 15); 

	CWnd	*p_edit_static_average = (CWnd*)GetDlgItem( IDC_HISTGRAM_DLG_STATIC_AVERAGE  );//�G�f�B�b�g�{�b�N�X�̐����̕����istatic�j
	p_edit_static_average->MoveWindow( DATA_FIELD_X + 5 , DATA_FIELD_Y + 5, 30, 15); 


	//�W���΍���\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_stdiv = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_STDIV  );
	p_edit_stdiv->MoveWindow( DATA_FIELD_X + 160 , DATA_FIELD_Y + 5 , 50, 15 ); 

	CWnd	*p_edit_static_stdiv = (CWnd*)GetDlgItem( IDC_HISTGRAM_DLG_STATIC_STDIV  );//�G�f�B�b�g�{�b�N�X�̐����̕����istatic�j
	p_edit_static_stdiv->MoveWindow( DATA_FIELD_X + 110 , DATA_FIELD_Y + 5 , 50 , 15 ); 

	//�}�E�X�Ń|�C���g���ꂽ�Z�x��\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_pointed_brightness = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_POINTED_BRIGHTNESS  );
	p_edit_pointed_brightness->MoveWindow( DATA_FIELD_X + 5 , DATA_FIELD_Y + 25 , 100, 15 ); 

	//�}�E�X�Ń|�C���g���ꂽ�Z�x�̓x����\���G�f�B�b�g�{�b�N�X
	CWnd	*p_edit_pointed_hist = (CWnd*)GetDlgItem( IDC_EDIT_HISTGRAM_POINTED_HIST  );
	p_edit_pointed_hist->MoveWindow( DATA_FIELD_X + 5 , DATA_FIELD_Y + 42 , 100, 15 ); 

	//�\���Z�x�����W�𒲐߂���{�^��
	CWnd	*p_button_min_max = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET_MIN_MAX  );//min-max
	p_button_min_max->MoveWindow( DATA_FIELD_W + 2 , DATA_FIELD_Y , 55, 15 ); 

	CWnd	*p_button_ave = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET_AVE  );
	p_button_ave->MoveWindow( DATA_FIELD_W + 2 , DATA_FIELD_Y + 15 , 55, 15 ); //ave�}sdv

	CWnd	*p_button_0_256 = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET_0_256  );
	p_button_0_256->MoveWindow( DATA_FIELD_W + 2 , DATA_FIELD_Y + 15*2 , 55, 15 ); //0-256

	CWnd	*p_button_pi = (CWnd*)GetDlgItem( IDC_BUTTON_HISTGRAM_SET_PI_PI  );
	p_button_pi->MoveWindow( DATA_FIELD_W + 2 , DATA_FIELD_Y + 15*3 , 55, 15 ); //-pi - pi
}


//id�Ŏ������{�^���Ƀr�b�g�}�b�v���֘A�Â���
HBITMAP CHistgramDlg::set_bitmap_button(HBITMAP h_bmp, UINT id)
{
	CButton	*p_button = (CButton*)GetDlgItem(id);
	return	p_button->SetBitmap( h_bmp );
}


//�J���[�o�[��\���r�b�g�}�b�v�̍쐬
void CHistgramDlg::create_color_bar()
{
	p_color_bar_data = new BYTE[ COLOR_BAR_W * COLOR_BAR_H ];//�J���[�o�[�̔Z�x�f�[�^�̍쐬
		for(int j=0   ;   j< COLOR_BAR_H   ;   j++)
			for(int i=0   ;   i< COLOR_BAR_W   ;   i++)
				*( p_color_bar_data + i + j * COLOR_BAR_W ) = (BYTE)i;

	p_color_bar_data_info = (BITMAPINFO*)new char[ sizeof(BITMAPINFOHEADER) + 1024];//BITMAPINFOHEADER�ƃJ���[�p���b�g�̃��������擾����
																		
	p_color_bar_data_info->bmiHeader.biSize = 40;//�w�b�_�̃T�C�Y
	p_color_bar_data_info->bmiHeader.biWidth = COLOR_BAR_W;//�J���[�o�[�̕�
	p_color_bar_data_info->bmiHeader.biHeight = COLOR_BAR_H;//�J���[�o�[�̍���
	p_color_bar_data_info->bmiHeader.biPlanes = 1;
	p_color_bar_data_info->bmiHeader.biBitCount = 8;//�J���[�o�[��1��f������̃T�C�Y
	p_color_bar_data_info->bmiHeader.biCompression = 0;//�J���[�o�[�̈��k�`��
	p_color_bar_data_info->bmiHeader.biSizeImage   = 0;
	p_color_bar_data_info->bmiHeader.biXPelsPerMeter = 0;
	p_color_bar_data_info->bmiHeader.biYPelsPerMeter = 0;
	p_color_bar_data_info->bmiHeader.biClrUsed = 0;
	p_color_bar_data_info->bmiHeader.biClrImportant = 0;

	ZeroMemory( p_color_bar_data_info->bmiColors , 1024);//�J���[�p���b�g���[���N���A�i�S�č��j
}


//�q�X�g�O������`�悷��
//do_show_values �� true �ɂ���ƃq�X�g�O�����̐ݒ�l���ĕ\������
void CHistgramDlg::DrawHistgram()
{
	int	i;
	float	disp_hist, histgram_max;
	CDC		*p_dc;
	CDataUnit	*p_du;


	p_du = API.GetDataUnit( CURRENT_IMAGE );//���ݑI�𒆂̃f�[�^���擾����
		if(p_du == NULL)	return;//�f�[�^��������΂Ȃɂ����Ȃ�

	p_dc = GetDC();//�f�o�C�X�R���e�L�X�g���擾

		if( !API.GetEditValue(m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;//�q�X�g�O�����ő�l���擾����

	p_dc->Rectangle( 0 , 0 , HISTGRAM_W , HISTGRAM_H );//�w�i�̋�`��`�悷��

		for(i=0 ; i<256 ; i++){
			disp_hist = HISTGRAM_H / histgram_max * (float)p_du->BrightLevel[i];//�x����\������Ƃ��̍���

			//�x����\������i�c���C���j
			p_dc->MoveTo(i, HISTGRAM_H);
			p_dc->LineTo(i, HISTGRAM_H - (long)disp_hist);
		}
	
	ReleaseDC(p_dc);//�f�o�C�X�R���e�L�X�g���J��

	draw_color_bar();//�J���[�o�[��`�悷��
	
	//�q�X�g�O�����̐ݒ�l���ĕ\������
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_AVERAGE , p_du->Average);
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_STDIV , p_du->Stdiv);
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , p_du->BrightMin);
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , p_du->BrightMax);
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , ( p_du->BrightMax   +   p_du->BrightMin ) / 2 );
	API.SetEditValue( m_hWnd,  IDC_EIDT_HISTGRAM_RANGE_WIDTH , ( p_du->BrightMax   -   p_du->BrightMin ) / 2 );

}


//�J���[�o�[��`�悷��
void CHistgramDlg::draw_color_bar()
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );//�I�𒆂̉摜�f�[�^���擾����

		//�J���[�p���b�g�̐ݒ�
		if(p_du != NULL)
			memcpy( p_color_bar_data_info->bmiColors , p_du->pDispBufferBmpInfo->bmiColors, 1024);//�I�𒆂̃f�[�^�̃J���[�p���b�g���R�s�[

		else
			ZeroMemory( p_color_bar_data_info->bmiColors , 1024);//�J���[�p���b�g���[���N���A�i�S�č��j

	HDC	hdc = ::GetDC( m_hWnd );//�f�o�C�X�R���e�L�X�g���擾����
	::StretchDIBits( hdc,	0 , HISTGRAM_H	 , COLOR_BAR_W , COLOR_BAR_H ,//�J���[�o�[�̃r�b�g�}�b�v�iDIB�j��\��t����
							0 ,	0			 , COLOR_BAR_W , COLOR_BAR_H ,
							p_color_bar_data ,  p_color_bar_data_info ,  DIB_RGB_COLORS , SRCCOPY);
	::ReleaseDC(m_hWnd, hdc);	//�f�o�C�X�R���e�L�X�g���J������
}

//�����l�̃����W������\�������W�̍ő�ƍŏ������߂�
bool CHistgramDlg::set_min_max_from_mid_width(float mid, float width, float *min, float *max)
{
	float temp_max, temp_min;
	
	temp_max = mid + width;
	temp_min = mid - width;

		if(temp_min >= temp_max)	return	false;//�召�֌W���t�ɂȂ����炵���ς�

	*max = temp_max;
	*min = temp_min;
	return	true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHistgramDlg, CDialog)
	//{{AFX_MSG_MAP(CHistgramDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_MAX_INC, OnButtonHistgramMaxInc)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_MAX_DEC, OnButtonHistgramMaxDec)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_MAX_HLF, OnButtonHistgramMaxHlf)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_MAX_DBL, OnButtonHistgramMaxDbl)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MIN_INC, OnButtonRangeMinInc)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MIN_DEC, OnButtonRangeMinDec)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MIN_HLF, OnButtonRangeMinHlf)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MIN_DBL, OnButtonRangeMinDbl)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MID_INC, OnButtonRangeMidInc)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MID_DEC, OnButtonRangeMidDec)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MID_HLF, OnButtonRangeMidHlf)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MID_DBL, OnButtonRangeMidDbl)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_WIDTH_INC, OnButtonRangeWidthInc)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_WIDTH_DEC, OnButtonRangeWidthDec)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_WIDTH_HLF, OnButtonRangeWidthHlf)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_WIDTH_DBL, OnButtonRangeWidthDbl)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MAX_INC, OnButtonRangeMaxInc)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MAX_DEC, OnButtonRangeMaxDec)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MAX_HLF, OnButtonRangeMaxHlf)
	ON_BN_CLICKED(IDC_BUTTON_RANGE_MAX_DBL, OnButtonRangeMaxDbl)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET, OnHistgramSet)
	ON_EN_CHANGE(IDC_EIDT_HISTGRAM_RANGE_MIN, OnChangeHistgramRangeMin)
	ON_EN_CHANGE(IDC_EIDT_HISTGRAM_RANGE_MAX, OnChangeHistgramRangeMax)
	ON_EN_CHANGE(IDC_EIDT_HISTGRAM_RANGE_MID, OnChangeHistgramRangeMid)
	ON_EN_CHANGE(IDC_EIDT_HISTGRAM_RANGE_WIDTH, OnChangeHistgramRangeWidth)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET_MIN_MAX, OnHistgramSetMinMax)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET_AVE, OnHistgramSetAve)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET_0_256, OnHistgramSet0256)
	ON_BN_CLICKED(IDC_BUTTON_HISTGRAM_SET_PI_PI, OnHistgramSetPiPi)
	//}}AFX_MSG_MAP
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
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
BOOL CHistgramDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	create_color_bar();//�J���[�o�[�̍쐬
	show_dlg_item();//�_�C�A���O�A�C�e���̔z�u

	MoveWindow(API.main_window_size.cx, 0, HISTGRAM_WIN_W,  HISTGRAM_WIN_H);//�E�C���h�E�̑傫���ݒ�
	flg_str_from_key = false;
	is_draging_r=false;

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/********************************************************************
�@  �\  ��  �� : ���̃E�B���h�E��\����Ԃɂ���
��    ��    �� : OnShowWindow
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


}

/********************************************************************
�@  �\  ��  �� :�L�[�{�[�h����̓���
��    ��    �� : PreTranslateMessage
��          �� : 
��    ��    �l : 
�@          �\ : ���^�[���F�q�X�g�O�����ݒ�ύX�̔��f
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CHistgramDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN ){
		CPoint	point;

		switch(pMsg->wParam){
			case VK_RETURN://Enter
				OnHistgramSet() ;
				return  true;

			case VK_ESCAPE://Esc
				return  true;

			case VK_UP://��
				return  true;

			case VK_DOWN://��
				return  true;

			case VK_LEFT://��
				return  true;

			case VK_RIGHT://��
				return true;

			case VK_DELETE://Del
				break;

			default:
				flg_str_from_key = true;
				break;
		}
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
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
void CHistgramDlg::OnDestroy() 
{
	CDialog::OnDestroy();
		
	delete[]	p_color_bar_data_info;
	delete[]	p_color_bar_data;
}

/********************************************************************
�@  �\  ��  �� :�ĕ`��
��    ��    �� : OnPaint
��          �� : 
��    ��    �l : 
�@          �\ : �q�X�g�O�������ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	DrawHistgram();
	// �`��p���b�Z�[�W�Ƃ��� CDialog::OnPaint() ���Ăяo���Ă͂����܂���
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�J�[�\�����ʉ߂���
��    ��    �� : OnMouseMove
��          �� : 
��    ��    �l : 
�@          �\ : �ʉ߂����ʒu�̃q�X�g�O�����̗v�f��\������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	static int oldpoint_x = -1;//�O��}�E�X�Ń|�C���g�����_��x���W
	CDC		*p_dc;
	float	max, min, coef;
	CRect	histgram_field( 0,0, HISTGRAM_W, HISTGRAM_H );//�q�X�g�O������`���̈��\����`
	CString	bri, hist;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );

		if( p_du == NULL )		return;//���ݑI�𒆂̉摜�������Ƀf�[�^��������Ή������Ȃ�

	p_dc = GetDC();//�f�o�C�X�R���e�L�X�g�̎擾
	p_dc->SetROP2(R2_NOT);//�F���]������y��
		//if( oldpoint_x != -1 ){//�O��ɕ`������������ꍇ�́C�h��Ȃ���������
		//	p_dc->MoveTo( oldpoint_x , HISTGRAM_H -1 );
		//	p_dc->LineTo( oldpoint_x , 0 );	
		//}
	DrawHistgram();//�q�X�g�O�����`��

		if( histgram_field.PtInRect(point) )
		{//���݂̃}�E�X�|�C���^���q�X�g�O������ɂ���ꍇ�̏���

			

			//�q�X�g�O������Ń}�E�X�Ń|�C���g�����Z�x�̓x����\�����𔽓]�y���ŕ`��
			p_dc->MoveTo( point.x , HISTGRAM_H -1 );
			p_dc->LineTo( point.x , 0 );
			oldpoint_x = point.x;

			//�q�X�g�O������Ń}�E�X�Ń|�C���g�����Z�x�̕ϊ��O�̒l�Ƃ��̓x����\������
			coef = (float)((p_du->BrightMax - p_du->BrightMin)/256.0);
			min = point.x * coef  +  p_du->BrightMin;
			max = (point.x + 1) * coef  +  p_du->BrightMin;
	
				if(point.x == 255)
					bri = API.ToString( min ) + _T("�`");
				else if(point.x == 0)	
					bri = _T("�`") + API.ToString( max );
				else
					bri = API.ToString( min ) + _T("�`") + API.ToString( max );

			hist.Format("�x���F%d" , p_du->BrightLevel[ point.x ] );


			if(is_draging_r)
			{//�E�h���b�O��
				int sx=0,ex=0;
				if(hist_value_drag_start > point.x){
					sx = point.x+1;	ex = hist_value_drag_start;
				}else if(hist_value_drag_start < point.x){
					ex = point.x-1;	sx = hist_value_drag_start;
				}

				for(int x=sx ; x<=ex ; x++){//�h���b�O�͈͂𔽓]
					p_dc->MoveTo( x , HISTGRAM_H -1 );
					p_dc->LineTo( x , 0 );
				}
			}


		}else{
			bri = _T("");
			hist = _T("");
			oldpoint_x = -1;
			is_draging_r=false;

		}
	API.SetEditValue( m_hWnd , IDC_EDIT_HISTGRAM_POINTED_BRIGHTNESS , bri );
	API.SetEditValue( m_hWnd , IDC_EDIT_HISTGRAM_POINTED_HIST , hist );

	ReleaseDC(p_dc);//�f�o�C�X�R���e�L�X�g�̊J��
	CDialog::OnMouseMove(nFlags, point);
}

/********************************************************************
�@  �\  ��  �� :�x���̍ő�l�̒l�ύX�{�^������
��    ��    �� : OnButtonHistgramMaxInc
��          �� : 
��    ��    �l : 
�@          �\ : 1����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonHistgramMaxInc() 
{
	float	histgram_max;
		if( !API.GetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;
		if( histgram_max >= FLT_MAX )		return;
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , histgram_max + 1);
	DrawHistgram();
}

/********************************************************************
�@  �\  ��  �� :�x���̍ő�l�̒l�ύX�{�^������
��    ��    �� : OnButtonHistgramMaxDec
��          �� : 
��    ��    �l : 
�@          �\ : 1����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonHistgramMaxDec() 
{
	float	histgram_max;
		if( !API.GetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;
		if( histgram_max <= 1 )		return;
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , histgram_max + 1);
	DrawHistgram();
}

/********************************************************************
�@  �\  ��  �� :�x���̍ő�l�̒l�ύX�{�^������
��    ��    �� : OnButtonHistgramMaxDec
��          �� : 
��    ��    �l : 
�@          �\ : �l�𔼕��ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonHistgramMaxHlf() 
{
	float	histgram_max;
		if( !API.GetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;
		if( histgram_max <= 2 )		return;
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , histgram_max / 2);
	DrawHistgram();
}

/********************************************************************
�@  �\  ��  �� :�x���̍ő�l�̒l�ύX�{�^������
��    ��    �� : OnButtonHistgramMaxDec
��          �� : 
��    ��    �l : 
�@          �\ : �l���Q�{�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonHistgramMaxDbl() 
{
	float	histgram_max;
		if( !API.GetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , &histgram_max ))		return;
		if( histgram_max >= FLT_MAX )		return;
	API.SetEditValue( m_hWnd, IDC_EDIT_HISTGRAM_MAX , histgram_max * 2);
	DrawHistgram();
}

/********************************************************************
�@  �\  ��  �� :�\���ŏ��l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMinInc
��          �� : 
��    ��    �l : 
�@          �\ : �P����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMinInc() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min + 1, range_max );
}

/********************************************************************
�@  �\  ��  �� :�\���ŏ��l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMinDec
��          �� : 
��    ��    �l : 
�@          �\ : �P����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMinDec() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min - 1, range_max );
}

/********************************************************************
�@  �\  ��  �� :�\���ŏ��l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMinHlf
��          �� : 
��    ��    �l : 
�@          �\ : �l�𔼕��ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMinHlf() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min / 2 , range_max );
}

/********************************************************************
�@  �\  ��  �� :�\���ŏ��l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMinDbl
��          �� : 
��    ��    �l : 
�@          �\ : �l���Q�{�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMinDbl() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min * 2 , range_max );
}

/********************************************************************
�@  �\  ��  �� :�\���ő�l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMaxInc
��          �� : 
��    ��    �l : 
�@          �\ : �P����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMaxInc() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max + 1 );	
}

/********************************************************************
�@  �\  ��  �� :�\���ő�l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMaxDec
��          �� : 
��    ��    �l : 
�@          �\ : �P����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMaxDec() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max - 1 );		
}

/********************************************************************
�@  �\  ��  �� :�\���ő�l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMaxHlf
��          �� : 
��    ��    �l : 
�@          �\ : �l�𔼕��ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMaxHlf() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max / 2 );		
}

/********************************************************************
�@  �\  ��  �� :�\���ő�l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMaxDbl
��          �� : 
��    ��    �l : 
�@          �\ : �l���Q�{�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMaxDbl() 
{
	float	range_min, range_max;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ))		return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max * 2 );		
}

/********************************************************************
�@  �\  ��  �� :�\�������l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMidInc
��          �� : 
��    ��    �l : 
�@          �\ : �P����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMidInc() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid + 1, range_width, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );	
}

/********************************************************************
�@  �\  ��  �� :�\�������l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMidDec
��          �� : 
��    ��    �l : 
�@          �\ : �P����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMidDec() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid - 1, range_width, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
�@  �\  ��  �� :�\�������l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMidHlf
��          �� : 
��    ��    �l : 
�@          �\ : �l�𔼕��ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMidHlf() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid / 2, range_width, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );			
}

/********************************************************************
�@  �\  ��  �� :�\�������l�̒l�ύX�{�^������
��    ��    �� : OnButtonRangeMidDbl
��          �� : 
��    ��    �l : 
�@          �\ : �l���Q�{�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeMidDbl() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid * 2, range_width, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
�@  �\  ��  �� :�\�����̒l�ύX�{�^������
��    ��    �� : OnButtonRangeWidthInc
��          �� : 
��    ��    �l : 
�@          �\ : �P����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeWidthInc() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid, range_width + 1, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
�@  �\  ��  �� :�\�����̒l�ύX�{�^������
��    ��    �� : OnButtonRangeWidthDec
��          �� : 
��    ��    �l : 
�@          �\ : �P����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeWidthDec() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid, range_width - 1, &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );			
}

/********************************************************************
�@  �\  ��  �� :�\�����̒l�ύX�{�^������
��    ��    �� : OnButtonRangeWidthHlf
��          �� : 
��    ��    �l : 
�@          �\ : �l�𔼕��ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeWidthHlf() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid, range_width / 2 , &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
�@  �\  ��  �� :�\�����̒l�ύX�{�^������
��    ��    �� : OnButtonRangeWidthDbl
��          �� : 
��    ��    �l : 
�@          �\ : �l���Q�{�ɂ���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnButtonRangeWidthDbl() 
{
	float	range_max, range_min, range_mid, range_width;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID , &range_mid ))		return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ))		return;	
		if(!set_min_max_from_mid_width( range_mid, range_width * 2 , &range_min, &range_max) )	return;
	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );		
}

/********************************************************************
�@  �\  ��  �� :�uSET�v�{�^���������ꂽ
��    ��    �� : OnHistgramSet
��          �� : 
��    ��    �l : 
�@          �\ : �\�������W�𔽉f���ĕ\���摜�f�[�^���X�V���q�X�g�O�����ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnHistgramSet() 
{
		float	range_min, range_max;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ) )	return;

	API.SetDispRange( CURRENT_IMAGE, range_min, range_max );
}

/********************************************************************
�@  �\  ��  �� :�x���ő�l��\���G�f�B�b�g�{�b�N�X�̒l�ύX
��    ��    �� : OnChangeHistgramRangeMin
��          �� : 
��    ��    �l : 
�@          �\ : �G�f�B�b�g�{�b�N�X�̒��ڕҏW
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnChangeHistgramRangeMin() 
{
	float	range_max, range_min, range_mid, range_width;

		if( !flg_str_from_key )		return;//�L�[�{�[�h����̕������͈ȊO�ɂ͉������Ȃ�
	flg_str_from_key = false;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ) )	return;

		if( range_max < range_min)		return;

	range_mid   = (float)( (range_max + range_min)/2.0 );
	range_width = (float)( (range_max - range_min)/2.0 );

	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID   , range_mid );
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH   , range_width );
}

/********************************************************************
�@  �\  ��  �� :�\���ő�l��\���G�f�B�b�g�{�b�N�X�̒l�ύX
��    ��    �� : OnChangeHistgramRangeMax
��          �� : 
��    ��    �l : 
�@          �\ : �G�f�B�b�g�{�b�N�X�̒��ڕҏW
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnChangeHistgramRangeMax() 
{
	float	range_max, range_min, range_mid, range_width;

		if( !flg_str_from_key )		return;//�L�[�{�[�h����̕������͈ȊO�ɂ͉������Ȃ�
	flg_str_from_key = false;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX , &range_max ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN , &range_min ) )	return;

		if( range_max < range_min)		return;

	range_mid   = (float)( (range_max + range_min)/2.0 );
	range_width = (float)( (range_max - range_min)/2.0 );

	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID   , range_mid );
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH   , range_width );	
}

/********************************************************************
�@  �\  ��  �� :�\�������l��\���G�f�B�b�g�{�b�N�X�̒l�ύX
��    ��    �� : OnChangeHistgramRangeMid
��          �� : 
��    ��    �l : 
�@          �\ : �G�f�B�b�g�{�b�N�X�̒��ڕҏW
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnChangeHistgramRangeMid() 
{
	float	range_max, range_min, range_mid, range_width;

		if( !flg_str_from_key )		return;//�L�[�{�[�h����̕������͈ȊO�ɂ͉������Ȃ�
	flg_str_from_key = false;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID   , &range_mid ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ) )	return;
		if(!set_min_max_from_mid_width( range_mid, range_width , &range_min, &range_max) )	return;

	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX   , range_max );
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN   , range_min );
}

/********************************************************************
�@  �\  ��  �� :�\������\���G�f�B�b�g�{�b�N�X�̒l�ύX
��    ��    �� : OnChangeHistgramRangeWidth
��          �� : 
��    ��    �l : 
�@          �\ : �G�f�B�b�g�{�b�N�X�̒��ڕҏW
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnChangeHistgramRangeWidth() 
{
	float	range_max, range_min, range_mid, range_width;

		if( !flg_str_from_key )		return;//�L�[�{�[�h����̕������͈ȊO�ɂ͉������Ȃ�
	flg_str_from_key = false;

		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MID   , &range_mid ) )	return;
		if( !API.GetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_WIDTH , &range_width ) )	return;
		if( !set_min_max_from_mid_width( range_mid, range_width , &range_min, &range_max) )	return;

	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MAX   , range_max );
	API.SetEditValue( m_hWnd, IDC_EIDT_HISTGRAM_RANGE_MIN   , range_min );
}

/********************************************************************
�@  �\  ��  �� :�uMax-Min�v�{�^������
��    ��    �� : OnHistgramSetMinMax
��          �� : 
��    ��    �l : 
�@          �\ : �\���͈͂��u�ő�@�|�@�ŏ��v�ɐݒ肵�ĕ\���摜�f�[�^���X�V���q�X�g�O�����ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnHistgramSetMinMax() 
{
	float min, max;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;

	p_du->GetDataMinMax(&min, &max);
	API.SetDispRange( CURRENT_IMAGE, min, max);
}

/********************************************************************
�@  �\  ��  �� :�uave�}sdv2�v�{�^������
��    ��    �� : OnHistgramSetMinMax
��          �� : 
��    ��    �l : 
�@          �\ : �\���͈͂��u���ϒl�@�}�@�W���΍��~2�v�ɐݒ肵�ĕ\���摜�f�[�^���X�V���q�X�g�O�����ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnHistgramSetAve() 
{
	float min, max;
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;

		if(!set_min_max_from_mid_width( p_du->Average , p_du->Stdiv * 2 , &min, &max) )	return;
	API.SetDispRange( CURRENT_IMAGE, min, max);	
}

/********************************************************************
�@  �\  ��  �� :�u0�`256�v�{�^������
��    ��    �� : OnHistgramSetMinMax
��          �� : 
��    ��    �l : 
�@          �\ : �\���͈͂��u0�`256�v�ɐݒ肵�ĕ\���摜�f�[�^���X�V���q�X�g�O�����ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnHistgramSet0256() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;

	API.SetDispRange( CURRENT_IMAGE, 0, 256);	
}

/********************************************************************
�@  �\  ��  �� :�u-�΁`�΁v�{�^������
��    ��    �� : OnHistgramSetMinMax
��          �� : 
��    ��    �l : 
�@          �\ : �\���͈͂��u-�΁`�΁v�ɐݒ肵�ĕ\���摜�f�[�^���X�V���q�X�g�O�����ĕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnHistgramSetPiPi() 
{
	CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du == NULL)	return;

	API.SetDispRange( CURRENT_IMAGE, -(float)PI, (float)PI);	
}


/********************************************************************
�@  �\  ��  �� : �}�E�X�E�{�^������
��    ��    �� : OnHistgramSetMinMax
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect	histgram_field( 0,0, HISTGRAM_W, HISTGRAM_H );//�q�X�g�O������`���̈��\����`

	if( histgram_field.PtInRect(point) )
	{
		is_draging_r=true;//�E�h���b�O���̃t���O�𗧂Ă�

		hist_value_drag_start = point.x;//�E�h���b�O�J�n���W
	}

	CDialog::OnRButtonDown(nFlags, point);
}


/********************************************************************
�@  �\  ��  �� : �}�E�X�E�{�^�������グ
��    ��    �� : OnRButtonUp
��          �� : 
��    ��    �l : 
�@          �\ : �h���b�O�I���B�\���͈͂��h���b�O��Ԃɂ��킹��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CHistgramDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	CRect	histgram_field( 0,0, HISTGRAM_W, HISTGRAM_H );//�q�X�g�O������`���̈��\����`

	if( histgram_field.PtInRect(point) && is_draging_r==true)
	{
		CDataUnit	*p_du = API.GetDataUnit( CURRENT_IMAGE );
		if(p_du)
		{
			int sx=0, ex=0;
			if(hist_value_drag_start > point.x){
				sx = point.x+1;	ex = hist_value_drag_start;
			}else if(hist_value_drag_start < point.x){
				ex = point.x-1;	sx = hist_value_drag_start;
			}

			if(sx<ex)
			{
				float coef = (float)((p_du->BrightMax - p_du->BrightMin)/256.0);
				float new_min = sx * coef;
				float new_max = ex * coef;

				CString str;
				str.Format("%f�`%f�̔Z�x�͈͂ŕ\�����܂�", new_min, new_max);
				if( AfxMessageBox( str , MB_OKCANCEL | MB_ICONINFORMATION ) == IDOK ){
					API.SetDispRange( CURRENT_IMAGE, new_min, new_max);
				}

				
			}
		}

	}

	is_draging_r=false;//�E�h���b�O���̃t���O��Q����
	DrawHistgram();//�q�X�g�O�����ĕ`��

	CDialog::OnRButtonUp(nFlags, point);
}
