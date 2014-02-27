// PageContainerWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "PageContainerWnd.h"

#include "importheader.h"

#include <gdiplus.h>
#include "GDIRenderEngine.h"
#include "Direct2DRenderEngine.h"
#include "tcp_message.h"
#include "DataManager.h"
#include "FileLog.h"
#include "ColorUtil.h"

#include "../xml/pugixml.hpp"
#include "cssweb_charset.h"
#include "user_defined_msg.h"

// CPageContainerWnd

IMPLEMENT_DYNAMIC(CPageContainerWnd, CWnd)

CPageContainerWnd::CPageContainerWnd()
{
	m_nPageNavWndHeight = 20;
}

CPageContainerWnd::~CPageContainerWnd()
{
}


BEGIN_MESSAGE_MAP(CPageContainerWnd, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_MESSAGE(MSG_ACTIVE_PAGE, OnActivePage)
END_MESSAGE_MAP()



// CPageContainerWnd 消息处理程序





void CPageContainerWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	//GetDC()

	if (m_pRender)
	{
		m_pRender->BeginRender();

		CColorUtil clr(255, 0, 0);
		
		CRect rcTmp = m_rcClient;
		//rcTmp.DeflateRect(1,1);
		m_pRender->DrawRectangle(rcTmp, clr);

		m_pRender->DrawText(m_rcClient, _T("页容器"), clr);

		m_pRender->EndRender();
	}

}




BOOL CPageContainerWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::PreCreateWindow(cs);

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	/*
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
	*/
}


BOOL CPageContainerWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CWnd::OnEraseBkgnd(pDC);
	//return TRUE;
}


int CPageContainerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
	//m_pRender = new Direct2DRenderEngine( m_hWnd );
	//m_pRender->Initialize();

	m_pRender = new GDIRenderEngine( m_hWnd );

	m_wndPageNav.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0, 0, 0), this, 1234);

	return 0;
}


void CPageContainerWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(m_rcClient);

	// 设置页导航头
	m_rcPageNavWnd.left = m_rcClient.left;
	m_rcPageNavWnd.top = m_rcClient.top;
	m_rcPageNavWnd.right = m_rcClient.right;
	m_rcPageNavWnd.bottom = m_nPageNavWndHeight;
	m_wndPageNav.MoveWindow(m_rcPageNavWnd);

	// 设置页面
	m_rcPageWnd.left = m_rcClient.left;
	m_rcPageWnd.top = m_rcPageNavWnd.bottom;
	m_rcPageWnd.right = m_rcClient.right;
	m_rcPageWnd.bottom = m_rcClient.bottom;

//	std::map<CString, CPageWnd*>::iterator it = m_Pages.begin();
//	for (; it != m_Pages.end(); it++)
//	{
//		CPageWnd * pPageWnd = it->second;
//		//pPageWnd->SetWindowPos();
//		pPageWnd->MoveWindow(m_rcClient, TRUE);
//	}
}

/*
装载页面
*/
void CPageContainerWnd::LoadPage(CString sPageXMLFile)
{
	pugi::xml_document doc;

	if (!doc.load_file(sPageXMLFile))
	{
		return;
	}


	pugi::xpath_node_set nodes = doc.select_nodes("/pages/page");
	
	

	for (pugi::xpath_node_set::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		pugi::xpath_node node = *it;

		
		CPageInfo pageInfo;

		std::string title = node.node().child_value("title");
		std::string caption = Utf8ToGBK(title);
		//USES_CONVERSION;
		//CString temp = A2W(title);

		std::string xmlfile = node.node().child_value("xmlfile");
		
		pageInfo.m_sPageCaption = caption.c_str();
		pageInfo.m_sPageFile = xmlfile.c_str();
		pageInfo.m_pPageWnd = NULL;

		m_vPages.push_back(pageInfo);		
	}
	
	int size = m_vPages.size();

	for (std::vector<CPageInfo>::iterator it = m_vPages.begin(); it != m_vPages.end(); it++)
	{
		CPageInfo& page = *it;
		
		m_wndPageNav.AddPage(page.m_sPageCaption);
	}
	
	// 默认打开第一个页面
	OpenPage();
}


BOOL CPageContainerWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CPageContainerWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = NULL;
	}
}



LRESULT CPageContainerWnd::OnActivePage(WPARAM wParam, LPARAM lParam)
{
	TRACE("激活页面%d\n", wParam);

	int nPageIndex = wParam;
	int size = m_vPages.size();

	// 前一个页面休眠
	InactivePage();

	if (m_vPages[nPageIndex].m_pPageWnd == NULL)
	{
		OpenPage(nPageIndex);
	}
	else
	{
		ActivePage(nPageIndex);
	}


	return 0;
}

void CPageContainerWnd::OpenPage(int nPageIndex)
{
	int nPages = m_vPages.size();

	if (nPages < 1)
		return;

	if (nPageIndex >= nPages)
		return;


	CPageWnd * pPageWnd = new CPageWnd();

	m_rcPageWnd.left = m_rcClient.left;
	m_rcPageWnd.top = m_rcPageNavWnd.bottom;
	m_rcPageWnd.right = m_rcClient.right;
	m_rcPageWnd.bottom = m_rcClient.bottom;


	pPageWnd->Create(NULL, _T("PAGEWND"), WS_CHILD|WS_VISIBLE, m_rcPageWnd, this, 5678, NULL);

	


	if (!pPageWnd->OpenPage(m_vPages[nPageIndex].m_sPageFile))
	{
		delete pPageWnd;

		AfxMessageBox(_T("装入页面失败"));
		return;
	}

	m_vPages[nPageIndex].m_pPageWnd = pPageWnd;
	

	pPageWnd->ShowWindow(SW_SHOW);
	pPageWnd->UpdateWindow();

	// 重新设置当前活动页
	m_nCurrentActivePage = nPageIndex;

	// 发请求，获取数据，并展现
	pPageWnd->ShowQuote();

}

void CPageContainerWnd::ActivePage(int nPageIndex)
{
	// 
	m_vPages[nPageIndex].m_pPageWnd->ActivePage();

	// 显示窗口
	m_vPages[nPageIndex].m_pPageWnd->ShowWindow(SW_SHOW);
	m_vPages[nPageIndex].m_pPageWnd->UpdateWindow();

	// 重新设置当前活动页
	m_nCurrentActivePage = nPageIndex;

}

void CPageContainerWnd::InactivePage()
{
	// 不再发送和接收请求
	m_vPages[m_nCurrentActivePage].m_pPageWnd->InactivePage();

	// 隐藏窗口
	m_vPages[m_nCurrentActivePage].m_pPageWnd->ShowWindow(SW_HIDE);
	
}
