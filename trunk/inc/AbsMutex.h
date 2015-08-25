
#ifndef ABSMUTEX_H_
# define ABSMUTEX_H_

#if defined _WIN32
#include	<windows.h>
#else
#include	<pthread.h>
#endif
#include	"IAbsMutex.h"

class AbsMutex : public IAbsMutex
{
public:
	AbsMutex(void);
	~AbsMutex(void);
	void	CreateMutex();
	void	LockMutex();
	void	UnLockMutex();
	void	TryLockMutex();
	void	DestroyMutex();

private:
#if defined _WIN32
	CRITICAL_SECTION	_Mutex;
#else
	pthread_mutex_t		_Mutex;
#endif
	bool			_Initialized;
};

#endif //ABSMUTEX_H_
