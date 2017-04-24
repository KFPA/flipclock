#include "stdafx.h"
#include "UiAnimationWnd.h"
#include <math.h>
#include <helper/splitstring.h>

#ifndef IID_PPV_ARGS
extern "C++"
{
    template<typename T> void** IID_PPV_ARGS_Helper(T** pp) 
    {
        // make sure everyone derives from IUnknown
        static_cast<IUnknown*>(*pp);
        
        return reinterpret_cast<void**>(pp);
    }
}

#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), IID_PPV_ARGS_Helper(ppType)
#endif



namespace SOUI{
	#define HOUR_MINUTE_DISPLAY_COUNT   2
	#define SECOND_DIPALY_COUNT    2
    #define TIEMRID_MODE    1

    //////////////////////////////////////////////////////////////////////////
    //  CUiAnimation
    template<> CUiAnimation* SSingleton<CUiAnimation>::ms_Singleton = NULL;
    

    CUiAnimation::CUiAnimation(IUIAnimationTimer *pUiAniTimer)
    {
        SASSERT(pUiAniTimer);
        m_pAnimationTimer = pUiAniTimer;
        
        // Create Animation Transition Library

        HRESULT hr = CoCreateInstance(
            __uuidof(UIAnimationTransitionLibrary),
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pTransitionLibrary)
            );
        if (SUCCEEDED(hr))
        {
            // Create the Transition Factory to wrap interpolators in transitions

            hr = CoCreateInstance(
                __uuidof(UIAnimationTransitionFactory),
                NULL,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&m_pTransitionFactory)
                );
        }
        
        SASSERT(SUCCEEDED(hr));
    }

    HRESULT CUiAnimation::Init()
    {
        SASSERT(!getSingletonPtr());
        
            // Create Animation Timer
        SComPtr<IUIAnimationTimer>    pAnimationTimer;

        HRESULT hr = CoCreateInstance(
            __uuidof(UIAnimationTimer),
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pAnimationTimer)
            );
        if (SUCCEEDED(hr))
        {
            //创建一个UIAnimation的单例
            new CUiAnimation(pAnimationTimer);
        }

        return hr;
    }

    void CUiAnimation::Free()
    {
        if(getSingletonPtr())
        {
            delete getSingletonPtr();
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // CUIAnimationManagerEventHandler
    class CUIAnimationManagerEventHandler : public SUnknownImpl<IUIAnimationManagerEventHandler>
    {
    public:
        CUIAnimationManagerEventHandler(SUiAnimationWnd * pOwner);
        ~CUIAnimationManagerEventHandler();
        
        virtual /* [annotation] */ 
            __checkReturn
            HRESULT STDMETHODCALLTYPE OnManagerStatusChanged( 
            /* [annotation][in] */ 
            __in  UI_ANIMATION_MANAGER_STATUS newStatus,
            /* [annotation][in] */ 
            __in  UI_ANIMATION_MANAGER_STATUS previousStatus);

        COM_INTERFACE_BEGIN()
            COM_INTERFACE(IUIAnimationManagerEventHandler)
        COM_INTERFACE_END()

    protected:
        SUiAnimationWnd *m_pOwner;
    };

    CUIAnimationManagerEventHandler::CUIAnimationManagerEventHandler(SUiAnimationWnd * pOwner):m_pOwner(pOwner)
    {
        m_pOwner->AddRef();
    }

    HRESULT STDMETHODCALLTYPE CUIAnimationManagerEventHandler::OnManagerStatusChanged(/* [annotation][in] */ __in UI_ANIMATION_MANAGER_STATUS newStatus, /* [annotation][in] */ __in UI_ANIMATION_MANAGER_STATUS previousStatus)
    {
        if (newStatus == UI_ANIMATION_MANAGER_BUSY)
        {
            m_pOwner->Invalidate();
        }

        return S_OK;
    }

    CUIAnimationManagerEventHandler::~CUIAnimationManagerEventHandler()
    {
        m_pOwner->Release();
    }

    //////////////////////////////////////////////////////////////////////////
    //  CImgThumnail
    CImgThumnail::CImgThumnail():m_pImg(NULL)
    {

    }

    HRESULT CImgThumnail::Init(IUIAnimationManager * pAnimationMgr,ISkinObj * pImg, double x, double y)
    {
        m_pImg = pImg;

        // Create the animation variables for the x and y coordinates

        HRESULT hr = pAnimationMgr->CreateAnimationVariable(
            x,
            &m_pAnimationVariableX
            );
        if (SUCCEEDED(hr))
        {
            hr = pAnimationMgr->CreateAnimationVariable(
                y,
                &m_pAnimationVariableY
                );
        }
        return hr;
    }

    void CImgThumnail::Render(IRenderTarget *pRT,const CRect &rcClient)
    {
        SASSERT(m_pImg);
        DOUBLE x = 0;
        HRESULT hr = m_pAnimationVariableX->GetValue(
            &x
            );
        if (SUCCEEDED(hr))
        {
            DOUBLE y = 0;
            hr = m_pAnimationVariableY->GetValue(
                &y
                );
            if (SUCCEEDED(hr))
            {
                CRect rc(CPoint((int)x,(int)y)+rcClient.TopLeft(),GetSize());
                m_pImg->Draw(pRT,&rc,0,0xFF);
            }
        }
    }

    CSize CImgThumnail::GetSize()
    {
        SASSERT(m_pImg);
        return m_pImg->GetSkinSize();
    }

    //////////////////////////////////////////////////////////////////////////
    //  CUiAnimationIconLayout
    CUiAnimationIconLayout::CUiAnimationIconLayout(SUiAnimationWnd *pOwner,IBitmap *pBmpMode)
        :m_pOwner(pOwner),m_plstIcon(NULL),m_nIcons(0)
    {
		m_arrModal.RemoveAll();

        HRESULT hr = CoCreateInstance(
            __uuidof(UIAnimationManager),
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pAnimationManager)
            );
        if(SUCCEEDED(hr))
        {
            CUIAnimationManagerEventHandler      *  pAnimationEvtHandler = new CUIAnimationManagerEventHandler(m_pOwner);
            hr = m_pAnimationManager->SetManagerEventHandler(pAnimationEvtHandler);
            pAnimationEvtHandler->Release();
        }

        LPDWORD pPixels = (LPDWORD)pBmpMode->LockPixelBits();
        SIZE    sz = pBmpMode->Size();
        int iX1=0;
        for(int i=0;i<=sz.cx;i++)
        {
            if(i==sz.cx || pPixels[i] == 0xFFFF00FF) //分割线
            {
                int iX2=i;
                SIZE szWord={iX2-iX1,sz.cy};
                
                CHARBITS *pCharBits = new CHARBITS;
                pCharBits->pBits = new char[szWord.cx * szWord.cy];
                pCharBits->sz=szWord;
                
                for(int y =0 ;y< szWord.cy;y++)
                {
                    LPDWORD pLine = pPixels + y * sz.cx;
                    for(int x=0;x<szWord.cx;x++)
                    {
                        pCharBits->pBits[y*szWord.cx+x] = pLine[iX1+x] != 0xFFFFFFFF;
                    }
                }
                m_arrCharBits.Add(pCharBits);
                iX1=iX2+1;
            }
        }
    }

	CUiAnimationIconLayout::CUiAnimationIconLayout()
	{

	}


    CUiAnimationIconLayout::~CUiAnimationIconLayout()
    {
        if(m_plstIcon) delete [] m_plstIcon;
        for(UINT i=0;i<m_arrCharBits.GetCount();i++)
        {
            delete m_arrCharBits[i];
        }
    }

    void CUiAnimationIconLayout::SetIcons(ISkinObj *pIcon,int nIcons )
    {
        m_plstIcon = new CImgThumnail[nIcons];
        m_nIcons=nIcons;
        for(int i=0;i<m_nIcons;i++)
        {
            m_plstIcon[i].Init(GetAnimationMgr(),pIcon,0.0,0.0);
        }
    }

	HRESULT CUiAnimationIconLayout::Arrange(const CSize & sz, BOOL bSizeChanged/* = FALSE*/)
    {
        
        IUIAnimationStoryboard *pStoryboard;
        HRESULT hr = m_pAnimationManager->CreateStoryboard(
            &pStoryboard
            );
        if (SUCCEEDED(hr))
        {
            // Arrange the thumbnails, adding transitions to move each thumbnail to a random new location
            POINT *pts = new POINT[m_nIcons];
			BOOL bRet = GetIconsPos(sz, pts, bSizeChanged);

			if(FALSE == bRet) 
			{			
				hr = ScheduleStoryboard(pStoryboard);	
				delete[]pts;
				pStoryboard->Release();
				return hr;
			}
          

            for (int i = 0; i < m_nIcons; i++)
            {
                CSize sizeIcon = m_plstIcon[i].GetSize();
                DOUBLE xDest, yDest;
                xDest = pts[i].x;
                yDest = pts[i].y;

                // Get the current position
                // Note that this technique is valid only when the storyboard will begin playing immediately

                DOUBLE xCur;
                hr = m_plstIcon[i].m_pAnimationVariableX->GetValue(&xCur);
                if (SUCCEEDED(hr))
                {
                    DOUBLE yCur;
                    hr = m_plstIcon[i].m_pAnimationVariableY->GetValue(&yCur);
                    if (SUCCEEDED(hr))
                    {
                        // Add transitions for x and y movement

                        if (fabs(xDest - xCur) > fabs(yDest - yCur))
                        {
                            // If the thumbnail has further to travel horizontally than vertically, use a parabolic transition
                            // on X that will determine the duration of the storyboard, and stretch an accelerate-decelerate
                            // transition on Y to have the same duration.

                            hr = AddIconTransitions(
                                pStoryboard,
                                m_plstIcon[i].m_pAnimationVariableX,
                                xDest,
                                m_plstIcon[i].m_pAnimationVariableY,
                                yDest
                                );
                        }
                        else
                        {
                            // If the thumbnail has further to travel vertically than horizontally, use a parabolic transition
                            // on Y that will determine the duration of the storyboard, and stretch an accelerate-decelerate
                            // transition on X to have the same duration.

                            hr = AddIconTransitions(
                                pStoryboard,
                                m_plstIcon[i].m_pAnimationVariableY,
                                yDest,
                                m_plstIcon[i].m_pAnimationVariableX,
                                xDest
                                );
                        }
                    }
                }

                if (FAILED(hr))
                {
                    break;
                }
            }

            if (SUCCEEDED(hr))
            {
                hr = ScheduleStoryboard(pStoryboard);
            }
            delete []pts;
            pStoryboard->Release();
        }

        return hr;
    }



    // Adds two transitions to a storyboard: one primary parabolic transition, which will determine
    // the storyboard duration, and a secondary accelerate-decelerate transition, which will be
    // stretched to that duration.

    HRESULT CUiAnimationIconLayout::AddIconTransitions(
        IUIAnimationStoryboard *pStoryboard,
        IUIAnimationVariable *pVariablePrimary,
        DOUBLE valuePrimary,
        IUIAnimationVariable *pVariableSecondary,
        DOUBLE valueSecondary
        )
    {
        const DOUBLE ACCELERATION = 2000;
        const DOUBLE ACCELERATION_RATIO = 0.3;
        const DOUBLE DECELERATION_RATIO = 0.3;

        IUIAnimationTransition *pTransitionPrimary;
        HRESULT hr = CUiAnimation::getSingleton().m_pTransitionLibrary->CreateParabolicTransitionFromAcceleration(
            valuePrimary,
            0.0,
            ACCELERATION,
            &pTransitionPrimary
            );
        if (SUCCEEDED(hr))
        {
            hr = pStoryboard->AddTransition(
                pVariablePrimary,
                pTransitionPrimary
                );
            if (SUCCEEDED(hr))
            {
                UI_ANIMATION_KEYFRAME keyframeEnd;
                hr = pStoryboard->AddKeyframeAfterTransition(
                    pTransitionPrimary,
                    &keyframeEnd
                    );
                if (SUCCEEDED(hr))
                {
                    IUIAnimationTransition *pTransitionSecondary;
                    hr = CUiAnimation::getSingleton().m_pTransitionLibrary->CreateAccelerateDecelerateTransition(
                        1.0,    // Will be overwritten, so unimportant
                        valueSecondary,
                        ACCELERATION_RATIO,
                        DECELERATION_RATIO,    
                        &pTransitionSecondary
                        );
                    if (SUCCEEDED(hr))
                    {
                        hr = pStoryboard->AddTransitionBetweenKeyframes(
                            pVariableSecondary,
                            pTransitionSecondary,
                            UI_ANIMATION_KEYFRAME_STORYBOARD_START,
                            keyframeEnd
                            );
                        pTransitionSecondary->Release();
                    }	            
                }
            }

            pTransitionPrimary->Release();
        }

        return hr;
    }

    HRESULT CUiAnimationIconLayout::ScheduleStoryboard( IUIAnimationStoryboard *pStoryboard )
    {
        UI_ANIMATION_SECONDS secondsNow;
        HRESULT hr = CUiAnimation::getSingleton().m_pAnimationTimer->GetTime(
            &secondsNow
            );
        if (SUCCEEDED(hr))
        {
            hr = pStoryboard->Schedule(
                secondsNow
                );
        }

        return hr;

    }

	void CUiAnimationIconLayout::SetLayoutOffset(CSize size, ISkinObj *pIcon)
	{
		m_arrOffset.RemoveAll();

		SIZE szModel = m_arrCharBits[0]->sz;
		CSize szClient = size - pIcon->GetSkinSize();

		double dScale = 1.0;
		if (szModel.cx * szClient.cy > szModel.cy* szClient.cx)
		{
			dScale = (szClient.cx*1.0 / szModel.cx) / 2;
		}
		else
		{
			dScale = (szClient.cy*1.0 / szModel.cy) / 2;
		}
		int nWid = (int)((szModel.cx*dScale));
		int nHei = (int)((szModel.cy*dScale));

		int yOffset = (szClient.cy - nHei) / 3;

		OFFSET offset;
		int xHourFirOffset = (szClient.cx - nWid * 4) / 5;
		offset.xOffset = xHourFirOffset;
		offset.yOffset = yOffset;
		offset.dScale = dScale;
		m_arrOffset.Add(offset);

		int xHourSecOffset = 2 * xHourFirOffset + nWid;
		offset.xOffset = xHourSecOffset;
		offset.yOffset = yOffset;
		offset.dScale = dScale;
		m_arrOffset.Add(offset);

		int xMinuFirOffset = 2 * xHourSecOffset - xHourFirOffset;
		offset.xOffset = xMinuFirOffset;
		offset.yOffset = yOffset;
		offset.dScale = dScale;
		m_arrOffset.Add(offset);

		int xMinuSecOffset = 4 * xHourFirOffset + 3 * nWid;
		offset.xOffset = xMinuSecOffset;
		offset.yOffset = yOffset;
		offset.dScale = dScale;
		m_arrOffset.Add(offset);

		m_nxDotOffset = xMinuFirOffset - xHourFirOffset / 2;
		m_nyDotFirOffset = szClient.cy / 3 + (nHei * 5) / 12;
		m_nyDotSecOffset = szClient.cy / 3 - nHei / 12;


		int xSecFirOffset = m_nxDotOffset - nWid / 2;
		offset.xOffset = xSecFirOffset;
		offset.yOffset = yOffset + nHei;
		offset.dScale = dScale / 2;
		m_arrOffset.Add(offset);

		int xSecSecOffset = m_nxDotOffset;
		offset.xOffset = xSecSecOffset;
		offset.yOffset = yOffset + nHei;
		offset.dScale = dScale / 2;
		m_arrOffset.Add(offset);

	}

	BOOL CUiAnimationIconLayout::GetIconsPos(const CSize & sz, LPPOINT pts, BOOL bSizeChanged)
    {
		
		if (m_arrCharBits.GetCount() != 10)
		{
			return FALSE;
		}
		SYSTEMTIME last_refresh_time;
		::GetLocalTime(&last_refresh_time);
		
		SArray<INT> arrModal;
		arrModal.InsertAt(0, last_refresh_time.wHour / 10);
		arrModal.InsertAt(1, last_refresh_time.wHour % 10);
		arrModal.InsertAt(2, last_refresh_time.wMinute / 10);
		arrModal.InsertAt(3, last_refresh_time.wMinute % 10);
		arrModal.InsertAt(4, last_refresh_time.wSecond / 10);
		arrModal.InsertAt(5, last_refresh_time.wSecond % 10);
		BOOL bIsUpdate = FALSE;
		if (m_arrModal.IsEmpty())
		{
			for (size_t i = 0; i < arrModal.GetCount(); i++)
			{			
				m_arrModal.InsertAt(i, arrModal.GetAt(i));		
				bIsUpdate = TRUE;
			}
		}
		else
		{
			for (size_t i = 0; i < arrModal.GetCount(); i++)
			{
				if (0 > i || m_arrModal.GetCount() < i) continue;
				if (arrModal.GetAt(i) != m_arrModal.GetAt(i))
				{
					m_arrModal.InsertAt(i, arrModal.GetAt(i));
					bIsUpdate = TRUE;
				}
			}
		}

		if (!bSizeChanged && !bIsUpdate && m_arrModal.GetCount() > 0)
		{
			return FALSE;
		}

		int idx = 0;
		double dScale = 0;
		int xOffset = 0;
		int yOffset = 0;
		for (size_t i = 0; i < arrModal.GetCount(); i++)
		{
			CHARBITS * pCharBit = m_arrCharBits[arrModal.GetAt(i)];	
			OFFSET offset;
			if (i >= 0 && i < m_arrOffset.GetCount())
			{
				offset = m_arrOffset.GetAt(i);
			}
			xOffset = offset.xOffset;
			yOffset = offset.yOffset;
			dScale = offset.dScale;

			for (int y = 0; y < pCharBit->sz.cy && idx < m_nIcons - 1; y++)
			{
				for (int x = 0; x < pCharBit->sz.cx && idx < m_nIcons - 1; x++)
				{
					if (pCharBit->pBits[y*pCharBit->sz.cx + x])
					{
						SASSERT(idx < m_nIcons);
						DOUBLE xDest, yDest;

						xDest = xOffset + x * dScale;
						yDest = yOffset + y * dScale;
						pts[idx++] = CPoint((int)xDest, (int)yDest);
					}
				}
			}
		}

       
		for (; idx < m_nIcons - 200; idx++)
		{
			pts[idx] = CPoint(m_nxDotOffset, m_nyDotFirOffset);
		}
		for (; idx < m_nIcons; idx++)
		{
			pts[idx] = CPoint(m_nxDotOffset, m_nyDotSecOffset);
		}
		return TRUE;
    }

    void CUiAnimationIconLayout::OnPaint( IRenderTarget *pRT ,const CRect &rcClient)
    {
        UI_ANIMATION_SECONDS secondsNow;
        HRESULT hr = CUiAnimation::getSingleton().m_pAnimationTimer->GetTime(
            &secondsNow
            );
        if(SUCCEEDED(hr))
        {
            m_pAnimationManager->Update(
                secondsNow
                );
        }

        for(int i=0;i< m_nIcons;i++)
        {
            m_plstIcon[i].Render(pRT,rcClient);
        }

        UI_ANIMATION_MANAGER_STATUS status;
        hr = m_pAnimationManager->GetStatus(
            &status
            );
        if (SUCCEEDED(hr) && status == UI_ANIMATION_MANAGER_BUSY)
        {
            m_pOwner->Invalidate();
        }

    }



    //////////////////////////////////////////////////////////////////////////
    //  SUiAnimationWnd
	SUiAnimationWnd::SUiAnimationWnd(void) :m_pSkinIcon(NULL), m_pLayout(NULL), m_pAniMode(NULL), m_bResized(FALSE), m_iconCount(460), m_bUpdateOffset(TRUE)
    {
        m_bClipClient = TRUE;
    }

    SUiAnimationWnd::~SUiAnimationWnd(void)
    {
        if(m_pAniMode) m_pAniMode->Release();
    }

    
    int SUiAnimationWnd::OnCreate(void *)
    {
        SWindow::OnCreate(NULL);
        if(!m_pSkinIcon || !m_pAniMode)
        {
            SASSERT_FMT(FALSE,_T("please set icon and bmpmode value in xml!"));
            return -1;
        }

		m_pLayout = new CUiAnimationIconLayout(this, m_pAniMode);
		m_pLayout->SetIcons(m_pSkinIcon, m_iconCount);
		
        
        SetTimer(TIEMRID_MODE,100);
        return 0;
    }

    void SUiAnimationWnd::OnDestroy()
    {
        SWindow::OnDestroy();
		if (m_pLayout) delete m_pLayout;
    }

    void SUiAnimationWnd::OnPaint( IRenderTarget *pRT )
    {

		CRect rcClient;
		GetClientRect(&rcClient);
		if (m_pLayout)
		{
			if (m_bUpdateOffset)
			{
				m_pLayout->SetLayoutOffset(rcClient.Size(), m_pSkinIcon);
				m_bUpdateOffset = FALSE;
			}
			m_pLayout->OnPaint(pRT, rcClient);
		}       
    }

    void SUiAnimationWnd::OnTimer(char cEvt)
    {
        if(cEvt == TIEMRID_MODE)
        {		
            if(IsVisible(TRUE))
            {
                CRect rc;
                GetClientRect(&rc);
                CSize sz=rc.Size();
				m_pLayout->Arrange(sz,TRUE);	   
            }
			
        }
		SetMsgHandled(FALSE);
			
    }

    void SUiAnimationWnd::OnSize(UINT nType, CSize size)
    { 
        SWindow::OnSize(nType,size);
        if(m_pLayout && IsVisible(TRUE))
        {
            m_bResized = FALSE;		
			m_pLayout->Arrange(size);			   
        }
        else
			m_bResized =TRUE;
    }

    void SUiAnimationWnd::OnShowWindow( BOOL bShow, UINT nStatus )
    {
        SWindow::OnShowWindow(bShow,nStatus);
        if(bShow && m_bResized)
        {
            CRect rcClient;
            GetClientRect(&rcClient);
            OnSize(0,rcClient.Size());
        }
    }




}