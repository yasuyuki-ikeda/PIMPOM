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
//PIMPOM�S�̂ŋ��ʓI�Ɏg���ϐ��E�֐��������Ŏ�������
/*******************************************************************/
#pragma once

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "global_define.h"
#include "global_typedef.h"

#include "resource_core.h"


#include "DataUnit.h"
#include "Calculate.h"
#include "SocketCtrl.h"
#include "PIMPOMDlg.h"
#include "DataDlg.h"
#include "HistgramDlg.h"
#include "PointCloudDlg.h"
#include "ConvertDataDlg.h"
#include "ResizeDataDlg.h"
#include "TransformDlg.h"
#include "ConfirmDlg.h"
#include "CalcDlg1.h"
#include "MarkDlg.h"
#include "IO3DDlg.h"
#include "FilterCtrlDlg.h"
#include "ConvertDataDlg.h"
#include "PointCloudDlg.h"
#include "DataPickDlg.h"
#include "BatchDlg.h"
#include "MultiImageDispDlg.h"
#include "C3DImgDispDlg.h"
#include "WorkAreaDlg.h"
#include "SocketCtrlDlg.h"
#include "BinalizeDlg.h"
#include "CommandPromptDlg.h"
#include "ChartDlg.h"
#include "PythonBatchDlg.h"



///*************************************
//         �\����   ��   �`         
///*************************************
typedef struct{
	CString			dllname;
	CString			caption;
	HINSTANCE		hInst;
}CUSTOM_FUNC;

typedef struct
{
	BYTE *pBytes;
	int	 width;
	int  height;
}BITMAP_FONT;

///*************************************
//         �N���X   ��   �`         
///*************************************


