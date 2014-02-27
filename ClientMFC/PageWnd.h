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

	// ҳ����
	CString m_sPageCaption;
	
	// ҳ���沼�ֶ����ļ�
	CString m_sPageXMLFile;

	// ��ǰ����
	CBaseWnd * m_pCurrentWnd;

	CRect m_rcClient;

	std::vector<CBaseWnd*> m_ChildWnds;

	// �ָ��ߵĿ��
	int m_nSepWidth;
	int m_nSepHeight;
	// �ָ��ߵ���ɫ

	std::map<int, CBaseWnd*> m_mapWnds;
	tree<int> m_treeWnds;

public:
	CString GetPageCaption() {return m_sPageCaption;};

	

	// �ӳ־û��ļ��м���ҳ��
	BOOL OpenPage(CString sPageXMLFile);
	void ReadNode(rapidxml::xml_node<> * parent_node);
	void ReadNodeAttribute(rapidxml::xml_node<> * node);
	void AddToParent(rapidxml::xml_node<> * node, CBaseWnd* pWnd);
	int GetType(std::string sType);


	// �־û�ҳ��
	BOOL SavePage();

	// ����ҳ��
	void ActivePage();

	// ������ҳ��
	void InactivePage();

	void OutputWindowTree(); // �������ڲ���

	// �������󣬻�ȡ����
	void ShowQuote();
	
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


