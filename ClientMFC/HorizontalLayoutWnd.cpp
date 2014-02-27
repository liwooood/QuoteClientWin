// HorizontalLayoutWnd.cpp : ʵ���ļ�
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



// CHorizontalLayoutWnd ��Ϣ�������




void CHorizontalLayoutWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CBaseContainerWnd::OnPaint()
	GetClientRect(m_rcClient);

	if (m_pRender)
	{
		m_pRender->BeginRender();

		CColorUtil clr(255,0,0);

		m_pRender->DrawRectangle(m_rcClient, clr);
		//m_pRender->DrawText(m_rcClient, _T("ˮƽ����"), clr);

		m_pRender->EndRender();
	}
}
