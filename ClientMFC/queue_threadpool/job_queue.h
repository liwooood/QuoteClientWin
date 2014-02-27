#ifndef _JOB_QUEUE_
#define _JOB_QUEUE_

#include <deque>

#include <boost/noncopyable.hpp>
#include <boost/utility/value_init.hpp>
#include <boost/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>


template<typename Job>
class job_queue : boost::noncopyable
{
public:
	typedef Job job_type;
	typedef std::deque<job_type> queue_type;

	typedef boost::mutex mutex_type;
	typedef typename mutex_type::scoped_lock lock_type;
	typedef boost::condition_variable_any condition_type;

	// 要求有拷贝构造函数和重载operator=
	BOOST_CONCEPT_ASSERT((boost::SGIAssignable<job_type>));
	// 要求有缺省构造函数
	BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<job_type>));

public:
	queue_type queue_;

private:
	mutex_type mutex_;
	condition_type condition_;
	bool stop_flag_;

public:
	job_queue(void):
	  stop_flag_(false)
	{
	}

	~job_queue(void)
	{
	}

	void push(const job_type& x)
	{
		lock_type lock(mutex_);
		queue_.push_back(x);
		condition_.notify_one();
	}

	// 应该采用void pop(job_type& job)更合理
	job_type pop()
	{
		lock_type lock(mutex_);
		while(queue_.empty() && !stop_flag_)
		{
			condition_.wait(mutex_);
		}

		if (stop_flag_)
		{
			//相当于return job_type();
			return boost::initialized_value;
		}

		BOOST_ASSERT(!queue_.empty());

		job_type tmp = queue_.front();
		queue_.pop_front();
		return tmp;
	}

	void stop()
	{
		stop_flag_ = true;
		condition_.notify_all();
	}

};
#endif
