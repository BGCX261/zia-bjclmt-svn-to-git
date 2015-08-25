#ifndef UTCP_H_
# define UTCP_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <unistd.h>
#include <map>
#include <utility>
#include <string>
#include "AbsMutex.h"

#include "ITCP.h"

class Server;

class uTCP: public ITCP
{
public:

	uTCP();
	~uTCP(void);

	void start(const int port);
	void stop(void);
	void listenAccept(void);
	void loop(void);
	void read(int* client, char* data);
	void write(const int client, char* data);

	void parsing(std::string& buf, const int client); //a remplire celon utilisation

private:

	int msend(const int s, const char* data, const int len);
	int mrecv(const int s, char* data, const int len);

	static int _nbLoad;
	bool _isLoaded;

	int _serverSocket;
	sockaddr_in _serverService;

	int _clientSocket;
	sockaddr_in _clientService;

	std::map<int, sockaddr_in> _socketList;
	std::string _recv;
	std::string _send;
	AbsMutex * _mutex;
	int _clienttosend;
	int _clienttorecv;
	bool _tosend;
	bool loopOn;
};

typedef uTCP TCP;

#endif
