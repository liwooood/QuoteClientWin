#pragma once
class ZlibUtil
{
public:
	ZlibUtil(void);
	~ZlibUtil(void);

	bool compress();
	bool uncompress();
};

