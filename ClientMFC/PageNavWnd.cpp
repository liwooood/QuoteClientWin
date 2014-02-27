// PageNavWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "PageNavWnd.h"
#include "PageContainerWnd.h"
#include "user_defined_msg.h"


// CPageNavWnd

IMPLEMENT_DYNAMIC(CPageNavWnd, CWnd)

CPageNavWnd::CPageNavWnd()
{
}

CPageNavWnd::~CPageNavWnd()
{
}


BEGIN_MESSAGE_MAP(CPageNavWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPageNavWnd 消息处理程序




void CPageNavWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	TRACE("OnPaint......\n");

	Render();

}

void CPageNavWnd::Render()
{
	
	

	if (m_pRender)
	{
		m_pRender->BeginRender();
		
		CColorUtil clr(0,0,0);
		m_pRender->DrawRectangle(m_rcClient, clr);



		CColorUtil clrBgInactive(128,128,128);
		CColorUtil clrCaptionInactive(255,255,255);

		CColorUtil clrBgActive(256,182,232);
		CColorUtil clrCaptionActive(0,0,0);

		for (int i=0; i<m_vTabCaption.size(); i++)
		{
			if (i == m_nActivePage)
			{
				CRect& rect = m_vTabRect[i];
				TRACE("Render 矩形left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
				CString& s = m_vTabCaption[i];
				m_pRender->FillRectangle(m_vTabRect[i], clrBgActive);
				m_pRender->DrawText(rect, s, clrCaptionActive);
			}
			else
			{
				CRect& rect = m_vTabRect[i];
				TRACE("Render 矩形left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
				CString& s = m_vTabCaption[i];
				m_pRender->FillRectangle(m_vTabRect[i], clrBgInactive);
				m_pRender->DrawText(rect, s, clrCaptionInactive);
			}
		}

		m_pRender->EndRender();
	}

}


int CPageNavWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	TRACE("OnCreate......\n");

	m_pRender = new GDIRenderEngine(m_hWnd);
	

	

	m_nActivePage = 0;
	m_nColSep = 4;

	return 0;
}

void CPageNavWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = NULL;
	}
}


void CPageNavWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//TRACE("CPageNavWnd OnLButtonDown\n");
	
	int nPage = -1;

	for (int i=0; i< m_vTabRect.size(); i++)
	{
		if (m_vTabRect[i].PtInRect(point))
		{
			nPage = i;
			ActivePage(nPage);
			break;
		}
	}

	
	

	CWnd::OnLButtonDown(nFlags, point);
}


void CPageNavWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//TRACE("CPageNavWnd OnMouseMove\n");

	CWnd::OnMouseMove(nFlags, point);
}






void CPageNavWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	TRACE("OnSize......\n");
	
	InitWnd();
}

void CPageNavWnd::InitWnd()
{
	GetClientRect(m_rcClient);

	if (m_pRender)
	{
		m_pRender->BeginRender();

		for(int i=0; i< m_vTabCaption.size(); i++)
		{
			CSize size = m_pRender->GetStringSize(_T("宋体"), 12, m_vTabCaption[i]);
			
			//TRACE("导航组件高度%d\n", size.cy);
			
			CRect rect;
			if (i==0)
				rect.left = 0;
			else
				rect.left = m_vTabRect[i-1].right + 1;

			rect.top = m_rcClient.top;
			rect.right = rect.left + size.cx + m_nColSep;
			rect.bottom = m_rcClient.bottom;
			
			//rect.bottom = 20;

			m_vTabRect[i]= rect;
			TRACE("InitWnd 矩形left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
		}

		m_pRender->EndRender();
	}
}

void CPageNavWnd::AddPage(CString sPageCaption)
{
	m_vTabCaption.push_back(sPageCaption);
	m_vTabRect.push_back(CRect(0,0,0,0));

	InitWnd();
	
	Render();
}

void CPageNavWnd::ActivePage(int nPage)
{
	// 相同页，无需激活
	if (nPage == m_nActivePage)
		return;

	m_nActivePage = nPage;

	GetParent()->SendMessage(MSG_ACTIVE_PAGE, (WPARAM)m_nActivePage);
	
	Render();
}
