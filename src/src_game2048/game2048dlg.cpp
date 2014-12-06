// game2048Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "game2048app.h"
#include "game2048Dlg.h"
#include "resource.h"
#include "game2048core.h"
#include "GameSettingDlg.h"
#include "ConsoleWnd.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum UM
{
    UM_GAME_WIN = WM_USER + 1,
    UM_GAME_FAIL
};

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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cgame2048Dlg 对话框

CGame2048Dlg::CGame2048Dlg(CWnd* pParent /*=NULL*/)
: CDialog(CGame2048Dlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pGame = NULL;
    m_pConsole = NULL;
}

CGame2048Dlg::~CGame2048Dlg()
{
}


void CGame2048Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGame2048Dlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
    ON_WM_NCHITTEST()
    ON_WM_KEYUP()
    ON_MESSAGE(UM_GAME_WIN, OnGameWin)
    ON_MESSAGE(UM_GAME_FAIL, OnGameFail)
    ON_WM_DESTROY()
    //	ON_WM_CONTEXTMENU()
    //	ON_WM_RBUTTONUP()
    ON_WM_NCRBUTTONUP()
    ON_COMMAND(ID_POPUP_32771, &CGame2048Dlg::OnContextMenuSetting)
    ON_COMMAND(ID_POPUP_32773, &CGame2048Dlg::OnContextMenuClose)
    ON_COMMAND(ID_POPUP_32772, &CGame2048Dlg::OnContextMenuRestart)
    ON_COMMAND(ID_POPUP_32775, &CGame2048Dlg::OnContextMenuSwitchWnd)
END_MESSAGE_MAP()


// Cgame2048Dlg 消息处理程序

BOOL CGame2048Dlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    _InitGame(GAME_MODE_4X4);
    _InitWindow();
    _InitActionMap();

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CGame2048Dlg::_InitGame(GAME_MODE gameMode)
{
    // 防止重复初始化
    if (m_pGame != NULL)
    {
        goto Exit0;
    }

    m_pGame = new CGame2048Core;
    if (m_pGame == NULL)
    {
        _FC_LOG("new game failed!");
        goto Exit0;
    }

    if (m_pGame->Init(gameMode))
    {
        m_pGame->Start();
    }

Exit0:
    return;
}

void CGame2048Dlg::_UninitGame()
{
    if (m_pGame)
    {
        m_pGame->Uninit();

        delete m_pGame;
        m_pGame = NULL;
    }
}


void CGame2048Dlg::_InitWindow()
{
    ::SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU);
    ::SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_LTRREADING);
    _ResizeWindow();
}

