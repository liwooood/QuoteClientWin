#include "stdafx.h"
#include "GDIRenderEngine.h"


GDIRenderEngine::GDIRenderEngine(HWND hWnd)
	:m_hWnd(hWnd)
{
}


GDIRenderEngine::~GDIRenderEngine(void)
{
}


void GDIRenderEngine::Render()
{
}



HRESULT GDIRenderEngine::Initialize()
{
	HRESULT hr = S_OK;

	m_font.CreatePointFont(100, _T("ËÎÌו"));

	return hr;
}

HRESULT GDIRenderEngine::BeginRender(LPRECT rcClient)
{
	HRESULT hr = S_OK;

	if (rcClient == NULL)
		GetClientRect(m_hWnd, m_rcClient);
	else
		m_rcClient = rcClient;

	
	//m_hDC = BeginPaint(m_hWnd, &ps );
	
	m_hDC = GetDC(m_hWnd);

	
	
	m_hMemDC = ::CreateCompatibleDC(m_hDC);

	
	m_bBmp = CreateCompatibleBitmap(m_hDC, m_rcClient.Width(), m_rcClient.Height());

	m_hOldBmp = (HBITMAP) SelectObject(m_hMemDC, m_bBmp);

	

	HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
	::FillRect(m_hMemDC, m_rcClient, hBrush);
	DeleteObject(hBrush);

	::SetBkMode(m_hMemDC, TRANSPARENT);
	::SetBkColor(m_hMemDC, RGB(255,255,255));

//	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0,0,255));
//	SelectObject(MemDC, hPen);
//	CPoint pt;
//	::MoveToEx(MemDC, 0, 0, &pt);
//	::LineTo(MemDC, 100, 100);
	//int nHeight = -MulDiv(12, GetDeviceCaps(m_hDC, LOGPIXELSY), 72);
	//m_font.CreatePointFont(12, _T("ËÎÌו"));
	//m_hFont = CreateFont(nHeight, 0, 0, 0, 0, FALSE, FALSE, FALSE, 0, 0, 0, 0, 0, _T("ËÎÌו"));
	//::CreateFont(
	//::CreateFontIndirectEx(
	
	
	m_hOldFont = (HFONT)::SelectObject(m_hMemDC, m_font.GetSafeHandle());

	return hr;
}

HRESULT GDIRenderEngine::EndRender()
{
	HRESULT hr = S_OK;

	
	//StretchBlt(m_hDC, 20, 20, m_rcClient.Width(), m_rcClient.Height(), m_hMemDC, m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), SRCCOPY);
	//StretchBlt(m_hDC, m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), m_hMemDC, m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), SRCCOPY);
	//BitBlt(m_hDC, m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), m_hMemDC, 0, 0, SRCCOPY);
	BitBlt(m_hDC, m_rcClient.left, m_rcClient.top, m_rcClient.Width(), m_rcClient.Height(), m_hMemDC, 0, 0, SRCCOPY);
	//BitBlt(m_hDC, 0, 0, m_rcClient.Width(), m_rcClient.Height(), m_hMemDC, 0, 0, SRCCOPY);

	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_bBmp);

	SelectObject(m_hMemDC, m_hOldFont);

	DeleteDC(m_hMemDC);

	ReleaseDC(m_hWnd, m_hDC);

	//EndPaint(m_hWnd, &ps);

	return hr;
}

void GDIRenderEngine::DrawText(LPRECT rect, LPCTSTR text, CColorUtil& clr, int nHorizontal, int nVertical)
{
	//HBRUSH hBrush = CreateSolidBrush(RGB(clr.r, clr.g, clr.b));
	
	//HBRUSH hOldObj = (HBRUSH) ::SelectObject(m_hMemDC, hBrush);
	
	::SetTextColor(m_hMemDC, RGB(clr.r, clr.g, clr.b));
	//::SetBkColor();
	
	//::TextOut(m_hMemDC, 0, 0, text, _tcslen(text));
	::DrawText(m_hMemDC, text, -1, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	//::DrawTextEx();
	//::TabbedTextOut();

	

	//::SelectObject(m_hMemDC, hOldObj);
	//DeleteObject(hBrush);
	//::InvalidateRect();
}

void GDIRenderEngine::DrawLine(float x1, float y1, float x2, float y2, CColorUtil& clr)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(clr.r, clr.g, clr.b));
	HPEN hPenOld = (HPEN)::SelectObject(m_hMemDC, hPen);

	CPoint pt;
	::MoveToEx(m_hMemDC, x1, y1, &pt);
	::LineTo(m_hMemDC, x2, y2);

	::SelectObject(m_hMemDC, hPenOld);
	::DeleteObject(hPen);
}

void GDIRenderEngine::OnResize(UINT width, UINT height)
{
}

void GDIRenderEngine::DrawRectangle(LPRECT rect, CColorUtil& clr)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(clr.r, clr.g, clr.b));
	HPEN hPenOld = (HPEN)::SelectObject(m_hMemDC, hPen);

	HBRUSH hBrush = ::CreateSolidBrush(RGB(clr.r, clr.g, clr.b));
	HBRUSH hBrushOld = (HBRUSH)::SelectObject(m_hMemDC, hBrush);

	
	//::Rectangle(m_hMemDC, rect->left, rect->top, rect->right, rect->bottom);
	::FrameRect(m_hMemDC, rect, hBrush);

	::SelectObject(m_hMemDC, hPenOld);
	::DeleteObject(hPen);

	::SelectObject(m_hMemDC, hBrushOld);
	::DeleteObject(hBrush);
}

CSize GDIRenderEngine::GetStringSize(CString FontName, int FontSize, CString str)
{
	//CPaintDC dc(this);
	//::GetTextExtentExPoint(
	//CDC hDC = ::GetDC(NULL);
	//if (!pDC)
	//	return CSize(0,0);

	//CFont font;
	//font.CreatePointFont(FontSize, FontName);
	//dc.SelectObject(font);

	//int nDPIx = GetDeviceCaps(m_hMemDC, LOGPIXELSX);
	//int nDPIy = GetDeviceCaps(m_hMemDC, LOGPIXELSY);

	CSize size;
	
	GetTextExtentExPoint(m_hMemDC, str, str.GetLength(), 0, NULL, NULL, &size);

	//int textWidth = .cx * 72 / nDPIx;
	//int textHeight = pDC->GetTextExtent(str).cy * 72 / nDPIy;

	
	//size.SetSize(textWidth, textHeight);
	return size;
}

void GDIRenderEngine::FillRectangle(LPRECT rect, CColorUtil& clr)
{

	HBRUSH hBrush = ::CreateSolidBrush(RGB(clr.r, clr.g, clr.b));
	//HBRUSH hBrushOld = (HBRUSH)::SelectObject(m_hMemDC, hBrush);

	
	::FillRect(m_hMemDC, rect, hBrush);
	


//	::SelectObject(m_hMemDC, hBrushOld);
//	::DeleteObject(hBrush);
}
void GDIRenderEngine::InvalidRect(LPRECT rect)
{
	::InvalidateRect(m_hWnd, rect, TRUE);
}
