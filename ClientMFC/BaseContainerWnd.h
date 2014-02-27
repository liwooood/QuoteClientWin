#pragma once

#include "BaseWnd.h"

// 树节点

class CBaseContainerWnd : public CBaseWnd
{
	DECLARE_DYNAMIC(CBaseContainerWnd)

public:
	CBaseContainerWnd();
	virtual ~CBaseContainerWnd();

protected:
	DECLARE_MESSAGE_MAP()

protected:
		// 有可能是CBaseControlWnd的派生类
	// 有可能是CBaseContainerWnd的派生类

	CBaseWnd * m_pFirst;
	CBaseWnd * m_pSecond;

public:
	void SetFirstChildWnd(CBaseWnd* pWnd){m_pFirst = pWnd;};
	void SetSecondChildWnd(CBaseWnd* pWnd){m_pSecond = pWnd;};
	virtual void PrintWindowInfo();
};


