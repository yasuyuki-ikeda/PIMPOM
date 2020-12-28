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
//CFilterCtrlDlg
//�t�B���^�ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "FilterCtrlDlg.h"


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

CFilterCtrlDlg::CFilterCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilterCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterCtrlDlg)
	//}}AFX_DATA_INIT
}


void CFilterCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterCtrlDlg)
	DDX_Control(pDX, IDC_COMBO_FILTER_TYPE, m_cmbo_filter_type);
	DDX_Control(pDX, IDC_COMBO_FILTER_SIZE, m_cmbo_filter_size);
	//}}AFX_DATA_MAP
}

/********************************************************************
�@  �\  ��  �� : �_�C�A���O�A�C�e���̕\����ԕύX
��    ��    �� : show_dlg_item
��          �� :
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::show_dlg_item()
{
	if(API.GetCheck(m_hWnd,IDC_RADIO_FILTER_OVERWITE)){
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO), false);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO), false);
	}else{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO), true);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO), true);
	}
}

/********************************************************************
�@  �\  ��  �� : �I���摜�ɑ΂��ăt�B���^���s
��    ��    �� : execute
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool CFilterCtrlDlg::execute(int src_num, int dst_num)
{
	CDataUnit	*p_du;
	int			filterID;
	CSize		filter_size_c;
	int			filter_size;
	bool		forMulti;
	bool		ret=true;
	int			ctrlID[] = { IDC_EDIT_FILTER_PARAM0 ,IDC_EDIT_FILTER_PARAM1, IDC_EDIT_FILTER_PARAM2, IDC_EDIT_FILTER_PARAM3, IDC_EDIT_FILTER_PARAM4, IDC_EDIT_FILTER_PARAM5, IDC_EDIT_FILTER_PARAM6 };

	p_du = API.GetDataUnit(src_num);
		if(!p_du)	return false;
		
	//�t�B���^�^�C�v���R���{�{�b�N�X����擾
	filterID = m_cmbo_filter_type.GetCurSel();


	//�t�B���^�T�C�Y���R���{�{�b�N�X����擾
	if (m_cmbo_filter_size.GetCount() > 0) {
		m_filter_param[filterID].kernel_size_sel = m_cmbo_filter_size.GetCurSel();
	}
	filter_size = m_filter_param[filterID].kernel_size[m_filter_param[filterID].kernel_size_sel];


	//�e��p�����^�_�C�A���O�A�C�e������擾

	//�~�`
	if (m_filter_param[filterID].circle_shape >= 0) {
		if (API.GetCheck(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE)) {
			m_filter_param[filterID].circle_shape = 1;
		}
		else {
			m_filter_param[filterID].circle_shape = 0;
		}
	}

	//�ɐ�
	if (m_filter_param[filterID].direction >= 0)
	{
		if (API.GetCheck(m_hWnd, IDC_RADIO_FILTER_X)) {//���̂�
			m_filter_param[filterID].direction = 1;
			filter_size_c.cx = filter_size;
			filter_size_c.cy = 1;
		}
		else if (API.GetCheck(m_hWnd, IDC_RADIO_FILTER_Y) ) {//�c�̂�
			m_filter_param[filterID].direction = 2;
			filter_size_c.cx = 1;
			filter_size_c.cy = filter_size;
		}
		else {//�c��
			m_filter_param[filterID].direction = 0;
			filter_size_c.cx = filter_size;
			filter_size_c.cy = filter_size;
		}
	}
	else {
		filter_size_c.cx = filter_size;
		filter_size_c.cy = filter_size;
	}


	//padding
	if (m_filter_param[filterID].padding_type >= 0)
	{
		if (API.GetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD1)) {//�O������
			m_filter_param[filterID].padding_type = 1;
		}
		else if (API.GetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD2)) {//�܂�Ԃ�
			m_filter_param[filterID].padding_type = 2;
		}
		else {//�c��
			m_filter_param[filterID].padding_type = 0;
		}
	}

	//���ׂẴy�[�W�^�`���l����ΏۂƂ���
	forMulti = API.GetCheck(m_hWnd, IDC_CHECK_FILTER_CTRL_FOR_MULTI);


	//���̑��̃p�����^
	for (int i = 0; i < FILTER_PARAM_MAX; i++) {
		if (m_filter_param[filterID].param_name[0] != 0) {
			API.GetEditValue(m_hWnd, ctrlID[i], &m_filter_param[filterID].param[i]);
		}
	}
	



	//�J�[�l���\���pDU
	if (pKernelDU){
		delete	pKernelDU;
		pKernelDU = NULL;
	}

	if(filter_size_c.cx>0 && filter_size_c.cy>0){
		pKernelDU = new CDataUnit(FLOAT_FORMAT, filter_size_c, 1);
	}

		//�t�B���^��ނ�����
		switch(filterID)
		{
			case FILTER_MEAN://�ړ�����
				ret = API.FilterSmooth(src_num, dst_num, filter_size_c, false, forMulti);
				break;

			case FILTER_MEAN_M://�ő�E�ŏ����������ړ�����
				ret = API.FilterSmooth(src_num, dst_num, filter_size_c, true, forMulti);
				break;

			case FILTER_GAUSS://�K�E�X
				ret = API.FilterGauss(src_num, dst_num, filter_size, m_filter_param[filterID].direction, m_filter_param[filterID].padding_type, forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_MEDIAN://���f�B�A��
				ret = API.FilterMedian(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_DILATION://�c��
				ret = API.FilterDilation(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_EROSION://���k
				ret = API.FilterErosion(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_OPENING://�I�[�v�j���O
				ret = API.FilterOpening(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_CLOSING://�N���[�W���O
				ret = API.FilterClosing(src_num, dst_num, filter_size_c, m_filter_param[filterID].circle_shape, forMulti);
				break;

			case FILTER_BILATERAL://�o�C���e�����t�B���^
				ret = API.FilterBilateral(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], m_filter_param[filterID].param[1], forMulti);
				break;

			case FILTER_SOBEL://�G�b�W���o(sobel)
				ret = API.FilterEdgeSobel(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], m_filter_param[filterID].direction, forMulti);
				break;

			case FILTER_LOG://Laplacian of Gaussian
				ret = API.FilterLaplaceGauss(src_num, dst_num, filter_size, m_filter_param[filterID].direction, 
																m_filter_param[filterID].param[0], 
																m_filter_param[filterID].param[1],
																m_filter_param[filterID].padding_type,
																forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_GABOR://Gabor
				ret = API.FilterGabor(src_num, dst_num, filter_size, m_filter_param[filterID].param[3], 
																	m_filter_param[filterID].param[2], 
																	m_filter_param[filterID].param[4], 
																	m_filter_param[filterID].param[5], 
																	m_filter_param[filterID].param[0], 
																	m_filter_param[filterID].param[1], 
																	m_filter_param[filterID].padding_type, 
																	forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_SIGMOID://Sigmoid
				ret = API.FilterEdgeSigmoid(src_num, dst_num, filter_size, m_filter_param[filterID].param[3], 
																		m_filter_param[filterID].param[2], 
																		m_filter_param[filterID].param[0], 
																		m_filter_param[filterID].param[1], 
																		m_filter_param[filterID].padding_type, 
																		forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_LOG_ZEROCLS://�G�b�W�א���(LoG�[���N���X)
				ret = API.FilterEdgeLog(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], 
																		m_filter_param[filterID].direction, 
																		m_filter_param[filterID].padding_type, forMulti);
				break;

			case FILTER_CANNY://�G�b�W�א���(Canny)
				ret = API.FilterEdgeCanny(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], m_filter_param[filterID].param[1], forMulti);
				break;

			case FILTER_GAUSS_UNSHADE://�V�F�[�f�B���O����
				ret = API.FilterGaussUnshade(src_num, dst_num, filter_size, m_filter_param[filterID].param[0], 
																			m_filter_param[filterID].param[1], 
																			m_filter_param[filterID].direction,
																			m_filter_param[filterID].padding_type, 
																			forMulti, pKernelDU->pFloatData);
				break;

			case FILTER_NEGA://�Z�x���]
				ret = API.FilterNega(src_num, dst_num, forMulti);
				break;

			case FILTER_MIRROR_LR://�����i���E�j
				ret = API.FilterMirror(src_num, dst_num, 0, forMulti);
				break;

			case FILTER_MIRROR_UD://�����i�㉺�j
				ret = API.FilterMirror(src_num, dst_num, 1, forMulti);
				break;

			case FILTER_ROT180://180��
				ret = API.FilterMirror(src_num, dst_num, 2, forMulti);
				break;

			case FILTER_CONVOLUTION:
			{
				int kernelSize;
				ret = get_conv_kernel(&kernelSize, pConvolutionKernel);

				if (ret)
				{
					if (kernelSize == filter_size_c.cx)
					{
						ret = API.FilterConvolution(src_num, dst_num, filter_size_c, pConvolutionKernel,
													m_filter_param[filterID].param[0],
													m_filter_param[filterID].param[1],
													m_filter_param[filterID].padding_type, forMulti);

						if (ret){
							memcpy(pKernelDU->pFloatData, pConvolutionKernel, filter_size_c.cx*filter_size_c.cy*sizeof(float));
						}
					}
					else{
						API.MessageBox("kernel value size invalid");
						ret = false;
					}
				}
				else{
					API.MessageBox("kernel value sintax error");
				}
			}
				break;

			case FILTER_CLAHE://CLAHE(Contrast Limited Adaptive Histogram Equalization)
				ret = API.FilterCLAHE(src_num, dst_num, m_filter_param[filterID].param[0], m_filter_param[filterID].param[1], m_filter_param[filterID].param[2], forMulti);
				break;

			case FILTER_LOCAL_CONTRAST://�Ǐ��R���g���X�g�g��(sliding window�� 255*( x - ave - s*��)/(2*��) )
				ret = API.FilterLocalContrast(src_num, dst_num, m_filter_param[filterID].param[0], m_filter_param[filterID].param[1], forMulti);
				break;

			case FILTER_LOCAL_CONTRAST_P://�Ǐ��R���g���X�g�g��(�p�b�`�������� (x-min)/(max-min))
				ret = API.FilterLocalContrastPatch(src_num, dst_num, m_filter_param[filterID].param[0], forMulti);
				break;

			case FILTER_DFT://���U�t�[���G�ϊ�
				ret = API.FilterDFT(src_num, dst_num, m_filter_param[filterID].param[0]);
				break;
		}

	//�J�[�l���\��
		if (ret)
		{
			draw_kernel();
		}

	return ret;

}


/********************************************************************
�@  �\  ��  �� : �t�B���^�p�����^�̏�����
��    ��    �� : init_filter_params
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::init_filter_params()
{
	memset(m_filter_param, 0, sizeof(FILTER_CTRL_PARAMS)*FILTER_COUNT);

	strcpy(m_filter_param[FILTER_MEAN].name, "�ړ�����");
	strcpy(m_filter_param[FILTER_MEAN_M].name, "�ړ�����(�ő�/�ŏ�����)");
	strcpy(m_filter_param[FILTER_GAUSS].name, "�K�E�V�A��");
	strcpy(m_filter_param[FILTER_MEDIAN].name, "���f�B�A��");
	strcpy(m_filter_param[FILTER_DILATION].name, "�c��");
	strcpy(m_filter_param[FILTER_EROSION].name, "���k");
	strcpy(m_filter_param[FILTER_OPENING].name, "�I�[�v�j���O");
	strcpy(m_filter_param[FILTER_CLOSING].name, "�N���[�W���O");
	strcpy(m_filter_param[FILTER_BILATERAL].name, "�o�C���e����");
	strcpy(m_filter_param[FILTER_SOBEL].name, "�G�b�W���o(sobel)");
	strcpy(m_filter_param[FILTER_LOG].name, "Laplacian of Gaussian(LoG)");
	strcpy(m_filter_param[FILTER_GABOR].name, "�K�{�[��");
	strcpy(m_filter_param[FILTER_SIGMOID].name, "�G�b�W���o(�V�O���C�h)");
	strcpy(m_filter_param[FILTER_LOG_ZEROCLS].name, "�G�b�W�א���(LoG�[���N���X)");
	strcpy(m_filter_param[FILTER_CANNY].name, "�G�b�W�א���(Canny)");
	strcpy(m_filter_param[FILTER_GAUSS_UNSHADE].name, "�V�F�[�f�B���O����(�K�E�V�A�����ȍ���)");
	strcpy(m_filter_param[FILTER_NEGA].name, "�Z�x���]");
	strcpy(m_filter_param[FILTER_MIRROR_LR].name, "���E���]");
	strcpy(m_filter_param[FILTER_MIRROR_UD].name, "�㉺���]");
	strcpy(m_filter_param[FILTER_ROT180].name, "180�����]");
	strcpy(m_filter_param[FILTER_CONVOLUTION].name, "�R���{�����[�V����");
	strcpy(m_filter_param[FILTER_CLAHE].name, "CLAHE(���׽Đ����t���K���^˽ĸ��ѕ��R��)");
	strcpy(m_filter_param[FILTER_LOCAL_CONTRAST].name, "�Ǐ����׽Ċg��(255*( x - ave - s*��)/(2*��))");
	strcpy(m_filter_param[FILTER_LOCAL_CONTRAST_P].name, "�Ǐ����׽Ċg��(x-min)/(max-min)");
	strcpy(m_filter_param[FILTER_DFT].name, "���U�t�[���G�ϊ�");

	for (int id = 0; id < FILTER_COUNT; id++)
	{

		switch (id)
		{
		case FILTER_MEAN://������
				 //not break
		case FILTER_MEAN_M://�ő�E�ŏ���������������
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = 0;//�ɐ��ݒ肷��
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_GAUSS://�K�E�V�A��
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = 0;//�ɐ��ݒ肷��
			m_filter_param[id].padding_type = 0;//padding�ݒ肷��

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_MEDIAN://���f�B�A��
				   //not break
		case FILTER_DILATION://�c��
					 //not break
		case FILTER_EROSION://���k
					//not break
		case FILTER_OPENING://�I�[�v�j���O
					//not break
		case FILTER_CLOSING://�N���[�W���O
			m_filter_param[id].circle_shape = 0;//�~�`�ݒ肷��
			m_filter_param[id].direction = 0;//�ɐ��ݒ肷��
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�

			m_filter_param[id].kernel_size[0] = 2;
			m_filter_param[id].kernel_size_sel = 1;
			for (int i = 1, filter_size = 3; filter_size <= LANK_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}

			break;

		case FILTER_BILATERAL://�o�C���e�����t�B���^
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�
			strcpy(m_filter_param[id].param_name[0], "�l�̃�");		m_filter_param[id].param[0] = 10;
			strcpy(m_filter_param[id].param_name[1], "�����̃�");	m_filter_param[id].param[1] = 10;

			for (int i = 0, filter_size = 3; filter_size <= LANK_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_SOBEL://�G�b�W���o(sobel)
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = 0;//�ɐ��ݒ肷��
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�
			strcpy(m_filter_param[id].param_name[0], "�Z�x臒l");	m_filter_param[id].param[0] = 100;

			for (int i = 0, filter_size = 3; filter_size <= 9; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_LOG://Laplacian of gaussian
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = 0;//�ɐ��ݒ肷��	
			m_filter_param[id].padding_type = 0;//padding�ݒ肷��
			strcpy(m_filter_param[id].param_name[0], "�I�t�Z�b�g");		m_filter_param[id].param[0] = 128;
			strcpy(m_filter_param[id].param_name[1], "�R���g���X�g");	m_filter_param[id].param[1] = 1;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_GABOR://Gabor
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = 0;//padding�ݒ肷��
			strcpy(m_filter_param[id].param_name[0], "�I�t�Z�b�g");		m_filter_param[id].param[0] = 128;
			strcpy(m_filter_param[id].param_name[1], "�R���g���X�g");	m_filter_param[id].param[1] = 1;
			strcpy(m_filter_param[id].param_name[2], "���g��");			m_filter_param[id].param[2] = 2;
			strcpy(m_filter_param[id].param_name[3], "�G����");			m_filter_param[id].param[3] = 0.3;
			strcpy(m_filter_param[id].param_name[4], "��]�p");			m_filter_param[id].param[4] = 0;
			strcpy(m_filter_param[id].param_name[5], "�����ʑ�");		m_filter_param[id].param[5] = 0;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_SIGMOID://Sigmoid
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = 0;//padding�ݒ肷��
			strcpy(m_filter_param[id].param_name[0], "�I�t�Z�b�g");		m_filter_param[id].param[0] = 128;
			strcpy(m_filter_param[id].param_name[1], "�R���g���X�g");	m_filter_param[id].param[1] = 1;
			strcpy(m_filter_param[id].param_name[2], "��]�p");			m_filter_param[id].param[2] = 0;
			strcpy(m_filter_param[id].param_name[3], "k");				m_filter_param[id].param[3] = 1;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_LOG_ZEROCLS://�G�b�W�א���(LoG�[���N���X)
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = 0;//�ɐ��ݒ肷��
			m_filter_param[id].padding_type = 0;//padding�ݒ肷��
			strcpy(m_filter_param[id].param_name[0], "�Z�x臒l");	m_filter_param[id].param[0] = 100;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_CANNY://�G�b�W�א���(Canny)
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�
			strcpy(m_filter_param[id].param_name[0], "�Z�x臒l(��)");	m_filter_param[id].param[0] = 100;
			strcpy(m_filter_param[id].param_name[1], "�Z�x臒l(��)");	m_filter_param[id].param[0] = 30;

			for (int i = 0, filter_size = 3; filter_size <= 7; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_GAUSS_UNSHADE://�V�F�[�f�B���O����
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = 0;//�ɐ��ݒ肷��
			m_filter_param[id].padding_type = 0;//padding�ݒ肷��
			strcpy(m_filter_param[id].param_name[0], "�I�t�Z�b�g");		m_filter_param[id].param[0] = 0;
			strcpy(m_filter_param[id].param_name[1], "�R���g���X�g");	m_filter_param[id].param[1] = 1;

			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_NEGA://�Z�x���]
			//not break;
		case FILTER_MIRROR_LR://�����i���E�j
			//not break;
		case FILTER_MIRROR_UD://�����i�㉺�j
			//not break;
		case FILTER_ROT180://180��
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�
			break;

		case FILTER_CONVOLUTION:
			m_filter_param[id].padding_type = 0;//padding�ݒ肷��
			strcpy(m_filter_param[id].param_name[0], "�I�t�Z�b�g");		m_filter_param[id].param[0] = 0;
			strcpy(m_filter_param[id].param_name[1], "�R���g���X�g");	m_filter_param[id].param[1] = 1;
			for (int i = 0, filter_size = 3; filter_size <= COMB_FILTER_SIZE_MAX; i++, filter_size += 2)
			{
				m_filter_param[id].kernel_size[i] = filter_size;
			}
			break;

		case FILTER_CLAHE:
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�
			strcpy(m_filter_param[id].param_name[0], "�x�����");		m_filter_param[id].param[0] = 40;
			strcpy(m_filter_param[id].param_name[1], "��������");		m_filter_param[id].param[1] = 8;
			strcpy(m_filter_param[id].param_name[2], "�c������");		m_filter_param[id].param[2] = 8;
			break;

		case FILTER_LOCAL_CONTRAST:
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�
			strcpy(m_filter_param[id].param_name[0], "�Ǐ��̈�T�C�Y");		m_filter_param[id].param[0] = 200;
			strcpy(m_filter_param[id].param_name[1], "�X�P�[��s");		m_filter_param[id].param[1] = 3;
			break;

		case FILTER_LOCAL_CONTRAST_P:
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�
			strcpy(m_filter_param[id].param_name[0], "������");		m_filter_param[id].param[0] = 8;
			break;

		case FILTER_DFT:
			m_filter_param[id].circle_shape = -1;//�~�`�ݒ肵�Ȃ�
			m_filter_param[id].direction = -1;//�ɐ��ݒ肵�Ȃ�
			m_filter_param[id].padding_type = -1;//padding�ݒ肵�Ȃ�
			strcpy(m_filter_param[id].param_name[0], "�t�ϊ�");		m_filter_param[id].param[0] = 0;
			break;
		}
	}
}


/********************************************************************
�@  �\  ��  �� : �J�[�l���̕\��
��    ��    �� : draw_kernel
��          �� :
��    ��    �l :
�@          �\ :�@
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::draw_kernel()
{
	CDC *pdc = GetDC();
	float kernelMin, kernelMax;
	float kernelCenterLineX[COMB_FILTER_SIZE_MAX];
	float kernelCenterLineY[COMB_FILTER_SIZE_MAX];
	CPoint  orig;
	float  graphH = COMB_FILTER_SIZE_MAX / 3;
	CRect	kernelDispRgn(pKernelIF->FieldCoordinate.x, pKernelIF->FieldCoordinate.y,
		pKernelIF->FieldCoordinate.x + COMB_FILTER_SIZE_MAX + graphH,
		pKernelIF->FieldCoordinate.y + COMB_FILTER_SIZE_MAX + graphH);



	//�w�i��`�悷��
	pdc->Rectangle(kernelDispRgn);


	if (pKernelDU)
	{
		//�J�[�l���̉摜��\������
		pKernelDU->GetDataMinMax(&kernelMin, &kernelMax);
		pKernelDU->SetDispRange(kernelMin, kernelMax);
		pKernelDU->UpdateDispBuffer();
		pKernelIF->DrawImage(pKernelDU, false);

		//�J�[�l���̒��S���C���̕��z���擾����

		for (int i = 0; i < pKernelDU->DataSize.cx; i++) {
			kernelCenterLineX[i] = pKernelDU->pFloatData[i + pKernelDU->DataSize.cy / 2 * pKernelDU->DataSize.cx];
		}

		for (int i = 0; i < pKernelDU->DataSize.cy;i++) {
			kernelCenterLineY[i] = pKernelDU->pFloatData[pKernelDU->DataSize.cx/2 + i * pKernelDU->DataSize.cx];
		}

		//�J�[�l���̒��S���C���̕��z��\������
		CPen penCX(PS_SOLID,1,RGB(255,0,0));
		CPen penCY(PS_SOLID, 1,RGB(0, 0, 255));
		CPen penGuide(PS_DASH, 1, RGB(0, 0, 0));
		CPen *poldpen;
		
		//�������̃K�C�h��
		poldpen = pdc->SelectObject(&penGuide);
		pdc->MoveTo(kernelDispRgn.left, kernelDispRgn.top);
		pdc->LineTo(kernelDispRgn.left, kernelDispRgn.bottom);
		pdc->MoveTo(kernelDispRgn.left + pKernelDU->DataSize.cx, kernelDispRgn.top);
		pdc->LineTo(kernelDispRgn.left + pKernelDU->DataSize.cx, kernelDispRgn.bottom);
		pdc->SelectObject(poldpen);
		
		poldpen = pdc->SelectObject(&penCX);
		pdc->MoveTo(kernelDispRgn.left, kernelDispRgn.top + pKernelDU->DataSize.cy/2);
		pdc->LineTo(kernelDispRgn.left + pKernelDU->DataSize.cx, kernelDispRgn.top + pKernelDU->DataSize.cy / 2);
		pdc->SelectObject(poldpen);

		//�������̃O���t
		for (int i = 0; i < pKernelDU->DataSize.cx ; i++) 
		{
			float x = kernelDispRgn.left + i;
			float y = kernelDispRgn.bottom - (kernelCenterLineX[i] - kernelMin) / (kernelMax - kernelMin) * graphH;
			pdc->SetPixel(x, y, RGB(255, 0, 0));
			pdc->SetPixel(x, y-1, RGB(255, 0, 0));
		}

		//�c�����̃K�C�h��
		poldpen = pdc->SelectObject(&penGuide);
		pdc->MoveTo(kernelDispRgn.left, kernelDispRgn.top);
		pdc->LineTo(kernelDispRgn.right, kernelDispRgn.top);
		pdc->MoveTo(kernelDispRgn.left, kernelDispRgn.top + pKernelDU->DataSize.cy);
		pdc->LineTo(kernelDispRgn.right, kernelDispRgn.top + pKernelDU->DataSize.cy);
		pdc->SelectObject(poldpen);

		poldpen = pdc->SelectObject(&penCY);
		pdc->MoveTo(kernelDispRgn.left + pKernelDU->DataSize.cx/2, kernelDispRgn.top);
		pdc->LineTo(kernelDispRgn.left + pKernelDU->DataSize.cx / 2, kernelDispRgn.top + pKernelDU->DataSize.cy);
		pdc->SelectObject(poldpen);

		//�c�����̃O���t
		for (int i = 0; i < pKernelDU->DataSize.cy ; i++)
		{
			float x = kernelDispRgn.right - (kernelCenterLineY[i] - kernelMin) / (kernelMax - kernelMin) * graphH;
			float y = kernelDispRgn.top + i;
			pdc->SetPixel(x, y, RGB(0, 0, 255));
			pdc->SetPixel(x-1, y, RGB(0, 0, 255));
		}
	}

	ReleaseDC(pdc);
}


/********************************************************************
�@  �\  ��  �� : �R���{�����[�V�����J�[�l���̒l���G�f�B�b�g�{�b�N�X�֕\������
��    ��    �� : set_conv_kernel
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::set_conv_kernel(int filter_size)
{	
	CString message, tmpstr;
	for (int j = 0; j < filter_size; j++)
	{
		for (int i = 0; i < filter_size; i++){
			tmpstr.Format("%0.5f", pConvolutionKernel[i + j*filter_size]);
			message += tmpstr;

			if (i < filter_size - 1){
				message += CString(", ");
			}
		}

		if (j < filter_size - 1){
			message += CString("\r\n");
		}
	}

	API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_CTRL_CONV_KERNEL, message);

}

/********************************************************************
�@  �\  ��  �� : �G�f�B�b�g�{�b�N�X����R���{�����[�V�����J�[�l���̒l���擾����
��    ��    �� : get_conv_kernel
��          �� :
��    ��    �l :����������true
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool CFilterCtrlDlg::get_conv_kernel(int *pkernel_size, float *pkernel)
{
	CString message, submes;

	API.GetEditValue(m_hWnd, IDC_EDIT_FILTER_CTRL_CONV_KERNEL, &message);//�G�f�B�b�g�{�b�N�X���炷�ׂĂ̕��������o��
	
	//�s�񐔂𐔂���
	int  cntH = 0;//�s���̃J�E���^
	int cntW_perH[COMB_FILTER_SIZE_MAX] = { 0 };//�s���Ƃ̗񐔂̃J�E���^
	for (int i = 0; i < message.GetLength() ; i++)
	{
		if ( message[i] == '\n' || i == message.GetLength()-2)//���s�����������܂��͍Ō�ɓ��B����
		{
			cntW_perH[cntH]++;//�񐔃C���N�������g
			if (cntW_perH[cntH] > COMB_FILTER_SIZE_MAX){//�񐔂�����𒴂�����A�E�g
				return false;
			}

			cntH++;//�s���C���N�������g
			if (cntH > COMB_FILTER_SIZE_MAX){//�s��������𒴂�����A�E�g
				return false;
			}
		}
		else if (message[i] == ','){//�J���}����������
			cntW_perH[cntH]++;//�񐔃C���N�������g
			if (cntW_perH[cntH] > COMB_FILTER_SIZE_MAX){//�񐔂�����𒴂�����A�E�g
				return false;
			}
		}
	}

	if (cntH < 3)	return false;//�s����3�����Ȃ�A�E�g

	for (int i = 0; i < cntH-1; i++)
	{
		if (cntW_perH[i] != cntH){//�񐔂��������A�E�g
			return false;
		}
	}

	//���l��ǂݏo��
	int cnt=0;
	for (int i = 0; i < message.GetLength() - 1; i++)
	{
		if (message[i] == '\n' || i == message.GetLength() - 2)//���s�����������܂��͍Ō�ɓ��B����
		{
			pkernel[cnt] = atof(submes.GetBuffer());	submes.ReleaseBuffer();		submes.Empty();
			cnt++;
		}
		else if (message[i] == ','){//�J���}����������
			pkernel[cnt] = atof(submes.GetBuffer());	submes.ReleaseBuffer();		submes.Empty();
			cnt++;
		}
		else if (isdigit(message[i]) || isalpha(message[i]) || ispunct(message[i])){
			submes += CString(message[i]);
		}
	}

	*pkernel_size = cntH;

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFilterCtrlDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterCtrlDlg)
	ON_BN_CLICKED(IDC_BUTTON_FILTER_EXEC, OnFilterExec)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_FILTER_OVERWITE, &CFilterCtrlDlg::OnBnClickedRadioFilterOverwite)
	ON_BN_CLICKED(IDC_RADIO_FILTER_TO_IMAGE, &CFilterCtrlDlg::OnBnClickedRadioFilterToImage)
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_TYPE, &CFilterCtrlDlg::OnCbnSelchangeComboFilterType)
	ON_BN_CLICKED(IDC_BUTTON_FILTER_CTRL_SET_ORG_IMG, &CFilterCtrlDlg::OnBnClickedButtonFilterCtrlSetOrgImg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_FILTER_SIZE, &CFilterCtrlDlg::OnCbnSelchangeComboFilterSize)
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
BOOL CFilterCtrlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//if( API.GetDataUnit(CURRENT_IMAGE) ==NULL)	return false;


	//�u���ʂ�ʃ������֓����v�������l�ɂ���
	API.SetCheck(m_hWnd, IDC_RADIO_FILTER_TO_IMAGE,true);
	show_dlg_item();


	//���ʂ�����摜�������ԍ���ݒ�
	int dst_num, src_num; 
	src_num = API.GetCurrentImageNumber();
	dst_num = src_num + 1;

	API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO, src_num);
	if(dst_num<API.GetDataUnitNumber())		API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO, dst_num);

	//�t�B���^������
	init_filter_params();

	//�t�B���^��ރR���{�{�b�N�X�̐ݒ�
	m_cmbo_filter_type.DeleteString(0);
	for (int i = 0; i < FILTER_COUNT; i++) {
		if (m_filter_param[i].name[0]!=0) {
			m_cmbo_filter_type.AddString( CString(m_filter_param[i].name) );
		}
	}
	m_cmbo_filter_type.SetCurSel(0);
	

	//�t�B���^�T�C�Y�̏����l�ݒ�
	OnCbnSelchangeComboFilterType();

	//�R���{�����[�V�����p�J�[�l��
	pConvolutionKernel = new float[COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX];
	memset(pConvolutionKernel, 0, COMB_FILTER_SIZE_MAX*COMB_FILTER_SIZE_MAX*sizeof(float));

	//�t�B���^�J�[�l���\��
	CRect knl_rgn;
	API.GetDialogItemRect(m_hWnd, IDC_STATIC_FILTER_CTRL_KERNEL, &knl_rgn);
	API.SetDialogItemRect(m_hWnd, IDC_STATIC_FILTER_CTRL_KERNEL, CRect(knl_rgn.left, knl_rgn.top, COMB_FILTER_SIZE_MAX, COMB_FILTER_SIZE_MAX));

	pKernelIF = new CImageField(m_hWnd, CSize(COMB_FILTER_SIZE_MAX, COMB_FILTER_SIZE_MAX), CPoint(knl_rgn.left, knl_rgn.top));
	pKernelDU = NULL;
	



	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}


void CFilterCtrlDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (pKernelIF) {
		delete	pKernelIF;
	}

	if (pKernelDU) {
		delete	pKernelDU;
	}

	if (pConvolutionKernel){
		delete[]	pConvolutionKernel;
	}
}

/********************************************************************
�@  �\  ��  �� : ���s�{�^������
��    ��    �� : OnFilterExec
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::OnFilterExec() 
{

	if(API.GetCheck(m_hWnd, IDC_RADIO_FILTER_OVERWITE))
	{
		API.BackupDataUnit(CURRENT_IMAGE);//���݂̃f�[�^�o�b�N�A�b�v

		if( !execute(CURRENT_IMAGE,CURRENT_IMAGE))	return ;

		API.DrawImage();//�`��
	}
	else
	{
		int dst_no=0,src_no=0;
		API.GetEditValue(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO, &src_no);
		API.GetEditValue(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO, &dst_no);


		if( !execute(src_no,dst_no))	return ;

		API.DrawImage(dst_no);//�`��
	}

}


/********************************************************************
�@  �\  ��  �� : �u���ݑI�𒆂̉摜�������ɏ㏑���v
��    ��    �� : OnBnClickedRadioFilterOverwite
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::OnBnClickedRadioFilterOverwite()
{
	show_dlg_item();
}


/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnBnClickedRadioFilterToImage
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::OnBnClickedRadioFilterToImage()
{
	show_dlg_item();
}


/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnCbnSelchangeComboFilterType
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::OnCbnSelchangeComboFilterType()
{
	int			filterID = m_cmbo_filter_type.GetCurSel();
	CString		oldSizeStr("");

	//�t�B���^�T�C�Y������
	if(m_cmbo_filter_size.GetCount()>0)
	{
		if(m_cmbo_filter_size.GetCurSel()>=0){
			m_cmbo_filter_size.GetLBText(m_cmbo_filter_size.GetCurSel(), oldSizeStr);//���ݑI�𒆂̒l������Αޔ�
		}

		while(m_cmbo_filter_size.DeleteString(0));//�t�B���^�T�C�Y�R���{�{�b�N�X�̑S�v�f���N���A
	}


	//�t�B���^�T�C�Y��ݒ�
	for (int i = 0;  i<FILTER_SIZE_LIST_MAX; i++)
	{
		int kernel_size = m_filter_param[filterID].kernel_size[i];
		if(kernel_size>0)
		{
			CString str;
			str.Format("%d�~%d", kernel_size, kernel_size);
			m_cmbo_filter_size.AddString(str);
		}
	}

	if (m_cmbo_filter_size.GetCount() > 0)
	{
		m_cmbo_filter_size.SetCurSel(m_filter_param[filterID].kernel_size_sel);

		//�ޔ������Ă����l�Ɠ������̂�����ΑI������
		for (int i = 0; i < m_cmbo_filter_size.GetCount(); i++){
			CString sizeStr;
			m_cmbo_filter_size.GetLBText(i, sizeStr);//���ݑI�𒆂̒l��ێ�
			if (sizeStr == oldSizeStr){
				m_cmbo_filter_size.SetCurSel(i);
				m_filter_param[filterID].kernel_size_sel = i;
				break;
			}
		}
	}


	//padding
	int padding_type = m_filter_param[filterID].padding_type;
	if (padding_type >= 0) 
	{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_STATIC_FILTER_CTRL_PAD), true);//�upadding�v�O���[�v�{�b�N�X
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD1), true);//�u�[���v���W�I�{�^��
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD2), true);//�u���Ӊ�f�v���W�I�{�^��
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD0), true);//�u�܂�Ԃ��v���W�I�{�^��

		if (padding_type == 1) {
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD1, true);
		}
		else if (padding_type == 2) {
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD2, true);
		}
		else {
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD0, true);
		}
	}
	else {
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_STATIC_FILTER_CTRL_PAD), false);//�upadding�v�O���[�v�{�b�N�X
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD1), false);//�u�[���v���W�I�{�^��
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD2), false);//�u���Ӊ�f�v���W�I�{�^��
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_CTRL_PAD0), false);//�u�܂�Ԃ��v���W�I�{�^��
	}



	//�~�`
	int circle_shape = m_filter_param[filterID].circle_shape;
	if (circle_shape >= 0) 
	{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE), SW_SHOW);//�u�~�`�}�X�N�v�`�F�b�N�{�b�N�X

		if (circle_shape == 1) {
			API.SetCheck(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE, true);
		}
		else {
			API.SetCheck(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE, false);
		}
	}
	else {
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_CHECK_FILTER_CTRL_CIRCLE), SW_HIDE);//�u�~�`�}�X�N�v�`�F�b�N�{�b�N�X
	}


	//�ɐ�
	int direction = m_filter_param[filterID].direction;
	if (direction >= 0) 
	{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_XY), SW_SHOW);//�u�c�������v���W�I�{�^��
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_X), SW_SHOW);//�u�������̂݁v���W�I�{�^��
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_Y), SW_SHOW);//�u�c�����̂݁v���W�I�{�^��

		if (direction == 1) {//���̂�
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_X,true);
		}
		else if (direction == 2) {//�c�̂�
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_Y, true);
		}
		else {//�c��
			API.SetCheck(m_hWnd, IDC_RADIO_FILTER_XY, true);
		}
	}
	else
	{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_XY), SW_HIDE);//�u�c�������v���W�I�{�^��
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_X), SW_HIDE);//�u�������̂݁v���W�I�{�^��
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_RADIO_FILTER_Y), SW_HIDE);//�u�c�����̂݁v���W�I�{�^��
	}

	//�R���{�����[�V�����J�[�l��
	if (filterID == FILTER_CONVOLUTION)
	{
		set_conv_kernel(m_filter_param[filterID].kernel_size[m_filter_param[filterID].kernel_size_sel]);
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_CTRL_CONV_KERNEL), SW_SHOW);
	}
	else{
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_EDIT_FILTER_CTRL_CONV_KERNEL), SW_HIDE);
	}



	//���̑��̃p�����^
	int			caption[] = { IDC_STATIC_FILTER_CTRL_PARAM0 ,IDC_STATIC_FILTER_CTRL_PARAM1, IDC_STATIC_FILTER_CTRL_PARAM2, IDC_STATIC_FILTER_CTRL_PARAM3, IDC_STATIC_FILTER_CTRL_PARAM4, IDC_STATIC_FILTER_CTRL_PARAM5, IDC_STATIC_FILTER_CTRL_PARAM6 };
	int			ctrlID[] = { IDC_EDIT_FILTER_PARAM0 ,IDC_EDIT_FILTER_PARAM1, IDC_EDIT_FILTER_PARAM2, IDC_EDIT_FILTER_PARAM3, IDC_EDIT_FILTER_PARAM4, IDC_EDIT_FILTER_PARAM5, IDC_EDIT_FILTER_PARAM6 };

	for (int i = 0; i < FILTER_PARAM_MAX; i++) 
	{
		if (m_filter_param[filterID].param_name[i][0] != 0) 
		{
			::SetWindowText(::GetDlgItem(m_hWnd, caption[i]), CString(m_filter_param[filterID].param_name[i]));
			API.SetEditValue(m_hWnd, ctrlID[i], m_filter_param[filterID].param[i]);

			::ShowWindow(::GetDlgItem(m_hWnd, caption[i]), SW_SHOW);
			::ShowWindow(::GetDlgItem(m_hWnd, ctrlID[i]), SW_SHOW);
		}
		else {
			::ShowWindow(::GetDlgItem(m_hWnd, caption[i]), SW_HIDE);
			::ShowWindow(::GetDlgItem(m_hWnd, ctrlID[i]), SW_HIDE);
		}
	}

			

}



void CFilterCtrlDlg::OnCbnSelchangeComboFilterSize()
{
	int filterID = m_cmbo_filter_type.GetCurSel();

	m_filter_param[filterID].kernel_size_sel = m_cmbo_filter_size.GetCurSel();
	
	set_conv_kernel(m_filter_param[filterID].kernel_size[m_filter_param[filterID].kernel_size_sel]);
}


/********************************************************************
�@  �\  ��  �� : �\���摜�����摜�ɂ���
��    ��    �� : OnBnClickedButtonBinalizeSetOrgImg
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::OnBnClickedButtonFilterCtrlSetOrgImg()
{
	int orgNo;

	orgNo = API.GetCurrentImageNumber();

	if(orgNo>=0 && orgNo+1<API.GetDataUnitNumber())
	{
		API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_FROM_IMAGE_NO, orgNo);
		API.SetEditValue(m_hWnd, IDC_EDIT_FILTER_TO_IMAGE_NO, orgNo+1);
	}

}

/********************************************************************
�@  �\  ��  �� : 
��    ��    �� : OnPaint
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CFilterCtrlDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	draw_kernel();
}