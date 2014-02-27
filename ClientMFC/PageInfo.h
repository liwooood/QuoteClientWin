#pragma once
#include "PageWnd.h"

class CPageInfo
{
public:
	CPageInfo(void);
	~CPageInfo(void);

public:
	CString m_sPageCaption;
	CString m_sPageFile;
	CPageWnd * m_pPageWnd;

};