class AFX_EXT_CLASS CPimpomAPI
{
public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//PIMPOM�S�̂Ŏg����ϐ�
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�W���̃��[�h���X�_�C�A���O�ւ̃|�C���^
	CPIMPOMDlg		*pPIMMOMDlg;//���C���E�C���h�E�̃|�C���^
	CDataDlg		*pDataDlg;//�f�[�^�E�C���h�E�̃|�C���^
	CHistgramDlg	*pHistgramDlg;//�q�X�g�O�����E�C���h�E�̃|�C���^
	CFilterCtrlDlg  *pFlterCtrlDlg;//�ėp�t�B���^�E�B���h�E�̃|�C���^
	CCalcDlg		*pCalcDlg;//�v�Z�ݒ�p�E�C���h�E�̃|�C���^
	CBinalizeDlg	*pBinalizeDlg;//2�l���E�B���h�E�̃|�C���^
	CBatchDlg		*pBatchDlg;//�o�b�`�����E�B���h�E�̃|�C���^
	CMultiImageDispDlg	*pMultiImageDispDlg;//�����摜�\���p�E�B���h�E�̃|�C���^
	C3DImgDispDlg	*p3DImgDispDlg;//3D�摜�̈ꗗ�\���E�B���h�E�̃|�C���^
	CWorkAreaDlg	*pWorkAreaDlg;//��Ɨ̈�ҏW�p�E�C���h�E�̃|�C���^
	CMarkDlg		*pMarkDlg;//�}�[�N�\���p�E�C���h�E�̃|�C���^
	CDataPickDlg	*pDataPickDlg;//�f�[�^�̒��ڕҏW�p�E�C���h�E�̃|�C���^
	CSocketCtrlDlg	*pSocketCtrlDlg;//�\�P�b�g�ʐM�̐ݒ�p�E�C���h�E�̃|�C���^
	CChartDlg		*pChartDlg;//�O���t�\���p�E�B���h�E�̃|�C���^
	CCommandPromptDlg	*pCmdPrptDlg;//�R�}���h�v�����v�g�E�B���h�E�̃|�C���^
	CPythonBatchDlg *pPythonBatchDlg;//Python�o�b�`�����E�B���h�E�̃|�C���^



	//���ݒ�
	CSize		main_image_size;//���C���摜�\���T�C�Y
	CSize		main_window_size;//���C���E�B���h�E�T�C�Y
	int			data_window_width;//�f�[�^�E�B���h�E�̕�
	int			thumbneil_colum;//�f�[�^�E�B���h�E�ɕ\������T���l�C���P�s�̉摜���i�����܂��̓E�B���h�E�Ɏ��܂�Ȃ��ꍇ�͋����I�ɐ܂�Ԃ��j
	LOGFONT		default_font;//�f�t�H���g�t�H���g
	COLORREF	default_bg_color;//�f�t�H���g�w�i�F
	COLORREF	default_fg_color;//�f�t�H���g�O�i�F
	int			rgb_gain[3];//�J���[�����m�N���ɕϊ�����Ƃ��̃Q�C���~1000
	int			socket_delimiter;//�\�P�b�g�ʐM���̃f���~�^�̃^�C�v(0:CRLF 1:CR 2:LF)
	CSize		small_image_size;//�摜�������ꗗ�\���E�B���h�E�ɕ\������摜�T�C�Y	
	int			invisible;//��ʕs����
	int			batch_execute_rule;//�o�b�`���s�̃��[��(0:�R�}���h�v�����v�g->�J�X�^���t�@���N�V���� 1�F�J�X�^���t�@���N�V����->�R�}���h�v�����v�g �@0,1�ȊO:�R�}���h�v�����v�g�̂�)
	int			capture_image_number;//�r�f�I�L���v�`�����ʂ��͂���摜�ԍ�
	int			pimpom_id;//PIMPOM�̃V���A��ID(�����N����)
	CString		python_batch_file;//�o�b�`���s����python�̃t�@�C���p�X

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���̃N���X�̐����E�j��
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	CPimpomAPI(void);
	~CPimpomAPI(void);
	CString			GetProfile(CString key1, CString key2);//ini�f�[�^��ǂݏo��
	void			WriteProfile(CString key1, CString key2, CString str);//ini�f�[�^��ݒ肷��
	void			InitSettingFromProfile();


	//////////////////////////////////////////////////////////////////////////////
	//�摜�������̑���
	//////////////////////////////////////////////////////////////////////////////

	//�摜�������ւ̃A�N�Z�X
	long			GetDataUnitNumber();//�摜�������̑���
	long			GetCurrentImageNumber();//���ݑI�𒆂̉摜�������̔ԍ���Ԃ�
	bool			SetCurrentImageNumber(long number);//���ݑI�𒆂̉摜�������̔ԍ���ύX����i�ĕ`��͂��Ȃ��j
	bool			SetDispPage(long number, long page);
	bool			SetDispChannel(long number, long channel);
	CDataUnit		*GetDataUnit(long number);//DataUnit��Ԃ� (�摜�������������ꍇ��NULL���Ԃ�)
	bool			GetDataTypeSize(long number, int *ptype, int *pwidth, int *pheight, int *ppages);//�摜�������̌^�E�T�C�Y�E���s���擾
	bool			GetDataMinMax(long number, float *pMin, float *pMax);//�f�[�^�̍ő�E�ŏ����擾����
	unsigned char	*GetByteMemory(long number, CSize size, bool do_clear_memory);//BYTE�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	float			*GetFloatMemory(long number, CSize size, bool do_clear_memory);//FLOAT�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	unsigned char	*Get3DMemory(long number, CSize size, long page, bool do_clear_memory);//3D�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	float			*GetF3DMemory(long number, CSize size, long page, bool do_clear_memory);//FLOAT3D�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	float			*GetComplexMemory(long number, CSize size, bool do_clear_memory);//���f���^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	float			*GetXYMemory(long number, CSize size, bool do_clear_memory);//XY�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	float			*GetXYZMemory(long number, CSize size, bool do_clear_memory);//XYZ�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	short			*GetShortMemory(long number, CSize size, bool do_clear_memory);//16bit�����t���^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	WORD			*GetWordMemory(long number, CSize size, bool do_clear_memory);//16bit�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	long			*GetLongMemory(long number, CSize size, bool do_clear_memory);//13bit�����t���^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	DWORD			*GetDwordMemory(long number, CSize size, bool do_clear_memory);//13bit�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	unsigned char	*GetRGBMemory(long number, CSize size, bool do_clear_memory);//RGB�J���[�^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	unsigned char	*GetRGB3DMemory(long number, CSize size, long page, bool do_clear_memory);//RGB�J���[����^�������̐擪�A�h���X���擾(�f�[�^�������ꍇ�͐V�K����)
	BYTE			*GetMaskMemory(long number);//�}�X�N�������̐擪�A�h���X���擾 (�f�[�^�������ꍇ��NULL���Ԃ�)
	CString			GetSrcFilePathName(long number);//�t�@�C������ǂݍ��܂ꂽ�摜�������ɑ΂��A�ǂݍ��݌��̃t�@�C���̃t���p�X��Ԃ�
	CString			GetSrcFilePath(long number);
	CString			GetSrcFileName(long number);
	CString			GetSrcFileTitle(long number);
	


	//�摜�������̐V�K�����E�R�s�[�E�폜�E����
	CDataUnit		*FormatDataUnit(long number, long type, CSize size, long page, bool do_clear_memory);//�w�肵���`���̉摜�f�[�^�̃��������擾���� (�������擾�����s�����ꍇ��NULL���Ԃ�)
	bool			CopyDataUnit(long src_number, long dest_number, bool do_copy_data, bool do_copy_mask, bool do_copy_workarea, bool do_copy_range, bool do_copy_mark, bool do_copy_pos_scale);//�摜���������R�s�[����
	bool			CopyDataUnit(long src_number, long dest_number, bool show_confirm=false);//�摜���������R�s�[����
	bool			DeleteDataUnit(long number);//�摜��������j������
	bool			BackupDataUnit(long number);//	�摜���������o�b�N�A�b�v����
	bool			RecoverDataUnit(long number);//�o�b�N�A�b�v���ꂽ�摜�������𕜊�������


	//�摜�������̕ϊ�
	bool			ConvertDataUnit(long number, CONVERT_DATA_OPTION option, bool show_confirm=false);//�摜�������̃^�C�v��ύX
	bool			ConvertDataUnit(long number, int type_to, int rgb_to, int comp_to, bool satulate, int byte_from, int color_gray_method, bool show_message=false);
	bool			ConvertDispToByte(long number);//�摜��������BYTE�^�ɃR���o�[�g
	bool			ConvertMaskToByte(long number);//�}�X�N��BYTE�^�ɃR���o�[�g
	bool			ClipDataUnit(long number, CRect clip_rect, bool show_message=false);//�摜���N���b�s���O
	bool			ClipDataUnit(long number, long dst_data_number, bool do_clear_dst, CRect clip_rect, bool show_message=false);//�摜���N���b�s���O
	bool			ResizeDataUnit(long number, CSize new_size, bool do_stretch, 
									int offset_x=0, int offset_y=0, bool show_confirm=false, int reduction_type=0);//�摜�̃T�C�Y��ύX
	bool			ResizeDataUnit(long number, long dst_data_number, bool do_clear_dst, 
									CSize new_size, bool do_stretch, int offset_x, int offset_y, bool show_message=false, int reduction_type=0);//�摜�̃T�C�Y��ύX
	bool			PerspectiveTransDataUnit(long number, double p_param[8], bool show_message=false, int outRgnMode=0);//�p�[�X�y�N�e�B�u�ϊ�
	bool			PerspectiveTransDataUnit(long number, long dst_data_number, bool do_clear_dst, double p_param[8], bool show_message=false, int outRgnMode = 0);//�p�[�X�y�N�e�B�u�ϊ�
	bool			PerspectiveTransDataUnit(long number, 
											double ti, double tj, double tX, double tY, 
											double theta, double phai, double psi, 
											double fl_phisical, double ccd_pix_size, bool show_message=false, double *perspective_param=NULL,
											double magnify=1, int outRgnMode = 0);//�B��������z�肵���p�[�X�y�N�e�B�u�ϊ�
	bool			PerspectiveTransDataUnit(long number,long dst_data_number, bool do_clear_dst, 
											 double ti, double tj, double tX, double tY, 
											 double theta, double phai, double psi, 
											 double fl_phisical, double ccd_pix_size, bool show_message=false, double *perspective_param=NULL,
											 double magnify=1, int outRgnMode = 0);//�B��������z�肵���p�[�X�y�N�e�B�u�ϊ�
	bool			IsoTransDataUnit(long number, double ti, double tj, double theta, double magnify, bool show_message=false, double *perspective_param=NULL, int outRgnMode = 0);//���s�ړ��A��]�ړ��A�g��
	bool			IsoTransDataUnit(long number, long dst_data_number, bool do_clear_dst, 
									 double ti, double tj, double theta, double magnify, bool show_message=false, double *perspective_param=NULL, int outRgnMode = 0);//���s�ړ��A��]�ړ��A�g��
	bool			PerspectiveTransCoor(double p_param[8], double srcX, double srcY, double *pDstX, double *pDstY);//�p�[�X�y�N�e�B�u���W�ϊ�
	bool			CalcDataValue(long number, long type, CSize size, CString com_str, bool satulate, CString *pErr_str=NULL, CProgressCtrl *pProg=NULL);//�����ŉ�f�l�����߂�
	bool			CalcDataValue(long number, CString com_str, bool satulate, CString *pErr_str=NULL, CProgressCtrl *pProg=NULL);//�����ŉ�f�l�����߂�
	bool			CalcMaskValue(long number, CString com_str, double mask_max, double mask_min, CString *pErr_str=NULL, CProgressCtrl *pProg=NULL);//�����Ń}�X�N�l�����߂�
	bool			FilterSmooth(long number, long dst_data_number, CSize filter_size, bool rem_min_max, bool toMultiImage=false);//�������i�ړ����ρj
	bool			FilterGauss(long number, long dst_data_number, int filter_size, int direction = 0, int pad_type = 0, bool toMultiImage = false, float *pKernel = NULL);//�������i�K�E�V�A���j
	bool			FilterGabor(long image_num, long dst_data_number, int filter_size, float gamma, float freq, float theta, float	psi, float offset = 0, float contrast = 1, int pad_type = 0, bool toMultiImage = false, float *pKernel = NULL);//�K�{�[���t�B���^
	bool			FilterGaussUnshade(long image_num, long dst_data_number, int filter_size, float offset, float contrast, int direction = 0, int pad_type = 0, bool toMultiImage = false, float *pKernel = NULL);//�K�E�V�A�����ȍ����ɂ��V�F�[�f�B���O����
	bool			FilterMedian(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false);//���f�B�A���t�B���^
	bool			FilterBilateral(long image_num, long dst_data_number, int filter_size, float sigmaVal, float sigmaDist, bool toMultiImage=false);//�o�C���e�����t�B���^
	bool			FilterErosion(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false, bool doErosion = true);//���k
	bool			FilterDilation(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false);//�c��
	bool			FilterOpening(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false);//�I�[�v�j���O
	bool			FilterClosing(long number, long dst_data_number, CSize filter_size, bool circleMask = false, bool toMultiImage = false);//�N���[�W���O
	bool			FilterEdgeSobel(long number, long dst_data_number, int filter_size, float min_thresh, int direction = 0, bool toMultiImage = false);//�\�[�x���ɂ��G�b�W���o
	bool			FilterLaplaceGauss(long image_num, long dst_data_number, int filter_size, int direction = 0, float offset = 0, float contrast = 1, int pad_type = 0, bool toMultiImage = false, float *pKernel = NULL);//���v���V�A���I�u�K�E�V�A���t�B���^
	bool			FilterEdgeLog(long number, long dst_data_number, int filter_size, double min_thresh, int direction = 0, int pad_type = 0, bool toMultiImage = false);//LoG�ɂ��א��G�b�W���o
	bool			FilterEdgeCanny(long image_num, long dst_data_number, int filter_size, float thresh, float thresh_weak,  bool toMultiImage=false);//canny�ɂ��א��G�b�W���o
	bool			FilterEdgeSigmoid(long image_num, long dst_data_number, int filter_size, float k, float theta, float offset = 0, float contrast = 1, int pad_type = 0, bool toMultiImage = false, float *pOutKernel = NULL);
	bool			FilterMirror(long image_num, long dst_data_number, int dir, bool toMultiImage = false);//�������]
	bool			FilterNega(long image_num, long dst_data_number, bool toMultiImage = false);//�Z�x���]
	bool			FilterConvolution(long image_num, long dst_data_number, CSize kernel_size, float *pKernel, float offset = 0, float contrast = 1, int pad_type = 0, bool toMultiImage=false);//�R���{�����[�V����
	bool			FilterCLAHE(long image_num, long dst_data_number, double clipLimit, int tileW, int tileH, bool toMultiImage = false);//CLAHE (Contrast Limited Adaptive Histogram Equalization)
	bool			FilterLocalContrast(long image_num, long dst_data_number, int filter_size, float contrast, bool toMultiImage);//�Ǐ��R���g���X�g�g��(sliding window�� 255*( x - ave - s*��)/(2*��) )
	bool			FilterLocalContrastPatch(long image_num, long dst_data_number, int patch_cnt, bool toMultiImage);//�Ǐ��R���g���X�g�g��(�p�b�`�������� (x-min)/(max-min))
	bool			FilterDFT(long image_num, long dst_data_number, bool invert);//DFT
	bool			Binalize(long src_number, long dst_number, double min_thresh, double max_thresh, bool inverse, BYTE fg_val = 255, BYTE bg_val = 0, bool show_message = false ,bool toMultiImage = false);//�Q�l��
	bool			Binalize(long src_number, double min_thresh, double max_thresh, bool inverse);//�Q�l���i���ʂ��}�X�N�ɓ����j
	double			BinalizeThresholdOtsu(long src_number);//��Â̎�@��2�l��臒l�����߂�
	double			BinalizeThresholdPTile(long src_number, int param);//P�^�C����2�l��臒l�����߂�
	bool			BinalizeLocalGauss(long src_number, long dst_number, int filter_size, int offset, bool inverse, BYTE fg_val = 255, BYTE bg_val = 0, int pad_type = 0, bool show_message = false, bool toMultiImage = false);//�K�E�V�A�����ȍ����ɂ��2�l��
	bool			BinalizeLocalLaplaceGauss(long src_number, long dst_number, int filter_size, int offset, bool inverse, BYTE fg_val = 255, BYTE bg_val = 0, int pad_type = 0, bool show_message = false, bool toMultiImage = false);//LOG�K�E�V�A���ɂ��2�l��
	bool			BinalizeNick(long src_number, long dst_number, int windowSize, float k, bool inverse, BYTE fg_val = 255, BYTE bg_val = 0, bool show_message = false, bool toMultiImage = false);//NICK��2�l��
	int				Label(long src_number, long dst_number, bool sort_label, int min_area, int *area = NULL, float *gx = NULL, float *gy = NULL, BYTE fg_val = 255, BYTE bg_val = 0, bool show_message = false, bool toMultiImage = false);//���x�����O
	int				HoughLines(long image_num, int pixSkip, int dirSkip, int voteThresh, double minLineLength, double maxLineGap );//Canny�t�B���^�ȂǂŒ��o�����G�b�W�摜�ɑ΂��A�m���IHough�ϊ����s��
	bool			BlendPoisson(long image_num, long blend_data_number, long dst_data_number, int offsetx, int offsety, int mode=2);//�|���\���摜����

	//2�摜�ԉ��Z
	bool			Calc2DataUnit(int calc_type, int src_imageA, int src_imageB, int dst_image, int dst_type, double weightA = 1, double weightB = 1, double bias = 0, bool satulate = true);
	bool			Linear2DataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, double weightA = 1, double weightB = 1, double bias = 0, bool satulate = true);//2�摜�̒l�̐��`���Z
	bool			SumDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate = true);//2�摜�̘a
	bool			SubtractDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate = true);//2�摜�̍�
	bool			SubAbsDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate); //2�摜�̍��̐�Βl
	bool			MultiplyDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate = true);//2�摜�̐�
	bool			DivideDataUnit(int src_imageA, int src_imageB, int dst_image, int dst_type, bool satulate = true);//2�摜�̏�
	bool			LinearDataUnit(int src_image, int dst_image, int dst_type, double weight, double bias, bool satulate = true);//1�摜�̒l�̐��`���Z
	bool			AbsDataUnit(int src_image, int dst_image);//�摜�̒l�̐�Βl���Ƃ�


	//�摜�������ւ̕`��
	bool			PrepeareDrawing(int num, int copy_src_num);//���m�N���摜���J���[�摜�ɂ���
	void			DrawLineOnImage(long number, double x0, double y0, double x1, double y1, int pen_size, COLORREF col, int copy_image_num=-1);//�����`��iRGB�j
	void			DrawLineOnImage(long number, double x0, double y0, double x1, double y1, int pen_size, double val);//�����`��i�O���[�X�P�[���j
	void			DrawLineOnImageF(long number, double a, double b, double c, int pen_size, COLORREF col, int copy_image_num=-1);//���������璼���`��iRGB�j
	void			DrawLineOnImageF(long number, double a, double b, double c, int pen_size, double val);//���������璼���`��i�O���[�X�P�[���j
	void			DrawCursorOnImage(long number, double x, double y, int pen_size, COLORREF col, int copy_image_num=-1);//�\���J�[�\���`��iRGB�j
	void			DrawCursorOnImage(long number, double x, double y, int pen_size, double val);//�\���J�[�\���`��i�O���[�X�P�[���j
	void			DrawCircleOnImage(long number, double cx, double cy, double rad, COLORREF col, bool fill, int copy_image_num);//�~�`��iRGB�j
	void			DrawCircleOnImage(long number, double cx, double cy, double rad, double val, bool fill);//�~�`��i�O���[�X�P�[���j
	void			DrawRectOnImage(long number, double x0, double y0, double x1, double y1, COLORREF col, bool fill=false, COLORREF fill_col=0, int copy_image_num=-1);//��`�`��iRGB�j
	void			DrawRectOnImage(long number, double x0, double y0, double x1, double y1, double val, bool fill=false, double fill_val=0);//��`�`��
	void			DrawTextOnImage(long number, int x, int y, char str[], COLORREF col, int src_image_num=-1);//�e�L�X�g�`��iRGB�j
	void			DrawTextOnImage(long number, int x, int y, char str[], double val);//�e�L�X�g�`��i�O���[�X�P�[���j
	void			DrawTextOnImage(long number, int x, int y, COLORREF col, char *format, ...);//�t�H�[�}�b�g�w��Ńe�L�X�g�`��iRGB�j
	void			DrawTextOnImage(long number, int x, int y, double val, char *format, ...);//�t�H�[�}�b�g�w��Ńe�L�X�g�`��i�O���[�X�P�[���j
	void			DrawPointOnImage(long number, int x, int y, int dot_size, COLORREF col, int copy_image_num=-1);//�h�b�g�`��iRGB�j
	void			DrawPointOnImage(long number, int x, int y, int dot_size, double val);//�h�b�g�`��i�O���[�X�P�[���j
	void			DrawPointOnMask(long number, int x, int y, int dot_size, BYTE val);//�h�b�g�`��(�}�X�N)
	void			FloodFillOnImage(long number, int x, int y, COLORREF col);//�̈�h��Ԃ��iRGB�j
	void			FloodFillOnImage(long number, int x, int y, double val);//�̈�h��Ԃ��i�O���[�X�P�[���j
	void			FloodFillOnMask(long number, int x, int y, BYTE val);//�̈�h��Ԃ�(�}�X�N)
	void			FillWorkAreaOnImage(long number, COLORREF col);//��Ɨ̈���h��Ԃ��iRGB�j
	void			FillWorkAreaOnImage(long number, double val);//��Ɨ̈���h��Ԃ��i�O���[�X�P�[���j
	void			FillWorkAreaOnMask(long number, BYTE val);//��Ɨ̈���h��Ԃ�(�}�X�N)
	void			DrawImageOnImage(long number, long src_image_num, int offsetX, int offsetY, int mixType);

	//�摜�������̕\��
	void			DrawImage(long number = CURRENT_IMAGE, bool do_update = true, bool do_adjust = false, bool redraw_background=true);//�摜��\������
	void			DrawImage(long number, CString name, bool do_update, bool do_adjust);//�摜��\������
	bool			ScaleImage(long number, float scale);//�\���摜�X�P�[���ύX
	void			DrawCurrentImageFast(bool do_update = true);//���ݑI�𒆂̉摜��\������
	bool			SetDispRange(long number = CURRENT_IMAGE, float min=0 , float max=256);//�\���Z�x�̃����W��ύX���ĕ`�悷��
	bool			SetDispRangeAuto(long number = CURRENT_IMAGE, int type=0);//�\���Z�x�̃����W��ύX���ĕ`�悷��
	bool			ChangePalette(int number, short palette_id);//�J���[�p���b�g��ύX����

	//��Ɨ̈�
	bool			SetWorkArea(long number, long figure_number, RECT rect, bool ornot );//��Ɨ̈����`�ɐݒ�i��`�͐��K������Ă��Ȃ��Ă��悢�j
	bool			SetWorkArea(long number, long figure_number, FIGURE_LINE line, bool ornot);//��Ɨ̈�𕝂������ɐݒ�
	bool			SetWorkArea(long number, long figure_number, FIGURE_ELLIPSE ellipse, bool ornot);//��Ɨ̈��ȉ~�ɐݒ�
	bool			SetWorkArea(long number, long figure_number, FIGURE_CIRCLE circle, bool ornot);//��Ɨ̈�𕝂��~���ɐݒ�
	bool			SetWorkArea(long number, long figure_number, FIGURE_ARC arc, bool ornot);//��Ɨ̈�𕝂��~�ʂɐݒ�
	int				GetWorkArea(long number, long figure_number, FIGURE_UNION *area, bool *ornot);//��Ɨ̈���擾�i�߂�l�͍�Ɨ̈�̎�ށB���s������-1�j


	//�}�[�N
	long			AddMark(long number, float x, float y, double *p_attrib=NULL);//�_�}�[�N��ǉ�����
	long			AddLineMark(long number, float x, float y, float x2, float y2, double *p_attrib=NULL);//���}�[�N��ǉ�����
	long			AddMark(long number, CMark src_mark);//�}�[�N��ǉ�����
	bool			DeleteMark(long number, long mark_number);//�}�[�N���폜����
	bool			ClearMark(long number);//���ׂẴ}�[�N���폜����
	bool			GetMarkVal(long number, long mark_number, float *px, float *py, double *p_val, double *p_attrib=NULL);//�}�[�N�̍��W���擾����
	long			GetMarkNumber(long number);//�}�[�N�̐����擾����


	//////////////////////////////////////////////////////////////////////////////////
	//���C����ʂւ̕\���ւ��鏈��
	//////////////////////////////////////////////////////////////////////////////////
	//GDI�ŕ`��
	bool			ClipSegment( CRect region , double *p0x, double *p0y, double *p1x, double *p2y);//�̈�Ő�����؂���
	bool			ClipSegmentF( CRect region , double a, double b, double c, double *p0x, double *p0y, double *p1x, double *p1y);//�̈�Œ���(ax+by+c=0)��؂���
	bool			DrawClippedSegment(CDC *p_dc, CRect region , double p0x, double p0y, double p1x, double p1y, int size, COLORREF col);///�̈�Ő؂�����������`�悷��D
	bool			DrawClippedSegment(CDC *p_dc, CRect region , double p0x, double p0y, double p1x, double p1y);///�̈�Ő؂�����������`�悷��D
	bool			DrawClippedRect(CDC *p_dc, CRect region , CRect draw_rect, int size, COLORREF col);//�̈�Ő؂�������`��`�悷��
	bool			DrawClippedRect(CDC *p_dc, CRect region , CRect draw_rect);//�̈�Ő؂�������`��`�悷��
	void			DrawLine(double x0, double y0, double x1, double y1, int pen_size, COLORREF col, int width=1);//������`��
	void			DrawLine(double x0, double y0, double x1, double y1, int width=1);//������`��
	void			DrawCursor(double x, double y, int pen_size, COLORREF col);//�\���J�[�\����`��
	void			DrawCursor(double x, double y);//�\���J�[�\����`��
	void			DrawRect(double left, double top, double right, double bottom, int pen_size, COLORREF col);//��`�`��
	void			DrawRect(double left, double top, double right, double bottom);//��`�`��
	void			DrawCircle(double x, double y, double r, int pen_size, COLORREF col, int width=1);//�~��`��
	void			DrawCircle(double x, double y, double r, int width=1);//�~��`��
	void			DrawEllipse(double x, double y, double rx, double ry, int pen_size, COLORREF col, int width=1);//�ȉ~��`��
	void			DrawEllipse(double x, double y, double rx, double ry);//�ȉ~��`��
	void			DrawText(double x, double y, CString str, COLORREF col, int size=-1);//�e�L�X�g�`��
	void			DrawText(double x, double y, CString str);//�e�L�X�g�`��
	void			DrawText(double x, double y, COLORREF col, int size, char *format, ...);//�t�H�[�}�b�g�w��Ńe�L�X�g�`��

	//���C����ʏ�̍��W�Ɖ摜���W�Ƃ̑Ή����Ƃ�
	bool			GetSrcCoor( double dest_coor_x, double dest_coor_y , double *p_src_coor_x, double *p_src_coor_y);//���C����ʂ̍��W����摜���̍��W�ɕϊ�����
	bool			GetDestCoor( double src_coor_x , double src_coor_y , double *p_dest_coor_x, double *p_dest_coor_y);//�摜���̍��W���烁�C����ʓ��̍��W�ɕϊ�����


	///////////////////////////////////////////////////////////////////////////////////
	//�_�C�A���O���\�[�X�Ƃ̃f�[�^�̂����		
	///////////////////////////////////////////////////////////////////////////////////
	bool			GetDialogItemRect(HWND h_wnd, UINT id, CRect *pRct);//�_�C�A���O�A�C�e���̗̈���擾����
	bool			SetDialogItemRect(HWND h_wnd, UINT id, CRect Rct);//�_�C�A���O�A�C�e���̗̈��ݒ肷��
	bool			GetEditValue(HWND h_wnd, UINT id , int *value);//�G�f�B�b�g�{�b�N�X����f�[�^(int)�����o��
	bool			GetEditValue(HWND h_wnd, UINT id , short *value);//�G�f�B�b�g�{�b�N�X����f�[�^(short)�����o��
	bool			GetEditValue(HWND h_wnd, UINT id , float *value);//�G�f�B�b�g�{�b�N�X����f�[�^(float)�����o��
	bool			GetEditValue(HWND h_wnd, UINT id , double *value);//�G�f�B�b�g�{�b�N�X����f�[�^(double)�����o��
	bool			SetEditValue(HWND h_wnd, UINT id , double value);//�G�f�B�b�g�{�b�N�X�Ƀf�[�^(double)��ݒ肷��
	bool			GetEditValue(HWND h_wnd, UINT id , CString *str);//�G�f�B�b�g�{�b�N�X����f�[�^(CString)�����o��
	bool			SetEditValue(HWND h_wnd, UINT id , CString str);//�G�f�B�b�g�{�b�N�X�Ƀf�[�^(CString)��ݒ肷��
	bool			GetCheck(HWND h_wnd, UINT id );//�{�^������`�F�b�N�̏�Ԃ��擾����
	bool			SetCheck(HWND h_wnd, UINT id ,bool value);//�{�^���Ƀ`�F�b�N�̏�Ԃ�ݒ肷��
	CString			ToString(float value, int ordered_length=-1, char fillchar=' ', char filldir=1);//���l�𕶎���ɕϊ�����
	bool			ToNumber(CString str, double *presult);//������𐔒l�ɕϊ�����
	void			ShowMessage(bool do_clear, int num, bool force, CString str);//���b�Z�[�W��\������ido_clear��true�ɂ���ƑO�̃��b�Z�[�W�������j
	void			ShowMessage(bool do_clear, int num, bool force, char *format, ...);//�t�H�[�}�b�g�w��Ń��b�Z�[�W��\������
	void			ShowMessage(bool do_clear, char *format, ...);//�t�H�[�}�b�g�w��Ń��b�Z�[�W��\������i���b�Z�[�W�P�ɋ����\���j
	void			MessageBox(char *format, ...);//�t�H�[�}�b�g�w��Ń��b�Z�[�W�{�b�N�X��\������
	bool			DialogOpen(int id, bool show);
	void			DialogDelete(int id);
	bool			UserDialogOpen(int n);
	bool			UserDialogDelete(int n);
	bool			UserDialogOnDrawMainImage(int n, int image_number);
	bool			UserDialogOnMouseDown(int n, POINT point);
	bool			UserDialogOnMouseDrug(int n, POINT start_point, POINT currnet_point);
	bool			UserDialogOnFinishMouseDrug(int n, POINT start_point, POINT currnet_point);
	bool			UserDialogBatchExecute(int n, BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);
	bool			UserDialogReceive(int n, BYTE *data, int *ndata);
	bool			BatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);
	int				GetUserDlgCaptions(CString caption[]);//���[�U�_�C�A���O�̃L���v�V�������擾

	//���C���E�B���h�E�t���̊e��@�\��Ԏ擾�E�ύX
	int				GetMarkEditMode();//�}�[�N�ҏW���[�h�̏�Ԏ擾
	void			SetMarkEditMode(int mode);//�}�[�N�ҏW���[�h�̏�Ԑݒ�
	bool			GetWorkAreaEditMode();//��Ɨ̈�ҏW���[�h�̏�Ԏ擾
	void			SetWorkAreaEditMode(bool onoff);//��Ɨ̈�ҏW���[�h�̏�Ԑݒ�
	bool			GetRulerToolMode();//2�_�Ԃ̋�������c�[���̏�Ԏ擾
	void			SetRulerToolMode(bool onoff);//2�_�Ԃ̋�������c�[���̏�Ԑݒ�
	int				GetDataPickEditType();//�t���[�n���h�ҏW���[�h�̏�Ԏ擾
	void			SetDataPickEditType(int mode);//�t���[�n���h�ҏW���[�h�̏�Ԑݒ�
	bool			GetCalcToolMode();//�摜�����̏�Ԏ擾
	void			SetCalcToolMode(bool onoff);//�摜�����̏�Ԏ擾
	bool			GetHistgramToolMode();//�q�X�g�O�����̏�Ԏ擾
	void			SetHistgramToolMode(bool onoff);//�q�X�g�O�����̏�Ԏ擾
	int				GetVideoCaptureThroughMode();//�r�f�I�L���v�`���̃X���[�\����Ԃ̎擾
	void			SetVideoCaptureThroughMode(int mode);//�r�f�I�L���v�`���̃X���[�\����Ԃ̐ݒ�
	bool			CopyToClipbord(int image_num=CURRENT_IMAGE);//�\���摜���N���b�v�{�[�h�ɃR�s�[����
	bool			PasteFromClipbord(int image_num);//�N���b�v�{�[�h�ɂ���摜���������ɓ����i�q�f�a�̂݁j

	////////////////////////////////////////////////////////////////////////////////
	//�t�@�C��IO
	////////////////////////////////////////////////////////////////////////////////
	bool			SetFileIOLog(CString filename);//�t�@�C��IO�̃��O���Ƃ�
	CString			GetFileIOLog(int no);//�t�@�C��IO�̃��O��Ԃ�
	///���[�h
	bool			LoadByteImage(long number, CString	filename, CSize size);//8bit�^�̃f�[�^��ǂݍ���
	bool			LoadByteImage(BYTE *p_data , CString	filename, CSize size);//8bit�^�̃f�[�^��ǂݍ���
	bool			LoadShortImage(long number, CString filename, CSize size);//16bit�������^�̃f�[�^��ǂݍ���
	bool			LoadShortImage(short *p_data, CString filename, CSize size);//16bit�������^�̃f�[�^��ǂݍ���
	bool			LoadWordImage(long number, CString	filename, CSize size);//16bit�����Ȃ��^�̃f�[�^��ǂݍ���
	bool			LoadWordImage(WORD *p_data, CString	filename, CSize size);//16bit�����Ȃ��^�̃f�[�^��ǂݍ���
	bool			LoadLongImage(long number, CString	filename, CSize size);//32bit�������^�̃f�[�^��ǂݍ���
	bool			LoadLongImage(long *p_data, CString	filename, CSize size);//32bit�^�̃f�[�^��ǂݍ���
	bool			LoadDwordImage(long image_number, CString filename, CSize size);//32bit�����Ȃ��^�̃f�[�^��ǂݍ���
	bool			LoadDWordImage(DWORD *p_data, CString filename, CSize size);//32bit�����Ȃ��^�̃f�[�^��ǂݍ���
	bool			LoadBitmap(long number, CString	filename, int partX=-1, int partY=-1, int partW=-1, int partH=-1);//�r�b�g�}�b�v��ǂݍ���
	bool			LoadSizedBitmap(BYTE *p_data , CSize size, bool is_rgb, CString	filename);//�傫���w��Ńr�b�g�}�b�v���J��
	bool			LoadAvi(long number, CString	filename);//AVI��ǂݍ���
	bool			LoadFloatImage(long number, CString	filename, CSize size);//float(32���������_)�^�̃f�[�^��ǂݍ���
	bool			LoadFloatImage(float *p_data, CString	filename, CSize size);
	bool			LoadComplexImage(long number, CString	filename, CSize size);//���f���^�̃f�[�^��ǂݍ���
	bool			LoadXYImage(long number, CString	filename, CSize size);//XY�^�̃f�[�^��ǂݍ���
	bool			LoadXYZImage(long number, CString	filename, CSize size);//XYZ�^�̃f�[�^��ǂݍ���
	bool			Load3DImage(long number, CString	filename, CSize size, long page);//3D�^�i8bit����j�f�[�^��ǂݍ���
	bool			LoadF3DImage(long number, CString	filename, CSize size, long page);//Float3D�i32bit����j�f�[�^��ǂݍ���
	bool			SelectAndLoadImageFile(long number);//�g���q����^�C�v�𔻒f���ăt�@�C�����J��
	bool			SelectAndLoadImageFile(long number, CString filename, short width=0, short height=0, short page=1);//�g���q����^�C�v�𔻒f���ăt�@�C����ǂݍ���
	bool			LoadArchivedImage(long number , CString filename);//IMAGE�^�t�@�C�����J��
	bool			LoadAnyImage(long number, CString filename);//Bitmap�ȊO�̉摜���J��
	bool			LoadIFZ(long number, CString filename);
	bool			LoadBFZ(long number, CString filename);

	///�Z�[�u
	bool			SaveViewAsRaw(long number, CString	pathname);//�\���f�[�^��RAW�t�@�C���Ƃ��ĕۑ�����
	bool			SaveViewAsBitmap(long number, CString	pathname);//�\���f�[�^���r�b�g�}�b�v�Ƃ��ĕۑ�����
	bool			SaveViewAsBitmap(long number);//�\���f�[�^���r�b�g�}�b�v�Ƃ��ĕۑ�����
	bool			SaveByteImage(BYTE *p_data , CString	pathname, CSize size, long page);//8bit�^�̃f�[�^��ۑ�����
	bool			SaveShortImage(short *p_data , CString	pathname, CSize size, long page);//16bit�����t���^�̃f�[�^��ۑ�����
	bool			SaveWordImage(WORD *p_data , CString	pathname, CSize size, long page);//16bit�^�̃f�[�^��ۑ�����
	bool			SaveDwordImage(DWORD *p_data , CString	pathname, CSize size, long page);//32bit�����t���^�̃f�[�^��ۑ�����
	bool			SaveLongImage(long *p_data , CString	pathname, CSize size, long page);//32bit�^�̃f�[�^��ۑ�����
	bool			SaveByteAsBitmap(BYTE *p_data , BITMAPINFO *pBmpInfo, CSize	size, CString pathname);
	bool			SaveFloatImage(float *p_data, CString	pathname, CSize size, long page);//float(32���������_)�^�̃f�[�^��ۑ�����
	bool			SelectAndSaveImageFile(long number);//�^�C�v�ʂɃt�@�C����ۑ�����
	bool			SelectAndSaveImageFile(long number, CString pathname);//�^�C�v�ʂɃt�@�C����ۑ�����
	bool			SaveArchivedImage(long number , CString pathname);//*pim�^�t�@�C����ۑ�����
	bool			SaveArchivedImage(long number );//*pim�^�t�@�C����ۑ�����

	//�t�@�C���_�C�A���O��\������
	bool			OpenFileDialog(int file_number_max, CString file_kinds,	
							 CString *pfile_path, CString pfile_names[], int *pfile_number,  char *def_path=NULL);
	bool			OpenFileDialog(int file_number_max, CString file_kind_ext, CString file_kind_disp,
							CString *pfile_path, CString	pfile_names[], int *pfile_number, char *def_path=NULL);
	bool			SaveFileDialog(CString	file_kinds, CString	*pfile_path);
	bool			FolderDialog(CString *pfolder);
	int				GetFileList(CString folderpath, CString file_kind_ext, int file_number_max, CString	pfile_names[]);

	static CString GetDefaultPath();//�A�v���P�[�V�����̐�΃p�X���擾����
	static CString GetVersion(const char  *KeyWord);//�A�v���P�[�V�����̃o�[�W���������擾����



	////////////////////////////////////////////////////////////////////////////////
	//���w�v�Z
	////////////////////////////////////////////////////////////////////////////////
	double			calc_mod( double x , double a);//��]
	float			calc_float_max( float *pf , long len);//�z��̍ő�l
	float			calc_float_min( float *pf , long len);//�z��̍ŏ��l
	double			calc_random();//��l���z�̗��������i0-1�j
	double			calc_gaus();//�K�E�X���z�̗��������i�W���΍�1�j
	double			calc_dist(double x0, double y0, double x1, double y1);//���[�N���b�h����
	bool			calc_lsm(float **ppA, float *pb, float *pX, int spl_num, int param_num);//�ŏ����@
	bool			calc_dft(float *pSrc, int length, float *pPower, float *pPhase=NULL);//���U�t�[���G�ϊ�

	///////////////////////////////////////////////////////////////////////////////////
	//�O���t�\��		
	///////////////////////////////////////////////////////////////////////////////////
	int				AddDataToChart(int chart_id, float value);//�܂���O���t�ɒl��ǉ����ĕ`�悷��
	bool			PlotDataToChart(int chart_id, int data_num, float *pValueArr, bool doClear=true);
	bool			PlotDataToChart(int chart_id, int data_num, double *pValueArr, bool doClear=true);
	bool			PlotDataToChart(int chart_id, int data_num, int *pValueArr, bool doClear=true);
	bool			PlotDataToChart(int chart_id, int data_num, BYTE *pValueArr, bool doClear=true);
	int				AddDataToChart2(int chart_id, float valueX, float valueY);// 2�����U�z�}�ɒl��ǉ����ĕ`�悷��
	bool			PlotDataToChart2(int chart_id, int data_num, float *pValueArrX, float *pValueArrY, bool doClear=true);
	bool			PlotDataToChart2(int chart_id, int data_num, double *pValueArrX, double *pValueArrY, bool doClear=true);
	bool			PlotDataToChart2(int chart_id, int data_num, int *pValueArrX, int *pValueArrY, bool doClear=true);
	bool			PlotDataToChart2(int chart_id, int data_num, BYTE *pValueArrX, BYTE *pValueArrY, bool doClear=true);
	void			ClearChart(int chart_id);


	////////////////////////////////////////////////////////////////////////////////
	//OpenCV�T�|�[�g
	////////////////////////////////////////////////////////////////////////////////
	void*			ConvertToCvImage(long number);//�摜�f�[�^��IplImage�^�ɕϊ�����
	bool			ConvertToCvImage2(long image_num, void *pSrcMat, void *pClippedMat=NULL);//�摜�f�[�^��cv::Mat�ɕϊ�����
	bool			Img2Mat(long image_num, void *pSrcMat, void *pClippedMat = NULL);//�摜�f�[�^��cv::Mat�ɕϊ�����
	bool			ConvertFromCvImage(long number, void *pMat, long src_image_num=-1);//IplImage�^���摜�f�[�^�ɕϊ�����
	bool			ConvertFromCvImage2(long image_num, void *pMat, long src_image_num=-1);//cv::Mat���摜�f�[�^�ɓW�J����
	bool			Mat2Img(long image_num, void *pMat, long src_image_num = -1);//cv::Mat���摜�f�[�^�ɓW�J����

	////////////////////////////////////////////////////////////////////////////////
	//�r�f�I�L���v�`��
	////////////////////////////////////////////////////////////////////////////////
	bool			CaptureInit(CSize size);//�r�f�I�L���v�`���h���C�o���J��
	void			CaptureDeInit();//�L���v�`���h���C�o�����
	bool			CaptureOne(bool doDrawImage=true);//�摜�P�܂���荞��
	bool			IsCaptureOpened();//�r�f�I�L���v�`�����J����Ă��邩�ǂ���

	////////////////////////////////////////////////////////////////////////////////
	//�ʐM
	////////////////////////////////////////////////////////////////////////////////
	//TCP/IP
	bool			OpenSocket(int type, UINT port, CString host, HWND wnd=NULL, void (*recive_callback)(HWND, BYTE*, int*)=NULL, void (*acc_callback)(HWND, int*)=NULL, void (*close_callback)(HWND)=NULL);//�\�P�b�g���J��
	int				SendString(CString str);//������𑗐M����
	void			CloseSocket();//�\�P�b�g�����
	int				GetSocketStatus(CString* pHostName, UINT* pPortNo);//�\�P�b�g�̏�Ԏ擾 0:�T�[�o 1:�N���C�A���g -1:�J���ꂸ

	////////////////////////////////////////////////////////////////////////////////
	//�o�b�N�O���E���h����
	////////////////////////////////////////////////////////////////////////////////
	int				GetBGProcID();//�ޯ���׳��ޏ�����ID�擾
	void			SetBGProcID(int id);//�ޯ���׳��ޏ�����ID�ݒ�
	void			StopBGProc();//�ޯ���׳��ޏ����I��

	unsigned long	Tick();


private:
	long			data_number;//�摜��������
	CDataUnit		**pp_data_unit;//�摜������
	long			current_image_number;//���ݑI�𒆂̉摜�ԍ�
	CDataUnit		*backup_data;//�o�b�N�A�b�v�p�摜������
	int				background_proc_id;//�o�b�N�O���E���h�����i�ʃX���b�h�j��ID
	CSocketCtrl		*pSockCtrl;//�\�P�b�g�ʐM
	CUSTOM_FUNC		custom_func[CUSTOM_FUNC_MAX];//�J�X�^���t�@���N�V����
};


