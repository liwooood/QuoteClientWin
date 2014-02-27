// VerticalLayoutWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "VerticalLayoutWnd.h"


// CVerticalLayoutWnd

IMPLEMENT_DYNAMIC(CVerticalLayoutWnd, CBaseContainerWnd)

CVerticalLayoutWnd::CVerticalLayoutWnd()
{

}

CVerticalLayoutWnd::~CVerticalLayoutWnd()
{
}


BEGIN_MESSAGE_MAP(CVerticalLayoutWnd, CBaseContainerWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CVerticalLayoutWnd 消息处理程序




void CVerticalLayoutWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseContainerWnd::OnPaint()

	GetClientRect(m_rcClient);

	if (m_pRender)
	{
		m_pRender->BeginRender();

		CColorUtil clr(0,0,255);

		m_pRender->DrawRectangle(m_rcClient, clr);
		//m_pRender->DrawText(m_rcClient, _T("垂直容器"), clr);

		m_pRender->EndRender();
	}
}
