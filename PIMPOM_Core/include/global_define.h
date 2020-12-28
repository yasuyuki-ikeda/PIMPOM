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
//PIMPOM�S�̂Ŏg�p�\�ȃ}�N�����`����
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <LIMITS.H>
#include <FLOAT.H>
#include <winuser.h>


///*************************************
//         �}  �N  ��   ��   �`         
///*************************************

//�h�c�̒�`�i������ׂ��炸�j//////////////////////////////////////////////////
//�p���b�g�̂h�c
#define		PALETTE_GRAYSCALE			(0)
#define		PALETTE_NEGATIVE			(1)
#define		PALETTE_RAINBOW				(2)
#define		PALETTE_BLACK_RAINBOW		(3)
#define		PALETTE_COSINE				(4)
#define		PALETTE_BLUE_YELLOW			(5)
#define		PALETTE_GREEN_RED			(6)
#define		PALETTE_LABEL				(7)


//�f�[�^�̎��
#define		BYTE_FORMAT			(0)
#define		FLOAT_FORMAT		(1)
#define		COMPLEX_FORMAT		(2)
#define		THREE_D_FORMAT		(3)
#define		FLOAT_3D_FORMAT		(4)
#define		XY_FORMAT			(5)
#define		XYZ_FORMAT			(6)
#define		RGB_FORMAT			(7)
#define		WORD_FORMAT			(8)
#define		LONG32_FORMAT		(9)		//Halcon�Ƌ������Ȃ��悤��
#define		RGB_3D_FORMAT		(10)
#define		SHORT16_FORMAT		(11)
#define		DWORD_FORMAT		(12)

//�f�[�^�R���o�[�g
#define		COMP_TO_REAL	(0)
#define		COMP_TO_IMAG	(1)
#define		RGB_TO_ALL	(0)
#define		RGB_TO_R	(1)
#define		RGB_TO_G	(2)
#define		RGB_TO_B	(3)

//���ݑI�𒆂̉摜��������\��
#define		CURRENT_IMAGE		(-1)

//�}�X�N
#define		MASKED_PIXEL				(1)
#define		UNMASKED_PIXEL				(0)		

#define		CUSTOM_MENU_ID				(9999)
#define		FILE_IO_LOG_MENU_ID			(9900)

//��Ɨ̈�̎��
#define		WORK_AREA_TYPE_NOTHING		(-1)
#define		WORK_AREA_TYPE_RECT			(0)
#define		WORK_AREA_TYPE_LINE			(1)
#define		WORK_AREA_TYPE_ELLIPSE		(2)
#define		WORK_AREA_TYPE_CIRCLE		(3)
#define		WORK_AREA_TYPE_ARC			(4)
#define		WORK_AREA_TYPE_POLYGON		(5)

//�}�[�N�̃^�C�v
#define		MARK_TYPE_NOTHING		(-1)
#define		MARK_TYPE_POINT			(0)
#define		MARK_TYPE_LINE			(1)



//�o�b�`���s����
#define		BATCH_EXEC_STATE_NORMAL		(0)//�ʏ���s���ꂽ
#define		BATCH_EXEC_STATE_SKIPPED	(1)//�������X�L�b�v���ꂽ
#define		BATCH_EXEC_STATE_RETRY		(2)//���g���C�v��
#define		BATCH_EXEC_STATE_STOP		(3)//���f�v��

//�ʐM�̎��
#define		SOCKET_SERVER		(0)	//�T�[�o�\�P�b�g
#define		SOCKET_CLIENT		(1)	//�N���C�A���g�\�P�b�g
#define		SOCKET_SERVER_SUB	(2)

//�o�b�N�O���E���h�����i�ʃX���b�h�j��ID
#define		BGPROC_NOTHING		(0)
#define		BGPROC_CAPTURING	(1)
#define		BGPROC_MOVIE		(2)
#define		BGPROC_BATCH_EXC	(3)

//���L���������g�����f�[�^�̓���
#define	 SHEAREMEM_NAME		"PIMPOM_SHAREDMEM"
#define  SHEAREMEM_HEADER_SIZE		(20)


#define		FILE_PATH_LEN_MAX			(2048)	//�t�@�C���p�X�̍ő咷
#define		MULTI_FILE_PATH_BUFFER		(500000)	//CFileDialog�̃o�b�t�@

//�����萔�̐ݒ�////////////////////////////////////////////////////////////////


#define		MAIN_W		(640)					//���C���C���[�W�̕��̃f�t�H���g�l
#define		MAIN_H		(480)					//���C���C���[�W�̍����̃f�t�H���g�l


//���C���E�B���h�E(MainDlg)
#define		MAIN_WIN_LEFT_MARGIN	(0)		//���C���C���[�W�\���̈悩�烁�C���E�B���h�E���[�܂ł̃}�[�W��
#define		MAIN_WIN_TOP_MARGIN		(0)		//���C���C���[�W�\���̈悩�烁�C���E�B���h�E��[�܂ł̃}�[�W��
#define		MAIN_WIN_RIGHT_MARGIN	(50)	//���C���C���[�W�\���̈悩�烁�C���E�B���h�E�E�[�܂ł̃}�[�W��
#define		MAIN_WIN_BOTTOM_MARGIN	(160)	//���C���C���[�W�\���̈悩�烁�C���E�B���h�E���[�܂ł̃}�[�W��

#define		DATA_SELECT_FIELD_Y		(0)		//�f�[�^�̕\���^�C�v�����߂�{�^����z�u����̈�̍���̂����W
#define		DATA_SELECT_FIELD_W		(25)		//�f�[�^�̕\���^�C�v�����߂�{�^����z�u����̈�̕�
#define		DATA_SELECT_FIELD_H		(100)		//�f�[�^�̕\���^�C�v�����߂�{�^����z�u����̈�̍���

