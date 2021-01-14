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
//�摜�������֒��ڕ`��
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "opencv_headers.h"
#include "PIMPOM_API.h"
#include "Labeling.h"


// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#define		BIGGER(A,B)			( ((A)>(B)) ? (A):(B) )
#define		SMALLER(A,B)		( ((A)<(B)) ? (A):(B) )

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�e���v���[�g�Ō^�Ȃ�(�S�Ă̌^�̉摜�ɑΉ�����)�֐�������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�摜��������ɒ����`��
template <class DATA>
void	draw_line_on_image(CPimpomAPI *pAPI, long image_num, double x0, double y0, double x1, double y1, int pen_size, DATA dat)
{
	int			r,g,b;
	double		dir[2], len;
	double xc,yc,x,y;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/B�̊e�l
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//�O���[�摜�����݂��郁�����ɑ΂��āADATA=RGBREF�ł��̊֐����Ă񂾏ꍇ�̑Ή�
		if (!p_du->DispRGB) {
			dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
		}
	}



	//�n�_�E�I�_�ԋ���
	len = sqrt((double)((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)));
	dir[0] = (double)(x1 - x0)/len;
	dir[1] = (double)(y1 - y0)/len;

	//�`��
	for(double d=0 ; d<=len ; d+=0.5)
	{
		xc = x0 + dir[0]*d ;
		yc = y0 + dir[1]*d ;
		for(double w=-pen_size/2 ; w<=pen_size/2 ; w++)
		{
			x = xc - dir[1]*w;
			y = yc + dir[0]*w;
			int i = (int)(x + 0.5);
			int j = (int)(y + 0.5);

			if(i>=0 && j>=0 && i<p_du->DataSize.cx && j<p_du->DataSize.cy)
			{
				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					p_du->SetDataValue(i, j, r, -1, 0);
					p_du->SetDataValue(i, j, g, -1, 1);
					p_du->SetDataValue(i, j, b, -1, 2);
				}else{//���m�N��
					p_du->SetDataValue(i, j, dat, -1, -1);
				}
			}
		}
	}
}


//�摜��������ɉ~�`��
template <class DATA>
void	draw_circle_on_image(CPimpomAPI *pAPI, long image_num, double cx, double cy, double rad, DATA dat)
{
	int			r,g,b;
	double dt;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;
	if( rad<=0 )	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/B�̊e�l
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//�O���[�摜�����݂��郁�����ɑ΂��āADATA=RGBREF�ł��̊֐����Ă񂾏ꍇ�̑Ή�
		if (!p_du->DispRGB) {
			dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
		}
	}

	dt = 0.5/rad;//�~�ʍ��݊p�x(�~�����0.5��f����)

	//�`��
	for(double t=0 ; t<=2*PI+dt ; t+=dt)
	{
		double x = rad*cos(t) + cx;
		double y = rad*sin(t) + cy;
		int i= (int)(x+0.5);
		int j= (int)(y+0.5);

		if(i>=0 && j>=0 && i<p_du->DataSize.cx && j<p_du->DataSize.cy)
		{
			if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
			{//RGB

				p_du->SetDataValue(i, j, r, -1, 0);
				p_du->SetDataValue(i, j, g, -1, 1);
				p_du->SetDataValue(i, j, b, -1, 2);
			}else{//���m�N��
				p_du->SetDataValue(i, j, dat, -1, -1);
			}
		}
	}
}


//�摜��������ɓh��Ԃ��~�`��
template <class DATA>
void	fill_circle_on_image(CPimpomAPI *pAPI, long image_num, double cx, double cy, double rad, DATA dat)
{
	int			r,g,b;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/B�̊e�l
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//�O���[�摜�����݂��郁�����ɑ΂��āADATA=RGBREF�ł��̊֐����Ă񂾏ꍇ�̑Ή�
		if (!p_du->DispRGB) {
			dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
		}
	}



	//�`��
	for(int j=(int)(cy-rad-1.0) ; j<=(int)(cy+rad+1.0) ; j++)
	{
		for(int i=(int)(cx-rad-1.0) ; i<=(int)(cx+rad+1.0) ; i++)
		{
			if(i>=0 && j>=0 && i<p_du->DataSize.cx && j<p_du->DataSize.cy)
			{
				double dx = (double)i - cx;
				double dy = (double)j - cy;

				if( fabs(dx) < rad+1 && fabs(dy) < rad+1)
				{
					//�~�ɊO�ڂ����`�̈���̉�f
					double d = dx*dx + dy*dy;
					double radBorderThr = (rad+0.75)*(rad+0.75);
					double radInnerThr = (rad-0.75)*(rad-0.75);

					if( d < radBorderThr )
					{
						if( d < radInnerThr)
						{
							//�~�����̉�f = �w��F�œh��Ԃ�
							if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
							{//RGB
								p_du->SetDataValue(i, j, r, -1, 0);
								p_du->SetDataValue(i, j, g, -1, 1);
								p_du->SetDataValue(i, j, b, -1, 2);
							}else{//���m�N��
								p_du->SetDataValue(i, j, dat, -1, -1);
							}
						}
						else
						{
							//�~�����E����̉�f = ���ԐF�œh��Ԃ�
							int innersubPixCnt=0, subPixCnt=0;
							double innerRatio;
							for(double sy=dy-0.5 ; sy<=dy+0.5 ; sy+=0.2){
								for(double sx=dx-0.5 ; sx<=dx+0.5 ; sx+=0.2){
									subPixCnt++;
									if(sx*sx + sy*sy < rad*rad){
										innersubPixCnt++;
									}
								}
							}
							innerRatio = (double)innersubPixCnt/(double)subPixCnt;//��f�̉~������


							if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
							{//RGB
								double bgr = p_du->GetDataValue(i, j, -1, 0);
								double bgg = p_du->GetDataValue(i, j, -1, 1);
								double bgb = p_du->GetDataValue(i, j, -1, 2);

								double valr = (double)r*innerRatio +bgr*(1.0-innerRatio);
								double valg = (double)g*innerRatio +bgg*(1.0-innerRatio);
								double valb = (double)b*innerRatio +bgb*(1.0-innerRatio);

								p_du->SetDataValue(i, j, valr, -1, 0);
								p_du->SetDataValue(i, j, valg, -1, 1);
								p_du->SetDataValue(i, j, valb, -1, 2);
							}else{//���m�N��
								double bg = p_du->GetDataValue(i, j, -1, -1);
								double val = (double)dat*innerRatio +bg*(1.0-innerRatio);
								p_du->SetDataValue(i, j, val, -1, -1);
							}
						}
					}
				}
			}
		}
	}
}


