#ifndef		ILOADER_H_
# define	ILOADER_H_

#include	<iostream>

template<typename T>
class ILoader
{
public:
	virtual ~ILoader()	{}
	virtual	bool		LoadLibraries(const char*) = 0;
	virtual bool		LoadLibrary(const char*) = 0;
	virtual	bool		CloseLibraries() = 0;
	virtual	bool		CloseLibrary() = 0;
	virtual T*			getArchiver() = 0;
};

#endif	// ILOADER_H_
