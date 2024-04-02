#include "FileCreator.hpp"

FileCreator::FileCreator()
{
}

FileCreator::FileCreator(const std::string &fileName)
{
	_file_name = fileName;
}

FileCreator::~FileCreator()
{
}

int FileCreator::_createFile(const std::string &fileName, const std::string &content)
{
	try
	{
		std::ofstream file(fileName.c_str());
		file << content;
		file.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << "FileCreate Error: _createFile\n";
		std::cerr << e.what() << '\n';
		return (0);
	}
	return (1);
}

int FileCreator::_appendFile(const std::string &fileName, const std::string &content)
{
	try
	{
		std::ofstream file(fileName.c_str(), std::ios::app);
		file << content;
		file.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << "FileCreate Error: _appendFile\n";
		std::cerr << e.what() << '\n';
		return (0);
	}
	return (1);
}

// if file name is full path, set file name to file name only
void FileCreator::setFileName(const std::string &fileName)
{
	// find last '/' in fileName
	size_t pos = _file_name.find_last_of("/");
	if (pos != std::string::npos)
	{
		_file_name = _file_name.substr(pos + 1);
		_file_name = "./" + _file_name;
	}
	else
	{
		_file_name = fileName;
	}
}

void FileCreator::setContent(const std::string &content)
{
	_content = content;
}

void FileCreator::appendContent(const std::string &content)
{
	_content += content;
}

int FileCreator::check_file_exist(const std::string &fileName)
{
	std::ifstream file(fileName.c_str());
	if (file.is_open())
	{
		file.close();
		return 1;
	}
	return 0;
}

int FileCreator::createFile(int method)
{
	int res = 0;
	if (method == CREATE_FILE)
	{
		res = _createFile(_file_name, _content);
	}
	else if (method == APPEND_FILE)
	{
		res = _appendFile(_file_name, _content);
	}
	return res;
}

void test_file_creator(void)
{
	FileCreator file_creator("test.txt");
	file_creator.setContent("Hello World\n");
	file_creator.createFile(CREATE_FILE);
	file_creator.appendContent("Hello World\n");
	file_creator.createFile(APPEND_FILE);
}