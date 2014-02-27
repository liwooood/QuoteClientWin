#include "stdafx.h"
#include "ConnectAsync.h"
#include "FileLog.h"
#include "DataManager.h"


CConnectAsync::CConnectAsync(void)
	:work(ios)
	,socket(ios)
{
	StartDispatchThread();
}


CConnectAsync::~CConnectAsync(void)
{
	//ios.stop();
}

void CConnectAsync::StartDispatchThread()
{
	pThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &ios));
	
}

void CConnectAsync::Connect()
{
	std::string ip = "221.226.125.220";
	int port = 8084;

	//std::string ip = "210.13.95.57";
	//int port = 9001;
	

	boost::asio::ip::tcp::resolver resolver(ios);

	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), ip, boost::lexical_cast<std::string>(port));

	boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

	boost::asio::async_connect(
			socket, 
			iterator, 
			boost::bind(&CConnectAsync::ConnectHandler, this, boost::asio::placeholders::error)
		);
}

void CConnectAsync::ConnectHandler(const boost::system::error_code& error)
{
	if (error)
	{
		gFileLog::instance().Log("连接失败");
		return;
	}

	gFileLog::instance().Log("连接成功");
}

void CConnectAsync::Close()
{
	boost::system::error_code ignored_ec;
	socket.close(ignored_ec);
}

void CConnectAsync::Send(tcp_request_ptr request)
{
//	if (request != NULL && request->msg_header.size() == tcp_message::header_length)
//	{
//		return;
//	}

      boost::asio::async_write(
		  socket,
		  boost::asio::buffer(request->msg_header, tcp_message::header_length),
          boost::bind(&CConnectAsync::WriteHeaderHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, request)
		 );    
}

void CConnectAsync::WriteHeaderHandler(const boost::system::error_code& error, size_t bytes_transferred, tcp_request_ptr request)
{
	if (error)
	{
		gFileLog::instance().Log("写请求头出错");
		return;
	}

		boost::asio::async_write(
			socket,
			boost::asio::buffer(request->msg_body, request->msg_body.size()),
			bind(&CConnectAsync::WriteBodyHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, request)
		);
}

void CConnectAsync::WriteBodyHandler(const boost::system::error_code& error, size_t bytes_transferred, tcp_request_ptr request)
{
	// 删除tcp_request_ptr，防止内存泄漏
	// delete request

	if (error)
	{
		gFileLog::instance().Log("写请求内容出错");
		return;
	}

	tcp_response_ptr response = new tcp_response();

		boost::asio::async_read(
			socket,
			boost::asio::buffer(response->msg_header, tcp_message::header_length),
			bind(&CConnectAsync::ReadHeaderHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, response)
		);
}

void CConnectAsync::ReadHeaderHandler(const boost::system::error_code& error, size_t bytes_transferred, tcp_response_ptr response)
{
	if (error)
	{
		gFileLog::instance().Log("读返回包头出错");
		return;
	}

	if (!response->decode_header())
	{
		gFileLog::instance().Log("解析返回头出错");
		return;
	}

		boost::asio::async_read(
			socket, 
			boost::asio::buffer(response->msg_body, response->header.bodysize()),
			bind(&CConnectAsync::ReadBodyHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, response)
		);

}

void CConnectAsync::ReadBodyHandler(const boost::system::error_code& error, size_t bytes_transferred, tcp_response_ptr response)
{
	if (error)
	{
		gFileLog::instance().Log("读返回包内容出错");
		return;
	}

	// 存入返回队列
	int size = response->msg_header.size();
	gFileLog::instance().Log("返回包头大小" + boost::lexical_cast<std::string>(size));

	size = response->msg_body.size();
	gFileLog::instance().Log("返回包内容大小" + boost::lexical_cast<std::string>(size));

	sDataManager::instance().PushResponseToQueue(response);
}
