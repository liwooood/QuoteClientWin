#pragma once

#include "BaseControlWnd.h"

#include "IRenderEngine.h"
#include "Indicator.h"


// 时间周期
#define TIME_PERIOD_1M 1
#define TIME_PERIOD_15M 2
#define TIME_PERIOD_30M 3
#define TIME_PERIOD_60M 4
#define TIME_PERIOD_DAY 5
#define TIME_PERIOD_WEEK 6
#define TIME_PERIOD_MONTH 7
#define TIME_PERIOD_YEAR 8

// 主图线类型
#define MAIN_LINE_TYPE_NONE 0
#define MAIN_LINE_TYPE_KLINE 1
#define MAIN_LINE_TYPE_CLOSE 2

// 指标类型
#define INDICATOR_TYPE_MA 0
#define INDICATOR_TYPE_VOL 1
#define INDICATOR_TYPE_MACD 2
#define INDICATOR_TYPE_KDJ 3

#pragma pack(1)
struct kline_struct
{
    long long time;//20120101100100 
    int  id;//日周月年：依次递增；  分钟：每天从1开始递增
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
	// 时间周期
	int m_nTimePeriod;

	// 窗口个数
	int m_nIndicatorWnds;
	std::vector<CIndicator*> m_vIndicators;
	int m_nIndicatorHeight;
	int m_nIndicatorTotalHeight;

	// 当前处于活动状态的指标窗口
	int m_nActiveIndiatorWnd;


	// 时间轴高度
	int m_nTimeAxisHeight;

	std::vector<struct kline_struct> m_vData;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


