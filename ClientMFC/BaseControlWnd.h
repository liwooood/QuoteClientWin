#pragma once

#include "BaseWnd.h"
#include "BaseContainerWnd.h"

#include "FinanceProduct.h"
#include "tcp_message.h"


// 树叶

class CBaseControlWnd : public CBaseWnd
{
	DECLARE_DYNAMIC(CBaseControlWnd)

public:
	CBaseControlWnd();
	virtual ~CBaseControlWnd();

protected:
	DECLARE_MESSAGE_MAP()


public:
	virtual void ShowQuote(CFinanceProduct& financeProduct) = 0;
	virtual void RecvResponseCallback(tcp_message * response) = 0;

private:
	// 树叶，一定属于树节点
	CBaseContainerWnd m_wndParent;
};


