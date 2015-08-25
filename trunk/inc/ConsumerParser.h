#pragma once

#include "ProducterStream.h"
#include <map>

class ConsumerParser
{
private:
	typedef std::string::iterator sit;

	ProducterStream&		_producter;
	std::string				buffer;
	sit						cursor;
	std::map<std::string, sit>	_substr;
    std::map<std::string, sit>  _saves;

public:
	ConsumerParser(ProducterStream &);
	~ConsumerParser(void);
	bool peekChar(const char& c);
	bool readChar(const char& c);
	bool readRange(char begin, char end);
    bool readText(const std::string& text);
	bool readEOF();
	bool readUntil(const char& c);
	bool readUntilEOF();
	bool readInteger();
	bool readIdentifier();
    bool readBlanks();
	bool beginCapture(const std::string& tag);
	bool endCapture(const std::string& tag, std::string& out);
    void saveContext(const std::string& tag);
    void restoreContext(const std::string& tag);
};
