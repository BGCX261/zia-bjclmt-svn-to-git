#include "ConsumerParser.h"
#include <cstring>

ConsumerParser::~ConsumerParser(void){}

ConsumerParser::ConsumerParser(ProducterStream &p)
: _producter(p), buffer("")
{
	cursor = this->buffer.begin();
}

bool ConsumerParser::peekChar(const char& c)
{
	//if (this->cursor == this->buffer.end())
	//{
	//	try 
	//	{
 //           bool empty = this->buffer.empty();

 //           if (!empty)
 //               --(this->cursor);
	//		this->buffer = this->_producter.nextString();
 //           if (empty)
 //               this->cursor = this->buffer.begin();
	//	}
	//	catch(std::exception)
	//	{
	//		return false;
	//	}
	//}
	//if (this->cursor == this->buffer.end())
	//	return false;
 //   std::cout << std::endl << "buffer: " << *(this->cursor) << std::endl;
	//if (*(this->cursor) == c)
	//	return true;
	//return false;

    try
    {
        if (this->buffer.empty())
        {
            this->buffer = this->_producter.nextString();
            this->cursor = this->buffer.begin();
        }
    }
    catch (std::exception)
    {
        return (false);
    }
    if (this->cursor == this->buffer.end())
        return (false);
    if (*(this->cursor) == c)
        return (true);
    return (false);
}

bool ConsumerParser::readChar(const char& c)
{
	if (this->peekChar(c))
	{
		++(this->cursor);
		return true;
	}
	return false;
}

bool ConsumerParser::readRange(char begin, char end)
{
	if (begin > end)
		return false;
	for (int i = begin; i <= end; i++)
	{
		if (peekChar(i))
		{
			readChar(i);
			return true;
		}
	}
	return false;
}

bool ConsumerParser::readText(const std::string& text)
{
	sit tmp;
	tmp = this->cursor;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (!readChar(text[i]))
		{
			this->cursor = tmp;
			return false;
		}
	}
	return true;
}

bool ConsumerParser::readEOF()
{
	if (this->cursor == this->buffer.end())
		return true;
	return false;
}

bool ConsumerParser::readUntil(const char& c)
{
	sit tmp;

	tmp = this->cursor;
	while (this->cursor != this->buffer.end() && !peekChar(c))
		++(this->cursor);
    if (this->cursor == this->buffer.end())
        --(this->cursor);
	if (peekChar(c))
		return true;
	this->cursor = tmp;
	return false;
}

bool ConsumerParser::readUntilEOF()
{
    peekChar(' ');
    while (!readEOF())
    {
        peekChar(' ');
        ++(this->cursor);
    }
    return (true);
}

bool ConsumerParser::readInteger()
{
	sit tmp = this->cursor;
	if (readRange('0', '9'))
	{
		while (readRange('0', '9'));
		return true;
	}
	this->cursor = tmp;
	return false;
}

bool ConsumerParser::readIdentifier()
{
	sit tmp = this->cursor;

	if (readRange('a', 'z') || readRange('A', 'Z') || readChar('_'))
	{
		while (readRange('0', '9') || readRange('a', 'z') || readRange('A', 'Z') || readChar('_'));
		return true;
	}
	this->cursor = tmp;
	return false;
}

bool ConsumerParser::readBlanks()
{
    bool    res = false;
    std::string test;

    while (readChar(' ') || readChar('\n') || readChar('\t') || readChar('\r'))
        res = true;
    return (res);
}

bool ConsumerParser::beginCapture(const std::string& tag)
{
	_substr[tag] = this->cursor;
	return true;
}

bool ConsumerParser::endCapture(const std::string& tag, std::string& out)
{
	sit tmp = _substr[tag];
	if (tmp == this->cursor)
		return false;
	out = std::string(tmp, this->cursor);
	return true;
}

void     ConsumerParser::saveContext(const std::string& tag)
{
	_saves[tag] = this->cursor;
}

void     ConsumerParser::restoreContext(const std::string& tag)
{
    this->cursor = this->_saves[tag];
}