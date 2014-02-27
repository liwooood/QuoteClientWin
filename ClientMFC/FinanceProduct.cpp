#include "stdafx.h"
#include "FinanceProduct.h"
#include <boost/lexical_cast.hpp>


CFinanceProduct::CFinanceProduct(std::string sCode, std::string sMarket)
{
	this->m_sCode = sCode;
	this->m_sMarket = sMarket;
}

CFinanceProduct::CFinanceProduct(std::string sCode, int nMarket)
{
	this->m_sCode = sCode;
	this->m_sMarket = boost::lexical_cast<std::string>(nMarket);
}


CFinanceProduct::~CFinanceProduct(void)
{
}

std::string CFinanceProduct::GetFinanceProduct()
{
	return m_sCode + "." + m_sMarket;
}
