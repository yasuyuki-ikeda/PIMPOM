/*******************************************************************
                   License Agreement
    Ploaform for IMage Procession Obserber and Modifier

Copyright(c) 2020, Yasuyuki Ikeda All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met :

*Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and / or other materials provided with the distribution.
* The name of the copyright holders may not be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED.IN NO EVENT SHALL CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************/
//�O���v���O��������PIMPOM�Ƀf�[�^���o�͂��邽�߂̊֐��Q
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include <windows.h>

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************
#ifndef _PIMPOM_PLOT
#define _PIMPOM_PLOT

#define  PINPOM_WND_KEY		" - PIMPOM"
#define	 SHEAREMEM_NAME		"PIMPOM_SHAREDMEM"
#define  SHEAREMEM_EXPAND_MESSAGE_SIZE		50000
#define  PIMPOM_WND_CNT_MAX		10

// *************************************
//         �O    ��    ��    ��         
// *************************************

// *************************************
//         �\  ��  ��   ��   �`         
// *************************************
#pragma	warning(push)
#pragma	warning(disable:4200)
#pragma	pack(push, 8)

//�摜�f�[�^���󂯓n�����邽�߂̋��L�������̃w�b�_
typedef struct {
	int		format;//�摜�f�[�^�̃^�C�v
				   //	0:BYTE  
				   //	1:short
				   //	2:WORD
				   //	3:long
				   //	4:DWORD
	int		image_num;//�\����̉摜�������ԍ�
	int		width;//��
	int		height;//����
	int		page;//���s
	int		channel;//�`���l����
	int		size;//���L�������̃T�C�Y
	BYTE	pData[];//�摜�f�[�^
}SHEAREMEM_PLOT_IMAGE;

#pragma	pack(pop)
#pragma	warning(pop)

//�}�`�`��f�[�^���󂯓n�����邽�߂̋��L�������̃w�b�_
typedef struct {
	int		type;//�`��^�C�v�@0:�w�i�ƂȂ�摜�̃R�s�[  1:�摜�`��  2:�_�`��  3:�����`�� 4:��`�`��
	int		image_num;////�\����̉摜�������ԍ�
	int		x0;
	int		y0;
	int		x1;
	int		y1;
	int		size;
	BYTE	red;//�`��F(�ԁj
	BYTE	green;//�`��F(�΁j
	BYTE	blue;//�`��F(�j
	int		copy_src;//�w�i�ƂȂ�摜�������ԍ�
	char	message[5000];//���b�Z�[�W
}SHEAREMEM_EXT_PROC;

//

// *************************************
//         ��    �I    ��    ��         
// *************************************
static int pimpom_target_id = 0;

// *************************************
//         �O    ��    ��    ��         
// *************************************



