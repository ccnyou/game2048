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
    if (pGame == NULL)
    {
        _FC_LOG("Invalid game");
        return FALSE;
    }

    m_pGame = pGame;

    _InitConsole();
    _InitAppearance();
    _Welcome();
    _DrawGame();

    return TRUE;
}

BOOL CConsoleWnd::Uninit()
{
    if (m_fpInput)
    {
        fclose(m_fpInput);
    }

    if (m_fpOutput)
    {
        fclose(m_fpInput);
    }

    BOOL bRet = FreeConsole();

    return bRet;
}

BOOL CConsoleWnd::Run()
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
    ASSERT(m_pGame != NULL);

    if (m_pGame->IsWin())
    {
        printf("You did it! \n");
        system("pause");
        return FALSE;
    }
    else if (m_pGame->IsFail())
    {
        printf("跪了! \n");
        system("pause");
        return FALSE;
    }

    return TRUE;
}


void CConsoleWnd::_Welcome() const
{
    printf("操作说明: w s a d 移动，q 返回窗口模式\n");
}

void CConsoleWnd::_HandleInput( int nInput ) const
{
    ASSERT(m_pGame != NULL);

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

}

BOOL CConsoleWnd::_IsQuitCommand( int nInput ) const
{
    if (nInput == 'Q')
    {
        return TRUE;
    }

    return FALSE;
}

void CConsoleWnd::_InitAppearance() const
{
    ASSERT(m_pGame != NULL);

    system("color 1A");
}


void CConsoleWnd::_DrawGame() const
{
    ASSERT(m_pGame != NULL);

    for (int i = 0; i < m_pGame->GetRowCount(); i++)
    {
        for (int j = 0; j < m_pGame->GetColumnCount(); j++)
        {
            printf("%5d", m_pGame->GetAt(i, j));
        }
        printf("\n");
    }
}

void CConsoleWnd::_ClearScreen() const
{
    system("cls");
}


BOOL CConsoleWnd::_InitConsole()
{
    BOOL bRet = AllocConsole();
    if (!bRet)
    {
        _FC_LOG("AllocConsole failed");
        return bRet;
    }

    freopen_s(&m_fpOutput, "CONOUT$", "w+t", stdout);
    freopen_s(&m_fpInput, "CONIN$", "r+t", stdin);

    return bRet;
}

int CConsoleWnd::_Getch()
{
    int nInput = EOF;
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwErr = 0;
    SetLastError(dwErr);

    INPUT_RECORD inputRecord = {0};
    DWORD dwRead;
    BOOL bRet = FALSE;
    do 
    {
        bRet = ReadConsoleInput(hStdin, &inputRecord, 1L, &dwRead);
        if (!bRet)
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
