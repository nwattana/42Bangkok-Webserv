#include <iostream>
#include <fstream>


/*
* set env root_dir then execute cgi
*/
int main(int ac, char *va[], char *env[])
{
	std::string line;
	std::string content;
	std::ifstream file("index.html");
	if (file.is_open())
	{
		while (getline(file, line))
		{
			content += line;
		}
		file.close();
	}
	else
	{
		std::cout << "Unable to open file";
	}
	std::cout << "Content-type: text/html\n\n";
	std::cout << content;
	return 0;
}