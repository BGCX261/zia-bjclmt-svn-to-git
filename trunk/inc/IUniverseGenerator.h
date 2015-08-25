#ifndef     IUNIVERSEGENERATOR_H_
# define    IUNIVERSEGENERATOR_H_

# include   <iostream>
# include   <string>
# include   "IModuleManager.hpp"

class   IUniverseGenerator
{
public:
    virtual void						refresh()										= 0;
    virtual ZIA::API::IModuleManager*	createObject(std::string type)					= 0;
    virtual void		crushObject(std::string type, ZIA::API::IModuleManager* object)	= 0;
};

#endif      //IUNIVERSEGENERATOR
