// BaseContainerWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "BaseContainerWnd.h"


// CBaseContainerWnd

IMPLEMENT_DYNAMIC(CBaseContainerWnd, CBaseWnd)

CBaseContainerWnd::CBaseContainerWnd()
{

}

CBaseContainerWnd::~CBaseContainerWnd()
{
}


BEGIN_MESSAGE_MAP(CBaseContainerWnd, CBaseWnd)
END_MESSAGE_MAP()



// CBaseContainerWnd ��Ϣ�������


void CBaseContainerWnd::PrintWindowInfo()
{
	TRACE("��������id=%d, ����λ��=%d, �Ӵ���1=%d, �Ӵ���2=%d, ��������=%d, ��Ȱٷֱ�=%f, �߶Ȱٷֱ�=%f\n", m_nId,m_nPosition, m_pFirst->GetId(), m_pSecond->GetId(), m_nType, m_fWidth, m_fHeight);
}
