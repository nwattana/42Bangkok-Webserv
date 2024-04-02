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
#include "exception.hpp"


// Convert int to string dec
inline std::string SSTR(int x) {
	std::ostringstream oss;
	oss << std::dec << x;
	return oss.str();
}

inline std::string toupper_string(std::string str) {
	std::string result = "";
	for (size_t i = 0; i < str.length(); i++) {
		result += toupper(str[i]);
	}
	return result;
}

#ifndef LOG
# define LOG 0
#endif

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

void printLog(std::string message);

std::string read_file(std::string file_path);
std::string resolve_double_slash(std::string path);
char **ft_vector_string_to_char_array(std::vector<std::string> vec);
void free_char_array(char **arr);
std::string ft_trimchar_set(std::string str, std::string char_set);

// delete me
void test_use_execute_cgi();


// debug helper
void db_red(std::string message);
void db_green(std::string message);
void db_yellow(std::string message);
void db_blue(std::string message);

std::pair<std::string, std::string> ft_split_pair(std::string str, const std::string delimiter);



// Parse error
std::string router_parser(std::string route);
std::string port_parser(std::string port);