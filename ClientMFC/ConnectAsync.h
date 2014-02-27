#pragma once

#include "importheader.h"
#include "tcp_message.h"

class CConnectAsync
{
public:
	CConnectAsync(void);
	~CConnectAsync(void);

	
	void Connect();
	void Send(tcp_request_ptr msg);
	void Close();

private:
	boost::asio::io_service ios;
	boost::asio::io_service::work work;
	boost::asio::ip::tcp::socket socket;


	boost::thread * pThread; // 可以改用boost::shared_ptr

	void StartDispatchThread();
	void ConnectHandler(const boost::system::error_code& error);
	void WriteHeaderHandler(const boost::system::error_code& error, size_t bytes_transferred, tcp_request_ptr request);
	void WriteBodyHandler(const boost::system::error_code& error, size_t bytes_transferred, tcp_request_ptr request);
	void ReadHeaderHandler(const boost::system::error_code& error, size_t bytes_transferred, tcp_response_ptr response);
	void ReadBodyHandler(const boost::system::error_code& error, size_t bytes_transferred, tcp_response_ptr response);
};

