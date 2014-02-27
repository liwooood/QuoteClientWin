#include "stdafx.h"
#include "ColorUtil.h"


CColorUtil::CColorUtil(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}


CColorUtil::~CColorUtil(void)
{
}

D2D_COLOR_F CColorUtil::ToD2D()
{
	D2D_COLOR_F clr;
	clr.a = a;
	clr.r = r;
	clr.g = g;
	clr.b = b;

	return clr;
}