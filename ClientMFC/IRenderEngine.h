#pragma once
#include "ColorUtil.h"
#include "RenderEngineCommon.h"

class IRenderEngine
{
public:
	IRenderEngine(void);
	~IRenderEngine(void);

	virtual void Render()=0;

	virtual HRESULT Initialize() = 0;
	virtual HRESULT BeginRender(LPRECT rcClient=NULL) = 0;
	virtual HRESULT EndRender() = 0;

	virtual void DrawText(LPRECT rect, LPCTSTR text, CColorUtil& clr, int nHorizontal=TEXT_ALIGN_HORZ_LEFT, int nVertical=TEXT_ALIGN_VERT_CENTER) = 0;
	virtual void DrawLine(float x1, float y1, float x2, float y2, CColorUtil& clr) = 0;
	virtual void OnResize(UINT width, UINT height) =0;
	virtual void DrawRectangle(LPRECT rect, CColorUtil& clr) = 0;
	virtual void FillRectangle(LPRECT rect, CColorUtil& clr) = 0;
	virtual CSize GetStringSize(CString FontName, int FontSize, CString str) = 0;
	virtual void InvalidRect(LPRECT rect) = 0;
};

