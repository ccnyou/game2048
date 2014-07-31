// game2048.cpp : ����Ӧ�ó��������Ϊ��
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


// Cgame2048App ����

CGame2048App::CGame2048App()
{
    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
    m_ulGdiplusToken = 0;
}


// Ψһ��һ�� Cgame2048App ����

CGame2048App theApp;


// Cgame2048App ��ʼ��

BOOL CGame2048App::InitInstance()
{
    // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
    // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
    //����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
    // �����ؼ��ࡣ
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

    AfxEnableControlContainer();

    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

    BOOL bRet = FALSE;

    bRet = _InitGdiplus();
    if (!bRet)
    {
        AfxMessageBox(_T("��ʼ�� Gdiplus ʧ��"));
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
