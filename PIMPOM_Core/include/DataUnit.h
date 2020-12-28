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
//CDataUnit
//�摜�f�[�^�𓝍��I�Ɉ����N���X
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "global_define.h"
#include "global_typedef.h"

///*************************************
//         �N���X   ��   �`         
///*************************************
class AFX_EXT_CLASS CWorkArea
{
public:
	int					Type[WORKAREA_FIG_NUM];//�}�`�̃^�C�v
	bool				OrNot[WORKAREA_FIG_NUM];
	FIGURE_UNION		Figure[WORKAREA_FIG_NUM];//�}�`
	int					FigureEditMode[WORKAREA_FIG_NUM];//�}�`�ҏW���[�h 0:�������� 1:�S�̈ړ�  2�ȍ~:���_�ړ�

	void			Initialize(CSize image_size);
	void			Initialize(CSize image_size, int no, int type, bool ornot);//��Ɨ̈������������
	bool			PtInWorkArea(CPoint point);//�w�肳�ꂽ���W����Ɨ̈���ɓ����Ă��邩�`�F�b�N����
	bool			PtInWorkArea(CPoint point, int no);//�w�肳�ꂽ���W���w�肳�ꂽ�ԍ��̍�Ɨ̈���ɓ����Ă��邩�`�F�b�N����
	void			Copy(CWorkArea *area);//��Ɨ̈���R�s�[����
	CRect			GetCircumRect();//�S�̈�ɊO�ڂ����`�����߂�
	CRect			GetCircumRect(int no);//�w�肳�ꂽ�ԍ��̍�p�̈�ɊO�ڂ����`�����߂�
	bool			Move(int dx, int dy);//���ׂĂ̍�Ɨ̈�𕽍s�ړ�������
	bool			Move(int no, int dx, int dy);//�w�肳�ꂽ�ԍ��̍�Ɨ̈�𕽍s�ړ�������
	bool			Scale(int no, float scalex, float scaley, CPoint offset);//�X�P�[�����O����
	static void		GetLineVertxes(FIGURE_LINE line, CPoint point[4]);//���������̂S���_�����߂�

	int				GetWorkArea(int no, FIGURE_UNION *fig, bool *ornot);
	bool			SetWorkArea(int no, RECT rect, bool ornot);
	bool			SetWorkArea(int no, FIGURE_LINE line, bool ornot);
	bool			SetWorkArea(int no, FIGURE_ELLIPSE ellipse, bool ornot);
	bool			SetWorkArea(int no, FIGURE_CIRCLE circle, bool ornot);
	bool			SetWorkArea(int no, FIGURE_ARC arc, bool ornot);


};


class AFX_EXT_CLASS CMark
{
public:
	short			type;
	FIGURE_UNION	figure;
	//float	x;//X���W
	//float	y;//Y���W
	double	attrib[MARK_ATTRIB_NUM];//����


	//�R���X�g���N�^
	CMark();
	CMark(float x, float y);
	CMark(float x1, float y1, float x2, float y2);
	virtual ~CMark();//�f�X�g���N�^
};


class AFX_EXT_CLASS CDataUnit  
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�ϐ�
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	///�f�[�^�Ɋւ���ϐ�
	long			DataType;//�摜�f�[�^�̌^
	CString			DataName;//�f�[�^��
	CString			SrcFilePath;//�t�@�C���p�X(�t�@�C������ǂݍ��܂ꂽ�摜�ɑ΂���)
	CSize			DataSize;//�摜�f�[�^�̍����^��
	BYTE			*pByteData;//�摜�f�[�^�i8�r�b�g�����^�j�̐擪�|�C���^
	short			*pShortData;//�摜�f�[�^�i��������16�r�b�g�����j�̐擪�|�C���^
	WORD			*pWordData;//�摜�f�[�^�i�����Ȃ�16�r�b�g�����j�̐擪�|�C���^
	long			*pLongData;//�摜�f�[�^�i��������32�r�b�g����)�̐擪�|�C���^
	DWORD			*pDwordData;//�摜�f�[�^�i�����Ȃ�32�r�b�g����)�̐擪�|�C���^
	float			*pFloatData;//�摜�f�[�^�i32�r�b�g���������_�j�̐擪�|�C���^
	long			PageNumber;//�摜�f�[�^�̃y�[�W��
	long			ChannelNumber;//�摜�f�[�^�̃`���l����
	float			Average;//�摜�f�[�^�̕���
	float			Stdiv;//�摜�f�[�^�̕W���΍�
	bool			complex_image_power_log;//���f���^�f�[�^�̃p���[�X�y�N�g����log���Ƃ�

	///�\���摜�Ɋւ���ϐ�
	BYTE			*pDispBuffer;//�\���摜�i8bit���m�N���j
	BYTE			*pDispBufferRGB;//�\���摜�i24bit�J���[�j
	BITMAPINFO		*pDispBufferBmpInfo;//�\���摜(8bit)�̃r�b�g�}�b�v���
	BITMAPINFO		*pDispBufferBmpInfoRGB;//�\���摜(24bit)�̃r�b�g�}�b�v�̏��
	long			DispPage;//�\���y�[�W
	long			DispChannel;//�\���`���l��
	bool			DispRGB;//RGB�J���[�̕\��
	long			DispBufferRow;//�\���摜(8bit)�̃r�b�g�񕝁i�K���S�̔{���łȂ��Ă͂Ȃ�Ȃ��j
	long			DispBufferRowRGB;//�\���摜(24bit)�̃r�b�g�񕝁i�K���S�̔{���łȂ��Ă͂Ȃ�Ȃ��j
	float			BrightMax;//�\���Z�x�̍ő�l
	float			BrightMin;//�\���Z�x�̍ŏ��l
	float			DispScale;//�g�嗦
	CPoint			DataDispOffset;//�摜�̍���̍��W
	long			BrightLevel[256];//�\���Z�x���x���̕��z

	///��Ɨ̈�Ɋւ���ϐ�
	CWorkArea		WorkArea;//��Ɨ̈�
	bool			DoUseWorkArea;//��Ɨ̈�g�p�t���O

	///�}�X�N�Ɋւ���ϐ�
	BYTE			*pMaskData;	//�}�X�N�f�[�^			�}�X�N�������� : MASKED_PIXEL	 		�}�X�N�������Ȃ� : UNMASKED_PIXEL
	bool			DoUseMask;//�}�X�N�g�p�t���O

	//�}�[�N�Ɋւ���ϐ�
	long			MarkNum;//�}�[�N��
	CMark			*pMarkMap[MARK_NUMBER_MAX];//�}�[�N�̃|�C���^


