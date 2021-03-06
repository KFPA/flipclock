#include "stdafx.h"
#include "SettingDlg.h"

#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#include "STurn3DView.h"
#include "StyleTileViewAdapter.h"
CSettingDlg::CSettingDlg() : SHostWnd(_T("LAYOUT:XML_SETTINGWND"))
{
	m_bLayoutInited = FALSE;
}


CSettingDlg::~CSettingDlg()
{
}

int CSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CSettingDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	m_bLayoutInited = TRUE;
	
	ITEMDATA itemData;
	itemData.pszItemSkin = L"skin_picture";
	itemData.pszItemDes = L"THIS IS";
	m_arrData.Add(itemData);
	STileView* pStyleTileView = FindChildByName2<STileView>(R.name.page_style_tile);
	if (pStyleTileView)
	{
		CStyleTileViewAdapter* pAdapter = new CStyleTileViewAdapter(this);
		pStyleTileView->SetAdapter(pAdapter);
		pAdapter->SetArray(m_arrData);
		pAdapter->Release();
	}
	return 0;
}

void CSettingDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CSettingDlg::OnSettingOk()
{
	STabCtrl *pTab = FindChildByName2<STabCtrl>(R.name.tab_main);
	if (pTab)
	{
		STurn3dView * pTurn3d = FindChildByName2<STurn3dView>(R.name.turn3dview);
		if (pTurn3d)
		{
			pTurn3d->Turn(pTab->GetPage(_T("setitem")), pTab->GetPage(_T("item")), FALSE);
		}
	}
}

void CSettingDlg::OnSetting()
{
	STabCtrl *pTab = FindChildByName2<STabCtrl>(R.name.tab_main);
	if (pTab)
	{
		STurn3dView * pTurn3d = FindChildByName2<STurn3dView>(R.name.turn3dview);
		if (pTurn3d)
		{
			pTurn3d->Turn(pTab->GetPage(_T("item")), pTab->GetPage(_T("setitem")), TRUE);
		}
	}
}
