#pragma once

class IAbsMutex
{
public:
	virtual ~IAbsMutex(){};
	virtual void	CreateMutex() = 0;
	virtual void	LockMutex() = 0;
	virtual void	UnLockMutex() = 0;
	virtual void	TryLockMutex() = 0;
	virtual void	DestroyMutex() = 0;
};
