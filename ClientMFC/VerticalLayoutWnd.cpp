// VerticalLayoutWnd.cpp : ʵ���ļ�
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



// CVerticalLayoutWnd ��Ϣ�������




void CVerticalLayoutWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CBaseContainerWnd::OnPaint()

	GetClientRect(m_rcClient);

	if (m_pRender)
	{
		m_pRender->BeginRender();

		CColorUtil clr(0,0,255);

		m_pRender->DrawRectangle(m_rcClient, clr);
		//m_pRender->DrawText(m_rcClient, _T("��ֱ����"), clr);

		m_pRender->EndRender();
	}
}
