#pragma once
#include "../helper/SAdapterBase.h"
class CStyleTileViewAdapter :
	public SAdapterBase
{
public:
	CStyleTileViewAdapter(SHostWnd* pDlg);
	~CStyleTileViewAdapter();

	/*
	function����������
	arrData���������ݵ����飬���ñ���	
	*/
	void SetArray(SArray<ITEMDATA>& arrData);
protected:	
	virtual int getCount();
	virtual void getView(int position, SWindow *pItem, pugi::xml_node xmlTemplate);
private:
	SArray<ITEMDATA*> m_itemInfo;
	SHostWnd* m_pDlg;
};

