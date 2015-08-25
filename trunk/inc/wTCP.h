#ifndef WTCP_H_
# define WTCP_H_

#define _WINSOCK2API_
#include <WinSock2.h>
#include <windows.h>
#include <map>
#include <utility>

#include "ITCP.h"
#include "AbsMutex.h"

class Server;

class wTCP: public ITCP
{
public:

	wTCP();
	~wTCP(void);

	void start(const int port);
	void stop(void);
	void listenAccept(void);
	void loop(void);
	void write(const int client, char* data);
	void read(int* client, char* data);

	void parsing(std::string& buf, const int client); //a remplire celon utilisation

private:

	int msend(const SOCKET s, char* data, const int len);
	int mrecv(const SOCKET s, char* data, const int len);

	static int _nbLoad;
	bool _isLoaded;

	SOCKET _serverSocket;
	sockaddr_in _serverService;

	SOCKET _clientSocket;
	sockaddr_in _clientService;

	std::map<SOCKET, sockaddr_in> _socketList;
	std::string _recv;
	std::string _send;
	AbsMutex * _mutex;
	int _clienttosend;
	int _clienttorecv;
	bool _tosend;
	bool _loopOn;
};

typedef wTCP TCP;

#endif
