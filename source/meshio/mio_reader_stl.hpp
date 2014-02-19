#ifndef __INC_MIO_READER_STL_HPP__
#define __INC_MIO_READER_STL_HPP__

#include "mio_mesh.hpp"
#include "mio_exception.hpp"
#include "mio_reader_stl_ascii.hpp"
#include "mio_reader_stl_binary.hpp"

namespace mio
{

	class reader_stl
	{
	public:
		static void load(const std::string & filename, mesh & m);
		static void load(std::istream & in, mesh & m);

	private:
		reader_stl();
	};

	inline void reader_stl::load(const std::string & filename, mesh & m)
	{
		std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);
		if (!in.is_open()) throw exception("reader_stl::load: could not open input file");
		load(in, m);
	}
	
	inline void reader_stl::load(std::istream & in, mesh & m)
	{
		if (reader_stl_ascii::isSTL(in))
		{
			reader_stl_ascii::load(in,m);
		}
		else
		{
			reader_stl_binary::load(in,m);
		}
	}

}

#endif // __INC_MIO_READER_STL_HPP__