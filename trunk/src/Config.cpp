#include "Config.h"


Config::Config()
{
}

Config::~Config(void)
{
}

bool Config::RecupConfig(const std::string& file)
{
	if(RecupXml(file) == false)
	{
		if(WriteXml(file) == false)
			return false;
		if(RecupXml(file) == false)
			return false;
	}
	return true;
}

void Config::AffConfig(void) 
{
	std::cout << "Module(s) a charger" << std::endl;
	std::list<std::string>::iterator it = this->_mod.begin();
	for(;it != this->_mod.end(); ++it)
	{
		std::cout << "	-"<< *it << std::endl;
	}
	std::cout << "dir Web : " << this->_dir << std::endl;
	std::cout << "Port serveur : " << this->_port << std::endl;
	std::cout << "Ssl State : " << this->_sslState <<  std::endl;
	std::cout << "Ssl dir : " << this->_sslDir <<  std::endl;
	std::cout << "Ssl Port  : "<< this->_sslPort << std::endl;
	std::cout << "Ssl Certificate : "<< this->_sslCertificate << std::endl;
	std::cout << "Ssl CertifKeyFile : " << this->_sslCertifKeyFile << std::endl << std::endl;

}

bool Config::RecupXml(const std::string& file)
{
	TiXmlDocument doc(file.c_str());
	if(!doc.LoadFile())
	{
		std::cerr << "erreur lors du chargement" << std::endl;
		std::cerr << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
		return false;
	}
	TiXmlHandle hdl(&doc);
	//recuperation des modules
	TiXmlElement* elem = hdl.FirstChildElement().FirstChildElement().FirstChildElement().Element();
	if(!elem)
	{
		std::cerr << "le noeud à atteindre n'existe pas" << std::endl;
		return false;
	}
	while(elem)
	{
		this->_mod.push_back(elem->Attribute("name"));
		elem = elem->NextSiblingElement(); 
	}
	//recuperation du dir
	elem = hdl.FirstChildElement().FirstChildElement().Element();
	elem = elem->NextSiblingElement();
	if(!elem)
	{
		std::cerr << "le noeud à atteindre n'existe pas" << std::endl;
		return false;
	}
	this->_dir = elem->Attribute("name");
	//recuperation du port
	elem = elem->NextSiblingElement();
	if(!elem)
	{
		std::cerr << "le noeud à atteindre n'existe pas" << std::endl;
		return false;
	}
	this->_port = elem->Attribute("value");
	//SSL state
	elem = elem->NextSiblingElement();
	if(!elem)
	{
		std::cerr << "le noeud à atteindre n'existe pas" << std::endl;
		return false;
	}
	this->_sslState = elem->Attribute("value");
	//SSL dir
	elem = elem->NextSiblingElement();
	if(!elem)
	{
		std::cerr << "le noeud à atteindre n'existe pas" << std::endl;
		return false;
	}
	this->_sslDir = elem->Attribute("value");
	//SSL port
	elem = elem->NextSiblingElement();
	if(!elem)
	{
		std::cerr << "le noeud à atteindre n'existe pas" << std::endl;
		return false;
	}
	this->_sslPort = elem->Attribute("value");
	//SSL certificatefile
	elem = elem->NextSiblingElement();
	if(!elem)
	{
		std::cerr << "le noeud à atteindre n'existe pas" << std::endl;
		return false;
	}
	this->_sslCertificate = elem->Attribute("value");
	//SSL certificatekeyfile
	elem = elem->NextSiblingElement();
	if(!elem)
	{
		std::cerr << "le noeud à atteindre n'existe pas" << std::endl;
		return false;
	}
	this->_sslCertifKeyFile = elem->Attribute("value");
	return true;
}

bool Config::WriteXml(const std::string& file)
{
	std::ofstream xml(file.c_str(), std::ios::out | std::ios::trunc);
	if(!xml | xml.bad())
		return false;
	xml.clear();
	xml << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << std::endl;
	xml << "<configServer>" << std::endl;
	xml << "	<loadModule>" << std::endl;
	xml << "		<module name=\"ssl\"/>" << std::endl;
	xml << "		<module name=\"module2\"/>" << std::endl;
	xml << "	</loadModule>" << std::endl;
	xml << "	<dir name=\"www\"/>" << std::endl;
	xml << "	<port value=\"8080\" />" << std::endl;
	xml << "	<sslState value=\"on\" />" << std::endl;
	xml << "	<sslDir value=\"sslwww\" />" << std::endl;
	xml << "	<sslPort value=\"4430\" />" << std::endl;
	xml << "	<sslCertificatefile value=\"path\" />" << std::endl;
	xml << "	<sslCertificatekeyfile value=\"path\" />" << std::endl;
	xml << "</configServer>" << std::endl;
	xml.close();
	return (true);
}

const std::list<std::string>& Config::getModule(void) const
{
	return this->_mod;
}
const std::string& Config::getDir(void) const
{
	return this->_dir;
}
const std::string& Config::getPort(void) const
{
	return this->_port;
}
const std::string& Config::getSslState(void) const
{
	return this->_sslState;
}
const std::string& Config::getSslPort(void) const
{
	return this->_sslPort;
}
const std::string& Config::getSslCertif(void) const
{
	return this->_sslCertificate;
}
const std::string& Config::getSslCertifKeyFile(void) const
{
	return this->_sslCertifKeyFile;
}