#include "StdAfx.h"
#include "ConsoleWnd.h"
#include "game2048core.h"
#include <conio.h>
#include <string>
#include <cstdio>

CConsoleWnd::CConsoleWnd(void) 
: m_fpInput(NULL)
, m_fpOutput(NULL)
{
}

CConsoleWnd::~CConsoleWnd(void)
{
}

BOOL CConsoleWnd::Init( CGame2048Core* pGame )
{
    BOOL bRet = FALSE;

    m_pGame = pGame;

    if (!_InitConsole())
    {
        goto Exit0;
    }

    _InitAppearance();
    _Welcome();
    _DrawGame();

    bRet = TRUE;
Exit0:

    return bRet;
}

BOOL CConsoleWnd::Uninit()
{
    BOOL bRet = FALSE;

    if (m_fpInput)
    {
        fclose(m_fpInput);
    }

    if (m_fpOutput)
    {
        fclose(m_fpInput);
    }

    bRet = FreeConsole();

    return bRet;
}

int CConsoleWnd::Run()
{
    do 
    {
        int nInput = _Getch();

        nInput = toupper(nInput);
        if (_IsQuitCommand(nInput))
        {
            break;
        }

        _HandleInput(nInput);
        _ClearScreen();
        _Welcome();
        _DrawGame();

        if (!_JudgeGameResult())
        {
            break;
        }

    } while (TRUE);

    return 0;
}


BOOL CConsoleWnd::_JudgeGameResult() const
{
    BOOL bRet = TRUE;

    if (m_pGame == NULL)
    {
        bRet = FALSE;
        goto Exit0;
    }

    if (m_pGame->IsWin())
    {
        printf("You did it! \n");
        system("pause");
        bRet = FALSE;
    }
    else if (m_pGame->IsFail())
    {
        printf("跪了! \n");
        system("pause");
        bRet = FALSE;
    }

Exit0:

    return bRet;
}


void CConsoleWnd::_Welcome() const
{
    printf("操作说明: w s a d 移动，q 返回窗口模式\n");
}

void CConsoleWnd::_HandleInput( int nInput ) const
{
    if (m_pGame == NULL)
    {
        goto Exit0;
    }

    switch(nInput)
    {
    case 'W':
        m_pGame->SlideUp();
        break;
    case 'S':
        m_pGame->SlideDown();
        break;
    case 'A':
        m_pGame->SlideLeft();
        break;
    case 'D':
        m_pGame->SlideRight();
        break;
    default:
        break;
    }

Exit0:

    return;
}

BOOL CConsoleWnd::_IsQuitCommand( int nInput ) const
{
    BOOL bRet = FALSE;

    if (nInput == 'Q')
    {
        bRet = TRUE;
    }

    return bRet;
}

void CConsoleWnd::_InitAppearance() const
{
    system("color 1A");
}


void CConsoleWnd::_DrawGame() const
{
    if (m_pGame != NULL)
    {
        for (int i = 0; i < m_pGame->GetRowCount(); i++)
        {
            for (int j = 0; j < m_pGame->GetColumnCount(); j++)
            {
                printf("%5d", m_pGame->GetAt(i, j));
            }
            printf("\n");
        }
    }
}

void CConsoleWnd::_ClearScreen() const
{
    system("cls");
}

BOOL CConsoleWnd::_InitConsole()
{
    BOOL bRet = FALSE;
        
    bRet = ::AllocConsole();
    if (!bRet)
    {
        _FC_LOG("AllocConsole failed");
        goto Exit0;
    }

    freopen_s(&m_fpOutput, "CONOUT$", "w+t", stdout);
    freopen_s(&m_fpInput, "CONIN$", "r+t", stdin);

    if (m_fpInput == NULL || m_fpOutput == NULL)
    {
        bRet = FALSE;
        goto Exit0;
    }

    bRet = TRUE;

Exit0:

    return bRet;
}

int CConsoleWnd::_Getch()
{
    int nInput = EOF;
    HANDLE hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwErr = 0;
    INPUT_RECORD inputRecord = {0};
    DWORD dwRead = 0;
    BOOL bStatusCode = FALSE;

    ::SetLastError(dwErr);

    do 
    {
        bStatusCode = ::ReadConsoleInput(hStdin, &inputRecord, 1L, &dwRead);
        if (!bStatusCode)
        {
            _FC_LOG("ReadConsoleInput failed!");
            break;
        }

        if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown)
        {
            nInput = inputRecord.Event.KeyEvent.uChar.AsciiChar;
            break;
        }

    } while (TRUE);

    return nInput;
}
