#ifndef __INC_MIO_FILETYPE_HPP__
#define __INC_MIO_FILETYPE_HPP__

#include "mio_exception.hpp"
#include "mio_util.hpp"

namespace mio
{

	struct filetype
	{
		enum Enum
		{
			Unknown,
			STL,
			AF,
			Count
		};
		
		static Enum getFromFileName(const std::string & filename)
		{
			std::string ext = util::getFileExtension(filename);
			for (int e = Unknown + 1; e != Count; ++e)
				if (util::equalsNoCase_ASCII(ext, getExtension(static_cast<Enum>(e))))
					return static_cast<Enum>(e);
			return Unknown;
		}

		static std::string getExtension(Enum type)
		{
			switch (type)
			{
			case STL: return std::string("stl");
			case AF: return std::string("af");
			default:
				break;
			}
			return std::string();
		}
	};

}

#endif // __INC_MIO_FILETYPE_HPP__