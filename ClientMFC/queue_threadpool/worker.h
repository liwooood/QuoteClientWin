#ifndef _WORKER_
#define _WORKER_

#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

//#define BOOST_ALL_NO_LIB
#include <boost/thread.hpp>

#include "job_queue.h"

template<typename Queue>
class worker
{
public:
	typedef Queue queue_type;
	typedef typename Queue::job_type job_type;
	typedef boost::function<bool(job_type&)> func_type;

private:
	queue_type& queue_;
	func_type func_;

	int thread_num_;
	boost::thread_group threads_;
	std::list<boost::thread*> threads;

public:
	template<typename Func>
	worker(queue_type& q, Func func, int n=1):
	queue_(q), func_(func), thread_num_(n)
	{
		BOOST_ASSERT(n>0);
	}

	worker(queue_type& q, int n=1):
		queue_(q), thread_num_(n)
	{
		BOOST_ASSERT(n>0);
	}

public:
	void start()
	{
		BOOST_ASSERT(func_);

		if (threads_.size() > 0)
			return;

		for (int i=0; i<thread_num_; ++i)
		{
			boost::thread *thread = threads_.create_thread(boost::bind(&worker::do_work, this));
			threads.push_back(thread);
		}

		
	}

	template<typename Func>
	void start(Func func)
	{
		func_ = func;
		start();
	}

	void run()
	{
		start();
		threads_.join_all();
	}

private:
	void do_work()
	{
		for (;;)
		{
			job_type job = queue_.pop();

			if (!func_ || !func_(job))
			{
				break;
			}
		}
	}

public:
	void stop()
	{
		func_ = 0;
		queue_.stop();

		for(std::list<boost::thread*>::iterator it=threads.begin(),end=threads.end();
                it!=end;
                ++it)
		{
			threads_.remove_thread(*it);

			if (*it != NULL)
				delete *it;
		}

	}
};
#endif
