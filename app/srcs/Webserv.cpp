/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkaiyawo <kkaiyawo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:36:56 by kkaiyawo          #+#    #+#             */
/*   Updated: 2024/03/11 14:55:43 by kkaiyawo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

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

void printStringVector(std::vector<std::string> vec, std::string delimiter)
{
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
