#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <exception>

class ProducterStream
{
public:
	ProducterStream(void);
	~ProducterStream(void);
	std::string nextString();
	bool loadFile(const char *path);
    bool loadString(const std::string& str);
	bool loadStdin();

private:
	std::ifstream		_stream;
    std::string         _buffer;
    std::string         _type;
};
