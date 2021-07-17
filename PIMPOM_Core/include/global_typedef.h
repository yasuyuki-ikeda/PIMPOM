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
//PIMPOM�S�̂Ŏg�p�\�ȍ\���̂��`����
/*******************************************************************/
#pragma once

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

typedef struct {			/*  (x - x0)/dx = (y - y0)/dy = (z - z0)/dz  */
	double dx;
	double dy;
	double dz;
	double x0;
	double y0;
	double z0;
} Line1;


typedef struct{
	double	x;
	double	y;
	double	z;
}COOR_XYZ;

typedef struct{
	double	i;
	double	j;
}COOR_IJ;


typedef struct{
	int type_to;//�ϊ���̉摜�������̌^
	int rgb_to;//RGB�^�ɕϊ�����ꍇ�̓��͐� �i1:R���� 2:G���� 3:B���� 1�`3�ȊO:RGB�S�����j 
	int comp_to;//���f���^�̑}����i0:�����@0�ȊO:�����j
	bool satulate;//�^���̏㉺���𒴂���l��ϊ�����Ƃ��O�a�����邩�ǂ����i0�ȊO�F�O�a������@0:�I�[�o�[�t���[������j
	int byte_from;//type_to��BYTE�^�̂Ƃ��A�\��������or�}�X�N����������ϊ����邩�ǂ����i0:�f�[�^���� 1:�\������������ 2:�}�X�N���� 3:�^���J���[����j
	int color_gray_method;//RGB->�O���[�ϊ����@ (1:�ʓx 2:�F��  �����ȊO:���x)
	int  threeD_page;//3D�ɕϊ�����Ƃ��̃y�[�W��
	int  threeD_offset;//3D�ɕϊ�����Ƃ��̃y�[�W�I�t�Z�b�g
}CONVERT_DATA_OPTION;


typedef struct{
	float		x;				// ��P�_�w���W
	float		y;				// ��P�_�x���W
}FIGURE_POINT;

typedef struct{
	float		left;				// ��P�_�w���W
	float		top;				// ��P�_�x���W
	float		right;				// ��Q�_�w���W
	float		bottom;				// ��Q�_�x���W
}FIGURE_RECT;

typedef struct{
	float		x1;					// ��P�_�w���W
	float		y1;					// ��P�_�x���W
	float		x2;					// ��Q�_�w���W
	float		y2;					// ��Q�_�x���W
	float		w;					// ��
}FIGURE_LINE;

typedef struct{
	float		cx;					// �ȉ~�̒��S�_�w���W
	float		cy;					// �ȉ~�̒��S�_�x���W
	float		rx;					// �ȉ~�̂w�������a
	float		ry;					// �ȉ~�̂w�������a
}FIGURE_ELLIPSE;


typedef struct {
	float		cx;					// �~�̒��S�_�w���W
	float		cy;					// �~�̒��S�_�x���W
	float		r;					// �~�̔��a
	float		w;					// ��
}FIGURE_CIRCLE;

typedef struct{
	float		cx;					// �~�ʂ̒��S�_�w���W
	float		cy;					// �~�ʂ̒��S�_�x���W
	float		r;					// �~�ʂ̔��a
	float		start;				// �~�ʂ̊J�n�p�x
	float		end;				// �~�ʂ̊J�n�p�x
	float		w;					// ��
}FIGURE_ARC;

typedef struct{
	int				count;				// ���_�̐�
	FIGURE_POINT	point[10];			// ���_�̍��W�z��
}FIGURE_POLYGON;


typedef union{
	FIGURE_POINT	point;
	FIGURE_RECT		rect;//��`��Ɨ̈�
	FIGURE_LINE		line;//����̈�
	FIGURE_ELLIPSE	ellipse;//�ȉ~��̈�
	FIGURE_CIRCLE	circle;//�~����̈�
	FIGURE_ARC		arc;//�~�ʏ�̈�
	FIGURE_POLYGON	polygon;//���p�`��̈�
}FIGURE_UNION;



typedef struct {

	long			DataType;//�f�[�^�̃^�C�v
	char			DataName[1024];//�f�[�^��
	long			DataSizeX;//�f�[�^�̃T�C�Y
	long			DataSizeY;//�f�[�^�̃T�C�Y
	long			PageNumber;//�f�[�^�̉��s��
	long			ChannelNumber;//1��f�̃f�[�^��
	long			DispPage;//�\���y�[�W
	long			DispChannel;//�\���`���l��
	bool			DispRGB;//RGB�J���[���̕\��

	long			MarkNum;//�}�[�N�� �F MARK_NUMBER_MAX�����邱��
	long			MarkX[MARK_NUMBER_MAX_OLD];//�}�[�N�̍��W�F���̐ݒ�͌Â�����g��Ȃ�����
	long			MarkY[MARK_NUMBER_MAX_OLD];//�}�[�N�̍��W�F���̐ݒ�͌Â�����g��Ȃ�����
	

	float			BrightMax;//�\���Z�x�̍ő�l
	float			BrightMin;//�\���Z�x�̍ŏ��l

	float			DispScale;//�g�嗦
	long			DataDispOffsetX;//�摜�]�����̍���̍��W
	long			DataDispOffsetY;//�摜�]�����̍���̍��W

	int				WorkAreaType[WORKAREA_FIG_NUM];//��Ɨ̈�̐}�`�^�C�v
	bool			WorkAreaOrNot[WORKAREA_FIG_NUM];//��Ɨ̈�OR/NOT���
	FIGURE_UNION		WorkAreaFig[WORKAREA_FIG_NUM];//��Ɨ̈�̐}�`
	//RECT			WorkArea;//��Ɨ̈�
	//FIGURE_LINE		WorkAreaLine;
	//FIGURE_ELLIPSE	WorkAreaEllipse;
	//FIGURE_CIRCLE		WorkAreaCircle;
	//FIGURE_ARC		WorkAreaArc;
	//WORKAREA_POLYGON	WorkAreaPolygon;
	bool			DoUseWorkArea;//��Ɨ̈���g�����ǂ�����\���t���O

	bool			DoUseMask;//�}�X�N�������邩�ǂ�����\���t���O


} DATA_UNIT_INFO;


//�o�b�`���s���̓��̓p�����^
typedef struct{
	char	param[BATCH_IN_PARAM_NUM][1024];//���̓p�����^
	char	imae_file_path[FILE_PATH_LEN_MAX];//���͉摜�̃t�@�C����
	int		image_number;//���͉摜�������Ă��郁�����ԍ�
	bool	AutoExecFlg;//�A�����s���ł��邱�Ƃ�����
}BATCH_INPUT;


//�o�b�`���s���̏o�̓p�����^
typedef struct{
	char	result[BATCH_OUT_PARAM_NUM][1024];//�o�̓p�����^
	int		state; //0:�ʏ폈�� 1:�������΂��� 2:���������g���C 3:�����I��
}BATCH_OUTPUT;