#ifdef __cplusplus
extern "C" {
#endif


	//�^�C�g����key���܂ރE�B���h�E�n���h������������
	static HWND pimpom_plot_find_window_from_key(HWND hWndParent, TCHAR key[])
	{
		TCHAR szBuff[512];

		SendMessage(hWndParent, WM_GETTEXT, 255, (LPARAM)szBuff);
		if (_tcsstr(szBuff, key)) {
			return hWndParent;
		}
		else {
			HWND hWndChild = ::FindWindowEx(hWndParent, NULL, _T("#32770"), NULL);
			if (hWndChild == NULL)	return NULL;

			do {
				HWND wnd = pimpom_plot_find_window_from_key(hWndChild, key);
				if (wnd)	return wnd;

			} while ((hWndChild = ::FindWindowEx(hWndParent, hWndChild, _T("#32770"), NULL)) != NULL);
		}
		return NULL;
	}


	static void makeWindowKey(int id, TCHAR key[])
	{
		if (id == 0) {
			_stprintf(key, _T(" - PIMPOM  "));
		}
		else if (id>0) {
			_stprintf(key, _T(" - PIMPOM %d"), id);
		}
		else {
			_stprintf(key, _T(" - PIMPOM"));
		}
	}

	//PIMPOM�Ƀ��b�Z�[�W���M����
	static void pimpom_plot_send_data(UINT Msg, UINT wParam, int id)
	{
		HWND hWnd;
		TCHAR key[256];

		makeWindowKey(id, key);

		//hWnd = pimpom_plot_find_window_from_key(::GetDesktopWindow(), PINPOM_WND_KEY);
		hWnd = pimpom_plot_find_window_from_key(::GetDesktopWindow(), key);
		if (hWnd == NULL)	return;

		SendMessage(hWnd, Msg, wParam, (LPARAM)NULL);
	}

	//���L���������擾����
	static void *pimpom_plot_get_shere_mem(int size, HANDLE *hShare, int doWrite)
	{
		if (doWrite)
		{//�������ݗp
#ifdef _M_AMD64
			*hShare = CreateFileMapping((HANDLE)0xffffffffffffffff, NULL, PAGE_READWRITE, 0, size, _T(SHEAREMEM_NAME));
#else
			*hShare = CreateFileMapping((HANDLE)0xffffffff, NULL, PAGE_READWRITE, 0, size, _T(SHEAREMEM_NAME));
#endif
			if (!*hShare)	return NULL;

			return ::MapViewOfFile(*hShare, FILE_MAP_WRITE, 0, 0, size);
		}
		else
		{//�ǂݏo���p
			*hShare = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, FILE_MAP_READ, 0, size, _T(SHEAREMEM_NAME));
			if (!*hShare)	return NULL;

			return ::MapViewOfFile(*hShare, FILE_MAP_READ, 0, 0, size);
		}
	}

	//���L���������������
	static void pimpom_plot_free_shere_mem(void *pMem, HANDLE hShare)
	{
		::UnmapViewOfFile(pMem);
		::CloseHandle(hShare);
	}

	//�摜�f�[�^�̃s�N�Z���T�C�Y
	static int get_pix_size(int format)
	{
		int pix_size = sizeof(BYTE);
		switch (format)
		{
		case 0://BYTE�^������
			pix_size = sizeof(BYTE);
			break;

		case 1://SHORT�^������
			pix_size = sizeof(short);
			break;

		case 2://WORD�^������
			pix_size = sizeof(WORD);
			break;

		case 3://LONG�^������
			pix_size = sizeof(long);
			break;

		case 4://DWORD�^������
			pix_size = sizeof(DWORD);
			break;

		case 5://float�^������
			pix_size = sizeof(float);
			break;

		case 10://3D�^������
			pix_size = sizeof(BYTE);
			break;

		case 11://F3D�^������
			pix_size = sizeof(float);
			break;

		case 20://RGB�^������
			pix_size = sizeof(BYTE);
			break;

		case 21://�}�X�N������
			pix_size = sizeof(BYTE);
			break;

		default:
			return 0;
		}

		return pix_size;
	}



	//PIMPOM�ɉ摜�f�[�^���o�͂���
	static void pimpom_plot_image(int format, int num, int width, int height, int page, int channel, void* pData)
	{
		HANDLE	hShare;
		SHEAREMEM_PLOT_IMAGE	*pShereMem;
		int		pix_size, sheremem_size;

		pix_size = get_pix_size(format);//�摜�f�[�^�̃s�N�Z���T�C�Y
		sheremem_size = sizeof(SHEAREMEM_PLOT_IMAGE) + width*height*pix_size*page*channel;//�摜�f�[�^�̃T�C�Y
		pShereMem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(sheremem_size, &hShare, 1);//���L�������擾
		if (!pShereMem)	return;

		//���L�������Ƀv���b�g����摜�f�[�^�̏�������
		pShereMem->format = format;
		pShereMem->image_num = num;
		pShereMem->width = width;
		pShereMem->height = height;
		pShereMem->page = page;
		pShereMem->channel = channel;
		pShereMem->size = sheremem_size;
		memcpy(pShereMem->pData, pData, width*height*pix_size*page*channel);

		pimpom_plot_send_data(WM_USER + 100, 0, pimpom_target_id);//�v���b�g�̃��b�Z�[�W�𑗐M
		pimpom_plot_free_shere_mem(pShereMem, hShare);//���L�����������
	}


	//PIMPOM����摜�f�[�^���o�͂���
	static void pimpom_get_image(int format, int num, int width, int height, int page, int channel, void* pData)
	{
		HANDLE	hShare;
		SHEAREMEM_PLOT_IMAGE	*pShereMem;
		int		pix_size, sheremem_size;

		pix_size = get_pix_size(format);//�摜�f�[�^�̃s�N�Z���T�C�Y
		sheremem_size = sizeof(SHEAREMEM_PLOT_IMAGE) + width*height*pix_size*page*channel;//�摜�f�[�^�̃T�C�Y
		pShereMem = (SHEAREMEM_PLOT_IMAGE*)pimpom_plot_get_shere_mem(sheremem_size, &hShare, 1);//���L�������擾
		if (!pShereMem)	return;

		//���L�������Ƀv���b�g����摜�f�[�^�̏�������
		pShereMem->format = format;
		pShereMem->image_num = num;
		pShereMem->width = width;
		pShereMem->height = height;
		pShereMem->page = page;
		pShereMem->channel = channel;
		pShereMem->size = sheremem_size;


		pimpom_plot_send_data(WM_USER + 102, 0, pimpom_target_id);//���b�Z�[�W�𑗐M
		memcpy(pData, pShereMem->pData, width*height*pix_size*page*channel);


		pimpom_plot_free_shere_mem(pShereMem, hShare);//���L�����������
	}




	//PIMPOM�̃E�B���h�EID��񋓂���
	static int getTargetIDs(int IDs[])
	{
		int wndCnt = 0;

		for (int id = 0; id < PIMPOM_WND_CNT_MAX; id++)
		{
			HWND hWnd;
			TCHAR key[256];

			makeWindowKey(id, key);
			hWnd = pimpom_plot_find_window_from_key(::GetDesktopWindow(), key);
			if (hWnd) {
				IDs[wndCnt] = id;
				wndCnt++;
			}
		}

		return wndCnt;
	}



	//�f�[�^�𑗂�PIMPOM��ID���w�肷��
	static void setTargetID(int target)
	{
		pimpom_target_id = target;
	}


	/********************************************************************
	�@  �\  ��  �� : PIMPOM��BYTE�摜�f�[�^���o�͂���
	��    ��    �� : PlotByteImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	BYTE* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotByteImage(int num, int width, int height, BYTE* pData)
	{
		pimpom_plot_image(0, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM��short�摜�f�[�^���o�͂���
	��    ��    �� : PlotShortImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	short* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotShortImage(int num, int width, int height, short* pData)
	{
		pimpom_plot_image(1, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM��unsigned short�摜�f�[�^���o�͂���
	��    ��    �� : PlotWordImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	unsigned short* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotWordImage(int num, int width, int height, WORD* pData)
	{
		pimpom_plot_image(2, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM��long�摜�f�[�^���o�͂���
	��    ��    �� : PlotLongImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	long* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotLongImage(int num, int width, int height, long* pData)
	{
		pimpom_plot_image(3, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM��unsigned long�摜�f�[�^���o�͂���
	��    ��    �� : PlotDwordImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	unsigned long* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotDwordImage(int num, int width, int height, DWORD* pData)
	{
		pimpom_plot_image(4, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM��float�摜�f�[�^���o�͂���
	��    ��    �� : PlotFloatImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	float* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotFloatImage(int num, int width, int height, float* pData)
	{
		pimpom_plot_image(5, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM��3D�摜�f�[�^���o�͂���
	��    ��    �� : Plot3DImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	int page   (in)���s
	BYTE* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void Plot3DImage(int num, int width, int height, int page, BYTE* pData)
	{
		pimpom_plot_image(10, num, width, height, page, 1, pData);
	}


	/********************************************************************
	�@  �\  ��  �� : PIMPOM��Float3D�摜�f�[�^���o�͂���
	��    ��    �� : PlotF3DImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	int page   (in)���s
	BYTE* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotF3DImage(int num, int width, int height, int page, float* pData)
	{
		pimpom_plot_image(11, num, width, height, page, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM��RGB�摜�f�[�^���o�͂���
	��    ��    �� : PlotRGBImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	BYTE* pData (in)�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotRGBImage(int num, int width, int height, BYTE* pData)
	{
		pimpom_plot_image(20, num, width, height, 1, 3, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����BYTE�摜�f�[�^����͂���
	��    ��    �� : GetByteImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	BYTE* pData (out)�摜�f�[�^�̐擪�|�C���^ width*height�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetByteImage(int num, int width, int height, BYTE* pData)
	{
		pimpom_get_image(0, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����short�摜�f�[�^����͂���
	��    ��    �� : GethortImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	short* pData (out)�摜�f�[�^�̐擪�|�C���^ width*height�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetShortImage(int num, int width, int height, short* pData)
	{
		pimpom_get_image(1, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����unsigned short�摜�f�[�^����͂���
	��    ��    �� : GetWordImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	unsigned short* pData (out)�摜�f�[�^�̐擪�|�C���^ width*height�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetWordImage(int num, int width, int height, WORD* pData)
	{
		pimpom_get_image(2, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����long�摜�f�[�^����͂���
	��    ��    �� : GetLongImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	long* pData (out)�摜�f�[�^�̐擪�|�C���^ width*height�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetLongImage(int num, int width, int height, long* pData)
	{
		pimpom_get_image(3, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����unsigned long�摜�f�[�^����͂���
	��    ��    �� : PlotDwordImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	unsigned long* pData (out)�摜�f�[�^�̐擪�|�C���^ width*height�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetDwordImage(int num, int width, int height, DWORD* pData)
	{
		pimpom_get_image(4, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����float�摜�f�[�^����͂���
	��    ��    �� : GetFloatImage
	��          �� : int num�@	(in)�摜�������ԍ�
	 int width	(in)��
	int height (in)����
	float* pData (out)�摜�f�[�^�̐擪�|�C���^ width*height�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetFloatImage(int num, int width, int height, float* pData)
	{
		pimpom_get_image(5, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����3D�摜�f�[�^����͂���
	��    ��    �� : Get3DImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	int page   (in)���s
	BYTE* pData (out)�摜�f�[�^�̐擪�|�C���^ width*height*page�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void Get3DImage(int num, int width, int height, int page, BYTE* pData)
	{
		pimpom_get_image(10, num, width, height, page, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����Float3D�摜�f�[�^����͂���
	��    ��    �� : GetF3DImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	int page   (in)���s
	float* pData (out)�摜�f�[�^�̐擪�|�C���^ width*height*page�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetF3DImage(int num, int width, int height, int page, float* pData)
	{
		pimpom_get_image(11, num, width, height, page, 1, pData);
	}


	/********************************************************************
	�@  �\  ��  �� : PIMPOM����RGB�摜�f�[�^����͂���
	��    ��    �� : GetRGBImage
	��          �� : int num�@	(in)�摜�������ԍ�
	int width	(in)��
	int height (in)����
	BYTE* pData (out)�摜�f�[�^�̐擪�|�C���^  width*height*3�̃������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetRGBImage(int num, int width, int height, BYTE* pData)
	{
		pimpom_get_image(20, num, width, height, 1, 3, pData);
	}


	//PIMPOM�ɐ}�`�f�[�^���o�͂���
	static void pimpom_draw_figure(int type, int num, int x0, int y0, int x1, int y1, int size, BYTE r, BYTE g, BYTE b, int copy_src)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = type;
		pShereMem->image_num = num;
		pShereMem->x0 = x0;
		pShereMem->y0 = y0;
		pShereMem->x1 = x1;
		pShereMem->y1 = y1;
		pShereMem->size = size;
		pShereMem->red = r;
		pShereMem->green = g;
		pShereMem->blue = b;
		pShereMem->copy_src = copy_src;

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}

	//PIMPOM�Ƀe�L�X�g�f�[�^���o�͂���
	static void pimpom_draw_text(int type, int num, int x, int y, char mes[], BYTE r, BYTE g, BYTE b, int copy_src)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = type;
		pShereMem->image_num = num;
		pShereMem->x0 = x;
		pShereMem->y0 = y;
		pShereMem->x1 = 0;
		pShereMem->y1 = 0;
		pShereMem->size = 0;
		pShereMem->red = r;
		pShereMem->green = g;
		pShereMem->blue = b;
		pShereMem->copy_src = copy_src;
		strcpy(pShereMem->message, mes);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}

	/********************************************************************
	�@  �\  ��  �� : �}�`�`�揀��
	��    ��    �� : PrepeareDrawing
	��          �� : int num�@			(in)�`�悷��摜�������ԍ�
	int copy_src_num	(in)�w�i�摜�̉摜�������ԍ�
	��    ��    �l :
	�@          �\ : copy_src_num�̃������ɓ����Ă���摜��w�i�Ƃ���num�ɕ`��p��RGB�摜��V�K��������
	�}�`�`��O�ɂ��̊֐����Ăяo���Ă���
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PrepeareDrawing(int num, int copy_src_num)
	{
		pimpom_draw_figure(0, num, 0, 0, 0, 0, 0, 0, 0, 0, copy_src_num);
	}

	/********************************************************************
	�@  �\  ��  �� : �摜�`��
	��    ��    �� : DrawImage
	��          �� : int num�@			(in)�`�悷��摜�������ԍ�
	bool doAdjust		(in)��ʃT�C�Y�ɍ��킹�邩�ǂ���
	char *format, ...  (in)�摜�f�[�^��
	��    ��    �l :
	�@          �\ : �摜��������ɐ}�`�`�悵�����ƁA�\���̂��߂ɂ��̊֐����Ăяo��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void DrawImage2(int num, bool doAdjust, char *format, ...)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;
		va_list list;
		char	buf[5000] = { 0 };

		//�f�[�^�����擾
		va_start(list, format);
		vsprintf(buf, format, list);
		va_end(list);

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 1;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%d,%s\n", doAdjust, buf);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}


	static void DrawImage(int num)
	{
		DrawImage2(num, false, "");
	}

	/********************************************************************
	�@  �\  ��  �� : �摜��������ɓ_��`�悷��
	��    ��    �� : DrawPointOnImage
	��          �� : int	num�@		(in)�`�悷��摜�������ԍ�
	int	x,y			(in)�_�̍��W
	BYTE	r,g,b		(in)�_�̕`��F
	��    ��    �l :
	�@          �\ : �摜��������ɓ_��`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void DrawPointOnImage(int num, int x, int y, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(2, num, x, y, 0, 0, 1, r, g, b, -1);
	}

	/********************************************************************
	�@  �\  ��  �� : �摜��������ɒ�����`�悷��
	��    ��    �� : DrawLineOnImage
	��          �� : int	num�@			(in)�`�悷��摜�������ԍ�
	int	x0,y0,x1,y1		(in)�����̎n�_�I�_���W
	BYTE	r,g,b			(in)�_�̕`��F
	��    ��    �l :
	�@          �\ : �摜��������ɓ_��`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void DrawLineOnImage(int num, int x0, int y0, int x1, int y1, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(3, num, x0, y0, x1, y1, 1, r, g, b, -1);
	}

	/********************************************************************
	�@  �\  ��  �� : �摜��������ɋ�`��`�悷��
	��    ��    �� : DrawRectOnImage
	��          �� : int	num�@			(in)�`�悷��摜�������ԍ�
	int	x0,y0,x1,y1		(in)��`�̂S�����W
	BYTE	r,g,b			(in)�_�̕`��F
	��    ��    �l :
	�@          �\ : �摜��������ɓ_��`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void DrawRectOnImage(int num, int x0, int y0, int x1, int y1, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(4, num, x0, y0, x1, y1, 1, r, g, b, -1);
	}

	/********************************************************************
	�@  �\  ��  �� : �摜��������ɉ~�`��`�悷��
	��    ��    �� : DrawCircleOnImage
	��          �� : int	num�@			(in)�`�悷��摜�������ԍ�
	int	cx,cy			(in)���S���W
	int	rad				(in)���a
	bool	fill			(in)����h��Ԃ����ǂ���
	BYTE	r,g,b			(in)�_�̕`��F
	��    ��    �l :
	�@          �\ : �摜��������ɓ_��`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void DrawCircleOnImage(int num, int cx, int cy, int rad, bool fill, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(5, num, cx, cy, rad, fill, 1, r, g, b, -1);
	}


	/********************************************************************
	�@  �\  ��  �� : �摜��������ɏ\���J�[�\����`�悷��
	��    ��    �� : DrawCursorOnImage
	��          �� : int	num�@			(in)�`�悷��摜�������ԍ�
	int	x,y				(in)���W
	BYTE	r,g,b			(in)�_�̕`��F
	��    ��    �l :
	�@          �\ : �摜��������ɓ_��`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void DrawCursorOnImage(int num, int x, int y, BYTE r, BYTE g, BYTE b)
	{
		pimpom_draw_figure(6, num, x, y, 0, 0, 1, r, g, b, -1);
	}


	/********************************************************************
	�@  �\  ��  �� : �摜��������ɋ�`��`�悷��
	��    ��    �� : DrawRectOnImage
	��          �� : int	num�@			(in)�`�悷��摜�������ԍ�
	int	x0,y0,x1,y1		(in)��`�̂S�����W
	BYTE	r,g,b			(in)�_�̕`��F
	��    ��    �l :
	�@          �\ : �摜��������ɓ_��`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void DrawTextOnImage(int num, int x, int y, BYTE r, BYTE g, BYTE b, char *format, ...)
	{
		va_list list;
		char	buf[5000];

		va_start(list, format);
		vsprintf(buf, format, list);
		va_end(list);

		pimpom_draw_text(10, num, x, y, buf, r, g, b, -1);
	}

	/********************************************************************
	�@  �\  ��  �� : ���b�Z�[�W���o��
	��    ��    �� : ShowMessage
	��          �� :
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void ShowMessage(char *format, ...)
	{
		va_list list;
		char	buf[5000];

		va_start(list, format);
		vsprintf(buf, format, list);
		va_end(list);

		pimpom_draw_text(11, 0, 0, 0, buf, 0, 0, 0, -1);
	}


	/********************************************************************
	�@  �\  ��  �� : �����炵�����b�Z�[�W���o��(�ߋ��̂��̂̓N���A)
	��    ��    �� : ShowMessage
	��          �� :
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void ShowNewMessage(char *format, ...)
	{
		va_list list;
		char	buf[5000];

		va_start(list, format);
		vsprintf(buf, format, list);
		va_end(list);

		pimpom_draw_text(12, 0, 0, 0, buf, 0, 0, 0, -1);
	}

	/********************************************************************
	�@  �\  ��  �� : �}�[�J�[�ǉ�
	��    ��    �� : AddMark
	��          �� : int	num�@			(in)�摜�������ԍ�
	float x,y				(in)���W
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void AddMark(int num, float x, float y, double atrrib0 = 0, double atrrib1 = 0, double atrrib2 = 0)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 13;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%f,%f,%lf,%lf,%lf\n", x, y, atrrib0, atrrib1, atrrib2);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}


	/********************************************************************
	�@  �\  ��  �� : �O���t�ɒl�ǉ�
	��    ��    �� : AddDataToChart
	��          �� : int	num�@			(in)�O���t�ԍ�
	float val				(in)�l
	��    ��    �l :
	�@          �\ : �܂���O���t�ɒl��ǉ����ĕ`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void AddDataToChart(int num, float val)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 20;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%f\n", val);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}

	/********************************************************************
	�@  �\  ��  �� : �O���t�ɒl�ǉ�
	��    ��    �� : AddDataToChart2
	��          �� : int	num�@			(in)�O���t�ԍ�
	float valx,valy		(in)�l
	��    ��    �l :
	�@          �\ : 2�����U�z�}�ɒl��ǉ����ĕ`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void AddDataToChart2(int num, float valx, float valy)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 21;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%f,%f\n", valx, valy);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}

	/********************************************************************
	�@  �\  ��  �� : �O���t���N���A
	��    ��    �� : ClearChart
	��          �� : int	num�@			(in)�O���t�ԍ�
	��    ��    �l :
	�@          �\ : 2�����U�z�}�ɒl��ǉ����ĕ`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void ClearChart(int num)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC), &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 22;
		pShereMem->image_num = num;

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}


	/********************************************************************
	�@  �\  ��  �� : ���[�U�_�C�A���O�̃o�b�`���s�֐����Ă�
	��    ��    �� : BatchExecute
	��          �� : int	num�@				(in)���[�U�_�C�A���O�̔ԍ�
	char	param[5][1024]		(in)�o�b�`���s�̓��̓p�����^
	char	result[15][1024]	(out)�o�b�`���s�̏o�̓p�����^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void BatchExecute(
		int	num,
		char	imae_file_path[2048],//���͉摜�t�@�C��
		char	param[5][1024],//���̓p�����^
		char	result[15][1024],//�o�̓p�����^
		int	*state//���s����
	)
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;
		char *ptr;

		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);
		if (!pShereMem)	return;

		pShereMem->type = 30;
		pShereMem->image_num = num;

		//�p�����^��������
		ptr = (char*)pShereMem + sizeof(SHEAREMEM_EXT_PROC);
		sprintf(ptr, "%s", imae_file_path);
		ptr += 2048;

		for (int i = 0; i<5; i++) {
			sprintf(ptr, "%s", param[i]);
			ptr += 1024;
		}

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);//�]��

																  //���ʓǂݏo��
		ptr = (char*)pShereMem + sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE / 2;
		for (int i = 0; i<15; i++) {
			strcpy(result[i], ptr);
			ptr += 1024;
		}

		*state = (int)(*ptr);



		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}


	/********************************************************************
	�@  �\  ��  �� : �R�}���h���s
	��    ��    �� : ExecuteCommand
	��          �� : (in)command
	(out)result
	(out)err
	��    ��    �l : ���s���� (����: 1   ���s:0)
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static int ExecuteCommand(char command[], char result[], char err[])
	{
		HANDLE	hShare;
		SHEAREMEM_EXT_PROC *pShereMem;
		char *ptr;
		int ret;



		pShereMem = (SHEAREMEM_EXT_PROC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE, &hShare, 1);
		if (!pShereMem)	return 0;

		pShereMem->type = 100;

		//�R�}���h��������
		ptr = (char*)pShereMem + sizeof(SHEAREMEM_EXT_PROC);
		sprintf(ptr, command);


		//�]��
		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);

		//���ʓǂݏo��
		ptr = (char*)pShereMem + sizeof(SHEAREMEM_EXT_PROC) + SHEAREMEM_EXPAND_MESSAGE_SIZE / 2;
		ret = ptr[0];				//�擪Byte : ���s���� (����: 1   ���s:0)
		strcpy(result, &ptr[5000]);	//�擪����5000�`10000Byte : ���s����
		strcpy(err, &ptr[10000]);//�擪����10000�`15000Byte : �G���[���b�Z�[�W

		pimpom_plot_free_shere_mem(pShereMem, hShare);

		return ret;
	}


	/********************************************************************
	�@  �\  ��  �� : PIMPOM����摜�f�[�^�̌^���ƃT�C�Y���o�͂���
	��    ��    �� : GetDataTypeSize
	��          �� : 
					int		*ptype		(out)�摜�������̌^
					int		*pwidth		(out)�摜��
					int		*pheight	(out)�摜����
					int		*ppages		(out)�摜�̉��s
	��    ��    �l : ���s���� (����: 1   ���s:0)
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	//PIMPOM����摜�f�[�^�̌^���ƃT�C�Y���o�͂���
	static void GetDataTypeSize(int num, int *ptype, int *pwidth, int *pheight, int *ppages)
	{
		int  tmp[4] = { -1 };

		for (int i = 0; i < 4; i++)
		{
			char command[1024] = { 0 };
			char result[1024] = { 0 };
			char err[1024] = { 0 };

			sprintf(command, "GetDataTypeSize(%d,%d)", num,i);
			if (ExecuteCommand(command, result, err)) {
				tmp[i] = atoi(result);
			}
		}

		*ptype = tmp[0];
		*pwidth = tmp[1];
		*pheight = tmp[2];
		*ppages = tmp[3];
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM����}�X�N�摜�f�[�^����͂���
	��    ��    �� : GetMaskImage
	��          �� : int num�@	(in)�摜�������ԍ�
	BYTE* pData (out)�摜�f�[�^�̐擪�|�C���^ �������̈悪�m�ۂ���Ă��邱��
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void GetMaskImage(int num, BYTE* pData)
	{
		int width = 0, height = 0, type = 0, page = 0;
		GetDataTypeSize(num, &type, &width, &height, &page);//�摜�T�C�Y�擾

		pimpom_get_image(21, num, width, height, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM�Ƀ}�X�N�摜�f�[�^���o�͂���
	��    ��    �� : SetMaskImage
	��          �� : int num�@	(in)�摜�������ԍ�
	BYTE* pData (in)�}�X�N�摜�f�[�^�̐擪�|�C���^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void SetMaskImage(int num, BYTE* pData)
	{
		int width=0, height =0, type = 0, page = 0;
		GetDataTypeSize(num, &type, &width, &height, &page);//�摜�T�C�Y�擾

		if (width > 0 && height > 0) {
			pimpom_plot_image(21, num, width, height, 1, 1, pData);
		}
	}

	/********************************************************************
	�@  �\  ��  �� : ��Ɨ̈���擾
	��    ��    �� : GetWorkArea
	��          �� : int num�@	(in)�摜�������ԍ�
	                 int fig_n  (in)�}�`�ԍ�
	��    ��    �l :��Ɨ̈�(��`�̂�)
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static RECT GetWorkArea(int num, int fig_n)
	{
		int  tmp[4] = { 0 };
		RECT rct = {0};

		for (int i = 0; i < 4; i++)
		{
			char command[1024] = { 0 };
			char result[1024] = { 0 };
			char err[1024] = { 0 };

			sprintf(command, "GetWorkAreaVtx(%d,%d,%d)", num, fig_n, i);
			if (!ExecuteCommand(command, result, err))		return rct;

			tmp[i] = atoi(result);
		}

		rct.left = tmp[0];
		rct.top = tmp[1];
		rct.right = tmp[2];
		rct.bottom = tmp[3];

		return rct;
	}

	/********************************************************************
	�@  �\  ��  �� : ��Ɨ̈��ݒ�
	��    ��    �� : SetWorkArea
	��          �� : int num�@	(in)�摜�������ԍ�
					int fig_n  (in)�}�`�ԍ�
					RECT area  (in)��`
					bool *ornot			(out)true:OR�̈�@false:NOT�̈�
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void SetWorkArea(int num, int fig_n, int left, int top, int right, int bottom, bool ornot)
	{
		char command[1024] = { 0 };
		char result[1024] = { 0 };
		char err[1024] = { 0 };

		sprintf(command, "SetWorkArea(%d,%d,%d,%d,%d,%d,%d)", num, fig_n, left, top, right, bottom, ornot);
		if (!ExecuteCommand(command, result, err))		return ;
	}


#ifdef __cplusplus
}
#endif

#endif
