// game2048.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// Cgame2048App:
// 有关此类的实现，请参阅 game2048.cpp
//

class CGame2048App : public CWinApp
{
public:
    CGame2048App();

    // 重写
public:
    virtual BOOL InitInstance();

    // 实现

    DECLARE_MESSAGE_MAP()

    virtual int ExitInstance();

private:
    BOOL	_InitGdiplus();
    BOOL	_UninitGdiplus();

private:
    ULONG_PTR m_gdiplusToken;
};

extern CGame2048App theApp;