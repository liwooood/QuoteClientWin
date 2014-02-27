#pragma once
#include <string>

class CColInfo
{
public:
	CColInfo(void);
	~CColInfo(void);

public:
	// 列名字
	//std::string m_sColName; 
	int m_nColId;

	// 列标题
	CString m_sColCaption;

	// 列宽度
	int m_nColWidth;

	CRect m_rcCol;
};

