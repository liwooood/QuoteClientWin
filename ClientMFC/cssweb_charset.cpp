#include "stdafx.h"

#include "cssweb_charset.h"

std::string Utf8ToGBK(const std::string& strUtf8)
{
		std::string ret_data = "";
		int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, NULL,0);  
		unsigned short * wszGBK = new unsigned short[len+1];   
		memset(wszGBK, 0, len * 2 + 2);  
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8.c_str(), -1, (LPWSTR)wszGBK, len);
		len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
		char *szGBK=new char[len + 1];  
		memset(szGBK, 0, len + 1);  
		WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL,NULL);

		ret_data = std::string(szGBK);
		delete [] szGBK;
		delete [] wszGBK;

		return ret_data;
}
