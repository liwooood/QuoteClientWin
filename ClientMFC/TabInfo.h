#pragma once
class CTabInfo
{
public:
	CTabInfo(void);
	~CTabInfo(void);

public:
	CString m_sTabCaption;
	CRect m_rcTabRect;
	BOOL m_bHasMenu;
};

