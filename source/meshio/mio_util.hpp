#ifndef __INC_MIO_UTIL_HPP__
#define __INC_MIO_UTIL_HPP__

#include <cctype>
#include <string>

namespace mio
{

	struct util
	{
		static std::string getFileExtension(const std::string & filename);
		static bool nextWordEquals_ASCII(const std::string & token, std::istream & in, bool skipBlanks = true);
		static int compareNoCase_ASCII(const std::string & str0, const std::string & str1);
		static bool equalsNoCase_ASCII(const std::string & str0, const std::string & str1);
	};


	inline std::string util::getFileExtension(const std::string & filename)
	{
		std::string rval;
		std::size_t i = filename.find_last_of('.');
		for (std::size_t j = i + 1; j < filename.size(); ++j) rval.push_back(filename[j]);
		return rval;
	}
	
	inline bool util::nextWordEquals_ASCII(const std::string & token, std::istream & in, bool skipBlanks)
	{
		std::string buffer;
		std::streampos p = in.tellg();

		while (true)
		{
			char c = in.get();
			if (!in.good() || in.eof()) break;
			if (skipBlanks && std::isblank(c))
			{
				if (buffer.empty()) continue;
				else break;
			}
			if (!std::isalpha(c)) break;
			buffer.push_back(c);
		}

		in.seekg(p - in.tellg(), std::ios_base::cur);
		return equalsNoCase_ASCII(token, buffer);
	}

	inline int util::compareNoCase_ASCII(const std::string & str0, const std::string & str1)
	{
		int rval = 0;

		auto s0 = str0.begin();
		const auto s0_end = str0.end();

		auto s1 = str1.begin();
		const auto s1_end = str1.end();

		while ((s0 != s0_end) && (s1 != s1_end) && (rval == 0))
		{
			rval = std::tolower(*s0) - std::tolower(*s1);
			++s0;
			++s1;
		}

		if (rval == 0) rval = static_cast<int>(str0.size()) - static_cast<int>(str1.size());
		return (rval);
	}
	
	inline bool util::equalsNoCase_ASCII(const std::string & str0, const std::string & str1)
	{
		return (compareNoCase_ASCII(str0, str1) == 0);
	}
}


#endif // __INC_MIO_UTIL_HPP__