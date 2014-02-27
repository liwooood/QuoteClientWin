#pragma once

#include <vector>
#include <map>

#include "IRenderEngine.h"
#include "GDIRenderEngine.h"
#include "TabInfo.h"

class CTabCtrlWnd : public CWnd
{
	DECLARE_DYNAMIC(CTabCtrlWnd)

public:
	CTabCtrlWnd();
	virtual ~CTabCtrlWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnPopupMenu(UINT nId);
	afx_msg void OnUpdatePopupMenu(CCmdUI* pCCmdUI);
	afx_msg void OnTest();

private:
	IRenderEngine * m_pRender;
	CRect m_rcClient;

	
	// 当前活动页序号
	int m_nActiveTab;
	// 标签水平间隔
	int m_nTabSep;

	//std::map<std::string, CTabInfo> m_Tabs;
	std::vector<CTabInfo> m_Tabs;

private:
	// 构造界面元素
	// 相当于CDialog::InitDialog, CView::InitUpdate
	void InitWnd();

	void Render();

public:
	void InitTab();
	
	
	// 激活某一页面
	void ActiveTab(int nTab);




	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


