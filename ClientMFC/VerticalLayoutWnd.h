#pragma once
#include "BaseWnd.h"
#include "BaseContainerWnd.h"

// CVerticalLayoutWnd

class CVerticalLayoutWnd : public CBaseContainerWnd
{
	DECLARE_DYNAMIC(CVerticalLayoutWnd)

public:
	CVerticalLayoutWnd();
	virtual ~CVerticalLayoutWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:

public:
	
	afx_msg void OnPaint();
};


