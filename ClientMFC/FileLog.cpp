#include "stdafx.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem.hpp>

#include "FileLog.h"

#include "configmanager.h"


FileLog::FileLog(void)
{
}


FileLog::~FileLog(void)
{
}

void FileLog::Log(std::string log, std::string file)
{
	// ���������ģʽ��ֱ�ӷ��أ�����¼��־�ļ�
	if (sConfigManager::instance().m_nRunMode)
		return;

	TRACE("���������Ϣ��%s\n", log.c_str());


	// ����Ŀ¼
	std::string sLogDir = sConfigManager::instance().GetWorkerPath();

	boost::filesystem::path p(sLogDir);
	if (!boost::filesystem::exists(p))
	{
		boost::filesystem::create_directories(p);
	}

	std::string sLogFileName = sLogDir + "\\debug.log";
	

	std::ofstream outfile(sLogFileName.c_str(), std::ios_base::app);
	if (outfile.is_open())
	{
		boost::posix_time::ptime beginTime =  boost::posix_time::microsec_clock::local_time();
		std::string sBeginTime = boost::gregorian::to_iso_extended_string(beginTime.date()) + " " + boost::posix_time::to_simple_string(beginTime.time_of_day());
		outfile << "ʱ�䣺" << sBeginTime << "\n";

		outfile << "���ݣ�" << log << "\n";
			
		// ��һ��
		outfile << "\n";

		outfile.close();
	}

}
