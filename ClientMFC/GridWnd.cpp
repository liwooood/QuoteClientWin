// GridWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "GridWnd.h"

#include "../xml/pugixml.hpp"
#include "importheader.h"
#include <fstream>
#include "cssweb_charset.h"
#include "DataManager.h"
#include "FileLog.h"

// CGridWnd

IMPLEMENT_DYNAMIC(CGridWnd, CBaseControlWnd)

//USES_CONVERSION;

CGridWnd::CGridWnd()
{
	m_nFixedCols = 3;
	m_nBottomCtrlHeight = 20;
	m_nRowId = 1; // 行序号
}

CGridWnd::~CGridWnd()
{
}


BEGIN_MESSAGE_MAP(CGridWnd, CBaseControlWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CGridWnd 消息处理程序

void CGridWnd::ShowQuote(CFinanceProduct& financeProduct)
{
	quote::GridRequest grid_request;
	grid_request.add_stockcode("600000.1");
	grid_request.add_stockcode("600004.1");
	grid_request.add_stockcode("600005.1");
	grid_request.add_stockcode("600006.1");
	grid_request.add_stockcode("600007.1");
	
	grid_request.add_stockcode("600008.1");
	grid_request.add_stockcode("600009.1");
	grid_request.add_stockcode("600010.1");
	grid_request.add_stockcode("600011.1");
	grid_request.add_stockcode("600012.1");

	grid_request.add_stockcode("600015.1");
	grid_request.add_stockcode("600016.1");
	grid_request.add_stockcode("600017.1");
	grid_request.add_stockcode("600018.1");
	grid_request.add_stockcode("600019.1");

	grid_request.add_stockcode("600020.1");
	grid_request.add_stockcode("600021.1");
	grid_request.add_stockcode("600022.1");
	grid_request.add_stockcode("600026.1");
	grid_request.add_stockcode("600027.1");

	grid_request.add_stockcode("600028.1");
	grid_request.add_stockcode("600029.1");
	grid_request.add_stockcode("600030.1");
	grid_request.add_stockcode("600031.1");
	grid_request.add_stockcode("600033.1");

	
	int nPkgBodySize = grid_request.ByteSize();


	tcp_request_ptr request = new tcp_request();
	request->set_body_size(nPkgBodySize);
	grid_request.SerializeToArray(&(request->msg_body.front()), nPkgBodySize);
		
	request->encode_header(quote::PkgHeader::REQ_GRID, nPkgBodySize);
	request->SetView(this);

	//sDataManager::instance().PushRequestToQueue(request);
}

void CGridWnd::RecvResponseCallback(tcp_message * response)
{
	gFileLog::instance().Log("CGridWnd视图层收到应答");
	response->OutputMsgType();

	bool bRet = m_Data.ParseFromArray(&(response->msg_body.front()), response->msg_body.size());
	if (!bRet)
	{
		AfxMessageBox(_T("realtime error"));
	}
	
	for (int i=0; i < m_Data.grid_size(); i++)
	{
		const quote::Grid& row =  m_Data.grid(i);
		TRACE("金融品种代码%s\n", row.secucode().c_str());
	}
	
	
	Render();
}


void CGridWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseWnd::OnPaint()

	//CBrush brush(RGB(255,0,0));
	
	//dc.SelectObject(brush);
	//dc.TextOut(0, 0, _T("表格窗口"), 1);

	Render();
}


void CGridWnd::OnSize(UINT nType, int cx, int cy)
{
	CBaseControlWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	InitWnd();

	m_rcTabCtrl.left = 0;
	m_rcTabCtrl.right = 800; // 先写死，将来根据菜单项长度来计算
	m_rcTabCtrl.top = m_rcClient.bottom - m_nBottomCtrlHeight;
	m_rcTabCtrl.bottom = m_rcClient.bottom;

	
	m_wndTabCtrl.MoveWindow(m_rcTabCtrl);
}




