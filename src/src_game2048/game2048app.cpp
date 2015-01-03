// game2048.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "game2048app.h"
#include "game2048Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cgame2048App

BEGIN_MESSAGE_MAP(CGame2048App, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cgame2048App 构造

CGame2048App::CGame2048App()
{
    // TODO: 在此处添加构造代码，
    // 将所有重要的初始化放置在 InitInstance 中
    m_ulGdiplusToken = 0;
}


// 唯一的一个 Cgame2048App 对象

CGame2048App theApp;


// Cgame2048App 初始化

BOOL CGame2048App::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // 将它设置为包括所有要在应用程序中使用的
    // 公共控件类。
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    SetRegistryKey(_T("Game2048"));

    BOOL bRet = FALSE;
    CGame2048Dlg dlg;

    bRet = _InitGdiplus();
    if (!bRet)
    {
        AfxMessageBox(_T("初始化 Gdiplus 失败"));
        goto Exit0;
    }

    m_pMainWnd = &dlg;
    dlg.DoModal();

Exit0:

    return FALSE;
}

int CGame2048App::ExitInstance()
{
    _UninitGdiplus();
    return CWinApp::ExitInstance();
}

BOOL CGame2048App::_InitGdiplus()
{
    BOOL bRet = FALSE;
    Gdiplus::GdiplusStartupInput input = 0;
    Gdiplus::Status status = Gdiplus::Ok;
    ULONG_PTR ulToken = 0;

    status = Gdiplus::GdiplusStartup(&ulToken, &input, NULL);
    if (status != Gdiplus::Ok)
    {
        goto Exit0;
    }

    m_ulGdiplusToken = ulToken;
    bRet = TRUE;

Exit0:

    return bRet;
}

BOOL CGame2048App::_UninitGdiplus()
{
    Gdiplus::GdiplusShutdown(m_ulGdiplusToken);
    return TRUE;
}
