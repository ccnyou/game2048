#pragma once

class CGame2048Core;

const int CONSOLE_CELL_SPACING = 1;
const int CONSOLE_CELL_WIDTH = 5;
const int CONSOLE_CELL_HEIGHT = 5;

class CConsoleWnd
{

public:
    CConsoleWnd(void);
    ~CConsoleWnd(void);

    BOOL	Init(CGame2048Core*	pGame);
    BOOL	Uninit();
    int     Run();

private:
    BOOL	_InitConsole();
    void	_InitAppearance() const;
    void	_Welcome() const;
    void	_ClearScreen() const;
    void	_DrawGame() const;
    void	_HandleInput(int nInput) const;
    BOOL	_JudgeGameResult() const;
    BOOL	_IsQuitCommand(int nInput) const;
    int		_Getch();

private:
    CGame2048Core*	m_pGame;
    FILE*		m_fpInput;
    FILE*		m_fpOutput;
};
