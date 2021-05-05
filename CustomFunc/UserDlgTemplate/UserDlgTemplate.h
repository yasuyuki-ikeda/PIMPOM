#pragma once

#include "resource.h"		// ���C�� �V���{��
#include "opencv_headers.h"
#include "PIMPOM_API.h"


// ***************************************************************************
//�@�֐���`
// ***************************************************************************
#define USER		__declspec(dllexport)		/* User Functions/Variables			*/




extern "C" {
USER	int				OpenUserDlg(CPimpomAPI *pAPI);//���[�U�_�C�A���O�̋N��
USER	void			DestroyUserDlg();//���[�U�_�C�A���O�̔j��
USER	CPimpomAPI*		GetAPI();
USER	void			OnDrawMainImage_UserDlg(int image_number);//���C����ʕ`��
USER	void			OnMouseDown_UserDlg(POINT point);//���C����ʏ�ł̃}�E�X�{�^������
USER	void			OnMouseDrug_UserDlg(POINT start_point, POINT currnet_point);//���C����ʏ�ł̃}�E�X�h���b�O
USER	void			OnFinishMouseDrug_UserDlg(POINT start_point, POINT currnet_point);//���C����ʏ�ł̃}�E�X�h���b�O�I��
USER	void			OnBatchExecute_UserDlg(BATCH_INPUT *in_param, BATCH_OUTPUT *out_param);//�o�b�`���s
USER	void			OnReceive_UserDlg(BYTE *data, int *ndata);//TCP/IP�ʐM��M
}

#define API		(*GetAPI())


