#include "stdafx.h"
//#include <windows.h>



#include <list>
#include <iostream>
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>

#include "./xml/pugixml.hpp"
#include "ConfigManager.h"




//using namespace boost::filesystem;




ConfigManager::ConfigManager(void)
{
}

ConfigManager::~ConfigManager(void)
{
}

bool ConfigManager::LoadConfig()
{
	TCHAR szPath[255];
	memset(szPath, 0x00, sizeof(szPath));
	::GetModuleFileName(NULL, szPath, sizeof(szPath));
	

	boost::filesystem::path p(szPath);
	m_sPath = p.parent_path().string();

	m_sConfigFile = m_sPath + "\\config.xml";
	

	pugi::xml_document doc;
	if (!doc.load_file(m_sConfigFile.c_str()))
		return false;

	pugi::xpath_node node;
//	pugi::xpath_node_set nodes;

	node = doc.select_single_node("/config/RunMode");
	m_nRunMode = boost::lexical_cast<int>(node.node().child_value());

	node = doc.select_single_node("/config/server/quote");
	m_sQuoteServer = node.node().child_value();

	

	return true;
}