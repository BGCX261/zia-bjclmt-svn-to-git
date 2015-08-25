#ifndef     WINLIBLOADER_H_
# define    WINLIBLOADER_H_

# include <iostream>
# include <string>
# include <map>
# include "ILibLoader.h"

class WinLibLoader : public ILibLoader
{
public:
	WinLibLoader(void);
	~WinLibLoader(void);
	void							LoadOne(std::string file);
	void							LoadDir(std::string path);
	void							FreeOne(std::string file);
	void							FreeAll();
	libhandle						getLib(std::string file);
	const std::list<std::string>	getLibList() const;

private:
	std::map<std::string, libhandle> loaded;
};

#endif      //WINLIBLOADER_H_
