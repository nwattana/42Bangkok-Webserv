#pragma once
#include <string>
#include <cstdio>
#include "utils.hpp"
#include <vector>
#include <map>
#include "Constant.hpp"

class ChunkData
{
public:
	ChunkData();
	ChunkData(std::string header_part);
	ChunkData(std::string data, std::string content_diposition, std::string content_type, std::string boundary);
	ChunkData(ChunkData const &src);
	~ChunkData();
	ChunkData &operator=(ChunkData const &src);
	std::string data;
	std::string content_disposition;
	std::string content_type;
	std::string boundary;

	int append_data(std::string data);
	std::string get_data(void) const;
	std::map<std::string, std::string> get_h_map(void);

	int is_empty;
	int add_header(std::string h_line);

private:
	std::map<std::string, std::string> h_map;
};
