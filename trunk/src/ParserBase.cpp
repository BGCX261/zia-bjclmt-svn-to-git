#include    "ParserBase.h"

ParserBase::ParserBase() : st(NULL), parser(NULL)
{
}

ParserBase::~ParserBase()
{
    if (this->st != NULL && this->parser != NULL)
    {
        delete  this->st;
        delete  this->parser;
    }
}

bool        ParserBase::parseFile(const std::string& path)
{
    initBuffer();
	if (this->st->loadFile(path.c_str()))
    {
        this->parser->peekChar(' ');
        if (entryPoint())
            return (true);
    }
    return (false);
}

bool        ParserBase::parseString(const std::string& str)
{
    initBuffer();
	if (this->st->loadString(str))
    {
        this->parser->peekChar(' ');
        if (entryPoint())
            return (true);
    }
    return (false);
}

void        ParserBase::initBuffer()
{
    if (this->st != NULL && this->parser != NULL)
    {
        delete  this->st;
        delete  this->parser;
    }
    this->st = new ProducterStream();
    this->parser = new ConsumerParser(*st);
}
