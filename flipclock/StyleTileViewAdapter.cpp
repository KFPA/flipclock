#include "stdafx.h"
#include "StyleTileViewAdapter.h"


CStyleTileViewAdapter::CStyleTileViewAdapter(SHostWnd* pDlg)
{
	m_pDlg = pDlg;
}


CStyleTileViewAdapter::~CStyleTileViewAdapter()
{
}

int CStyleTileViewAdapter::getCount()
{
	return m_itemInfo.GetCount();
}

void CStyleTileViewAdapter::SetArray(SArray<ITEMDATA>& arrData)
{
	m_itemInfo.RemoveAll();
	for (size_t i = 0; i < arrData.GetCount();i++)
	{
		ITEMDATA& data = arrData.GetAt(i);
		m_itemInfo.Add(&data);
	}
	notifyDataSetChanged();
}

void CStyleTileViewAdapter::getView(int position, SWindow *pItem, pugi::xml_node xmlTemplate)
{
	if (pItem->GetChildrenCount() == 0)
	{
		pItem->InitFromXml(xmlTemplate);
	}

	ITEMDATA* pItemData = NULL;
	pItemData = (m_itemInfo.GetAt(position));

	SImageWnd *pImg = pItem->FindChildByName2<SImageWnd>(R.name.img_style);
	if (pImg)
	{
		pImg->SetSkin(GETSKIN(pItemData->pszItemSkin));
		pImg->SetVisible(TRUE);
	}

	SStatic* pDescription = pItem->FindChildByName2<SStatic>(R.name.text_des);
	if (pDescription)
	{
		pDescription->SetWindowText(pItemData->pszItemDes);
		pDescription->SetVisible(TRUE);
	}
}
