#include "stdafx.h"
#include "DataManager.h"
#include "FileLog.h"
#include "NetworkManager.h"
#include "BaseControlWnd.h"


 //CDataManager* CDataManager::m_instance = new CDataManager;

CDataManager::CDataManager(void)
{
}


CDataManager::~CDataManager(void)
{
}

void CDataManager::Init()
{
	m_tpRequest = NULL;
	m_tpRequest = new worker< job_queue<tcp_message*> >(m_qRequest, boost::bind(&CDataManager::RequestDispatchThread, this, _1), 1);

	m_tpResponse = NULL;
	m_tpResponse = new worker< job_queue<tcp_message*> >(m_qResponse, boost::bind(&CDataManager::ResponseDispatchThread, this, _1), 1);
}

void CDataManager::Start()
{
	StartRequestDispatchThreadPool();

	StartResponseDispatchThreadPool();
}

void CDataManager::Stop()
{
	StopRequestDispatchThreadPool();

	StopResponseDispatchThreadPool();
}

void CDataManager::UnInit()
{
	if (m_tpRequest != NULL)
	{
		delete m_tpRequest;
	}

	if (m_tpResponse != NULL)
	{
		delete m_tpResponse;
	}

	sNetworkManager::instance().Close();
}

void CDataManager::PushRequestToQueue(tcp_message* request)
{
	m_qRequest.push(request);
}

void CDataManager::StartRequestDispatchThreadPool()
{
	m_tpRequest->start();
}

void CDataManager::StopRequestDispatchThreadPool()
{
	m_tpRequest->stop();
}

bool CDataManager::RequestDispatchThread(tcp_message* request)
{
	sNetworkManager::instance().SendRequest(request);

	return true;
}



void CDataManager::PushResponseToQueue(tcp_message* response)
{
	m_qResponse.push(response);
}
	
void CDataManager::StartResponseDispatchThreadPool()
{
	m_tpResponse->start();
}

void CDataManager::StopResponseDispatchThreadPool()
{
	m_tpResponse->stop();
}

bool CDataManager::ResponseDispatchThread(tcp_message* response)
{
	int size = response->msg_header.size();
	gFileLog::instance().Log("返回包头大小" + boost::lexical_cast<std::string>(size));

	size = response->msg_body.size();
	gFileLog::instance().Log("返回包内容大小" + boost::lexical_cast<std::string>(size));

	gFileLog::instance().Log("错误码" + boost::lexical_cast<std::string>(response->header.errcode()));

	// 分发到视图层
	response->GetView()->RecvResponseCallback(response);

	return true;
}

