// TabCtrlWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "TabCtrlWnd.h"
#include "user_defined_msg.h"


// CTabCtrlWnd

IMPLEMENT_DYNAMIC(CTabCtrlWnd, CWnd)

CTabCtrlWnd::CTabCtrlWnd()
{

}

CTabCtrlWnd::~CTabCtrlWnd()
{
}


BEGIN_MESSAGE_MAP(CTabCtrlWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//ON_COMMAND_RANGE(MSG_GRID_POPUP_MENU_BEGIN, MSG_GRID_POPUP_MENU_END, &CTabCtrlWnd::OnPopupMenu)
	//ON_UPDATE_COMMAND_UI_RANGE(MSG_GRID_POPUP_MENU_BEGIN, MSG_GRID_POPUP_MENU_END, &CTabCtrlWnd::OnUpdatePopupMenu)
	ON_COMMAND(4096, OnTest)
END_MESSAGE_MAP()



// CTabCtrlWnd 消息处理程序




int CTabCtrlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pRender = new GDIRenderEngine(m_hWnd);

	m_nActiveTab = 0;
	m_nTabSep = 4;

	InitTab();

	return 0;
}


void CTabCtrlWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = NULL;
	}
}


void CTabCtrlWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for (int i=0; i< m_Tabs.size(); i++)
	{
		if (m_Tabs[i].m_rcTabRect.PtInRect(point))
		{
			ActiveTab(i);
			break;
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}


void CTabCtrlWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()

	Render();
}


void CTabCtrlWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	InitWnd();
}

void CTabCtrlWnd::InitWnd()
{
	GetClientRect(m_rcClient);

	if (m_pRender)
	{
		m_pRender->BeginRender();

		for(int i=0; i< m_Tabs.size(); i++)
		{
			CSize size = m_pRender->GetStringSize(_T("宋体"), 12, m_Tabs[i].m_sTabCaption);
			
			//TRACE("导航组件高度%d\n", size.cy);
			
			CRect rect;
			if (i==0)
				rect.left = 0;
			else
				rect.left = m_Tabs[i-1].m_rcTabRect.right + 1;

			rect.top = m_rcClient.top;
			rect.right = rect.left + size.cx + m_nTabSep;
			rect.bottom = m_rcClient.bottom;
			
			//rect.bottom = 20;

			m_Tabs[i].m_rcTabRect= rect;
			TRACE("InitWnd 矩形left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
		}

		m_pRender->EndRender();
	}
}

void CTabCtrlWnd::Render()
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

		for (int i=0; i<m_Tabs.size(); i++)
		{
			if (i == m_nActiveTab)
			{
				CRect& rect = m_Tabs[i].m_rcTabRect;
				TRACE("Render 矩形left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
				CString& s = m_Tabs[i].m_sTabCaption;
				m_pRender->FillRectangle(rect, clrBgActive);
				m_pRender->DrawText(rect, s, clrCaptionActive);
			}
			else
			{
				CRect& rect = m_Tabs[i].m_rcTabRect;
				TRACE("Render 矩形left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
				CString& s = m_Tabs[i].m_sTabCaption;
				m_pRender->FillRectangle(rect, clrBgInactive);
				m_pRender->DrawText(rect, s, clrCaptionInactive);
			}
		}

		m_pRender->EndRender();
	}
}

void CTabCtrlWnd::InitTab()
{
	CTabInfo tabZxg;
	tabZxg.m_sTabCaption = _T("自选");
	tabZxg.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabZxg);

	CTabInfo tabIndex;
	tabIndex.m_sTabCaption = _T("指数");
	tabIndex.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabIndex);

	CTabInfo tabStock;
	tabStock.m_sTabCaption = _T("股票");
	tabStock.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabStock);

	CTabInfo tabBond;
	tabBond.m_sTabCaption = _T("债券");
	tabBond.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabBond);

	CTabInfo tabFund;
	tabFund.m_sTabCaption = _T("基金");
	tabFund.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabFund);

	CTabInfo tabArea;
	tabArea.m_sTabCaption = _T("地区");
	tabArea.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabArea);

	CTabInfo tabIndustry;
	tabIndustry.m_sTabCaption = _T("行业");
	tabIndustry.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabIndustry);

	CTabInfo tabConcept;
	tabConcept.m_sTabCaption = _T("概念");
	tabConcept.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabConcept);
}


void CTabCtrlWnd::ActiveTab(int nTab)
{
	// 如果有菜单，就弹出菜单
	// 如果
	// 相同页，无需激活
	if (nTab == m_nActiveTab)
		return;

	m_nActiveTab = nTab;
	Render();

	if (!m_Tabs[m_nActiveTab].m_bHasMenu)
	{
		// 没有弹出式菜单
		GetParent()->SendMessage(MSG_ACTIVE_TAB, (WPARAM)m_nActiveTab);
	}	
	else
	{
		CMenu menu;
		menu.CreatePopupMenu();

		menu.AppendMenu(MF_BYCOMMAND | MF_STRING, MSG_GRID_POPUP_MENU_BEGIN, _T("上证d"));
		menu.AppendMenu(MF_BYCOMMAND | MF_STRING | MF_MENUBARBREAK, MSG_GRID_POPUP_MENU_BEGIN+1, _T("上证c"));
		menu.AppendMenu(MF_BYCOMMAND | MF_STRING, MSG_GRID_POPUP_MENU_END, _T("上证d"));
		
		int x = m_Tabs[m_nActiveTab].m_rcTabRect.left;
		int y = m_Tabs[m_nActiveTab].m_rcTabRect.top;

		CPoint pt(x, y);
		ClientToScreen(&pt); //将客户区的坐标转化为屏幕的坐标  

		//GetCursorPos(pt);
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_VERNEGANIMATION, pt.x, pt.y, this);
		//menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_VERNEGANIMATION | TPM_RETURNCMD, pt.x, pt.y, this);

	}
}

void CTabCtrlWnd::OnPopupMenu(UINT nId)
{
	switch (nId)
	{
	case MSG_GRID_POPUP_MENU_BEGIN:
		break;
	case MSG_GRID_POPUP_MENU_END:
		break;
	}
}
void CTabCtrlWnd::OnUpdatePopupMenu(CCmdUI* pCCmdUI)
{
TRACE("");
}


BOOL CTabCtrlWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::OnCommand(wParam, lParam);
}

void CTabCtrlWnd::OnTest()
{
	TRACE("");
}
