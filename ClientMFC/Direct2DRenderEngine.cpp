#include "StdAfx.h"
#include "Direct2DRenderEngine.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


template<typename Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

Direct2DRenderEngine::Direct2DRenderEngine( HWND hWnd )
{
	TRACE("Direct2DRenderEngine\n");

	CoInitialize( NULL );

	this->m_hWnd = hWnd;
	
	m_pD2DFactory = NULL;
	m_pRenderTarget = NULL;

	m_pDWriteFactory = NULL;
	m_pTextFormat = NULL;
	
	m_pBrushRed = NULL;
}



Direct2DRenderEngine::~Direct2DRenderEngine(void)
{
	TRACE("Direct2DRenderEngine\n");

	SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pRenderTarget);

	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pTextFormat);

	SafeRelease(&m_pBrushRed);

	CoUninitialize();

	/*
	for( std::vector<ID2D1Geometry*>::iterator it = m_Geometries.begin();
			it != m_Geometries.end(); ++it )
	{
			(*it)->Release();
			
	}

	m_Geometries.clear();
*/
}


HRESULT Direct2DRenderEngine::Initialize()
{
	return CreateDeviceIndependentResources();

}

// 创建设备无关资源
HRESULT Direct2DRenderEngine::CreateDeviceIndependentResources()
{
	TRACE("CreateDeviceIndependentResources\n");

	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

    // Create a shared DirectWrite factory.
    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
            );
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pDWriteFactory->CreateTextFormat(
            L"宋体",                // Font family name.
            NULL,                       // Font collection (NULL sets it to use the system font collection).
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            12.0f,
            L"en-us",
            &m_pTextFormat
            );
    }

	if (SUCCEEDED(hr))
    {
		//hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		
		hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
        //hr = m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    }

    if (SUCCEEDED(hr))
    {
		//hr = m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
        hr = m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

	return hr;
}


HRESULT Direct2DRenderEngine::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::Black, 1.0f) );

		CRect rcClient;
		GetClientRect(m_hWnd, rcClient);
		D2D1_RECT_F rectangle = D2D1::Rect( rcClient.left+10, rcClient.top+10, rcClient.right-10, rcClient.bottom-10 );	
		m_pRenderTarget->DrawRectangle(&rectangle, m_pBrushRed);



		hr = m_pRenderTarget->EndDraw();

	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

void Direct2DRenderEngine::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}
/*
void Direct2DRenderEngine::CreateRectangle( LPCRECT pRect, bool bFill )
{
	if (!m_pRenderTarget)
	{
		return;
	}

	D2D1_RECT_F rectangle = D2D1::Rect( pRect->left, pRect->top, pRect->right, pRect->bottom );

	//ID2D1PathGeometry * pPath;
	//pPath->
	//ID2D1RectangleGeometry* pRectangle;
	//m_pDirect2dFactory->CreateRectangleGeometry( rectangle, &pRectangle );
	//m_Geometries.push_back( pRectangle );
	//m_pDirect2dFactory->CreatePathGeometry(&pPath);
	m_pRenderTarget->DrawRectangle(&rectangle, m_pLightSlateGrayBrush);
}
*/
/*
void Direct2DRenderEngine::CreateRoundedRectangle(LPCRECT pRect, int radiusx, int radiusY, bool bFill )
{
	D2D1_ROUNDED_RECT rectangle = D2D1::RoundedRect( D2D1::Rect(pRect->left, pRect->top, pRect->right, pRect->bottom), radiusx, radiusY);
	ID2D1RoundedRectangleGeometry* pRRectangle;
	m_pDirect2dFactory->CreateRoundedRectangleGeometry( rectangle, &pRRectangle );
	m_Geometries.push_back( pRRectangle );
}

void Direct2DRenderEngine::CreateEllipse( LPCRECT pRectBoundingBox )
{
	int halfX = ( pRectBoundingBox->right - pRectBoundingBox->left ) /2;
	int halfY = ( pRectBoundingBox->bottom - pRectBoundingBox->top ) /2;
	D2D1_ELLIPSE ellipse = D2D1::Ellipse( D2D1::Point2( pRectBoundingBox->left + halfX, pRectBoundingBox->top + halfY), halfX, halfY );
	
	ID2D1EllipseGeometry* pEllipse;
	m_pDirect2dFactory->CreateEllipseGeometry( ellipse, &pEllipse );
	
	m_Geometries.push_back( pEllipse );
}
*/



