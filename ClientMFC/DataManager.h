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
	// ��ʼ�������캯������
	void Init();

	void Start();
	void Stop();

	// ������������
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

// ��������
public:
	void PushRequestToQueue(tcp_message* request);
	void StartRequestDispatchThreadPool();
	void StopRequestDispatchThreadPool();
	bool RequestDispatchThread(tcp_message* request);

//	void DeleteFinanceCode(std::string sFinanceCode);
private:
	job_queue<tcp_message*> m_qRequest;
	worker< job_queue<tcp_message*> > * m_tpRequest;
// ��������

// ����㷵������
public:
	void PushResponseToQueue(tcp_message* response);
	void StartResponseDispatchThreadPool();
	void StopResponseDispatchThreadPool();
	bool ResponseDispatchThread(tcp_message* response);
private:
	// �������㷵�ص�����
	job_queue<tcp_message*> m_qResponse;
	worker< job_queue<tcp_message*> > * m_tpResponse;
// ����㷵������

};


typedef safe_singleton<CDataManager> sDataManager;
#endif
