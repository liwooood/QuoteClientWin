#pragma once

#include "IRenderEngine.h"

class GDIRenderEngine: public IRenderEngine
{
public:
	GDIRenderEngine(HWND hWnd);
	~GDIRenderEngine(void);

	

private:
	HWND m_hWnd;

	CRect m_rcClient;
	HDC m_hDC;
	PAINTSTRUCT ps;

	HDC m_hMemDC;
	HBITMAP m_hOldBmp;
	HBITMAP m_bBmp;

	HFONT m_hFont;
	HFONT m_hOldFont;
	CFont m_font;

public:
	virtual void Render();

	virtual HRESULT Initialize();
	virtual HRESULT BeginRender(LPRECT rcClient=NULL);
	virtual HRESULT EndRender();
	virtual void DrawText(LPRECT rect, LPCTSTR text, CColorUtil& clr, int nHorizontal=TEXT_ALIGN_HORZ_LEFT, int nVertical=TEXT_ALIGN_VERT_CENTER);
	virtual void DrawLine(float x1, float y1, float x2, float y2, CColorUtil& clr);
	virtual void OnResize(UINT width, UINT height);
	virtual void DrawRectangle(LPRECT rect, CColorUtil& clr);
	virtual CSize GetStringSize(CString FontName, int FontSize, CString str);
	virtual void FillRectangle(LPRECT rect, CColorUtil& clr);
	virtual void InvalidRect(LPRECT rect);
};

 