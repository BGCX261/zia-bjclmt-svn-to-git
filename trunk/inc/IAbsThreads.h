
#ifndef IABSTHREADS_H_
# define IABSTHREADS_H_

class IAbsThreads
{
public:
	virtual			~IAbsThreads(){};
	virtual bool	AbsCreateThread(void* routine, void* parameters) = 0;
	virtual void	AbsJoinThread() = 0;
	virtual void	AbsKillThread() = 0;
};

#endif //IABSTHREADS_H_
