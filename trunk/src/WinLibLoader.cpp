#ifdef _WIN32

# include "WinLibLoader.h"

WinLibLoader::WinLibLoader(void)
{
}

WinLibLoader::~WinLibLoader(void)
{
}

void    WinLibLoader::LoadOne(std::string path)
{
    libhandle hinstDLL;
    size_t pos = path.find_last_of("\\");

    if (pos != std::string::npos)
    {
        std::string file = path.substr(pos + 1, path.size());
        if ((hinstDLL = LoadLibrary(path.c_str())))
            this->loaded[file] = hinstDLL;
    }
}

void    WinLibLoader::LoadDir(std::string path)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE hFind;
   std::string  toFind = path + "*.dll";

   hFind = FindFirstFile(toFind.c_str(), &FindFileData);
   if (hFind != INVALID_HANDLE_VALUE)
   {
       LoadOne(path + FindFileData.cFileName);
       while (FindNextFile(hFind, &FindFileData))
            LoadOne(path + FindFileData.cFileName);
   }
   FindClose(hFind);
}

void    WinLibLoader::FreeOne(std::string file)
{
    int nb = this->loaded.count(file);

    if (nb > 0)
    {
	    FreeLibrary(this->loaded[file]);
	    this->loaded.erase(file);
    }
}

void    WinLibLoader::FreeAll()
{
    std::map<std::string, libhandle>::iterator    begin = this->loaded.begin();
    std::map<std::string, libhandle>::iterator    end = this->loaded.end();

    while (begin != end)
    {
        FreeOne((*begin).first);
        begin = this->loaded.begin();
    }
}

ILibLoader::libhandle WinLibLoader::getLib(std::string file)
{
    return (this->loaded[file]);
}

const std::list<std::string>  WinLibLoader::getLibList() const
{
    std::list<std::string>  libList;
    std::map<std::string, libhandle>::const_iterator    cur = this->loaded.begin();
    std::map<std::string, libhandle>::const_iterator    end = this->loaded.end();

    for (; cur != end; ++cur)
        libList.push_back((*cur).first);
    return (libList);
}

#endif
