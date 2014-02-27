#pragma once
#include <string>
#include <list>

class CFinanceProduct
{
public:
	CFinanceProduct(std::string sCode, std::string sMarket);
	CFinanceProduct(std::string sCode, int nMarket);
	~CFinanceProduct(void);

private:
	std::string m_sCode;
	std::string m_sMarket;
	int m_nType; // Àà±ð
	//std::string 

public:
	std::string GetFinanceProduct();
};

