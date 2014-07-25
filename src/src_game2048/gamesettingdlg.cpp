// GameSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "game2048core.h"
#include "GameSettingDlg.h"


// CGameSettingDlg 对话框

IMPLEMENT_DYNAMIC(CGameSettingDlg, CDialog)

CGameSettingDlg::CGameSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGameSettingDlg::IDD, pParent)
, m_gameMode(GAME_MODE_INVALID)
, m_nRadioValue(0)
{
}

CGameSettingDlg::~CGameSettingDlg()
{
}

void CGameSettingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RADIO1, m_nRadioValue);
}


BEGIN_MESSAGE_MAP(CGameSettingDlg, CDialog)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDOK, &CGameSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()

GAME_MODE CGameSettingDlg::GetGameMode() const
{
    return m_gameMode;
}


// CGameSettingDlg 消息处理程序

void CGameSettingDlg::SetGameMode( GAME_MODE gameMode )
{
    m_gameMode = gameMode;
    m_nRadioValue = m_gameMode - GAME_MODE_4X4;
}

void CGameSettingDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: 在此处添加消息处理程序代码
}

void CGameSettingDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);
    m_gameMode = static_cast<GAME_MODE>(GAME_MODE_4X4 + m_nRadioValue);
    OnOK();
}

BOOL CGameSettingDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    return TRUE;
}
