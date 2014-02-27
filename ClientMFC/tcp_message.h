#ifndef _TCP_MESSAGE_
#define _TCP_MESSAGE_

#include <vector>
#include <string>

#include <boost/array.hpp>
#include <boost/scoped_array.hpp> 
#include <boost/noncopyable.hpp>
#include <boost/checked_delete.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "pkgheader.pb.h"
#include "communication.pb.h"

#include "FileLog.h"


class CBaseControlWnd;

class tcp_message : boost::noncopyable
{

public:
	tcp_message()
	{
	}

/*
	// 模板参数是销毁器
	template<typename Func>
	tcp_message(const tcp_session_ptr& s, Func func):
		session_(s), destroy_(func)
	{
	}
*/


	
	enum {header_length = 23};

	// 消息头
	typedef boost::array<char, header_length> msg_header_type;

	msg_header_type msg_header;
	quote::PkgHeader header;



	// 消息体
	std::vector<char> msg_body;
	//boost::shared_ptr<std::vector<char>> body;
	//boost::scoped_array<char> body; // new char[]
	

	void set_body_size(int body_size)
	{
		msg_body.clear();
		msg_body.resize(body_size);
	}


	bool decode_header()
	{
		// 从array<char>转成pb
		if (!header.ParseFromArray(msg_header.data(), msg_header.size()))
		{
			return false;
		}

		// 设置pkgbody的大小
		set_body_size(header.bodysize());

		return true;
	}

	bool encode_header(quote::PkgHeader::MsgType msgType, int body_size, int errcode=0)
	{
		header.set_bodysize(body_size);
		header.set_ver(1);
		header.set_enc(0);
		header.set_zip(0);
		header.set_more(0);
		header.set_msgtype(msgType);
		header.set_errcode(errcode);
		
		if (!header.SerializeToArray(&msg_header, header.ByteSize()))
			return false;

		return true;
	}

	CBaseControlWnd* m_pViewWnd;

	void SetView(CBaseControlWnd* pView)
	{
		m_pViewWnd = pView;
	}

	CBaseControlWnd* GetView() 
	{
		return m_pViewWnd;
	}

	void OutputMsgType()
	{
		switch (header.msgtype())
		{
			case quote::PkgHeader::RES_REALTIME:
			{
				gFileLog::instance().Log("RES_REALTIME\n");
				break;
			}
			case quote::PkgHeader::RES_GRID:
			{
				gFileLog::instance().Log("RES_GRID\n");
				break;
			}
			case quote::PkgHeader::RES_DETAIL:
			{
				gFileLog::instance().Log("RES_DETAIL\n");
				break;
			}
			case quote::PkgHeader::REQ_MIN:
			{
				gFileLog::instance().Log("REQ_MIN");
				break;
			}
		} // end switch		
	}

public:
	typedef boost::function<void(tcp_message*)> destroy_type;
private:
	destroy_type destroy_;

	void destroy()
	{
		try
		{
			if (destroy_)
			{
				

				if (this == NULL)
				{
					std::cout << "tcp_message destroyed by func this is null. \n";
				}
				else
				{
					//std::cout << "tcp_message destroyed by func" << std::endl;
					destroy_(this);
					
				}
			}
			else
			{
				
				if (this == NULL)
				{
					std::cout << "tcp_message destroyed by checked_delete. \n";
				}
				else
				{
					//std::cout << "tcp_message destroyed by checked_delete" << std::endl;
					boost::checked_delete(this);
				}
			}
		}
		catch(std::exception& e)
		{
			std::cout << "tcp_message detroy exception " << e.what() << "\n";
		}
		catch(...)
		{
			std::cout << "tcp_message detroy unknown exception \n";
		}
	}

};

typedef tcp_message tcp_request;
typedef tcp_message tcp_response;
typedef tcp_request* tcp_request_ptr;
typedef tcp_response* tcp_response_ptr;

#endif
