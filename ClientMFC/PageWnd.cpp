// PageWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "PageWnd.h"

#include "market.h"

// �������
#include "BaseContainerWnd.h"
#include "VerticalLayoutWnd.h"
#include "HorizontalLayoutWnd.h"

// �������
#include "BaseControlWnd.h"
#include "GridWnd.h"
#include "TickChartWnd.h"
#include "KLineChartWnd.h"
#include "RealtimeWnd.h"
#include "DetailWnd.h"

#include "boost/lexical_cast.hpp"
// CPageWnd

IMPLEMENT_DYNAMIC(CPageWnd, CWnd)

CPageWnd::CPageWnd()
{

}

CPageWnd::~CPageWnd()
{
}


BEGIN_MESSAGE_MAP(CPageWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPageWnd ��Ϣ�������


BOOL CPageWnd::OpenPage(CString sPageXMLFile)
{
	USES_CONVERSION;

	std::string file = T2A(sPageXMLFile);


	


	rapidxml::file<> f(file.c_str());

	rapidxml::xml_document<> doc;

	doc.parse<0>(f.data());

	rapidxml::xml_node<> * root = doc.first_node();
	TRACE("root %s=%s\n", root->name(), root->value());
		
	//rapidxml::xml_node<> * node = root->first_node("container");
	//TRACE("node %s=%s\n", node->name(), node->value());

	ReadNode(root);

	OutputWindowTree();

	tree<int>::iterator itTree = m_treeWnds.begin();
	for (; itTree != m_treeWnds.end(); itTree++)
	{
		int nId = *itTree;

		CBaseWnd * pWnd = m_mapWnds[nId];

		if (nId == 1)
		{
			// ������
			int nParentId = pWnd->GetParentWndId();
			pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_rcClient, this, pWnd->GetId());
		}
		else
		{
			int nParentId = pWnd->GetParentWndId();
			CBaseWnd * pParentWnd = m_mapWnds[nParentId];

			CRect rcParent(0,0,0,0);
			pParentWnd->GetClientRect(rcParent);

			CRect rcSelf = rcParent;
			if (pParentWnd->GetType() == CONTAINER_VERTICAL)
			{
				if (pWnd->GetPosition() == WND_POSITION_FIRST)
				{
					rcSelf.bottom = rcParent.Height() * pWnd->GetHeight();
				}
				else
				{
					rcSelf.top = rcParent.Height() - rcParent.Height() * pWnd->GetHeight();
				}
			}
			else
			{
				// ˮƽ����
				if (pWnd->GetPosition() == WND_POSITION_FIRST)
				{
					rcSelf.right = rcParent.Width() * pWnd->GetWidth();
				}
				else
				{
					rcSelf.left = rcParent.Width() - rcParent.Width() * pWnd->GetWidth();
				}
			}

			pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rcSelf, pParentWnd, pWnd->GetId());
		}
	} // end tree for

	return TRUE;
}

BOOL CPageWnd::SavePage()
{
	return TRUE;
}

void CPageWnd::ReadNode(rapidxml::xml_node<> * parent_node)
{
	for(rapidxml::xml_node<char> * node = parent_node->first_node(); node != NULL; node = node->next_sibling())
	{  
		if (strcmp(node->name(), "title") == 0)
		{
			// ����ҳ�����
			this->m_sPageCaption = node->value();
			continue;
		}

		TRACE("�ڵ����� %s=%s\n", node->name(), node->value());
		
		ReadNodeAttribute(node);
		
		TRACE("���ڵ����� %s\n", node->parent()->name());

		TRACE("\n");

		

		if (strcmp(node->name(), "view") == 0)
		{
		}

		if (strcmp(node->name(),"container") == 0)
		{
			ReadNode(node);
		}

		
	}  
}

/*
�������Թ�������
*/
void CPageWnd::ReadNodeAttribute(rapidxml::xml_node<> * node)
{
	int nId = -1;
	int nType = WND_UNKNOWN;
	
	int nPosition = -1;
	float fWidth = 0;
	float fHeight = 0;

	std::string name;
	std::string value;
	for(rapidxml::xml_attribute<char> * attr = node->first_attribute(); attr != NULL; attr = attr->next_attribute())  
	{  
		name = attr->name();
		value = attr->value();
		TRACE("����%s=%s\n", name.c_str(), value.c_str());
		
		if (name == "id")
			nId = boost::lexical_cast<int>(value);

		if (name == "type")
			nType = GetType(value);

		if (name == "position")
			nPosition = boost::lexical_cast<int>(value);

		if (name == "width")
			fWidth = boost::lexical_cast<float>(value);

		if (name == "height")
			fHeight = boost::lexical_cast<float>(value);

	}

	CBaseWnd* pWnd = NULL;
	switch (nType)
	{
	case CONTAINER_VERTICAL:
		pWnd = new CVerticalLayoutWnd();
		nType = CONTAINER_VERTICAL;
		break;
	case CONTAINER_HORIZONTAL:
		pWnd = new CHorizontalLayoutWnd();
		nType = CONTAINER_HORIZONTAL;
		break;
	case VIEW_TABLE:
		pWnd = new CGridWnd();
		nType = VIEW_TABLE;
		break;
	case VIEW_KLINE:
		pWnd = new CKLineChartWnd();
		nType = VIEW_KLINE;
		break;
	case VIEW_TICK:
		pWnd = new CTickChartWnd();
		nType = VIEW_TICK;
		break;
	case VIEW_REALTIME:
		pWnd = new CRealtimeWnd();
		nType = VIEW_REALTIME;
		break;
	case VIEW_DETAIL:
		pWnd = new CDetailWnd();
		nType = VIEW_DETAIL;
		break;
	default:
		pWnd = NULL;
	}

	if (pWnd != NULL)
	{
		pWnd->SetId(nId);
		pWnd->SetPosition(nPosition);
		pWnd->SetType(nType);
		pWnd->SetWidth(fWidth);
		pWnd->SetHeight(fHeight);

		AddToParent(node, pWnd);
		m_mapWnds[ pWnd->GetId()] = pWnd;
	}
}

