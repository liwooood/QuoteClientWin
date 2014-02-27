#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <map>
#include <string>
#include <vector>


//#include <boost/thread/detail/singleton.hpp>
#include "safe_singleton.h"



class ConfigManager
{
public:
	ConfigManager(void);
	~ConfigManager(void);
	
	bool LoadConfig();
	std::string& GetWorkerPath()
	{
		return m_sPath;
	}
public:
	int m_nRunMode;


	std::string m_sPath;
	std::string m_sConfigFile;
	std::string m_sQuoteServer;



};
typedef safe_singleton<ConfigManager> sConfigManager;

#endif
