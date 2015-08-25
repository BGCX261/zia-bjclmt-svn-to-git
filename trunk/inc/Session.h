#pragma once

#include <fstream>
#include "HttpParser.h"
#include "ISession.hpp"
#include "IBuffer.hpp"
#include "Server.h"
#include <sstream>
class Server;

class Session : public ZIA::API::ISession
{
public:
	Session(ZIA::API::socket_t socket, ZIA::API::uint32_t ip, ZIA::API::uint16_t port, bool isSsl);
	Session();
	~Session(void);
	ZIA::API::socket_t getSocket() const;
	ZIA::API::uint32_t getIp() const;
	ZIA::API::uint16_t getPort() const;
	std::size_t getProcessedRequestCount() const;
	void setBuffer(ZIA::API::IBuffer * buffer = 0);
	ZIA::API::IBuffer * getBuffer();
	void reset(socket_t socket, uint32_t ip, uint16_t port, bool isSsl);
	void parse();
	void sendGetFile();
	bool	isSsl() const;
private :
	HttpParser::t_cmd_line _cmdLine;
	HttpParser::t_header_map _header;
	HttpParser			_parser;
	ZIA::API::IBuffer * _buffer;
	ZIA::API::socket_t _socket;
	ZIA::API::uint32_t _ip;
	ZIA::API::uint16_t _port;
	int		 _nbRequest;
	bool		_ssl;
};
