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

	// 临时用于完成方案
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
	int m_nCaptionSeparate; // 标题之间分隔宽度
	int m_nMarginLeft;
	int m_nMarginTop;
	int m_nMarginRight;
	int m_nMarginBottom;

	int m_nCaptionHeight; // 标题栏高度

	CRect rcTick;
	double m_dTickMax; // 比昨收价高或者低的最大范围的值，用于确定比例关系
	float m_fMaxPrice;
	float m_fMinPrice;
	float m_fClosePrice;
	float m_fTickMiddle; // 中间坐标
	int m_nTickRows;

	CRect rcVolume;
	double m_dCjslMax; // 最高成交数量
//	double m_dCjjeMax; // 最高成交金额

	float m_fVolumeScale; // 成交量刻度数, 画几条横线

	int m_nTimeAxisHeight; // 底部时间轴高度

	int m_nYAxisWidth; // Y轴标题宽度，等于最大成交量加上一个空格的宽度

	int m_nScale; // 分时和成交量的绘图区域比例占比
	
	int m_nCols;
	int m_nColWidth;

	IRenderEngine* m_pRender;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnDestroyClipboard();
	afx_msg void OnDestroy();
};


