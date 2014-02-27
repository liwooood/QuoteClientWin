#ifndef NETWORK_MANAGER
#define NETWORK_MANAGER

#include "importheader.h"
#include <boost/thread/mutex.hpp>
//#include <boost/serialization/singleton.hpp>


#include "tcp_message.h"
#include "ConnectAsync.h"
#include "ConnectSync.h"
#include "safe_singleton.h"

#define NETWORK_SYNC 0
#define NETWORK_ASYNC 1

class CNetworkManager
{
public:
	CNetworkManager(void);
	//static CNetworkManager* m_instance;
	//static boost::mutex m_mutex;

private:


	CConnectAsync async;
	CConnectSync sync;

	// 同步或异步
	int m_nMode; 

public:
	/*
	static  CNetworkManager* getInstance()
	{
		if (m_instance == NULL)
		{
			boost::mutex::scoped_lock lock(m_mutex);
			m_instance = new CNetworkManager;
		}

		return m_instance;
	};
	*/

	bool Connect();
	bool SendRequest(tcp_message * request);
	void Close();
	

};
typedef safe_singleton<CNetworkManager> sNetworkManager;


#endif
