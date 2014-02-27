// TickChartWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "TickChartWnd.h"
#include "importheader.h"

#include <gdiplus.h>
#include "GDIRenderEngine.h"
#include "Direct2DRenderEngine.h"
#include "tcp_message.h"
#include "DataManager.h"
#include "FileLog.h"
#include "ColorUtil.h"

#include "FinanceProduct.h"
#include "market.h"


// CTickChartWnd

IMPLEMENT_DYNAMIC(CTickChartWnd, CBaseControlWnd)

CTickChartWnd::CTickChartWnd()
{
		m_pRender = NULL;

	m_nCaptionSeparate = 10;
	m_nCaptionHeight = 12;
	m_nTimeAxisHeight = 12;
	m_nMaxRowHeight = 25;

	//CSize size = GetStringSize(_T("Gabriola"), 12, _T("1234567"));
	m_nYAxisWidth = 60;
	m_nCols = 8;
	m_dCjslMax = 1000;
}

CTickChartWnd::~CTickChartWnd()
{
}


BEGIN_MESSAGE_MAP(CTickChartWnd, CBaseControlWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CTickChartWnd 消息处理程序




void CTickChartWnd::ShowQuote(CFinanceProduct& financeProduct)
{

	quote::MinRequest tick_request;

	tick_request.set_stockcode(financeProduct.GetFinanceProduct());
	
	tick_request.add_day(0);
	tick_request.set_day(0, 0);

	int nPkgBodySize = tick_request.ByteSize();


	tcp_request_ptr request = new tcp_request();
	request->set_body_size(nPkgBodySize);
	tick_request.SerializeToArray(&(request->msg_body.front()), nPkgBodySize);
		
	request->encode_header(quote::PkgHeader::REQ_MIN, nPkgBodySize);
	request->SetView(this);

	//sDataManager::instance().PushRequestToQueue(request);

}

void CTickChartWnd::RecvResponseCallback(tcp_message * response)
{
	gFileLog::instance().Log("CTickChartWnd视图层收到应答");
	response->OutputMsgType();

	int size = sizeof(struct tick_struct_raw);
	int count = response->msg_body.size() / size;
	m_nDataCount = count;
	
	
	

	for (int i=0; i<count; i++)
	{
		struct tick_struct_raw raw;
		memcpy(&raw, &(response->msg_body.front()) + (i*size), size);


		struct struct_tick tick;
		tick.cjjeTotal = raw.cjje;
		tick.cjslTotal = raw.cjsl;
		tick.leading_index = raw.leading_index;
		tick.pre_close = raw.pre_close;
		tick.quote_time = raw.quote_time;
		tick.xj = raw.xj;
		
		tick.avg = 0.0f;
		tick.cjje = 0.0f;
		tick.cjsl = 0.0f;
		
		std::string key = boost::lexical_cast<std::string>(tick.quote_time);
		key = key.substr(8, 6);

		long lKey = boost::lexical_cast<long>(key);
		data[lKey] = tick;

		

	}
	
	UpdateQuote();
}

void CTickChartWnd::UpdateQuote()
{
	// 得到成交量最大值
	std::map<long long, struct struct_tick>::iterator it = data.begin();
	
	
	double dCjslTotalPrev = 0.0f;
	double dCjjeTotalPrev = 0.0f;

	m_fMaxPrice = 0.0f;
	m_fMinPrice = 0.0f;
	m_fClosePrice = 0.0f;

	for (; it != data.end(); it++)
	{
		if (it == data.begin())
		{
			m_dCjslMax = it->second.cjslTotal;
			dCjslTotalPrev = it->second.cjslTotal;

			dCjjeTotalPrev = it->second.cjjeTotal;

			it->second.cjje = it->second.cjjeTotal;
			it->second.cjsl = it->second.cjslTotal;
			it->second.avg = it->second.cjjeTotal / it->second.cjslTotal / 100;


			m_fClosePrice = it->second.pre_close;

			//fMaxPricePrev = fMinPricePrev = 
			m_fMaxPrice = m_fMinPrice = m_fClosePrice;

			if (it->second.xj >= m_fClosePrice)
			{
				m_fMaxPrice = it->second.xj;
				//fMaxPricePrev = m_fMaxPrice;
				
			}

			if (it->second.xj <= m_fClosePrice)
			{
				m_fMinPrice = it->second.xj;
				//fMinPricePrev = m_fMinPrice;
			}

			continue;
		}
		else
		{
			

			struct struct_tick& current = it->second;
			
			// 算出成交数量
			double cjslTotal = current.cjslTotal;
			current.cjsl = cjslTotal - dCjslTotalPrev;
			dCjslTotalPrev = cjslTotal;

			// 算出成交金额
			double cjjeTotal = current.cjjeTotal;
			current.cjje = cjjeTotal - dCjjeTotalPrev;
			dCjjeTotalPrev = cjjeTotal;

			current.avg = current.cjjeTotal / current.cjslTotal/100;
			TRACE("均价:%lf\n", current.avg );

			if (current.cjsl > m_dCjslMax)
				m_dCjslMax = current.cjsl;

			//TRACE("当前key=%s, cjsl=%lf\n", boost::lexical_cast<std::string>(it->first).c_str(), current.cjsl);

			float xj = current.xj;
			if (xj >= m_fMaxPrice)
			{
				m_fMaxPrice = xj;
			}

			if (xj <= m_fMinPrice)
				m_fMinPrice = xj;
		}
	} // end for

	
	if ( (m_fMaxPrice - m_fClosePrice) > (m_fClosePrice - m_fMinPrice))
	{
		m_fMinPrice = m_fClosePrice - (m_fMaxPrice - m_fClosePrice);
	}
	else
	{
		m_fMaxPrice = m_fClosePrice + (m_fClosePrice - m_fMinPrice);
	}

	Render();
	
}

void CTickChartWnd::DrawCaption()
{
	LPCTSTR name = _T("浦发银行");
	

	CRect rcName;
	rcName.left = m_nYAxisWidth;
	rcName.top = 0;
	CSize size = GetStringSize(_T("宋体"), 12, name);
	
	

	rcName.right = rcName.left + size.cx + 10;
	rcName.bottom = rcName.top + m_nCaptionHeight;
	
	CColorUtil clr(192,192, 192);
	m_pRender->DrawText(rcName, name, clr);
	
	CRect rcTick;
	rcTick.left = rcName.right;
	rcTick.top = 0;
	rcTick.right = rcTick.left + 100;
	rcTick.bottom = rcName.top + m_nCaptionHeight;
	LPCTSTR tick = _T("分时");
	CColorUtil clrTick(255, 255, 255);
	m_pRender->DrawText(rcTick, tick, clrTick);
	

	CRect rcAvg;
	rcAvg.left = rcTick.right;
	rcAvg.top = 0;
	rcAvg.right = rcAvg.left + 100;
	rcAvg.bottom = rcAvg.top + m_nCaptionHeight;
	LPCTSTR avg = _T("均线");
	CColorUtil clrAvg(255, 255, 0);
	m_pRender->DrawText(rcAvg, avg, clrAvg);

	CRect rcVolume;
	rcVolume.left = rcAvg.right;
	rcVolume.top = 0;
	rcVolume.right = rcVolume.left + 100;
	rcVolume.bottom = rcVolume.top + m_nCaptionHeight;
	TRACE("标题高度%d\n", rcVolume.Height());
	LPCTSTR volume = _T("成交量");
	CColorUtil clrVolume(192, 192, 0);
	m_pRender->DrawText(rcVolume, volume, clrVolume);
}



void CTickChartWnd::DrawTick()
{
	rcTick.left = m_nYAxisWidth;
	rcTick.top = m_nCaptionHeight;

	rcTick.right = rcTick.left + (rcClient.Width() - m_nYAxisWidth*2);
	m_nColWidth = rcTick.Width() / m_nCols;
	TRACE("rc Width old: %d\n", rcTick.Width());
	TRACE(" nTimeInterval %d\n", m_nColWidth);
	rcTick.right = rcTick.left + (m_nColWidth * m_nCols);
	TRACE("rc Width new: %d\n", rcTick.Width());
	
	
	// 最小行高
	
	int nMinRowHeight = 10;
	

	m_nDrawAreaHeight = rcClient.Height() - m_nCaptionHeight - m_nTimeAxisHeight;
	rcTick.bottom = rcTick.top + m_nDrawAreaHeight * 0.7;

	// 根据高度，得到行数
	// 如果不能整除，需要增加新行
	m_nTickRows = rcTick.Height() / m_nMaxRowHeight;
	if ( (rcTick.Height() % m_nMaxRowHeight) != 0 )
	{
		m_nTickRows += 1;
	}
	if ((m_nTickRows % 2) != 0)
	{
		// 减少行数，意为着增加行高
		m_nTickRows -= 1;  

		// 增加行数，意为着减少行高
		//nRows += 1; 
	}
	TRACE("得到行数：%d\n", m_nTickRows);

	// 得到行高
	float nRowHeight = rcTick.Height() / m_nTickRows;
	TRACE("行高：%f\n", nRowHeight);

	// 最终的高度
	rcTick.bottom = rcTick.top + nRowHeight * m_nTickRows;

	CColorUtil clr(255, 0, 0);
	
	// 画横线


	for (int row=0; row < (m_nTickRows+1); row++)
	{
		m_pRender->DrawLine(rcTick.left, rcTick.top + (row * nRowHeight), rcTick.right, rcTick.top + (row * nRowHeight), clr);

		if (row == (m_nTickRows /2) )
		{
			m_pRender->DrawLine(rcTick.left, rcTick.top + (row * nRowHeight), rcTick.right, rcTick.top + (row * nRowHeight), clr);
			
		}
	}

	// 画竖线
	for (int col=0; col < (m_nCols+1); col++)
	{
		m_pRender->DrawLine(rcTick.left + (col*m_nColWidth), rcTick.top, rcTick.left + (col*m_nColWidth), rcTick.bottom, clr);
	}

	if (data.size() <= 0)
		return;

	CSize size = GetStringSize(_T("宋体"), 12, _T("0"));
	m_fTickMiddle = rcTick.top + ((m_nTickRows/2) * nRowHeight);

	CRect rcTickLabelLeft;
	rcTickLabelLeft.left = 0;
	rcTickLabelLeft.right = rcTickLabelLeft.left + m_nYAxisWidth;
	rcTickLabelLeft.top = m_fTickMiddle - size.cy/2; // 居中
	rcTickLabelLeft.bottom = rcTickLabelLeft.top + size.cy;

	CRect rcTickLabelRight;
	rcTickLabelRight.left = rcTick.right;
	rcTickLabelRight.right = rcTickLabelRight.left + m_nYAxisWidth;
	rcTickLabelRight.top = m_fTickMiddle - size.cy/2; // 居中
	rcTickLabelRight.bottom = rcTickLabelRight.top + size.cy;

	CColorUtil clrTickVolLabel(255,255,255);

	CString label;
	float fTickScale = (m_fMaxPrice - m_fClosePrice) / (m_nTickRows / 2);

	CString lblPercent;
	for (int row=0; row < (m_nTickRows/2 + 1); row++)
	{
		float tmp = m_fClosePrice + fTickScale * row;
		label.Format(_T("%.2f "), tmp);

		float fPercent = (tmp - m_fClosePrice) / m_fClosePrice * 100;
		lblPercent.Format(_T("%.2f%%"), fPercent);
		

		rcTickLabelLeft.top = m_fTickMiddle - row * nRowHeight - size.cy/2;
		rcTickLabelLeft.bottom = rcTickLabelLeft.top + size.cy;

		rcTickLabelRight.top = rcTickLabelLeft.top;
		rcTickLabelRight.bottom = rcTickLabelLeft.bottom;

		if (row == 0)
		{
			CColorUtil clrTickVolLabel(255,255,255);
			m_pRender->DrawText(rcTickLabelLeft, label, clrTickVolLabel, TEXT_ALIGN_HORZ_RIGHT, TEXT_ALIGN_VERT_CENTER);

			m_pRender->DrawText(rcTickLabelRight, lblPercent, clrTickVolLabel, TEXT_ALIGN_HORZ_LEFT, TEXT_ALIGN_VERT_CENTER);
		}
		else
		{
			CColorUtil clrTickVolLabel(255,0,0);
			m_pRender->DrawText(rcTickLabelLeft, label, clrTickVolLabel, TEXT_ALIGN_HORZ_RIGHT, TEXT_ALIGN_VERT_CENTER);

			m_pRender->DrawText(rcTickLabelRight, lblPercent, clrTickVolLabel, TEXT_ALIGN_HORZ_LEFT, TEXT_ALIGN_VERT_CENTER);
		}
	}

	// 画跌的部分
	CRect rcTickLabelLowerLeft;
	rcTickLabelLowerLeft.left = 0;
	rcTickLabelLowerLeft.right = rcTickLabelLowerLeft.left + m_nYAxisWidth;
	rcTickLabelLowerLeft.top = m_fTickMiddle - size.cy/2; // 居中
	rcTickLabelLowerLeft.bottom = rcTickLabelLowerLeft.top + size.cy;

	CRect rcTickLabelLowerRight;
	rcTickLabelLowerRight.left = rcTick.right;
	rcTickLabelLowerRight.right = rcTickLabelLowerRight.left + m_nYAxisWidth;
	rcTickLabelLowerRight.top = m_fTickMiddle - size.cy/2; // 居中
	rcTickLabelLowerRight.bottom = rcTickLabelLowerRight.top + size.cy;

	//CColorUtil clrTickVolLabel(255,255,255);

	//CString label;
	//float fTickScale = (m_fMaxPrice - m_fClosePrice) / (m_nTickRows / 2);

	//CString lblPercent;
	for (int row=0; row < m_nTickRows/2; row++)
	{
		float tmp = m_fClosePrice - fTickScale * row;
		label.Format(_T("%.2f "), tmp);

		float fPercent = (tmp - m_fClosePrice) / m_fClosePrice * 100;
		if (fPercent < 0)
			fPercent = fPercent * -1;

		lblPercent.Format(_T("%.2f%%"), fPercent);
		

		rcTickLabelLowerLeft.top = m_fTickMiddle + row * nRowHeight - size.cy/2;
		rcTickLabelLowerLeft.bottom = rcTickLabelLowerLeft.top + size.cy;

		rcTickLabelLowerRight.top = rcTickLabelLowerLeft.top;
		rcTickLabelLowerRight.bottom = rcTickLabelLowerLeft.bottom;

		if (row == 0)
		{
			CColorUtil clrTickVolLabel(255,255,255);
			m_pRender->DrawText(rcTickLabelLowerLeft, label, clrTickVolLabel, TEXT_ALIGN_HORZ_RIGHT, TEXT_ALIGN_VERT_CENTER);

			m_pRender->DrawText(rcTickLabelLowerRight, lblPercent, clrTickVolLabel, TEXT_ALIGN_HORZ_LEFT, TEXT_ALIGN_VERT_CENTER);
		}
		else
		{
			CColorUtil clrTickVolLabel(0,255,0);
			m_pRender->DrawText(rcTickLabelLowerLeft, label, clrTickVolLabel, TEXT_ALIGN_HORZ_RIGHT, TEXT_ALIGN_VERT_CENTER);

			m_pRender->DrawText(rcTickLabelLowerRight, lblPercent, clrTickVolLabel, TEXT_ALIGN_HORZ_LEFT, TEXT_ALIGN_VERT_CENTER);
		}
	}

	//画分时线
	long long key = 0;

	float fTickX1 = 0.0f;
	float fTickY1 = 0.0f;
	float fTickX2 = 0.0f;
	float fTickY2 = 0.0f;

	float fAvgX1 = 0.0f;
	float fAvgY1 = 0.0f;
	float fAvgX2 = 0.0f;
	float fAvgY2 = 0.0f;

	float fWidthScale = rcTick.Width() / 240.0f;
	TRACE("总宽%d, 宽度%d, 比例%f， 列宽%d\n", rcClient.Width(), rcTick.Width(), fWidthScale, m_nColWidth);

	int i=0;
	float fTickHeightScale = rcTick.Height() / 2 / (m_fMaxPrice - m_fClosePrice);

	boost::gregorian::date day = boost::gregorian::day_clock::local_day();

	std::string sForenoon = boost::gregorian::to_iso_extended_string(day);
	sForenoon += " 09:30:00";

	boost::posix_time::ptime ptForenoon;
	ptForenoon = boost::posix_time::time_from_string(sForenoon);

	boost::posix_time::time_iterator itForenoon(ptForenoon, boost::posix_time::minutes(1));
	for (; itForenoon < ptForenoon + boost::posix_time::hours(3); itForenoon++)
	{
		if (itForenoon->time_of_day().hours() == 12)
			continue;
		if ( itForenoon->time_of_day().hours() == 11 && itForenoon->time_of_day().minutes() > 30 )
			continue;

		//std::string sKey = boost::gregorian::to_iso_string(itForenoon->date()) + boost::posix_time::to_iso_string(itForenoon->time_of_day());
		std::string sKey = boost::posix_time::to_iso_string(itForenoon->time_of_day());
		if (sKey.length() != 6)
			sKey = "0" + sKey;
		//TRACE("上午的数据%s\n", sKey.c_str());

		key = boost::lexical_cast<long long>(sKey);

		std::map<long long, struct struct_tick>::iterator it = data.find(key);
		if (it == data.end())
		{
			// 可能由于停牌原因，需要跳过，但是位置需要向后移动
			TRACE("上午找不到的数据%s\n", sKey.c_str());

			fTickX2 = rcTick.left + i*fWidthScale;

			fAvgX2 = rcTick.left + i*fWidthScale;

			continue;
		}
		//TRACE("key=%s， cjsl=%lf\n", sKey.c_str(), it->second.cjsl);
		
		
		fTickX2 = rcTick.left + i * fWidthScale;

		fAvgX2 = rcTick.left + i * fWidthScale;

		struct struct_tick& tick = it->second;
		
		

		

		

		if (tick.xj > m_fClosePrice)
		{
			float fHeight = (tick.xj - m_fClosePrice) * fTickHeightScale;
			fTickY2 = m_fTickMiddle - fHeight;
		}
		else if (tick.xj < m_fClosePrice)
		{
			float fHeight = (m_fClosePrice - tick.xj) * fTickHeightScale;
			fTickY2 = m_fTickMiddle + fHeight;
		}
		else if (tick.xj == m_fClosePrice)
		{
			fTickY2 = m_fTickMiddle;
		}

		// 算均线
		//float fAvgPrice = tick.cjje / tick.cjsl / 100;//问一下肖波，除100?
		//TRACE("均价：%f\n", fAvgPrice);

		if (tick.avg > m_fClosePrice)
		{
			float fHeight = (tick.avg - m_fClosePrice) * fTickHeightScale;
			fAvgY2 = m_fTickMiddle - fHeight;
		}
		else if (tick.avg < m_fClosePrice)
		{
			float fHeight = (m_fClosePrice - tick.avg) * fTickHeightScale;
			fAvgY2 = m_fTickMiddle + fHeight;
		}
		else if (tick.avg == m_fClosePrice)
		{
			fAvgY2 = m_fTickMiddle;
		}

		if (i==0)
		{
			// 第一笔
			fTickX1 = fTickX2;
			fTickY1 = fTickY2;


			fAvgX1 = fAvgX2;
			fAvgY1 = fAvgY2;
		}
		else
		{
			CColorUtil clrTickLine(255, 255, 255);
			m_pRender->DrawLine(fTickX1, fTickY1, fTickX2, fTickY2, clrTickLine);
			fTickX1 = fTickX2;
			fTickY1 = fTickY2;

			CColorUtil clrAvgLine(192, 192, 0);
			m_pRender->DrawLine(fAvgX1, fAvgY1, fAvgX2, fAvgY2, clrAvgLine);
			fAvgX1 = fAvgX2;
			fAvgY1 = fAvgY2;
		}
		
		i++;

	}

	TRACE("实际笔数%d, 已画笔数=%d\n", data.size(), i);

	std::string sAfternoon = boost::gregorian::to_iso_extended_string(day);
	sAfternoon += " 13:01:00";

	boost::posix_time::ptime ptAfternoon;
	ptAfternoon = boost::posix_time::time_from_string(sAfternoon);

	boost::posix_time::time_iterator itAfternoon(ptAfternoon, boost::posix_time::minutes(1));
	for (; itAfternoon < ptAfternoon + boost::posix_time::hours(2); itAfternoon++)
	{
		//std::string sKey = boost::gregorian::to_iso_string(itAfternoon->date()) + boost::posix_time::to_iso_string(itAfternoon->time_of_day());
		std::string sKey = boost::posix_time::to_iso_string(itAfternoon->time_of_day());
		if (sKey.length() != 6)
			sKey = "0" + sKey;
		//TRACE("下午的数据%s\n", sKey.c_str());

		key = boost::lexical_cast<long long>(sKey);

		std::map<long long, struct struct_tick>::iterator it = data.find(key);
		if (it == data.end())
		{
			// 可能由于停牌原因，需要跳过，但是位置需要向后移动
			TRACE("下午找不到的数据%s\n", sKey.c_str());
			fTickX2 = rcTick.left + i * fWidthScale;
			fAvgX2 = rcTick.left + i * fWidthScale;

			continue;
		}
		//TRACE("key=%s， cjsl=%lf\n", sKey.c_str(), it->second.cjsl);
		
		fTickX2 = rcTick.left + i * fWidthScale;
		fAvgX2 = rcTick.left + i * fWidthScale;

		struct struct_tick& tick = it->second;
		//float fTickScale = rcTick.Height() / 2 / m_fMaxPrice;

		

		

		if (tick.xj > m_fClosePrice)
		{
			float fHeight = (tick.xj - m_fClosePrice) * fTickHeightScale;
			fTickY2 = m_fTickMiddle - fHeight;
		}
		else if (tick.xj < m_fClosePrice)
		{
			float fHeight = (m_fClosePrice - tick.xj) * fTickHeightScale;
			fTickY2 = m_fTickMiddle + fHeight;
		}
		else if (tick.xj == m_fClosePrice)
		{
			fTickY2 = m_fTickMiddle;
		}

		// 算均线
		//float fAvgPrice = tick.cjje / tick.cjsl / 100;//问一下肖波，除100?
		//TRACE("均价：%f\n", fAvgPrice);

		if (tick.avg > m_fClosePrice)
		{
			float fHeight = (tick.avg - m_fClosePrice) * fTickHeightScale;
			fAvgY2 = m_fTickMiddle - fHeight;
		}
		else if (tick.avg < m_fClosePrice)
		{
			float fHeight = (m_fClosePrice - tick.avg) * fTickHeightScale;
			fAvgY2 = m_fTickMiddle + fHeight;
		}
		else if (tick.avg == m_fClosePrice)
		{
			fAvgY2 = m_fTickMiddle;
		}

		if (i==0)
		{
			// 第一笔
			fTickX1 = fTickX2;
			fTickY1 = fTickY2;

			fAvgX1 = fAvgX2;
			fAvgY1 = fAvgY2;
		}
		else
		{
			CColorUtil clrLine(255, 255, 255);
			m_pRender->DrawLine(fTickX1, fTickY1, fTickX2, fTickY2, clrLine);

			fTickX1 = fTickX2;
			fTickY1 = fTickY2;

			CColorUtil clrAvgLine(192, 192, 0);
			m_pRender->DrawLine(fAvgX1, fAvgY1, fAvgX2, fAvgY2, clrAvgLine);
			fAvgX1 = fAvgX2;
			fAvgY1 = fAvgY2;

		}
		
		i++;

	}
	
	TRACE("实际笔数%d, 已画笔数=%d\n", m_nDataCount, i);

}

void CTickChartWnd::DrawVolume()
{

	rcVolume.left = m_nYAxisWidth;
	rcVolume.top = rcTick.bottom;
	
	rcVolume.right = rcTick.right;
	rcVolume.bottom = rcVolume.top + (m_nDrawAreaHeight - rcTick.Height());
	TRACE("成交量bottomy调整前:%d\n", rcVolume.bottom);

	// 根据高度，得到行数
	// 如果不能整除，需要增加新行
	int nRows = rcVolume.Height() / m_nMaxRowHeight;
	if ( (rcVolume.Height() % m_nMaxRowHeight) != 0 )
	{
		nRows += 1;
	}
	TRACE("成交量行数：%d\n", nRows);

	// 得到行高
	float nRowHeight = rcVolume.Height() / nRows;
	TRACE("成交量行高：%f\n", nRowHeight);

	// 最终的高度
	rcVolume.bottom = rcVolume.top + nRowHeight * nRows;
	TRACE("成交量bottom调整后:%d\n", rcVolume.bottom);


	CColorUtil clr(255, 0, 0);

	// 画横线
	for (int row=0; row < (nRows+1); row++)
	{
		m_pRender->DrawLine(rcVolume.left, rcVolume.top + (row * nRowHeight), rcVolume.right, rcVolume.top + (row * nRowHeight), clr);
	}

	
	
	// 画竖线
	for (int col=0; col < (m_nCols+1); col++)
	{
		m_pRender->DrawLine(rcVolume.left + (col*m_nColWidth), rcVolume.top, rcVolume.left + (col*m_nColWidth), rcVolume.bottom, clr);
	}

	// 画刻度文本
	//m_dCjslMax = 1000;
	m_fVolumeScale = rcVolume.Height() / m_dCjslMax;
	TRACE("成交量比例值：%f\n", m_fVolumeScale);

	CRect rcVolLabelRight, rcVolLabelLeft;

	CColorUtil clrVolLabel(192, 192, 0);

	CSize size = GetStringSize(_T("宋体"), 12, _T("09:30"));
	int y = rcVolume.top - size.cy / 2;

	for (int nRow=nRows, i=0; nRow>0; nRow--, i++)
	{
		int tmp = nRow * nRowHeight / m_fVolumeScale;
		CString label;
		label.Format(_T("%d"), tmp);
		OutputDebugString(label);

		rcVolLabelRight.left = rcVolume.right;
		rcVolLabelRight.right = rcVolLabelRight.left + m_nYAxisWidth;
		rcVolLabelRight.top = y + (i * nRowHeight);
		rcVolLabelRight.bottom = rcVolLabelRight.top + m_nCaptionHeight;
		m_pRender->DrawText(rcVolLabelRight, label, clrVolLabel);

		rcVolLabelLeft.left = 0;
		rcVolLabelLeft.right = rcVolLabelLeft.left + m_nYAxisWidth;
		rcVolLabelLeft.top = y + (i * nRowHeight);
		rcVolLabelLeft.bottom = rcVolLabelLeft.top + m_nCaptionHeight;
		m_pRender->DrawText(rcVolLabelLeft, label, clrVolLabel, TEXT_ALIGN_HORZ_RIGHT);
	}

	// 画数据
	if (data.size() <= 0)
		return;

	long long key = 0;

	float x1 = rcVolume.left;
	float x2 = x1;
	float y1 = 0.0f;
	float y2 = 0.0f;

	float fWidthScale = rcVolume.Width() / 240.0f;
	TRACE("总宽%d, 宽度%d, 比例%f， 列宽%d\n", rcClient.Width(), rcVolume.Width(), fWidthScale, m_nColWidth);

	int i=0;

	boost::gregorian::date day = boost::gregorian::day_clock::local_day();

	std::string sForenoon = boost::gregorian::to_iso_extended_string(day);
	sForenoon += " 09:30:00";

	boost::posix_time::ptime ptForenoon;
	ptForenoon = boost::posix_time::time_from_string(sForenoon);

	boost::posix_time::time_iterator itForenoon(ptForenoon, boost::posix_time::minutes(1));
	for (; itForenoon < ptForenoon + boost::posix_time::hours(3); itForenoon++)
	{
		if (itForenoon->time_of_day().hours() == 12)
			continue;
		if ( itForenoon->time_of_day().hours() == 11 && itForenoon->time_of_day().minutes() > 30 )
			continue;

		//std::string sKey = boost::gregorian::to_iso_string(itForenoon->date()) + boost::posix_time::to_iso_string(itForenoon->time_of_day());
		std::string sKey = boost::posix_time::to_iso_string(itForenoon->time_of_day());
		if (sKey.length() != 6)
			sKey = "0" + sKey;
		//TRACE("上午的数据%s\n", sKey.c_str());

		key = boost::lexical_cast<long long>(sKey);

		std::map<long long, struct struct_tick>::iterator it = data.find(key);
		if (it == data.end())
		{
			// 可能由于停牌原因，需要跳过，但是位置需要向后移动
			TRACE("上午找不到的数据%s\n", sKey.c_str());
			x1 = x1 + fWidthScale;
			continue;
		}
		//TRACE("key=%s， cjsl=%lf\n", sKey.c_str(), it->second.cjsl);
		
		

		struct struct_tick& tick = it->second;
		float nHeight = tick.cjsl * m_fVolumeScale;
		y1 = rcVolume.bottom - nHeight;
		x2 = x1;
		y2 = rcVolume.bottom;

		CColorUtil clrLine(192, 192, 0);
		m_pRender->DrawLine(x1, y1, x2, y2, clrLine);
		x1 = x1 + fWidthScale;
		
		i++;

	}

	TRACE("实际笔数%d, 已画笔数=%d\n", data.size(), i);

	std::string sAfternoon = boost::gregorian::to_iso_extended_string(day);
	sAfternoon += " 13:01:00";

	boost::posix_time::ptime ptAfternoon;
	ptAfternoon = boost::posix_time::time_from_string(sAfternoon);

	boost::posix_time::time_iterator itAfternoon(ptAfternoon, boost::posix_time::minutes(1));
	for (; itAfternoon < ptAfternoon + boost::posix_time::hours(2); itAfternoon++)
	{
		//std::string sKey = boost::gregorian::to_iso_string(itAfternoon->date()) + boost::posix_time::to_iso_string(itAfternoon->time_of_day());
		std::string sKey = boost::posix_time::to_iso_string(itAfternoon->time_of_day());
		if (sKey.length() != 6)
			sKey = "0" + sKey;
		//TRACE("下午的数据%s\n", sKey.c_str());

		key = boost::lexical_cast<long long>(sKey);

		std::map<long long, struct struct_tick>::iterator it = data.find(key);
		if (it == data.end())
		{
			// 可能由于停牌原因，需要跳过，但是位置需要向后移动
			TRACE("下午找不到的数据%s\n", sKey.c_str());
			x1 = x1 + fWidthScale;
			continue;
		}
		//TRACE("key=%s， cjsl=%lf\n", sKey.c_str(), it->second.cjsl);
		
		

		struct struct_tick& tick = it->second;
		float nHeight = tick.cjsl * m_fVolumeScale;
		y1 = rcVolume.bottom - nHeight;
		x2 = x1;
		y2 = rcVolume.bottom;

		CColorUtil clrLine(192, 192, 0);
		m_pRender->DrawLine(x1, y1, x2, y2, clrLine);
		x1 = x1 + fWidthScale;

		i++;
	}
	
	TRACE("实际笔数%d, 已画笔数=%d\n", m_nDataCount, i);
}

void CTickChartWnd::DrawTimeAxis()
{
	std::list<LPCTSTR> timeAxisLabel;
	timeAxisLabel.push_back(_T("09:30"));
	timeAxisLabel.push_back(_T("10:00"));
	timeAxisLabel.push_back(_T("10:30"));
	timeAxisLabel.push_back(_T("11:00"));
	timeAxisLabel.push_back(_T("13:00"));
	timeAxisLabel.push_back(_T("13:30"));
	timeAxisLabel.push_back(_T("14:00"));
	timeAxisLabel.push_back(_T("14:30"));
	timeAxisLabel.push_back(_T("15:00"));

	CSize size = GetStringSize(_T("Gabriola"), 12, _T("09:30"));
	int nTimeAxisLableWidth = size.cx; // 稍微放大一点，不然会换行
	//TRACE("时间轴文本宽度：%d, 高度:%d\n", nTimeAxisLableWidth, size.cy);

	CRect rcTimeAxis;
	rcTimeAxis.left = m_nYAxisWidth; 
	rcTimeAxis.top = rcVolume.bottom;
	//rcTimeAxis.top = rcClient.Height() - m_nTimeAxisHeight;
	rcTimeAxis.right = rcTimeAxis.left + nTimeAxisLableWidth;
	rcTimeAxis.bottom = rcTimeAxis.top + m_nTimeAxisHeight;
	
	//TRACE("时间轴高度%d\n", rcTimeAxis.Height());

	CColorUtil clrTimeAxisLabel(255, 0, 0);

	int i=0;
	int x = m_nYAxisWidth - nTimeAxisLableWidth/2;

	for (std::list<LPCTSTR>::iterator it=timeAxisLabel.begin(); it != timeAxisLabel.end(); ++it)
	{
		LPCTSTR label = *it;

		rcTimeAxis.left = x + i*m_nColWidth;
		i++;
		rcTimeAxis.right = rcTimeAxis.left + nTimeAxisLableWidth + 10;

		m_pRender->DrawText(rcTimeAxis, label, clrTimeAxisLabel);
	}

}

CSize CTickChartWnd::GetStringSize(CString FontName, int FontSize, CString str)
{
	//CPaintDC dc(this);
	
	CDC *pDC = GetDC();
	if (!pDC)
		return CSize(0,0);

	CFont font;
	font.CreatePointFont(FontSize, FontName);
	//dc.SelectObject(font);

	int nDPIx = pDC->GetDeviceCaps(LOGPIXELSX);
	int nDPIy = pDC->GetDeviceCaps(LOGPIXELSY);

	int textWidth = pDC->GetTextExtent(str).cx * 72 / nDPIx;
	int textHeight = pDC->GetTextExtent(str).cy * 72 / nDPIy;

	CSize size;
	size.SetSize(textWidth, textHeight);
	return size;
}

void CTickChartWnd::Render()
{
	if (m_pRender)
	{
		TRACE("Render\n");

		m_pRender->BeginRender();

		GetClientRect(rcClient);

		DrawCaption();
		DrawTick();
		DrawVolume();
		DrawTimeAxis();

		m_pRender->EndRender();
	}
}


void CTickChartWnd::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseWnd::OnPaint()

	Render();
}


void CTickChartWnd::OnSize(UINT nType, int cx, int cy)
{
	CBaseWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( m_pRender && cx && cy )
	{
		//Render();
		m_pRender->OnResize( cx, cy );
	}
}


int CTickChartWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseControlWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pRender = new Direct2DRenderEngine(m_hWnd);
	m_pRender->Initialize();

	
	CFinanceProduct code("600000", MARKET_SH);
	//ShowQuote(code);

	return 0;
}


//void CTickChartWnd::OnDestroyClipboard()
//{
//	CBaseControlWnd::OnDestroyClipboard();
//
//	// TODO: 在此处添加消息处理程序代码
//}


void CTickChartWnd::OnDestroy()
{
	CBaseControlWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	
}
