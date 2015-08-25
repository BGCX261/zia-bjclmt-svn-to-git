
#ifndef ABSTHREADS_H_
# define ABSTHREADS_H_

#if defined _WIN32
	#include <windows.h>
#else
	#include <pthread.h>
		#include <signal.h>
#endif
#include "IAbsThreads.h"

class AbsThreads : public IAbsThreads
{
public:
	AbsThreads(void);
	~AbsThreads(void);
	bool	AbsCreateThread(void* routine, void* parameters);
	void	AbsJoinThread();
	void	AbsKillThread();

private:
#if defined _WIN32
	HANDLE	_ThreadId;
#else
	pthread_t	_ThreadId;
#endif
};

#endif //ABSTHREADS_H_
