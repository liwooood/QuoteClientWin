// RealtimeWnd.cpp : ʵ���ļ�
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



// CRealtimeWnd ��Ϣ�������


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
	gFileLog::instance().Log("CRealtimeWnd��ͼ���յ�Ӧ��");
	response->OutputMsgType();

	bool bRet = m_Data.ParseFromArray(&(response->msg_body.front()), response->msg_body.size());
	if (!bRet)
	{
		AfxMessageBox(_T("realtime error"));
	}
	
	for (int i=0; i < m_Data.realtime_size(); i++)
	{
		const quote::RealTimeResponse_RealTime& response =  m_Data.realtime(i);
		TRACE("����Ʒ�ִ���%s\n", response.secucode().c_str());
	}
	
	
	Render();
}


void CRealtimeWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CBaseWnd::OnPaint()

	if (m_pRender)
	{
		GetClientRect(m_rcClient);

		m_pRender->BeginRender();

		CColorUtil clr(255, 0, 0);
		m_pRender->DrawRectangle(m_rcClient, clr);
		//m_pRender->DrawText(m_rcClient, _T("ʵʱ���"), clr);
		Render();

		m_pRender->EndRender();
	}
}


void CRealtimeWnd::Render()
{
	CColorUtil clrLine(255, 0,0);
	CColorUtil clr(192, 192, 0);

	m_sCode = _T("600000");
	m_sName = _T("�ַ�����");
	
	
	CSize sizeCode = m_pRender->GetStringSize(_T("����"), 12, m_sCode);
	TRACE("���ڿ��%d, ������%d, �߶�%d\n", m_rcClient.Width(), sizeCode.cx, sizeCode.cy);

	CSize sizeName = m_pRender->GetStringSize(_T("����"), 12, m_sName);

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
	CSize size = m_pRender->GetStringSize(_T("����"), 12, _T("����"));
	m_nLabelWidth = size.cx;
	m_nLabelHeight = size.cy;
	int nValueWidth = (nWidth - size.cx*2) / 2; // ��ȼ����������ȣ�����2
	
	
	top = CreateRect(top, _T("xj"), _T("ί��"), _T("open"), _T("ί��"));
	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);

	top = CreateRect(top, _T("bid5_price"), _T("����"), _T("bid5_volume"), _T(""));
	top = CreateRect(top, _T("bid4_price"), _T("����"), _T("bid4_volume"), _T(""));
	top = CreateRect(top, _T("bid3_price"), _T("����"), _T("bid3_volume"), _T(""));
	top = CreateRect(top, _T("bid2_price"), _T("����"), _T("bid2_volume"), _T(""));
	top = CreateRect(top, _T("bid1_price"), _T("��һ"), _T("bid1_volume"), _T(""));
	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);
	top = CreateRect(top, _T("ask1_price"), _T("��һ"), _T("ask1_volume"), _T(""));
	top = CreateRect(top, _T("ask2_price"), _T("���"), _T("ask2_volume"), _T(""));
	top = CreateRect(top, _T("ask3_price"), _T("����"), _T("ask3_volume"), _T(""));
	top = CreateRect(top, _T("ask4_price"), _T("����"), _T("ask4_volume"), _T(""));
	top = CreateRect(top, _T("ask5_price"), _T("����"), _T("ask5_volume"), _T(""));
	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);

	top = CreateRect(top, _T("xj"), _T("�ּ�"), _T("open"), _T("��"));
	top = CreateRect(top, _T("xj"), _T("�ǵ�"), _T("open"), _T("���"));
	top = CreateRect(top, _T("xj"), _T("�Ƿ�"), _T("open"), _T("���"));
	top = CreateRect(top, _T("xj"), _T("����"), _T("open"), _T("����"));
	top = CreateRect(top, _T("xj"), _T("����"), _T("open"), _T("����"));
	m_pRender->DrawLine(0, top, m_rcClient.right, top, clrLine);

	top = CreateRect(top, _T("xj"), _T("����"), _T("open"), _T("�ɱ�"));
	top = CreateRect(top, _T("xj"), _T("����"), _T("open"), _T("��ͨ"));
	top = CreateRect(top, _T("xj"), _T("����(һ)"), _T("open"), _T("PE(��)"));
}


int CRealtimeWnd::CreateRect(int top, CString sIdOne, CString sLabelOne, CString sIdTwo, CString sLabelTwo)
{
	CString key;
	
	CColorUtil clrLabel(192, 192, 192);
	CSize sizeOne = m_pRender->GetStringSize(_T("����"), 12, sLabelOne);
	CSize sizeTwo = m_pRender->GetStringSize(_T("����"), 12, sLabelTwo);

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

	// TODO:  �ڴ������ר�õĴ�������
	CFinanceProduct financeProduct("600000", MARKET_SH);
	//ShowQuote(financeProduct);


	return 0;
}
