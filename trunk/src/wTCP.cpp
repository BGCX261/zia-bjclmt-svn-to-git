#ifdef _WIN32

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <exception>
#include "Server.h"
#include "wTCP.h"

#define LEN 4096

int wTCP::_nbLoad = 0;

wTCP::wTCP()
{
	if (wTCP::_nbLoad == 0)
	{
		WSADATA wsaData;
		WORD wVersion = MAKEWORD(2,2);
		if(WSAStartup(wVersion, &wsaData) != 0)
		{
			//throw std::exception("WSAStartup error");
			std::cout <<"WSAStartup error: " << WSAGetLastError() << std::endl;
		}
	}
	wTCP::_nbLoad++;
	this->_isLoaded = true;
	this->_loopOn = true; //mettre la boucle en marche
	this->_mutex = new AbsMutex();
}

wTCP::~wTCP(void)
{
	wTCP::_nbLoad--;
	if (wTCP::_nbLoad <= 0)
	{
		this->_isLoaded = false;
		wTCP::_nbLoad = 0;
		WSACleanup();
	}
	this->_mutex->DestroyMutex();
	delete this->_mutex;
}

void wTCP::start(const int port)
{
	this->_mutex->UnLockMutex();
	if (this->_isLoaded == true)
	{
		if ((this->_serverSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, NULL)) == SOCKET_ERROR)
		{
			//throw std::exception("WSASocket error");
			std::cout <<"WSASocket error: " << WSAGetLastError() << std::endl;
		}

		ZeroMemory(&this->_serverService, sizeof(this->_serverService));
		this->_serverService.sin_family = AF_INET;
		this->_serverService.sin_addr.s_addr = INADDR_ANY;
		WSAHtons(this->_serverSocket, port, &this->_serverService.sin_port);

		if (bind(this->_serverSocket, (SOCKADDR*)&this->_serverService, sizeof(this->_serverService)) == SOCKET_ERROR) 
		{
			closesocket(this->_serverSocket);
			//throw std::exception("Bind error");
			std::cout <<"Bind error: " << WSAGetLastError() << std::endl;
		}
	}
}

void wTCP::stop(void)
{
	closesocket(this->_clientSocket);
	this->_mutex->LockMutex();
	this->_loopOn = false;
	closesocket(this->_serverSocket);
	std::map<SOCKET, sockaddr_in>::iterator ita = this->_socketList.begin();
	for(;ita != this->_socketList.end(); ++ita)
	{
		closesocket(ita->first);
	}
	this->_socketList.clear();
	this->_mutex->UnLockMutex();
}

void wTCP::listenAccept(void)
{
	int size = sizeof(this->_clientService);
	if ((listen(this->_serverSocket, SOMAXCONN)) == SOCKET_ERROR) 
	{
		//throw std::exception("Listen error");
		std::cout <<"Listen error: " << WSAGetLastError() << std::endl;
		closesocket(this->_serverSocket);
	}

	if ((this->_clientSocket = WSAAccept(this->_serverSocket, (struct sockaddr*)&this->_clientService, &size, NULL, NULL)) == INVALID_SOCKET) 
	{
		//throw std::exception("WSAAccept error");
		std::cout <<"WSAAccept error: " << WSAGetLastError() << std::endl;
		closesocket(this->_clientSocket);
	}
	closesocket(this->_serverSocket);
}

