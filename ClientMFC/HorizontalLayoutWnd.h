#pragma once
#include "BaseWnd.h"
#include "BaseContainerWnd.h"



class CHorizontalLayoutWnd : public CBaseContainerWnd
{
	DECLARE_DYNAMIC(CHorizontalLayoutWnd)

public:
	CHorizontalLayoutWnd();
	virtual ~CHorizontalLayoutWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	

	afx_msg void OnPaint();
};


