#include "stdafx.h"
#include "Indicator.h"
#include "GDIRenderEngine.h"
#include "KLineChartWnd.h"

#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/round.hpp>

#include "FileLog.h"


CIndicator::CIndicator(HWND hWnd)
	:m_hWnd(hWnd)
{
	m_pRender = NULL;

	m_pRender = new GDIRenderEngine(m_hWnd);
	m_pRender->Initialize();
}


CIndicator::~CIndicator(void)
{
	if (m_pRender)
	{
		delete m_pRender;
		m_pRender = NULL;
	}
}

void CIndicator::Render(std::vector<struct kline_struct>* pvData)
{
	m_pvData = pvData;
	
	

	if (!m_pRender)
		return;

	m_pRender->BeginRender(m_rcArea);

	if (m_nIndicatorType == INDICATOR_TYPE_MA)
	{
		KLine();
		

	}
	else if (m_nIndicatorType == INDICATOR_TYPE_VOL)
	{
		Vol();		
	}
	else
	{
		CColorUtil clr(0, 0, 255);
		m_pRender->DrawRectangle(m_rcMemDC, clr);

		m_pRender->DrawText(m_rcMemDC, _T("macd"), clr);
	}

	m_pRender->EndRender();
}

void CIndicator::SetArea(CRect rcArea)
{
	m_rcArea = rcArea;

	m_rcMemDC.left = 0;
	m_rcMemDC.top = 0;
	m_rcMemDC.right = m_rcMemDC.left + m_rcArea.Width();
	m_rcMemDC.bottom = m_rcMemDC.top + m_rcArea.Height();
}

