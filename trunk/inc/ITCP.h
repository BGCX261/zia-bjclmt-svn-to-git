#ifndef ITCP_H_
# define ITCP_H_

#include <string>

class ITCP
{
public:

	virtual ~ITCP(void){};

	virtual void start(const int port) = 0;
	virtual void stop(void) = 0;

	virtual void listenAccept(void) = 0;
	virtual void loop(void) = 0;

	virtual void read(int* client, char* data) = 0;
	virtual void write(const int client, char* data) = 0;

	virtual void parsing(std::string& buf, const int client) = 0;

};
#endif
