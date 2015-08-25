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
     * Création d'un nouveau buffer à partir d'une
     * chaîne de caractères.
     */
	IBuffer* allocate(std::string const& string);

    /**
     * Création d'un nouveau buffer à partir d'un
     * tableau de données brutes.
     */
	IBuffer* allocate(char const* data, std::size_t size);

    /**
     * Création d'un nouveau buffer de la taille spécifiée.
     */
	IBuffer* allocate(std::size_t size);

    /**
     * Signale au gestionnaire que ce buffer ne sera plus utilisé.
     */
	void release(IBuffer* buffer);
private:
	std::list<IBuffer * > _freeBuffer;
	std::list<IBuffer * > _usedBuffer;
};
