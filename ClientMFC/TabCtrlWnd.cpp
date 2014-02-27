// TabCtrlWnd.cpp : ʵ���ļ�
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



// CTabCtrlWnd ��Ϣ�������




int CTabCtrlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_pRender = new GDIRenderEngine(m_hWnd);

	m_nActiveTab = 0;
	m_nTabSep = 4;

	InitTab();

	return 0;
}


void CTabCtrlWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = NULL;
	}
}


void CTabCtrlWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

	Render();
}


void CTabCtrlWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
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
			CSize size = m_pRender->GetStringSize(_T("����"), 12, m_Tabs[i].m_sTabCaption);
			
			//TRACE("��������߶�%d\n", size.cy);
			
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
			TRACE("InitWnd ����left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
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
				TRACE("Render ����left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
				CString& s = m_Tabs[i].m_sTabCaption;
				m_pRender->FillRectangle(rect, clrBgActive);
				m_pRender->DrawText(rect, s, clrCaptionActive);
			}
			else
			{
				CRect& rect = m_Tabs[i].m_rcTabRect;
				TRACE("Render ����left %d, top %d, right %d, bottom %d\n", rect.left, rect.top, rect.right, rect.bottom);
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
	tabZxg.m_sTabCaption = _T("��ѡ");
	tabZxg.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabZxg);

	CTabInfo tabIndex;
	tabIndex.m_sTabCaption = _T("ָ��");
	tabIndex.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabIndex);

	CTabInfo tabStock;
	tabStock.m_sTabCaption = _T("��Ʊ");
	tabStock.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabStock);

	CTabInfo tabBond;
	tabBond.m_sTabCaption = _T("ծȯ");
	tabBond.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabBond);

	CTabInfo tabFund;
	tabFund.m_sTabCaption = _T("����");
	tabFund.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabFund);

	CTabInfo tabArea;
	tabArea.m_sTabCaption = _T("����");
	tabArea.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabArea);

	CTabInfo tabIndustry;
	tabIndustry.m_sTabCaption = _T("��ҵ");
	tabIndustry.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabIndustry);

	CTabInfo tabConcept;
	tabConcept.m_sTabCaption = _T("����");
	tabConcept.m_bHasMenu = TRUE;
	m_Tabs.push_back(tabConcept);
}


void CTabCtrlWnd::ActiveTab(int nTab)
{
	// ����в˵����͵����˵�
	// ���
	// ��ͬҳ�����輤��
	if (nTab == m_nActiveTab)
		return;

	m_nActiveTab = nTab;
	Render();

	if (!m_Tabs[m_nActiveTab].m_bHasMenu)
	{
		// û�е���ʽ�˵�
		GetParent()->SendMessage(MSG_ACTIVE_TAB, (WPARAM)m_nActiveTab);
	}	
	else
	{
		CMenu menu;
		menu.CreatePopupMenu();

		menu.AppendMenu(MF_BYCOMMAND | MF_STRING, MSG_GRID_POPUP_MENU_BEGIN, _T("��֤d"));
		menu.AppendMenu(MF_BYCOMMAND | MF_STRING | MF_MENUBARBREAK, MSG_GRID_POPUP_MENU_BEGIN+1, _T("��֤c"));
		menu.AppendMenu(MF_BYCOMMAND | MF_STRING, MSG_GRID_POPUP_MENU_END, _T("��֤d"));
		
		int x = m_Tabs[m_nActiveTab].m_rcTabRect.left;
		int y = m_Tabs[m_nActiveTab].m_rcTabRect.top;

		CPoint pt(x, y);
		ClientToScreen(&pt); //���ͻ���������ת��Ϊ��Ļ������  

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
	// TODO: �ڴ����ר�ô����/����û���

	return CWnd::OnCommand(wParam, lParam);
}

void CTabCtrlWnd::OnTest()
{
	TRACE("");
}
