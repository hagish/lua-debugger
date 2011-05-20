/*
 * The MIT License - see LICENSE file for details
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_


#include <string>

class StringUtils
{
public:
//	template<typename T>
	static unsigned int toUInt(const std::string &s);

	static std::string toUpper(const std::string &s);

	static bool regexSearch(const std::string &regex, unsigned int braketNr, const std::string &text, std::string &matched);
};

#endif /* STRINGUTILS_H_ */
