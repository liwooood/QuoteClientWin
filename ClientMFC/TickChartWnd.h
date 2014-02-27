#pragma once

#include <string>
#include <map>
#include <vector>
#include <unordered_map>

#include "IRenderEngine.h"
#include "FinanceProduct.h"
#include "tcp_message.h"
#include "BaseControlWnd.h"

#pragma pack(1)
struct tick_struct_raw
{
	long long quote_time;//yyyyMMddHHmmSS
	float pre_close;
//			float open_today;
//			float high;
//			float low;
	float xj;
	double cjsl;
	double cjje;
	float leading_index;
};
#pragma pack()

struct struct_tick
{
	long long quote_time;//yyyyMMddHHmmSS
	float pre_close;
//			float open_today;
//			float high;
//			float low;
	float xj;
	double cjslTotal;
	double cjjeTotal;
	float leading_index;

	float avg;
	float cjsl;
	float cjje;
};

class CTickChartWnd : public CBaseControlWnd
{
	DECLARE_DYNAMIC(CTickChartWnd)

public:
	CTickChartWnd();
	virtual ~CTickChartWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnPaint();

	// ��ʱ������ɷ���
	virtual void ShowQuote(CFinanceProduct& financeProduct);
	virtual void RecvResponseCallback(tcp_message * response);
	void UpdateQuote();

private:
	void Render();
	void DrawCaption();
	void DrawTick();
	void DrawVolume();
	void DrawTimeAxis();

	CSize GetStringSize(CString FontName, int FontSize, CString str);

private:
	std::map<long long, struct struct_tick> data;
	int m_nDataCount;

	CRect rcClient;
	int m_nDrawAreaHeight;
	int m_nMaxRowHeight;
	int m_nCaptionSeparate; // ����֮��ָ����
	int m_nMarginLeft;
	int m_nMarginTop;
	int m_nMarginRight;
	int m_nMarginBottom;

	int m_nCaptionHeight; // �������߶�

	CRect rcTick;
	double m_dTickMax; // �����ռ۸߻��ߵ͵����Χ��ֵ������ȷ��������ϵ
	float m_fMaxPrice;
	float m_fMinPrice;
	float m_fClosePrice;
	float m_fTickMiddle; // �м�����
	int m_nTickRows;

	CRect rcVolume;
	double m_dCjslMax; // ��߳ɽ�����
//	double m_dCjjeMax; // ��߳ɽ����

	float m_fVolumeScale; // �ɽ����̶���, ����������

	int m_nTimeAxisHeight; // �ײ�ʱ����߶�

	int m_nYAxisWidth; // Y������ȣ��������ɽ�������һ���ո�Ŀ��

	int m_nScale; // ��ʱ�ͳɽ����Ļ�ͼ�������ռ��
	
	int m_nCols;
	int m_nColWidth;

	IRenderEngine* m_pRender;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnDestroyClipboard();
	afx_msg void OnDestroy();
};


