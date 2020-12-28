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
//�t�@�C��IO(CDataUnit�̃A�[�J�C�u)
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
�@  �\  ��  �� :  �摜�������̃A�[�J�C�u�f�[�^���t�@�C������ǂݏo��
��    ��    �� : LoadArchivedImage
��          �� : long		image_number(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		pathname	(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
 2016.0512   Y.Ikeda         �}�[�N�̈�������ύX
********************************************************************/
bool	CPimpomAPI::LoadArchivedImage(long image_number , CString pathname)
{
	CFile		file;
	CDataUnit	*p_du;
	CString		str;
	int	n;

		if(image_number == CURRENT_IMAGE)		image_number =GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

		if(!file.Open(pathname ,  CFile::modeRead | CFile::typeBinary ) ){//�t�@�C�����J��
			str = pathname + _T(" ���J���܂���");
			AfxMessageBox(str);
			return	false;
		}




//////�w�b�_��ǂݍ���
	char	buf[ sizeof(DATA_UNIT_INFO) ];
	DATA_UNIT_INFO	*header;

	file.Read( buf , sizeof(DATA_UNIT_INFO));
	header = (DATA_UNIT_INFO*)buf;

	


///////�������擾
	CSize	size( header->DataSizeX , header->DataSizeY );

		switch( header->DataType ){
			case BYTE_FORMAT://BYTE
				p_du = FormatDataUnit( image_number , BYTE_FORMAT, size, 1, true);
				break;

			case THREE_D_FORMAT://3D
				p_du = FormatDataUnit( image_number , THREE_D_FORMAT, size, header->PageNumber, true);
				break;

			case FLOAT_FORMAT://float
				p_du = FormatDataUnit( image_number , FLOAT_FORMAT, size, 1, true);
				break;

			case COMPLEX_FORMAT://complex
				p_du = FormatDataUnit( image_number , COMPLEX_FORMAT, size, 1, true);
				break;

			case XY_FORMAT://XY
				p_du = FormatDataUnit( image_number , XY_FORMAT, size, 1, true);
				break;

			case XYZ_FORMAT://XYZ
				p_du = FormatDataUnit( image_number , XYZ_FORMAT, size, 1, true);
				break;

			case FLOAT_3D_FORMAT://F3D
				p_du = FormatDataUnit( image_number , FLOAT_3D_FORMAT, size, header->PageNumber, true);
				break;

			case RGB_FORMAT://RGB
				p_du = FormatDataUnit( image_number , RGB_FORMAT, size, 1, true);
				break;

			case SHORT16_FORMAT://Short
				p_du = FormatDataUnit( image_number , SHORT16_FORMAT, size, 1, true);
				break;

			case WORD_FORMAT://Word
				p_du = FormatDataUnit( image_number , WORD_FORMAT, size, 1, true);
				break;

			case LONG32_FORMAT://Long
				p_du = FormatDataUnit( image_number , LONG32_FORMAT, size, 1, true);
				break;

			case DWORD_FORMAT://DWord
				p_du = FormatDataUnit( image_number , DWORD_FORMAT, size, 1, true);
				break;

			case RGB_3D_FORMAT://RGB3D
				p_du = FormatDataUnit( image_number , RGB_3D_FORMAT, size, header->PageNumber, true);
				break;

			default:
				//AfxMessageBox("�T�|�[�g����Ă��Ȃ��f�[�^�^�C�v�ł�");
				str.Format("%d", header->DataType );
				AfxMessageBox(str);
				file.Close();
				return	false;
		}

		if(p_du == NULL){
			file.Close();
			return	false;//�������擾�Ɏ��s�����Ƃ�
		}
	

///////�\���f�[�^��ǂݍ���
	if(header->DataType != RGB_FORMAT && header->DataType != RGB_3D_FORMAT){//�O���[
		file.Read( p_du->pDispBuffer , p_du->DispBufferRow * p_du->DataSize.cy );
		//file.Read( p_du->pDispBufferBmpInfo->bmiColors , 1024 );
	}else{//RGB
		file.Read( p_du->pDispBufferRGB , p_du->DispBufferRowRGB * p_du->DataSize.cy );
	}

///////�}�X�N�f�[�^��ǂݍ���
	file.Read( p_du->pMaskData , p_du->DataSize.cx * p_du->DataSize.cy );

////////���f�[�^������ǂݍ���
	switch( header->DataType ){
		case BYTE_FORMAT:
			file.Read( p_du->pByteData , p_du->DataSize.cx * p_du->DataSize.cy );
			break;

		case THREE_D_FORMAT://3D
			{
				for(n=0 ; n<p_du->PageNumber ; n++){
					file.Read( p_du->pByteData + p_du->DataSize.cx * p_du->DataSize.cy * n ,  p_du->DataSize.cx * p_du->DataSize.cy );
					str.Format("%d/%d���ǂݍ���", n+1, p_du->PageNumber);//���܂ŉ����ǂݍ��񂾂������b�Z�|�W�Ƃ��ĕ\��
				}
				p_du->DispPage = header->DispPage;
				p_du->DispChannel = header->DispChannel;
			}
			break;

		case FLOAT_FORMAT://float
			file.Read( p_du->pFloatData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) );
			break;
		case COMPLEX_FORMAT://complex
			file.Read( p_du->pFloatData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) * 2 );
			p_du->CreateComplexPolarComponents();//�p���[�A�ʑ������߂�
			break;

		case XY_FORMAT://XY
			file.Read( p_du->pFloatData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) * 2 );
			break;

		case XYZ_FORMAT://XYZ
			file.Read( p_du->pFloatData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) * 3 );
			break;

		case FLOAT_3D_FORMAT://F3D
			{
				for(n=0 ; n<p_du->PageNumber ; n++){
					float	*addr = p_du->pFloatData + p_du->DataSize.cx * p_du->DataSize.cy * n;
					file.Read( addr , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) );
					str.Format("%d/%d���ǂݍ���", n+1, p_du->PageNumber);//���܂ŉ����ǂݍ��񂾂������b�Z�|�W�Ƃ��ĕ\��
				}
				p_du->DispPage = header->DispPage;
				p_du->DispChannel = header->DispChannel;
			}
			break;

		case RGB_FORMAT://RGB
			file.Read( p_du->pByteData ,  p_du->DataSize.cx * p_du->DataSize.cy * 3);
			p_du->DispRGB = header->DispRGB;
			break;

		case SHORT16_FORMAT://Short
			file.Read( p_du->pShortData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(short) );
			break;

		case WORD_FORMAT://Word
			file.Read( p_du->pWordData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(WORD) );
			break;

		case LONG32_FORMAT://Long
			file.Read( p_du->pLongData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(long) );
			break;

		case DWORD_FORMAT://Dword
			file.Read( p_du->pDwordData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(DWORD) );
			break;

		case RGB_3D_FORMAT://RGB3D
			{
				for(n=0 ; n<p_du->PageNumber ; n++){
					for(int c=0 ; c<p_du->ChannelNumber ; c++){
						int offset = n*p_du->ChannelNumber + c;
						file.Read( p_du->pByteData + p_du->DataSize.cx * p_du->DataSize.cy * offset ,  p_du->DataSize.cx * p_du->DataSize.cy );
					}
					str.Format("%d/%d�ǂݍ���", n+1, p_du->PageNumber);//���܂ŉ����ǂݍ��񂾂������b�Z�|�W�Ƃ��ĕ\��
				}
				p_du->DispPage = header->DispPage;
				p_du->DispChannel = header->DispChannel;
			}
			break;
	}


	//�}�[�N��ǂݍ���
	if(header->MarkNum > MARK_NUMBER_MAX_OLD )//�V�����t�H�[�}�b�g�F�}�[�N�͉ϒ��f�[�^���摜�̂��Ƃɓ����Ă���
	{
		long markNum;
		file.Read( &markNum, sizeof(long));
		for(int i=0 ; i<markNum ; i++)
		{
			CMark mark;
			file.Read( &mark.type, sizeof(short));
			file.Read( &mark.figure, sizeof(FIGURE_UNION));
			p_du->AddMark(mark);
		}
	}
	else//�Â��t�H�[�}�b�g�F�}�[�N�͓_�݂̂̌Œ蒷�f�[�^���w�b�_�ɓ����Ă���
	{
		for(int i=0 ; i<header->MarkNum ; i++){
			p_du->AddMark( CMark(header->MarkX[i], header->MarkY[i]));
		}
	}


	////�\���ݒ�
	p_du->DataName = CString(header->DataName);

	p_du->BrightMax = header->BrightMax;
	p_du->BrightMin = header->BrightMin;

	p_du->DispScale = header->DispScale;
	p_du->DataDispOffset = CPoint(header->DataDispOffsetX, header->DataDispOffsetY);

	memcpy(p_du->WorkArea.Type, header->WorkAreaType, sizeof(int)*WORKAREA_FIG_NUM);
	memcpy(p_du->WorkArea.OrNot, header->WorkAreaOrNot, sizeof(bool)*WORKAREA_FIG_NUM);
	memcpy(p_du->WorkArea.Figure, header->WorkAreaFig, sizeof(FIGURE_UNION)*WORKAREA_FIG_NUM);

	p_du->DoUseWorkArea = header->DoUseWorkArea;

	p_du->DoUseMask = header->DoUseMask;



	//�ǂݍ��񂾃t�@�C���̃p�X���c��
	p_du->SrcFilePath = pathname;

	file.Close();

	
	SetFileIOLog(pathname);//�t�@�C�����̃��O���c��


	return		true;
}


