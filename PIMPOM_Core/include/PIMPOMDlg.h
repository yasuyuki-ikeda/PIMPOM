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
//CPIMPOMDlg
//���C���E�B���h�E
/*******************************************************************/


//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#pragma once
#include "ImageField.h"

//**************************************
//         �萔��`        
//**************************************
//���b�Z�[�W
#define WM_EXTERNAL_INPUT    WM_USER+100
#define WM_EXTERNAL_OUTPUT	 WM_USER+102
#define WM_EXTERNAL_PROCESS    WM_USER+101

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************

///*************************************
//         �N���X   ��   �`         
///*************************************
class PIMPOM_EXT CPIMPOMDlg : public CDialog
{
	enum { IDD = IDD_PIMPOM_DLG };// �_�C�A���O �f�[�^


public:
	CPIMPOMDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^
	void			DrawImage(bool redraw_background);//���C���摜��`�悷��
	void			DispDataValue( double point_x, double point_y );//�w�肵�����W�i�E�C���h�E�̃N���C�A���g�̈�j�̃f�[�^��\������
	CImageField*	GetMainImage();//���C���摜(CImageField)�ւ̃|�C���^���擾����
	void			StopMovie();//�r�f�I�Đ���~
	void			ShowMessage(bool do_clear, int num, bool force, CString str);//���b�Z�[�W��\������
	void			SetClickedCoordinate(CPoint point);//�N���b�N���W��ݒ肷��
	void			BackGroundProc();

