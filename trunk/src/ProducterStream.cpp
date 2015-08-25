#include "ProducterStream.h"

ProducterStream::ProducterStream(void)
{
}

ProducterStream::~ProducterStream(void)
{
}

std::string		ProducterStream::nextString()
{
    if (this->_type == "file")
    {
	    std::string str = "";

        if (this->_stream.fail() || this->_stream.eof())
		    throw std::exception();
        while (!(this->_stream.eof()))
            str += this->_stream.get();
	    return str;
    }
    else
    {
        return (this->_buffer);
    }
}

bool ProducterStream::loadFile(const char *path)
{
    this->_type = "file";
    this->_stream.open(path, std::ifstream::in);
    if (this->_stream.is_open())
        return (true);
    else
        return (false);
}

bool ProducterStream::loadString(const std::string& str)
{
    this->_type = "string";
    this->_buffer = str;
    if (str != "")
        return (true);
    return (false);
}

bool ProducterStream::loadStdin()
{
	this->_type = "input";
	return true;
}
