#pragma once

#include "types.hpp"
//#include "Request.h"
#include <map>

#ifdef _WIN32
	#include "wTCP.h"
#else
	#include "uTCP.h"
#endif

#define POOL_SESSION_SIZE 30;

#include <list>
#include "Config.h"
#include "AbsThreads.h"
#include "AbsMutex.h"
#include "BufferManager.h"
#include "Session.h"

using namespace ZIA::API;

class Session;

class Server
{
private:
	Server(void);
	~Server(void);
	//std::map<int , Request*> _requests;
	ITCP*							_network;
	ITCP*							_networkSsl;
	AbsThreads*						_threadNetwork;
	AbsThreads*						_threadNetworkSsl;
	uint16_t						_port;
	uint16_t						_portssl;
	BufferManager					_manBuf;
	static Server*					_server;
	std::list<Session *>			_freeSession;
	std::map<socket_t , Session*>	_sessions;
	Config							_xml;
	std::string						_www;
	std::list<std::string >			_moduleList;
	bool							_sslState;
	uint16_t						_sslPort;
	std::string						_sslCertificate;
	std::string						_ssl;

public:
	ITCP*							getNetwork();
	ITCP*							getNetworkSsl();
	const std::string &				getWWW();
	Config &						getXml();
	static Server&					getInstance();
	static void						releaseInstance();
	Session*						getSession(int socket);
	void							addSession(socket_t socket, char * ip, uint16_t port, bool isSsl);
	void							removeSession(socket_t socket);
	BufferManager*					manageBuf();
};

void *threadNetwork(void * object);
