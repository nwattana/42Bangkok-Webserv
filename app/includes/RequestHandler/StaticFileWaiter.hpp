#pragma once
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "utils.hpp"
#include "exception.hpp"


// check file extension
// check file is accesible
// check file magin [optional]
// read file and return a string body
class StaticFileWaiter
{
private:
	std::string _path_to_file;
	std::string _file_extension;

	int extenion_len;
	int is_path_set;

public:
	StaticFileWaiter();
	StaticFileWaiter(std::string path_to_file);
	StaticFileWaiter(std::string path_to_file, std::string file_extension);

	short check_file_extension(std::string file_extension);
	short check_file_accesible();
	short check_file(std::string file_path);

	std::string generate_content_type(std::string file_extension);
	std::string get_content_type(void);
	std::string read_file(void);
};
