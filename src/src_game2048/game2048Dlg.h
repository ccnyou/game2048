// game2048Dlg.h : 头文件
//

#pragma once


class CGame2048Core;
class CConsoleWnd;


enum GAME_MODE;

const int UM_GAME_WIN					= WM_USER + 1;
const int UM_GAME_FAIL					= UM_GAME_WIN + 1;
const int DEFAULT_GAME_AREA_WIDTH		= 334;
const int DEFAULT_GAME_AREA_HEIGHT		= 334;
const int DEFAULT_WIN_PAD				= 1; 
const int DEFAULT_WND_WIDTH				= DEFAULT_GAME_AREA_WIDTH + 2 * DEFAULT_WIN_PAD;
const int DEFAULT_TEXT_AREA_HEIGHT		= 66;
const int DEFAULT_WND_HEIGHT			= DEFAULT_GAME_AREA_HEIGHT + DEFAULT_TEXT_AREA_HEIGHT;
const int MAX_GAME_PNG_NUMBER			= 2048;
const int DEFAULT_CELL_SPACING			= 10;
const int DEFAULT_CELL_WIDTH			= 71;
const int DEFAULT_CELL_HEIGHT			= 71;

// Cgame2048Dlg 对话框
class CGame2048Dlg : public CDialog
{
    typedef void (CGame2048Core::*PACTION)(void);
    // 构造
public:
    CGame2048Dlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CGame2048Dlg();

    // 对话框数据
    enum { IDD = IDD_GAME2048_DIALOG };

    typedef boost::shared_ptr<Gdiplus::Image> ImagePtr;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

    // 实现
protected:
    // 生成的消息映射函数
    virtual BOOL		OnInitDialog();
    afx_msg BOOL		OnEraseBkgnd(CDC* pDC);
    afx_msg void		OnSysCommand(UINT uID, LPARAM lParam);
    afx_msg void		OnKeyUp(UINT uChar, UINT uRepCnt, UINT uFlags);
    afx_msg void		OnPaint();
    afx_msg void		OnDestroy();
    afx_msg void		OnNcRButtonUp(UINT nHitTest, CPoint point);
    afx_msg void		OnContextMenuSetting();
    afx_msg void		OnContextMenuClose();
    afx_msg void		OnContextMenuRestart();
    afx_msg void		OnContextMenuSwitchWnd();
    afx_msg HCURSOR		OnQueryDragIcon();
    afx_msg LRESULT		OnNcHitTest(CPoint point);
    afx_msg LRESULT		OnGameWin(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT		OnGameFail(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()

private:
    void			_PaintClient(CDC* dc);
    void			_GetGameRect(CRect* pRect) const;
    void			_InitGame(GAME_MODE gameMode);
    void			_UninitGame();
    void			_InitWindow();
    void			_InitActionMap();
    void			_ResizeWindow();
    void			_SetGameMode(GAME_MODE gameMode);
    void			_JudgeGameResult();

    void			_DrawGame( Gdiplus::Graphics* pGraphics );
    void			_DrawGameBackground( Gdiplus::Graphics* pGraphics );
    void			_DrawGameCells( Gdiplus::Graphics* pGraphics );
    void			_DrawScore( Gdiplus::Graphics* pGraphics );
    void			_DrawFrame( Gdiplus::Graphics* graphics, const CRect& clientRect );

    ImagePtr		_LoadNumberImage(int n);
    ImagePtr		_LoadImageFromResource(LPCTSTR lpName, LPCTSTR lpType, HMODULE hInst = NULL);

private:
    HICON					m_hIcon;
    CGame2048Core*				m_pGame;
    ImagePtr				m_bgImage;
    std::vector<ImagePtr>	m_imagesCache;
    CConsoleWnd*			m_pConsole;
    std::map<UINT, PACTION>	m_actionMap;
};
