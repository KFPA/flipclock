//stamp:10679dc8ff99062d
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _LAYOUT{
			public:
			_LAYOUT(){
				XML_MAINWND = _T("LAYOUT:XML_MAINWND");
				XML_SETTINGWND = _T("LAYOUT:XML_SETTINGWND");
				PAGE_SETTINGS = _T("LAYOUT:PAGE_SETTINGS");
				PAGE_ITEM = _T("LAYOUT:PAGE_ITEM");
				PAGE_STYLE = _T("LAYOUT:PAGE_STYLE");
			}
			const TCHAR * XML_MAINWND;
			const TCHAR * XML_SETTINGWND;
			const TCHAR * PAGE_SETTINGS;
			const TCHAR * PAGE_ITEM;
			const TCHAR * PAGE_STYLE;
		}LAYOUT;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
		}values;
		class _IMG{
			public:
			_IMG(){
				png_mode = _T("IMG:png_mode");
				png_picture = _T("IMG:png_picture");
				png_bkgnd = _T("IMG:png_bkgnd");
				png_shadow = _T("IMG:png_shadow");
				png_bkmain = _T("IMG:png_bkmain");
				png_tableft = _T("IMG:png_tableft");
				png_tabicon = _T("IMG:png_tabicon");
				png_bk1 = _T("IMG:png_bk1");
			}
			const TCHAR * png_mode;
			const TCHAR * png_picture;
			const TCHAR * png_bkgnd;
			const TCHAR * png_shadow;
			const TCHAR * png_bkmain;
			const TCHAR * png_tableft;
			const TCHAR * png_tabicon;
			const TCHAR * png_bk1;
		}IMG;
		class _ICON{
			public:
			_ICON(){
				ICON_LOGO = _T("ICON:ICON_LOGO");
			}
			const TCHAR * ICON_LOGO;
		}ICON;
		class _translator{
			public:
			_translator(){
				lang_cn = _T("translator:lang_cn");
			}
			const TCHAR * lang_cn;
		}translator;
		class _SMENU{
			public:
			_SMENU(){
				menu_tray = _T("SMENU:menu_tray");
			}
			const TCHAR * menu_tray;
		}SMENU;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"btn_close",65539},
		{L"btn_min",65540},
		{L"btn_setting",65544},
		{L"btn_setting_cancel",65543},
		{L"img_style",65547},
		{L"page_style_tile",65546},
		{L"set_form",65538},
		{L"set_style",65545},
		{L"tab_main",65536},
		{L"tab_settings",65541},
		{L"test",65542},
		{L"text_des",65548},
		{L"turn3dview",65537}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			btn_close = namedXmlID[0].strName;
			btn_min = namedXmlID[1].strName;
			btn_setting = namedXmlID[2].strName;
			btn_setting_cancel = namedXmlID[3].strName;
			img_style = namedXmlID[4].strName;
			page_style_tile = namedXmlID[5].strName;
			set_form = namedXmlID[6].strName;
			set_style = namedXmlID[7].strName;
			tab_main = namedXmlID[8].strName;
			tab_settings = namedXmlID[9].strName;
			test = namedXmlID[10].strName;
			text_des = namedXmlID[11].strName;
			turn3dview = namedXmlID[12].strName;
		}
		 const wchar_t * btn_close;
		 const wchar_t * btn_min;
		 const wchar_t * btn_setting;
		 const wchar_t * btn_setting_cancel;
		 const wchar_t * img_style;
		 const wchar_t * page_style_tile;
		 const wchar_t * set_form;
		 const wchar_t * set_style;
		 const wchar_t * tab_main;
		 const wchar_t * tab_settings;
		 const wchar_t * test;
		 const wchar_t * text_des;
		 const wchar_t * turn3dview;
		}name;

		class _id{
		public:
		const static int btn_close	=	65539;
		const static int btn_min	=	65540;
		const static int btn_setting	=	65544;
		const static int btn_setting_cancel	=	65543;
		const static int img_style	=	65547;
		const static int page_style_tile	=	65546;
		const static int set_form	=	65538;
		const static int set_style	=	65545;
		const static int tab_main	=	65536;
		const static int tab_settings	=	65541;
		const static int test	=	65542;
		const static int text_des	=	65548;
		const static int turn3dview	=	65537;
		}id;

		class _string{
		public:
		const static int custom	=	0;
		const static int customscreensaver	=	1;
		const static int softwareinfo	=	2;
		const static int style	=	3;
		const static int title	=	4;
		const static int ver	=	5;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int comblue	=	1;
		const static int gray	=	2;
		const static int green	=	3;
		const static int red	=	4;
		const static int white	=	5;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
