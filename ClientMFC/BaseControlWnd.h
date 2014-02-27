#pragma once

#include "BaseWnd.h"
#include "BaseContainerWnd.h"

#include "FinanceProduct.h"
#include "tcp_message.h"


// ��Ҷ

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
	// ��Ҷ��һ���������ڵ�
	CBaseContainerWnd m_wndParent;
};


