// HorizontalLayoutWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "BaseWnd.h"
#include "HorizontalLayoutWnd.h"


// CHorizontalLayoutWnd

IMPLEMENT_DYNAMIC(CHorizontalLayoutWnd, CBaseContainerWnd)

CHorizontalLayoutWnd::CHorizontalLayoutWnd()
{

}

CHorizontalLayoutWnd::~CHorizontalLayoutWnd()
{
}


BEGIN_MESSAGE_MAP(CHorizontalLayoutWnd, CBaseContainerWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CHorizontalLayoutWnd 消息处理程序




void CHorizontalLayoutWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseContainerWnd::OnPaint()
	GetClientRect(m_rcClient);

	if (m_pRender)
	{
		m_pRender->BeginRender();

		CColorUtil clr(255,0,0);

		m_pRender->DrawRectangle(m_rcClient, clr);
		//m_pRender->DrawText(m_rcClient, _T("水平容器"), clr);

		m_pRender->EndRender();
	}
}
