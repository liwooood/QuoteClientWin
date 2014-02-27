#ifndef SAFE_SINGLETON_H
#define SAFE_SINGLETON_H

#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>

template <typename T>
class safe_singleton : boost::noncopyable
{
public:
	typedef T real_type;

	static real_type& instance()
	{
		boost::call_once(m_flag, init);
		return init();
	}

private:
	static real_type& init()
	{
		static real_type obj;
		return obj;
	};

private:
	//static boost::thread_detail::mu
	static boost::once_flag m_flag;
	
};

template<typename T>
boost::once_flag safe_singleton<T>::m_flag = BOOST_ONCE_INIT;

#endif