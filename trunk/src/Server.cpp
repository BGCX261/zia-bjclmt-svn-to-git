#include "Server.h"
#include <iostream>
#include "sslTCP.h"

Server::Server(void)
{
#if defined _WIN32
	this->_network = new wTCP();
	this->_networkSsl = new sslTCP();
#else
	this->_network = new uTCP();
#endif
	unsigned int j = POOL_SESSION_SIZE;
	this->_xml.RecupConfig("serverConfig.xml");
	this->_www = this->_xml.getDir();
	this->_moduleList = this->_xml.getModule();
	this->_port = atoi(this->_xml.getPort().c_str());
	this->_portssl = atoi(this->_xml.getSslPort().c_str());
	this->_threadNetwork = new AbsThreads();
	this->_threadNetworkSsl = new AbsThreads();
	for (unsigned int i = 0; i < j ;++i)
	{
		this->_freeSession.push_back(new Session());
	}
	this->_network->start(this->_port);
	this->_networkSsl->start(this->_portssl);
	this->_threadNetwork->AbsCreateThread(threadNetwork, this->_network);
	this->_threadNetworkSsl->AbsCreateThread(threadNetwork, this->_networkSsl);
	this->_xml.AffConfig();
	std::cout << "Server Launched" << std::endl << std::endl;
}

Server::~Server(void)
{
	std::list<Session *>::iterator it = this->_freeSession.begin();
	std::map<socket_t , Session*>::iterator	it2 = this->_sessions.begin();
	for (;it2 != this->_sessions.end(); ++it2)
	{
		this->_freeSession.push_back(it2->second);
	}
	this->_sessions.clear();
	for (; it != this->_freeSession.end(); ++it)
	{
		delete *it;
	}
	this->_freeSession.clear();
	this->_network->stop();
	this->_threadNetwork->AbsJoinThread();
	this->_networkSsl->stop();
	this->_threadNetworkSsl->AbsJoinThread();
	delete this->_threadNetwork;
	delete this->_threadNetworkSsl;
	delete this->_network;
	delete this->_networkSsl;
}

void Server::addSession(socket_t socket, char * ip, uint16_t port, bool isSsl)
{
	std::cout << "new session ip : " << ip << " socket : " << socket << " port : " << port << std::endl;
	int tmp;
	memcpy(&tmp, ip, 4);
	if (this->_freeSession.size() != 0)
	{
		this->_sessions[socket] = *this->_freeSession.begin();
		this->_freeSession.erase(this->_freeSession.begin());
		this->_sessions[socket]->reset(socket, tmp, port, isSsl);
		std::cout << std::endl << "Using free session  new socket : " << socket << " ip : " << ip << " port : " << port << std::endl << std::endl;
	}
	else
	{
		std::cout << std::endl << "NOFREE Creating new session  new socket : " << socket << " ip : " << ip << " port : " << port << std::endl << std::endl ;
		this->_sessions[socket] = new Session(socket, tmp, port, isSsl);
	}
}

void Server::removeSession(socket_t socket)
{
	unsigned int j = POOL_SESSION_SIZE;
	std::cout << "Session removed closed socket number : " << socket << std::endl;
	if (this->_freeSession.size() > j)
		delete (this->_sessions[socket]);
	else
	{
		this->_freeSession.push_back(this->_sessions[socket]);
		this->_freeSession.back()->setBuffer();
	}
	this->_sessions.erase(socket);
}

Server *Server::_server = NULL;

Server& Server::getInstance()
{
	if  (Server::_server == NULL)
	{
		Server::_server = new Server();
	}
	return (*Server::_server);
}

void Server::releaseInstance()
{
	if (Server::_server != NULL)
	{
		delete Server::_server;
		Server::_server = NULL;
	}
}

Session * Server::getSession(int socket)
{
	std::map<socket_t , Session*>::iterator tmp;
	if ((tmp = this->_sessions.find(socket)) != this->_sessions.end())
		return (tmp->second);
	return (0);
}

Config & Server::getXml()
{
	return (this->_xml);
}

BufferManager * Server::manageBuf()
{
	return (&this->_manBuf);
}

const std::string &		Server::getWWW()
{
	return (this->_www);
}

ITCP *		Server::getNetwork()
{
	return (this->_network);
}

ITCP*		Server::getNetworkSsl()
{
	return this->_networkSsl;
}

void *threadNetwork(void * object)
{
	TCP * net = static_cast<TCP*>(object);
	net->loop();
	return NULL;
}
