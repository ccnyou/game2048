// GameSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "game2048core.h"
#include "GameSettingDlg.h"


// CGameSettingDlg �Ի���

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


// CGameSettingDlg ��Ϣ�������

void CGameSettingDlg::SetGameMode( GAME_MODE gameMode )
{
	m_gameMode = gameMode;
	m_nRadioValue = m_gameMode - GAME_MODE_4X4;
}

void CGameSettingDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}

void CGameSettingDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_gameMode = static_cast<GAME_MODE>(GAME_MODE_4X4 + m_nRadioValue);
	OnOK();
}

BOOL CGameSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;
}
