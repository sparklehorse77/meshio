#ifndef __INC_MIO_FILE_WRITER_HPP__
#define __INC_MIO_FILE_WRITER_HPP__

#include "mio_filetype.hpp"
#include "mio_writers.hpp"

namespace mio
{
	class file_writer
	{
	public:
		static void save(const std::string & filename, const mesh & m)
		{
			const filetype::Enum ft = filetype::getFromFileName(filename);
			switch (ft)
			{
			case filetype::AF: writer_af::save(filename, m); break;
			case filetype::STL: writer_stl_ascii::save(filename, m); break;
			default: 
				throw exception("file_writer::save: could not determine file type");
			}
		}
	private:
		file_writer();
	};
}

#endif // __INC_MIO_FILE_WRITER_HPP__