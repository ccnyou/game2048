#pragma once

#include "resource.h"

// CGameSettingDlg 对话框
enum GAME_MODE;

class CGameSettingDlg : public CDialog
{
    DECLARE_DYNAMIC(CGameSettingDlg)

public:
    CGameSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CGameSettingDlg();

    // 对话框数据
    enum { IDD = IDD_DIALOG1 };

public:
    void		SetGameMode(GAME_MODE gameMode);
    GAME_MODE	GetGameMode() const;

protected:
    virtual BOOL		OnInitDialog();
    virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    afx_msg void OnDestroy();
    afx_msg void OnBnClickedOk();
    DECLARE_MESSAGE_MAP()

private:
    GAME_MODE	m_gameMode;
    int			m_nRadioValue;
};
