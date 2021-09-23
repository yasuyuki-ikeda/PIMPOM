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

#define  PIMPOM_PLOT_FORMAT_BYTE_IMAGE		(0)
#define  PIMPOM_PLOT_FORMAT_SHORT_IMAGE		(1)
#define  PIMPOM_PLOT_FORMAT_WORD_IMAGE		(2)
#define  PIMPOM_PLOT_FORMAT_LONG_IMAGE		(3)
#define  PIMPOM_PLOT_FORMAT_DWORD_IMAGE		(4)
#define  PIMPOM_PLOT_FORMAT_FLOAT_IMAGE		(5)
#define  PIMPOM_PLOT_FORMAT_3D_IMAGE		(10)
#define  PIMPOM_PLOT_FORMAT_F3D_IMAGE		(11)
#define  PIMPOM_PLOT_FORMAT_RGB_IMAGE		(20)
#define  PIMPOM_PLOT_FORMAT_MASK_IMAGE		(21)
#define  PIMPOM_PLOT_FORMAT_FLOAT_1D_CHART		(30)
#define  PIMPOM_PLOT_FORMAT_DOUBLE_1D_CHART		(31)
#define  PIMPOM_PLOT_FORMAT_INT_1D_CHART		(32)
#define  PIMPOM_PLOT_FORMAT_BYTE_1D_CHART		(33)
#define  PIMPOM_PLOT_FORMAT_FLOAT_2D_CHART		(40)
#define  PIMPOM_PLOT_FORMAT_DOUBLE_2D_CHART		(41)
#define  PIMPOM_PLOT_FORMAT_INT_2D_CHART		(42)
#define  PIMPOM_PLOT_FORMAT_BYTE_2D_CHART		(43)

#define  PIMPOM_PLOT_COMMAND_PREPAIR_DRAWING	(0)
#define  PIMPOM_PLOT_COMMAND_DRAW_IMAGE	(1)
#define  PIMPOM_PLOT_COMMAND_DRAW_POINT_ON_IMAGE	(2)
#define  PIMPOM_PLOT_COMMAND_DRAW_LINE_ON_IMAGE	(3)
#define  PIMPOM_PLOT_COMMAND_DRAW_RECT_ON_IMAGE	(4)
#define  PIMPOM_PLOT_COMMAND_DRAW_CIRCLE_ON_IMAGE	(5)
#define  PIMPOM_PLOT_COMMAND_DRAW_CURSOR_ON_IMAGE	(6)
#define  PIMPOM_PLOT_COMMAND_DRAW_TEXT_ON_IMAGE	(10)
#define  PIMPOM_PLOT_COMMAND_SHOW_MESSAGE	(11)
#define  PIMPOM_PLOT_COMMAND_SHOW_NEW_MESSAGE	(12)
#define  PIMPOM_PLOT_COMMAND_ADD_MARK	(13)
#define  PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT	(20)
#define  PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT2	(21)
#define  PIMPOM_PLOT_COMMAND_CLEAR_CAHRT	(22)
#define  PIMPOM_PLOT_COMMAND_BATCH_EXECUTE	(30)
#define  PIMPOM_PLOT_COMMAND_EXECUTE	(100)

#define  PIMPOM_PLOT_ASYNC_PARAM_LEN  (1000)
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
				   //	0:BYTE 1:short 2:WORD 3:long 4:DWORD 5:FLOAT 10:3D 11:FLOAT3D: 20 RGB: 21 MASK
				   //	30�`33:�܂���O���t�\���p�f�[�^  40�`43:�U�z�}�\���p�f�[�^
	int	image_num;//�\����̉摜�������ԍ�
	int		width;//��
	int		height;//����
	int		page;//���s
	int		channel;//�`���l����
	int		size;//���L�������̃T�C�Y
	BYTE	pData[];//�摜�f�[�^
}SHEAREMEM_PLOT_IMAGE;

#pragma	pack(pop)
#pragma	warning(pop)

typedef struct {
	int		asyncFlg;//1 : PIMPOM����O���v���O�����ւ̔񓯊��o��  2:�O���v���O��������PIMPOM�ւ̔񓯊�����  0:���o�͕s��
	float	params[PIMPOM_PLOT_ASYNC_PARAM_LEN];//�񓯊��ʐM�p�����[�^
	SHEAREMEM_PLOT_IMAGE  memory;
}SHEAREMEM_PLOT_IMAGE_ASYNC;


