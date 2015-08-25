#ifndef     PARSERBASE_H_
# define    PARSERBASE_H_

# include   <iostream>
# include   <string>
# include   "ProducterStream.h"
# include   "ConsumerParser.h"

class   ParserBase
{
public:
    ParserBase();
    ~ParserBase();

    bool    parseFile(const std::string&    path);
    bool    parseString(const std::string&  str);

protected:
    virtual bool    entryPoint()   = 0;
    void            initBuffer();

    ProducterStream*    st;
    ConsumerParser*     parser;
};

#endif      //PARSERBASE_H_
