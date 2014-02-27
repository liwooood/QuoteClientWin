#pragma once

#include <vector>
#include <map>

#include "BaseWnd.h"
#include "BaseContainerWnd.h"
#include "IRenderEngine.h"
#include "GDIRenderEngine.h"

// xml
#include "../xml/rapidxml.hpp"
#include "../xml/rapidxml_utils.hpp"

#include "tree_structure\tree.hh"


class CPageWnd : public CWnd
{
	DECLARE_DYNAMIC(CPageWnd)

public:
	CPageWnd();
	virtual ~CPageWnd();

protected:
	DECLARE_MESSAGE_MAP()



private:
	IRenderEngine * m_pRender;

	// 页标题
	CString m_sPageCaption;
	
	// 页界面布局定义文件
	CString m_sPageXMLFile;

	// 当前活动组件
	CBaseWnd * m_pCurrentWnd;

	CRect m_rcClient;

	std::vector<CBaseWnd*> m_ChildWnds;

	// 分隔线的宽度
	int m_nSepWidth;
	int m_nSepHeight;
	// 分隔线的颜色

	std::map<int, CBaseWnd*> m_mapWnds;
	tree<int> m_treeWnds;

public:
	CString GetPageCaption() {return m_sPageCaption;};

	

	// 从持久化文件中加载页面
	BOOL OpenPage(CString sPageXMLFile);
	void ReadNode(rapidxml::xml_node<> * parent_node);
	void ReadNodeAttribute(rapidxml::xml_node<> * node);
	void AddToParent(rapidxml::xml_node<> * node, CBaseWnd* pWnd);
	int GetType(std::string sType);


	// 持久化页面
	BOOL SavePage();

	// 激活页面
	void ActivePage();

	// 不激活页面
	void InactivePage();

	void OutputWindowTree(); // 仅仅用于测试

	// 发送请求，获取数据
	void ShowQuote();
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


