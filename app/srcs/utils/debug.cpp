#include "utils.hpp"

void db_red(std::string _msg)
{
	std::cout << RED << _msg << RESET;
}

void db_green(std::string _msg)
{
	std::cout << GREEN << _msg << RESET;
}

void db_yellow(std::string _msg)
{
	std::cout << YELLOW << _msg << RESET;
}

void db_blue(std::string _msg)
{
	std::cout << BLUE << _msg << RESET;
}