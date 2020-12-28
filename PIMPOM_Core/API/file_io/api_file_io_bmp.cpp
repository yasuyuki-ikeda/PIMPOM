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
//�t�@�C��IO(Bitmap)
/*******************************************************************/

//**************************************
//         �C���N���[�h�t�@�C��         
//**************************************
#include "stdafx.h"
#include "PIMPOM_API.h"
#include "atlimage.h"

// *************************************
//         �}  �N  ��   ��   �`         
// *************************************

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

// *************************************
//         ��    ��    �Q    ��         
// *************************************


/********************************************************************
�@  �\  ��  �� :  �r�b�g�}�b�v�t�@�C����ǂݍ���
��    ��    �� : LoadBitmapPart
��          �� : long		image_number	(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
				int		partX,partY		(in)�����ǂݍ��݈ʒu
				int		partW,partH		(in)�����ǂݍ��݃T�C�Y
��    ��    �l : ��������� true
�@          �\ : 8bit�ɸہA24bit�װ�ɑΉ�
�����ǂݍ��ݑΉ�
���t         �쐬��          ���e
------------ --------------- ---------------------------------------
Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadBitmap(long image_number, CString	pathname, int partX, int partY, int partW, int partH)
{
	CFile  file;
	BITMAPFILEHEADER	bmp_file_header;
	BITMAPINFOHEADER	bmp_info_header;
	long				bmp_width_gap;
	CDataUnit			*p_du;
	CSize				size;
	unsigned char		*ptmp_rgb;
	int			i,j;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

		if( ! file.Open(pathname,CFile::modeRead|CFile::typeBinary) )	return false;

	//BITMAPFILEHEADER�̓ǂ݂���
	file.Read( &bmp_file_header , sizeof(BITMAPFILEHEADER) );
		if( bmp_file_header.bfType != 0x4d42){	
			AfxMessageBox(_T("���̃t�@�C���̓r�b�g�}�b�v�ł͂���܂���"));
			file.Close();
			return false;
		}

	//BITMAPINFOHEADER�̓ǂ݂���
	file.Read( &bmp_info_header, sizeof(BITMAPINFOHEADER) );
	size.cx = bmp_info_header.biWidth;//�T�C�Y����
	size.cy = bmp_info_header.biHeight;

	//
	if (bmp_info_header.biSize > sizeof(BITMAPINFOHEADER)){
		file.Seek(bmp_info_header.biSize - sizeof(BITMAPINFOHEADER), CFile::current);
	}

		if(bmp_info_header.biBitCount == 8){//8�r�b�g�r�b�g�}�b�v
			//�V�������������擾
			p_du = FormatDataUnit( image_number , BYTE_FORMAT, size, 1, true);
			if( p_du == NULL )	return	false;//�������擾�Ɏ��s�����Ƃ�

			////DIB�f�[�^����RAW�f�[�^���̍�
			if( size.cx % 4 == 0)	bmp_width_gap = 0;	
			else			bmp_width_gap = 4 - size.cx % 4;

			////�J���[�p���b�g�̓ǂݍ���
			file.Read( p_du->pDispBufferBmpInfo->bmiColors, 1024);



			//�f�[�^�ǂݍ���
			if (partX >= 0 && partY >= 0 && partX < size.cx - 1 && partY < size.cy-1 && partW > 0 && partH > 0 )
			{//�����ǂݍ���

				if (partX + partW > size.cx ){
					partW = size.cx - partX;
				}

				if (partY + partH > size.cy ){
					partH = size.cy - partY;
				}

				for (j = size.cy - 1; j >= 0; j--)
				{//�㉺���]

					if (j >= partY && j < partY + partH)
					{
						file.Seek(partX, CFile::current);
						file.Read(p_du->pByteData + size.cx * j + partX, partW);
						file.Seek(size.cx - (partX + partW), CFile::current);
					}
					else{
						file.Seek(size.cx , CFile::current);
					}

					file.Seek(bmp_width_gap, CFile::current);
				}
			}
			else
			{//�S�̓ǂݍ���
				for (j = size.cy - 1; j >= 0; j--)
				{//�㉺���]
					file.Read(p_du->pByteData + size.cx * j, size.cx);
					file.Seek(bmp_width_gap, CFile::current);
				}
			}
		}
		else if(bmp_info_header.biBitCount == 24)//24�r�b�g�r�b�g�}�b�v
		{
			//�V�������������擾
			p_du = FormatDataUnit( image_number , RGB_FORMAT, size, 3, true);
			if( p_du == NULL )	return	false;//�������擾�Ɏ��s�����Ƃ�

			////DIB�f�[�^����RAW�f�[�^���̍�
			if( (size.cx*3) % 4 == 0)	bmp_width_gap = 0;	
			else				bmp_width_gap = 4 - (size.cx*3) % 4;

			////�f�[�^�̈ꎞ�ۊǏꏊ�̍쐬
			ptmp_rgb = (unsigned char*)malloc( (size.cx*3)*size.cy );
			if(!ptmp_rgb)	return	false;
			memset(ptmp_rgb, 0, (size.cx * 3)*size.cy);

			////�f�[�^�ǂݍ���
			if (partX >= 0 && partY >= 0 && partX < size.cx - 1 && partY < size.cy - 1 && partW > 0 && partH > 0)
			{//�����ǂݍ���

				if (partX + partW > size.cx ){
					partW = size.cx - partX;
				}

				if (partY + partH > size.cy ){
					partH = size.cy - partY;
				}


				for (j = size.cy - 1; j >= 0; j--)
				{
					if (j >= partY && j < partY + partH)
					{
						file.Seek(partX * 3, CFile::current);
						file.Read(ptmp_rgb + (size.cx * 3) * j + partX * 3, (partW * 3));
						file.Seek((size.cx - (partX + partW)) * 3, CFile::current);
					}
					else{
						file.Seek(size.cx * 3, CFile::current);
					}

					file.Seek(bmp_width_gap, CFile::current);
				}
			}
			else
			{//�S�̓ǂݍ���
				for(j = size.cy-1  ;  j>=0  ;  j-- ){//�㉺���]
					file.Read( ptmp_rgb  +	(size.cx*3) * j  , (size.cx*3)	);
					file.Seek( bmp_width_gap , CFile::current);
				}
			}



			////�f�[�^�̕ϊ�
			for(j=0 ; j<size.cy ; j++){
				for(i=0 ; i<size.cx ; i++){
					*(p_du->pByteData + i + j*size.cx )					   = *(ptmp_rgb + i*3 + j*(size.cx*3)+2);//r
					*(p_du->pByteData + i + j*size.cx + size.cx*size.cy)   = *(ptmp_rgb + i*3 + j*(size.cx*3)+1);//g
					*(p_du->pByteData + i + j*size.cx + size.cx*size.cy*2) = *(ptmp_rgb + i*3 + j*(size.cx*3)+0);//b
				}
			}
			free(ptmp_rgb);//�f�[�^�̈ꎞ�ۊǏꏊ�̊J��

		}else if(bmp_info_header.biBitCount == 32){//32�r�b�g�r�b�g�}�b�v
			//�V�������������擾
			p_du = FormatDataUnit( image_number , RGB_FORMAT, size, 3, true);
			if( p_du == NULL )	return	false;//�������擾�Ɏ��s�����Ƃ�

			////�f�[�^�̈ꎞ�ۊǏꏊ�̍쐬
			ptmp_rgb = (unsigned char*)malloc( (size.cx*4)*size.cy );
			if(!ptmp_rgb)	return	false;

			////�f�[�^�ǂݍ���
			for(j = size.cy-1  ;  j>=0  ;  j-- ){//�㉺���]
				file.Read( ptmp_rgb  +	(size.cx*4) * j  , (size.cx*4)	);
			}
			////�f�[�^�̕ϊ�
			for(j=0 ; j<size.cy ; j++){
				for(i=0 ; i<size.cx ; i++){
					*(p_du->pByteData + i + j*size.cx )					   = *(ptmp_rgb + i*4 + j*(size.cx*4)+2);//r
					*(p_du->pByteData + i + j*size.cx + size.cx*size.cy)   = *(ptmp_rgb + i*4 + j*(size.cx*4)+1);//g
					*(p_du->pByteData + i + j*size.cx + size.cx*size.cy*2) = *(ptmp_rgb + i*4 + j*(size.cx*4)+0);//b
				}
			}
			free(ptmp_rgb);//�f�[�^�̈ꎞ�ۊǏꏊ�̊J��

		}else{//8bit,24bit�ȊO
			if( !LoadAnyImage(image_number, pathname) ){
				file.Close();
				return false;
			}
		}



	p_du->DataName = file.GetFileTitle();//���O��t����
	p_du->SrcFilePath = pathname;//�ǂݍ��񂾃t�@�C���̃p�X���c��
	file.Close();

	SetFileIOLog(pathname);//�t�@�C�����̃��O���c��

	return	true;
}


/********************************************************************
�@  �\  ��  �� :  �T�C�Y/�t�H�[�}�b�g����Ńr�b�g�}�b�v�t�@�C����ǂݍ���
��    ��    �� : LoadSizedBitmap
��          �� : BYTE		*p_data		(out)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				CSize		size		(in)�摜�f�[�^�̃T�C�Y
				bool		is_rgb		(in)�t�H�[�}�b�g�itrue:24bit�װ  false:8bit�ɸہj
				CString		pathname	(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : �T�C�Y/̫�ϯĂ��قȂ�ꍇ�͓ǂݍ��܂Ȃ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::LoadSizedBitmap(BYTE *p_data , CSize size, bool is_rgb, CString	pathname)
{
	CFile  file;
	BITMAPFILEHEADER	bmp_file_header;
	BITMAPINFOHEADER	bmp_info_header;
	long				bmp_width_gap;


		if( ! file.Open(pathname,CFile::modeRead|CFile::typeBinary) )	return false;

		if(p_data == NULL)	return	false;//���������m�ۂ���Ă��Ȃ�

	//BITMAPFILEHEADER�̓ǂ݂���
	file.Read( &bmp_file_header , sizeof(BITMAPFILEHEADER) );
		if( bmp_file_header.bfType != 0x4d42){	
			//AfxMessageBox(_T("���̃t�@�C���̓r�b�g�}�b�v�ł͂���܂���"));
			file.Close();
			return false;
		}

	//BITMAPINFOHEADER�̓ǂ݂���
	file.Read( &bmp_info_header, sizeof(BITMAPINFOHEADER) );
		//�T�C�Y������Ȃ�
		if(	bmp_info_header.biWidth != size.cx   ||   bmp_info_header.biHeight != size.cy )
		{
			AfxMessageBox(pathname + "�̓T�C�Y�������܂���");
			file.Close();
			return false;
		}

		//�J���[/���m�N��������Ȃ�
		if( (is_rgb==false && bmp_info_header.biBitCount != 8) ||
			(is_rgb==true  && (bmp_info_header.biBitCount != 24  &&  bmp_info_header.biBitCount != 32)) )
		{
			AfxMessageBox(pathname + "�̓r�b�g�}�b�v�`���������܂���");
			file.Close();
			return false;
		}

		if(bmp_info_header.biBitCount == 8)//���m�N��
		{
			////DIB�f�[�^����RAW�f�[�^���̍�
			if( size.cx % 4 == 0)	bmp_width_gap = 0;	
			else					bmp_width_gap = 4 - size.cx % 4;

			file.Seek( 1024 , CFile::current );//�J���[�p���b�g�����΂�

			////�f�[�^�ǂݍ���
			for(int j = size.cy-1  ;  j>=0  ;  j-- ){//�㉺���]
				file.Read( p_data  +  size.cx * j  , size.cx  );
				file.Seek( bmp_width_gap , CFile::current);
			}
		}
		else if(bmp_info_header.biBitCount == 24)//24�J���[
		{
			////DIB�f�[�^����RAW�f�[�^���̍�
			if( size.cx % 4 == 0)	bmp_width_gap = 0;	
			else					bmp_width_gap = 4 - (size.cx*3) % 4;

			
			////�f�[�^�ǂݍ���
			BYTE *buffer = new BYTE[size.cx*3];
			if(!buffer)	return false;

			for(int j = size.cy-1  ;  j>=0  ;  j-- ){//�㉺���]
				file.Read( buffer , size.cx*3  );
				for(int i=0 ; i<size.cx ; i++){
					*(p_data  +  size.cx * j + i)					  = *(buffer + i*3 + 2);//r
					*(p_data  +  size.cx * j + i + size.cx*size.cy)   = *(buffer + i*3 + 1);//g
					*(p_data  +  size.cx * j + i + size.cx*size.cy*2) = *(buffer + i*3 + 0);//b
				}
				file.Seek( bmp_width_gap , CFile::current);
			}

			delete[] buffer;

		}
		else if(bmp_info_header.biBitCount == 32)//32�J���[
		{
			////�f�[�^�ǂݍ���
			BYTE *buffer = new BYTE[size.cx*4];
			if(!buffer)	return false;

			for(int j = size.cy-1  ;  j>=0  ;  j-- ){//�㉺���]
				file.Read( buffer , size.cx*4  );
				for(int i=0 ; i<size.cx ; i++){
					*(p_data  +  size.cx * j + i)					  = *(buffer + i*4 + 2);//r
					*(p_data  +  size.cx * j + i + size.cx*size.cy)   = *(buffer + i*4 + 1);//g
					*(p_data  +  size.cx * j + i + size.cx*size.cy*2) = *(buffer + i*4 + 0);//b
				}
			}

			delete[] buffer;
		}


	file.Close();

	SetFileIOLog(pathname);//�t�@�C�����̃��O���c��

	return	true;
}


/********************************************************************
�@  �\  ��  �� :  �\���f�[�^���r�b�g�}�b�v�Ƃ��ĕۑ�����
��    ��    �� : SaveViewAsBitmap
��          �� : long		image_number	(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname		(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : �ۑ������r�b�g�}�b�v�̃t�H�[�}�b�g�͈ȉ��̂Ƃ���
					RGB�^�ARGB 3D�^�� 24bit�װ
					����ȊO�� 8bit�ɸ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveViewAsBitmap(long image_number, CString	pathname)
{
	CFile		file;
	CDataUnit	*p_du;
	BITMAPFILEHEADER	bmp_file_header;
	CSize	size;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	p_du = GetDataUnit( image_number );//�w�肳�ꂽ�摜���������擾����
		if(p_du == NULL)	return	false;//�w�肳�ꂽ�摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�
	size = p_du->DataSize;

		if( ! file.Open(pathname , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //�t�@�C�����J��
	

	if( (p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT) || p_du->DispRGB == false){//���m�N��
		//BITMAPFILEHEADER �̍쐬�ƕۑ�
		bmp_file_header.bfType = 0x4d42;
		bmp_file_header.bfSize = 14+40+1024+(p_du->DispBufferRow * size.cy);
		bmp_file_header.bfReserved1 = 0;
		bmp_file_header.bfReserved2 = 0;
		bmp_file_header.bfOffBits = 54+1024 ;
		file.Write( &bmp_file_header , sizeof(BITMAPFILEHEADER) );

		//BITMAPINFO�̕ۑ�
		file.Write( p_du->pDispBufferBmpInfo , sizeof(BITMAPINFOHEADER) + 1024 );
		//�f�[�^�̕ۑ�
		file.Write( p_du->pDispBuffer , p_du->DispBufferRow * size.cy);

	}else{//RGB
		//BITMAPFILEHEADER �̍쐬�ƕۑ�
		bmp_file_header.bfType = 0x4d42;
		bmp_file_header.bfSize = 14+40+( p_du->DispBufferRowRGB * size.cy);
		bmp_file_header.bfReserved1 = 0;
		bmp_file_header.bfReserved2 = 0;
		bmp_file_header.bfOffBits = 54;
		file.Write( &bmp_file_header , sizeof(BITMAPFILEHEADER) );

		//BITMAPINFO�̕ۑ�
		file.Write( p_du->pDispBufferBmpInfoRGB , sizeof(BITMAPINFOHEADER) );
		//�f�[�^�̕ۑ�
		file.Write( p_du->pDispBufferRGB , p_du->DispBufferRowRGB * size.cy);
	}


	file.Close();

	SetFileIOLog(pathname);//�t�@�C�����̃��O���c��

	return	true;
}


/********************************************************************
�@  �\  ��  �� :  �\���f�[�^���r�b�g�}�b�v�Ƃ��ĕۑ�����
��    ��    �� : SaveViewAsBitmap
��          �� : long		image_number	(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ��������� true
�@          �\ : �ۑ�����t�@�C����I�����邽�߂̃t�@�C���_�C�A���O���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveViewAsBitmap(long image_number)
{
	CDataUnit	*p_du;

	if (image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	p_du = GetDataUnit(image_number);//���Ă����ꂽ�摜���������擾����
	if (p_du == NULL)	return	false;////�w�肳�ꂽ�摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�


	CString pathname,filename;
	filename = GetSrcFileName(image_number);
	CFileDialog myDLG(false , _T("bmp") , filename, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
							"�r�b�g�}�b�v(*.bmp)|*.BMP|���ׂẴt�@�C��(*.*)|*.*||");


		if( myDLG.DoModal() != IDOK)	return	false;
	pathname = myDLG.GetPathName();

	return	SaveViewAsBitmap( image_number , pathname);
}


/********************************************************************
�@  �\  ��  �� :  8bit�̃f�[�^���r�b�g�}�b�v�Ƃ��ĕۑ�����
��    ��    �� : SaveByteAsBitmap
��          �� : BYTE			*p_data			(in)�f�[�^�i�[�惁�����̈�̐擪�|�C���^
				BITMAPINFO		*pBmpInfo		(in)�r�b�g�}�b�v���
				CSize			size			(in)�摜�T�C�Y
				CString			pathname		(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : 8bit�ɸۃr�b�g�}�b�v�Ƃ��ĕۑ�
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveByteAsBitmap(BYTE *p_data , BITMAPINFO *pBmpInfo, CSize	size, CString pathname)
{
	CFile		file;
	BITMAPFILEHEADER	bmp_file_header;
	int j, gap;
	char	blank[4];

		if(p_data ==NULL)	return	false;
		if( ! file.Open(pathname , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //�t�@�C�����J��

	//BITMAPFILEHEADER �̍쐬�ƕۑ�
    bmp_file_header.bfType = 0x4d42;
    bmp_file_header.bfSize = 14+40+1024+( size.cx*size.cy);
    bmp_file_header.bfReserved1 = 0;
    bmp_file_header.bfReserved2 = 0;
    bmp_file_header.bfOffBits = 54+1024 ;
	file.Write( &bmp_file_header , sizeof(BITMAPFILEHEADER) );

	//BITMAPINFO�̕ۑ�
	file.Write( pBmpInfo , sizeof(BITMAPINFOHEADER) + 1024 );
	size.cx = pBmpInfo->bmiHeader.biWidth;
	size.cy = pBmpInfo->bmiHeader.biHeight;

	//�f�[�^�̕ۑ�
	if(size.cx%4 == 0)		gap = 0;
	else					gap = 4 - size.cx%4;

		for(j=size.cy - 1  ;  j>=0  ;  j--){//�㉺���]�����ăt�@�C�����͂���
			file.Write( p_data + j*size.cx , size.cx);
			file.Write( blank , gap );
		}

	file.Close();

	SetFileIOLog(pathname);//�t�@�C�����̃��O���c��

	return	true;
}




