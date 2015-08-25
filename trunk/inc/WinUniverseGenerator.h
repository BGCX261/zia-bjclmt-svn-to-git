#ifndef     WINUNIVERSEGENERATOR
# define    WINUNIVERSEGENERATOR

# include   <map>
# include   "IUniverseGenerator.h"
# include   "WinLibLoader.h"

class WinUniverseGenerator : public IUniverseGenerator
{
public:
    WinUniverseGenerator(WinLibLoader* loader);
    ~WinUniverseGenerator(void);

    void        refresh();
    ZIA::API::IModuleManager*    createObject(std::string type);
    void        crushObject(std::string type, ZIA::API::IModuleManager* object);

private:
    typedef ZIA::API::IModuleManager* (*makerPtr)();
    typedef void (*crushPtr)(ZIA::API::IModuleManager*);

    WinLibLoader*                     loader;
    std::map<std::string, makerPtr>   loadedObjects;
    std::map<std::string, crushPtr>   crushObjects;
};

#endif      //WINUNIVERSEGENERATOR
