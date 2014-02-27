#include "stdafx.h"

#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include "ConnectSync.h"
#include "FileLog.h"
#include "ConfigManager.h"
#include "DataManager.h"


CConnectSync::CConnectSync(void)
	:socket(ios)
{
	m_bConnected = false;

	std::vector<std::string> kv;
	boost::split(kv, sConfigManager::instance().m_sQuoteServer, boost::is_any_of(":"));

	m_sIP = kv[0];

	m_nPort = boost::lexical_cast<int>(kv[1]);
}


CConnectSync::~CConnectSync(void)
{
}

bool CConnectSync::Connect()
{
	try
	{
		boost::system::error_code ec;

		boost::asio::ip::tcp::resolver resolver(ios);

		boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), m_sIP, boost::lexical_cast<std::string>(m_nPort));

		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query, ec);

		boost::asio::connect(socket, iterator, ec);
	
		//socket->non_blocking(false);

		if (ec)
		{
			gFileLog::instance().Log("连接行情服务器失败");
			gFileLog::instance().Log(ec.message());
			m_bConnected = false;
			return false;
		}

		gFileLog::instance().Log("连接行情服务器成功!");
		m_bConnected = true;
		return true;
	}
	catch(std::exception& e)
	{
		gFileLog::instance().Log(e.what());
		m_bConnected = false;
		return false;
	}
	
}

bool CConnectSync::IsConnected()
{
	return m_bConnected;
}

bool CConnectSync::Send(tcp_request_ptr request)
{
	if (!m_bConnected)
		return false;

	boost::system::error_code ec;
	size_t bytes = 0;



	try
	{
		//boost::asio::write()
		bytes = socket.write_some( boost::asio::buffer(request->msg_header, request->msg_header.size()), ec);
		if (ec)
		{
			gFileLog::instance().Log("write package header:" + ec.message());
			return false;
		}

		bytes = socket.write_some( boost::asio::buffer(request->msg_body, request->msg_body.size()), ec);
		if (ec)
		{
			gFileLog::instance().Log("write package body:" + ec.message());
			return false;
		}

		tcp_message * response = new tcp_message();
		response->SetView(request->GetView());

		bytes = socket.read_some( boost::asio::buffer(response->msg_header, response->msg_header.size()), ec );
		if (ec)
		{
			gFileLog::instance().Log("read package header:" + ec.message());
			return false;
		}

		if (!response->decode_header())
			return false;

		gFileLog::instance().Log("返回包内容大小1：" + boost::lexical_cast<std::string>(response->header.bodysize()));

		int total_bytes = response->header.bodysize();
		int recv_bytes = 0;
		int read_bytes = total_bytes - recv_bytes;
		
		while (bytes = socket.read_some( boost::asio::buffer(&(response->msg_body.front()) + recv_bytes, read_bytes), ec ))
		{
			if (ec)
			{
				gFileLog::instance().Log("read package body:" + ec.message());
				return false;
			}

			recv_bytes += bytes;
			gFileLog::instance().Log("累计接收：" + boost::lexical_cast<std::string>(recv_bytes));
			
			if (recv_bytes >= total_bytes)
				break;

			read_bytes = total_bytes - recv_bytes;
		}

		gFileLog::instance().Log("全部接收：" + boost::lexical_cast<std::string>(recv_bytes));

		if (response->header.zip())
		{
			
			// 解压缩 
			std::vector<char> decompressed;
			
			
			
			boost::iostreams::filtering_streambuf<boost::iostreams::output> decompress_out;
			decompress_out.push(boost::iostreams::zlib_decompressor());
			decompress_out.push(boost::iostreams::back_inserter(decompressed));
			
			

			boost::iostreams::copy(boost::make_iterator_range(response->msg_body), decompress_out);
			
			
			
			
			int bodysize = decompressed.size();
			response->set_body_size(bodysize);
			memcpy(&(response->msg_body.front()), &decompressed[0], decompressed.size());
		}

		// 存入返回队列
		sDataManager::instance().PushResponseToQueue(response);
	
		return true;
	}
	catch(std::exception& e)
	{
		
		//gFileLog::instance().Log("network unknown exception:" + e.what());
		gFileLog::instance().Log( e.what());
		return false;
	}

	return true;
}

void CConnectSync::Close()
{
	boost::system::error_code ec;

	socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	
	if (ec)
	{
		gFileLog::instance().Log(ec.message());
		gFileLog::instance().Log("断开行情服务器连接失败!");
	}

	socket.close(ec);
	gFileLog::instance().Log("断开行情服务器连接成功!");
}

bool CConnectSync::ReConnect()
{
	Close();

	return Connect();
}

std::string CConnectSync::decompress(std::string data)
{
    std::stringstream compressed;
    compressed << data;

	
    
    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::zlib_decompressor());
    out.push(compressed);

	std::stringstream decompressed;
    boost::iostreams::copy(out, decompressed);

    return decompressed.str();
}