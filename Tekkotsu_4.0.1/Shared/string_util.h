//-*-c++-*-
#ifndef INCLUDED_string_util_h
#define INCLUDED_string_util_h

#include "attributes.h"
#include <string>
#include <vector>

//! some common string processing functions, for std::string
namespace string_util {
	//! uses the standard library's "locale" to convert case of a single character
	char localeToUpper(char c);
	
	//! uses the standard library's "locale" to convert case of a single character
	char localeToLower(char c);
	
	//! returns lower case version of @a s
	std::string makeLower(const std::string& s) ATTR_must_check;

	//! returns upper case version of @a s
	std::string makeUpper(const std::string& s) ATTR_must_check;

	//! returns @a str with @a pre removed - if @a pre is not fully matched, @a str is returned unchanged
	std::string removePrefix(const std::string& str, const std::string& pre) ATTR_must_check;
	
	//! removes whitespace (as defined by isspace()) from the beginning and end of @a str, and returns the result
	std::string trim(const std::string& str) ATTR_must_check;
	
	//! parses the input string into an arg list, with corresponding offsets of each arg in the original input
	bool parseArgs(const std::string& input, std::vector<std::string>& args, std::vector<unsigned int>& offsets); 

	//! replaces ~USER prefix with specified user's home directory, or ~ prefix with current HOME environment setting; returns str if no valid expansion is found
	std::string tildeExpansion(const std::string& str) ATTR_must_check;
	
	//! returns true if @a str matches @a re (assumes 'extended' regular expression, not 'basic'), false otherwise and throws std::string message on error
	/*! @param str The string to match
	 *  @param regex The (extended) regular expression which should be parsed and executed
	 *
	 *  This compiles the @a regex and then executes it... for repeated usage of the same
	 *  regular expression, you could be better off compiling it yourself and using the regex library directly. */
	bool reMatch(const std::string& str, const std::string& regex);

	//! returns true if @a str matches @a re (with optional @a flags to control interpretation), false otherwise and throws std::string message on error
	/*! @param str The string to match
	 *  @param regex The regular expression which should be parsed and executed
	 *  @param flags pass flags for regex (e.g. REG_EXTENDED)
	 *
	 *  This compiles the @a regex and then executes it... for repeated usage of the same
	 *  regular expression, you could be better off compiling it yourself and using the regex library directly. */
	bool reMatch(const std::string& str, const std::string& regex, int flags);

};

/*! @file
 * @brief Describes some useful functions for string manipulation in the string_util namespace
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.10 $
 * $State: Exp $
 * $Date: 2007/11/09 19:01:14 $
 */

#endif
