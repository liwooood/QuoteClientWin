

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

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

#include "ZlibUtil.h"




ZlibUtil::ZlibUtil(void)
{
}


ZlibUtil::~ZlibUtil(void)
{
}

bool ZlibUtil::compress()
{
	std::vector<char> content;

	std::ifstream file;
	file.open("d:\\zeroMQ消息模式分析.pptx", std::ios::binary);
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0);

	content.resize(size);
	file.read(&(content.front()), size);
	file.close(); 

		std::vector<char> compressed;
		boost::iostreams::filtering_streambuf<boost::iostreams::output> compress_out;
		compress_out.push(boost::iostreams::zlib_compressor());
		compress_out.push(boost::iostreams::back_inserter(compressed));

		//std::string temp(content.begin(), content.end());
		boost::iostreams::copy(boost::make_iterator_range(content), compress_out);

	std::ofstream save;
	save.open("d:\\zeroMQ消息模式分析.zip", std::ios::binary);
	save.write(&(compressed.front()), compressed.size());
	save.close(); 

	return true;
}

bool ZlibUtil::uncompress()
{
	std::vector<char> content;

	std::ifstream file;
	file.open("d:\\test.zip", std::ios::binary);
	file.seekg(0, std::ios::end);
	size_t size = file.tellg();
	file.seekg(0);

	content.resize(size);
	file.read(&(content.front()), size);
	file.close(); 

			std::vector<char> decompressed;

			

			boost::iostreams::filtering_streambuf<boost::iostreams::output> decompress_out;
			decompress_out.push(boost::iostreams::zlib_decompressor());
			decompress_out.push(boost::iostreams::back_inserter(decompressed));

			boost::iostreams::copy(boost::make_iterator_range(content), decompress_out);
			
	std::ofstream save;
	save.open("d:\\test_new.xls", std::ios::binary);
	save.write(&(decompressed.front()), decompressed.size());
	save.close(); 

	return true;
}

