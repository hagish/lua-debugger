/*
 * The MIT License - see LICENSE file for details
 */

#include "StringUtils.h"

#include <boost/regex.hpp>

#include <sstream>
#include <algorithm>

//template<typename T>
unsigned int StringUtils::toUInt(const std::string &s)
{
	std::istringstream is(s);
	unsigned int r;
	is >> r;
	return r;
}

struct upper
{
	int operator()(int c)
	{
		return std::toupper((unsigned char) c);
	}
};

std::string StringUtils::toUpper(const std::string &s)
{
	std::string r(s);
	std::transform(r.begin(), r.end(), r.begin(), upper());
	return r;
}

bool StringUtils::regexSearch(const std::string &regex, unsigned int braketNr,
		const std::string &text, std::string &matched)
{
	boost::regex pattern(regex);
	boost::match_results<std::string::const_iterator> what;
	if (boost::regex_search(text, what, pattern) && what[braketNr].matched)
	{
		matched = what[braketNr].str();
		return true;
	}
	else
	{
		return false;
	}
}

