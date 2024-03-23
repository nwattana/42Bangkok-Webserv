#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include "Constant.hpp"


// Convert int to string dec
inline std::string SSTR(int x) {
	std::ostringstream oss;
	oss << std::dec << x;
	return oss.str();
}

std::string getReasonPhrase(int statusCode);
std::string read_htlm_file(std::string file_path);
void						exitWithError(std::string errorMessage = "error", int mode = 0, int errnum = errno);
void 						printStringVector(std::vector<std::string> vec, std::string delimiter);
int 						findCharInString(std::string str, const char *c);
std::string 				ft_strtrim(std::string str);
std::vector<std::string> 	ft_split(std::string str, const char *delimiter);

std::string execute_cgi(int argc, char *argv[], char * const * envp);

int allow_method_parser(std::string allow_method);
int allow_method_str_to_int(std::string method);


std::string method_allow_to_string(int method);
std::string read_file(std::string file_path);
