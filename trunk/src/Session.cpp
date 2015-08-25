#include "Session.h"

Session::Session(ZIA::API::socket_t socket, ZIA::API::uint32_t ip, ZIA::API::uint16_t port, bool isSsl):
_ip(ip), _port(port), _socket(socket), _ssl(isSsl)
{
	this->_nbRequest = 0;
	this->_buffer = 0;
}

Session::Session(void)
{
}

Session::~Session(void)
{
	if (this->_buffer)
		Server::getInstance().manageBuf()->release(this->_buffer);
}

ZIA::API::socket_t Session::getSocket() const
{
	return (this->_socket);
}
 
ZIA::API::uint32_t Session::getIp() const
{
	return (this->_ip);
}

ZIA::API::uint16_t Session::getPort() const
 {
	return (this->_port);
 }

std::size_t Session::getProcessedRequestCount() const
 {
	 return (this->_nbRequest);
 }

bool	Session::isSsl() const
{
	return this->_ssl;
}

void Session::setBuffer(ZIA::API::IBuffer * buffer)
{
	if (this->_buffer != 0)
	{
		Server::getInstance().manageBuf()->release(this->_buffer);
	}
	this->_buffer = buffer;
	if (this->_buffer != 0 && this->_buffer->getRawData() != 0)
		this->parse();
}

ZIA::API::IBuffer * Session::getBuffer()
{
	return (this->_buffer);
}

void Session::reset(socket_t socket, uint32_t ip, uint16_t port, bool isSsl)
{
	this->_socket = socket;
	this->_port = port;
	this->_nbRequest = 0;
	this->_ip = 0;
	this->_ssl = isSsl;
	this->setBuffer();
}

void Session::parse()
{
	if (this->_buffer != NULL)
	{
		if (this->_parser.parseString(this->_buffer->getRawData()))
			std::cout << "Parsing complete " << std::endl;
		else
			std::cout << "Parsing Failed " << std::endl;
		this->_cmdLine = this->_parser.getCmdLineData();
		this->_header = this->_parser.getHeaderData();
	
  /*  std::cout << "CMDLINE DATA: " << std::endl;
	std::cout << this->_cmdLine.type << std::endl;
	std::cout << this->_cmdLine.url << std::endl;
	std::cout << this->_cmdLine.version << std::endl;

	//Affichage des data header

	HttpParser::t_header_map::iterator  cur = this->_header.begin();
	HttpParser::t_header_map::iterator  end = this->_header.end();

	std::cout << std::endl << "HEADER DATA: " << std::endl;
	while (cur != end)
		{
			if ((*cur).second != "")
			std::cout << (*cur).first << ": " << (*cur).second << std::endl;
			++cur;
		}
*/
	this->sendGetFile();
	}
}

void Session::sendGetFile()
{
	std::ifstream page;
	int length = 0;
	int total;
	char * pageFinale;
	std::string path;
	std::string pageTmp;
	std::string ch;
	std::string header = "HTTP/1.1 200 OK\r\nDate: Tue, 28 Sep 2010 23:20:31 GMT\r\nServer: Apache\r\nAccept-Ranges: bytes\r\nConnection: close\r\nContent-Type:text/html\r\nContent-Length:";

	path = Server::getInstance().getWWW();
	if (this->_cmdLine.url == "/")
		this->_cmdLine.url = "/index.html";
	path.append(this->_cmdLine.url);
//	std::cout << std::endl << "PATH : " << path << std::endl;
	std::fstream file(path.c_str());
	if ( !file ) 
	{
		std::cout << "fichier inexistant";
		return;
	}
	else 
	{
		bool continuer = true;
		while(!file.eof()) 
		{	
			std::getline(file, ch);
			pageTmp += ch;
		}
	}
	//std::cout << pageTmp << std::endl;
	file.close();
	length = pageTmp.size();
	std::stringstream st;
	total = length +pageTmp.size() + 10;
	st << total;
	std::string num;
	st >> num;
	//std::cout << "Size : " << total << std::endl;
	header += num;
	header += "\r\n\r\n";
	header += pageTmp;
	length = pageTmp.size();
	pageFinale = new char[length];

	memcpy(pageFinale, header.c_str(), length);

	Server::getInstance().getNetwork()->write(this->_socket, pageFinale);
}
