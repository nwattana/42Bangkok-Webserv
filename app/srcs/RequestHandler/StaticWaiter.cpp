#include "StaticFileWaiter.hpp"

/// @brief Module to Halp to serve static files
StaticFileWaiter::StaticFileWaiter() : _path_to_file(""), _file_extension("")
{
	is_path_set = 0;
}

StaticFileWaiter::StaticFileWaiter(std::string path_to_file) : _path_to_file(path_to_file)
{
	is_path_set = 1;

	// get file extension
	extenion_len = 0;
	for (int i = _path_to_file.length() - 1; i >= 0; i--)
	{
		if (_path_to_file[i] == '.')
		{
			_file_extension = _path_to_file.substr(i);
			extenion_len = _file_extension.length();
			break;
		}
	}
}

StaticFileWaiter::StaticFileWaiter(std::string path_to_file, std::string file_extension) : _path_to_file(path_to_file), _file_extension(file_extension)
{
	is_path_set = 1;
	extenion_len = _file_extension.length();
}

short StaticFileWaiter::check_file_extension(std::string file_path)
{
	if (is_path_set == 0)
	{
		throw FileNotAccessibleException("Path to file not set");
	}
	if (file_path.length() < extenion_len)
	{
		throw FileNotAccessibleException("File extension not found");
	}
	if (file_path.substr(file_path.length() - extenion_len) != _file_extension)
	{
		throw FileNotAccessibleException("File extension not found");
	}
	return 1;
}

short StaticFileWaiter::check_file_accesible()
{
	if (is_path_set == 0)
	{
		throw FileNotAccessibleException("Path to file not set");
	}
	return access(_path_to_file.c_str(), F_OK) == 0 ? 1 : 0;
}

// Merge all build errors into one
short StaticFileWaiter::check_file(std::string file_path)
{
	if (is_path_set == 0)
	{
		throw FileNotAccessibleException("Path to file not set");
	}
	if (check_file_extension(file_path) == 0)
	{
		throw FileNotAccessibleException("File extension not found");
	}
	if (check_file_accesible() == 0)
	{
		throw FileNotAccessibleException("File not found");
	}

	return 1;
}

std::string StaticFileWaiter::generate_content_type(std::string file_extension)
{
	if (file_extension == ".html")
		return "text/html";
	if (file_extension == ".css")
		return "text/css";
	if (file_extension == ".jpg")
		return "image/jpeg";
	if (file_extension == ".jpeg")
		return "image/jpeg";
	if (file_extension == ".png")
		return "image/png";
	if (file_extension == ".gif")
		return "image/gif";
	if (file_extension == ".svg")
		return "image/svg+xml";
	return "text/plain";
}

std::string StaticFileWaiter::get_content_type(void)
{
	if (_file_extension == ".html")
		return "text/html";
	if (_file_extension == ".css")
		return "text/css";
	if (_file_extension == ".jpg")
		return "image/jpeg";
	if (_file_extension == ".jpeg")
		return "image/jpeg";
	if (_file_extension == ".png")
		return "image/png";
	if (_file_extension == ".gif")
		return "image/gif";
	if (_file_extension == ".svg")
		return "image/svg+xml";
	return "text/plain";
}

std::string StaticFileWaiter::read_file(void)
{
	if (is_path_set == 0)
	{
		throw FileNotAccessibleException("Path to file not set");
	}
	if (check_file_accesible() == 0)
	{
		throw FileNotAccessibleException("File not found");
	}
	std::ifstream file(_path_to_file.c_str());
	std::string res;

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			res += line + "\n";
		}
		file.close();
	}
	else
	{
		throw FileNotAccessibleException("File not found");
	}
	return res;
}