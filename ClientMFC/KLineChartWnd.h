#pragma once

#include "BaseControlWnd.h"

#include "IRenderEngine.h"
#include "Indicator.h"


// ʱ������
#define TIME_PERIOD_1M 1
#define TIME_PERIOD_15M 2
#define TIME_PERIOD_30M 3
#define TIME_PERIOD_60M 4
#define TIME_PERIOD_DAY 5
#define TIME_PERIOD_WEEK 6
#define TIME_PERIOD_MONTH 7
#define TIME_PERIOD_YEAR 8

// ��ͼ������
#define MAIN_LINE_TYPE_NONE 0
#define MAIN_LINE_TYPE_KLINE 1
#define MAIN_LINE_TYPE_CLOSE 2

// ָ������
#define INDICATOR_TYPE_MA 0
#define INDICATOR_TYPE_VOL 1
#define INDICATOR_TYPE_MACD 2
#define INDICATOR_TYPE_KDJ 3

#pragma pack(1)
struct kline_struct
{
    long long time;//20120101100100 
    int  id;//�������꣺���ε�����  ���ӣ�ÿ���1��ʼ����
    float open;
    float high;
    float low;
    float close;
    double vol;
    double amt;
};
#pragma pack()

class CKLineChartWnd : public CBaseControlWnd
{
	DECLARE_DYNAMIC(CKLineChartWnd)

public:
	CKLineChartWnd();
	virtual ~CKLineChartWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void ShowQuote(CFinanceProduct& financeProduct);
	virtual void RecvResponseCallback(tcp_message * response);

private:
	void InitWnd();
	void Render();

public:
	afx_msg void OnPaint();

private:
	// ʱ������
	int m_nTimePeriod;

	// ���ڸ���
	int m_nIndicatorWnds;
	std::vector<CIndicator*> m_vIndicators;
	int m_nIndicatorHeight;
	int m_nIndicatorTotalHeight;

	// ��ǰ���ڻ״̬��ָ�괰��
	int m_nActiveIndiatorWnd;


	// ʱ����߶�
	int m_nTimeAxisHeight;

	std::vector<struct kline_struct> m_vData;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


