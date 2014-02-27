#pragma once

#include <map>
#include <vector>
#include <string>

#include "BaseControlWnd.h"
#include "IRenderEngine.h"

#include "RowInfo.h"
#include "ColInfo.h"
#include "TabCtrlWnd.h"


#define TABLE_COL_ID 0 
#define TABLE_COL_CODE 1
#define TABLE_COL_NAME 2
#define TABLE_COL_XJ 3
#define TABLE_COL_ZF 4
#define TABLE_COL_ZD 5
#define TABLE_COL_BID1 6
#define TABLE_COL_ASK1 7
#define TABLE_COL_XL 8
#define TABLE_COL_CJSL 9
#define TABLE_COL_HS 10
#define TABLE_COL_OPEN 11
#define TABLE_COL_HIGH 12
#define TABLE_COL_LOW 13
#define TABLE_COL_CLOSE 14
#define TABLE_COL_CJJE 15
#define TABLE_COL_ZHENFU 16
#define TABLE_COL_LB 17
#define TABLE_COL_WB 18
#define TABLE_COL_WC 19
  
// 自定义消息


class CGridWnd : public CBaseControlWnd
{
	DECLARE_DYNAMIC(CGridWnd)

public:
	CGridWnd();
	virtual ~CGridWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void ShowQuote(CFinanceProduct& financeProduct);
	virtual void RecvResponseCallback(tcp_message * response);

private:
	void InitWnd();
	void Render();
	

private:
	// 行相关变量定义=======================================================
	// 行高度
	int m_nRowHeight;

	// 行间隔，用于分隔行，以及画底部的线
	int m_nRowSepHeight;

	// 底部控件的高度
	int m_nBottomCtrlHeight;

	int m_nRows;

	// 行序号
	int m_nRowId;

	// 动态可变的行数
	// int代表行序号，高度不够就erase后面的行数
	std::map<int, CRowInfo> m_Rows;
	
	// 行相关变量定义=======================================================

	// 列相关变量定义============================================================
	// 列标题数，所有列
	// 列的排列顺序有可能改变
	std::map<int, CColInfo> m_ColHeader; // 列信息字典表
	std::vector<CColInfo> m_Cols;

	// 固定的列数
	int m_nFixedCols;

	// 固定列所占的宽度
	int m_nFixedColsWidth;

	// 可见范围内的列信息, 列名字
	std::vector<int> m_ColDisplay;

	// 列名,以及对应的矩形
	typedef std::map<int, CColInfo> ROW_TYPE;

	// 行序号和m_Rows同样，
	std::map<int, ROW_TYPE> m_Grid;
	// 列相关变量定义=========================================================

	// 数据
	// 用于判断数据是否有变化，如有变化才需要更新对应列的区域
	//std::map<std::string, GRID_STRUCT_TYPE> m_Data;
	quote::GridResponse m_Data;
	std::map<std::string, quote::Grid> m_AllData;

	// 板块选择组件
	CRect m_rcTabCtrl;
	CTabCtrlWnd m_wndTabCtrl;

	// 滚动条组件,自定义类或者CScrollCtrl

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void SetFixedCols(int nFixedCols) {m_nFixedCols = nFixedCols;};
	void AddCol(CColInfo colInfo);
	void LoadColumnMapXML();
	
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
