#ifndef _WIN32

# include <dirent.h>
# include <dlfcn.h>
# include <iostream>
# include <string>
# include "UnixLibLoader.h"

UnixLibLoader::UnixLibLoader(void)
{
}

UnixLibLoader::~UnixLibLoader(void)
{
}

void    UnixLibLoader::LoadOne(std::string path)
{
	libhandle handleSo;
	size_t pos = path.find_last_of("/");

	if (pos != std::string::npos)
	{
	std::string file = path.substr(pos + 1, path.size());
		if ((handleSo = dlopen(path.c_str(), RTLD_LAZY)))
			this->loaded[file] = handleSo;
	}
}

void    UnixLibLoader::LoadDir(std::string path)
{
	DIR* rep = NULL;
	rep = opendir(path.c_str());
	
	if (rep != NULL)
	{
		struct dirent* tree = NULL;

		while (tree = readdir(rep))
		{
			std::string ext = tree->d_name;
			size_t      pos = ext.find_last_of(".");

			if (pos != std::string::npos && (ext = ext.substr(pos + 1, ext.size())).compare("so") == 0)
				LoadOne(path + tree->d_name);
		}
		closedir(rep);
	}
}

void    UnixLibLoader::FreeOne(std::string file)
{
	int nb = this->loaded.count(file);

	if (nb > 0)
	{
		dlclose(this->loaded[file]);
		this->loaded.erase(file);
	}
}

void    UnixLibLoader::FreeAll()
{
	std::map<std::string, libhandle>::iterator    begin = this->loaded.begin();
	std::map<std::string, libhandle>::iterator    end = this->loaded.end();

	while (begin != end)
	{
		FreeOne((*begin).first);
		begin = this->loaded.begin();
	}
}

ILibLoader::libhandle UnixLibLoader::getLib(std::string file)
{
	return (this->loaded[file]);
}

const std::list<std::string>  UnixLibLoader::getLibList() const
{
	std::list<std::string>  libList;
	std::map<std::string, libhandle>::const_iterator    cur = this->loaded.begin();
	std::map<std::string, libhandle>::const_iterator    end = this->loaded.end();

	for (; cur != end; ++cur)
		libList.push_back((*cur).first);
	return (libList);
}

#endif