void CGridWnd::InitWnd()
{
	GetClientRect(m_rcClient);

	// 根据高度，计算出动态的行数
	
	int nHeight = m_rcClient.Height() - m_nBottomCtrlHeight;
	m_nRowHeight = 20;
	m_nRowSepHeight = 3;
	m_nRows = nHeight / (m_nRowHeight + m_nRowSepHeight);

	m_nRows += 1;  // 这里加了一行
	
	for (int i=0; i < (m_nRows); i++)
	{
		CRect rcRow;
		rcRow.left = 0;
		rcRow.top = i * (m_nRowHeight + m_nRowSepHeight);
		rcRow.right = m_rcClient.right;
		rcRow.bottom = rcRow.top + (m_nRowHeight + m_nRowSepHeight);

		CRowInfo row;
		row.m_rcRow = rcRow;

		m_Rows[i] = row;
	}

	// 根据宽度，计算出动态的列数，以及选择的可视列
	int nWidth = m_rcClient.Width();
	
	// 根据键盘左右键移动的位置，以及除去固定列所占宽度后，计算出显示哪几列？
	int nDisplayColsWidth = 0;
	m_ColDisplay.clear();
	for (int i=0; i < m_Cols.size(); i++)
	{
		CColInfo& col = m_Cols[i];

		nDisplayColsWidth += col.m_nColWidth;

		if (nDisplayColsWidth > nWidth)
		{
			break;
		}
		else
		{
			this->m_ColDisplay.push_back(col.m_nColId);
			TRACE("哪几列可以显示 %d\n", col.m_nColId);
		}
	}

	// 计算出动态表格，每一行，每一列的CRect
	for (int i=0; i<m_nRows; i++)
	{
		ROW_TYPE row;

		int left = 0;
		for (int j=0; j<m_ColDisplay.size(); j++)
		{
			int nColId = m_ColDisplay[j];

			// 根据列名，找到列定义的描述信息
			CColInfo col = m_ColHeader[nColId];
			
			CRect rcCol;
			rcCol.left = left;
			rcCol.right = rcCol.left + col.m_nColWidth;
			left = rcCol.right;

			rcCol.top = i * m_nRowHeight; // 没有包括m_nRowSepHeight
			rcCol.bottom = rcCol.top + m_nRowHeight;
			
			col.m_rcCol = rcCol;

			// 设置每一列的信息
			row[nColId] = col;
		}

		m_Grid[i] = row;
	}

}


void CGridWnd::AddCol(CColInfo colInfo)
{
	//m_ColHeader.push_back(colInfo);
}



int CGridWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseControlWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	GetClientRect(m_rcClient);

	LoadColumnMapXML();

	
	m_rcTabCtrl.left = 0;
	m_rcTabCtrl.right = 800; // 先写死，将来根据菜单项长度来计算
	m_rcTabCtrl.top = m_rcClient.bottom - m_nBottomCtrlHeight;
	m_rcTabCtrl.bottom = m_rcClient.bottom;

	m_wndTabCtrl.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, m_rcTabCtrl, this, 1234);

	return 0;
}

void CGridWnd::LoadColumnMapXML()
{
	USES_CONVERSION;

	std::string sPageXMLFile = "D:/workspace_windows/QuoteTrade/Client/ClientMFC/QuoteClient/grid.xml";
	std::ifstream stream(sPageXMLFile);

	pugi::xml_document doc;

	if (!doc.load(stream))
	{
		return;
	}


	pugi::xpath_node_set nodes = doc.select_nodes("/grid/column");
	
	

	for (pugi::xpath_node_set::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		pugi::xpath_node node = *it;

		CColInfo col;
		

		
		
		pugi::xml_node::attribute_iterator itAttr = node.node().attributes_begin();
		for (; itAttr != node.node().attributes_end(); itAttr++)
		{
			std::string name = itAttr->name();
			std::string value = itAttr->value();

			
			TRACE("属性%s=%s\n", name.c_str(),  value.c_str());

			if (name == "id")
			{
				col.m_nColId = boost::lexical_cast<int>(value);
			}
			else if (name == "caption")
			{
				std::string caption = Utf8ToGBK(value);
				col.m_sColCaption = caption.c_str();
				TRACE("列标题 = %s\n",  W2A(col.m_sColCaption));
			}
			else if (name == "width")
			{
				col.m_nColWidth = boost::lexical_cast<int>(value);
			}
		} // end for

		// 方便查找
		m_ColHeader[col.m_nColId] = col;
		// 实际使用
		m_Cols.push_back(col);
	}// end for

	// test only
	for (std::map<int, CColInfo>::iterator it = m_ColHeader.begin(); it != m_ColHeader.end(); it++)
	{
		TRACE("m_ColHeader %d\n", it->first);
	}
}


void CGridWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBaseControlWnd::OnLButtonDblClk(nFlags, point);
}


void CGridWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBaseControlWnd::OnLButtonDown(nFlags, point);
}

void CGridWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBaseControlWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CGridWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CBaseControlWnd::OnEraseBkgnd(pDC);
}
void CGridWnd::Render()
{

	if (!m_pRender)
	{
		return;
	}

	

	
	// test only
	

	if (m_Data.grid_size() <= 0)
	{
		return;

		// 没有数据
		m_pRender->BeginRender();
		CSize size = m_pRender->GetStringSize(_T("宋体"), 12, _T("名称"));

		// 每一行
		for (int i=0; i<m_nRows; i++)
		{
			ROW_TYPE& row = m_Grid[i];

			for(ROW_TYPE::iterator it = row.begin(); it != row.end(); it++)
			{
				int nColId = it->first;
				CColInfo& col = it->second;
				TRACE("grid col name %d\n", col.m_nColId);

				if (i == 0)
				{
					// 第一行为标题行
					CColorUtil clr(192,192,192);
					m_pRender->DrawText(col.m_rcCol, col.m_sColCaption, clr);
				}
				else
				{
					if (nColId == TABLE_COL_ID)
					{
						CColorUtil clr(192,192,192);
						CString sRowId;
						sRowId.Format(_T("%d"), m_nRowId);
						m_pRender->DrawText(col.m_rcCol, sRowId, clr);
						m_nRowId++;
					}
				}

			} // end col
		} // end row

		// 结束
		m_pRender->EndRender();
	}
	else
	{
		m_rcClient.bottom = m_rcClient.bottom - m_nBottomCtrlHeight;

		m_pRender->BeginRender(m_rcClient);
		
		ROW_TYPE& row = m_Grid[0];

		for(ROW_TYPE::iterator it = row.begin(); it != row.end(); it++)
		{
			int nColId = it->first;
			CColInfo& col = it->second;
			//TRACE("grid col name %d\n", col.m_nColId);

			// 第一行为标题行
			CColorUtil clr(192,192,192);
			m_pRender->DrawText(col.m_rcCol, col.m_sColCaption, clr);
		}
		
		
		for (int i=0; i<m_Data.grid_size(); i++)
		{
			const quote::Grid& grid = m_Data.grid(i);

			if (i >= m_nRows)
				break;

			// 新的数据m_Data和老的数据m_AllData进行比较

			// 根据可见列比较更新数据
			ROW_TYPE& row = m_Grid[i+1]; // 不包含标题行

			for (ROW_TYPE::iterator it = row.begin(); it != row.end(); it++)
			{
				int nColId = it->first;
				CColInfo& col = it->second;

				switch(nColId)
				{
				case TABLE_COL_ID:
					{
						CColorUtil clr(192,192,192);
						CString sRowId;
						sRowId.Format(_T("%d"), m_nRowId);
						m_pRender->DrawText(col.m_rcCol, sRowId, clr);
						m_nRowId++;
						break;
					}
				case TABLE_COL_CODE:
					{
					CColorUtil clr(192,192,0);
					m_pRender->DrawText(col.m_rcCol, CString(grid.secucode().c_str()), clr);
					//m_pRender->InvalidRect(col.m_rcCol);
					break;
					}
				case TABLE_COL_NAME:
					{
					CColorUtil clr(192,192,0);
					//m_pRender->DrawText(col.m_rcCol, CString(grid.se), clr);
					//m_pRender->InvalidRect(col.m_rcCol);
					break;
					}
				case TABLE_COL_XJ:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.close());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_ZF:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.zf());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_ZD:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.zd());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_BID1:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.bidprice1());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_ASK1:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.askprice1());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_XL:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%d"), grid.xs());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_CJSL:
					{
					CColorUtil clr(192,192,0);
					std::string value = boost::lexical_cast<std::string>(grid.vol());
					m_pRender->DrawText(col.m_rcCol, CString(value.c_str()), clr);
					break;
					}
				case TABLE_COL_HS:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.hs());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_OPEN:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.open());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_HIGH:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.high());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_LOW:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.low());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_CLOSE:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.close());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_CJJE:
					{
					CColorUtil clr(192,192,0);
					std::string value = boost::lexical_cast<std::string>(grid.amt());
					m_pRender->DrawText(col.m_rcCol, CString(value.c_str()), clr);
					break;
					}
				case TABLE_COL_ZHENFU:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.amp());
					m_pRender->DrawText(col.m_rcCol, value, clr);

					break;
					}
				case TABLE_COL_LB:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.lb());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_WB:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%.2f"), grid.wb());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				case TABLE_COL_WC:
					{
					CColorUtil clr(192,192,0);
					CString value;
					value.Format(_T("%d"), grid.wc());
					m_pRender->DrawText(col.m_rcCol, value, clr);
					break;
					}
				}
			}

			// 比较完成后，更新老的数据
			// m_AllData[code]  = m_Data;

		} // end for
		
		m_pRender->EndRender();

	} // end if

	

	//m_wndTabCtrl.ShowWindow(SW_SHOW);
	//m_wndTabCtrl.UpdateWindow();

}
