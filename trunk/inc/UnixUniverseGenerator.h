#if !defined _WIN32
#ifndef     UNIXUNIVERSEGENERATOR
# define    UNIXUNIVERSEGENERATOR

# include   <map>
# include   "IUniverseGenerator.h"
# include   "UnixLibLoader.h"

class UnixUniverseGenerator : public IUniverseGenerator
{
public:
    UnixUniverseGenerator(UnixLibLoader* loader);
    ~UnixUniverseGenerator(void);

    void        refresh();
    IObject*    createObject(std::string type);
    void        crushObject(std::string type, IObject* object);

private:
    typedef IObject* (*makerPtr)();
    typedef void (*crushPtr)(IObject*);

    UnixLibLoader*                    loader;
    std::map<std::string, makerPtr>   loadedObjects;
    std::map<std::string, crushPtr>   crushObjects;
};

#endif      //UNIXUNIVERSEGENERATOR
#endif
