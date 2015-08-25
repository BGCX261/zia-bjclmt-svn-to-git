#pragma once

#include <iostream>
#include <string>
#include <list>
#include <fstream>

#include "tinyxml.h"

class Config
{

public:

	Config(void);
	~Config(void);

	bool RecupConfig(const std::string& file);
	const std::list<std::string>& getModule(void) const;
	const std::string& getDir(void) const;
	const std::string& getPort(void) const;
	const std::string& getSslState(void) const;
	const std::string& getSslPort(void) const;
	const std::string& getSslCertif(void) const;
	const std::string& getSslCertifKeyFile(void) const;

	void AffConfig(void);

private:

	bool WriteXml(const std::string& file);
	bool RecupXml(const std::string& file);
	
	std::list<std::string> _mod;
	std::string _dir;
	std::string _port;
	std::string _sslState;
	std::string _sslDir;
	std::string _sslPort;
	std::string _sslCertificate;
	std::string _sslCertifKeyFile;

};

