#pragma once

#include <vector>

#include "IRenderEngine.h"
#include "GDIRenderEngine.h"

class CPageNavWnd : public CWnd
{
	DECLARE_DYNAMIC(CPageNavWnd)

public:
	CPageNavWnd();
	virtual ~CPageNavWnd();

protected:
	DECLARE_MESSAGE_MAP()

private:
	IRenderEngine * m_pRender;
	CRect m_rcClient;

	std::vector<CString> m_vTabCaption;
	std::vector<CRect> m_vTabRect;
	
	// 当前活动页序号
	int m_nActivePage;
	// 列间隔
	int m_nColSep;

public:
	
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();

	void AddPage(CString sPageCaption);
	
	// 增加新的空白页面
	void AddNewPage();
	
	// 激活某一页面
	void ActivePage(int nPage);

	// 如果点了标签的关闭区域
	void ClosePage();

	// 构造界面元素
	// 相当于CDialog::InitDialog, CView::InitUpdate
	void InitWnd();

	// 渲染界面
	void Render();

	
};


