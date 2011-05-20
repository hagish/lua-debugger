/*
 * The MIT License - see LICENSE file for details
 */

#include "FileUtils.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string FileUtils::getContent(const std::string &file)
{
	std::ifstream f(file.c_str());

	if (f)
	{
		std::stringstream buffer;
		buffer << f.rdbuf();
		f.close();

		return buffer.str();
	}
	else
	{
		return std::string();
	}
}
