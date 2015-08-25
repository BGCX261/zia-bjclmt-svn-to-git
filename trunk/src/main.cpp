#include <string>
#include <iostream>

#include "Server.h"
#include "AbsThreads.h"


class ServerManager
{
public:
	static void	*start(void *)
	{
		Server::getInstance();
		return NULL;
	}

	static bool	restart(const std::string& pcmd)
	{
		Server::releaseInstance();
#if defined	_WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		Server::getInstance();
		return true;
	}

	static bool	stop(const std::string& pcmd)
	{
		Server::releaseInstance();
		return false;
	}

	static bool	load(const std::string& pcmd)
	{
		std::size_t		found;
		std::string	module_name;

		found = pcmd.find_first_of(" ");
		module_name = pcmd.substr(found, pcmd.size() - found);
		if (module_name.size() <= 0)
			return true;
		//do something with the module name...
		return true;
	}
};

typedef	bool (*ptrFctCmd)(const std::string&);

int												main(void)
{
	bool										isStarted = true;
	std::size_t									found;
	std::string									command;
	std::map<std::string, ptrFctCmd>			commandList;
	std::map<std::string, ptrFctCmd>::iterator	itCommand;
	
	commandList["restart"] = &ServerManager::restart;
	commandList["stop"] = &ServerManager::stop;
	commandList["load"] = &ServerManager::load;
	
	//start ...

	Server::getInstance();
	while (isStarted)
	{
		std::cout << "Command: ";
		std::cin >> command;
		found = command.find_first_of(" ");
		if (found == std::string::npos)
		{
			found = command.size();
		}
		std::string tmpCmd = command.substr(0, found);
		if ((itCommand = commandList.find(tmpCmd)) != commandList.end())
		{
			isStarted = (*itCommand->second)(command);
		}
		else
		{
			std::cout << "Alice is Dead" << std::endl;
		}
	}
	//stop
	std::cout << "stoped" << std::endl;
	return 0;
}
