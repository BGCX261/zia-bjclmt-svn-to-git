#include "Buffer.h"
#include <iostream>

Buffer::Buffer()
{
	//std::cout << "new empty buffer" << std::endl;
}

Buffer::~Buffer(void)
{
	std::cout << "**************** DELETING BUFFERRRR *****************" << std::endl;
	this->_buf.clear();
}


char* Buffer::getRawData()
{
	if (this->_buf.size() == 0)
		return(0);
	char * tmp = new char[this->_buf.size()];
	memcpy(tmp, this->_buf.c_str(), this->_buf.size());
	return(tmp);
}

std::size_t Buffer::getSize() const
{
	return(this->_buf.size());
}

void Buffer::setSize(std::size_t size)
{
	this->_buf.resize(size);
}

void Buffer::setSize(std::size_t start, std::size_t size)
{
	this->_buf = this->_buf.substr(start, size);
}

void Buffer::setContent(std::string string, std::size_t size)
{
	//std::cout << "Setting buffer content : size = " << size << " content = " << string << std::endl; 
	this->_buf = string;
	if (size != -1)
		this->_buf.resize(size);
}