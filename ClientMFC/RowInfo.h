#pragma once
class CRowInfo
{
public:
	CRowInfo(void);
	~CRowInfo(void);

	// ���о������򣬰����зָ���
	// ��Ҫ���ָ���
	// ��Ҫ���������ɫ
	// ��Ҫ��ǰ��ɫ
	CRect m_rcRow;

	CRect m_rcRowSep;

	CString m_sFinanceCode;
};

