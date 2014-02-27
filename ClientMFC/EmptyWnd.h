#pragma once
#include "BaseControlWnd.h"

// CEmptyWnd

class CEmptyWnd : public CBaseControlWnd
{
	DECLARE_DYNAMIC(CEmptyWnd)

public:
	CEmptyWnd();
	virtual ~CEmptyWnd();

protected:
	DECLARE_MESSAGE_MAP()
};


