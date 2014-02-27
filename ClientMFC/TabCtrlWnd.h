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

	
	// ��ǰ�ҳ���
	int m_nActiveTab;
	// ��ǩˮƽ���
	int m_nTabSep;

	//std::map<std::string, CTabInfo> m_Tabs;
	std::vector<CTabInfo> m_Tabs;

private:
	// �������Ԫ��
	// �൱��CDialog::InitDialog, CView::InitUpdate
	void InitWnd();

	void Render();

public:
	void InitTab();
	
	
	// ����ĳһҳ��
	void ActiveTab(int nTab);




	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


