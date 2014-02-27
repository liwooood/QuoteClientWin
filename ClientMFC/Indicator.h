#ifndef INDICATOR_H
#define INDICATOR_H

#include <vector>

#include "IRenderEngine.h"

class CIndicator
{
public:
	CIndicator(HWND hWnd);
	~CIndicator(void);

public:
	

	// 指标窗口区域
	CRect m_rcArea;

	// 真正的绘图区域
	CRect m_rcMemDC;

	// 指标标题区域
	CRect m_rcCaption;
	int m_nCaptionHeight;


	// 右边价格，数量的宽度
	int m_nRightAxisWidth;

	// 指标绘图区域
	CRect m_rcDraw;

	// 画线类型，只对主图有效
	int m_nLineType;

	// 指标类型
	int m_nIndicatorType;

	// 窗口句柄
	HWND m_hWnd;

	IRenderEngine* m_pRender;

	//价格区间
	float m_fMaxPrice;
	float m_fMinPrice;
	long m_lMaxVolume;

	// 最大量
	double m_dVolume;

	// k线
	int m_nKLineWidth;

	// 数据图形之间的分隔宽度
	int m_nSepWidth;

	// 右边留空几个数据
	int m_nMarginCount;
	int m_nMarginWidth; // m_nMarginWidth = m_nKLineWidth + m_nSepWidth

	// 信息雷达高度
	int m_nInfoRadarHeight;

	// 底部权息资料高度
	int m_nWeightHeight;
	
	// 根据宽度，以及去掉右边留空，可以画几根k线
	int m_nDrawKLineCount;

	// 行高
	int m_nRowHeight;
	int m_nMinRows;

	float m_fScale;

public:
	//void SetWnd(HWND hWnd){m_hWnd = hWnd;}

	void InitWnd();
	void Render(std::vector<struct kline_struct>* pvData);

	void SetArea(CRect rcArea);

private:
	void KLine();
	void Vol();
	void MACD();

	void Compute();

	std::vector<struct kline_struct>* m_pvData;
	
};

#endif
