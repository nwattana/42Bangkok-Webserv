#include "utils.hpp"

std::string read_file(std::string file_path)
{
	std::cout << "Reading file: " << file_path << std::endl;
	std::ifstream file(file_path.c_str());
	std::string str;
	std::string file_content;

	if (file.is_open())
	{
		while (std::getline(file, str))
		{
			file_content += str;
		}
		file.close();
	}
	return file_content;
}