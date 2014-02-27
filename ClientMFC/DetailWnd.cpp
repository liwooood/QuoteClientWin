// DetailWnd.cpp : 实现文件
//

#include "stdafx.h"

#include "ClientMFC.h"
#include "DetailWnd.h"
#include "market.h"
#include "DataManager.h"
#include "FileLog.h"



// CDetailWnd

IMPLEMENT_DYNAMIC(CDetailWnd, CBaseControlWnd)

CDetailWnd::CDetailWnd()
{

}

CDetailWnd::~CDetailWnd()
{
}


BEGIN_MESSAGE_MAP(CDetailWnd, CBaseControlWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CDetailWnd 消息处理程序

void CDetailWnd::ShowQuote(CFinanceProduct& financeProduct)
{
	quote::DetailRequest detail_request;
	detail_request.set_stockcode(financeProduct.GetFinanceProduct());
	detail_request.set_len(30);
	//detail_request.set_direction()
	//detail_request.set_id()

	
	

	int nPkgBodySize = detail_request.ByteSize();


	tcp_request_ptr request = new tcp_request();
	request->set_body_size(nPkgBodySize);
	detail_request.SerializeToArray(&(request->msg_body.front()), nPkgBodySize);
		
	request->encode_header(quote::PkgHeader::REQ_DETAIL, nPkgBodySize);
	request->SetView(this);

	//sDataManager::instance().PushRequestToQueue(request);
}

void CDetailWnd::RecvResponseCallback(tcp_message * response)
{
	gFileLog::instance().Log("CDetailWnd视图层收到应答");
	response->OutputMsgType();

	int size = sizeof(DETAIL_STRUCT_TYPE);
	int count = response->msg_body.size() / size;
	//m_nDataCount = count;

	for (int i=0; i<count; i++)
	{
		DETAIL_STRUCT_TYPE detail;
		memcpy(&detail, &(response->msg_body.front()) + (i*size), size);
		m_Data.push_back(detail);
	}
	
	Render();
}



void CDetailWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseWnd::OnPaint()

	Render();
	
}

void CDetailWnd::Render()
{
	GetClientRect(m_rcClient);

	if (!m_pRender)
		return;

	m_pRender->BeginRender();

	
	CColorUtil clrRed(255,0,0);
	CColorUtil clrGreen(0,255,0);
	CColorUtil clrLabel(192,192,192);
	CColorUtil clrVolume(192,192,0);
	
	
	m_nHeight = m_rcClient.Height();
	m_nWidth = m_rcClient.Width();


	


	CSize sizeTimeCol = m_pRender->GetStringSize(_T("宋体"), 12, _T("15:00"));
	CSize sizeDirectionCol = m_pRender->GetStringSize(_T("宋体"), 12, _T("BS"));

	m_nRowHeight = sizeTimeCol.cy;
	m_nRowSepHeight = 2;

	m_nRows = m_nHeight / (m_nRowHeight + m_nRowSepHeight);


	int m_nColWidth = (m_nWidth - sizeTimeCol.cx - sizeDirectionCol.cx) / 2;

	int top = 0;

	for (int nRow=0; nRow < m_nRows; nRow++)
	{
		std::vector<CRect> col;

		CRect rcTime, rcPrice, rcVolume, rcDirection;

		
		top = nRow * (m_nRowHeight + m_nRowSepHeight);

		rcTime.left = 0;
		rcTime.top = top;
		rcTime.right = rcTime.left + sizeTimeCol.cx;
		rcTime.bottom = rcTime.top + sizeTimeCol.cy;
		col.push_back(rcTime);

		rcPrice.left = rcTime.right;
		rcPrice.top = top;
		rcPrice.right = rcPrice.left + m_nColWidth;
		rcPrice.bottom = rcPrice.top + sizeTimeCol.cy;
		col.push_back(rcPrice);

		rcVolume.left = rcPrice.right;
		rcVolume.top = top;
		rcVolume.right = rcVolume.left + m_nColWidth;
		rcVolume.bottom = rcVolume.top + sizeTimeCol.cy;
		col.push_back(rcVolume);

		rcDirection.left = rcVolume.right;
		rcDirection.top = top;
		rcDirection.right = m_rcClient.right;
		rcDirection.bottom = rcDirection.top + sizeTimeCol.cy;
		col.push_back(rcDirection);

		m_Table.push_back(col);
	}
	
	if (m_Data.size() == 0)
		return;

	int nPos = 0;
	if (m_Data.size() < m_nRows)
	{
		nPos = 0;
	}
	else
	{
		nPos = m_Data.size()  - m_nRows;
	}
	
	for (int nRow=0; nRow<m_nRows; nRow++)
	{
		if ( (nPos + nRow) >= m_Table.size())
			break;

		DETAIL_STRUCT_TYPE& detail = m_Data[nPos + nRow];

		std::vector<CRect>& col = m_Table[nRow];
		CColorUtil clr(255, 0,0);

		// 时间
		CString sTime;
		sTime.Format(_T("%lld"), detail.quote_time);
		CString sHour = sTime.Mid(8, 2);
		CString sSecond = sTime.Mid(10, 2);
		CString sTimelabel = sHour + ":" + sSecond;

		m_pRender->DrawText(col[0], sTimelabel, clrLabel);

		// 价格
		CString sPrice;
		sPrice.Format(_T("%.2f"), detail.price);
		m_pRender->DrawText(col[1], sPrice, clr);

		// 数量
		CString sVolume;
		sVolume.Format(_T("%d"), detail.xl);
		m_pRender->DrawText(col[2], sVolume, clrVolume);

		CString sDirection;
		sDirection.Format(_T("%c"), detail.nwp);
		if (sDirection == "B")
			m_pRender->DrawText(col[3], sDirection, clrRed);
		else
			m_pRender->DrawText(col[3], sDirection, clrGreen);
		
	}

	m_pRender->EndRender();
}


int CDetailWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseControlWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//CFinanceProduct financeProduct("600000", MARKET_SH);
	//ShowQuote(financeProduct);

	return 0;
}
