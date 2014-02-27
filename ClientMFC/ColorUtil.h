#pragma once
#include <d2d1.h>
#include <d2d1helper.h>

class CColorUtil
{
public:
	CColorUtil(float r, float g, float b, float a=1.0f);
	~CColorUtil(void);
	
public:
	D2D_COLOR_F ToD2D();
	COLORREF ToGDI();


	float r;
	float g;
	float b;
	float a;

};

