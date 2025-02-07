#include "string_util.h"
#include <cctype>
#include <locale>
#include <pwd.h>
#include <stdlib.h>
#include <regex.h>

using namespace std;

namespace string_util {

	//! reference to the current standard library 'locale'
	static const std::locale& curLocale=std::locale::classic();
	
	char localeToUpper(char c) {
		return std::toupper(c,curLocale);
	}

	char localeToLower(char c) {
		return std::tolower(c,curLocale);
	}
	
	string makeUpper(const string& s) {
		string ans(s); // yes, I actually checked if it's faster to copy and then overwrite or reserve and use std::back_inserter(ans)
		std::transform(ans.begin(), ans.end(), ans.begin(), (int(*)(int)) std::toupper);
		return ans;
	}
	
	string makeLower(const string& s) {
		string ans(s);
		std::transform(ans.begin(), ans.end(), ans.begin(), (int(*)(int)) std::tolower);
		return ans;
	}

	string removePrefix(const string& str, const string& pre) {
		if(str.compare(0,pre.size(),pre)==0)
			return str.substr(pre.size());
		return string();
	}
	
	string tildeExpansion(const string& str) {
		string ans;
		if(str[0]!='~') {
			return str;
		}
#ifndef PLATFORM_APERIOS
		else if(str=="~" || str[1]=='/') {
			char* home=getenv("HOME");
			if(home==NULL)
				return str;
			if(str=="~")
				return home;
			return home+str.substr(1);
		} else {
			string::size_type p=str.find('/');
			struct passwd * pw;
			pw=getpwnam(str.substr(1,p-1).c_str());
			if(pw==NULL)
				return str;
			return pw->pw_dir+str.substr(p);
		}
#else
		return str.substr(1);
#endif
	}

	string trim(const string& str) {
		if(str.size()==0)
			return str;
		unsigned int b=0;
		unsigned int e=str.size()-1;
		while(b<str.size() && isspace(str[b]))
			b++;
		while(b<e && isspace(str[e]))
			e--;
		return str.substr(b,e-b+1);
	}
	
	bool parseArgs(const string& input, vector<string>& args, vector<unsigned int>& offsets) {
		string cur;
		bool isDoubleQuote=false;
		bool isSingleQuote=false;
		args.clear();
		offsets.clear();
		unsigned int begin=-1U;
		for(unsigned int i=0; i<input.size(); i++) {
			char c=input[i];
			if(begin==-1U && !isspace(c))
				begin=i;
			switch(c) {
			case ' ':
			case '\n':
			case '\r':
			case '\t':
			case '\v':
			case '\f':
				if(isSingleQuote || isDoubleQuote)
					cur+=c;
				else if(cur.size()!=0) {
					args.push_back(cur);
					offsets.push_back(begin);
					cur.clear();
					begin=-1U;
				}
				break;
			case '\\':
				if(i==input.size()-1) { //escaped line break
					return false;
				} else
					cur.push_back(input[++i]);
				break;
			case '"':
				if(isSingleQuote)
					cur.push_back(c);
				else
					isDoubleQuote=!isDoubleQuote;
				break;
			case '\'':
				if(isDoubleQuote)
					cur+=c;
				else
					isSingleQuote=!isSingleQuote;
				break;
			default:
				cur+=c;
				break;
			}
		}
		if(cur.size()>0) {
			args.push_back(cur);
			offsets.push_back(begin);
		}
		return !isDoubleQuote && !isSingleQuote;
	}

	bool reMatch(const std::string& str, const std::string& regex) {
		return reMatch(str,regex,REG_EXTENDED);
	}
	
	bool reMatch(const std::string& str, const std::string& regex, int flags) {
		regex_t re;
		if(int err=regcomp(&re,regex.c_str(),flags | REG_NOSUB)) {
			char msg[128];
			regerror(err,&re,msg,128);
			string errmsg;
			errmsg.append("Bad filter '").append(regex).append("': ").append(msg);
			regfree(&re);
			throw errmsg;
		}
		int match=regexec(&re,str.c_str(),0,NULL,0);
		regfree(&re);
		if(match==0) {
			return true;
		} else if(match==REG_NOMATCH) {
			return false;
		} else {
			char msg[128];
			regerror(match,&re,msg,128);
			string errmsg;
			errmsg.append("Regex error on reMatch('").append(str).append("', '").append(regex).append("'): ").append(msg);
			throw errmsg;
		}
	}

}

/*! @file
 * @brief Implements some useful functions for string manipulation in the string_util namespace
 * @author ejt (Creator)
 *
 * $Author: ejt $
 * $Name: tekkotsu-4_0-branch $
 * $Revision: 1.12 $
 * $State: Exp $
 * $Date: 2007/11/09 19:01:14 $
 */
