#include "utils.hpp"

void db_red(std::string _msg)
{
	if (LOG > 1)
		std::cout << RED << _msg << RESET << std::endl;
}

void db_green(std::string _msg)
{
	if (LOG > 2)
		std::cout << GREEN << _msg << RESET << std::endl;
}

void db_yellow(std::string _msg)
{
	if (LOG > 3)
		std::cout << YELLOW << _msg << RESET << std::endl;
}

void db_blue(std::string _msg)
{
	if (LOG > 4)
		std::cout << BLUE << _msg << RESET << std::endl;
}