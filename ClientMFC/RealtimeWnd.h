#pragma once

#include "BaseControlWnd.h"
#include "IRenderEngine.h"

#define REALTIME_CODE 0
#define REALTIME_NAME 1


class CRealtimeWnd : public CBaseControlWnd
{
	DECLARE_DYNAMIC(CRealtimeWnd)

public:
	CRealtimeWnd();
	virtual ~CRealtimeWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void ShowQuote(CFinanceProduct& financeProduct);
	virtual void RecvResponseCallback(tcp_message * response);

	afx_msg void OnPaint();

private:
	std::map<CString, CRect> m_mapData;
	std::vector<CString> m_vIds;
	std::map<CString, CString> m_mapKeyValue;

	void Render();

	CString m_sCode;
	CString m_sName;

	int CreateRect(int top, CString sId, CString sLabel);

	int m_nLabelWidth, m_nLabelHeight;
	int m_nX2;
	int CreateRect(int top, CString sIdOne, CString sLabelOne, CString sIdTwo, CString sLabelTwo);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	quote::RealTimeResponse m_Data;
};


