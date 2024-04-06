#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

class BuildInCGI
{
public:
	BuildInCGI();
	BuildInCGI(char *envp);

private:
	std::string request;
};