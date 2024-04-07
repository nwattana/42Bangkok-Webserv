#include "ChunkData.hpp"

ChunkData::ChunkData()
{
	data = "";
	content_type ="";
	content_disposition="";
	is_empty = 1;
}

ChunkData::ChunkData(std::string header_part)
{
	data = "";
	std::vector<std::string> token;
	std::string clone_header=header_part;
	std::vector<std::string>::iterator it;
	token = ft_split(header_part, "\n");
	int pos, pos2;
	std::string h_token;

	pos = clone_header.find("\r\n");
	while (pos != -1)
	{
		h_token = clone_header.substr(0, pos);
		clone_header = clone_header.substr(pos + 2);
		pos2 = h_token.find(": ");
		if (pos2 != -1)
		{
			std::string key = h_token.substr(0, pos2);
			std::string value = h_token.substr(pos2 + 2);
			h_map[key] = value;
		}
		pos = clone_header.find("\r\n");
	}
	is_empty = 0;
}


ChunkData::ChunkData(std::string data, std::string content_disposition, std::string content_type, std::string boundary){
	data=data;
	content_disposition=content_disposition;
	content_type=content_type;
	boundary=boundary;
	is_empty = 0;
}


ChunkData::ChunkData(ChunkData const &src)
{
	std::cout << YELLOW << "CHUNKDATA COPY CONSTRUCTOR CALL " << RESET << std::endl;
	if (this != &src)
	{
		this->data = src.data;
		this->content_disposition=src.content_disposition;
		this->content_type=src.content_type;
		this->boundary=src.boundary;
		this->is_empty=src.is_empty;
		std::map<std::string , std::string > temp = src.get_h_map();
		std::map<std::string , std::string >::const_iterator m_it=temp.begin();
		for (m_it = temp.begin(); m_it != temp.end(); m_it++)
		{
			this->set_h_map((*m_it).first, (*m_it).second);
		}
	}
}


ChunkData& ChunkData::operator=(ChunkData const &src)
{
	*this = src;
	return *this;
}

ChunkData::~ChunkData()
{
	// nothing
}

int ChunkData::append_data(std::string data)
{
	try
	{
		this->data += data;
		this->is_empty= 0;
	}
	catch (std::exception &e)
	{
		std::cerr << RED << e.what() << RESET << std::endl;
		return (FAILURE);
	}
	return (SUCCESS);
}

std::string ChunkData::get_data(void) const
{
	return this->data;
}

std::map<std::string, std::string> ChunkData::get_h_map(void) const
{
	return this->h_map;
}

int ChunkData::add_header(std::string h_line)
{
	std::string key;
	std::string value;
	int pos;

	pos = h_line.find(":");
	if (pos == -1)
		return (FAILURE);
	key = h_line.substr(0, pos);
	value= h_line.substr(pos+1);
	h_map[key]=value;
	return (SUCCESS);
}

int ChunkData::update_content(void)
{
	std::map<std::string , std::string >::iterator m_it=h_map.begin();
	std::vector<std::string> temp_v;
	std::string temp_s;
	for (m_it = h_map.begin(); m_it != h_map.end(); m_it++)
	{
		std::cout << GREEN << (*m_it).first << BLUE << (*m_it).second << RESET << std::endl;
		if ((*m_it).first == "Content-Disposition")
		{
			temp_s = ft_strtrim_char_set((*m_it).second, "\r\n");
			temp_v = ft_split(temp_s, ";");
			if (temp_v.size() != 3)
			{
				return (-1);
			}
			content_disposition = temp_v[0];
			name = ft_strtrim_char_set(ft_split(temp_v[1], "=")[1], "\"");
			file_name = ft_strtrim_char_set(ft_split(temp_v[2], "=")[1], "\"");
		}
		if ((*m_it).first == "Content-Type")
		{
			content_type = (*m_it).second;
		}
	}
	return (1);
}

void ChunkData::set_h_map(std::string key, std::string value)
{
	h_map[key] = value;
}