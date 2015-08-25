#pragma once

#include <list>
#include "IBufferManager.hpp"
#include "IBuffer.hpp"
#include "Buffer.h"

#define POOL_BUF_SIZE 30;

using namespace ZIA::API;

class BufferManager : public IBufferManager
{
public:
	BufferManager(void);
	~BufferManager(void);
    /**
     * Cr�ation d'un nouveau buffer � partir d'une
     * cha�ne de caract�res.
     */
	IBuffer* allocate(std::string const& string);

    /**
     * Cr�ation d'un nouveau buffer � partir d'un
     * tableau de donn�es brutes.
     */
	IBuffer* allocate(char const* data, std::size_t size);

    /**
     * Cr�ation d'un nouveau buffer de la taille sp�cifi�e.
     */
	IBuffer* allocate(std::size_t size);

    /**
     * Signale au gestionnaire que ce buffer ne sera plus utilis�.
     */
	void release(IBuffer* buffer);
private:
	std::list<IBuffer * > _freeBuffer;
	std::list<IBuffer * > _usedBuffer;
};
