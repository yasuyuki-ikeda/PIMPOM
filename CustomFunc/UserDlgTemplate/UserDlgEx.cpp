// UserDlg.cpp : �����t�@�C��
//



#include "stdafx.h"
#include "UserDlgTemplate.h"
#include "UserDlgEx.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUserDlgEx�����o�֐�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CUserDlgEx, CDialog)


//�R���X�g���N�^
CUserDlgEx::CUserDlgEx(CWnd* pParent /*=NULL*/)
	: CDialog(CUserDlgEx::IDD, pParent)
{

	
}

//�f�X�g���N�^
CUserDlgEx::~CUserDlgEx()
{

}

//DDX
void CUserDlgEx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���C����ʑ���̃��b�Z�[�W �n���h��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//���C����ʕ`��
void CUserDlgEx::OnDrawMainImage(int image_number)
{

}

//���C����ʏ�ł̃}�E�X�{�^������
void CUserDlgEx::OnMouseDownOnImage(CPoint point)
{

}

//���C����ʏ�ł̃}�E�X�h���b�O��
void CUserDlgEx::OnMouseDrugOnImage(CPoint start_point, CPoint currnet_point)
{

}

//���C����ʏ�ł̃}�E�X�h���b�O�I��
void CUserDlgEx::OnFinishMouseDrugOnImage(CPoint start_point, CPoint currnet_point)
{

}

//TCP/IP�ʐM�ɂ��f�[�^��M
void CUserDlgEx::OnReceive(BYTE *data, int *ndata)
{

}

//�o�b�`���s�p
void CUserDlgEx::OnBatchExecute(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUserDlg���b�Z�[�W �n���h��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserDlgEx, CDialog)

END_MESSAGE_MAP()


BOOL CUserDlgEx::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}


