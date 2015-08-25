#ifndef _WIN32

#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <exception>

#include "Server.h"
#include "uTCP.h"

#define LEN 4096

int uTCP::_nbLoad = 0;

uTCP::uTCP(Server& parent): _parent(parent)
{
	uTCP::_nbLoad ++;
	this->_isLoaded = true;
	this->loopOn = true;
	this->_mutex = new AbsMutex();
}

uTCP::~uTCP(void)
{
	uTCP::_nbLoad--;
	if (uTCP::_nbLoad <= 0)
	{
		this->_isLoaded = false;
	}
	this->_mutex->DestroyMutex();
	delete this->_mutex;	
}

void uTCP::start(const int port)
{
	this->_mutex->UnLockMutex();
	if (this->_isLoaded == true)
	{
		if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
			//throw std::exception("socket error");
			perror("socket : ");
		}

		memset(&this->_serverService, 0, sizeof(this->_serverService));
		this->_serverService.sin_family = AF_INET;
		this->_serverService.sin_addr.s_addr = INADDR_ANY;
		this->_serverService.sin_port = htons(port);

		if (bind(this->_serverSocket, (struct sockaddr*)&this->_serverService, sizeof(this->_serverService)) < 0) 
		{
			close(this->_serverSocket);
			//throw std::exception("Bind error");
			perror("bind error : ");
		}
	}
}

void uTCP::stop(void)
{
	close(this->_clientSocket);
	this->_mutex->LockMutex();
	this->_loopOn = false;
	close(this->_serverSocket);
	std::map<int, sockaddr_in>::iterator ita = this->_socketList.begin();
	for(;ita != this->_socketList.end(); ++ita)
	{
		close(ita->first);
	}
	this->_socketList.clear();
	this->_mutex->UnLockMutex();
}

void uTCP::listenAccept(void)
{
	socklen_t size = sizeof(this->_clientService);
	if ((listen(this->_serverSocket, SOMAXCONN)) < 0) 
	{
		//throw std::exception("Listen error");
		perror("Listen error : ");
		close(this->_serverSocket);
	}

	if ((this->_clientSocket = accept(this->_serverSocket, (struct sockaddr*)&this->_clientService, &size)) < 0) 
	{
		//throw std::exception("Accept error");
		perror("Accept Error : ");
		close(this->_clientSocket);
	}
	close(this->_serverSocket);
}

int uTCP::msend(const int s, const char *data, const int len)
{
	int SendBytes = 0;

	if ((SendBytes = send(s, data, len, 0)) < 0)
	{
		//throw std::exception("Send error");
		perror("Send error : ");
	}
	return SendBytes;
}

int uTCP::mrecv(const int s, char *data, const int len)
{
	int RecvBytes = 0;

	if ((RecvBytes = recv(s, data, len, 0)) < 0)
	{
		//throw std::exception("Recv error");
		perror("Recv error : ");

	}
	return RecvBytes;
}

void uTCP::loop(void)
{
	std::cout << "Network loop launched" << std::endl;
	int bytesrecv = 1;
	int bytessent = 1;
	int nbfd = 3;

	this->_tosend = false;
	std::string buff_send;

	socklen_t size = sizeof(this->_clientService);

	fd_set writefds;
	fd_set readfds;
	int NonBlock = 1;

	std::map<int, sockaddr_in>::iterator ita ;
	std::map<int, sockaddr_in>::iterator ite ;

	if ((listen(this->_serverSocket, SOMAXCONN)) < 0) 
	{
		//throw std::exception("Listen error");
		perror("Listen error : ");
		close(this->_serverSocket);
	}

	while(this->loopOn == true)
	{
		//initialisation des champ de bits
		FD_ZERO(&writefds);
		FD_ZERO(&readfds);

		FD_SET(this->_serverSocket, &readfds);

		ita = this->_socketList.begin();
		ite = this->_socketList.end();

		//changement d'etat a chaque tour de chaque socket
		for(;ita != ite; ++ita)
		{
			FD_SET((*ita).first, &writefds);
			FD_SET((*ita).first, &readfds);
		}
		/**********************************SELECT***********************************/
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 10000;
		if (select(nbfd + 1, &readfds, &writefds, NULL, &timeout) < 0)
		{
			//throw std::exception("Select error");
			perror("Select error : ");
		}
		this->_mutex->LockMutex();
		if (this->_loopOn == true)
		{
			/************************************ACCEPT*********************************/
			if (FD_ISSET(this->_serverSocket, &readfds))
			{
				if ((this->_clientSocket = accept(this->_serverSocket, (struct sockaddr*)&this->_clientService, &size)) < 0) 
				{
					//throw std::exception("accept error");
					perror("Accept error : ");
					close(this->_clientSocket);
				}
				NonBlock = 1;
				if (ioctl(this->_clientSocket, FIONBIO, &NonBlock) < 0)
				{
					//throw std::exception("ioctlsocket error");
					perror("Ioctlsocket error : ");
				}
				/***************************STOCKAGE DS LES MAPS************************/
				this->_socketList[this->_clientSocket] = this->_clientService;
				std::cout << "New client socket : " << this->_clientSocket << std::endl;
				Server::addSession(this->_clientSocket, this->_clientService.sin_addr, this->_clientService.sin_port);
				nbfd++;

				//nettoyage de la socket client
				memset(&this->_clientSocket, 0, sizeof(this->_clientSocket));
				memset(&this->_clientService, 0, sizeof(this->_clientService));
			}
			/********************CHECK DES DIFFERENTES SOCKETS CLIENTS******************/
			ita = this->_socketList.begin();
			ite = this->_socketList.end();

			for(;ita != ite; ++ita)
			{
				if (FD_ISSET((*ita).first, &readfds))
				{
					char* r = new char[LEN];
					if ((bytesrecv = mrecv((*ita).first, r, LEN)) == 0)
					{	
						Server::getInstance().removeSession(ita->first);
						close(ita->first);
						this->_socketList.erase(ita);		   //effacement ds la map	
						nbfd--;
						break;
					}
					else
					{
						r[bytesrecv] = '\0';
						this->_recv.clear();
						this->_recv = r;
						this->_clienttorecv = (*ita).first;

						//Commande a effectuer lors de la reception de quelque chose
						this->parsing(this->_recv, (*ita).first);

					}
					delete r;
				}
				if (FD_ISSET((*ita).first, &writefds))
				{
					if (this->_tosend == true && (*ita).first == this->_clienttosend)
					{
						buff_send.clear();
						buff_send = this->_send;
						if ((bytessent = msend((*ita).first, (char*)(buff_send.c_str()), buff_send.length())) == 0)
						{
							Server::getInstance().removeSession(ita->first);
							close(ita->first);
							this->_socketList.erase(ita);		   //effacement ds la map
							nbfd--;
							break;
						}
						this->_tosend = false;
					}
				}
			}
		}
		this->_mutex->UnLockMutex();
		usleep(10);
	}
}

void uTCP::read(int* client, char* data)
{
	const char* tmp = new char[this->_recv.length()];
	tmp = this->_recv.c_str();
	data = (char*)tmp;
	memset(&this->_recv, 0, sizeof(this->_recv));
	client = &this->_clienttorecv;
}

void uTCP::write(const int client, char* data)
{
	this->_clienttosend = client;
	this->_send = data;
	this->_tosend = true;
}

void uTCP::parsing(std::string& buf, const int client)
{
	Session * tmpSession;
	if ((tmpSession = Server::getInstance().getSession(client)) != 0)
	{
		tmpSession->setBuffer(Server::getInstance().manageBuf()->allocate(buf));
	}
}

#endif
