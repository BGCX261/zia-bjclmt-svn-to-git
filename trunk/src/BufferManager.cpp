#include "BufferManager.h"
#include <iostream>

BufferManager::BufferManager(void)
{
	IBuffer * tmp;
	int j = POOL_BUF_SIZE;
	for (int i = 0; i < j; ++i)
	{
		tmp = dynamic_cast<IBuffer *>(new Buffer());
		_freeBuffer.push_back(tmp);
	}
}

BufferManager::~BufferManager(void)
{
	std::list<IBuffer *>::iterator freeIt = this->_freeBuffer.begin();
	std::list<IBuffer *>::iterator fullIt = this->_usedBuffer.begin();

	std::list<IBuffer *>::iterator tmp;

	while (freeIt != this->_freeBuffer.end())
	{
		tmp = freeIt;
		++freeIt;
		delete *tmp;
	}

	while (fullIt != this->_usedBuffer.end())
	{
		tmp = fullIt;
		++fullIt;
		delete *tmp;
	}
}

IBuffer* BufferManager::allocate(std::string const& string)
{
	if (this->_freeBuffer.size() != 0)
	{
		this->_usedBuffer.push_back(*this->_freeBuffer.begin());
		this->_freeBuffer.erase(this->_freeBuffer.begin());
	}
	else
	{
		this->_usedBuffer.push_back(dynamic_cast<IBuffer *>(new Buffer()));
	}
	dynamic_cast<Buffer *>(this->_usedBuffer.back())->setContent(string);
	return (this->_usedBuffer.back());
}	

IBuffer* BufferManager::allocate(char const* data, std::size_t size)
{
	if (this->_freeBuffer.size() != 0)
	{
		this->_usedBuffer.push_back(*this->_freeBuffer.begin());
		this->_freeBuffer.erase(this->_freeBuffer.begin());
	}
	else
	{
		this->_usedBuffer.push_back(dynamic_cast<IBuffer *>(new Buffer()));
	}
	dynamic_cast<Buffer *>(this->_usedBuffer.back())->setContent(data, size);
	return (this->_usedBuffer.back());
}

IBuffer* BufferManager::allocate(std::size_t size)
{
	if (this->_freeBuffer.size() != 0)
	{
		this->_usedBuffer.push_back(*this->_freeBuffer.begin());
		this->_freeBuffer.erase(this->_freeBuffer.begin());
	}
	else
	{
		this->_usedBuffer.push_back(dynamic_cast<IBuffer *>(new Buffer()));
	}
	dynamic_cast<Buffer *>(this->_usedBuffer.back())->setContent("", size);
	return (this->_usedBuffer.back());
}

void BufferManager::release(IBuffer* buffer)
{
	unsigned int j = POOL_BUF_SIZE;
	std::list<IBuffer *>::iterator it = this->_usedBuffer.begin();
	while (it != this->_usedBuffer.end() && *it != buffer)
		++it;
	if (it != this->_usedBuffer.end())
	{
		if (this->_freeBuffer.size() > j)
		{
			delete *it;
			this->_usedBuffer.erase(it);
		}
		else
		{
			this->_freeBuffer.push_back(*it);
			this->_usedBuffer.erase(it);
		}
		dynamic_cast<Buffer *>(this->_freeBuffer.back())->setContent("", 0);
		std::cout << "Buffer released" << std::endl;
	}
}
