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
//CPimpomAPI
//数値計算
/*******************************************************************/

//**************************************
//         インクルードファイル         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"

// *************************************
//         マ  ク  ロ   定   義         
// *************************************

// *************************************
//         外    部    宣    言         
// *************************************

// *************************************
//         構  造  体   定   義         
// *************************************

// *************************************
//         静    的    変    数         
// *************************************

// *************************************
//         外    部    変    数         
// *************************************

// *************************************
//         関    数    参    照         
// *************************************


double	CPimpomAPI::calc_mod( double x , double a){
	double y;

	y = x - a*floor( x / a );
	return	y;
}

//float型の配列から最大値を求める
float	CPimpomAPI::calc_float_max( float *pf , long len)
{
	int i;
	float	max;

	if(pf==NULL){
		AfxMessageBox("グローバル calc_float_max( float *pf , long len) : 引数が配列の先頭ポインタをさしていません");
	}

	max = *pf;
	for(i=0 ; i<len ; i++){
		if( *(pf + i)>max ){
			max = *(pf + i);
		}
	}
	return	max;
}

//float型の配列から最小値を求める
float	CPimpomAPI::calc_float_min( float *pf , long len)
{
	int i;
	float	min;

	if(pf==NULL){
		AfxMessageBox("グローバル calc_float_max( float *pf , long len) : 引数が配列の先頭ポインタをさしていません");
	}

	min = *pf;
	for(i=0 ; i<len ; i++){
		if( *(pf + i)<min ){
			min = *(pf + i);
		}
	}
	return	min;
}

//0～1の間の乱数
double	CPimpomAPI::calc_random()
{
	double	ret;
	ret = (double)rand();
	ret /= 32767.0;
	return	ret;
}

//ガウス乱数 ( 標準偏差1 )
double  CPimpomAPI::calc_gaus()	//Box-Muller Method
{
	static bool	flg = false;
	static double	remind;
	double	x1, x2, R_sq, tmp;

		if( !flg ){
			do{
				x1 = 2.0*calc_random() - 1.0;
				x2 = 2.0*calc_random() - 1.0;
				R_sq = x1*x1  +  x2*x2;

			}while( R_sq >= 1.0  ||  R_sq==0.0 );

			tmp = sqrt( -2.0*log( R_sq ) / R_sq );

			remind = x1 * tmp;
			flg = true;
			return	x2 * tmp;

		}else{
			flg = false;
			return  remind;
		}
}


//ユークリッド距離
double  CPimpomAPI::calc_dist(double x0, double y0, double x1, double y1)
{
	return sqrt( (x0-x1)*(x0-x1) + (y0-y1)*(y0-y1) );
}


//最小二乗法
bool CPimpomAPI::calc_lsm(float **ppA, float *pb, float *pX, int spl_num, int param_num)
{
    int i,j,k;
    float **ppAT, **ppC, *pd;
    float tmpval;
    double   p,q;


	//メモリ領域確保
	ppAT = (float**)new DWORD[ spl_num ];
	for(i=0 ; i<spl_num ; i++){
		ppAT[i] = new float[param_num];
	}

	ppC = (float**)new DWORD[ param_num ];
	for(i=0 ; i<param_num ; i++){
		ppC[i] = new float[ param_num ];
	}

	pd = new float[param_num];


    /* [a]の転置行列をつくる */
    for(i=0; i<spl_num; i++){
		for(j=0; j<param_num; j++){
			ppAT[i][j] = ppA[j][i];
		}
    }

	/* [c]=[a]^T*[a]をつくる*/
    for(i=0; i<param_num; i++){
        for(j=0; j<param_num; j++){
            tmpval=0;

				for(k=0; k<spl_num; k++){
					tmpval += ppAT[k][i] * ppA[j][k];
				}
            ppC[i][j] = tmpval;
        }
    }

/* [d]=[a]^T[b]をつくる*/
    for(i=0; i<param_num; i++){
        tmpval=0;
			for(k=0; k<spl_num; k++){
            tmpval += ppAT[k][i] * pb[k];
        }
        pd[i] = tmpval;
    }

//[c]の逆行列を作る
	for(k=0; k<param_num ; ++k){
        p = ppC[k][k];
        ppC[k][k]=1.0;
			for (j=0; j<param_num; ++j)
						ppC[k][j] /= p;

			for(i=0; i<param_num; ++i){
				if(i!=k){
					q = ppC[i][k];
					ppC[i][k]=0.0;

					for(j=0; j<param_num; ++j )
		                ppC[i][j] -= q * ppC[k][j];
				}
			}
	}

/* [z]=[c]-1[d]をつくる*/

    for(i=0; i<param_num; i++){
        tmpval = 0;
	        for(k=0; k<param_num; k++){
		        tmpval += ppC[i][k] * pd[k];
			}
        pX[i] = tmpval;
    }


	//メモリ領域開放
	for(i=0 ; i</*spl_num*/4 ; i++){
		delete[]	ppAT[i];
	}
	delete[]	ppAT;

	for(i=0 ; i<param_num ; i++){
		delete[]	ppC[i];
	}
	delete[]	ppC;

	delete[]	pd;

	return true;
}