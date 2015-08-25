#if !defined _WIN32
#ifndef     UNIXLIBLOADER_H_
# define    UNIXLIBLOADER_H_

# include <iostream>
# include <string>
# include <map>
# include <list>
# include "ILibLoader.h"

class UnixLibLoader : public ILibLoader
{
public:
    UnixLibLoader(void);
    ~UnixLibLoader(void);

          void                      LoadOne(std::string file);
          void                      LoadDir(std::string path);
          void                      FreeOne(std::string file);
          void                      FreeAll();
          libhandle                 getLib(std::string file);
    const std::list<std::string>    getLibList()             const;

private:
    std::map<std::string, libhandle> loaded;
};

#endif      //UNIXLIBLOADER_H_
#endif