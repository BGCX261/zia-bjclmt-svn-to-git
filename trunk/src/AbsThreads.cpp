#include <iostream>
#include "AbsThreads.h"
//#include "threadException.h"

AbsThreads::AbsThreads(void)
{
	this->_ThreadId = NULL;
}

AbsThreads::~AbsThreads(void)
{
	//AbsKillThread();
}

bool	AbsThreads::AbsCreateThread(void* routine, void* parameters)
{
#if defined _WIN32
	if (this->_ThreadId == NULL)
		this->_ThreadId = CreateThread(NULL, 0, static_cast<LPTHREAD_START_ROUTINE>(routine), parameters, 0, NULL);
	if (this->_ThreadId == NULL)
	{
//		throw (threadException("Unable to create thread (CreateThread)"));
		return (false);
	}
	return true;
#else
	if (this->_ThreadId == NULL)
	if (pthread_create(&(this->_ThreadId), NULL, (void*(*)(void*)) routine, parameters) != 0)
	  {
		throw (threadException("Unable to create thread (pthread_create)"));
		return (false);
	  }
  return true;
#endif
}

void	AbsThreads::AbsJoinThread()
{
#if defined _WIN32
	if (this->_ThreadId != NULL)
	{
		WaitForSingleObject(this->_ThreadId, INFINITE);
	}
#else
	if (this->_ThreadId != NULL && pthread_join(this->_ThreadId, NULL) != 0)
	{
	  throw (threadException("Unable to join thread (pthread_join)"));
	}
#endif
}

void	AbsThreads::AbsKillThread()
{
#if defined _WIN32
	if (this->_ThreadId != NULL)
	{
		std::cout << "ExitCode: " << TerminateThread(this->_ThreadId, 0) << std::endl;
		CloseHandle(this->_ThreadId);
		this->_ThreadId = NULL;
	}
#else
	if (this->_ThreadId != NULL && pthread_kill(this->_ThreadId, SIGSTOP) != 0)
	throw (threadException("Unable to kill thread (pthread_kill SIGSTOP)"));
  else
	this->_ThreadId = NULL;
#endif
}
