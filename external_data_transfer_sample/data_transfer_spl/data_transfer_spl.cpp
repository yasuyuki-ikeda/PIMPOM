// data_transfer_spl.cpp : 
//
//・外部プログラムで画像を生成してPIMPOMにデータ出力を行い、さらにそれを読み出す
//・外部プログラムからPIMPOMの画像上に図形描画する
//・外部プログラムからPIMPOMのグラフにプロットする

#include "stdafx.h"
#include <pimpom_plot.h>

int _tmain(int argc, _TCHAR* argv[])
{
	 {
		int num = 0, type=0, width = 512, height = 512, page = 10, channel=3;


		//画像メモリ0に入っている画像のサイズを取得
		GetDataTypeSize(num, &type, &width, &height, &page);

		if (type < 0) 
		{
			//入っていなければ、コマンドラインから入力
			printf("<画像番号>,<高さ>,<幅>,<奥行>\n");
			if (scanf_s("%d,%d,%d,%d", &num, &width, &height, &page) != 4)	return 0;
		}
		else {

		}


		//画像生成用メモリ
		BYTE	*pMemB = (BYTE*)malloc(width*height*page*channel);
		short	*pMemS = (short*)malloc(width*height * sizeof(short));
		WORD	*pMemW = (WORD*)malloc(width*height * sizeof(WORD));
		long	*pMemL = (long*)malloc(width*height * sizeof(long));
		DWORD	*pMemDW = (DWORD*)malloc(width*height * sizeof(DWORD));
		float	*pMemF = (float*)malloc(width*height * sizeof(float)*page);



		////////////////////////////////////////////////////////////////////////////////
		//画像を生成してPIMPOMに出力する
		///////////////////////////////////////////////////////////////////////////////

		//BYTE画像を生成する
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width] = (BYTE)i;
			}
		}
		PlotByteImage(num, width, height, pMemB);//画像をPIMPOMに出力する


		//SHORT画像をつくる
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemS[i + j*width] = (short)i;
			}
		}
		PlotShortImage(num + 1, width, height, pMemS); //PIMPOMに出力する


		//WORD画像をつくる
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemW[i + j*width] = (WORD)i;
			}
		}
		PlotWordImage(num + 2, width, height, pMemW);//PIMPOMに出力する

		//LONG画像をつくる
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemL[i + j*width] = (long)i;
			}
		}
		PlotLongImage(num + 3, width, height, pMemL);//PIMPOMに出力する


		//DWORD画像をつくる
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemDW[i + j*width] = (DWORD)i;
			}
		}
		PlotDwordImage(num + 4, width, height, pMemDW);//PIMPOMに出力する


		//FLOAT画像をつくる
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemF[i + j*width] = (float)i;
			}
		}
		PlotFloatImage(num + 5, width, height, pMemF);//PIMPOMに出力する

		//3D画像をつくる
		for (int k = 0; k<page; k++) {
			for (int j = 0; j<height; j++) {
				for (int i = 0; i<width; i++) {
					pMemB[i + j*width + k*width*height] = (BYTE)(i + k * 5);
				}
			}
		}
		Plot3DImage(num + 6, width, height, page, pMemB);//PIMPOMに出力する


		//F3D画像をつくる
		for (int k = 0; k<page; k++) {
			for (int j = 0; j<height; j++) {
				for (int i = 0; i<width; i++) {
					pMemF[i + j*width + k*width*height] = (float)(i + k * 5);
				}
			}
		}
		PlotF3DImage(num + 7, width, height, page, pMemF);//PIMPOMに出力する

		//RGB画像をつくる
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width + 0 * width*height] = (BYTE)(255 - i);//r
				pMemB[i + j*width + 1 * width*height] = (BYTE)(100);//g
				pMemB[i + j*width + 2 * width*height] = (BYTE)(i);//b
			}
		}
		PlotRGBImage(num + 8, width, height, pMemB);//PIMPOMに出力する


		//マスク画像をつくる
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width ] = ((i - width / 2)*(i - width / 2) + (j - height / 2)*(j - height / 2) < 100 * 100) ? 0: 1;
			}
		}
		SetMaskImage(num, pMemB);//PIMPOMに出力する

		//作業領域設定
		SetWorkArea(num, 0, width / 2 - 100, height / 2 - 100, width / 2 + 100, height / 2 + 100,true);


		////////////////////////////////////////////////////////////////////////////////
		//画像をPIMPOMから取得
		///////////////////////////////////////////////////////////////////////////////


		GetByteImage(num, width, height, pMemB);//PIMPOMからBYTE画像を取得
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width] += 100;
			}
		}
		PlotByteImage(num + 20, width, height, pMemB);//画像をPIMPOMにプロット


		GetShortImage(num + 1, width, height, pMemS);//PIMPOMからSHORT画像を取得
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemS[i + j*width] += 100;
			}
		}
		PlotShortImage(num + 21, width, height, pMemS);//画像をPIMPOMにプロット


		GetWordImage(num + 2, width, height, pMemW);	//PIMPOMからWORD画像を取得
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemW[i + j*width] += 100;
			}
		}
		PlotWordImage(num + 22, width, height, pMemW);//画像をPIMPOMにプロット

		GetLongImage(num + 3, width, height, pMemL);	//PIMPOMからLONG画像を取得
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemL[i + j*width] += 100;
			}
		}
		PlotLongImage(num + 23, width, height, pMemL);//画像をPIMPOMにプロット


		GetDwordImage(num + 4, width, height, pMemDW);	//PIMPOMからDWORD画像を取得
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemDW[i + j*width] += 100;
			}
		}
		PlotDwordImage(num + 24, width, height, pMemDW);//画像をPIMPOMにプロット


		GetFloatImage(num + 5, width, height, pMemF);	//PIMPOMからFLOAT画像を取得
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemF[i + j*width] += 100;
			}
		}
		PlotFloatImage(num + 25, width, height, pMemF);//画像をPIMPOMにプロット

		Get3DImage(num + 6, width, height, page, pMemB);	//PIMPOMから3D画像を取得
		for (int k = 0; k<page; k++) {
			for (int j = 0; j<height; j++) {
				for (int i = 0; i<width; i++) {
					pMemB[i + j*width + k*width*height] += 100;
				}
			}
		}
		Plot3DImage(num + 26, width, height, page, pMemB);//画像をPIMPOMにプロット


		GetF3DImage(num + 7, width, height, page, pMemF);	//PIMPOMからF3D画像を取得
		for (int k = 0; k<page; k++) {
			for (int j = 0; j<height; j++) {
				for (int i = 0; i<width; i++) {
					pMemF[i + j*width + k*width*height] += 100;
				}
			}
		}
		PlotF3DImage(num + 27, width, height, page, pMemF);//画像をPIMPOMにプロット

		GetRGBImage(num + 8, width, height, pMemB);//PIMPOMからRGB画像を取得
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width + 0 * width*height] += 100;
				pMemB[i + j*width + 1 * width*height] += 100;
				pMemB[i + j*width + 2 * width*height] += 100;
			}
		}
		PlotRGBImage(num + 28, width, height, pMemB);//画像をPIMPOMにプロット


		//PIMPOMからマスク画像をつくる
		GetMaskImage(num, pMemB);//PIMPOMに出力する
		for (int j = 0; j<height; j++) {
			for (int i = 0; i<width; i++) {
				pMemB[i + j*width] = !pMemB[i + j*width];
			}
		}
		SetMaskImage(num+20, pMemB);//PIMPOMに出力する

		//作業領域取得
		RECT rct= GetWorkArea(num, 0);
		SetWorkArea(num + 20, 0, rct.left, rct.top, rct.right, rct.bottom, true);


		free(pMemB);
		free(pMemS);
		free(pMemW);
		free(pMemL);
		free(pMemDW);
		free(pMemF);



		////////////////////////////////////////////////////////////////////////////////
		//図形をプロット
		///////////////////////////////////////////////////////////////////////////////
		//線をプロット
		PrepeareDrawing(num + 10, num);//準備
		for (int i = 0; i<100; i++) {
			DrawPointOnImage(num + 10, i, i, 0, 255, 0);//プロット
		}
		DrawImage(num + 10);//表示

							//直線をプロット
		PrepeareDrawing(num + 11, num + 1);//準備
		for (int i = 0; i<10; i++) {
			DrawLineOnImage(num + 11, 20, 20 + 20 * i, 100, 20 + 20 * i, 255, 0, 0);//プロット
		}
		DrawImage(num + 11);//表示

							//矩形をプロット
		PrepeareDrawing(num + 12, num + 2);//準備
		DrawRectOnImage(num + 12, 20, 20, 80, 80, 0, 0, 255);//プロット
		DrawImage(num + 12);//表示


							//十字カーソルをプロット
		PrepeareDrawing(num + 13, num + 2);//準備
		DrawCursorOnImage(num + 13, 100, 100, 0, 0, 255);//プロット
		DrawImage(num + 13);//表示


							//円形をプロット
		PrepeareDrawing(num + 14, num + 3);//準備
		DrawCircleOnImage(num + 14, 100, 100, 50, true, 255, 0, 255);//プロット
		DrawImage(num + 14);//表示


							//マークをつける
		for (int i = 0; i<100; i++) {
			AddMark(num, i*0.3 + 10, i);//プロット
		}

		////////////////////////////////////////////////////////////////////////////////
		//グラフ描画
		///////////////////////////////////////////////////////////////////////////////

		//折れ線グラフ
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


		//2次元散布図
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
		//バッチ実行
		///////////////////////////////////////////////////////////////////////////////
		char	param[5][1024] = { 0 };//入力パラメタ
		char	result[15][1024] = { 0 };//出力パラメタ
		char	filename[1024] = { 0 };
		int		state = 0;

		sprintf(param[0], "5");
		sprintf(param[1], "12");

		BatchExecute(40, filename, param, result, &state);

		ShowMessage("%s", result[0]);
	}
	return 0;

}

