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
    // 如果一个运行在 Windows XP 上的应用程序清单指定要
    // 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
    //则需要 InitCommonControlsEx()。否则，将无法创建窗口。
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // 将它设置为包括所有要在应用程序中使用的
    // 公共控件类。
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    // 标准初始化
    // 如果未使用这些功能并希望减小
    // 最终可执行文件的大小，则应移除下列
    // 不需要的特定初始化例程
    // 更改用于存储设置的注册表项
    // TODO: 应适当修改该字符串，
    // 例如修改为公司或组织名
    SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    BOOL bRet = FALSE;

    bRet = _InitGdiplus();
    if (!bRet)
    {
        AfxMessageBox(_T("初始化 Gdiplus 失败"));
        goto Exit0;
    }

    {
        CGame2048Dlg dlg;
        m_pMainWnd = &dlg;
        dlg.DoModal();
    }

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
