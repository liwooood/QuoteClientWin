#pragma once

#include <map>

#include "BaseControlWnd.h"
#include "IRenderEngine.h"


#pragma pack(1) 
struct detail_struct
{
	int id;//记录的累计ID
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
	// 高度
	int m_nHeight;
	int m_nWidth;

	// 行高
	int m_nRowHeight;
	// 行间隔高度
	int m_nRowSepHeight;

	// 行数
	int m_nRows;
	int m_nCols;

	void Render();
	
	// 当前id
	int m_nId;

	// 明细数据
	std::vector<DETAIL_STRUCT_TYPE> m_Data;
	// 二维数组表格
	std::vector<std::vector<CRect> > m_Table;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