//�摜��������ɋ�`�`��
template <class DATA>
void	draw_rect_on_image(CPimpomAPI *pAPI, long image_num, double x0, double y0, double x1, double y1, DATA dat, bool fill, DATA fill_dat)
{
	int			r,g,b;
	double		tmp;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/B�̊e�l
		r = (COLORREF)fill_dat % 0x100;
		g = ((COLORREF)fill_dat / 0x100) % 0x100;
		b = ((COLORREF)fill_dat / 0x100 / 0x100) % 0x100;

		//�O���[�摜�����݂��郁�����ɑ΂��āADATA=RGBREF�ł��̊֐����Ă񂾏ꍇ�̑Ή�
		if (!p_du->DispRGB) {
			dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
			fill_dat = (DATA)r;
		}
	}



	//�n�_�I�_���W�̑召�C��
	if(x0>x1){
		tmp = x0;	x0=x1;	x1=tmp;
	}

	if(y0>y1){
		tmp = y0;	y0=y1;	y1=tmp;
	}






	//�����h��Ԃ�
	if(fill)
	{
		int			x0i, x1i, y0i, y1i;//�����̍��W
		x0i = (int)x0+1;
		y0i = (int)y0+1;
		x1i = (int)x1;
		y1i = (int)y1;


		//����
		for(int j=y0i ; j<=y1i ; j++)
		{
			for(int i=x0i ; i<=x1i ; i++)
			{
				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					p_du->SetDataValue(i, j, r, -1, 0);
					p_du->SetDataValue(i, j, g, -1, 1);
					p_du->SetDataValue(i, j, b, -1, 2);
				}else{//���m�N��
					p_du->SetDataValue(i, j, fill_dat, -1, -1);
				}
			}
		}

		//��ӋߖT(����A�E��p�܂�)
		{
			double bg,fg;
			bg = y0-(int)y0;
			fg = 1.0 - bg;

			for(int i=x0i-1 ; i<=x1i+1 ; i++)
			{
				double bg2, fg2;
				if(i==x0i-1){//����p
					fg2 = fg*( (int)x0 + 1.0 - x0 );
					bg2 = 1.0 - fg2;
				}else if(i==x1i+1){//�E��p
					fg2 = fg*( x1 - (int)x1 );
					bg2 = 1.0 - fg2;
				}else{
					fg2 = fg;
					bg2 = bg;
				}

				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					double bg_r = p_du->GetDataValue(i, (int)y0, -1, 0);
					double bg_g = p_du->GetDataValue(i, (int)y0, -1, 1);
					double bg_b = p_du->GetDataValue(i, (int)y0, -1, 2);

					p_du->SetDataValue(i, (int)y0, r*fg2 + bg_r*bg2 , -1, 0);
					p_du->SetDataValue(i, (int)y0, g*fg2 + bg_g*bg2 , -1, 1);
					p_du->SetDataValue(i, (int)y0, b*fg2 + bg_b*bg2 , -1, 2);
				}else{//���m�N��
					double bg_val = p_du->GetDataValue(i, (int)y0, -1, -1);
					p_du->SetDataValue(i, (int)y0, fill_dat*fg2 + bg_val*bg2, -1, -1);
				}
			}
		}
		
		//���ӋߖT
		{
			double bg,fg;
			bg = x0-(int)x0;
			fg = 1.0 - bg;

			for(int j=y0i ; j<=y1i ; j++)
			{
				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					double bg_r = p_du->GetDataValue((int)x0, j, -1, 0);
					double bg_g = p_du->GetDataValue((int)x0, j, -1, 1);
					double bg_b = p_du->GetDataValue((int)x0, j, -1, 2);

					p_du->SetDataValue((int)x0, j, r*fg + bg_r*bg , -1, 0);
					p_du->SetDataValue((int)x0, j, g*fg + bg_g*bg , -1, 1);
					p_du->SetDataValue((int)x0, j, b*fg + bg_b*bg , -1, 2);
				}else{//���m�N��
					double bg_val = p_du->GetDataValue((int)x0, j, -1, -1);
					p_du->SetDataValue((int)x0, j, fill_dat*fg + bg_val*bg , -1, -1);
				}
			}
		}

		//���ӋߖT(�����A�E���p�܂�)
		{
			double bg,fg;
			fg = y1-(int)y1;
			bg = 1.0 - fg;

			for(int i=x0i-1 ; i<=x1i+1 ; i++)
			{
				double bg2, fg2;
				if(i==x0i-1){//����p
					fg2 = fg*( (int)x0 + 1.0 - x0 );
					bg2 = 1.0 - fg2;
				}else if(i==x1i+1){//�E��p
					fg2 = fg*( x1 - (int)x1 );
					bg2 = 1.0 - fg2;
				}else{
					fg2 = fg;
					bg2 = bg;
				}

				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					double bg_r = p_du->GetDataValue(i, (int)y1+1, -1, 0);
					double bg_g = p_du->GetDataValue(i, (int)y1+1, -1, 1);
					double bg_b = p_du->GetDataValue(i, (int)y1+1, -1, 2);

					p_du->SetDataValue(i, (int)y1+1, r*fg2 + bg_r*bg2 , -1, 0);
					p_du->SetDataValue(i, (int)y1+1, g*fg2 + bg_g*bg2 , -1, 1);
					p_du->SetDataValue(i, (int)y1+1, b*fg2 + bg_b*bg2 , -1, 2);
				}else{//���m�N��
					double bg_val = p_du->GetDataValue(i, (int)y1+1, -1, -1);
					p_du->SetDataValue(i, (int)y1+1, fill_dat*fg2 + bg_val*bg2, -1, -1);
				}
			}
		}

		//�E�ӋߖT
		{
			double bg,fg;
			fg = x1-(int)x1;
			bg = 1.0 - fg;

			for(int j=y0i ; j<=y1i ; j++)
			{
				if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
				{//RGB
					double bg_r = p_du->GetDataValue((int)x1+1, j, -1, 0);
					double bg_g = p_du->GetDataValue((int)x1+1, j, -1, 1);
					double bg_b = p_du->GetDataValue((int)x1+1, j, -1, 2);

					p_du->SetDataValue((int)x1+1, j, r*fg + bg_r*bg , -1, 0);
					p_du->SetDataValue((int)x1+1, j, g*fg + bg_g*bg , -1, 1);
					p_du->SetDataValue((int)x1+1, j, b*fg + bg_b*bg , -1, 2);
				}else{//���m�N��
					double bg_val = p_du->GetDataValue((int)x1+1, j, -1, -1);
					p_du->SetDataValue((int)x1+1, j, fill_dat*fg + bg_val*bg , -1, -1);
				}
			}
		}

		//����

	}
	else
	{
		//�O�g�̂�
		draw_line_on_image(pAPI, image_num, x0, y0, x1, y0, 1, dat);//��
		draw_line_on_image(pAPI, image_num, x1, y0, x1, y1, 1, dat);//�E
		draw_line_on_image(pAPI, image_num, x1, y1, x0, y1, 1, dat);//��
		draw_line_on_image(pAPI, image_num, x0, y1, x0, y0, 1, dat);//��
	}
}





