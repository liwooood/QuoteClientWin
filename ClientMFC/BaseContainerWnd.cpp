// BaseContainerWnd.cpp : 实现文件
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



// CBaseContainerWnd 消息处理程序


void CBaseContainerWnd::PrintWindowInfo()
{
	TRACE("容器窗口id=%d, 窗口位置=%d, 子窗口1=%d, 子窗口2=%d, 窗口类型=%d, 宽度百分比=%f, 高度百分比=%f\n", m_nId,m_nPosition, m_pFirst->GetId(), m_pSecond->GetId(), m_nType, m_fWidth, m_fHeight);
}
