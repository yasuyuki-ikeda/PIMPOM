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
#pragma once


///*************************************
//         �}  �N  ��   ��   �`         
///*************************************
#define	FILTER_COUNT			(25)//�t�B���^��
#define FILTER_PARAM_MAX		(7)//�p�����^�ő吔
#define FILTER_SIZE_LIST_MAX	(100)

#define FILTER_MEAN				(0)//�ړ�����
#define FILTER_MEAN_M			(1)//�ő�E�ŏ����������ړ�����
#define FILTER_GAUSS			(2)//�K�E�X
#define FILTER_MEDIAN			(3)//���f�B�A��
#define FILTER_DILATION			(4)//�c��
#define FILTER_EROSION			(5)//���k
#define FILTER_OPENING			(6)//�I�[�v�j���O
#define FILTER_CLOSING			(7)//�N���[�W���O
#define FILTER_BILATERAL		(8)//�o�C���e�����t�B���^
#define FILTER_SOBEL			(9)//�G�b�W���o(sobel)
#define FILTER_LOG				(10)//Laplacian of Gaussian
#define FILTER_GABOR			(11)//�K�{�[���t�B���^
#define FILTER_SIGMOID			(12)//�G�b�W���o(�V�O���C�h)
#define FILTER_LOG_ZEROCLS		(13)//�G�b�W�א���(LoG�[���N���X)
#define FILTER_CANNY			(14)//�G�b�W�א���(Canny)
#define FILTER_GAUSS_UNSHADE	(15)//�V�F�[�f�B���O����
#define FILTER_NEGA				(16)//�Z�x���]
#define FILTER_MIRROR_LR		(17)//�����i���E�j
#define FILTER_MIRROR_UD		(18)//�����i�㉺�j
#define FILTER_ROT180			(19)//180��
#define FILTER_CONVOLUTION		(20)//�R���{�����[�V����
#define FILTER_CLAHE			(21)//CLAHE (Contrast Limited Adaptive Histogram Equalization)
#define FILTER_LOCAL_CONTRAST	(22)//�Ǐ��R���g���X�g�g��(sliding window�� 255*( x - ave - s*��)/(2*��) )
#define FILTER_LOCAL_CONTRAST_P	(23)//�Ǐ��R���g���X�g�g��(�p�b�`�������� (x-min)/(max-min))
#define FILTER_DFT				(24)//���U�t�[���G�ϊ�


// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

//�t�B���^���
typedef struct _FILTER_CTRL_PARAMS{
	char	name[100];//�t�B���^��
	int		kernel_size[FILTER_SIZE_LIST_MAX];//�J�[�l���T�C�Y���X�g
	int		kernel_size_sel;//�I�𒆂̃J�[�l���T�C�Y
	int		padding_type;	//padding  (����:�ݒ�Ȃ�  1:�O���̉�f������  2:�O���Ő܂�Ԃ�   ����ȊO:�[��)
	int		direction;//�t�B���^����   �i����:�ݒ�Ȃ�  1:�� 2:�c ����ȏ�:�c���j�@
	int		circle_shape;//�~�`�̃t�B���^�ɂ��邩�ǂ��� �i����:�ݒ�Ȃ�  0: ���Ȃ� ����ȏ�F����) 
	float	param[FILTER_PARAM_MAX];//�p�����^�̒l
	char	param_name[FILTER_PARAM_MAX][100];//�p�����^����

}FILTER_CTRL_PARAMS;

///*************************************
//         �N���X   ��   �`         
///*************************************


class CFilterCtrlDlg : public CDialog
{
	enum { IDD = IDD_FILTER_CTRL_DLG };// �_�C�A���O �f�[�^

public:
	CFilterCtrlDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^





protected:
	CComboBox	m_cmbo_filter_type;
	CComboBox	m_cmbo_filter_size;
	FILTER_CTRL_PARAMS	m_filter_param[FILTER_COUNT];//�t�B���^���
	CImageField *pKernelIF;
	CDataUnit	*pKernelDU;
	float		*pConvolutionKernel;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	bool	execute(int src_num, int dst_num);
	void	show_dlg_item();
	void	init_filter_params();
	void	draw_kernel();
	void	set_conv_kernel(int kernel_size);
	bool	get_conv_kernel(int *pkernel_size, float *pkernel);

	
//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnFilterExec();
	afx_msg void OnBnClickedRadioFilterOverwite();
	afx_msg void OnBnClickedRadioFilterToImage();
	afx_msg void OnCbnSelchangeComboFilterType();
	afx_msg void OnBnClickedButtonFilterCtrlSetOrgImg();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeComboFilterSize();
};
