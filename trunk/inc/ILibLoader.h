#ifndef     ILIBLOADER_H_
# define    ILIBLOADER_H_

# include <iostream>
# include <string>
# include <list>

# ifdef _WIN32
    #include <windows.h>
# endif

class   ILibLoader
{
public:
    #ifdef _WIN32
        typedef HMODULE libhandle;
    #else
        typedef void* libhandle;
    #endif

    virtual       void                      LoadOne(std::string file)      = 0;
    virtual       void                      LoadDir(std::string path)      = 0;
    virtual       void                      FreeOne(std::string file)      = 0;
    virtual       void                      FreeAll()                      = 0;
    virtual       libhandle                 getLib(std::string file)       = 0;
    virtual const std::list<std::string>    getLibList()             const = 0;
};

#endif      //ILIBLOADER_H_
