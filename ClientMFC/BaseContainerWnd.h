#pragma once

#include "BaseWnd.h"

// ���ڵ�

class CBaseContainerWnd : public CBaseWnd
{
	DECLARE_DYNAMIC(CBaseContainerWnd)

public:
	CBaseContainerWnd();
	virtual ~CBaseContainerWnd();

protected:
	DECLARE_MESSAGE_MAP()

protected:
		// �п�����CBaseControlWnd��������
	// �п�����CBaseContainerWnd��������

	CBaseWnd * m_pFirst;
	CBaseWnd * m_pSecond;

public:
	void SetFirstChildWnd(CBaseWnd* pWnd){m_pFirst = pWnd;};
	void SetSecondChildWnd(CBaseWnd* pWnd){m_pSecond = pWnd;};
	virtual void PrintWindowInfo();
};


