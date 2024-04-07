#pragma once
#include <iostream>
#include <fstream>
#include "Constant.hpp"
// RED
#include <stdlib.h>

#define CREATE_FILE 0
#define APPEND_FILE 1

class FileCreator {
private:
	std::string _file_name;
	std::string _content;
	int _createFile(const std::string& fileName, const std::string& content);
	int _appendFile(const std::string& fileName, const std::string& content);

public:
	FileCreator();
	FileCreator(const std::string& fileName);
	~FileCreator();

	// int createFile(void);
	int createFile(int method);

	std::string setFileName(const std::string& fileName);
	void setContent(const std::string& content);
	void appendContent(const std::string& content);
	int check_file_exist(const std::string& fileName);
	void append_path(std::string path);

	std::string getFileName(void) const;
	void clean(void);
};

void test_file_creator(void);