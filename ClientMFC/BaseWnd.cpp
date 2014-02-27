// BaseWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "BaseWnd.h"


// CBaseWnd

IMPLEMENT_DYNAMIC(CBaseWnd, CWnd)

CBaseWnd::CBaseWnd()
{

}

CBaseWnd::~CBaseWnd()
{
}


BEGIN_MESSAGE_MAP(CBaseWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CBaseWnd 消息处理程序


void CBaseWnd::PrintWindowInfo()
{
	TRACE("窗口id=%d, 窗口位置=%d,  窗口类型=%d, 宽度百分比=%f, 高度百分比=%f\n", m_nId, m_nPosition,  m_nType, m_fWidth, m_fHeight);
}

int CBaseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pRender = new GDIRenderEngine(m_hWnd);
	m_pRender->Initialize();

	return 0;
}


void CBaseWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
}


void CBaseWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (m_pRender)
	{
		delete m_pRender ;
		m_pRender = NULL;
	}
}
