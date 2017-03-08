#include "stdafx.h"
#include "SettingDlg.h"


CSettingDlg::CSettingDlg() : SHostWnd(_T("LAYOUT:XML_SETTINGWND"))
{
	m_bLayoutInited = FALSE;
}


CSettingDlg::~CSettingDlg()
{
}

int CSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#ifdef DWMBLUR	//win7Ã«²£Á§¿ª¹Ø
	MARGINS mar = { 5, 5, 30, 5 };
	DwmExtendFrameIntoClientArea(m_hWnd, &mar);
#endif

	SetMsgHandled(FALSE);
	return 0;
}

BOOL CSettingDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}

void CSettingDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}
