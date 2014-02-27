// RealtimeWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "RealtimeWnd.h"
#include "market.h"
#include "FileLog.h"
#include "DataManager.h"


// CRealtimeWnd

IMPLEMENT_DYNAMIC(CRealtimeWnd, CBaseControlWnd)

CRealtimeWnd::CRealtimeWnd()
{

}

CRealtimeWnd::~CRealtimeWnd()
{
}


BEGIN_MESSAGE_MAP(CRealtimeWnd, CBaseControlWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CRealtimeWnd 消息处理程序


void CRealtimeWnd::ShowQuote(CFinanceProduct& financeProduct)
{
	quote::RealTimeRequest realtime_request;
	realtime_request.add_stockcode(financeProduct.GetFinanceProduct());
	int nPkgBodySize = realtime_request.ByteSize();


	tcp_request_ptr request = new tcp_request();
	request->set_body_size(nPkgBodySize);
	realtime_request.SerializeToArray(&(request->msg_body.front()), nPkgBodySize);
		
	request->encode_header(quote::PkgHeader::REQ_REALTIME, nPkgBodySize);
	request->SetView(this);

	//sDataManager::instance().PushRequestToQueue(request);
}

void CRealtimeWnd::RecvResponseCallback(tcp_message * response)
{
	gFileLog::instance().Log("CRealtimeWnd视图层收到应答");
	response->OutputMsgType();

	bool bRet = m_Data.ParseFromArray(&(response->msg_body.front()), response->msg_body.size());
	if (!bRet)
	{
		AfxMessageBox(_T("realtime error"));
	}
	
	for (int i=0; i < m_Data.realtime_size(); i++)
	{
		const quote::RealTimeResponse_RealTime& response =  m_Data.realtime(i);
		TRACE("金融品种代码%s\n", response.secucode().c_str());
	}
	
	
	Render();
}


void CRealtimeWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseWnd::OnPaint()

	if (m_pRender)
	{
		GetClientRect(m_rcClient);

		m_pRender->BeginRender();

		CColorUtil clr(255, 0, 0);
		m_pRender->DrawRectangle(m_rcClient, clr);
		//m_pRender->DrawText(m_rcClient, _T("实时组件"), clr);
		Render();

		m_pRender->EndRender();
	}
}