HRESULT Direct2DRenderEngine::BeginRender(LPRECT rcClient)
{
	//TRACE("BeginRender\n");

	HRESULT hr = S_OK;

	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::Black, 1.0f) );

		//CRect rcClient;
		//GetClientRect(m_hWnd, rcClient);
		//D2D1_RECT_F rectangle = D2D1::Rect( rcClient.left+10, rcClient.top+10, rcClient.right-10, rcClient.bottom-10 );	
		//m_pRenderTarget->DrawRectangle(&rectangle, m_pBrushRed);
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}
//创建设备相关资源
HRESULT Direct2DRenderEngine::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (m_hWnd == NULL)
		return -1;

	if (m_pRenderTarget == NULL)
	{
		TRACE("CreateDeviceResources\n");

		RECT rc;
		GetClientRect(m_hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
			);

		// Create a Direct2D render target.
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size),
			&m_pRenderTarget
			);


		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Red),
				&m_pBrushRed
				);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&m_pBrushWhite
				);
		}
	}

	return hr;
}

// 释放设备相关资源
void Direct2DRenderEngine::DiscardDeviceResources()
{
	TRACE("DiscardDeviceResources\n");

	SafeRelease(&m_pRenderTarget);

	SafeRelease(&m_pBrushRed);

	SafeRelease(&m_pBrushWhite);
}

HRESULT Direct2DRenderEngine::EndRender()
{
	//TRACE("EndRender\n");

	HRESULT hr = S_OK;

	if (m_pRenderTarget)
	{
		hr = m_pRenderTarget->EndDraw();
	}
	

	return hr;
}

void Direct2DRenderEngine::DrawText(LPRECT rc, LPCTSTR text, CColorUtil& clr,int nHorizontal, int nVertical)
{
	int length = _tcslen(text);

	D2D1_RECT_F layoutRect = D2D1::RectF(
		static_cast<FLOAT>(rc->left),
		static_cast<FLOAT>(rc->top),
		static_cast<FLOAT>(rc->right),
		static_cast<FLOAT>(rc->bottom)
    );

	


	if (!m_pRenderTarget)
		return;

	IDWriteTextFormat* pTextFormat = NULL;
    m_pDWriteFactory->CreateTextFormat(
            L"宋体",                // Font family name.
            NULL,                       // Font collection (NULL sets it to use the system font collection).
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            12.0f,
            L"en-us",
            &pTextFormat
            );
	
	switch (nHorizontal)
	{
	case TEXT_ALIGN_HORZ_CENTER:
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case TEXT_ALIGN_HORZ_RIGHT:
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	case TEXT_ALIGN_HORZ_LEFT:
	default:
		pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	}

	ID2D1SolidColorBrush * pBrush = NULL;
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r, clr.g, clr.b), &pBrush);

	m_pRenderTarget->DrawText(text, length, pTextFormat, layoutRect, pBrush);

	SafeRelease(&pBrush);
	SafeRelease(&pTextFormat);

	//D2D1_RECT_F rectangle = D2D1::Rect(layoutRect.left, layoutRect.top, layoutRect.right, layoutRect.bottom);	
	//m_pRenderTarget->DrawRectangle(&rectangle, m_pBrushWhite);
}

void Direct2DRenderEngine::DrawLine(float x1, float y1, float x2, float y2, CColorUtil& clr)
{
	ID2D1SolidColorBrush *brush = NULL;
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(clr.r, clr.g, clr.b), &brush);

	m_pRenderTarget->DrawLine(
                D2D1::Point2F(x1, y1),
                D2D1::Point2F(x2, y2),
                brush,
				1.0f,
				NULL
                );

	SafeRelease(&brush);
}

void Direct2DRenderEngine::DrawRectangle(LPRECT rect, CColorUtil& clr)
{
}

CSize Direct2DRenderEngine::GetStringSize(CString FontName, int FontSize, CString str)
{
	return CSize(0,0);
}

void Direct2DRenderEngine::FillRectangle(LPRECT rect, CColorUtil& clr)
{
}
void Direct2DRenderEngine::InvalidRect(LPRECT rect)
{
}