	CImageField		*p_image_disp_main;//���C���摜
	CString			cmdLine;//�R�}���h���C��


protected:
	HICON			m_hIcon;//�A�C�R��
	double			latest_clicked_coordinate_x, latest_clicked_coordinate_y;//�ł��Ō�ɃN���b�N�������W(�摜�]���O�̃f�[�^���W)
	double			latest_mbutton_clicked_coordinate_x, latest_mbutton_clicked_coordinate_y;//�ł��Ō�ɃN���b�N�������W(�摜�]���O�̃f�[�^���W)
	bool			is_draging;//�}�E�X�h���b�O���ł��邱�Ƃ�\���t���O
	bool			is_draging_mbutton;//�}�E�X�����{�^���Ńh���b�O���ł��邱�Ƃ�\���t���O
	bool			do_draw_mark;//�}�[�N��`�����߂̃t���O
	CBitmap			bmp_button_3d[4], bmp_button_complex[4], bmp_button_xy[2], bmp_button_xyz[4],//�{�^���Ɋ֘A�t����r�b�g�}�b�v�I�u�W�F�N�g
		bmp_button_rgb[4],
		bmp_button_paint, bmp_button_calc, bmp_button_mark, bmp_button_work_area, bmp_button_mask, bmp_button_histgram,
		bmp_button_work_area_edit, bmp_button_mark_edit, bmp_button_calc_distance, bmp_button_calc_distance2,
		bmp_button_scale_adjust_field, bmp_button_no_scale,
		bmp_button_video, bmp_button_video2, bmp_button_inc, bmp_button_dec;
	CString			message[2];//���b�Z�[�W�{�b�N�X�ɕ\�����镶����
	CToolTipCtrl	m_toolTip;
	double			calc_begin_x, calc_begin_y, calc_end_x, calc_end_y;//�����̌v�Z���s���n�_�ƏI�_
	CImageField		*p_image_disp_piece;//�\������f�[�^�l�̎��ӂ̔Z�x���z��\�����邽�߂̉摜
	HANDLE			hThread;//���C���v���Z�X�̃X���b�h

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	void movie();//�r�f�I�Đ�
	void set_data_select_button(long type);//�\���f�[�^�I���{�^����ݒ肷��
	void draw_2points_distance();//2�_�Ԃ̋�����\������
	void draw_marks();//�}�[�N��\��
	void draw_work_area();//��Ɨ̈��`��
	void arrange_data_value_disp_field();//�f�[�^�̒l�\��������̈��z�u����
	void arrange_message_disp_field();//���b�Z�[�W��\�����邽�߂̃_�C�A���O�A�C�e���̔z�u����
	void change_disp_data_by_button(short button_number);//�{�^�����������Ƃ��Ƀf�[�^�\���������Ȃ�
	void set_bitmap_for_data_button();//�f�[�^�I���{�^���Ƀr�b�g�}�b�v���֘A�Â���
	void set_bitmap_for_layer_button();//���C���[�I���{�^���Ƀr�b�g�}�b�v���֘A�t����
	void arrange_buttons();//�{�^����z�u����
	HBITMAP set_bitmap_button(HBITMAP h_bmp, UINT id);//id�Ŏ������{�^���Ƀr�b�g�}�b�v���֘A�Â���
	void load_bitmaps();//�{�^���ɕ\������r�b�g�}�b�v��ǂݍ���
	void disp_information_titlebar();//�^�C�g���o�[�ɉ摜�̖��O�Ǝ�ނ�\��
	void set_scrollbar( CScrollBar &sc, int page, int pos, int min, int max);//�X�N���[���o�[�̐ݒ�
	long control_main_image_scrollbar(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);///���C����ʕ\���̂��߂̃X�N���[���o�[�̑���
	void on_change_color_palette(short palette_id);//�[���J���[�p���b�g�̕ύX


//���b�Z�[�W�n���h��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPimponDlgDataSelect0();
	afx_msg void OnPimponDlgDataSelect1();
	afx_msg void OnPimponDlgDataSelect2();
	afx_msg void OnPimponDlgDataSelect3();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPimponDlgButtonPaint();
	afx_msg void OnPimponDlgButtonCalc();
	afx_msg void OnPimponDlgButtonWorkArea();
	afx_msg void OnDestroy();
	afx_msg void OnPimponDlgButtonMark();
	afx_msg void OnPimponDlgButtonRulerTool();
	afx_msg void OnToolRuler();
	afx_msg void OnBnClickedButtonPimponDlgCalcDistance2();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenSerial();
	afx_msg void OnPimponDlgButtonMask();
	afx_msg void OnFileSaveData();
	afx_msg void OnFileSaveView();
	afx_msg void OnFileSaveImage();
	afx_msg void OnPimponDlgButtonScaleDbl();
	afx_msg void OnPimponDlgButtonScaleHlf();
	afx_msg void OnPimponDlgButtonScaleAdjustFiled();
	afx_msg void OnPimponDlgButtonNoScale();
	afx_msg void OnFileSaveSerial();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnEditUndo();
	afx_msg void OnEditConvertType();
	afx_msg void OnEditResize();
	afx_msg void OnCalcImage();
	afx_msg void OnFilter();
	afx_msg void OnEditDelete();
	afx_msg void OnFilePrint();
	afx_msg void OnEditCopyClipbord();
	afx_msg void OnViewColorGrayscale();
	afx_msg void OnViewColorNega();
	afx_msg void OnViewColorRainbow();
	afx_msg void OnViewColorRainbow2();
	afx_msg void OnViewColorCosine();
	afx_msg void OnViewColorYtob();
	afx_msg void OnViewMakefont();
	afx_msg void OnViewColorGtor();
	afx_msg void OnFileQuit();
	afx_msg void OnHelpVersion();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnBnClickedCheckVideoCapture();
	afx_msg void OnToolVideocapture();
	afx_msg void OnImageTransform();
	afx_msg void OnEditPaint();
	afx_msg void OnToolSocket();
	afx_msg void OnBinalize();
	afx_msg void OnBnClickedPimponDlgCheckVideoCapture2();
	afx_msg void OnToolBatch();
	afx_msg void OnCustomAdd();
	afx_msg void OnMenuCustomFunc(UINT uID);
	afx_msg void OnMenuLoadIOLog(UINT uID);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg LRESULT OnMessageExtInput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageExtOutput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageExtProcess(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTcnSelchangeTabPimpomDlgMessage(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPimponDlgHistgram();
	afx_msg void OnEditRename();
	afx_msg void OnEditPasteClipbord();
	afx_msg void OnEditDeleteAll();
	afx_msg void OnHelpSpecificationSheet();
	afx_msg void OnHelpApireference();
	afx_msg void OnHelpApireference2();
	afx_msg void OnHelpApireference3();
	afx_msg void OnViewMultiImageDisp();
	afx_msg void OnFileSaveAllimage();
	afx_msg void OnViewColorLabel();
	afx_msg void OnToolCommandPrompt();
	afx_msg void OnEditMark();
	afx_msg void OnEditWorkarea();
	afx_msg void OnViewMark();
	afx_msg void OnViewWorkarea();
	afx_msg void OnToolHistgram();
	afx_msg void OnViewSetupEnvironment();
	afx_msg void OnView3dImgDisp();
	afx_msg void OnFileLoadAllimage();
	afx_msg void OnToolPythonBatch();
};