//�摜�]���𔺂�Ȃ����b�Z�[�W���󂯓n�����邽�߂̋��L�������̃w�b�_
typedef struct {
	int		type;//���b�Z�[�W�^�C�v�@
                  //0:�摜��ւ̕`��̔w�i�ƂȂ�摜�̃R�s�[  1:�摜�`��  2:�摜��ɓ_�`��  3:�摜��ɒ����`�� 4:�摜��ɋ�`�`�� 5:�摜��ɉ~�`�� 6:�摜��ɏ\���J�[�\���`�� 10:�摜��Ƀe�L�X�g�`�� 
                  //11:���b�Z�[�W�G���A�ɒǉ��\��   12:���b�Z�[�W�G���A�ɐV�K�\��  13:�}�[�N�ǉ�  20:�܂���O���t�v�f�ǉ�  21:�U�z�}�v�f�ǉ�  22:�O���t�N���A 
                  //30:�o�b�`���s  100:�X�N���v�g�R�}���h���s
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
static int pimpom_async_ids[2] = {2,1};//�񓯊����͂�ID [0]���̃v���Z�X��ID  [1]����̃v���Z�X��ID

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
		else if (id > 0) {
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
		case PIMPOM_PLOT_FORMAT_BYTE_IMAGE://BYTE�^������
			pix_size = sizeof(BYTE);
			break;

		case PIMPOM_PLOT_FORMAT_SHORT_IMAGE://SHORT�^������
			pix_size = sizeof(short);
			break;

		case PIMPOM_PLOT_FORMAT_WORD_IMAGE://WORD�^������
			pix_size = sizeof(WORD);
			break;

		case PIMPOM_PLOT_FORMAT_LONG_IMAGE://LONG�^������
			pix_size = sizeof(long);
			break;

		case PIMPOM_PLOT_FORMAT_DWORD_IMAGE://DWORD�^������
			pix_size = sizeof(DWORD);
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_IMAGE://float�^������
			pix_size = sizeof(float);
			break;

		case PIMPOM_PLOT_FORMAT_3D_IMAGE://3D�^������
			pix_size = sizeof(BYTE);
			break;

		case PIMPOM_PLOT_FORMAT_F3D_IMAGE://F3D�^������
			pix_size = sizeof(float);
			break;

		case PIMPOM_PLOT_FORMAT_RGB_IMAGE://RGB�^������
			pix_size = sizeof(BYTE);
			break;

		case PIMPOM_PLOT_FORMAT_MASK_IMAGE://�}�X�N������
			pix_size = sizeof(BYTE);
			break;

		case PIMPOM_PLOT_FORMAT_FLOAT_1D_CHART://1D�O���t(float)
		case PIMPOM_PLOT_FORMAT_FLOAT_2D_CHART://2D�O���t(float)
			pix_size = sizeof(float);
			break;

		case PIMPOM_PLOT_FORMAT_DOUBLE_1D_CHART://1D�O���t(double)
		case PIMPOM_PLOT_FORMAT_DOUBLE_2D_CHART://2D�O���t(double)
			pix_size = sizeof(double);
			break;

		case PIMPOM_PLOT_FORMAT_INT_1D_CHART://1D�O���t(int)
		case PIMPOM_PLOT_FORMAT_INT_2D_CHART://2D�O���t(int)
			pix_size = sizeof(int);
			break;

		case PIMPOM_PLOT_FORMAT_BYTE_1D_CHART://1D�O���t(BYTE)
		case PIMPOM_PLOT_FORMAT_BYTE_2D_CHART://2D�O���t(BYTE)
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

		pimpom_plot_send_data(WM_USER + 100, 0, pimpom_target_id);//���L����������PIMPOM�ɓǂݏo���悤�Ƀ��b�Z�[�W�𑗐M
		pimpom_plot_free_shere_mem(pShereMem, hShare);//���L�����������
	}


	//PIMPOM����摜�f�[�^����͂���
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


		pimpom_plot_send_data(WM_USER + 102, 0, pimpom_target_id);//PIMPOM���狤�L�������Ƀf�[�^�i�[����悤�Ƀ��b�Z�[�W�𑗐M
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
	�@  �\  ��  �� : PIMPOM�Ƃ̋��L����������摜�f�[�^��񓯊��ǂݍ��݂���
	��    ��    �� : GetImageAsync
	��          �� :
	int num�@	(in)�摜�������ԍ�
	int format (out)�f�[�^�^��
	int *height (out)����
	int *page   (out)���s
	int *channel   (out)�`���l��
	void** ppData (out)�摜�f�[�^�̃|�C���^�ւ̃|�C���^�i�����ɐV�������������m�ۂ����j
	int checkAsyncFlg  (in)�񓯊��Q�ƃt���O�̃`�F�b�N������B���]�̂��̂ł����true��Ԃ��B
					(�����̏ꍇ�̓`�F�b�N�����ɓǂݏo��)
	float* pAsyncParams (out)�p�����^
	��    ��    �l :����������true
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static int pimpom_get_image_async(int format, int *pImageNum, int *pWidth, int *pHeight, int *pPage, int *pChannel, void** ppData, int checkAsyncFlg, float* pAsyncParams)
	{
		SHEAREMEM_PLOT_IMAGE_ASYNC	*pShmem;//���L�������̈�
		SHEAREMEM_PLOT_IMAGE_ASYNC	shmem_header = { 0 };
		HANDLE	hShare;//���L�������̃n���h��
		int pix_size;//�摜�f�[�^�̃s�N�Z���T�C�Y
		int data_size;

		//���L�������̈�̃w�b�_������ǂ�
		pShmem = (SHEAREMEM_PLOT_IMAGE_ASYNC*)pimpom_plot_get_shere_mem(sizeof(SHEAREMEM_PLOT_IMAGE_ASYNC), &hShare, 0);
		if (!pShmem)	return 0;

		memcpy(&shmem_header, pShmem, sizeof(SHEAREMEM_PLOT_IMAGE_ASYNC));
		pimpom_plot_free_shere_mem(pShmem, hShare);//���L���������

		if (pAsyncParams) {
			memcpy(pAsyncParams, shmem_header.params, PIMPOM_PLOT_ASYNC_PARAM_LEN * sizeof(float));
		}

		if (checkAsyncFlg >= 0) 
		{//�񓯊��ǂݍ��݉\���`�F�b�N
			if (shmem_header.asyncFlg != checkAsyncFlg) {
				return 0;//�񓯊��Q�ƃt���O�����]�̂��̂ł͂Ȃ�
			}
			else {
				return 1;//�񓯊��Q�ƃt���O�����]�̂��̂ł���
			}
		}
		else 
		{//�񓯊��ǂݍ��ݎ��s

		 //�摜�������w�b�_�ُ̈�`�F�b�N
			if (shmem_header.memory.format != format || shmem_header.memory.width <= 0 || shmem_header.memory.height <= 0
				|| shmem_header.memory.channel <= 0 || shmem_header.memory.page <= 0 || shmem_header.memory.size <= 0)	return 0;

			//���L�������̈�̑S�̓ǂݒ���
			pShmem = (SHEAREMEM_PLOT_IMAGE_ASYNC*)pimpom_plot_get_shere_mem(shmem_header.memory.size, &hShare, 0);
			if (!pShmem)	return 0;

			pix_size = get_pix_size(shmem_header.memory.format);
			data_size = pix_size*shmem_header.memory.width*shmem_header.memory.height*shmem_header.memory.page*shmem_header.memory.channel;

			*ppData = (void**)malloc(data_size);//�f�[�^�p�̃������̈�m��
			if ((*ppData) == NULL) {
				pimpom_plot_free_shere_mem(pShmem, hShare);//���L���������
				return 0;
			}

			memcpy(*ppData, pShmem->memory.pData, data_size);
			pimpom_plot_free_shere_mem(pShmem, hShare);//���L���������

			*pImageNum = shmem_header.memory.image_num;
			*pWidth = shmem_header.memory.width;
			*pHeight = shmem_header.memory.height;
			*pChannel = shmem_header.memory.channel;
			*pPage = shmem_header.memory.page;
		}

		return 1;
	}

	/********************************************************************
	�@  �\  ��  �� : PIMPOM�Ƃ̋��L�������։摜�f�[�^��񓯊��������݂���
	��    ��    �� : PlotImageAsync
	��          �� :
	int num�@	(in)�摜�������ԍ�
	int format (in)�f�[�^�^��
	int width	(in)��
	int height (in)����
	int page   (in)���s
	int channel   (in)�`���l��
	void* pData (in)�摜�f�[�^�̐擪�|�C���^
	int asyncFlg (in)�񓯊��Q�ƃt���O�̃Z�b�g(�����̏ꍇ�͓����I��)
	float* pAsyncParams (in)�p�����^
	��    ��    �l :
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void pimpom_plot_image_async(int format, int num, int width, int height, int page, int channel, void* pData, int asyncFlg, float* pAsyncParams)
	{
		static HANDLE	hShare = NULL;
		static SHEAREMEM_PLOT_IMAGE_ASYNC	*pShereMem = NULL;
		int		pix_size, sheremem_size;


		if (pShereMem) {
			pimpom_plot_free_shere_mem(pShereMem, hShare);//���L�����������
		}

	
		pix_size = get_pix_size(format);//�摜�f�[�^�̃s�N�Z���T�C�Y
		sheremem_size = sizeof(SHEAREMEM_PLOT_IMAGE_ASYNC) + width*height*pix_size*page*channel;//�摜�f�[�^�̃T�C�Y
		pShereMem = (SHEAREMEM_PLOT_IMAGE_ASYNC*)pimpom_plot_get_shere_mem(sheremem_size, &hShare, 1);//���L�������擾
		if (!pShereMem)	return;


		pShereMem->memory.format = format;
		pShereMem->memory.width = width;
		pShereMem->memory.height = height;
		pShereMem->memory.page = page;
		pShereMem->memory.channel = channel;
		pShereMem->memory.size = sheremem_size;
		memcpy(pShereMem->memory.pData, pData, width*height*pix_size*page*channel);

		pShereMem->asyncFlg = asyncFlg;
		if (pAsyncParams) {
			memcpy(pShereMem->params, pAsyncParams, PIMPOM_PLOT_ASYNC_PARAM_LEN*sizeof(float));
		}
	}


	/********************************************************************
	�@  �\  ��  �� : �񓯊��Q�ƃt���O�̒l�������l0�ɖ߂�
	��    ��    �� : InitAysnc
	��          �� :
	��    ��    �l : 
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void InitAysnc(int thisIsPIMPOM)
	{
		if (thisIsPIMPOM) {
			pimpom_async_ids[0] = 1;
			pimpom_async_ids[1] = 2;
		}
	}

	/********************************************************************
	�@  �\  ��  �� : �񓯊��Q�Ɖ\���`�F�b�N����
	��    ��    �� : EnableAysnc
	��          �� :�@
	float* pAsyncParams (out)�p�����^
	��    ��    �l : ����̃v���Z�X��ID�Ȃ��1��Ԃ�����ȊO��0��Ԃ�
	�@          �\ :
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static int EnableAysnc(float* pAsyncParams)
	{
		return pimpom_get_image_async(0, NULL, NULL, NULL, NULL, NULL, NULL, pimpom_async_ids[1], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_BYTE_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotByteImageAsync(int num, int width, int height, BYTE* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_BYTE_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_SHORT_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotShortImageAsync(int num, int width, int height, short* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_SHORT_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_WORD_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotWordImageAsync(int num, int width, int height, WORD* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_WORD_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_LONG_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotLongImageAsync(int num, int width, int height, long* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_LONG_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_DWORD_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotDwordImageAsync(int num, int width, int height, DWORD* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_DWORD_IMAGE, num, width, height, 1, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_FLOAT_IMAGE, num, width, height, 1, 1, pData);
	}

	static void PlotFloatImageAsync(int num, int width, int height, float* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_FLOAT_IMAGE, num, width, height, 1, 1, pData,  pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_3D_IMAGE, num, width, height, page, 1, pData);
	}

	static void Plot3DImageAsync(int num, int width, int height, int page, BYTE* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_3D_IMAGE, num, width, height, page, 1, pData,  pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_F3D_IMAGE, num, width, height, page, 1, pData);
	}

	static void PlotF3DImageAsync(int num, int width, int height, int page, float* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_F3D_IMAGE, num, width, height, page, 1, pData, pimpom_async_ids[0], pAsyncParams);
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
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_RGB_IMAGE, num, width, height, 1, 3, pData);
	}

	static void PlotRGBImageAsync(int num, int width, int height, BYTE* pData, float* pAsyncParams)
	{
		pimpom_plot_image_async(PIMPOM_PLOT_FORMAT_RGB_IMAGE, num, width, height, 1, 3, pData,  pimpom_async_ids[0], pAsyncParams);
	}


	/********************************************************************
	�@  �\  ��  �� : 1D�f�[�^��܂���\��
	��    ��    �� : PlotDataToChart
	��          �� :
	int	num�@			(in)�O���t�ԍ�
	int			dataNum		(in)�f�[�^�̐�
	float		*pData				(in)�l
	��    ��    �l :
	�@          �\ : �܂���O���t�ɒl��`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotDataToChartF(int num, int dataNum, float *pData)
	{
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_FLOAT_1D_CHART, num, dataNum, 1, 1, 1, pData);
	}

	static void PlotDataToChartD(int num, int dataNum, double *pData)
	{
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_DOUBLE_1D_CHART, num, dataNum, 1, 1, 1, pData);
	}

	static void PlotDataToChartI(int num, int dataNum, int *pData)
	{
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_INT_1D_CHART, num, dataNum, 1, 1, 1, pData);
	}

	static void PlotDataToChartB(int num, int dataNum, BYTE *pData)
	{
		pimpom_plot_image(PIMPOM_PLOT_FORMAT_BYTE_1D_CHART, num, dataNum, 1, 1, 1, pData);
	}

	/********************************************************************
	�@  �\  ��  �� : 2D�f�[�^���U�z�}�\��
	��    ��    �� : PlotDataToChart
	��          �� :
	int			num�@			(in)�O���t�ԍ�
	int			dataNum		(in)�f�[�^�̐�
	float		*pDataX,*pDataY		(in)�l
	��    ��    �l :
	�@          �\ : 2�����U�z�}�ɒl��`�悷��
	���t         �쐬��          ���e
	------------ --------------- ---------------------------------------
	Y.Ikeda         �V�K�쐬
	********************************************************************/
	static void PlotDataToChart2F(int num, int dataNum, float *pDataX, float *pDataY)
	{
		float *pdataBuf = new float[dataNum * 2];
		memcpy(pdataBuf, pDataX, dataNum * sizeof(float));
		memcpy(pdataBuf + dataNum, pDataY, dataNum * sizeof(float));

		pimpom_plot_image(PIMPOM_PLOT_FORMAT_FLOAT_2D_CHART, num, dataNum, 2, 1, 1, pdataBuf);

		delete[] pdataBuf;
	}

	static void PlotDataToChart2D(int num, int dataNum, double *pDataX, double *pDataY)
	{
		double *pdataBuf = new double[dataNum * 2];
		memcpy(pdataBuf, pDataX, dataNum * sizeof(double));
		memcpy(pdataBuf + dataNum, pDataY, dataNum * sizeof(double));

		pimpom_plot_image(PIMPOM_PLOT_FORMAT_DOUBLE_2D_CHART, num, dataNum, 2, 1, 1, pdataBuf);

		delete[] pdataBuf;
	}

	static void PlotDataToChart2I(int num, int dataNum, int *pDataX, int *pDataY)
	{
		int *pdataBuf = new int[dataNum * 2];
		memcpy(pdataBuf, pDataX, dataNum * sizeof(int));
		memcpy(pdataBuf + dataNum, pDataY, dataNum * sizeof(int));

		pimpom_plot_image(PIMPOM_PLOT_FORMAT_INT_2D_CHART, num, dataNum, 2, 1, 1, pdataBuf);

		delete[] pdataBuf;
	}

	static void PlotDataToChart2B(int num, int dataNum, BYTE *pDataX, BYTE *pDataY)
	{
		BYTE *pdataBuf = new BYTE[dataNum * 2];
		memcpy(pdataBuf, pDataX, dataNum * sizeof(BYTE));
		memcpy(pdataBuf + dataNum, pDataY, dataNum * sizeof(BYTE));

		pimpom_plot_image(PIMPOM_PLOT_FORMAT_BYTE_2D_CHART, num, dataNum, 2, 1, 1, pdataBuf);

		delete[] pdataBuf;
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_BYTE_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetByteImageAsync(int *num, int *width, int *height, BYTE** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_BYTE_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_SHORT_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetShortImageAsync(int *num, int *width, int *height, short** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_SHORT_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_WORD_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetWordImageAsync(int *num, int *width, int *height, WORD** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_WORD_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_LONG_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetLongImageAsync(int *num, int *width, int *height, long** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_LONG_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_DWORD_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetDwordImageAsync(int *num, int *width, int *height, DWORD** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_DWORD_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_FLOAT_IMAGE, num, width, height, 1, 1, pData);
	}

	static int GetFloatImageAsync(int *num, int *width, int *height, float** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_FLOAT_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_3D_IMAGE, num, width, height, page, 1, pData);
	}

	static int Get3DImageAsync(int *num, int *width, int *height, int *page, BYTE** ppData)
	{
		int channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_3D_IMAGE, num, width, height, page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_F3D_IMAGE, num, width, height, page, 1, pData);
	}

	static int GetF3DImageAsync(int *num, int *width, int *height, int *page, float** ppData)
	{
		int channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_F3D_IMAGE, num, width, height, page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_get_image(PIMPOM_PLOT_FORMAT_RGB_IMAGE, num, width, height, 1, 3, pData);
	}

	static int GetRGBImageAsync(int *num, int *width, int *height, BYTE** ppData)
	{
		int page, channel;
		return pimpom_get_image_async(PIMPOM_PLOT_FORMAT_RGB_IMAGE, num, width, height, &page, &channel, (void**)ppData, -1, NULL);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_PREPAIR_DRAWING, num, 0, 0, 0, 0, 0, 0, 0, 0, copy_src_num);
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_DRAW_IMAGE;
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_POINT_ON_IMAGE, num, x, y, 0, 0, 1, r, g, b, -1);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_LINE_ON_IMAGE, num, x0, y0, x1, y1, 1, r, g, b, -1);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_RECT_ON_IMAGE, num, x0, y0, x1, y1, 1, r, g, b, -1);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_CIRCLE_ON_IMAGE, num, cx, cy, rad, fill, 1, r, g, b, -1);
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
		pimpom_draw_figure(PIMPOM_PLOT_COMMAND_DRAW_CURSOR_ON_IMAGE, num, x, y, 0, 0, 1, r, g, b, -1);
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

		pimpom_draw_text(PIMPOM_PLOT_COMMAND_DRAW_TEXT_ON_IMAGE, num, x, y, buf, r, g, b, -1);
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

		pimpom_draw_text(PIMPOM_PLOT_COMMAND_SHOW_MESSAGE, 0, 0, 0, buf, 0, 0, 0, -1);
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

		pimpom_draw_text(PIMPOM_PLOT_COMMAND_SHOW_NEW_MESSAGE, 0, 0, 0, buf, 0, 0, 0, -1);
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_ADD_MARK;
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT;
		pShereMem->image_num = num;
		sprintf(pShereMem->message, "%f\n", val);

		pimpom_plot_send_data(WM_USER + 101, 0, pimpom_target_id);
		pimpom_plot_free_shere_mem(pShereMem, hShare);
	}



	/********************************************************************
	�@  �\  ��  �� : �O���t�ɒl�ǉ�
	��    ��    �� : AddDataToChart2
	��          �� : 
	int	num�@			(in)�O���t�ԍ�
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_ADD_DATA_TO_CAHRT2;
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_CLEAR_CAHRT;
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_BATCH_EXECUTE;
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
	�@  �\  ��  �� : �X�N���v�g�R�}���h���s
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

		pShereMem->type = PIMPOM_PLOT_COMMAND_EXECUTE;

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

		pimpom_get_image(PIMPOM_PLOT_FORMAT_MASK_IMAGE, num, width, height, 1, 1, pData);
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
			pimpom_plot_image(PIMPOM_PLOT_FORMAT_MASK_IMAGE, num, width, height, 1, 1, pData);
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
