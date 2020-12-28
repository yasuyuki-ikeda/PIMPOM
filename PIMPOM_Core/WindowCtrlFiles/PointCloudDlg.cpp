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
//CPointCloudDlg
//�O�����_�Q�\���E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
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

extern CPimpomAPI	API;

// *************************************
//         ��    ��    �Q    ��         
// *************************************


//////////////////////////////////////////////////////////////////////////////////////////
//  �����o�֐�
//////////////////////////////////////////////////////////////////////////////////////////

CPointCloudDlg::CPointCloudDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointCloudDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointCloudDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CPointCloudDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointCloudDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPointCloudDlg, CDialog)
	//{{AFX_MSG_MAP(CPointCloudDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_POINT_CLOUD_CTRL_ROTATE, OnPointCloudCtrlRotate)
	ON_BN_CLICKED(IDC_CHECK_POINT_CLOUD_CTRL_SCALE, OnPointCloudCtrlScale)
	ON_BN_CLICKED(IDC_CHECK_POINT_CLOUD_CTRL_TRANSLATE, OnPointCloudCtrlTranslate)
	//}}AFX_MSG_MAP
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
BOOL CPointCloudDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	MoveWindow( 0,0, POINT_CLOUD_W + 5, POINT_CLOUD_H + 30 );
	sampling_size = CSize(1,1);
	Rendering( sampling_size );
	is_druging = false;
	is_draging_mbutton = false;
	API.SetCheck( m_hWnd , IDC_CHECK_POINT_CLOUD_CTRL_ROTATE , true );

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/********************************************************************
�@  �\  ��  �� : �X�N���[���p�r�b�g�}�b�v�̏�����
��    ��    �� : init_bmp
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::init_bmp()
{
	p_screen_bmp_info = (LPBITMAPINFO)new char[ sizeof(BITMAPINFOHEADER) + 1024 ];

    p_screen_bmp_info->bmiHeader.biSize = 40;
    p_screen_bmp_info->bmiHeader.biWidth = POINT_CLOUD_W;
    p_screen_bmp_info->bmiHeader.biHeight = -POINT_CLOUD_H;
    p_screen_bmp_info->bmiHeader.biPlanes = 1;
    p_screen_bmp_info->bmiHeader.biBitCount = 8;
    p_screen_bmp_info->bmiHeader.biCompression = 0;
    p_screen_bmp_info->bmiHeader.biSizeImage   = 0;
    p_screen_bmp_info->bmiHeader.biXPelsPerMeter = 0;
    p_screen_bmp_info->bmiHeader.biYPelsPerMeter = 0;
    p_screen_bmp_info->bmiHeader.biClrUsed = 0;
    p_screen_bmp_info->bmiHeader.biClrImportant = 0;

	p_screen_bmp_data = new unsigned char[ POINT_CLOUD_W*POINT_CLOUD_H ];
	ZeroMemory( p_screen_bmp_data , POINT_CLOUD_W*POINT_CLOUD_H );
}

/********************************************************************
�@  �\  ��  �� : �E�C���h�E�̔j��
��    ��    �� : OnDestroy
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	CDialog::OnDestroy();

	delete[]	p_transformed_x;
	delete[]	p_transformed_y;
	delete[]	p_transformed_z;
	delete[]	p_original_z;
	delete[]	pZbuffer;
	delete[]	p_screen_bmp_data;
	delete[]	p_screen_bmp_info;
	delete[]	p_mask;
	delete[]	p_pixel_flg;
	delete[]	p_brightness;	
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
void CPointCloudDlg::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	draw_screen();
}

/********************************************************************
�@  �\  ��  �� : �X�N���[���p�r�b�g�}�b�v��`��
��    ��    �� : draw_screen
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::draw_screen()
{
	HDC hdc = ::GetDC(m_hWnd);
	::StretchDIBits(hdc, 0, 25, POINT_CLOUD_W, POINT_CLOUD_H,
						0, 0, POINT_CLOUD_W, POINT_CLOUD_H, 
						p_screen_bmp_data, p_screen_bmp_info, DIB_RGB_COLORS, SRCCOPY);
	::ReleaseDC(m_hWnd, hdc);	

	draw_scale();
}

/********************************************************************
�@  �\  ��  �� : �_�Q�f�[�^�쐬
��    ��    �� : CreatePointCloud
��          �� : CDataUnit *p_du	(in)���ƂȂ�XYZ�^�f�[�^
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CPointCloudDlg::CreatePointCloud(CDataUnit *p_du)
{
		if(p_du == NULL)	return	IDCANCEL;
		if(p_du->DataType != XYZ_FORMAT)	return	IDCANCEL;

	data_size = p_du->DataSize;
	bmp_data_width = p_du->DispBufferRow;
	p_transformed_x = new float[ data_size.cx*data_size.cy ];
	p_transformed_y = new float[ data_size.cx*data_size.cy ];
	p_transformed_z = new float[ data_size.cx*data_size.cy ];
	p_original_z = new float[ data_size.cx*data_size.cy ];
	pZbuffer = new float[ POINT_CLOUD_W*POINT_CLOUD_H ];
	p_pixel_flg = new unsigned char[ POINT_CLOUD_W*POINT_CLOUD_H ];
	p_mask = new unsigned char[ data_size.cx*data_size.cy ];
	p_brightness = new unsigned char[ bmp_data_width*data_size.cy ];

	init_bmp();
	memcpy( p_screen_bmp_info->bmiColors , p_du->pDispBufferBmpInfo->bmiColors , sizeof(RGBQUAD)*256 );
	memcpy( p_brightness , p_du->pDispBuffer , bmp_data_width*data_size.cy );

	scale = (float)0.7;
	translate = CPoint(0,0);

	memcpy( p_transformed_x, p_du->pFloatData , data_size.cx*data_size.cy*sizeof(float) ); 
	memcpy( p_transformed_y, p_du->pFloatData + data_size.cx*data_size.cy	,	data_size.cx*data_size.cy*sizeof(float) ); 
	memcpy( p_transformed_z, p_du->pFloatData + data_size.cx*data_size.cy*2,	data_size.cx*data_size.cy*sizeof(float) ); 
	memcpy( p_original_z , p_transformed_z, data_size.cx*data_size.cy*sizeof(float) );
	memcpy( p_mask , p_du->pMaskData , data_size.cx*data_size.cy );

	return	DoModal();
}

/********************************************************************
�@  �\  ��  �� : ���s�����@�œ_�Q�������_�����O
��    ��    �� : Rendering
��          �� : CSize sampling	(in)�_�Q�f�[�^�̃T���v�����O�Ԋu
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::Rendering(CSize sampling)
{
	float			*px, *py, *pz , *pz0;
	unsigned char	*p_ma, *p_bri;
	long			i, j, addr, x, y;


	ZeroMemory( p_screen_bmp_data , POINT_CLOUD_W * POINT_CLOUD_H);//�X�N���[����\���r�b�g�}�b�v�̒��g���[���N���A
	ZeroMemory( p_pixel_flg , POINT_CLOUD_W * POINT_CLOUD_H);//�X�N���[���̉�f�ɂ��łɒl���������܂�Ă��邱�Ƃ�\���t���O���[���N���A

		for(j=0 ; j<data_size.cy ; j+= sampling.cy){//�X�N���[���̏c�����̃��[�v�i��_�Ԋu�ŃT���v�����O�j
			px = p_transformed_x + j*data_size.cx;
			py = p_transformed_y + j*data_size.cx;
			pz = p_transformed_z + j*data_size.cx;
			pz0 = p_original_z + j*data_size.cx;
			p_ma = p_mask + j*data_size.cx;
			p_bri = p_brightness + (data_size.cy -1 -j) * bmp_data_width;

				for( i=0 ; i<data_size.cx ; i+=sampling.cx){//�X�N���[���̉������̃��[�v�i��_�Ԋu�ŃT���v�����O�j
					x = (long)((*px) * scale + POINT_CLOUD_W/2 + translate.x);
					y = (long)((*py) * scale + POINT_CLOUD_H/2 + translate.y);

						if( *p_ma == 0 )
						if( ( x>=0  &&  x<POINT_CLOUD_W)   &&   ( y>=0  &&  y<POINT_CLOUD_H) ){//���W����ʓ��ɓ����Ă��邩�ǂ���
							addr =  x + (POINT_CLOUD_H -1 - y  ) * POINT_CLOUD_W;
								
								//�A�_����
								if( *(p_pixel_flg + addr) == 0 ){//�X�N���[���̉�f�ɂ͂��߂Ēl��`�����ޏꍇ
									*( p_screen_bmp_data + addr ) = *p_bri ;
									*( p_pixel_flg + addr) = 1;
									*( pZbuffer + addr) = *pz;//z�o�b�t�@�ɉ��s�����������킦��

								}else{ //�X�N���[���̉�f�ɏ㏑������ꍇ�i�A�_�������K�v�j

									if( *(pZbuffer + addr) < *pz ){//�ǂ���̒l��\�����邩Z�o�b�t�@������ׂĔ��f����
										*( p_screen_bmp_data + addr ) = *p_bri ;
										*( p_pixel_flg + addr) = 1;
										*( pZbuffer + addr) = *pz;
									}
								}
						}

					px +=sampling.cx;
					py +=sampling.cx;
					pz +=sampling.cx;
					pz0 +=sampling.cx;
					p_ma +=sampling.cx;
					p_bri +=sampling.cx;
				}
		}
	draw_screen();
}

/********************************************************************
�@  �\  ��  �� : �X�P�[��(�k��)��`��
��    ��    �� : draw_scale
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::draw_scale()
{
	CPoint	scale_origin( POINT_CLOUD_W - 300 , POINT_CLOUD_H - 20);
	CSize	scale_size( 300, 25 );
	CDC		*pDC = GetDC();
	float	n, scale_length;
	CString	str;
	CFont	font,*p_old_font;

	n = (float)log10( ( 300 - 20 ) / scale );
	scale_length = (float)pow( (double)10, (int)n );
	str.Format( "%f mm" , scale_length );

	scale_origin.x = (long)(POINT_CLOUD_W - (scale_length*scale + 20) - 10);
	scale_size.cx = (long)(scale_length*scale + 20);

	font.CreateFontIndirect(&API.default_font);
	p_old_font = pDC->SelectObject(&font);

	pDC->Rectangle( scale_origin.x , scale_origin.y , scale_origin.x + scale_size.cx , scale_origin.y + scale_size.cy );
	pDC->TextOut( (int)(scale_origin.x + 10 + scale_length*scale/2 - 15), scale_origin.y + 5 , str, 6);
	pDC->TextOut( (int)(scale_origin.x + 10 + scale_length*scale/2 + 16), scale_origin.y + 5 , CString("mm") );
	pDC->MoveTo( scale_origin.x + 10 , scale_origin.y + 20 );
	pDC->LineTo( (int)(scale_origin.x + 10 + scale_length*scale) , scale_origin.y + 20 );
	pDC->MoveTo( scale_origin.x + 10 , scale_origin.y + 20 );
	pDC->LineTo( scale_origin.x + 10 , scale_origin.y + 17 );
	pDC->MoveTo( (int)(scale_origin.x + 10 + scale_length*scale) , scale_origin.y + 20 );
	pDC->LineTo( (int)(scale_origin.x + 10 + scale_length*scale) , scale_origin.y + 17 );
	
	pDC->SelectObject(p_old_font);
	ReleaseDC(pDC);
	font.DeleteObject();
}

/********************************************************************
�@  �\  ��  �� : �_�Q��Y������ɉ�]����
��    ��    �� : RotationPhai
��          �� : float	phai	(in)��]�p�x[rad]
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::RotationPhai(float phai)
{
	int i;
	double	temp;
	double	sin_phai, cos_phai;
	float	*px, *pz;

	sin_phai = sin( phai );
	cos_phai = cos( phai );

	px = p_transformed_x;
	pz = p_transformed_z;
		for(i=0  ;  i<data_size.cx*data_size.cy  ;  i++){
			//��]������
			temp = cos_phai * (*px) - sin_phai * (*pz);
			*pz = (float)(sin_phai * (*px) + cos_phai * (*pz));
			*px = (float)temp;

			px++;
			pz++;
		}	
}

/********************************************************************
�@  �\  ��  �� : �_�Q��X������ɉ�]����
��    ��    �� : RotationTheta
��          �� : float	theta	(in)��]�p�x[rad]
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::RotationTheta(float theta)
{
	int i;
	double	temp;
	double	sin_theta, cos_theta;
	float	*py, *pz;

	sin_theta = sin( theta );
	cos_theta = cos( theta );

	py = p_transformed_y ;
	pz = p_transformed_z ;
		for(i=0  ;  i<data_size.cx*data_size.cy  ;  i++){
			//��]������
			temp = cos_theta * (*py) - sin_theta * (*pz);
			*pz = (float)(sin_theta * (*py) + cos_theta * (*pz));
			*py = (float)temp;

			py++;
			pz++;
		}
}

/********************************************************************
�@  �\  ��  �� : 3D�R���g���[�����@�̎擾
��    ��    �� : get_ctrl_type
��          �� : 
��    ��    �l : 0:��]   1:�g��/�k��   2:���s�ړ�
�@          �\ : ���W�I�{�^���̒l���擾
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
short CPointCloudDlg::get_ctrl_type()
{
	if (is_druging)
	{//���{�^���h���b�O
		if (API.GetCheck(m_hWnd, IDC_CHECK_POINT_CLOUD_CTRL_ROTATE)) {//��]
			return	0;

		}
		else if (API.GetCheck(m_hWnd, IDC_CHECK_POINT_CLOUD_CTRL_SCALE)) {//�g��/�k��
			return	1;

		}
		else if (API.GetCheck(m_hWnd, IDC_CHECK_POINT_CLOUD_CTRL_TRANSLATE)) {//���s�ړ�
			return	2;
		}
	}
	else if(is_draging_mbutton)
	{//�����{�^���h���b�O
		return	2;
	}


	return 0;
}

/********************************************************************
�@  �\  ��  �� : 3D�R���g���[�����@�̐ݒ�
��    ��    �� : set_ctrl_type
��          �� : short type		(in)0:��]   1:�g��/�k��   2:���s�ړ�
��    ��    �l : 
�@          �\ : ���W�I�{�^���̒l��ݒ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::set_ctrl_type(short type)
{
	bool	ctrl_flg[3];//�R���g���[���^�C�v�I���{�^���̏�Ԃ����߂�t���O

		for(int n=0 ; n<3 ; n++){//type �̒l�ɂ���ăt���O��������
			if(n == type)		ctrl_flg[n] = true;
			else				ctrl_flg[n] = false;
		}

	//�t���O�̒l�ŃR���g���[���^�C�v�I���{�^���̏�Ԃ����߂�
	API.SetCheck( m_hWnd , IDC_CHECK_POINT_CLOUD_CTRL_ROTATE , ctrl_flg[0] );
	API.SetCheck( m_hWnd , IDC_CHECK_POINT_CLOUD_CTRL_SCALE  , ctrl_flg[1] );
	API.SetCheck( m_hWnd , IDC_CHECK_POINT_CLOUD_CTRL_TRANSLATE , ctrl_flg[2] );
}

/********************************************************************
�@  �\  ��  �� : �}�E�X�̍��{�^���������ꂽ
��    ��    �� : OnLButtonDown
��          �� : 
��    ��    �l : 
�@          �\ : �}�E�X�h���b�O�̊J�n
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	GetCapture();//�}�E�X�L���v�`���[
	is_druging = true;	//�h���b�O����\���t���O�𗧂Ă�
	mouse_down_point = point;//�}�E�X�����������W���L������
}

/********************************************************************
�@  �\  ��  �� : �}�E�X�̍��{�^���������ꂽ
��    ��    �� : OnLButtonUp
��          �� : 
��    ��    �l : 
�@          �\ : �}�E�X�h���b�O�̏I��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();//�}�E�X�L���v�`���[���J������
	is_druging = false;//�h���b�O���̃t���O��|��
	Rendering( sampling_size );	//�X�N���[���ɕ`�悷��
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
void CPointCloudDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	GetCapture();//�}�E�X���L���v�`���[����
	is_draging_mbutton = true;//�h���b�O����\���t���O�𗧂Ă�
	mouse_down_point = point;//�}�E�X�����������W���L������
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
void CPointCloudDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();//�}�E�X�L���v�`���[���J������
	is_draging_mbutton = false;
	Rendering(sampling_size);	//�X�N���[���ɕ`�悷��
}

/********************************************************************
�@  �\  ��  �� :�}�E�X�z�C�[���̉�]
��    ��    �� : OnMouseWheel
��          �� :
��    ��    �l :
�@          �\ : �g��E�k��
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CPointCloudDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//�X�P�[����ύX���ĕ`��
	if (zDelta>0) {
		scale *= 1.1;//10%�g��
	}
	else {
		scale *= 0.9;//10%�k��
	}

	Rendering(sampling_size);	//�X�N���[���ɕ`�悷��

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


/********************************************************************
�@  �\  ��  �� : �}�E�X�|�C���^���ړ�����
��    ��    �� : OnMouseMove
��          �� : 
��    ��    �l : 
�@          �\ : �}�E�X�h���b�O��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	int		dx,dy;
	double	temp;

		if( !is_druging && !is_draging_mbutton)	return;//�h���b�O���łȂ��ꍇ�͉������Ȃ�
		if( point.y<20 )	return;//�}�E�X�|�C���^��y���W��20��菬�����i�R���g���[���^�C�v�I���{�^���́j�̈�ł͂Ȃɂ����Ȃ�

	//�h���b�O�ɂ��ړ��ʌv�Z
	dx = mouse_down_point.x - point.x;
	dy = -( mouse_down_point.y - point.y );

		switch( get_ctrl_type() ){
			case 0:	//��]
				RotationPhai( (float)(dx * 0.02) );
				RotationTheta( (float)(dy * 0.02) );
				break;

			case 1://�g��E�k��
					if( (temp = scale + dy*0.01) > 0 )		scale = (float)temp;//�k�ڂ��������Ȃ�߂��Ȃ��悤�ɂ���
				break;

			case 2://�����ړ�
				translate += CPoint( -dx, -dy );
				break;
		}
	Rendering( CSize(6,6) );

	mouse_down_point = point;
	
	CDialog::OnLButtonUp(nFlags, point);
}

/********************************************************************
�@  �\  ��  �� : �u��]�v�{�^���������ꂽ
��    ��    �� : OnPointCloudCtrlRotate
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::OnPointCloudCtrlRotate() 
{
	set_ctrl_type(0);
}

/********************************************************************
�@  �\  ��  �� : �u�g��/�k���v�{�^���������ꂽ
��    ��    �� : OnPointCloudCtrlScale
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::OnPointCloudCtrlScale() 
{
	set_ctrl_type(1);
}

/********************************************************************
�@  �\  ��  �� : �u���s�ړ��v�{�^���������ꂽ
��    ��    �� : OnPointCloudCtrlTranslate
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CPointCloudDlg::OnPointCloudCtrlTranslate() 
{
	set_ctrl_type(2);
}


