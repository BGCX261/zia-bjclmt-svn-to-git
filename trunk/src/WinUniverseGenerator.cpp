#ifdef _WIN32

# include "WinUniverseGenerator.h"

WinUniverseGenerator::WinUniverseGenerator(WinLibLoader* loader) : loader(loader)
{
}

WinUniverseGenerator::~WinUniverseGenerator(void)
{
}

void        WinUniverseGenerator::refresh()
{
    std::list<std::string>              toLoad = this->loader->getLibList();
    std::list<std::string>::iterator    cur = toLoad.begin();
    std::list<std::string>::iterator    end = toLoad.end();

    for (; cur != end; ++cur)
    {
        ILibLoader::libhandle   hinstLib = this->loader->getLib(*cur);
        makerPtr ProcAdd = (makerPtr) GetProcAddress(hinstLib, "getInstance");
        crushPtr ProcDel = (crushPtr) GetProcAddress(hinstLib, "destroyInstance");

        if (ProcAdd != NULL && ProcDel != NULL)
        {
            this->loadedObjects[(*cur)] = ProcAdd;
            this->crushObjects[(*cur)] = ProcDel;
        }
    }
}

ZIA::API::IModuleManager*  WinUniverseGenerator::createObject(std::string type)
{
    return ((*(this->loadedObjects[type]))());
}

void      WinUniverseGenerator::crushObject(std::string type, ZIA::API::IModuleManager* object)
{
    (*(this->crushObjects[type]))(object);
}

#endif