void CIndicator::KLine()
{
	if (m_pvData->size() == 0)
		return;

	m_nCaptionHeight = 20;
	m_nRightAxisWidth = 60;
	m_nInfoRadarHeight = 5;
	m_nWeightHeight = 8;

	m_nKLineWidth = 4;
	m_nSepWidth = 2;

	m_nMarginCount = 5;
	m_nMarginWidth = (m_nKLineWidth + m_nSepWidth) * m_nMarginCount;


	m_rcCaption.left = 0;
	m_rcCaption.top = 0;
	m_rcCaption.right = m_rcMemDC.right - m_nRightAxisWidth;
	m_rcCaption.bottom = m_rcCaption.top + m_nCaptionHeight; // 标题栏高度


	m_rcDraw.left = 0;
	m_rcDraw.top = m_rcCaption.bottom + m_nInfoRadarHeight;
	m_rcDraw.right = m_rcMemDC.right - m_nRightAxisWidth;
	m_rcDraw.bottom = m_rcMemDC.bottom - m_nWeightHeight;

	m_nDrawKLineCount = (m_rcDraw.Width() - m_nMarginWidth) / (m_nKLineWidth + m_nSepWidth);

	CColorUtil clr(255, 0, 0);
	m_pRender->DrawRectangle(m_rcCaption, clr);
	m_pRender->DrawText(m_rcCaption, _T("标题"), clr);

	m_pRender->DrawRectangle(m_rcDraw, clr);
	//m_pRender->DrawText(m_rcDraw, _T("kline"), clr);

	// 显示数据大于实际数据
	if (m_nDrawKLineCount > m_pvData->size())
	{
		m_nDrawKLineCount = m_pvData->size();
	}

	m_fMaxPrice = 0.0f;
	m_fMinPrice = 9999.0f;
	for (int i = (m_pvData->size() - m_nDrawKLineCount); i< m_pvData->size(); i++)
	{
		struct kline_struct& kline = m_pvData->at(i);

		if (kline.high > m_fMaxPrice)
			m_fMaxPrice = kline.high;

		if (kline.low < m_fMinPrice)
			m_fMinPrice = kline.low;
	}

	std::string log = "最高价";
	log += boost::lexical_cast<std::string>(m_fMaxPrice);
	gFileLog::instance().Log(log);

	log = "最低价";
	log += boost::lexical_cast<std::string>(m_fMinPrice);
	gFileLog::instance().Log(log);

	log = "要画的k线个数";
	log += boost::lexical_cast<std::string>(m_nDrawKLineCount);
	gFileLog::instance().Log(log);


	
	m_nRowHeight = 30.0f;
	m_nMinRows = 2;

	int nMaxRows = 0;

	// 根据高度得到行数
	nMaxRows = m_rcDraw.Height() / m_nRowHeight;
	//nMaxRows = 2;
	//std::cout << "行数" << nMaxRows << "\n";

	




	//int min = 159;
	//int max = 163;

	//int min = 146;
	//int max = 227;

	int min = m_fMinPrice * 100;
	int max = m_fMaxPrice * 100;
	// 价差
	int c = max - min;
	m_fScale = m_rcDraw.Height() / (float)c;

	//std::cout << "最小值" << min << ", 最大值" << max << "\n";


	//float tmp = nHeight % nRowHeight;

	//if (tmp != 0)
	//	nRows += 1;

	int scale = 0;
	int scalevar = 0;

	
	
	
	if (c > m_nMinRows * 5)
	{
		scalevar = 5;
	}
	else if (c > m_nMinRows * 2)
	{
		scalevar = 2;
	}
	else
	{
		scalevar = 1;
	}
	scale = scalevar;

	
	
	int nRows = 0;
	while (true)
	{
		float tmp = c / scale;
		nRows = boost::math::round(tmp);
		//std::cout << "价差=" << c << " / scale=" << scale << " = " << nRows << "\n";

		
		if (nRows > nMaxRows)
		{
			scale = scale + scalevar;
			continue;
		}
		else
		{
			break;
		}
	}

	//std::cout << "差"<< c << "，按什么来分=" << scale << ", 分成几份，画几条线=" << nRows << "\n";
	gFileLog::instance().Log("价差" + boost::lexical_cast<std::string>(c));
	gFileLog::instance().Log("刻度值" + boost::lexical_cast<std::string>(scale));
	gFileLog::instance().Log("份数" + boost::lexical_cast<std::string>(nRows));
	
	// 方案一：找到第一个，然后在找到的第一个基础上一直循环累加，可以提高性能
	// 方案二：一直循环查找，性能不好
	
	int axisValue = 0;
	for (int i=min; i < max; i++)
	{
		if (i % scale == 0)
		{
			axisValue = i;
			
			gFileLog::instance().Log("第一个坐标值=" + boost::lexical_cast<std::string>(axisValue));
			break;
		}
	}

	for (int i=0; i<nRows; i++)
	{
		if (i==0)
		{
			

			CColorUtil clr(255, 0, 0);
			float x1 = m_rcDraw.left;
			float y1 = m_rcDraw.bottom - ((axisValue - min) * m_fScale);
			float x2 = m_rcDraw.right;
			float y2 = y1;
			m_pRender->DrawLine(x1, y1, x2, y2, clr);

			CSize size = m_pRender->GetStringSize(_T("宋体"), 12, _T("0"));

			CRect rcAxisLabel;
			rcAxisLabel.left = m_rcDraw.right;
			rcAxisLabel.top = y1 - (size.cy/2);
			rcAxisLabel.right = rcAxisLabel.left + m_nRightAxisWidth;
			rcAxisLabel.bottom = rcAxisLabel.top + size.cy;

			float price = axisValue / 100.0f;
			CString strLabel;
			strLabel.Format(_T("%.2f"), price);
			m_pRender->DrawText(rcAxisLabel, strLabel, clr);
			

			gFileLog::instance().Log("全部坐标值=" + boost::lexical_cast<std::string>(axisValue));
		}
		else
		{
			axisValue = axisValue + scale;

			CColorUtil clr(255, 0, 0);
			float x1 = m_rcDraw.left;
			float y1 = m_rcDraw.bottom - ((axisValue - min) * m_fScale);
			float x2 = m_rcDraw.right;
			float y2 = y1;
			m_pRender->DrawLine(x1, y1, x2, y2, clr);

			CSize size = m_pRender->GetStringSize(_T("宋体"), 12, _T("0"));

			CRect rcAxisLabel;
			rcAxisLabel.left = m_rcDraw.right;
			rcAxisLabel.top = y1 - (size.cy/2);
			rcAxisLabel.right = rcAxisLabel.left + m_nRightAxisWidth;
			rcAxisLabel.bottom = rcAxisLabel.top + size.cy;

			float price = axisValue / 100.0f;
			CString strLabel;
			strLabel.Format(_T("%.2f"), price);
			m_pRender->DrawText(rcAxisLabel, strLabel, clr);

			gFileLog::instance().Log("全部坐标值=" + boost::lexical_cast<std::string>(axisValue));
		}
	}

	// 画k线
	float x = 1.0f;
	for (int i = (m_pvData->size() - m_nDrawKLineCount); i< m_pvData->size(); i++)
	{
		struct kline_struct& kline = m_pvData->at(i);

		int open = kline.open * 100;
		int high = kline.high * 100;
		int low = kline.low * 100;
		int close = kline.close * 100;

			float fHighY = m_rcDraw.bottom - ((high - min) * m_fScale);
			float fOpenY = m_rcDraw.bottom - ((open - min) * m_fScale);
			float fLowY = m_rcDraw.bottom - ((low - min) * m_fScale);
			float fCloseY = m_rcDraw.bottom - ((close - min) * m_fScale);

		if (close > open)
		{
			// 涨
			CColorUtil clr(255, 0, 0);
			


			m_pRender->DrawLine(x + m_nKLineWidth/2, fHighY, x + m_nKLineWidth/2, fCloseY, clr);

			CRect rcKLine;
			rcKLine.left = x;
			rcKLine.top = fCloseY;
			rcKLine.right = rcKLine.left + m_nKLineWidth;
			rcKLine.bottom = fOpenY;
			m_pRender->DrawRectangle(rcKLine, clr);

			m_pRender->DrawLine(x + m_nKLineWidth/2, fOpenY, x + m_nKLineWidth/2, fLowY, clr);
		
			x = x + m_nKLineWidth + m_nSepWidth;
		}
		else if (close < open)
		{
			// 跌
			CColorUtil clr(84, 255, 255);
			



			m_pRender->DrawLine(x + m_nKLineWidth/2, fHighY, x + m_nKLineWidth/2, fOpenY, clr);

			CRect rcKLine;
			rcKLine.left = x;
			rcKLine.top = fOpenY;
			rcKLine.right = rcKLine.left + m_nKLineWidth;
			rcKLine.bottom = fCloseY;
			m_pRender->DrawRectangle(rcKLine, clr);

			m_pRender->DrawLine(x + m_nKLineWidth/2, fCloseY, x + m_nKLineWidth/2, fLowY, clr);
		
			x = x + m_nKLineWidth + m_nSepWidth;
		}
		else
		{
			// white
			CColorUtil clr(255, 255, 255);



			m_pRender->DrawLine(x + m_nKLineWidth/2, fHighY, x + m_nKLineWidth/2, fOpenY, clr);
			
			m_pRender->DrawLine(x, fOpenY, x + m_nKLineWidth, fOpenY, clr);

			m_pRender->DrawLine(x + m_nKLineWidth/2, fCloseY, x + m_nKLineWidth/2, fLowY, clr);
		
			x = x + m_nKLineWidth + m_nSepWidth;
		}
	} // end for
}

