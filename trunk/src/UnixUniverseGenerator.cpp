#ifndef _WIN32

# include <dlfcn.h>
# include "UnixUniverseGenerator.h"

UnixUniverseGenerator::UnixUniverseGenerator(UnixLibLoader* loader) : loader(loader)
{
}

UnixUniverseGenerator::~UnixUniverseGenerator(void)
{
}

void        UnixUniverseGenerator::refresh()
{
    std::list<std::string>              toLoad = this->loader->getLibList();
    std::list<std::string>::iterator    cur = toLoad.begin();
    std::list<std::string>::iterator    end = toLoad.end();

    for (; cur != end; ++cur)
    {
        ILibLoader::libhandle   hinstLib = this->loader->getLib(*cur);
        void*                   getSym = dlsym(hinstLib, "getInstance");
        void*                   crushSym = dlsym(hinstLib, "destroyInstance");

        makerPtr ProcAdd = (makerPtr) getSym;
        crushPtr ProcDel = (crushPtr) crushSym;

        if (ProcAdd != NULL && ProcDel != NULL)
        {
            this->loadedObjects[(*cur)] = ProcAdd;
            this->crushObjects[(*cur)] = ProcDel;
        }
    }
}

IObject*  UnixUniverseGenerator::createObject(std::string type)
{
    return ((*(this->loadedObjects[type]))());
}

void      UnixUniverseGenerator::crushObject(std::string type, IObject* object)
{
    (*(this->crushObjects[type]))(object);
}

#endif