//�摜��������Ƀe�L�X�g�`��
template <class DATA>
void draw_text_on_image(CPimpomAPI *pAPI, long image_num, int x, int y, char str[], DATA dat )
{
	int			imgx,imgy;
	int			r,g,b;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/B�̊e�l
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//�O���[�摜�����݂��郁�����ɑ΂��āADATA=RGBREF�ł��̊֐����Ă񂾏ꍇ�̑Ή�
		if (!p_du->DispRGB) {
			dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
		}
	}





	//�L�����o�X�ɕ�������������
	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
	float scale = (float)pAPI->default_font.lfHeight / (float)22;
	int thickness = 1;
	int baseline = 0;
	cv::Size textSize = getTextSize(cv::String(str), fontFace, scale, thickness, &baseline);
	cv::Mat canvas = cv::Mat::zeros(textSize.height, textSize.width, CV_8UC1);
	cv::putText(canvas, cv::String(str), cv::Point(0, textSize.height-1), fontFace, scale, cv::Scalar(255), thickness, cv::LINE_8);

	//�摜�f�[�^�ɃL�����o�X���R�s�[����
	for (int j = 0; j<canvas.rows; j++)
	{
		for (int i = 0; i<canvas.cols; i++)
		{
			imgx = x + i;
			imgy = y + j;

			if (canvas.at<unsigned char>(j,i) > 0)
			{
				if ((p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT) && p_du->DispRGB)
				{//RGB
					p_du->SetDataValue(imgx, imgy, r, -1, 0);
					p_du->SetDataValue(imgx, imgy, g, -1, 1);
					p_du->SetDataValue(imgx, imgy, b, -1, 2);
				}
				else {//���m�N��
					p_du->SetDataValue(imgx, imgy, dat, -1, -1);
				}
			}
		}
	}



}


//�摜��������Ƀh�b�g�`��
template <class DATA>
void draw_point_on_image(CPimpomAPI *pAPI, long image_num, CPoint point, int dot_size, DATA dat, bool tomask)
{
	int			r,g,b;
	int			dot_size_half = dot_size/2;
	CDataUnit *p_du;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;

	//�h�b�g�ʒu���摜��ɂ��邩�`�F�b�N
	if(point.x<0 || point.y<0 || point.x >= p_du->DataSize.cx || point.y >= p_du->DataSize.cy)	return ;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/B�̊e�l
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//RGB�ɑ΂��āADATA=RGBREF�ł��̊֐����Ă񂾏ꍇ�̑Ή�
		if (!p_du->DispRGB) {
			dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
		}
	}



	////�O���[�摜�����݂��郁�����ɑ΂��āA���̊֐����Ă񂾏ꍇ�̑Ή�
	//if( !( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB) ){
	//	dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
	//}


	for(int j=-dot_size_half ; j<=dot_size_half ; j++)
	{
		for(int i=-dot_size_half ; i<=dot_size_half ; i++)
		{
			if(i*i+j*j<=dot_size_half*dot_size_half)//�~�`�h�b�g
			{
				CPoint pos(point.x+i, point.y+j);

				if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(pos) )//��Ɨ̈�ɓ����Ă��邩�ǂ�������
				{
					if(!tomask)
					{
						if( !p_du->DoUseMask || p_du->GetMaskValue(pos)==UNMASKED_PIXEL )//�}�X�N���������Ă��邩�ǂ�������
						if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
						{//RGB
							p_du->SetDataValue(pos.x, pos.y, r, -1,0);
							p_du->SetDataValue(pos.x, pos.y, g, -1,1);
							p_du->SetDataValue(pos.x, pos.y, b, -1,2);
						}else{//���m�N��
							p_du->SetDataValue(pos.x, pos.y, (double)dat, -1, -1);
						}
					}else{//�}�X�N
						p_du->SetMaskValue(pos.x, pos.y, (BYTE)dat);
					}
				}
			}
		}
	}
}



