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
  
// �Զ�����Ϣ


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
	// ����ر�������=======================================================
	// �и߶�
	int m_nRowHeight;

	// �м�������ڷָ��У��Լ����ײ�����
	int m_nRowSepHeight;

	// �ײ��ؼ��ĸ߶�
	int m_nBottomCtrlHeight;

	int m_nRows;

	// �����
	int m_nRowId;

	// ��̬�ɱ������
	// int��������ţ��߶Ȳ�����erase���������
	std::map<int, CRowInfo> m_Rows;
	
	// ����ر�������=======================================================

	// ����ر�������============================================================
	// �б�������������
	// �е�����˳���п��ܸı�
	std::map<int, CColInfo> m_ColHeader; // ����Ϣ�ֵ��
	std::vector<CColInfo> m_Cols;

	// �̶�������
	int m_nFixedCols;

	// �̶�����ռ�Ŀ��
	int m_nFixedColsWidth;

	// �ɼ���Χ�ڵ�����Ϣ, ������
	std::vector<int> m_ColDisplay;

	// ����,�Լ���Ӧ�ľ���
	typedef std::map<int, CColInfo> ROW_TYPE;

	// ����ź�m_Rowsͬ����
	std::map<int, ROW_TYPE> m_Grid;
	// ����ر�������=========================================================

	// ����
	// �����ж������Ƿ��б仯�����б仯����Ҫ���¶�Ӧ�е�����
	//std::map<std::string, GRID_STRUCT_TYPE> m_Data;
	quote::GridResponse m_Data;
	std::map<std::string, quote::Grid> m_AllData;

	// ���ѡ�����
	CRect m_rcTabCtrl;
	CTabCtrlWnd m_wndTabCtrl;

	// ���������,�Զ��������CScrollCtrl

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
