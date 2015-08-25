#pragma once

#include "IBuffer.hpp"
#include <string>

class Buffer : public ZIA::API::IBuffer
{
public:
	Buffer(void);
	~Buffer(void);
	char* getRawData();
	std::size_t getSize() const;
	void setSize(std::size_t size);
	void setSize(std::size_t start, std::size_t size);
	void setContent(std::string, std::size_t size = -1);
private:
	std::string _buf;
};
