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

#include "stdafx.h"
#include "PIMPOM_API.h"



/////////////////////////////////////////////////////////////////////////////
// CConfirmBox: CConfirmDlgダイアログのラッパー



CConfirmBox::CConfirmBox()
{
	the_confirm_dlg.InitSetting();
}

void CConfirmBox::SetWidth(int titlesize, int editsize)
{
	return the_confirm_dlg.SetWidth( titlesize,  editsize);
}

void CConfirmBox::SetTitle(CString title)
{
	return the_confirm_dlg.SetTitle(title);
}

bool CConfirmBox::RegistVar(CString title, bool *pval)
{
	return the_confirm_dlg.RegistVar(title, pval);
}

bool CConfirmBox::RegistVar(CString title, CString *pval)
{
	return the_confirm_dlg.RegistVar(title, pval);
}

bool CConfirmBox::RegistVar(CString title, double *pval)
{
	return the_confirm_dlg.RegistVar(title, pval);
}

bool CConfirmBox::RegistVar(CString title, float *pval)
{
	return the_confirm_dlg.RegistVar(title, pval);
}

bool CConfirmBox::RegistVar(CString title, short *pval)
{
	return the_confirm_dlg.RegistVar(title, pval);
}

bool CConfirmBox::RegistVar(CString title, int *pval)
{
	return the_confirm_dlg.RegistVar(title, pval);
}

int  CConfirmBox::DoModal()
{
	return the_confirm_dlg.DoModal();
}