//�摜��������ɓh��Ԃ��`��
template <class DATA>
void flood_fill_on_image(CPimpomAPI *pAPI, long image_num, CPoint point, DATA dat, bool tomask)
{
	int			r,g,b;
	CDataUnit	*p_du;
	CRect		calc_area;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;
	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�

	//�h��Ԃ��J�n�ʒu���摜��ɂ��邩�`�F�b�N
	if(point.x<0 || point.y<0 || point.x >= p_du->DataSize.cx || point.y >= p_du->DataSize.cy)	return ;

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/B�̊e�l
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//�O���[�摜�����݂��郁�����ɑ΂��āADATA=RGBREF�ł��̊֐����Ă񂾏ꍇ�̑Ή�
		if (!p_du->DispRGB) {
			dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
		}
	}



	//�h��Ԃ��̈�����x�����O�Ō��߂�/////////////////////////////////////
	LabelingBS	labeling;
	unsigned char *pbinary;
	short *plabel;
	short fill_label_no;

	pbinary = (unsigned char*)malloc( p_du->DataSize.cx * p_du->DataSize.cy );//�Q�l���摜
	if(pbinary==NULL)	return ;
	memset(pbinary,0,p_du->DataSize.cx * p_du->DataSize.cy);

	plabel = (short*)malloc( p_du->DataSize.cx * p_du->DataSize.cy * sizeof(short));//���x���摜
	if(plabel==NULL){
		free(plabel);
		return ;
	}


	if(!tomask)
	{
		if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
		{//RGB
			//�I����f�̐F���s�b�N���āA���F�����̂Q�l���摜������
			double valr,valg,valb;
			valr = p_du->GetDataValue(point, -1, 0);
			valg = p_du->GetDataValue(point, -1, 1);
			valb = p_du->GetDataValue(point, -1, 2);
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//��Ɨ̈�ɓ����Ă��邩�ǂ�������
					if( !p_du->DoUseMask || p_du->GetMaskValue(CPoint(i,j))==UNMASKED_PIXEL )//�}�X�N���������Ă��邩�ǂ�������
					if( p_du->GetDataValue(CPoint(i,j),-1,0) == valr && //�����F�ł��邩�ǂ����m�F
						p_du->GetDataValue(CPoint(i,j),-1,1) == valg && 
						p_du->GetDataValue(CPoint(i,j),-1,2) == valb 
						){
							*(pbinary + i + j*p_du->DataSize.cx) = 1;
						}
				}
			}

			//���x�����O���āA�h��Ԃ����x���ԍ����Ƃ�
			labeling.Exec( pbinary, plabel, p_du->DataSize.cx, p_du->DataSize.cy, true, 0 );
			fill_label_no = *(plabel + point.x + point.y*p_du->DataSize.cx);

			//�ʂ�Ԃ�
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( *(plabel + i + j*p_du->DataSize.cx) == fill_label_no){
						p_du->SetDataValue(i, j, r, -1,0);
						p_du->SetDataValue(i, j, g, -1,1);
						p_du->SetDataValue(i, j, b, -1,2);
					}
				}
			}
		}
		else
		{//���m�N��
			//�I����f�̐F���s�b�N���āA���F�����̂Q�l���摜������
			DATA val = (DATA)p_du->GetDataValue(point,-1,-1);
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//��Ɨ̈�ɓ����Ă��邩�ǂ�������
					if( !p_du->DoUseMask || p_du->GetMaskValue(CPoint(i,j))==UNMASKED_PIXEL )//�}�X�N���������Ă��邩�ǂ�������
					if( p_du->GetDataValue(CPoint(i,j),-1,-1) == val){//�����l�ł��邩�ǂ����m�F
						*(pbinary + i + j*p_du->DataSize.cx) = 1;
					}else{
						*(pbinary + i + j*p_du->DataSize.cx) = 0;
					}
				}
			}

			//���x�����O���āA�h��Ԃ����x���ԍ����Ƃ�
			labeling.Exec( pbinary, plabel, p_du->DataSize.cx, p_du->DataSize.cy, true, 0 );
			fill_label_no = *(plabel + point.x + point.y*p_du->DataSize.cx);

			//�ʂ�Ԃ�
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( *(plabel + i + j*p_du->DataSize.cx) == fill_label_no){
						p_du->SetDataValue(i, j, dat, -1, -1);
					}
				}
			}
		}
	}
	else
	{//�}�X�N�摜
			//�I����f�̐F���s�b�N���āA���F�����̂Q�l���摜������
			BYTE val = p_du->GetMaskValue(point);
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( !p_du->DoUseWorkArea || p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//��Ɨ̈�ɓ����Ă��邩�ǂ�������
					if( p_du->GetMaskValue(CPoint(i,j)) == val){//�����l�ł��邩�ǂ����m�F
						*(pbinary + i + j*p_du->DataSize.cx) = 1;
					}else{
						*(pbinary + i + j*p_du->DataSize.cx) = 0;
					}
				}
			}

			//���x�����O���āA�h��Ԃ����x���ԍ����Ƃ�
			labeling.Exec( pbinary, plabel, p_du->DataSize.cx, p_du->DataSize.cy, true, 0 );
			fill_label_no = *(plabel + point.x + point.y*p_du->DataSize.cx);

			//�ʂ�Ԃ�
			for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
			{
				for(int i=calc_area.left ; i<=calc_area.right ; i++)
				{
					if( *(plabel + i + j*p_du->DataSize.cx) == fill_label_no){
						p_du->SetMaskValue(i, j, (BYTE)dat);
					}
				}
			}
	}

	free(plabel);
	free(pbinary);

}

//��Ɨ̈�����h��Ԃ�
template <class DATA>
void fill_workarea_on_image(CPimpomAPI *pAPI, long image_num, DATA dat, bool tomask)
{
	int			r,g,b;
	CDataUnit	*p_du;
	CRect		calc_area;

	if( (p_du = pAPI->GetDataUnit(image_num))==NULL)	return;
	if( !p_du->DoUseWorkArea )	return;//��Ɨ̈��\���̏ꍇ�͂Ȃɂ����Ȃ�

	calc_area = p_du->GetRectArea();//�����Ώۂ̋�`�͈�

	if (p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT)
	{
		//R/G/B�̊e�l
		r = (COLORREF)dat % 0x100;
		g = ((COLORREF)dat / 0x100) % 0x100;
		b = ((COLORREF)dat / 0x100 / 0x100) % 0x100;

		//�O���[�摜�����݂��郁�����ɑ΂��āADATA=RGBREF�ł��̊֐����Ă񂾏ꍇ�̑Ή�
		if (!p_du->DispRGB) {
			dat = (DATA)r;//r�̒l���O���[�l�Ƃ��Ďg��
		}
	}



	for(int j=calc_area.top ; j<=calc_area.bottom ; j++)
	{
		for(int i=calc_area.left ; i<=calc_area.right ; i++)
		{
			if( p_du->WorkArea.PtInWorkArea(CPoint(i,j)) )//��Ɨ̈�ɓ����Ă��邩�ǂ�������
			{
				if(!tomask)
				{
					if( ( p_du->DataType == RGB_FORMAT || p_du->DataType == RGB_3D_FORMAT ) && p_du->DispRGB )
					{//RGB
						p_du->SetDataValue(i, j, r, -1,0);
						p_du->SetDataValue(i, j, g, -1,1);
						p_du->SetDataValue(i, j, b, -1,2);
					}else{//���m�N��
						p_du->SetDataValue(i, j, dat, -1, -1);
					}
				}
				else//�}�X�N
				{
					p_du->SetMaskValue(i, j, (BYTE)dat);
				}
			}
		}
	}
}