void CGame2048Dlg::_ResizeWindow()
{
    CRect gameRect;

    int nWndWidth = 0;
    int nWndHeight = 0;

    _GetGameRect(&gameRect);

    nWndWidth = gameRect.Width() + 2 * DEFAULT_WIN_PAD;
    nWndHeight = gameRect.Height() + DEFAULT_TEXT_AREA_HEIGHT + 2 * DEFAULT_WIN_PAD;

    SetWindowPos(NULL, -1, -1, nWndWidth, nWndHeight, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
    CenterWindow();
}

void CGame2048Dlg::OnDestroy()
{
    CDialog::OnDestroy();

    _UninitGame();
}


void CGame2048Dlg::OnSysCommand(UINT uID, LPARAM lParam)
{
    if ((uID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(uID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGame2048Dlg::OnPaint()
{

    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        _FC_LOG("OnPaint begin");

        if (m_pGame)
        {
            CPaintDC dc(this);
            _PaintClient(&dc);
            _JudgeGameResult();
        }

        _FC_LOG("OnPaint end");
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CGame2048Dlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


BOOL CGame2048Dlg::OnEraseBkgnd(CDC*)
{
    return TRUE;
}

#pragma region 绘制

void CGame2048Dlg::_PaintClient( CDC* pDC )
{
    ASSERT(m_pGame != NULL && pDC != NULL);

    CDC memDC;
    CRect clientRect;
    CWnd* pDcWindow = NULL;
    HBITMAP hMembmp = NULL;
    HBITMAP hOldbmp = NULL;
    Gdiplus::Graphics* pGraphics = NULL;

    if (pDC == NULL || m_pGame == NULL)
    {
        goto Exit0;
    }

    memDC.CreateCompatibleDC(pDC);

    pDcWindow = pDC->GetWindow();
    if (!pDcWindow)
    {
        goto Exit0;
    }

    pDcWindow->GetClientRect(&clientRect);

    hMembmp = CreateCompatibleBitmap(pDC->GetSafeHdc(), clientRect.Width(), clientRect.Height());
    hOldbmp = (HBITMAP)SelectObject(memDC.GetSafeHdc(), hMembmp);

    pGraphics = Gdiplus::Graphics::FromHDC(memDC.GetSafeHdc());
    if (!pGraphics)
    {
        goto Exit0;
    }

    pGraphics->Clear(Gdiplus::Color(255, 255, 255));

    _FC_LOG("_DrawGame begin");
    _DrawGame(pGraphics);
    _FC_LOG("_DrawGame end");

    _DrawScore(pGraphics);
    _DrawFrame(pGraphics, clientRect);


    ::BitBlt(pDC->GetSafeHdc(), 0, 0, clientRect.Width(), clientRect.Height(), memDC.GetSafeHdc(), 0, 0, SRCCOPY);

Exit0:

    if (pGraphics)
    {
        delete pGraphics;
    }

    if (hOldbmp)
    {
        SelectObject(memDC.GetSafeHdc(), hOldbmp);
    }

    if (hMembmp)
    {
        DeleteObject(hMembmp);
    }

    return;
}

void CGame2048Dlg::_DrawScore( Gdiplus::Graphics* pGraphics )
{
    ASSERT(pGraphics != NULL && m_pGame != NULL);

    CString strScoreText;
    Gdiplus::FontFamily fontFamily(_T("宋体"));
    Gdiplus::Font aFont(&fontFamily, 20, Gdiplus::FontStyleBold, Gdiplus::UnitPoint);
    Gdiplus::RectF boundRect;
    Gdiplus::PointF origin;
    Gdiplus::Color blueColor = static_cast<Gdiplus::ARGB>(Gdiplus::Color::Blue);
    Gdiplus::SolidBrush textBrush(blueColor);
    CRect wndRect, gameRect;
    float fScoreAreaX = 0;
    float fScroeAreaY = 0;
    int nWndWidth = 0;
    int nWndHeight = 0;
    int nGameHeight = 0;

    if (pGraphics == NULL)
    {
        goto Exit0;
    }

    GetClientRect(&wndRect);
    _GetGameRect(&gameRect);

    nWndWidth = wndRect.Width();
    nWndHeight = wndRect.Height();
    nGameHeight = gameRect.Height();

    strScoreText.Format( _T("当前分数：%d"), m_pGame->GetScore());
    pGraphics->MeasureString(strScoreText, strScoreText.GetLength(), &aFont, Gdiplus::PointF(0.0f, 0.0f), &boundRect);
    /// 让分数文字居中的起点偏移 x
    fScoreAreaX = (nWndWidth - boundRect.Width) / 2;
    /// 让分数文字居中的起点偏移 y = 游戏区域高度 + 相对空白区域的居中偏移
    fScroeAreaY = nGameHeight + (nWndHeight - nGameHeight - boundRect.Height) / 2;
    origin = Gdiplus::PointF(fScoreAreaX, fScroeAreaY);
    pGraphics->DrawString(strScoreText, strScoreText.GetLength(), &aFont, origin, &textBrush);

Exit0:

    return;
}

void CGame2048Dlg::_DrawFrame( Gdiplus::Graphics* pGraphics, const CRect& clientRect )
{
    ASSERT(pGraphics != NULL);

    Gdiplus::Pen blackPen(Gdiplus::Color(254, 0, 0, 0), static_cast<Gdiplus::REAL>(DEFAULT_WIN_PAD));
    Gdiplus::Pen grayPen(Gdiplus::Color(254, 200, 200, 200), static_cast<Gdiplus::REAL>(DEFAULT_WIN_PAD));
    CRect deflateRect = clientRect;

    if (pGraphics == NULL)
    {
        goto Exit0;
    }

    deflateRect.DeflateRect(0, 0, DEFAULT_WIN_PAD, DEFAULT_WIN_PAD);
    pGraphics->DrawRectangle(&blackPen, deflateRect.left, deflateRect.top, deflateRect.Width(), deflateRect.Height());
    deflateRect.DeflateRect(DEFAULT_WIN_PAD, DEFAULT_WIN_PAD, DEFAULT_WIN_PAD, DEFAULT_WIN_PAD);
    pGraphics->DrawRectangle(&grayPen, deflateRect.left, deflateRect.top, deflateRect.Width(), deflateRect.Height());

Exit0:
    return;
}

void CGame2048Dlg::_DrawGameBackground( Gdiplus::Graphics* pGraphics )
{
    ASSERT(pGraphics != NULL && m_pGame != NULL);

    CRect gameRect;
    Gdiplus::Rect tmpRect;

    if (pGraphics == NULL)
    {
        goto Exit0;
    }

    if (!m_bgImage)
    {
        m_bgImage = _LoadImageFromResource(MAKEINTRESOURCE(IDR_PNG_BG), _T("PNG"));
    }

    if (m_bgImage.get() == NULL)
    {
        _FC_LOG("load image from resource failed!");
        return;
    }

    
    _GetGameRect(&gameRect);

    tmpRect = Gdiplus::Rect(gameRect.left + DEFAULT_WIN_PAD, gameRect.top + DEFAULT_WIN_PAD, gameRect.Width(), gameRect.Height());
    pGraphics->DrawImage(m_bgImage.get(), tmpRect, 0, 0, m_bgImage->GetWidth(), m_bgImage->GetHeight(), Gdiplus::UnitPixel);

Exit0:
    return;
}

void CGame2048Dlg::_DrawGameCells( Gdiplus::Graphics* pGraphics )
{
    ASSERT(pGraphics != NULL && m_pGame != NULL);

    Gdiplus::Graphics& graphics = *pGraphics;

    ImagePtr image;
    for (int i = 0; i < m_pGame->GetRowCount(); ++i)
    {
        for (int j = 0; j < m_pGame->GetColumnCount(); ++j)
        {
            int nNumber = m_pGame->GetAt(i, j);
            if (nNumber == INVALID_GAME_NUMBER)
            {
                _FC_LOG("m_pGame->GetAt(%d, %d) failed!", i, j);
                i = m_pGame->GetRowCount();
                break;
            }

            image = _LoadNumberImage(nNumber);
            if (image.get() == NULL)
            {
                _FC_LOG("load number(%d) image from resource failed!", nNumber);
                i = m_pGame->GetRowCount();
                break;
            }

            int nOrgX = (j + 1) * DEFAULT_CELL_SPACING + j * DEFAULT_CELL_WIDTH + DEFAULT_WIN_PAD;
            int nOrgY = (i + 1) * DEFAULT_CELL_SPACING + i * DEFAULT_CELL_HEIGHT + DEFAULT_WIN_PAD;
            Gdiplus::Rect rect(nOrgX, nOrgY, image->GetWidth(), image->GetHeight());
            graphics.DrawImage(image.get(), rect, 0, 0, image->GetWidth(), image->GetHeight(), Gdiplus::UnitPixel);
        }
    }
}

void CGame2048Dlg::_DrawGame( Gdiplus::Graphics* pGraphics )
{
    _DrawGameBackground(pGraphics);
    _DrawGameCells(pGraphics);
}
#pragma endregion 


LRESULT CGame2048Dlg::OnNcHitTest(CPoint point)
{
    UNREFERENCED_PARAMETER(point);
    return HTCAPTION;
    //return CDialog::OnNcHitTest(point);
}


/**
* @brief 计算一个数是2的多少次方
*/
inline int GetLog2(int n)
{
    ASSERT(n != 0 && n % 2 == 0 && n <= MAX_GAME_PNG_NUMBER);
    int nRet = 0;
    do 
    {
        n /= 2;
        nRet ++;
    } while (n != 1);

    return nRet;
}

boost::shared_ptr<Gdiplus::Image> CGame2048Dlg::_LoadNumberImage(int n)
{
    ASSERT(n % 2 == 0 && n <= MAX_GAME_PNG_NUMBER);

    ImagePtr image;
    if (m_imagesCache.size() == 0)
    {
        int imageCount = GetLog2(MAX_GAME_PNG_NUMBER) + 1;
        m_imagesCache.insert(m_imagesCache.begin(), imageCount, image);
    }

    int nIndex = 0;
    if (n != 0)
    {
        nIndex = GetLog2(n);
    }

    int nResId = nIndex + IDR_PNG0;
    image = m_imagesCache[nIndex];
    if (!image)
    {
        image = _LoadImageFromResource(MAKEINTRESOURCE(nResId), _T("PNG"));
        m_imagesCache[nIndex] = image;
    }

    return image;
}

void CGame2048Dlg::OnKeyUp(UINT uChar, UINT uRepCnt, UINT uFlags)
{
    PACTION pAction = m_actionMap[uChar];
    if (pAction)
    {
        (m_pGame->*pAction)();
        Invalidate(FALSE);
    }

    CDialog::OnKeyUp(uChar, uRepCnt, uFlags);
}


void CGame2048Dlg::_InitActionMap()
{
    m_actionMap.clear();
    m_actionMap[VK_LEFT] = m_actionMap['A'] = &CGame2048Core::SlideLeft;
    m_actionMap[VK_UP] = m_actionMap['W'] = &CGame2048Core::SlideUp;
    m_actionMap[VK_DOWN] = m_actionMap['S'] = &CGame2048Core::SlideDown;
    m_actionMap[VK_RIGHT] = m_actionMap['D'] = &CGame2048Core::SlideRight;
}


LRESULT CGame2048Dlg::OnGameWin(WPARAM, LPARAM)
{
    MessageBox(_T("你赢了，点击确定结束游戏"), _T("Good Job!"));
    EndDialog(0);
    return 0;
}

LRESULT CGame2048Dlg::OnGameFail(WPARAM, LPARAM)
{
    MessageBox(_T("游戏已经失败了!，点击确定结束游戏"), _T("Pity"));
    EndDialog(0);
    return 0;
}


void CGame2048Dlg::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
    CMenu menu;
    menu.LoadMenu(IDR_MENU1);
    CMenu* pPopup = menu.GetSubMenu(0);
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
    CDialog::OnNcRButtonUp(nHitTest, point);
}

void CGame2048Dlg::OnContextMenuSetting()
{
    CGameSettingDlg dlg;
    dlg.SetGameMode(m_pGame->GetGameMode());
    int nRet = dlg.DoModal();
    if (nRet == IDOK)
    {
        _SetGameMode(dlg.GetGameMode());
    }
}

void CGame2048Dlg::OnContextMenuClose()
{
    int nChoice = 0;
    nChoice = MessageBox(_T("确认放弃游戏？"), _T("提示"), MB_YESNO);

    if (nChoice == IDYES)
    {
        EndDialog(0);
    }
}

void CGame2048Dlg::OnContextMenuRestart()
{
    if (!m_pGame)
    {
        return;
    }

    BOOL bRestart = TRUE;
    if (m_pGame->GetScore() != 0)
    {
        int nChoice = 0;
        nChoice = MessageBox(_T("确认放弃当前游戏并重新开始？"), _T("提示"), MB_YESNO);
        bRestart = nChoice == IDYES ? TRUE : FALSE;
    }

    if (bRestart)
    {
        GAME_MODE oldGameMode = m_pGame->GetGameMode();
        _UninitGame();
        _InitGame(oldGameMode);
        Invalidate(FALSE);
    }
}

void CGame2048Dlg::_SetGameMode( GAME_MODE gameMode )
{
    if (gameMode == m_pGame->GetGameMode())
    {
        return;
    }

    BOOL bRestart = TRUE;
    if (m_pGame->GetScore() != 0)
    {
        int nChoice = 0;
        nChoice = MessageBox(_T("确认放弃当前游戏并重新开始？"), _T("提示"), MB_YESNO);
        bRestart = nChoice == IDYES ? TRUE : FALSE;
    }

    if (bRestart)
    {
        _UninitGame();
        _InitGame(gameMode);
        _ResizeWindow();
        Invalidate(FALSE);
    }
}

void CGame2048Dlg::_GetGameRect( CRect* pRect ) const
{
    ASSERT(pRect != NULL && m_pGame != NULL);

    GAME_MODE gameMode = m_pGame->GetGameMode();
    ASSERT(gameMode == GAME_MODE_4X4 || gameMode == GAME_MODE_8x8);

    int nWidth = 0;
    int nHeight = 0;

    switch(gameMode)
    {
    case GAME_MODE_4X4:
        {
            nWidth = 5 * DEFAULT_CELL_SPACING + 4 * DEFAULT_CELL_WIDTH;
            nHeight = 5 * DEFAULT_CELL_SPACING + 4 * DEFAULT_CELL_HEIGHT;
        }
        break;
    case GAME_MODE_8x8:
        {
            nWidth = 9 * DEFAULT_CELL_SPACING + 8 * DEFAULT_CELL_WIDTH;
            nHeight = 9 * DEFAULT_CELL_SPACING + 8 * DEFAULT_CELL_HEIGHT;
        }
        break;

    default:
        _FC_LOG("Game Mode Error");
        break;
    }

    pRect->SetRect(0, 0, nWidth, nHeight);
}


void CGame2048Dlg::OnContextMenuSwitchWnd()
{
    if (m_pConsole != NULL)
    {
        return;
    }

    m_pConsole = new CConsoleWnd;
    if (m_pConsole == NULL)
    {
        _FC_LOG("new CConsoleWnd failed");
        return;
    }

    ShowWindow(SW_HIDE);

    BOOL bRet = m_pConsole->Init(m_pGame);
    if (bRet)
    {
        m_pConsole->Run();
        m_pConsole->Uninit();
    }

    delete m_pConsole;
    m_pConsole = NULL;

    ShowWindow(SW_NORMAL);
}

void CGame2048Dlg::_JudgeGameResult()
{
    ASSERT(m_pGame != NULL);

    if (m_pGame->IsWin())
    {
        PostMessage(UM_GAME_WIN, 0, 0);
    }
    else if (m_pGame->IsFail())
    {
        PostMessage(UM_GAME_FAIL, 0, 0);
    }
}

CGame2048Dlg::ImagePtr CGame2048Dlg::_LoadImageFromResource( LPCTSTR lpName, LPCTSTR lpType, HMODULE hInst /*= NULL*/ )
{
    if (hInst == NULL)
    {
        hInst = AfxGetApp()->m_hInstance;
    }

    Gdiplus::Image* image = NULL;
    do 
    {
        HRSRC hResInfo = FindResource(hInst, lpName, lpType);
        DWORD size = SizeofResource(hInst, hResInfo);
        HGLOBAL hResData = LoadResource(hInst, hResInfo);
        const void* data = LockResource(hResData);
        if (data == NULL)
        {
            break;
        }

        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
        if (hMem == NULL)
        {
            break;
        }

        void* buffer = GlobalLock(hMem);
        if (buffer == NULL)
        {
            GlobalFree(hMem);
            break;
        }

        CopyMemory(buffer, data, size);
        IStream* stream = NULL;
        HRESULT hResult = CreateStreamOnHGlobal(hMem, FALSE, &stream);
        if (hResult == S_OK)
        {
            image = Gdiplus::Image::FromStream(stream);
            stream->Release();
        }

        GlobalUnlock(hMem);
        GlobalFree(hMem);
    } while (FALSE);

    boost::shared_ptr<Gdiplus::Image> imagePtr(image);
    return imagePtr;
}