/********************************************************************
�@  �\  ��  �� :  �摜�������̃A�[�J�C�u�f�[�^���t�@�C���ɕۑ�����
��    ��    �� : SaveArchivedImage
��          �� : long		image_number(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
��    ��    �l : ��������� true
�@          �\ : �ۑ�����t�@�C����I�Ԃ��߂̃t�@�C���_�C�A���O���o��
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
********************************************************************/
bool	CPimpomAPI::SaveArchivedImage(long image_number )
{
	CDataUnit	*p_du;
	CString filename, filestr;
	CFile  file;


		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	p_du = GetDataUnit( image_number );//���Ă����ꂽ�摜���������擾����
		if( p_du == NULL )		return	false;//�w�肳�ꂽ�摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�


	//�t�@�C���_�C�A���O
	filestr.LoadString(IDS_FILETYPE_ARCHIVED);
	CFileDialog myDLG(false , _T("pim") , NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filestr);

		if(myDLG.DoModal() != IDOK)  return false;
	filename = myDLG.GetPathName();

	return	SaveArchivedImage( image_number , filename);
}


/********************************************************************
�@  �\  ��  �� :  �摜�������̃A�[�J�C�u�f�[�^���t�@�C���ɕۑ�����
��    ��    �� : SaveArchivedImage
��          �� : long		image_number(in)�f�[�^�i�[��̉摜�������ԍ�(-1�ɂ����ꍇ�͌��ݑI�𒆂̉摜)
				CString		filename	(in)�t�@�C���̃t���p�X
��    ��    �l : ��������� true
�@          �\ : 
���t         �쐬��          ���e
------------ --------------- --------------------------------------- 
             Y.Ikeda         �V�K�쐬
 2016.0512   Y.Ikeda         �}�[�N�̈�������ύX
********************************************************************/
bool	CPimpomAPI::SaveArchivedImage(long image_number , CString filename)
{
	CFile		file;
	CDataUnit	*p_du;
	CString		str;

		if(image_number == CURRENT_IMAGE)		image_number = GetCurrentImageNumber();//���ݑI�𒆂̉摜���w�肷��ꍇ

	p_du = GetDataUnit( image_number );//���Ă����ꂽ�摜���������擾����
		if( p_du == NULL )		return	false;//�w�肳�ꂽ�摜�������Ƀf�[�^��������΂Ȃɂ����Ȃ�

		if( ! file.Open(filename , CFile::modeWrite | CFile::modeCreate | CFile::typeBinary ) )	return false; //�t�@�C�����J��

//�w�b�_�̕ۑ�
	DATA_UNIT_INFO	header;
	
	header.DataType = p_du->DataType;
	sprintf(header.DataName, "%s", p_du->DataName.GetBuffer());	p_du->DataName.ReleaseBuffer();
	header.DataSizeX = p_du->DataSize.cx;
	header.DataSizeY = p_du->DataSize.cy;
	header.PageNumber = p_du->PageNumber;
	header.ChannelNumber = p_du->ChannelNumber;
	header.DispPage = p_du->DispPage;
	header.DispChannel = p_du->DispChannel;
	header.DispRGB = p_du->DispRGB;
	header.MarkNum = MARK_NUMBER_MAX;//�Â��}�[�N���͗̈�ɂ́A�傫���l�����邱�ƂŐV�t�H�[�}�b�g�Ƌ�ʂ���
	//for(int i=0 ; i< p_du->GetMarkNumber() ; i++)
	//{
	//	CMark mark;
	//	p_du->GetMark(i,&mark);
	//	header.MarkX[i] = mark.figure.point.x;
	//	header.MarkY[i] = mark.figure.point.y;
	//}
	header.BrightMax = p_du->BrightMax;
	header.BrightMin = p_du->BrightMin;
	header.DispScale = p_du->DispScale;
	header.DataDispOffsetX = p_du->DataDispOffset.x;
	header.DataDispOffsetY = p_du->DataDispOffset.y;
	header.DoUseWorkArea = p_du->DoUseWorkArea;


	memcpy(header.WorkAreaType, p_du->WorkArea.Type, sizeof(int)*WORKAREA_FIG_NUM);
	memcpy(header.WorkAreaOrNot, p_du->WorkArea.OrNot, sizeof(bool)*WORKAREA_FIG_NUM);
	memcpy(header.WorkAreaFig, p_du->WorkArea.Figure, sizeof(FIGURE_UNION)*WORKAREA_FIG_NUM);

	file.Write( &header , sizeof(DATA_UNIT_INFO));//�w�b�_�̕ۑ�

////�\���摜�̕ۑ�
	if(p_du->DataType != RGB_FORMAT && p_du->DataType != RGB_3D_FORMAT){//�O���[
		file.Write( p_du->pDispBuffer , p_du->DispBufferRow * p_du->DataSize.cy );
		//file.Write( p_du->pDispBufferBmpInfo->bmiColors, 1024 );
	}else{//RGB
		file.Write( p_du->pDispBufferRGB , p_du->DispBufferRowRGB * p_du->DataSize.cy );
	}

////�}�X�N�摜�̕ۑ�
	file.Write( p_du->pMaskData , p_du->DataSize.cx * p_du->DataSize.cy );
	header.DoUseMask = p_du->DoUseMask;


/////���f�[�^�̕ۑ�
	switch( p_du->DataType ){
		case BYTE_FORMAT:
			//not break
		case THREE_D_FORMAT:
			{
				for(int n=0 ; n<p_du->PageNumber ; n++){
					BYTE	*addr = p_du->pByteData + p_du->DataSize.cx * p_du->DataSize.cy * n;
					file.Write( addr , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(BYTE) );
					str.Format("%d/%d���ǂݍ���", n+1, p_du->PageNumber);//���܂ŉ����ۑ������������b�Z�[�W�Ƃ��ĕ\������
					ShowMessage( true, 0, false, str );
				}
			}
			break;

		case FLOAT_FORMAT:
			//not break
		case COMPLEX_FORMAT:
			//not break
		case XY_FORMAT:
			//not break
		case XYZ_FORMAT:
			//not break
		case FLOAT_3D_FORMAT:
			{
				for(int n=0 ; n<p_du->PageNumber ; n++){
					float	*addr = p_du->pFloatData + p_du->DataSize.cx * p_du->DataSize.cy * n;
					file.Write( addr , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(float) );
					str.Format("%d/%d���ǂݍ���", n+1, p_du->PageNumber);//���܂ŉ����ۑ������������b�Z�[�W�Ƃ��ĕ\������
					ShowMessage( true, 0, false, str );
				}
			}
			break;

		case RGB_FORMAT:
			//not break
		case RGB_3D_FORMAT:
			{
				for(int n=0 ; n<p_du->PageNumber ; n++){
					BYTE	*addr = p_du->pByteData + p_du->DataSize.cx * p_du->DataSize.cy * p_du->ChannelNumber * n;
					file.Write( addr , p_du->DataSize.cx * p_du->DataSize.cy * p_du->ChannelNumber * sizeof(BYTE) );
					str.Format("%d/%d���ǂݍ���", n+1, p_du->PageNumber);//���܂ŉ����ۑ������������b�Z�[�W�Ƃ��ĕ\������
					ShowMessage( true, 0, false, str );
				}
			}
			break;

		case SHORT16_FORMAT:
			file.Write( p_du->pShortData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(short) );
			break;

		case WORD_FORMAT:
			file.Write( p_du->pWordData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(WORD) );
			break;

		case LONG32_FORMAT:
			file.Write( p_du->pLongData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(long) );
			break;

		case DWORD_FORMAT:
			file.Write( p_du->pDwordData , p_du->DataSize.cx * p_du->DataSize.cy * sizeof(DWORD) );
			break;
	}
	
	//�}�[�N�̕ۑ�:�ϒ��f�[�^���摜�̂��Ƃɓ����
	long markNum = p_du->GetMarkNumber();
	file.Write( &markNum, sizeof(long) );
	
	for(int i=0 ; i<markNum ; i++){
		CMark mark;
		p_du->GetMark(i,&mark);

		file.Write( &mark.type, sizeof(short));
		file.Write( &mark.figure, sizeof(FIGURE_UNION));
	}


	file.Close();

	SetFileIOLog(filename);//�t�@�C�����̃��O���c��

	return	true;
}


