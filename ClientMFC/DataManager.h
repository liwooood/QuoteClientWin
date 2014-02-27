#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <boost/thread/mutex.hpp>

#include "tcp_message.h"
#include "./queue_threadpool/job_queue.h"
#include "./queue_threadpool/worker.h"
#include "safe_singleton.h"


class CDataManager
{
public:
	CDataManager(void);
	~CDataManager(void);
	//static CDataManager* m_instance;
	//static boost::mutex m_mutex;

public:
	// 初始化，构造函数调用
	void Init();

	void Start();
	void Stop();

	// 析构函数调用
	void UnInit();
	/*
	static CDataManager* getInstance()
	{
		if (m_instance == NULL)
		{
			boost::mutex::scoped_lock lock(m_mutex);
			m_instance = new CDataManager;
		}

		return m_instance;
	};
	*/

// 发送请求
public:
	void PushRequestToQueue(tcp_message* request);
	void StartRequestDispatchThreadPool();
	void StopRequestDispatchThreadPool();
	bool RequestDispatchThread(tcp_message* request);

//	void DeleteFinanceCode(std::string sFinanceCode);
private:
	job_queue<tcp_message*> m_qRequest;
	worker< job_queue<tcp_message*> > * m_tpRequest;
// 发送请求

// 网络层返回数据
public:
	void PushResponseToQueue(tcp_message* response);
	void StartResponseDispatchThreadPool();
	void StopResponseDispatchThreadPool();
	bool ResponseDispatchThread(tcp_message* response);
private:
	// 存放网络层返回的数据
	job_queue<tcp_message*> m_qResponse;
	worker< job_queue<tcp_message*> > * m_tpResponse;
// 网络层返回数据

};


typedef safe_singleton<CDataManager> sDataManager;
#endif