protected:
	void set_bmp_info( CSize size );//BITMAPINFO������������



public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�֐�
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�����E�j��
	CDataUnit();
	CDataUnit(long type, CSize size, long page);//�R���X�g���N�^
	virtual ~CDataUnit();//�f�X�g���N�^
	void Destroy();
	bool Format(long type, CSize size, long page);


	//�R�s�[�E��r
	bool Copy(CDataUnit *p_src_du);
	bool Copy(CDataUnit *p_src_du, bool do_copy_data, bool do_copy_mask, bool do_copy_workarea,
					 bool do_copy_range, bool do_copy_mark, bool do_copy_pos_scale);//�摜�f�[�^���R�s�[����
	bool Compare( CDataUnit *p_du );//�摜�f�[�^�̌^�ƃT�C�Y���r���� (�����Ȃ�true��Ԃ��@�قȂ��false��Ԃ�)
	bool Compare(long type, CSize size, long page);
	


	//�f�[�^�̎擾�E�ݒ�
	unsigned char	GetDisplayIntensity(CPoint point);//�w�肵����f�̕\���Z�x���擾����
	double			GetDataValue(CPoint point, long page=-1, int channel=-1);//�w�肵�����W�̉摜�f�[�^�l���擾����
	double			GetDataValue(double x, double y, long page=-1, int channel=-1);//�w�肵�����W�̉摜�f�[�^�l���擾����
	BYTE			GetMaskValue(CPoint point);////�w�肵�����W�̃}�X�N�l���擾����
	bool			SetDataValue(int x, int y, double data, long page, int channel, bool satulate=true);//�w�肳�ꂽ���W�̉摜�f�[�^�l��ݒ肷��
	bool			SetMaskValue(int x, int y, BYTE mask);//�w�肳�ꂽ���W�̃}�X�N�l��ݒ肷��
	void			ClearMask();//�}�X�N�N���A
	void			InitWorkArea();//��Ɨ̈�̏�����
	bool			SetMaskByWorkArea();//��Ɨ̈�O���Ƀ}�X�N��������
	CRect			GetRectArea();//��Ɨ̈�ɊO�ڂ����`�̈���擾����
	void			CreateComplexPolarComponents();//�������W�����i�����C�����j����ɍ��W�����́i�p���[�C�ʑ��j�����߂�i���f���^�f�[�^�̂��߂̏����j
	void			CreateComplexDescartesComponents();//�ɍ��W�������璼�����W���������߂�i���f���^�f�[�^�̂��߂̏����j
	double			GetMaxLimitValue();//�f�[�^�̏���l
	double			GetMinLimitValue();//�f�[�^�̉����l
	bool			Convert(CONVERT_DATA_OPTION option);//�f�[�^�̌^��ϊ�����
	int				AddMark(CMark mark);
	bool			DeleteMark(long idx);//�}�[�N���폜����
	void			ClearMark();//�}�[�N��S�폜����
	bool			GetMark(long idx, CMark *pmark);//�}�[�N�̍��W���擾����
	long			GetMarkNumber();//�}�[�N�̐����擾����
	bool			SetMark(long idx, CMark mark);//

	//�摜�\��
	void			UpdateDispBuffer();//�\���摜���X�V����
	void			GetDataMinMax(float *min, float *max);//�摜�f�[�^�̍ő�^�ŏ����擾
	bool			SetDispRange(float min, float max);//�\���Z�x�̍ő�^�ŏ����w�肷��
	void			SetDispScale(float scale);//�g�嗦��ύX����
	bool			MoveDispPage(int offset);//�\���y�[�W�����݂���w��ʂ����ړ�������
	bool			SetDispPage(int page);//�\���y�[�W��ݒ肷��
	bool			SetDispChannel(int channel);//�\���`���l����ݒ肷��
	void			ChangePalette(short palette_id);//�J���[�p���b�g��ύX����


};

