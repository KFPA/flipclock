#pragma once

#include "UIAnimation.h"
#include <atl.mini/SComHelper.h>
#include <ShlObj.h>

namespace SOUI
{
    class CUiAnimation : public SSingleton<CUiAnimation>
    {
    public:
        static HRESULT Init();
        static void Free();
        
        SComPtr<IUIAnimationTimer>              m_pAnimationTimer;
        SComPtr<IUIAnimationTransitionLibrary>  m_pTransitionLibrary;
        SComPtr<IUIAnimationTransitionFactory>  m_pTransitionFactory;
    protected:
        CUiAnimation(IUIAnimationTimer *pUiAniTimer);
    };
    
    class CImgThumnail
    {
    public:
    CImgThumnail();
    
    HRESULT Init(IUIAnimationManager * pAnimationMgr,ISkinObj * pImg, double x, double y);

    CSize GetSize();
    void Render(IRenderTarget *pRT,const CRect &rcClient);
    
    CAutoRefPtr<IUIAnimationVariable> m_pAnimationVariableX;
    CAutoRefPtr<IUIAnimationVariable> m_pAnimationVariableY;

    protected:
    CAutoRefPtr<ISkinObj>   m_pImg;
    };
    
    class SUiAnimationWnd;
    class CUiAnimationIconLayout
    {
	protected:
        struct CHARBITS
        {
            CHARBITS():pBits(NULL){}
            ~CHARBITS(){if(pBits) delete []pBits;}

            char *pBits;
            CSize  sz;
        };
    public:
		CUiAnimationIconLayout();
        CUiAnimationIconLayout(SUiAnimationWnd *pOwner,IBitmap *pBmpMode,int nIconCount);
        ~CUiAnimationIconLayout();

        void SetIcons(ISkinObj *pIcon,int nIcons);
		void SetLayoutOffset(CSize size, ISkinObj *pIcon);

        HRESULT Arrange(const CSize & sz,BOOL bSizeChanged = FALSE);
        
        IUIAnimationManager * GetAnimationMgr() {return m_pAnimationManager;}

     

        void OnPaint(IRenderTarget *pRT,const CRect &rcClient);
    protected:
		virtual BOOL GetIconsPos(const CSize & sz, LPPOINT pts, BOOL bSizeChanged);

        HRESULT AddIconTransitions(
            IUIAnimationStoryboard *pStoryboard,
            IUIAnimationVariable *pVariablePrimary,
            DOUBLE valuePrimary,
            IUIAnimationVariable *pVariableSecondary,
            DOUBLE valueSecondary
            );

        HRESULT ScheduleStoryboard(
            IUIAnimationStoryboard *pStoryboard
            );

        SComPtr<IUIAnimationManager>    m_pAnimationManager;
        CImgThumnail *  m_plstIcon;
        int             m_nIcons;
        SArray<CHARBITS*>     m_arrCharBits;



        SUiAnimationWnd     * m_pOwner;
	private:
		struct MODLEINFO
		{
			INT nNumber;
			INT nPtpos;
			BOOL bUpdate;
		};
		SArray<MODLEINFO> m_arrModal;

		struct OFFSET
		{
			int xOffset = 0;
			int yOffset = 0;
			double dScale = 1.0;
		};
		SArray<OFFSET> m_arrOffset;

		int m_nxDotOffset = 0;
		int m_nyDotFirOffset = 0;
		int m_nyDotSecOffset = 0;

		MODLEINFO modalInfo;

		POINT *m_pts;
    };





    class SUiAnimationWnd : public SWindow
    {
        SOUI_CLASS_NAME(SUiAnimationWnd,L"uianimationwnd")
    public:
        SUiAnimationWnd(void);
        ~SUiAnimationWnd(void);
        
    public:
        int OnCreate(void *);
        void OnDestroy();
        void OnSize(UINT nType, CSize size);
        void OnPaint(IRenderTarget *pRT);
        void OnTimer(char cEvt);
        void OnShowWindow(BOOL bShow, UINT nStatus);
        SOUI_MSG_MAP_BEGIN()
            MSG_WM_CREATE(OnCreate)
            MSG_WM_DESTROY(OnDestroy)
            MSG_WM_PAINT_EX(OnPaint)
            MSG_WM_TIMER_EX(OnTimer)
        SOUI_MSG_MAP_END()

        SOUI_ATTRS_BEGIN()
            ATTR_SKIN(L"icon",m_pSkinIcon,FALSE)
			ATTR_INT(L"iconCount",m_iconCount,FALSE)
            ATTR_IMAGE(L"aniMode",m_pAniMode,FALSE)
			ATTR_INT(L"time", m_nTime, FALSE)
        SOUI_ATTRS_END()
    protected:
        
        ISkinObj        *               m_pSkinIcon;
        IBitmap         *               m_pAniMode;
        CUiAnimationIconLayout *        m_pLayout;
		INT                             m_iconCount;
		INT                             m_nTime;
		BOOL m_bUpdateOffset;
    };

}