/********************************************************************
�@  �\  ��  �� : �摜��������ɒ����`��iRGB�^�ARGB 3D�^�̉摜�������ɑΉ��j
��    ��    �� : DrawLineOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 double		x0,y0			(in)�n�_���W
				 double		x1,y1			(in)�I�_���W
				 int		pen_size		(in)�`�悷��y���T�C�Y
				 COLORREF	col				(in)�`��F
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
                 copy_image_num�ɉ摜�ԍ����w�肷��ƁA�w�i�Ƃ��ĉ摜���R�s�[���Ă��炻�̏�ɕ`�悷��
				 �O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/

void	CPimpomAPI::DrawLineOnImage(long image_num, double x0, double y0, double x1, double y1, int pen_size, COLORREF col, int copy_image_num)
{
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber())
	{
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	draw_line_on_image(this, image_num,x0,y0,x1,y1,pen_size,col);
}


/********************************************************************
�@  �\  ��  �� : �摜��������ɒ����`��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawLineOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 double		x0,y0			(in)�n�_���W
				 double		x1,y1			(in)�I�_���W
				 int		pen_size		(in)�`�悷��y���T�C�Y
				 double		val				(in)�`��l
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawLineOnImage(long image_num, double x0, double y0, double x1, double y1, int pen_size, double val)
{
	draw_line_on_image(this, image_num,x0,y0,x1,y1,pen_size,val);
}


/********************************************************************
�@  �\  ��  �� : �摜��������ɒ����`��iRGB�^�ARGB 3D�^�̉摜�������ɑΉ��j
��    ��    �� : DrawLineOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 double		a,b,c			(in)����������(ax+by+c=0)
				 int		pen_size		(in)�`�悷��y���T�C�Y
				 COLORREF	col				(in)�`��F
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
                 copy_image_num�ɉ摜�ԍ����w�肷��ƁA�w�i�Ƃ��ĉ摜���R�s�[���Ă��炻�̏�ɕ`�悷��
				 �O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/

void	CPimpomAPI::DrawLineOnImageF(long image_num, double a, double b, double c, int pen_size, COLORREF col, int copy_image_num)
{
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber())
	{
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	CDataUnit *pdu = GetDataUnit(image_num);
	if(!pdu)	return;

	double x0,y0,x1,y1;
	if( !ClipSegmentF( CRect(0,0,pdu->DataSize.cx-1,pdu->DataSize.cy-1), a, b, c, &x0, &y0, &x1, &y1) )	return;

	draw_line_on_image(this, image_num,x0,y0,x1,y1,pen_size,col);
}



/********************************************************************
�@  �\  ��  �� : �摜��������ɒ����`��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawLineOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 double		a,b,c			(in)����������(ax+by+c=0)
				 int		pen_size		(in)�`�悷��y���T�C�Y
				 double		val				(in)�`��l
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawLineOnImageF(long image_num, double a, double b, double c, int pen_size, double val)
{
	CDataUnit *pdu = GetDataUnit(image_num);
	if(!pdu)	return;

	double x0,y0,x1,y1;
	if( !ClipSegmentF( CRect(0,0,pdu->DataSize.cx-1,pdu->DataSize.cy-1), a, b, c, &x0, &y0, &x1, &y1) )	return;

	draw_line_on_image(this, image_num,x0,y0,x1,y1,pen_size,val);
}


/********************************************************************
�@  �\  ��  �� : �摜��������ɏ\���J�[�\���`��iRGB�^�ARGB 3D�^�̉摜�������ɑΉ��j
��    ��    �� : DrawCursorOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 double		x,y				(in)���W
				 int		pen_size		(in)�`�悷��y���T�C�Y
				 COLORREF	col				(in)�`��F
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
                 copy_image_num�ɉ摜�ԍ����w�肷��ƁA�w�i�Ƃ��ĉ摜���R�s�[���Ă��炻�̏�ɕ`�悷��
				 �O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawCursorOnImage(long image_num, double x, double y, int pen_size, COLORREF col, int copy_image_num)
{
	long		mark_length = 20;//�}�[�N�̃��C���̒���

	DrawLineOnImage(image_num, (double)(x - mark_length/2) , y , (double)(x + mark_length/2) , y, pen_size, col, copy_image_num);
	DrawLineOnImage(image_num, x , (double)(y - mark_length/2) , x , (double)(y + mark_length/2), pen_size, col);
}


/********************************************************************
�@  �\  ��  �� : �摜��������ɏ\���J�[�\���`��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawCursorOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 double		x,y			(in)���W
				 int		pen_size		(in)�`�悷��y���T�C�Y
				 double		val				(in)�`��l
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawCursorOnImage(long image_num, double x, double y, int pen_size, double val)
{
	long		mark_length = 20;//�}�[�N�̃��C���̒���

	DrawLineOnImage(image_num, (double)(x - mark_length/2) , y , (double)(x + mark_length/2) , y, pen_size, val);
	DrawLineOnImage(image_num, x , (double)(y - mark_length/2) , x , (double)(y + mark_length/2), pen_size, val);
}


/********************************************************************
�@  �\  ��  �� : �摜��������ɉ~�`��iRGB�^�ARGB 3D�^�̉摜�������ɑΉ��j
��    ��    �� : DrawCircleOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 double		cx,cy			(in)���S���W
				 double		rad				(in)���a
				 COLORREF	col				(in)�`��F
				 bool		fill			(in)����h��Ԃ����ǂ���
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
                 copy_image_num�ɉ摜�ԍ����w�肷��ƁA�w�i�Ƃ��ĉ摜���R�s�[���Ă��炻�̏�ɕ`�悷��
				 �O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//RGB�^�ARGB 3D�^�̉摜�������ɑΉ�
void	CPimpomAPI::DrawCircleOnImage(long image_num, double cx, double cy, double rad, COLORREF col, bool fill, int copy_image_num)
{
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber()){
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	if(fill){
		fill_circle_on_image(this, image_num,cx,cy,rad,col);
	}else{
		draw_circle_on_image(this, image_num,cx,cy,rad,col);
	}
}

/********************************************************************
�@  �\  ��  �� : �摜��������ɉ~�`��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawCircleOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 double		cx,cy			(in)���S���W
				 double		rad				(in)���a
				 double		val				(in)�`��l
				 bool		fill			(in)����h��Ԃ����ǂ���
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
      
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawCircleOnImage(long image_num, double cx, double cy, double rad, double val, bool fill)
{
	if(fill){
		fill_circle_on_image(this, image_num,cx,cy,rad,val);
	}else{
		draw_circle_on_image(this, image_num,cx,cy,rad,val);
	}
}


/********************************************************************
�@  �\  ��  �� : �摜��������ɋ�`�`��(RGB�^�ARGB 3D�^�̉摜�������ɑΉ�)
��    ��    �� : DrawRectOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				double		x0			(in)���Ӎ��W
				double		y0			(in)�E�Ӎ��W
				double		x1			(in)��Ӎ��W
				double		y1			(in)���Ӎ��W
				int			pen_size	(in)�`�悷��y���T�C�Y
				COLORREF	col			(in)�`��F
				bool		fill		(in)����h��Ԃ����ǂ���
				COLORREF	fill_col	(in)����h��Ԃ��F
				int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
                 copy_image_num�ɉ摜�ԍ����w�肷��ƁA�w�i�Ƃ��ĉ摜���R�s�[���Ă��炻�̏�ɕ`�悷��
				 �O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawRectOnImage(long image_num, double x0, double y0, double x1, double y1, COLORREF col, bool fill, COLORREF fill_col, int copy_image_num)
{
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber()){
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	draw_rect_on_image(this, image_num, x0,y0,x1,y1,col, fill, fill_col);
}


/********************************************************************
�@  �\  ��  �� : �摜��������ɋ�`�`��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawRectOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				double		x0			(in)���Ӎ��W
				double		y0			(in)�E�Ӎ��W
				double		x1			(in)��Ӎ��W
				double		y1			(in)���Ӎ��W
				int			pen_size	(in)�`�悷��y���T�C�Y
				double		val			(in)�`��l
				bool		fill		(in)����h��Ԃ����ǂ���
				double		fill_val	(in)����h��Ԃ��l
				int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��

���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//RGB�^�ARGB 3D�^�ȊO�̉摜�������ɑΉ�
void	CPimpomAPI::DrawRectOnImage(long image_num, double x0, double y0, double x1, double y1, double val, bool fill, double fill_val)
{
	draw_rect_on_image(this, image_num, x0,y0,x1,y1,val, fill, fill_val);
}


/********************************************************************
�@  �\  ��  �� : �摜��������Ƀe�L�X�g�`��(RGB�^�ARGB 3D�^�̉摜�������ɑΉ�)
��    ��    �� : DrawTextOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 int		x,y				(in)�`����W
				 char		str[],			(in)�e�L�X�g
				 COLORREF	col				(in)�`��F
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : ASCII�R�[�h�̂ݑΉ�
				 �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
                 copy_image_num�ɉ摜�ԍ����w�肷��ƁA�w�i�Ƃ��ĉ摜���R�s�[���Ă��炻�̏�ɕ`�悷��
				 �O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//RGB�^�ARGB 3D�^�̉摜�������ɑΉ�
void	CPimpomAPI::DrawTextOnImage(long image_num, int x, int y, char str[], COLORREF col, int copy_image_num)
{
	if(str==NULL)	return;

	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber()){
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	draw_text_on_image(this, image_num, x, y, str, col);
}


/********************************************************************
�@  �\  ��  �� : �摜��������Ƀe�L�X�g�`��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawTextOnImage
��          �� : int		image_num		(in)�摜�������ԍ�
				 int		x,y				(in)�`����W
				 char		str[],			(in)�e�L�X�g
				 double		val				(in)�`��l
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : ASCII�R�[�h�̂ݑΉ�
				�����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawTextOnImage(long image_num, int x, int y, char str[], double val)
{
	draw_text_on_image(this, image_num, x, y, str, val);
}


/********************************************************************
�@  �\  ��  �� : �t�H�[�}�b�g�w��ŉ摜��������Ƀe�L�X�g�`��iRGB�^�ARGB 3D�^�̉摜�������ɑΉ��j
��    ��    �� : DrawTextOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 int		x,y			(in)�`����W
				 COLORREF	col			(in)�`��F
				 char		*format, ...(in)�e�L�X�g
��    ��    �l : 
�@          �\ : ASCII�R�[�h�̂ݑΉ�
				�����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
				�O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawTextOnImage(long image_num, int x, int y, COLORREF col, char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	DrawTextOnImage(image_num,x,y,buf,col);
}


/********************************************************************
�@  �\  ��  �� : �t�H�[�}�b�g�w��ŉ摜��������Ƀe�L�X�g�`��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawTextOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 int		x,y			(in)�`����W
				 double		val			(in)�`��l
				 char		*format, ...(in)�e�L�X�g
��    ��    �l : 
�@          �\ : ASCII�R�[�h�̂ݑΉ�
				�����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawTextOnImage(long image_num, int x, int y, double val, char *format, ...)
{
	va_list list;
	char	buf[5000];

	va_start( list, format );
	vsprintf(buf,format, list);
	va_end( list );

	DrawTextOnImage(image_num,x,y,buf,val);
}




/********************************************************************
�@  �\  ��  �� : �摜��������Ƀh�b�g�`��iRGB�^�ARGB 3D�^�̉摜�������ɑΉ��j
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 int		x,y			(in)�`����W
				 int		dot_size	(in)�h�b�g�T�C�Y(���a)
				 COLORREF	col			(in)�`��F
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
				 �h�b�g�`��͉~
                 copy_image_num�ɉ摜�ԍ����w�肷��ƁA�w�i�Ƃ��ĉ摜���R�s�[���Ă��炻�̏�ɕ`�悷��
				 �O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawPointOnImage(long image_num, int x, int y, int dot_size, COLORREF col, int copy_image_num)
{
	//�J���[�w�i�摜��V���ɂ���ꍇ�̑Ή�
	if(copy_image_num>=0 && copy_image_num<GetDataUnitNumber()){
		CopyDataUnit(copy_image_num, image_num);
		CONVERT_DATA_OPTION	option;
		option.type_to = RGB_FORMAT;
		option.rgb_to = RGB_TO_ALL;
		ConvertDataUnit(image_num,option);	
	}

	draw_point_on_image(this, image_num, CPoint(x,y), dot_size, col, false);
}


/********************************************************************
�@  �\  ��  �� : �摜��������Ƀh�b�g�`��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 int		x,y			(in)�`����W
				 int		dot_size	(in)�h�b�g�T�C�Y(���a)
				 double		val			(in)�`��l
				 int		copy_image_num	(in)�R�s�[����摜�ԍ�
��    ��    �l : 
�@          �\ : �����y�[�W/�`���l��������ꍇ�͕\�����̂��̂ɕ`��
					�h�b�g�`��͉~
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawPointOnImage(long image_num, int x, int y, int dot_size, double val)
{
	draw_point_on_image(this, image_num, CPoint(x,y), dot_size, val, false);
}


/********************************************************************
�@  �\  ��  �� : �}�X�N��Ƀh�b�g�`��
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 int		x,y			(in)�`����W
				 int		dot_size	(in)�h�b�g�T�C�Y
				 BYTE		val			(in)�`��l�@1:�}�X�N���� 0:�}�X�N�Ȃ� ����ȊO�̒l�F�}�X�N�Ȃ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawPointOnMask(long image_num, int x, int y, int dot_size, BYTE val)
{
	if(val!=MASKED_PIXEL && val!=UNMASKED_PIXEL)	val=UNMASKED_PIXEL;

	draw_point_on_image(this, image_num, CPoint(x,y), dot_size, val, true);
}


/********************************************************************
�@  �\  ��  �� : �摜��������ŗ̈�h��Ԃ��������Ȃ��iRGB�^�ARGB 3D�^�̉摜�������ɑΉ��j
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 int		x,y			(in)�n�_���W
				 COLORREF	col			(in)�h��Ԃ��F
��    ��    �l : 
�@          �\ :�n�_���W�Ɠ����F�ł���̈���w��F�œh��Ԃ� 
				�����y�[�W/�`���l��������ꍇ�͕\�����̂��̂Ŏ��{
				�O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//RGB�^�ARGB 3D�^�̉摜�������ɑΉ�
void	CPimpomAPI::FloodFillOnImage(long image_num, int x, int y, COLORREF col)
{
	flood_fill_on_image(this, image_num, CPoint(x,y), col, false);
}


/********************************************************************
�@  �\  ��  �� : �摜��������ŗ̈�h��Ԃ��������Ȃ��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 int		x,y			(in)�n�_���W
				 double		val			(in)�h��Ԃ��l
��    ��    �l : 
�@          �\ :�n�_���W�Ɠ����F�ł���̈���w��F�œh��Ԃ� 
				�����y�[�W/�`���l��������ꍇ�͕\�����̂��̂Ŏ��{
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::FloodFillOnImage(long image_num, int x, int y, double val)
{
	flood_fill_on_image(this, image_num, CPoint(x,y), val, false);
}

/********************************************************************
�@  �\  ��  �� : �}�X�N��������ŗ̈�h��Ԃ��������Ȃ�
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 int		x,y			(in)�n�_���W
				 BYTE		val			(in)�`��l�@1:�}�X�N���� 0:�}�X�N�Ȃ� ����ȊO�̒l�F�}�X�N�Ȃ�
��    ��    �l : 
�@          �\ :
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::FloodFillOnMask(long image_num, int x, int y, BYTE val)
{
	flood_fill_on_image(this, image_num, CPoint(x,y), val, true);
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̉摜�f�[�^���w��F�ɓh��Ԃ��iRGB�^�ARGB 3D�^�̉摜�������ɑΉ��j
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 COLORREF	col			(in)�h��Ԃ��F
��    ��    �l : 
�@          �\ :�n�_���W�Ɠ����F�ł���̈���w��F�œh��Ԃ� 
				�����y�[�W/�`���l��������ꍇ�͕\�����̂��̂Ŏ��{
				�O���[�摜�����݂��郁�����ɑ΂��Ă��̊֐����Ă񂾏ꍇ��, �Ԑ�������f�l�Ƃ��Ďg��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//RGB�^�ARGB 3D�^�̉摜�������ɑΉ�
void	CPimpomAPI::FillWorkAreaOnImage(long image_num, COLORREF col)
{
	fill_workarea_on_image(this, image_num, col, false);
}


/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̉摜�f�[�^���w��F�ɓh��Ԃ��i�O���[�X�P�[���̉摜�������ɑΉ��j
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 double		val			(in)�h��Ԃ��l
��    ��    �l : 
�@          �\ :�n�_���W�Ɠ����F�ł���̈���w��F�œh��Ԃ� 
				�����y�[�W/�`���l��������ꍇ�͕\�����̂��̂Ŏ��{
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
//RGB�^�ARGB 3D�^�ȊO�̉摜�������ɑΉ�
void	CPimpomAPI::FillWorkAreaOnImage(long image_num, double val)
{
	fill_workarea_on_image(this, image_num, val, false);
}

/********************************************************************
�@  �\  ��  �� : ��Ɨ̈�̃}�X�N�f�[�^��h��Ԃ�
��    ��    �� : DrawPointOnImage
��          �� : int		image_num	(in)�摜�������ԍ�
				 BYTE		val			(in)�`��l�@1:�}�X�N���� 0:�}�X�N�Ȃ� ����ȊO�̒l�F�}�X�N�Ȃ�
��    ��    �l : 
�@          �\ :
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::FillWorkAreaOnMask(long image_num, BYTE val)
{
	fill_workarea_on_image(this, image_num, val, true);
}


/********************************************************************
�@  �\  ��  �� : ���m�N���摜���J���[�摜�ɂ���
��    ��    �� : PrepeareDrawing
��          �� : int num�@			(in)�`�悷��摜�������ԍ�
				 int copy_src_num	(in)�w�i�摜�̉摜�������ԍ�
��    ��    �l : 
�@          �\ : copy_src_num�̃������ɓ����Ă���摜��w�i�Ƃ���num�ɕ`��p��RGB�摜��V�K��������
				 �}�`�`��O�ɂ��̊֐����Ăяo���Ă���
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::PrepeareDrawing(int num, int copy_src_num)
{
	if( !CopyDataUnit(copy_src_num, num) )	return false;

	if( !ConvertDataUnit(num, RGB_FORMAT, 0, 0, true, false, 0 ))	return false;

	return true;
}



/********************************************************************
�@  �\  ��  �� : �摜��������ɕʂ̉摜��`�悷��
��    ��    �� : DrawImageOnImage
��          �� : long			image_num		(in)���n�Ƃ���摜�������ԍ�
				 long			src_image_num	(in)�`�悷��摜�f�[�^���������摜�������ԍ�
				 int			offsetX,		(in)�`��I�t�Z�b�gx���W
				 int			offsetY,		(in)�`��I�t�Z�b�gy���W
				 int			mixType			(in)�f�[�^�̍������@ 1:max, 2:min, other:����
��    ��    �l : 
�@          �\ : image_num�̃������ɓ����Ă���摜��w�i�Ƃ���src_image_num�ɂɓ����Ă���摜����������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void	CPimpomAPI::DrawImageOnImage(long number, long src_image_num, int offsetX, int offsetY, int mixType)
{
	CDataUnit *p_canvas_du;
	CDataUnit *p_draw_du;

	if( (p_canvas_du = GetDataUnit(number))==NULL)	return;
	if( (p_draw_du = GetDataUnit(src_image_num))==NULL)	return;


	if(  p_canvas_du->DataType == RGB_FORMAT || p_canvas_du->DataType == RGB_3D_FORMAT  )
	{
		p_canvas_du->DispRGB = true;

		if(p_draw_du->DataType == RGB_FORMAT || p_draw_du->DataType == RGB_3D_FORMAT)
		{//RGB->RGB
			p_draw_du->DispRGB = true;
			for(int src_y=0 ; src_y<p_draw_du->DataSize.cy ; src_y++)
			{
				for(int src_x=0 ; src_x<p_draw_du->DataSize.cx ; src_x++)
				{
					int dst_x = src_x + offsetX;
					int dst_y = src_y + offsetY;

					if(p_canvas_du->WorkArea.PtInWorkArea(CPoint(dst_x,dst_y)))
					{
						for(int c=0 ; c<3 ; c++){
							double baseVal = p_canvas_du->GetDataValue(dst_x,dst_y,-1,c);
							double drawVal = p_draw_du->GetDataValue(src_x,src_y,-1,c);

							if(mixType==1){//max
								p_canvas_du->SetDataValue(dst_x, dst_y, BIGGER(baseVal,drawVal), -1, c, true);
							}else if(mixType==2){//min
								p_canvas_du->SetDataValue(dst_x, dst_y, SMALLER(baseVal,drawVal), -1, c, true);
							}else{//average
								p_canvas_du->SetDataValue(dst_x, dst_y, (baseVal+drawVal)/2.0, -1, c, true);
							}
						}
					}
				}
			}
		}
		else
		{//RGB�ȊO->RGB
			for(double src_y=0 ; src_y<p_draw_du->DataSize.cy ; src_y++)
			{
				for(double src_x=0 ; src_x<p_draw_du->DataSize.cx ; src_x++)
				{
					double dst_x = src_x + offsetX;
					double dst_y = src_y + offsetY;

					if(p_canvas_du->WorkArea.PtInWorkArea(CPoint(dst_x,dst_y)))
					{
						double baseVal = p_canvas_du->GetDataValue(dst_x,dst_y,-1,0);
						double drawVal = p_draw_du->GetDataValue(src_x,src_y,-1,0);
						double val;

						if(mixType==1){//max
							val = BIGGER(baseVal,drawVal);
						}else if(mixType==2){//min
							val = SMALLER(baseVal,drawVal);
						}else{//average
							val = (baseVal+drawVal)/2.0;
						}
					
						p_canvas_du->SetDataValue(dst_x, dst_y, val, -1, 0, true);//r
						p_canvas_du->SetDataValue(dst_x, dst_y, val, -1, 1, true);//g
						p_canvas_du->SetDataValue(dst_x, dst_y, val, -1, 2, true);//b
					}
				}
			}
		}
	}
	else
	{
		if(p_draw_du->DataType == RGB_FORMAT || p_draw_du->DataType == RGB_3D_FORMAT)
		{//RGB->RGB�ȊO
			p_draw_du->DispRGB = true;
		}
			
		for(double src_y=0 ; src_y<p_draw_du->DataSize.cy ; src_y++)
		{
			for(double src_x=0 ; src_x<p_draw_du->DataSize.cx ; src_x++)
			{
				double dst_x = src_x + offsetX;
				double dst_y = src_y + offsetY;

				if(p_canvas_du->WorkArea.PtInWorkArea(CPoint(dst_x,dst_y)))
				{
					double baseVal = p_canvas_du->GetDataValue(dst_x,dst_y,-1,0);
					double drawVal = p_draw_du->GetDataValue(src_x,src_y,-1,0);
					double val;

					if(mixType==1){//max
						val = BIGGER(baseVal,drawVal);
					}else if(mixType==2){//min
						val = SMALLER(baseVal,drawVal);
					}else{//average
						val = (baseVal+drawVal)/2.0;
					}
					p_canvas_du->SetDataValue(dst_x,dst_y, val, -1, -1, true);
				}
			}
		}
	}
}