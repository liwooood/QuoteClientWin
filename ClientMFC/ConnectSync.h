#pragma once

#include "importheader.h"
#include "tcp_message.h"

class CConnectSync
{
public:
	CConnectSync(void);
	~CConnectSync(void);

	bool Connect();
	bool Send(tcp_request_ptr msg);
	void Close();
	bool ReConnect();
	bool IsConnected();

private:
	bool m_bConnected;
	boost::asio::io_service ios;
	boost::asio::ip::tcp::socket socket;
	std::string m_sIP;
	int m_nPort;
	std::string decompress(std::string data);

};

