#pragma once
#include <string>

class CColInfo
{
public:
	CColInfo(void);
	~CColInfo(void);

public:
	// ������
	//std::string m_sColName; 
	int m_nColId;

	// �б���
	CString m_sColCaption;

	// �п��
	int m_nColWidth;

	CRect m_rcCol;
};

