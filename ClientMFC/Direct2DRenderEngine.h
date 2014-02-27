#pragma once
#include <vector>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#include "IRenderEngine.h"

class Direct2DRenderEngine: public IRenderEngine
{
public:
	Direct2DRenderEngine( HWND hWnd ); // ctor
	~Direct2DRenderEngine(void); // dtor

	

	HRESULT OnRender(); // Called from OnPaint function

	void Render() {};

	virtual void OnResize(UINT width, UINT height);

	//void CreateRectangle( LPCRECT pRect, bool bFill );
	//void CreateRoundedRectangle(LPCRECT pRect, int radiusx, int radiusY, bool bFill );
	//void CreateEllipse( LPCRECT pRectBoundingBox );

	//void CreateSolidBrush( COLORREF rgbval );
	//void CreateGradientBrush( COLORREF rgbval );

private:
	HRESULT CreateDeviceIndependentResources(); //设备无关

	HRESULT CreateDeviceResources(); // Create resources for drawing
	void DiscardDeviceResources(); // Release resouces for drawing

private:

	HWND m_hWnd;

	ID2D1Factory*			m_pD2DFactory;
	ID2D1HwndRenderTarget*	m_pRenderTarget;

	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	ID2D1SolidColorBrush*	m_pBrushBlack;
	ID2D1SolidColorBrush*	m_pBrushWhite;
	ID2D1SolidColorBrush*	m_pBrushRed;
	ID2D1SolidColorBrush*	m_pBrushGreen;
	ID2D1SolidColorBrush*	m_pBrushYellow;


	//std::vector<ID2D1Geometry*> m_Geometries;
public:
	virtual HRESULT Initialize();
	virtual HRESULT BeginRender(LPRECT rcClient=NULL);
	virtual HRESULT EndRender();

	virtual void DrawText(LPRECT rect, LPCTSTR text, CColorUtil& clr, int nHorizontal=TEXT_ALIGN_HORZ_LEFT, int nVertical=TEXT_ALIGN_VERT_CENTER);
	virtual void DrawLine(float x1, float y1, float x2, float y2, CColorUtil& clr);
	virtual void DrawRectangle(LPRECT rect, CColorUtil& clr);

	virtual CSize GetStringSize(CString FontName, int FontSize, CString str);
	virtual void FillRectangle(LPRECT rect, CColorUtil& clr);
	virtual void InvalidRect(LPRECT rect);
};