/*
���Ӵ��ڻ�������������ӵ�����������
*/
void CPageWnd::AddToParent(rapidxml::xml_node<> * node, CBaseWnd* pWnd)
{
	if (strcmp(node->parent()->name(), "page") == 0)
	{
		
		tree<int>::iterator root = m_treeWnds.begin();
		m_treeWnds.insert(root, pWnd->GetId());
		
		return;
	}
	

	rapidxml::xml_attribute<char> * attr = node->parent()->first_attribute("id"); 
	int nParentId = boost::lexical_cast<int>(attr->value());
	TRACE("��ǰ�ڵ�id=%d, ���ڵ�id=%d\n", pWnd->GetId(), nParentId);

	CBaseWnd * pParentWnd = m_mapWnds[nParentId];

	// ���õ�ǰ���ڵĸ�����
	pWnd->SetParentWndId(nParentId);

	tree<int>::iterator it = std::find(m_treeWnds.begin(), m_treeWnds.end(), nParentId);
	m_treeWnds.append_child(it, pWnd->GetId());

	if (pWnd->GetPosition() == WND_POSITION_FIRST)
	{
		((CBaseContainerWnd*)pParentWnd)->SetFirstChildWnd(pWnd);
	}
	else if (pWnd->GetPosition() == WND_POSITION_SECOND)
	{
		((CBaseContainerWnd*)pParentWnd)->SetSecondChildWnd(pWnd);
	}
	else
	{
		
		TRACE("%s û�д���λ�ò���\n", node->name());
	}
}

int CPageWnd::GetType(std::string sType)
{
	int nType;

	if (sType == "vertical")
		nType = CONTAINER_VERTICAL;
	else if(sType == "horizontal")
		nType = CONTAINER_HORIZONTAL;
	else if (sType == "table")
		nType = VIEW_TABLE;
	else if (sType == "kline")
		nType = VIEW_KLINE;
	else if (sType == "tick")
		nType = VIEW_TICK;
	else if (sType == "realtime")
		nType = VIEW_REALTIME;
	else if (sType == "detail")
		nType = VIEW_DETAIL;
	else
		nType = WND_UNKNOWN;

	return nType;
}


void CPageWnd::OutputWindowTree()
{
	std::map<int, CBaseWnd*>::iterator itMap = m_mapWnds.begin();
	for (; itMap != m_mapWnds.end(); itMap++)
	{
		itMap->second->PrintWindowInfo();
	}

	tree<int>::iterator itTree = m_treeWnds.begin();
	for (; itTree != m_treeWnds.end(); itTree++)
	{
		TRACE("������%d\n",*itTree);
	}
}


int CPageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_pRender = new GDIRenderEngine(m_hWnd);

	return 0;
}


void CPageWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = NULL;
	}
}


void CPageWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

	

	if (m_pRender)
	{
		TRACE("CPageWnd::OnPaint()\n");

		m_pRender->BeginRender();

		CColorUtil clr(255,0,0);
		TRACE("page caption: %s\n", m_sPageCaption);
		m_pRender->DrawText(m_rcClient, _T("ҳ��"), clr);
		m_pRender->DrawRectangle(m_rcClient, clr);

		m_pRender->EndRender();
	}
}


void CPageWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	GetClientRect(m_rcClient);
}

void CPageWnd::ActivePage()
{
}

void CPageWnd::InactivePage()
{
}

void CPageWnd::ShowQuote()
{
	// ʵ�ַ�ʽһ�����������������Ϣ���첽����û����飬�Ƽ�ʹ��
	// ʵ�ַ�ʽ�������������Ѿ����첽���ˣ����Կ���˳����ã����ڲ��Ե�ʱ��ʹ��
	
	for (std::map<int, CBaseWnd*>::iterator itMap = m_mapWnds.begin(); itMap != m_mapWnds.end(); itMap++)
	{
		CBaseWnd* pWnd = itMap->second;

		int nType = pWnd->GetType();
		if ( nType == WND_UNKNOWN || nType == CONTAINER_VERTICAL || nType == CONTAINER_HORIZONTAL )
		{
			continue;
		}
		else
		{
			CFinanceProduct financeProduct("600000", MARKET_SH);
			((CBaseControlWnd*) pWnd)->ShowQuote(financeProduct);
		}
	}
}