void CIndicator::Vol()
{
	// 等差坐标
	// 除以5000*因子, 根据返回值画横线
	if (m_pvData->size() == 0)
		return;

	m_nCaptionHeight = 20;
	m_nRightAxisWidth = 60;
	m_nInfoRadarHeight = 0;
	m_nWeightHeight = 0;

	m_nKLineWidth = 4;
	m_nSepWidth = 2;

	m_nMarginCount = 5;
	m_nMarginWidth = (m_nKLineWidth + m_nSepWidth) * m_nMarginCount;


	m_rcCaption.left = 0;
	m_rcCaption.top = 0;
	m_rcCaption.right = m_rcMemDC.right - m_nRightAxisWidth;
	m_rcCaption.bottom = m_rcCaption.top + m_nCaptionHeight; // 标题栏高度


	m_rcDraw.left = 0;
	m_rcDraw.top = m_rcCaption.bottom + m_nInfoRadarHeight;
	m_rcDraw.right = m_rcMemDC.right - m_nRightAxisWidth;
	m_rcDraw.bottom = m_rcMemDC.bottom - m_nWeightHeight;

	m_nDrawKLineCount = (m_rcDraw.Width() - m_nMarginWidth) / (m_nKLineWidth + m_nSepWidth);

	CColorUtil clr(255, 0, 0);
	m_pRender->DrawRectangle(m_rcCaption, clr);
	m_pRender->DrawText(m_rcCaption, _T("标题"), clr);

	m_pRender->DrawRectangle(m_rcDraw, clr);
	//m_pRender->DrawText(m_rcDraw, _T("kline"), clr);

	// 显示数据大于实际数据
	if (m_nDrawKLineCount > m_pvData->size())
	{
		m_nDrawKLineCount = m_pvData->size();
	}

	m_lMaxVolume = 0l;
	
	for (int i = (m_pvData->size() - m_nDrawKLineCount); i< m_pvData->size(); i++)
	{
		struct kline_struct& kline = m_pvData->at(i);

		if (kline.vol > m_lMaxVolume)
			m_lMaxVolume = kline.vol;

	}

	std::string log = "最高价";
	log += boost::lexical_cast<std::string>(m_fMaxPrice);
	gFileLog::instance().Log(log);

	log = "最低价";
	log += boost::lexical_cast<std::string>(m_fMinPrice);
	gFileLog::instance().Log(log);

	log = "要画的k线个数";
	log += boost::lexical_cast<std::string>(m_nDrawKLineCount);
	gFileLog::instance().Log(log);


	
	m_nRowHeight = 30.0f;
	m_nMinRows = 1;

	int nMaxRows = 0;

	// 根据高度得到行数
	nMaxRows = m_rcDraw.Height() / m_nRowHeight;
	//nMaxRows = 2;
	//std::cout << "行数" << nMaxRows << "\n";

	




	//int min = 159;
	//int max = 163;

	//int min = 146;
	//int max = 227;

	//int min = m_fMinPrice * 100;
	//int max = m_fMaxPrice * 100;
	

	// 价差
	//int c = max - min;
	m_fScale = m_rcDraw.Height() / (m_lMaxVolume * 1.0f);

	//std::cout << "最小值" << min << ", 最大值" << max << "\n";


	//float tmp = nHeight % nRowHeight;

	//if (tmp != 0)
	//	nRows += 1;

	int scale = 0;
	int scalevar = 0;

	
	if (m_lMaxVolume > m_nMinRows * 5000)
	{
		scalevar = 5000;
	}
	else if (m_lMaxVolume > m_nMinRows * 2000)
	{
		scalevar = 1000;
	}
	else if (m_lMaxVolume > m_nMinRows * 1000)
	{
		scalevar = 1000;
	}
	else if (m_lMaxVolume > m_nMinRows * 500)
	{
		scalevar = 500;
	}
	else if (m_lMaxVolume > m_nMinRows * 200)
	{
		scalevar = 200;
	}
	else
	{
		scalevar = 100;
	}
	scale = scalevar;

	
	
	int nRows = 0;
	while (true)
	{
		float tmp = m_lMaxVolume / scale;
		nRows = boost::math::round(tmp);
		//std::cout << "价差=" << c << " / scale=" << scale << " = " << nRows << "\n";

		
		if (nRows > nMaxRows)
		{
			scale = scale + scalevar;
			continue;
		}
		else
		{
			break;
		}
	}

	//std::cout << "差"<< c << "，按什么来分=" << scale << ", 分成几份，画几条线=" << nRows << "\n";
	gFileLog::instance().Log("价差" + boost::lexical_cast<std::string>(m_lMaxVolume));
	gFileLog::instance().Log("刻度值" + boost::lexical_cast<std::string>(scale));
	gFileLog::instance().Log("份数" + boost::lexical_cast<std::string>(nRows));
	
	// 方案一：找到第一个，然后在找到的第一个基础上一直循环累加，可以提高性能
	// 方案二：一直循环查找，性能不好
	
	long axisValue = 0;
	for (long i=1; i < m_lMaxVolume; i++)
	{
		if (i % scale == 0)
		{
			axisValue = i;
			
			gFileLog::instance().Log("第一个坐标值=" + boost::lexical_cast<std::string>(axisValue));
			break;
		}
	}

	// 绘制轴文本
	for (int i=0; i<nRows; i++)
	{
		if (i==0)
		{
			

			CColorUtil clr(255, 0, 0);
			float x1 = m_rcDraw.left;
			float y1 = m_rcDraw.bottom - (axisValue * m_fScale);
			float x2 = m_rcDraw.right;
			float y2 = y1;
			m_pRender->DrawLine(x1, y1, x2, y2, clr);

			CSize size = m_pRender->GetStringSize(_T("宋体"), 12, _T("0"));

			CRect rcAxisLabel;
			rcAxisLabel.left = m_rcDraw.right;
			rcAxisLabel.top = y1 - (size.cy/2);
			rcAxisLabel.right = rcAxisLabel.left + m_nRightAxisWidth;
			rcAxisLabel.bottom = rcAxisLabel.top + size.cy;

			
			CString strLabel;
			strLabel.Format(_T("%ld"), axisValue);
			m_pRender->DrawText(rcAxisLabel, strLabel, clr);
			

			gFileLog::instance().Log("全部坐标值=" + boost::lexical_cast<std::string>(axisValue));
		}
		else
		{
			axisValue = axisValue + scale;

			CColorUtil clr(255, 0, 0);
			float x1 = m_rcDraw.left;
			float y1 = m_rcDraw.bottom - (axisValue * m_fScale);
			float x2 = m_rcDraw.right;
			float y2 = y1;
			m_pRender->DrawLine(x1, y1, x2, y2, clr);

			CSize size = m_pRender->GetStringSize(_T("宋体"), 12, _T("0"));

			CRect rcAxisLabel;
			rcAxisLabel.left = m_rcDraw.right;
			rcAxisLabel.top = y1 - (size.cy/2);
			rcAxisLabel.right = rcAxisLabel.left + m_nRightAxisWidth;
			rcAxisLabel.bottom = rcAxisLabel.top + size.cy;

			CString strLabel;
			strLabel.Format(_T("%ld"), axisValue);
			m_pRender->DrawText(rcAxisLabel, strLabel, clr);

			gFileLog::instance().Log("全部坐标值=" + boost::lexical_cast<std::string>(axisValue));
		}
	}

	// 画成交量
	float x = 1.0f;
	for (int i = (m_pvData->size() - m_nDrawKLineCount); i< m_pvData->size(); i++)
	{
		struct kline_struct& kline = m_pvData->at(i);

		int open = kline.open * 100;
		int close = kline.close * 100;

		long vol = kline.vol;

		float fVolY = m_rcDraw.bottom - (vol * m_fScale);

		if (close > open)
		{
			// 涨
			CColorUtil clr(255, 0, 0);
			
			CRect rcKLine;
			rcKLine.left = x;
			rcKLine.top = fVolY;
			rcKLine.right = rcKLine.left + m_nKLineWidth;
			rcKLine.bottom = m_rcDraw.bottom;
			m_pRender->DrawRectangle(rcKLine, clr);

		
			x = x + m_nKLineWidth + m_nSepWidth;
		}
		else if (close < open)
		{
			// 跌
			CColorUtil clr(84, 255, 255);
			
			CRect rcKLine;
			rcKLine.left = x;
			rcKLine.top = fVolY;
			rcKLine.right = rcKLine.left + m_nKLineWidth;
			rcKLine.bottom = m_rcDraw.bottom;
			m_pRender->DrawRectangle(rcKLine, clr);

		
			x = x + m_nKLineWidth + m_nSepWidth;
		}
		else
		{
			// white
			CColorUtil clr(255, 255, 255);


			CRect rcKLine;
			rcKLine.left = x;
			rcKLine.top = fVolY;
			rcKLine.right = rcKLine.left + m_nKLineWidth;
			rcKLine.bottom = m_rcDraw.bottom;
			m_pRender->DrawRectangle(rcKLine, clr);

		
			x = x + m_nKLineWidth + m_nSepWidth;
		}


	} // end for
}

void CIndicator::MACD()
{


	
}
