Prerequis:
	- avoir installer cmake 2.8.2
	- avoir checkout le trunk du svn
	
Comment faire pour creer/mettre a jour la solution Visual Studio 2010 avec Cmake:

cd build
cd vs2010
cmake ..\..\ -G "Visual Studio 10"

Si c'est la premiere fois,
Ouvrir CMakeCache.txt
et ajouter les lignes suivante avec les chemins qui correspondent a votre installation:

libs_name:STRING=WS2_32.lib
openssl_include_dir:FILEPATH=C:/openssl/include
openssl_lib_dir:FILEPATH=C:/openssl/lib


et paf ca fait des cho euh la solution est mise a jour.