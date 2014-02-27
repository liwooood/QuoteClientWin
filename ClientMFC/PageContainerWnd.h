#ifndef PAGE_CONTAINER_WND
#define PAGE_CONTAINER_WND


#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#include "IRenderEngine.h"
#include "FinanceProduct.h"
#include "tcp_message.h"
#include "BaseWnd.h"

#include "PageNavWnd.h"
#include "PageWnd.h"
#include "PageInfo.h"





class CPageContainerWnd : public CWnd
{
	DECLARE_DYNAMIC(CPageContainerWnd)

public:
	CPageContainerWnd();
	virtual ~CPageContainerWnd();

protected:
	DECLARE_MESSAGE_MAP()


	

public:
	
	afx_msg void OnPaint();

	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnActivePage(WPARAM wParam, LPARAM lParam);

private:
	CRect m_rcClient;

	CRect m_rcPageNavWnd;
	CPageNavWnd m_wndPageNav;
	
	
	CRect m_rcPageWnd;

	IRenderEngine * m_pRender;

	int m_nPageNavWndHeight;
	
	std::vector<CPageInfo> m_vPages;

	int m_nCurrentActivePage;
	int m_nPrevActivePage;

public:
	// 增加空页面，允许自定义
	void AddPage(); 

	// 装入指定页面
	void LoadPage(CString sPageXMLFile);

	// 打开页面，默认为第一页
	void OpenPage(int nPageIndex=0);

	// 激活页面
	void ActivePage(int nPageIndex);
	
	// 休眠页面
	void InactivePage();
	
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnDestroy();
};


#endif
