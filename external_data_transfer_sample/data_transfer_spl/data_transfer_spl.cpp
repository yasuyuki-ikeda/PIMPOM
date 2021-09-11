// data_transfer_spl.cpp : 
//
//�E�O���v���O�����ŉ摜�𐶐�����PIMPOM�Ƀf�[�^�o�͂��s���A����ɂ����ǂݏo��
//�E�O���v���O��������PIMPOM�̉摜��ɐ}�`�`�悷��
//�E�O���v���O��������PIMPOM�̃O���t�Ƀv���b�g����

#include "stdafx.h"
#include <pimpom_plot.h>

int _tmain(int argc, _TCHAR* argv[])
{
	 {
		int num = 0, type=0, width = 512, height = 512, page = 10, channel=3;


		//�摜������0�ɓ����Ă���摜�̃T�C�Y���擾
		GetDataTypeSize(num, &type, &width, &height, &page);

		if (type < 0) 
		{
			//�����Ă��Ȃ���΁A�R�}���h���C���������
			printf("<�摜�ԍ�>,<����>,<��>,<���s>\n");
			if (scanf_s("%d,%d,%d,%d", &num, &width, &height, &page) != 4)	return 0;
		}
		else {

		}


		//�摜�����p������
		BYTE	*pMemB = (BYTE*)malloc(width*height*page*channel);
		short	*pMemS = (short*)malloc(width*height * sizeof(short));
		WORD	*pMemW = (WORD*)malloc(width*height * sizeof(WORD));
		long	*pMemL = (long*)malloc(width*height * sizeof(long));
		DWORD	*pMemDW = (DWORD*)malloc(width*height * sizeof(DWORD));
		float	*pMemF = (float*)malloc(width*height * sizeof(float)*page);



		////////////////////////////////////////////////////////////////////////////////
		//�摜�𐶐�����PIMPOM�ɏo�͂���
		///////////////////////////////////////////////////////////////////////////////

		//BYTE�摜�𐶐�����
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width] = (BYTE)i;
			}
		}
		PlotByteImage(num, width, height, pMemB);//�摜��PIMPOM�ɏo�͂���


		//SHORT�摜������
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemS[i + j*width] = (short)i;
			}
		}
		PlotShortImage(num + 1, width, height, pMemS); //PIMPOM�ɏo�͂���


		//WORD�摜������
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemW[i + j*width] = (WORD)i;
			}
		}
		PlotWordImage(num + 2, width, height, pMemW);//PIMPOM�ɏo�͂���

		//LONG�摜������
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemL[i + j*width] = (long)i;
			}
		}
		PlotLongImage(num + 3, width, height, pMemL);//PIMPOM�ɏo�͂���


		//DWORD�摜������
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemDW[i + j*width] = (DWORD)i;
			}
		}
		PlotDwordImage(num + 4, width, height, pMemDW);//PIMPOM�ɏo�͂���


		//FLOAT�摜������
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemF[i + j*width] = (float)i;
			}
		}
		PlotFloatImage(num + 5, width, height, pMemF);//PIMPOM�ɏo�͂���

		//3D�摜������
		for (int k = 0; k<page; k++) {
			for (int j = 0; j<height; j++) {
				for (int i = 0; i<width; i++) {
					pMemB[i + j*width + k*width*height] = (BYTE)(i + k * 5);
				}
			}
		}
		Plot3DImage(num + 6, width, height, page, pMemB);//PIMPOM�ɏo�͂���


		//F3D�摜������
		for (int k = 0; k<page; k++) {
			for (int j = 0; j<height; j++) {
				for (int i = 0; i<width; i++) {
					pMemF[i + j*width + k*width*height] = (float)(i + k * 5);
				}
			}
		}
		PlotF3DImage(num + 7, width, height, page, pMemF);//PIMPOM�ɏo�͂���

		//RGB�摜������
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width + 0 * width*height] = (BYTE)(255 - i);//r
				pMemB[i + j*width + 1 * width*height] = (BYTE)(100);//g
				pMemB[i + j*width + 2 * width*height] = (BYTE)(i);//b
			}
		}
		PlotRGBImage(num + 8, width, height, pMemB);//PIMPOM�ɏo�͂���


		//�}�X�N�摜������
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width ] = ((i - width / 2)*(i - width / 2) + (j - height / 2)*(j - height / 2) < 100 * 100) ? 0: 1;
			}
		}
		SetMaskImage(num, pMemB);//PIMPOM�ɏo�͂���

		//��Ɨ̈�ݒ�
		SetWorkArea(num, 0, width / 2 - 100, height / 2 - 100, width / 2 + 100, height / 2 + 100,true);


		////////////////////////////////////////////////////////////////////////////////
		//�摜��PIMPOM����擾
		///////////////////////////////////////////////////////////////////////////////


		GetByteImage(num, width, height, pMemB);//PIMPOM����BYTE�摜���擾
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width] += 100;
			}
		}
		PlotByteImage(num + 20, width, height, pMemB);//�摜��PIMPOM�Ƀv���b�g


		GetShortImage(num + 1, width, height, pMemS);//PIMPOM����SHORT�摜���擾
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemS[i + j*width] += 100;
			}
		}
		PlotShortImage(num + 21, width, height, pMemS);//�摜��PIMPOM�Ƀv���b�g


		GetWordImage(num + 2, width, height, pMemW);	//PIMPOM����WORD�摜���擾
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemW[i + j*width] += 100;
			}
		}
		PlotWordImage(num + 22, width, height, pMemW);//�摜��PIMPOM�Ƀv���b�g

		GetLongImage(num + 3, width, height, pMemL);	//PIMPOM����LONG�摜���擾
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemL[i + j*width] += 100;
			}
		}
		PlotLongImage(num + 23, width, height, pMemL);//�摜��PIMPOM�Ƀv���b�g


		GetDwordImage(num + 4, width, height, pMemDW);	//PIMPOM����DWORD�摜���擾
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemDW[i + j*width] += 100;
			}
		}
		PlotDwordImage(num + 24, width, height, pMemDW);//�摜��PIMPOM�Ƀv���b�g


		GetFloatImage(num + 5, width, height, pMemF);	//PIMPOM����FLOAT�摜���擾
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemF[i + j*width] += 100;
			}
		}
		PlotFloatImage(num + 25, width, height, pMemF);//�摜��PIMPOM�Ƀv���b�g

		Get3DImage(num + 6, width, height, page, pMemB);	//PIMPOM����3D�摜���擾
		for (int k = 0; k<page; k++) {
			for (int j = 0; j<height; j++) {
				for (int i = 0; i<width; i++) {
					pMemB[i + j*width + k*width*height] += 100;
				}
			}
		}
		Plot3DImage(num + 26, width, height, page, pMemB);//�摜��PIMPOM�Ƀv���b�g


		GetF3DImage(num + 7, width, height, page, pMemF);	//PIMPOM����F3D�摜���擾
		for (int k = 0; k<page; k++) {
			for (int j = 0; j<height; j++) {
				for (int i = 0; i<width; i++) {
					pMemF[i + j*width + k*width*height] += 100;
				}
			}
		}
		PlotF3DImage(num + 27, width, height, page, pMemF);//�摜��PIMPOM�Ƀv���b�g

		GetRGBImage(num + 8, width, height, pMemB);//PIMPOM����RGB�摜���擾
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width + 0 * width*height] += 100;
				pMemB[i + j*width + 1 * width*height] += 100;
				pMemB[i + j*width + 2 * width*height] += 100;
			}
		}
		PlotRGBImage(num + 28, width, height, pMemB);//�摜��PIMPOM�Ƀv���b�g


		//PIMPOM����}�X�N�摜������
		GetMaskImage(num, pMemB);//PIMPOM�ɏo�͂���
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width] = !pMemB[i + j*width];
			}
		}
		SetMaskImage(num+20, pMemB);//PIMPOM�ɏo�͂���

		//��Ɨ̈�擾
		RECT rct= GetWorkArea(num, 0);
		SetWorkArea(num + 20, 0, rct.left, rct.top, rct.right, rct.bottom, true);


		free(pMemB);
		free(pMemS);
		free(pMemW);
		free(pMemL);
		free(pMemDW);
		free(pMemF);



		////////////////////////////////////////////////////////////////////////////////
		//�}�`���v���b�g
		///////////////////////////////////////////////////////////////////////////////
		//�����v���b�g
		PrepeareDrawing(num + 10, num);//����
		for (int i = 0; i<100; i++) {
			DrawPointOnImage(num + 10, i, i, 0, 255, 0);//�v���b�g
		}
		DrawImage(num + 10);//�\��

							//�������v���b�g
		PrepeareDrawing(num + 11, num + 1);//����
		for (int i = 0; i<10; i++) {
			DrawLineOnImage(num + 11, 20, 20 + 20 * i, 100, 20 + 20 * i, 255, 0, 0);//�v���b�g
		}
		DrawImage(num + 11);//�\��

							//��`���v���b�g
		PrepeareDrawing(num + 12, num + 2);//����
		DrawRectOnImage(num + 12, 20, 20, 80, 80, 0, 0, 255);//�v���b�g
		DrawImage(num + 12);//�\��


							//�\���J�[�\�����v���b�g
		PrepeareDrawing(num + 13, num + 2);//����
		DrawCursorOnImage(num + 13, 100, 100, 0, 0, 255);//�v���b�g
		DrawImage(num + 13);//�\��


							//�~�`���v���b�g
		PrepeareDrawing(num + 14, num + 3);//����
		DrawCircleOnImage(num + 14, 100, 100, 50, true, 255, 0, 255);//�v���b�g
		DrawImage(num + 14);//�\��


							//�}�[�N������
		for (int i = 0; i<100; i++) {
			AddMark(num, i*0.3 + 10, i);//�v���b�g
		}

		////////////////////////////////////////////////////////////////////////////////
		//�O���t�`��
		///////////////////////////////////////////////////////////////////////////////

		//�܂���O���t
		float dataF[100];
		double dataD[100];
		int dataI[100];
		BYTE dataB[100];
		for (int i = 0; i<100; i++) 
		{
			dataF[i] = dataD[i] = dataI[i] = dataB[i] = i % 8;
			AddDataToChart(0, dataF[i]);
		}
		PlotDataToChartF(1, 100, dataF);
		PlotDataToChartD(2, 100, dataD);
		PlotDataToChartI(3, 100, dataI);
		PlotDataToChartB(4, 100, dataB);


		//2�����U�z�}
		float dataXF[100], dataYF[100];
		double dataXD[100], dataYD[100];
		int dataXI[100], dataYI[100];
		BYTE dataXB[100], dataYB[100];
		for (int i = 0; i<100; i++) 
		{
			dataXF[i] = dataXD[i] = dataXI[i] = dataXB[i] = i % 8;
			dataYF[i] = dataYD[i] = dataYI[i] = dataYB[i] = i % 6;
			AddDataToChart2(5, dataXF[i], dataYF[i]);
		}
		PlotDataToChart2F(6, 100, dataXF, dataYF);
		PlotDataToChart2D(7, 100, dataXD, dataYD);
		PlotDataToChart2I(8, 100, dataXI, dataYI);
		PlotDataToChart2B(9, 100, dataXB, dataYB);

		////////////////////////////////////////////////////////////////////////////////
		//�o�b�`���s
		///////////////////////////////////////////////////////////////////////////////
		char	param[5][1024] = { 0 };//���̓p�����^
		char	result[15][1024] = { 0 };//�o�̓p�����^
		char	filename[1024] = { 0 };
		int		state = 0;

		sprintf(param[0], "5");
		sprintf(param[1], "12");

		BatchExecute(40, filename, param, result, &state);

		ShowMessage("%s", result[0]);
	}
	return 0;

}

