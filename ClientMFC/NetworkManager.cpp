#include "stdafx.h"

#include <thread>

#include "NetworkManager.h"

 //CNetworkManager* CNetworkManager::m_instance = new CNetworkManager;

CNetworkManager::CNetworkManager(void)
{
	// 0Õ¨≤Ω£¨1“Ï≤Ω
	int m_nMode = 0;
}




bool CNetworkManager::SendRequest(tcp_message * request)
{
	if (!sync.IsConnected())
	{
		if (!sync.Connect())
			return false;
	}
	
	return sync.Send(request);
}

void CNetworkManager::Close()
{
	sync.Close();
}

bool CNetworkManager::Connect()
{
	if (sync.IsConnected())
	{
		return true;

	}

	return sync.Connect();
}
