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
//CMarkDlg
//�}�[�N�̐ݒ�E�B���h�E
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "MarkDlg.h"

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


CMarkDlg::CMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkDlg)
	// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_MARK, m_list_contol);
}


/********************************************************************
�@  �\  ��  �� : ���X�g�̑I�𒆂̗v�f�ԍ���Ԃ�
��    ��    �� : get_selected_list_item
��          �� : 
��    ��    �l : ���X�g�̑I�𒆂̗v�f�ԍ�
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int	CMarkDlg::get_selected_list_item()
{
	return m_list_contol.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
}

/********************************************************************
�@  �\  ��  �� : ���X�g�̗v�f��I������
��    ��    �� : select_list_item
��          �� : int n (in)���X�g�̗v�f�ԍ�
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::select_list_item(int n)
{
	m_list_contol.SetItemState(n, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_list_contol.SetSelectionMark(n);
	m_list_contol.EnsureVisible(n,true);
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
void CMarkDlg::show_dlg_item()
{
	RECT clientRct={0};//�N���C�A���g�̈��\����`
	int clientWidth, clientHeight;

	::GetClientRect(m_hWnd, &clientRct);//�N���C�A���g�̈��\����`���擾

	clientWidth = (clientRct.right - clientRct.left);
	clientHeight = (clientRct.bottom - clientRct.top);




	//�}�[�N�ǉ�
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_MARK_TYPE0), 0, 0, 30, 30, true);//�_
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_MARK_TYPE1), 30, 0, 30, 30, true);//��

	//�}�[�N�폜
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_MARK_DELETE), 60, 0, 30, 30, true);

	//�}�[�N�S�폜
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_MARK_ALLDELETE), 90, 0, 30, 30, true);
	
	//�}�[�N�ԍ�����ʕ\�����邩�ǂ�����I������`�F�b�N�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_CHECK_MARK_NUM_DISP), 120, 0, 30, 30, true);

	//�}�[�N�ҏW
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_BUTTON_MARK_EDIT), 150, 0, 30, 30, true);

	//�l��\�����郊�X�g�{�b�N�X
	::MoveWindow(::GetDlgItem(m_hWnd, IDC_LIST_MARK), 0, 30, clientWidth, clientHeight - 40, true);
}



/********************************************************************
�@  �\  ��  �� : ���X�g������
��    ��    �� : init_list
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::init_list()
{
    LVCOLUMN    lvc;
    char       caption[32];

	m_list_contol.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;    // �L���t���O


	//No.
	sprintf(caption, "ID");
    lvc.iSubItem    = 0;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 30;          // ����
    m_list_contol.InsertColumn(0, &lvc);


	//Type.
	sprintf(caption, "Type");
    lvc.iSubItem    = 1;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 20;          // ����
    m_list_contol.InsertColumn(1, &lvc);

	//X
	sprintf(caption, "x");
    lvc.iSubItem    = 2;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 50;          // ����
    m_list_contol.InsertColumn(2, &lvc);
   
	//Y
	sprintf(caption, "y");
    lvc.iSubItem    = 3;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 50;          // ����
    m_list_contol.InsertColumn(3, &lvc);

	//X
	sprintf(caption, "x2");
    lvc.iSubItem    = 4;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 50;          // ����
    m_list_contol.InsertColumn(4, &lvc);
   
	//Y
	sprintf(caption, "y2");
    lvc.iSubItem    = 5;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 50;          // ����
    m_list_contol.InsertColumn(5, &lvc);

	//value
	sprintf(caption, "pt.val");
    lvc.iSubItem    = 6;            // �T�u�A�C�e���ԍ�
    lvc.pszText     = caption;   // ���o���e�L�X�g
    lvc.cx          = 50;          // ����
    m_list_contol.InsertColumn(6, &lvc);

	//attribute
	for(int i=0 ; i<MARK_ATTRIB_NUM ; i++)
	{
		sprintf(caption, "attrib%d",i);
		lvc.iSubItem    = 7+i;            // �T�u�A�C�e���ԍ�
		lvc.pszText     = caption;   // ���o���e�L�X�g
		lvc.cx          = 50;          // ����
		m_list_contol.InsertColumn(7+i, &lvc);
	}
}


/********************************************************************
�@  �\  ��  �� : ���X�g�v�f�\�[�g�̂��߂̔�r�֐�
��    ��    �� : compare_func
��          �� : 
��    ��    �l : 
�@          �\ : ��r�Ώۂ������Ƃ����l�Ȃ�΁A�傫���Ŕ�r
�@�@�@�@�@�@�@�@����ȊO�̏ꍇ�͕�����̏��Ŕ�r�@�@�@�@�@
		 ���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CALLBACK CMarkDlg::compare_func(LPARAM param1, LPARAM param2, LPARAM param3)
{
	CMarkDlg* pDlg = (CMarkDlg*)param3;
	int nSubItem  = pDlg->m_iSubItem;
	CString    strItem1 = pDlg->m_list_contol.GetItemText(param1, nSubItem);
	CString    strItem2 = pDlg->m_list_contol.GetItemText(param2, nSubItem);

	double numItem1 = atof(strItem1);
	double numItem2 = atof(strItem2);
	if(numItem1!=0.0 && numItem2!=0.0)//���l�ǂ����̔�r
	{
		if(pDlg->m_sort_dir_flg)	
			return (numItem1>numItem2);
		else
			return (numItem1<numItem2);
	}
	else//������ǂ����̔�r
	{
		if(pDlg->m_sort_dir_flg)
			return strcmp(strItem1, strItem2);
		else
			return strcmp(strItem2, strItem1);
	}
}


/********************************************************************
�@  �\  ��  �� : �}�[�N�ǉ��̃^�C�v
��    ��    �� : GetAddMarkTipe
��          �� : -1:�Ȃ��@0:�_�@1:��
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
int CMarkDlg::GetMarkEditType()
{
	if( API.GetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0) )
	{
		return MARK_TYPE_POINT;
	}
	else if(API.GetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1))
	{
		return MARK_TYPE_LINE;
	}
	else 
	{
		return -1;
	}
}


/********************************************************************
�@  �\  ��  �� : �}�[�N�ǉ��̃^�C�v
��    ��    �� : GetAddMarkTipe
��          �� : -1:�Ȃ��@0:�_�@1:��
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::SetMarkEditType(int type)
{
	if (type== MARK_TYPE_POINT)
	{
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0, true);
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1, false);
	}
	else if (type == MARK_TYPE_LINE)
	{
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0, false);
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1, true);
	}
	else
	{
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0, false);
		API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1, false);
	}
}


/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ń}�E�X�N���b�N���ꂽ�Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseDownOnImage
��          �� : 
��    ��    �l :
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnMouseDownOnImage(float point_x, float point_y)
{
	if (!IsWindowVisible())	return ;//��\����Ԃł͖���


	if (GetMarkEditType()== MARK_TYPE_POINT)
	{
		API.AddMark(CURRENT_IMAGE, point_x, point_y);//�_�}�[�N�ǉ�
	}
	else if (GetMarkEditType() == MARK_TYPE_LINE)
	{

	}
	else {
		
	}

	SelectMarkFromPoint(CURRENT_IMAGE, point_x, point_y);

}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ń}�E�X�h���b�O���ꂽ�Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseDrugOnImage
��          �� : 
��    ��    �l :
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnMouseDrugOnImage(float point0_x, float point0_y, float point1_x, float point1_y)
{
	if (!IsWindowVisible())	return ;//��\����Ԃł͖���

	if (GetMarkEditType() == MARK_TYPE_POINT)
	{

	}
	else if (GetMarkEditType() == MARK_TYPE_LINE)
	{
		API.DrawImage(CURRENT_IMAGE, false);//�ĕ`��
		API.DrawLine(point0_x, point0_y, point1_x, point1_y);
	}
	else {

	}

}


/********************************************************************
�@  �\  ��  �� : ���C����ʏ�Ń}�E�X�N���b�N���O���ꂽ�Ƃ��ɌĂ΂��֐�
��    ��    �� : OnMouseUpOnImage
��          �� : 
��    ��    �l :
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnMouseUpOnImage(float point0_x, float point0_y, float point1_x, float point1_y)
{
	if (!IsWindowVisible())	return ;//��\����Ԃł͖���

	if (GetMarkEditType() == MARK_TYPE_POINT)
	{

	}
	else if (GetMarkEditType() == MARK_TYPE_LINE)
	{
		API.AddLineMark(CURRENT_IMAGE, point0_x, point0_y, point1_x, point1_y);//���}�[�N�ǉ�
	}
	else {

	}
}

/********************************************************************
�@  �\  ��  �� : ���C����ʏ�ɕ`��
��    ��    �� : OnDrawOnImage
��          �� : 
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnDrawOnImage()
{
	if (!IsWindowVisible())	return;//��\����Ԃł͖���

	CDataUnit	*p_du = API.GetDataUnit(CURRENT_IMAGE);//���ݑI�𒆂̉摜��DataUnit���擾����
	if (p_du == NULL) {
		return;//�f�[�^��������Ή������Ȃ�
	}


	for (int n = 0; n<p_du->GetMarkNumber(); n++)
	{
		CMark mark;
		p_du->GetMark(n, &mark);

		if (mark.type == MARK_TYPE_POINT) 
		{//�_�̏ꍇ
			if (n == highlight_mark_id)
			{
				API.DrawCursor(mark.figure.point.x, mark.figure.point.y, 2, RGB(255, 0, 0));//�\���J�[�\����Ԃŕ`��
			}
			else
			{
				API.DrawCursor(mark.figure.point.x, mark.figure.point.y);
			}
		}
		else 
		{//�����̏ꍇ
			if (n == highlight_mark_id)
			{
				API.DrawCircle(mark.figure.line.x1, mark.figure.line.y1, 2.0, 1, RGB(255, 0, 0));//�n�_�ɉ~������
				API.DrawLine(mark.figure.line.x1, mark.figure.line.y1, mark.figure.line.x2, mark.figure.line.y2, 2, RGB(255, 0, 0));//����Ԃŕ\��
			}
			else
			{
				API.DrawLine(mark.figure.line.x1, mark.figure.line.y1, mark.figure.line.x2, mark.figure.line.y2);
			}
		}

		if (API.GetCheck(m_hWnd, IDC_CHECK_MARK_NUM_DISP))
		{//�}�[�N�̔ԍ�����ʂɕ`�悷��
				CString		str;
				str.Format("%d", n);
				API.DrawText(mark.figure.point.x, mark.figure.point.y, str, RGB(0, 0, 0));
		}
	}

	highlight_mark_id = -1;//�I�����O��
}


/********************************************************************
�@  �\  ��  �� : �}�[�N�̃��X�g�\��
��    ��    �� : ShowMarkVal
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::ShowMarkVal(long image_num)
{
	CDataUnit	*pdu=NULL;
	LVITEM       lvi={0};
	lvi.mask = LVIF_TEXT;
	char        buf[1024];
	

	m_list_contol.DeleteAllItems();//��U���ׂẴ��X�g�v�f���N���A

	pdu = API.GetDataUnit(image_num);
		if(pdu==NULL)	return;


	for(int i=0 ; i<pdu->GetMarkNumber() ; i++)
	{
		//API.GetMarkVal(image_num, i, &x, &y, &val, attrib);
		CMark mark;
		pdu->GetMark(i, &mark);

		//No.
		lvi.iItem = i;
		lvi.iSubItem = 0;
		sprintf(buf, "%d", i);
		lvi.pszText = buf;
		m_list_contol.InsertItem(&lvi);//���X�g�}��


		if(mark.type == MARK_TYPE_POINT)
		{
			//Type.
			lvi.iItem = i;
			lvi.iSubItem = 1;
			sprintf(buf, "P");
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//x
			lvi.iItem = i;
			lvi.iSubItem = 2;
			sprintf(buf, "%0.3f", mark.figure.point.x);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//y
			lvi.iItem = i;
			lvi.iSubItem = 3;
			sprintf(buf, "%0.3f", mark.figure.point.y);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//data value
			lvi.iItem = i;
			lvi.iSubItem = 6;
			sprintf(buf, "%lf", pdu->GetDataValue( mark.figure.point.x, mark.figure.point.y ));
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);
		}
		else
		{
			//Type.
			lvi.iItem = i;
			lvi.iSubItem = 1;
			sprintf(buf, "L");
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//x
			lvi.iItem = i;
			lvi.iSubItem = 2;
			sprintf(buf, "%0.3f", mark.figure.line.x1);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//y
			lvi.iItem = i;
			lvi.iSubItem = 3;
			sprintf(buf, "%0.3f", mark.figure.line.y1);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//x2
			lvi.iItem = i;
			lvi.iSubItem = 4;
			sprintf(buf, "%0.3f", mark.figure.line.x2);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//y2
			lvi.iItem = i;
			lvi.iSubItem = 5;
			sprintf(buf, "%0.3f", mark.figure.line.y2);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

			//data value
			lvi.iItem = i;
			lvi.iSubItem = 6;
			sprintf(buf, "" );
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);

		}

		//attribute
		for(int j=0 ; j<MARK_ATTRIB_NUM ; j++)
		{
			lvi.iItem = i;
			lvi.iSubItem = 7+j;
			sprintf(buf, "%lf", mark.attrib[j]);
			lvi.pszText = buf;
			m_list_contol.SetItem(&lvi);
		}
	}

}


/********************************************************************
�@  �\  ��  �� : �}�[�N�̑I��
��    ��    �� : SelectMarkFromPoint
��          �� : 
��    ��    �l : 
�@          �\ : �w�肳�ꂽ�_����ł��߂��}�[�N��T���A�I������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::SelectMarkFromPoint(long image_num, double x, double y)
{
	CDataUnit	*pdu=NULL;
	double		nearestDst=9999;
	int			nearestID=-1;

	pdu = API.GetDataUnit(image_num);
	if(pdu==NULL)	return;


	for(int i=0 ; i<pdu->GetMarkNumber() ; i++)
	{
		CMark mark;
		pdu->GetMark(i,&mark);

		double dist;
		if(mark.type == MARK_TYPE_POINT){//�_�̏ꍇ���Q�_�Ԃ̋����Ŕ��f
			//2�_�Ԃ̋���
			dist = sqrt( (mark.figure.point.x - x)*(mark.figure.point.x - x) + (mark.figure.point.y - y)*(mark.figure.point.y - y) );
		}
		else//�����̏ꍇ���n�_�ƏI�_�̈ʒu���画�f
		{
			float ip1 = (x - mark.figure.line.x1)*(mark.figure.line.x2 - mark.figure.line.x1) + (y - mark.figure.line.y1)*(mark.figure.line.y2 - mark.figure.line.y1);
			float ip2 = (x - mark.figure.line.x2)*(mark.figure.line.x1 - mark.figure.line.x2) + (y - mark.figure.line.y2)*(mark.figure.line.y1 - mark.figure.line.y2);
			
			if( ip1>0 && ip2>0 ){
				//�_(x,y)�ƒ���(x1,y1)-(x2,y2)�̋���
				float a = mark.figure.line.y1 - mark.figure.line.y2;
				float b = -mark.figure.line.x1 + mark.figure.line.x2;
				float c = (-b * mark.figure.line.y1) + (-a * mark.figure.line.x1);
				float dim = sqrt(a*a + b*b);
				if(dim!=0){	
					dist = fabs(a*x + b*y + c) / dim;
				}else{
					dist = 9999;
				}
			}else if(ip1>0){
				//�_(x,y)��(x2,y2)�̋���
				dist = sqrt( (mark.figure.line.x2 - x)*(mark.figure.line.x2 - x) + (mark.figure.line.y2 - y)*(mark.figure.line.y2 - y) );
			}else{
				//�_(x,y)��(x1,y1)�̋���
				dist = sqrt( (mark.figure.line.x1 - x)*(mark.figure.line.x1 - x) + (mark.figure.line.y1 - y)*(mark.figure.line.y1 - y) );
			}
		}

		if(dist < nearestDst){
			nearestDst = dist;
			nearestID = i;
		}
	}

	if(nearestID>=0)
	{
		highlight_mark_id = nearestID;
		select_list_item(nearestID);
	}

}

//////////////////////////////////////////////////////////////////////////////////////////
//  ���b�Z�[�W �n���h��
//////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMarkDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_MARK_NUM_DISP, OnCheckMarkNumDisp)
	ON_BN_CLICKED(IDC_BUTTON_MARK_ALLDELETE, OnButtonMarkAlldelete)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_MARK, &CMarkDlg::OnNMClickListMark)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_MARK, &CMarkDlg::OnLvnColumnclickListMark)
	ON_BN_CLICKED(IDC_BUTTON_MARK_DELETE, &CMarkDlg::OnBnClickedButtonMarkDelete)
	ON_BN_CLICKED(IDC_BUTTON_MARK_EDIT, &CMarkDlg::OnBnClickedButtonMarkEdit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MARK, &CMarkDlg::OnNMRClickListMark)
	ON_BN_CLICKED(IDC_CHECK_MARK_TYPE0, OnCheckMarkType0)
	ON_BN_CLICKED(IDC_CHECK_MARK_TYPE1, OnCheckMarkType1)
	ON_WM_SIZE()
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
BOOL CMarkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//�r�b�g�}�b�v�{�^��
	delbmp.LoadBitmap(IDB_BUTTON_WORKAREA_DEL);
	editbmp0.LoadBitmap(IDB_BUTTON_MARK_EDIT);
	editbmp1.LoadBitmap(IDB_BUTTON_MARK_EDIT_LINE);
	clearbmp.LoadBitmap(IDB_BUTTON_MARK_ALLDELETE);
	editbmp.LoadBitmap(IDB_BUTTON_EDIT);
	idbmp.LoadBitmap(IDB_BUTTON_MARK_ID);

	((CButton*)GetDlgItem(IDC_BUTTON_MARK_DELETE))->SetBitmap( (HBITMAP)delbmp ); 
	((CButton*)GetDlgItem(IDC_CHECK_MARK_TYPE0))->SetBitmap((HBITMAP)editbmp0);
	((CButton*)GetDlgItem(IDC_CHECK_MARK_TYPE1))->SetBitmap((HBITMAP)editbmp1);
	((CButton*)GetDlgItem(IDC_BUTTON_MARK_ALLDELETE))->SetBitmap((HBITMAP)clearbmp);
	((CButton*)GetDlgItem(IDC_BUTTON_MARK_EDIT))->SetBitmap((HBITMAP)editbmp);
	((CButton*)GetDlgItem(IDC_CHECK_MARK_NUM_DISP))->SetBitmap((HBITMAP)idbmp);

	//�c�[���`�b�v
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_MARK_TYPE0), "�_�}�[�N��ǉ�");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_MARK_TYPE1), "���}�[�N��ǉ�");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_MARK_DELETE), "�I�𒆂̃}�[�N���폜");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_MARK_ALLDELETE), "���ׂẴ}�[�N���폜");
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON_MARK_EDIT), "�}�[�N��ҏW");
	m_toolTip.AddTool(GetDlgItem(IDC_CHECK_MARK_NUM_DISP), "�}�[�N�ԍ���\��");


	show_dlg_item();//�_�C�A���O�A�C�e���̐���

	init_list();//���X�g������


	//�l�̏�����
	API.SetEditValue(m_hWnd, IDC_EDIT_MARK_FIND , 0);
	API.SetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0 , true);
	m_sort_dir_flg = true;
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}


/********************************************************************
�@  �\  ��  �� :�L�[�{�[�h����̓���
��    ��    �� : PreTranslateMessage
��          �� :
��    ��    �l :
�@          �\ : �J�[�\���L�[�F�J�[�\���̈ړ�
Del�L�[�F�I�𒆂̉摜�������N���A
Enter/Esc�L�[�F������
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
BOOL CMarkDlg::PreTranslateMessage(MSG* pMsg)
{
	POINT point;

	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_RETURN://Enter
			return  true;

		case VK_ESCAPE://Esc
			return  true;

		case VK_UP://��
			GetCursorPos(&point);
			SetCursorPos(point.x, point.y - 1);
			return  true;

		case VK_DOWN://��
			GetCursorPos(&point);
			SetCursorPos(point.x, point.y + 1);
			return  true;

		case VK_LEFT://��
			GetCursorPos(&point);
			SetCursorPos(point.x - 1, point.y);
			return  true;

		case VK_RIGHT://��
			GetCursorPos(&point);
			SetCursorPos(point.x + 1, point.y);
			return true;

		default:
			break;
		}
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		//�c�[���`�b�v�\��
		if (pMsg->hwnd == GetDlgItem(IDC_CHECK_MARK_TYPE0)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_MARK_TYPE1)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_MARK_DELETE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_MARK_ALLDELETE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_BUTTON_MARK_EDIT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_MARK_NUM_DISP)->m_hWnd
			)
		{
			m_toolTip.RelayEvent(pMsg);
		}
		else
		{
			m_toolTip.Pop();
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/********************************************************************
�@  �\  ��  �� : ���̃E�B���h�E��\����Ԃɂ���
��    ��    �� : OnShowWindow
��          �� :
��    ��    �l :
�@          �\ :
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);


}

/********************************************************************
�@  �\  ��  �� : �}�[�N�̔ԍ���ʕ`���ON�^OFF�ύX
��    ��    �� : OnCheckMarkNumDisp
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnCheckMarkNumDisp() 
{
	API.pPIMMOMDlg->DrawImage(true);
}



/********************************************************************
�@  �\  ��  �� : �S�����{�^������
��    ��    �� : OnButtonMarkAlldelete
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnButtonMarkAlldelete() 
{	
	if(! API.ClearMark(CURRENT_IMAGE) )	return;

	ShowMarkVal(CURRENT_IMAGE);
	API.pPIMMOMDlg->DrawImage(true);
}


/********************************************************************
�@  �\  ��  �� : �E�C���h�E����
��    ��    �� : OnClose
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnClose()
{
	API.SetMarkEditMode(0);

	CDialog::OnClose();
}


/********************************************************************
�@  �\  ��  �� : ���X�g�I��
��    ��    �� : OnNMClickListMark
��          �� : 
��    ��    �l : 
�@          �\ : �I�����ꂽ�J�[�\���̐F��ς��ĕ\��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnNMClickListMark(NMHDR *pNMHDR, LRESULT *pResult)
{
	CDataUnit	*pdu;
	int selectedid ;

	pdu = API.GetDataUnit(CURRENT_IMAGE);
	if( pdu==NULL )	return;

	if(m_list_contol.GetItemCount()==0)	return;

	selectedid = get_selected_list_item();
	if( 0<=selectedid && selectedid<pdu->GetMarkNumber())
	{
		CMark mark;
		pdu->GetMark(selectedid,&mark);
		highlight_mark_id = selectedid;

		
		if(mark.type == MARK_TYPE_POINT)//�_�}�[�N
		{
			API.pPIMMOMDlg->DispDataValue(mark.figure.point.x,mark.figure.point.y);
			API.pPIMMOMDlg->SetClickedCoordinate(CPoint(mark.figure.point.x, mark.figure.point.y));//���C���E�B���h�E�̃N���b�N���W���ړ�
			API.pPIMMOMDlg->DrawImage(true);
		}
		else//���}�[�N
		{
			API.pPIMMOMDlg->DispDataValue(mark.figure.line.x1, mark.figure.line.y1);
			API.pPIMMOMDlg->SetClickedCoordinate(CPoint(mark.figure.line.x1, mark.figure.line.y1));//���C���E�B���h�E�̃N���b�N���W���ړ�
			API.pPIMMOMDlg->DrawImage(true);
		}
	}else{
		API.pPIMMOMDlg->DrawImage(true);
	}

	*pResult = 0;
}


/********************************************************************
�@  �\  ��  �� : ���X�g�̌��o����������
��    ��    �� : OnLvnColumnclickListBatch
��          �� : 
��    ��    �l : 
�@          �\ : ���̍��ڂŃ��X�g�v�f���\�[�g����
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnLvnColumnclickListMark(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	m_sort_dir_flg = !m_sort_dir_flg;//�\�[�g������؂�ւ���
	m_iSubItem = pNMLV->iSubItem;//���X�g�̍��ڔԍ�
	m_list_contol.SortItems( compare_func, (LPARAM)this );

	for ( int i = 0; i < m_list_contol.GetItemCount(); i++ ){
			m_list_contol.SetItemData( i, (DWORD)i );
	}

	*pResult = 0;
}

/********************************************************************
�@  �\  ��  �� : �u�폜�v�{�^��������
��    ��    �� : OnBnClickedButtonMarkDelete
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnBnClickedButtonMarkDelete()
{
	CDataUnit	*pdu;
	int selectedid ;

	pdu = API.GetDataUnit(CURRENT_IMAGE);
	if( pdu==NULL )	return;

	if(m_list_contol.GetItemCount()==0)	return;

	selectedid = get_selected_list_item();
	if( 0<=selectedid && selectedid < pdu->GetMarkNumber())
	{
		pdu->DeleteMark(selectedid);

		API.DrawImage();
	}


}

/********************************************************************
�@  �\  ��  �� : �u�ҏW�v�{�^��������
��    ��    �� : OnBnClickedButtonMarkEdit
��          �� : 
��    ��    �l : 
�@          �\ : �ҏW�̂��߂̃_�C�A���O���o���B�ҏW��ɍĕ\������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnBnClickedButtonMarkEdit()
{
	CDataUnit	*pdu;
	int selectedid ;

	pdu = API.GetDataUnit(CURRENT_IMAGE);
	if( pdu==NULL )	return;

	if(m_list_contol.GetItemCount()==0)	return;

	selectedid = get_selected_list_item();
	if( 0<=selectedid && selectedid<pdu->GetMarkNumber() )
	{
		CMark mark;
		pdu->GetMark(selectedid, &mark);

		CConfirmDlg cfm;

		if(mark.type == MARK_TYPE_POINT){
			cfm.SetTitle("�}�[�J�[�ҏW(�_)");
			cfm.SetWidth(50,50);
			cfm.RegistVar("x", &mark.figure.point.x);
			cfm.RegistVar("y", &mark.figure.point.y);
		}else{
			cfm.SetTitle("�}�[�J�[�ҏW(��)");
			cfm.SetWidth(50,50);
			cfm.RegistVar("x1", &mark.figure.line.x1);
			cfm.RegistVar("y1", &mark.figure.line.y1);
			cfm.RegistVar("x2", &mark.figure.line.x2);
			cfm.RegistVar("y2", &mark.figure.line.y2);
		}

		for(int i=0 ; i<MARK_ATTRIB_NUM ; i++)
		{
			char caption[32];
			sprintf(caption, "attrib%d",i);
			cfm.RegistVar(caption, &mark.attrib[i]);
		}

		if(cfm.DoModal()==IDOK)
		{
			pdu->SetMark(selectedid, mark);

			API.DrawImage();
			API.DrawCursor(mark.figure.point.x, mark.figure.point.y, 2, RGB(255,0,0));
			select_list_item(selectedid);
		}
	}
}


/********************************************************************
�@  �\  ��  �� : �}�E�X�E�{�^��������
��    ��    �� : OnNMRClickListMark
��          �� : 
��    ��    �l : 
�@          �\ : �ҏW�̂��߂̃_�C�A���O���o���B�ҏW��ɍĕ\������
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnNMRClickListMark(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	OnBnClickedButtonMarkEdit();
	*pResult = 0;
}


/********************************************************************
�@  �\  ��  �� : �E�B���h�E�̃T�C�Y�ύX
��    ��    �� : OnSize
��          �� : 
��    ��    �l : 
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
void CMarkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	show_dlg_item();
}



void CMarkDlg::OnCheckMarkType0()
{
	if (API.GetCheck(m_hWnd, IDC_CHECK_MARK_TYPE0)) {
		API.SetMarkEditMode(2);
	}
}

void CMarkDlg::OnCheckMarkType1()
{
	if (API.GetCheck(m_hWnd, IDC_CHECK_MARK_TYPE1)) {
		API.SetMarkEditMode(3);
	}
}