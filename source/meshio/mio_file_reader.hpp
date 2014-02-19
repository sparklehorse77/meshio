#ifndef __INC_MIO_FILE_READER_HPP__
#define __INC_MIO_FILE_READER_HPP__

#include "mio_filetype.hpp"
#include "mio_readers.hpp"

namespace mio
{
	class file_reader
	{
	public:
		static void load(const std::string & filename, mesh & m)
		{
			const filetype::Enum ft = filetype::getFromFileName(filename);
			switch (ft)
			{
			case filetype::AF: reader_af::load(filename, m); break;
			case filetype::STL: reader_stl::load(filename, m); break;
			default: 
				throw exception("file_reader::load: could not determine file type");
			}
		}
	private:
		file_reader();
	};
}

#endif // __INC_MIO_FILE_READER_HPP__