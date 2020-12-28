// dllmain.cpp : DLL の初期化ルーチンを定義します。
//

#include "stdafx.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE PIMPOM_CoreDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved を使う場合はここを削除してください
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("PIMPOM_Core.DLL Initializing!\n");
		
		// 拡張 DLL を 1 回だけ初期化します。
		if (!AfxInitExtensionModule(PIMPOM_CoreDLL, hInstance))
			return 0;

		// この DLL をリソース チェーンへ挿入します。
		// メモ : この拡張 DLL が暗黙的に、MFC アプリケーションではなく
		//  ActiveX コントロールなどの MFC 標準 DLL によってリンクされている場合、
		//  以下の行を DllMain から削除して
		//  から削除して、この拡張 DLL からエクスポート
		//  配置してください。したがって、この拡張 DLL を使う標準 DLL は、
		//  その関数を明示的に呼び出して、
		//  を初期化するために明示的にその関数を呼び出します。
		//  それ以外の場合は、CDynLinkLibrary オブジェクトは
		//  標準 DLL のリソース チェーンへアタッチされず、
		//  その結果重大な問題となります。

		new CDynLinkLibrary(PIMPOM_CoreDLL);

		// ソケットの初期化
		// メモ : この拡張 DLL が暗黙的に、MFC アプリケーションではなく
		//  ActiveX コントロールなどの MFC 標準 DLL によってリンクされている場合、
		//  以下の行を DllMain から削除して
		//  この拡張 DLL からエクスポートされた別の関数に
		//  配置してください。したがって、この拡張 DLL を使う標準 DLL は、
		//  その関数を明示的に呼び出して、
		//  この拡張 DLL. を初期化しなければなりません。
		if (!AfxSocketInit())
		{
			return FALSE;
		}
	
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("PIMPOM_Core.DLL Terminating!\n");

		// デストラクタが呼び出される前にライブラリを終了します
		AfxTermExtensionModule(PIMPOM_CoreDLL);
	}
	return 1;   // OK
}

