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
		file << content.c_str();
		file.close();
	}
	catch (const std::exception &e)
	{
		std::cerr << "FileCreate Error: _createFile\n";
		std::cerr << e.what() << '\n';
		return (500);
	}
	return (201);
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
		return (500);
	}
	return (201);
}

// if file name is full path, set file name to file name only
std::string FileCreator::setFileName(const std::string &fileName)
{
	// find last '/' in fileName
	size_t pos = _file_name.find_last_of("/");
	if (pos != std::string::npos)
	{
		_file_name = _file_name.substr(pos + 1);
		_file_name = _file_name + fileName;
	}
	else
	{
		_file_name = fileName;
	}
	return (_file_name);
}

void FileCreator::setContent(const std::string &content)
{
	_content = content;
	// clean \r\n from content
	_content.erase(_content.length() - 2, 2);
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
	std::cout << BLUE << "Create File : " << YELLOW << _file_name << RESET << std::endl;
	if (method == CREATE_FILE)
	{
		// res = allow_type();
		if (this->check_file_exist(_file_name))
		{
			return (409);
		}
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

std::string FileCreator::getFileName(void) const
{
	return _file_name;
}

void FileCreator::append_path(std::string path)
{
	_file_name = path + _file_name;
}

void FileCreator::clean(void)
{
	_file_name = "";
	_content = "";
}

int FileCreator::allow_type(std::string type)
{
	int res;
	type = ft_strtrim_char_set(type, "\r\n ");
	if (type != "text/plain")
		return 400;
	return 201;
}