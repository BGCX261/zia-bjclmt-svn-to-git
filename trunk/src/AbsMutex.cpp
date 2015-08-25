
#if defined _WIN32
#include	<windows.h>
#else
#include	<pthread.h>
#endif
#include	"AbsMutex.h"

AbsMutex::AbsMutex(void)
{
  this->_Initialized = false;
}

AbsMutex::~AbsMutex(void)
{
  this->DestroyMutex();
}

void	AbsMutex::CreateMutex()
{
#if defined _WIN32
  InitializeCriticalSection(&(this->_Mutex));
#else
  pthread_mutex_init(&(this->_Mutex), NULL);
#endif
  this->_Initialized = true;
}

void	AbsMutex::LockMutex()
{
  if (this->_Initialized)
    {
#if defined _WIN32
      EnterCriticalSection(&(this->_Mutex));
#else
      pthread_mutex_lock(&(this->_Mutex));
#endif
    }
}

void	AbsMutex::UnLockMutex()
{
  if (this->_Initialized)
    {
#if defined _WIN32
      LeaveCriticalSection(&(this->_Mutex));
#else
      pthread_mutex_unlock(&(this->_Mutex));
#endif
    }
}

void	AbsMutex::TryLockMutex()
{
  if (this->_Initialized)
    {
#if defined _WIN32
      TryEnterCriticalSection(&(this->_Mutex));
#else
      pthread_mutex_trylock(&(this->_Mutex));
#endif
    }
}

void	AbsMutex::DestroyMutex()
{
  if (this->_Initialized)
    {
#if defined _WIN32
      DeleteCriticalSection(&(this->_Mutex));
#else
      pthread_mutex_destroy(&(this->_Mutex));
#endif
      this->_Initialized = false;
    }
}
