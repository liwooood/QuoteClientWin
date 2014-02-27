// KLineChartWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "KLineChartWnd.h"
#include "market.h"
#include "FileLog.h"
#include "DataManager.h"



// CKLineChartWnd

IMPLEMENT_DYNAMIC(CKLineChartWnd, CBaseControlWnd)

CKLineChartWnd::CKLineChartWnd()
{

}

CKLineChartWnd::~CKLineChartWnd()
{
}


BEGIN_MESSAGE_MAP(CKLineChartWnd, CBaseControlWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CKLineChartWnd 消息处理程序


void CKLineChartWnd::ShowQuote(CFinanceProduct& financeProduct)
{
	quote::CandleRequest candle_request;
	candle_request.set_stockcode("600000.1");
	candle_request.set_candlecycle(quote::CandleRequest::DAY);
	candle_request.set_len(250);
	//candle_request.set_len(1);
	candle_request.set_date("");
	candle_request.set_id(0);
	candle_request.set_direction(quote::CandleRequest::FORWARD);


	int nPkgBodySize = candle_request.ByteSize();

	tcp_request_ptr request = new tcp_request();
	request->set_body_size(nPkgBodySize);

	candle_request.SerializeToArray(&(request->msg_body.front()), nPkgBodySize);
		
	request->encode_header(quote::PkgHeader::REQ_CANDLE, nPkgBodySize);
	request->SetView(this);

	sDataManager::instance().PushRequestToQueue(request);
}

void CKLineChartWnd::RecvResponseCallback(tcp_message * response)
{
	gFileLog::instance().Log("CKLineChartWnd视图层收到应答");
	response->OutputMsgType();

	int size = sizeof(struct kline_struct);
	int count = response->msg_body.size() / size;


	for (int i=0; i<count; i++)
	{
		struct kline_struct kline;
		memcpy(&kline, &(response->msg_body.front()) + (i*size), size);

		m_vData.push_back(kline);
	}
	
	Render();
}


void CKLineChartWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseWnd::OnPaint()
	/*
	if (m_pRender)
	{
		GetClientRect(m_rcClient);

		m_pRender->BeginRender();

		CColorUtil clr(0, 255,0);
		m_pRender->DrawRectangle(m_rcClient, clr);
		m_pRender->DrawText(m_rcClient, _T("K线组件"), clr);

		m_pRender->EndRender();
	}
	*/
	Render();
}

void CKLineChartWnd::InitWnd()
{

	float n = 1.56;
	float n1 = ceil(n);
	float n2 = floor(1.56);
	
	m_nIndicatorWnds = 3;
	
	m_nTimeAxisHeight = 12;

	GetClientRect(m_rcClient);
	m_nIndicatorTotalHeight = m_rcClient.Height() - m_nTimeAxisHeight;
	m_nIndicatorHeight = m_nIndicatorTotalHeight / (m_nIndicatorWnds+1);

	CIndicator * main = new CIndicator(this->GetSafeHwnd());
	main->m_nLineType = MAIN_LINE_TYPE_KLINE;
	main->m_nIndicatorType = INDICATOR_TYPE_MA;
	CRect rcMain;
	rcMain.left = m_rcClient.left;
	rcMain.top = m_rcClient.top;
	rcMain.right = m_rcClient.right;
	rcMain.bottom = rcMain.top + m_nIndicatorHeight * 2;
	main->SetArea(rcMain);

	CIndicator *  vol = new CIndicator(this->GetSafeHwnd());
	vol->m_nLineType = MAIN_LINE_TYPE_NONE;
	vol->m_nIndicatorType = INDICATOR_TYPE_VOL;

	CRect rcVol;
	rcVol.left = m_rcClient.left;
	rcVol.top = rcMain.bottom;
	rcVol.right = m_rcClient.right;
	rcVol.bottom = rcVol.top + m_nIndicatorHeight;
	vol->SetArea(rcVol);

	CIndicator * macd = new CIndicator(this->GetSafeHwnd());
	macd->m_nLineType = MAIN_LINE_TYPE_NONE;
	macd->m_nIndicatorType = INDICATOR_TYPE_MACD;
	CRect rcMacd;
	rcMacd.left = m_rcClient.left;
	rcMacd.top = rcVol.bottom;
	rcMacd.right = m_rcClient.right;
	rcMacd.bottom = rcMacd.top + m_nIndicatorHeight;
	macd->SetArea(rcMacd);

	m_vIndicators.push_back(main);
	m_vIndicators.push_back(vol);
	m_vIndicators.push_back(macd);


	m_nActiveIndiatorWnd = 1; // 0永远是主图指标
}

void CKLineChartWnd::Render()
{
	for (std::vector<CIndicator*>::iterator it = m_vIndicators.begin(); it != m_vIndicators.end(); it++)
	{
		CIndicator * pIndicator = *it;

		pIndicator->Render(&m_vData);
	}
}


void CKLineChartWnd::OnSize(UINT nType, int cx, int cy)
{
	CBaseControlWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	InitWnd();
}
