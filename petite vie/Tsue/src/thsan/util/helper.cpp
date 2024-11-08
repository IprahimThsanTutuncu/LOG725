#include "pch.h"
#include "helper.h"

void readJson(const std::string& path, json* j)
{
	std::ifstream i(path);
	if (!i.fail())
		i >> *j;
	else
		std::cout << "Error: failed to load json file...\n";
}

void readJson(const std::string& path, nlohmann::ordered_json* j)
{
	std::ifstream i(path);
	i >> *j;
}

void writeJson(const std::string& path, const json& j)
{
	std::ofstream o(path);
	o << std::setw(4) << j << std::endl;
}

void writeJson(const std::string& path, const  nlohmann::ordered_json& j)
{
	std::ofstream o(path);
	o << std::setw(4) << j << std::endl;
}