int wTCP::msend(const SOCKET s, char *data, const int len)
{
	WSABUF DataBuf;
	DWORD SendBytes = 0;

	DataBuf.len = len;
	DataBuf.buf = data;

	if (WSASend(s, &DataBuf, 1, &SendBytes, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		//throw std::exception("WSASend error");
		std::cout <<"WSASend error: " << WSAGetLastError() << std::endl;
	}
	return SendBytes;
}

int wTCP::mrecv(const SOCKET s, char *data, const int len)
{
	WSABUF DataBuf;
	DWORD RecvBytes = 0;
	DWORD Flags = 0;

	DataBuf.len = len;
	DataBuf.buf = data;

	if (WSARecv(s, &DataBuf, 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
	{
		//throw std::exception("WSARecv error");
		std::cout <<"WSARecv error: " << WSAGetLastError() << std::endl;
	}
	return RecvBytes;
}

void wTCP::loop(void)
{
	std::cout << "Network loop launched" << std::endl;
	int bytesrecv = 1;
	int bytessent = 1;
	int nbfd = 3;

	this->_tosend = false;
	std::string		buff_send;

	int size = sizeof(this->_clientService);

	FD_SET Writer;
	FD_SET Reader;
	ULONG NonBlock = 1;

	std::map<SOCKET, sockaddr_in>::iterator ita;
	std::map<SOCKET, sockaddr_in>::iterator ite;

	if ((listen(this->_serverSocket, SOMAXCONN)) == SOCKET_ERROR) 
	{
		//throw std::exception("Listen error");
		std::cout <<"Listen error: " << WSAGetLastError() << std::endl;
		closesocket(this->_serverSocket);
	}

	while (this->_loopOn == true)
	{
		//initialisation des champ de bits
		FD_ZERO(&Reader);
		FD_ZERO(&Writer);

		FD_SET(this->_serverSocket, &Reader);

		ita = this->_socketList.begin();
		ite = this->_socketList.end();

		//changement d'etat a chaque tour de chaque socket
		for(;ita != ite; ++ita)
		{
			FD_SET((*ita).first, &Writer);
			FD_SET((*ita).first, &Reader);
		}
		/**********************************SELECT***********************************/
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 10000;
		if (select(nbfd + 1, &Reader, &Writer, NULL, &timeout) == SOCKET_ERROR)
		{
			//throw std::exception("Select error");
			std::cout <<"Select error: " << WSAGetLastError() << std::endl;
		}
		this->_mutex->LockMutex();
		if (this->_loopOn == true)
		{
			/************************************ACCEPT*********************************/
			if (FD_ISSET(this->_serverSocket, &Reader))
			{
				if ((this->_clientSocket = WSAAccept(this->_serverSocket, (struct sockaddr*)&this->_clientService, &size, NULL, NULL)) == INVALID_SOCKET) 
				{
					//throw std::exception("WSAAccept error");
					std::cout <<"WSAAccept error: " << WSAGetLastError() << std::endl;
					closesocket(this->_clientSocket);
				}
				NonBlock = 1;
				/***************************Settage du timeout******************************/
				timeval tv;
				tv.tv_sec  = 0;
				tv.tv_usec = 100;

				// Set Timeout for recv call
				if(setsockopt(this->_clientSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&tv), sizeof(timeval)))
				{
					std::cout <<"Setsockoption error: timeout" << WSAGetLastError() << std::endl;
				}
				/***************************STOCKAGE DS LES MAPS************************/
				this->_socketList[this->_clientSocket] = this->_clientService;
				std::cout << "New client socket : " << this->_clientSocket << std::endl;
				Server::getInstance().addSession(this->_clientSocket, inet_ntoa(this->_clientService.sin_addr), this->_clientService.sin_port, false);
				nbfd++;
				
				//nettoyage de la socket client
				ZeroMemory(&this->_clientSocket, sizeof(this->_clientSocket));
				ZeroMemory(&this->_clientService, sizeof(this->_clientService));
			}
			/********************CHECK DES DIFFERENTES SOCKETS CLIENTS******************/
			ita = this->_socketList.begin();
			ite = this->_socketList.end();

			for(;ita != ite; ++ita)
			{
				if (FD_ISSET((*ita).first, &Reader))
				{
					char* r = new char[LEN];
					if ((bytesrecv = mrecv((*ita).first, r, LEN)) == NULL)
					{
						Server::getInstance().removeSession(ita->first);
						closesocket(ita->first);
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
				if (FD_ISSET((*ita).first, &Writer))
				{
					if (this->_tosend == true && (*ita).first == this->_clienttosend)
					{
						buff_send.clear();
						buff_send = this->_send;
						if ((bytessent = msend((*ita).first, (char*)(buff_send.c_str()), buff_send.length())) == NULL)
						{
							Server::getInstance().removeSession(ita->first);
							closesocket(ita->first);
							this->_socketList.erase(ita); //effacemment ds la map 
							nbfd--;
							break;
						}
						this->_tosend = false;
					}
				}
			}
		}
		this->_mutex->UnLockMutex();
		Sleep(10);
	}
}

void wTCP::read(int* client, char* data)
{
	const char* tmp = new char[this->_recv.length()];
	tmp = this->_recv.c_str();
	data = (char*)tmp;
	ZeroMemory(&this->_recv, sizeof(this->_recv));
	client = &this->_clienttorecv;
}

void wTCP::write(const int client, char* data)
{
	this->_clienttosend = client;
	this->_send = data;
	this->_tosend = true;
}

void wTCP::parsing(std::string& buf, const int client)
{		
	Session * tmpSession;
	if ((tmpSession = Server::getInstance().getSession(client)) != 0)
	{
		tmpSession->setBuffer(Server::getInstance().manageBuf()->allocate(buf));
	}
}

#endif
