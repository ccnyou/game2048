// game2048.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cgame2048App:
// �йش����ʵ�֣������ game2048.cpp
//

class CGame2048App : public CWinApp
{
public:
    CGame2048App();

    // ��д
public:
    virtual BOOL InitInstance();

    // ʵ��

    DECLARE_MESSAGE_MAP()

    virtual int ExitInstance();

private:
    BOOL	_InitGdiplus();
    BOOL	_UninitGdiplus();

private:
    ULONG_PTR m_gdiplusToken;
};

extern CGame2048App theApp;