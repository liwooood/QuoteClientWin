#pragma once
class CRowInfo
{
public:
	CRowInfo(void);
	~CRowInfo(void);

	// 整行矩形区域，包含行分隔区
	// 需要画分隔线
	// 需要画间隔背景色
	// 需要画前景色
	CRect m_rcRow;

	CRect m_rcRowSep;

	CString m_sFinanceCode;
};

