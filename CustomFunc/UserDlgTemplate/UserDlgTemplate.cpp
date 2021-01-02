// UserDlgTemplate.cpp : DLL �̏��������[�`���ł��B
//

#include "stdafx.h"
#include "UserDlgTemplate.h"
#include "UserDlgEx.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ***************************************************************************
//�@�J�X�^���_�C�A���O�̃I�u�W�F�N�g
// ***************************************************************************
CUserDlgEx		*p_user_dlg = NULL;
CPimpomAPI		*p_api =NULL;

// ***************************************************************************
//  PimomCore���猩����֐�����
// ***************************************************************************

//���[�U�_�C�A���O�̋N��
USER	int		OpenUserDlg(CPimpomAPI *pAPI)
{
	p_api = pAPI;

	if(p_api){
		if(!p_user_dlg){
			p_user_dlg = new  CUserDlgEx();
			if(p_user_dlg){
				p_user_dlg->Create(IDD_USER_DLG_EX);	
			}
		}
		p_user_dlg->ShowWindow(SW_SHOW);
	}

	return 0;
}

//���[�U�_�C�A���O�̔j��
USER	void		DestroyUserDlg()
{
	if(p_user_dlg){
		delete	p_user_dlg;
		p_user_dlg = NULL;
		p_api = NULL;
	}
}


//PIMPOM_API�ւ̃|�C���^�Q��
USER	CPimpomAPI*		GetAPI()
{
	return p_api;
}


//���C����ʕ`��
USER	void	OnDrawMainImage_UserDlg(int image_number)
{
	if (p_user_dlg){
		if (p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnDrawMainImage(image_number);
		}
	}
}

//���C����ʏ�ł̃}�E�X�{�^������
USER	void	OnMouseDown_UserDlg(POINT point)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnMouseDownOnImage(point);
		}
	}
}

//���C����ʏ�ł̃}�E�X�h���b�O
USER	void	OnMouseDrug_UserDlg(POINT start_point, POINT currnet_point)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnMouseDrugOnImage(start_point, currnet_point);
		}
	}
}

//���C����ʏ�ł̃}�E�X�h���b�O�I��
USER	void	OnFinishMouseDrug_UserDlg(POINT start_point, POINT currnet_point)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnFinishMouseDrugOnImage(start_point, currnet_point);
		}
	}
}

//�o�b�`���s�p
USER	void	OnBatchExecute_UserDlg(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnBatchExecute(in_param,out_param);
		}
	}
}

//TCP/IP�ʐM��M
USER	void	OnReceive_UserDlg(BYTE *data, int *ndata)
{
	if(p_user_dlg){
		if(p_user_dlg->IsWindowVisible()){
			p_user_dlg->OnReceive(data, ndata);
		}
	}
}