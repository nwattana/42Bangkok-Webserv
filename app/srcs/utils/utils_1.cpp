#include "utils.hpp"

void	exitWithError(std::string errorMessage, int mode, int errnum)
{
	if (mode == 0)
		errorMessage = strerror(errnum);
	std::cerr << errorMessage << std::endl;
	exit(errnum);
}

/// @brief string tok
std::vector<std::string> ft_split(std::string str, const char * delimiter_set)
{
	std::vector<std::string> result;
	char *str_c = new char[str.length() + 1];
	strcpy(str_c, str.c_str());
	char *token = strtok(str_c, delimiter_set);
	while (token != NULL)
	{
		result.push_back(token);
		token = strtok(NULL, delimiter_set);
	}
	delete[] str_c;
	return result;
}

// Should Delete result
char **ft_vector_string_to_char_array(std::vector<std::string> vec)
{
	char **result = new char*[vec.size() + 1];
	for (size_t i = 0; i < vec.size(); i++)
	{
		result[i] = strdup(vec[i].c_str());
	}
	result[vec.size()] = NULL;
	return result;
}

std::string ft_strtrim(std::string str)
{
	size_t start = 0;
	size_t end = str.length() - 1;
	while (isspace(str[start]))
		start++;
	while (isspace(str[end]))
		end--;
	return str.substr(start, end - start + 1);
}

std::string ft_strtrim_char_set(std::string str, std::string char_set)
{
	size_t start = 0;
	size_t end = str.length() - 1;
	while (char_set.find(str[start]) != std::string::npos)
		start++;
	while (char_set.find(str[end]) != std::string::npos)
		end--;
	return str.substr(start, end - start + 1);
}


void printStringVector(std::vector<std::string> vec, std::string delimiter)
{
	if (vec.empty())
	{
		std::cout << "[Empty vector]" << std::endl;
		return;
	}
	for (size_t i = 0; i < vec.size(); i++)
	{
		std::cout << vec[i] << delimiter;
	}
	std::cout << std::endl;
}

int findCharInString(std::string str, const char *c)
{
	if (c == NULL)
		throw std::invalid_argument("Null pointer");
	if (str.empty())
		return 0;
	while (*c)
	{
		if (str.find(*c) != std::string::npos)
			return 1;
		c++;
	}
	return 0;
}

void printLog(std::string message)
{
	if (LOG > 5)
		std::cout << CYAN << "========= " << message << " =========\n";
}


void free_char_array(char **arr)
{
	for (size_t i = 0; arr[i] != NULL; i++)
	{
		free(arr[i]);
	}
	delete[] arr;
}

std::pair<std::string, std::string> ft_split_pair(std::string str, const std::string delimiter)
{
	std::string key, value;
	size_t pos = str.find(delimiter);
	if (pos == std::string::npos)
		throw DelimiterNotFoundException("Delimiter not found \"" + std::string(delimiter)+ "\"");
	key = str.substr(0, pos);
	value = str.substr(pos + delimiter.length());
	return std::make_pair(key, value);
}