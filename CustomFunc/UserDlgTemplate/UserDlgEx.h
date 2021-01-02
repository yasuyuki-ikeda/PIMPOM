#pragma once

#include "resource.h"		// ���C�� �V���{��


#include	"afxcmn.h"


// CUserDlg �_�C�A���O

class AFX_EXT_CLASS CUserDlgEx : public CDialog
{
	DECLARE_DYNAMIC(CUserDlgEx)

public:
	CUserDlgEx(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CUserDlgEx();
	void		OnDrawMainImage(int image_number);//���C����ʕ`��
	void		OnMouseDownOnImage(CPoint point);//���C����ʏ�ł̃}�E�X�{�^������
	void		OnMouseDrugOnImage(CPoint start_point, CPoint currnet_point);//���C����ʏ�ł̃}�E�X�h���b�O��
	void		OnFinishMouseDrugOnImage(CPoint start_point, CPoint currnet_point);//���C����ʏ�ł̃}�E�X�h���b�O�I��
	void		OnBatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);//�o�b�`���s�p
	void		OnReceive(BYTE *data, int *ndata);//TCP/IP�ʐM����M����

	// �_�C�A���O �f�[�^
	enum { IDD = IDD_USER_DLG_EX };

	


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()


};
