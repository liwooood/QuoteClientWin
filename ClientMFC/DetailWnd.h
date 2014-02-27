#pragma once

#include <map>

#include "BaseControlWnd.h"
#include "IRenderEngine.h"


#pragma pack(1) 
struct detail_struct
{
	int id;//��¼���ۼ�ID
	long long quote_time;//HHMMSS
	float price;
	int xl;
	int xe;
	char nwp;
};
#pragma pack()
typedef struct detail_struct DETAIL_STRUCT_TYPE;

class CDetailWnd : public CBaseControlWnd
{
	DECLARE_DYNAMIC(CDetailWnd)

public:
	CDetailWnd();
	virtual ~CDetailWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void ShowQuote(CFinanceProduct& financeProduct);
	virtual void RecvResponseCallback(tcp_message * response);

	afx_msg void OnPaint();

private:
	// �߶�
	int m_nHeight;
	int m_nWidth;

	// �и�
	int m_nRowHeight;
	// �м���߶�
	int m_nRowSepHeight;

	// ����
	int m_nRows;
	int m_nCols;

	void Render();
	
	// ��ǰid
	int m_nId;

	// ��ϸ����
	std::vector<DETAIL_STRUCT_TYPE> m_Data;
	// ��ά������
	std::vector<std::vector<CRect> > m_Table;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


