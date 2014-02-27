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
	
	// ��ǰ�ҳ���
	int m_nActivePage;
	// �м��
	int m_nColSep;

public:
	
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();

	void AddPage(CString sPageCaption);
	
	// �����µĿհ�ҳ��
	void AddNewPage();
	
	// ����ĳһҳ��
	void ActivePage(int nPage);

	// ������˱�ǩ�Ĺر�����
	void ClosePage();

	// �������Ԫ��
	// �൱��CDialog::InitDialog, CView::InitUpdate
	void InitWnd();

	// ��Ⱦ����
	void Render();

	
};