#define		LAYER_SELECT_FIELD_Y		(130)		//���C���[�̕\���^�C�v�����߂�{�^����z�u����̈�̍���̂����W
#define		LAYER_SELECT_FIELD_W		(25)		//���C���[�̕\���^�C�v�����߂�{�^����z�u����̈�̕�
#define		LAYER_SELECT_FIELD_H		(125)		//���C���[�̕\���^�C�v�����߂�{�^����z�u����̈�̍���

#define		DATA_VALUE_DISP_FIELD_X		(0)			//�f�[�^�̒l��\������̈�̍���̂����W
#define		DATA_VALUE_DISP_FIELD_W		(400)		//�f�[�^�̒l��\������̈�̕�
#define		DATA_VALUE_DISP_FIELD_H		(80)		//�f�[�^�̒l��\������̈�̍���

#define		MESSAGE_DISP_FIELD_X		(270)		//���b�Z�[�W��\������̈�̍���̂����W
#define		MESSAGE_DISP_FIELD_H		(150)		//���b�Z�[�W��\������̈�̍���

#define		DATA_NUMBER_DEFAULT		(100)	//�摜���������̃f�t�H���g�l
#define		DATA_NUMBER_MAX			(1000)	//�摜���������̍ő�l

#define		MARK_NUMBER_MAX_OLD		(256)	//�}�[�J�[�ő�l
#define		MARK_NUMBER_MAX			(2000)	//�}�[�J�[�ő�l

#define		CUSTOM_FUNC_MAX			(10)	//�J�X�^���t�@���N�V�����ő吔

#define		FILE_IO_LOG_MAX	(10)

#define		WORKAREA_EDIT_MARGIN	(10)	//��Ɨ̈�ҏW���ɂ܂�ő��삷��̈�̕�

//�f�[�^�E�B���h�E(DataDlg)
#define		DATA_WIN_W		(1150)				//�f�[�^�E�B���h�E�̕�
#define		DATA_WIN_H		(200)				//�f�[�^�E�B���h�E�̍���

#define		SMALL_W		(40)					//���C���[�W�̕��̃f�t�H���g�l
#define		SMALL_H		(30)					//���C���[�W�̍����̃f�t�H���g�l
#define		SMALL_MARGN_X	(15)				//���C���[�W�̉��Ԋu
#define		SMALL_MARGN_Y	(20)				//���C���[�W�̏c�Ԋu
#define     SMALL_COLUM  (20)					//���C���[�W�̕\���s��

//�q�X�g�O�����E�C���h�E(HistgramDlg)
#define		HISTGRAM_WIN_W		(340)			//�q�X�g�O�����E�C���h�E�̕�
#define		HISTGRAM_WIN_H		(320)			//�q�X�g�O�����E�C���h�E�̍���

#define		HISTGRAM_W		(256)				//�q�X�g�O�����̕�
#define		HISTGRAM_H		(150)				//�q�X�g�O�����̍���

//�}�[�N�E�C���h�E(MarkDlg)
#define		MARK_WIN_W		(330)			//�}�[�N�E�C���h�E�̕�
#define		MARK_WIN_H		(280)			//�}�[�N�E�C���h�E�̍���
#define		MARK_ATTRIB_NUM	(3)				//�}�[�N�̑�����

//��Ɨ̈�E�C���h�E(WorkAreaDlg)
#define		WORKAREA_WIN_W		(330)			//�E�C���h�E�̕�
#define		WORKAREA_WIN_H		(280)			//�E�C���h�E�̍���
#define		WORKAREA_FIG_NUM	(10)			//��Ɨ̐}�`��

//�_�S�\��
#define		POINT_CLOUD_W	(1000)		//�_�S�\������X�N���[���̕�
#define		POINT_CLOUD_H	(740)		//�_�S�\������X�N���[���̍���


//�t�H���g�̃T�C�Y
#define     FONT_SIZE       (11)

//�ʐM
#define		RECIVE_BUFFER_SIZE		(10000)	//��M�o�b�t�@�T�C�Y
#define		CALLBACK_FUNC_NUM		(2)	//�o�^�ł���R�[���o�b�N�֐��̍ő吔

//�o�b�`���s
#define		BATCH_IN_PARAM_NUM		(5)	//�o�b�`���s�̓��̓p�����^��
#define		BATCH_OUT_PARAM_NUM		(15)	//�o�b�`���s�̏o�̓p�����^��

//�O���t�\��
#define		CHART_NUM			(10)//�O���t�̐�
#define		CHART_DATA_NUM		(10000)//1�̃O���t�\������f�[�^�_�̐�
#define		CHART_HISTGRAM_SPLIT_MAX	(500)//�q�X�g�O�����x��������

//���w�I�萔�̐ݒ�//////////////////////////////////////////////////////////////

#define		PI			(3.141592653589793238)				//�~����

/////////////////////////////////////////////////////////////////////////////////
#define		CR		(0x0d)
#define		LF		(0x0a)

//�t�B���^�֘A///////////////////////////////////////////////////////////////////
#define		COMB_FILTER_SIZE_MAX		(61)	//��݂��݃t�B���^�̍ő�T�C�Y
#define		LANK_FILTER_SIZE_MAX		(21)	//���ʕt���t�B���^�̍ő�T�C�Y


#define		BINALIZE_LABEL_MAX			(9999)	//�ő僉�x����