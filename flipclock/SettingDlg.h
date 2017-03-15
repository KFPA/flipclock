#pragma once
class CSettingDlg :
	public SHostWnd
{
public:
	CSettingDlg();
	~CSettingDlg();

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
protected:
	void OnClose();
	void OnSettingOk();
	void OnSetting();
protected:
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		/*EVENT_NAME_COMMAND(R.name.btn_setting_ok, OnSettingOk)
		EVENT_NAME_COMMAND(R.name.btn_setting,OnSetting)*/
		EVENT_MAP_END()
		//HostWnd真实窗口消息处理
		BEGIN_MSG_MAP_EX(CSettingDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)

		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
		END_MSG_MAP()
private:
	SArray<ITEMDATA> m_arrData;
	BOOL			m_bLayoutInited;
};