void CRealtimeWnd::Render()
{
	CColorUtil clrLine(255, 0,0);
	CColorUtil clr(192, 192, 0);

	m_sCode = _T("600000");
	m_sName = _T("浦发银行");
	
	
	CSize sizeCode = m_pRender->GetStringSize(_T("宋体"), 12, m_sCode);
	TRACE("窗口宽度%d, 代码宽度%d, 高度%d\n", m_rcClient.Width(), sizeCode.cx, sizeCode.cy);

	CSize sizeName = m_pRender->GetStringSize(_T("宋体"), 12, m_sName);

	CRect rcCode, rcName;
	rcCode.left = (m_rcClient.Width() -  sizeCode.cx - sizeName.cx) / 2;
	rcCode.top = 0;
	rcCode.right = rcCode.left + sizeCode.cx;
	rcCode.bottom = rcCode.top + sizeCode.cy;
	
	m_pRender->DrawText(rcCode, m_sCode, clr);

	rcName.left = rcCode.right;
	rcName.top = 0;
	rcName.right = rcName.left + sizeName.cx;
	rcName.bottom = rcName.top + sizeName.cy;
	m_pRender->DrawText(rcName, m_sName, clr);
	int top = rcName.bottom;

	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);

	
	int nWidth = m_rcClient.Width();
	m_nX2 = nWidth / 2;
	CSize size = m_pRender->GetStringSize(_T("宋体"), 12, _T("标题"));
	m_nLabelWidth = size.cx;
	m_nLabelHeight = size.cy;
	int nValueWidth = (nWidth - size.cx*2) / 2; // 宽度减两个标题宽度，除以2
	
	
	top = CreateRect(top, _T("xj"), _T("委比"), _T("open"), _T("委差"));
	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);

	top = CreateRect(top, _T("bid5_price"), _T("卖五"), _T("bid5_volume"), _T(""));
	top = CreateRect(top, _T("bid4_price"), _T("卖四"), _T("bid4_volume"), _T(""));
	top = CreateRect(top, _T("bid3_price"), _T("卖三"), _T("bid3_volume"), _T(""));
	top = CreateRect(top, _T("bid2_price"), _T("卖二"), _T("bid2_volume"), _T(""));
	top = CreateRect(top, _T("bid1_price"), _T("卖一"), _T("bid1_volume"), _T(""));
	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);
	top = CreateRect(top, _T("ask1_price"), _T("买一"), _T("ask1_volume"), _T(""));
	top = CreateRect(top, _T("ask2_price"), _T("买二"), _T("ask2_volume"), _T(""));
	top = CreateRect(top, _T("ask3_price"), _T("买三"), _T("ask3_volume"), _T(""));
	top = CreateRect(top, _T("ask4_price"), _T("买四"), _T("ask4_volume"), _T(""));
	top = CreateRect(top, _T("ask5_price"), _T("买五"), _T("ask5_volume"), _T(""));
	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);

	top = CreateRect(top, _T("xj"), _T("现价"), _T("open"), _T("今开"));
	top = CreateRect(top, _T("xj"), _T("涨跌"), _T("open"), _T("最高"));
	top = CreateRect(top, _T("xj"), _T("涨幅"), _T("open"), _T("最低"));
	top = CreateRect(top, _T("xj"), _T("总量"), _T("open"), _T("量比"));
	top = CreateRect(top, _T("xj"), _T("外盘"), _T("open"), _T("内盘"));
	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);

	top = CreateRect(top, _T("xj"), _T("换手"), _T("open"), _T("股本"));
	top = CreateRect(top, _T("xj"), _T("净资"), _T("open"), _T("流通"));
	top = CreateRect(top, _T("xj"), _T("收益(一)"), _T("open"), _T("PE(动)"));
}


int CRealtimeWnd::CreateRect(int top, CString sIdOne, CString sLabelOne, CString sIdTwo, CString sLabelTwo)
{
	CString key;
	
	CColorUtil clrLabel(192, 192, 192);
	CSize sizeOne = m_pRender->GetStringSize(_T("宋体"), 12, sLabelOne);
	CSize sizeTwo = m_pRender->GetStringSize(_T("宋体"), 12, sLabelTwo);

	CRect rcLabel;
	rcLabel.left = 0;
	rcLabel.right = rcLabel.left + sizeOne.cx;
	rcLabel.top = top;
	rcLabel.bottom = rcLabel.top + m_nLabelHeight;
	key = sIdOne + _T("_Label");
	m_mapData[key] = rcLabel;
	m_pRender->DrawText(rcLabel, sLabelOne, clrLabel);

	CRect rcValue;
	rcValue.left = rcLabel.right;
	rcValue.right = m_nX2;
	rcValue.top = top;
	rcValue.bottom = rcValue.top + m_nLabelHeight;
	key = sIdOne + _T("_Value");
	m_mapData[key] = rcValue;

	CRect rcLabelTwo;
	rcLabelTwo.left = m_nX2;
	rcLabelTwo.right = rcLabelTwo.left + sizeTwo.cx;
	rcLabelTwo.top = top;
	rcLabelTwo.bottom = rcLabelTwo.top + m_nLabelHeight;
	key = sIdTwo + _T("_Label");
	m_mapData[key] = rcLabelTwo;
	m_pRender->DrawText(rcLabelTwo, sLabelTwo, clrLabel);

	CRect rcValueTwo;
	rcValueTwo.left = rcLabelTwo.right;
	rcValueTwo.right = m_rcClient.right;
	rcValueTwo.top = top;
	rcValueTwo.bottom = rcValueTwo.top + m_nLabelHeight;
	key = sIdTwo + _T("_Value");
	m_mapData[key] = rcValueTwo;

	return rcLabel.bottom;
}


int CRealtimeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseControlWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CFinanceProduct financeProduct("600000", MARKET_SH);
	//ShowQuote(financeProduct);


	return 0;
}
