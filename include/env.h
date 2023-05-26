#ifndef _ENV_H
#define _ENV_H
#include <string>


class EnvSystem {
public:
	static void Init();
	static void Release();
	static std::string Get(std::string);
	static void Set(std::string, std::string);
};

#endif