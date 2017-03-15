#pragma once
#include "../helper/SAdapterBase.h"
class CStyleTileViewAdapter :
	public SAdapterBase
{
public:
	CStyleTileViewAdapter(SHostWnd* pDlg);
	~CStyleTileViewAdapter();

	/*
	function：设置数据
	arrData：包含数据的数组，采用别名	
	*/
	void SetArray(SArray<ITEMDATA>& arrData);
protected:	
	virtual int getCount();
	virtual void getView(int position, SWindow *pItem, pugi::xml_node xmlTemplate);
private:
	SArray<ITEMDATA*> m_itemInfo;
	SHostWnd* m_pDlg;
};

