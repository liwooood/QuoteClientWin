// BaseWnd.cpp : ʵ���ļ�
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



// CBaseWnd ��Ϣ�������


void CBaseWnd::PrintWindowInfo()
{
	TRACE("����id=%d, ����λ��=%d,  ��������=%d, ��Ȱٷֱ�=%f, �߶Ȱٷֱ�=%f\n", m_nId, m_nPosition,  m_nType, m_fWidth, m_fHeight);
}

int CBaseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_pRender = new GDIRenderEngine(m_hWnd);
	m_pRender->Initialize();

	return 0;
}


void CBaseWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
}


void CBaseWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pRender)
	{
		delete m_pRender ;
		m_pRender = NULL;
	}
}
