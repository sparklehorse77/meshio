#ifndef __INC_MIO_WRITER_STL_BINARY_HPP__
#define __INC_MIO_WRITER_STL_BINARY_HPP__

#include "mio_mesh.hpp"
#include "mio_exception.hpp"
#include "mio_endian.hpp"

#include <cstdint>
#include <fstream>
#include <cstring>
#include <string>

namespace mio
{

	class writer_stl_binary
	{
	public:
		static void save(const std::string & filename, const mesh & m);
		static void save(std::ostream & os, const mesh & m);
	private:
		void write(std::size_t nbyte);
		void writeHeader();
		void writeFacets();
		void write();

		template<typename TYPE>
		void bStore(const std::size_t pos, const TYPE v);
		void uint32At(const std::size_t pos, const std::size_t value);
		void real32At(const std::size_t pos, const double value);

		writer_stl_binary(std::ostream & _os, const mesh & _m) : os(_os), m(_m) {}
		std::ostream & os;
		const mesh & m;
		char buffer[84];
	};

	inline void writer_stl_binary::save(const std::string & filename, const mesh & m)
	{
		std::ofstream out(filename, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if (!out.is_open()) throw exception("writer_stl_binary::save: could not open output file");
		save(out, m);
	}

	inline void writer_stl_binary::save(std::ostream & os, const mesh & m)
	{
		writer_stl_binary(os, m).write();
	}
	
	inline void writer_stl_binary::write(std::size_t nbyte)
	{
		os.write(buffer, nbyte);
		if (!os)
			throw exception("writer_stl_binary::write: fail on write");
	}

	template<typename TYPE>
	inline void writer_stl_binary::bStore(const std::size_t pos, const TYPE v)
	{
		memcpy(&buffer[pos], &v, sizeof(v));
	}

	inline void writer_stl_binary::uint32At(const std::size_t pos, const std::size_t value)
	{
		std::uint32_t v = static_cast<std::uint32_t>(value);
		v = endian::h_to_le(v);
		bStore(pos, v);
	}

	inline void writer_stl_binary::real32At(const std::size_t pos, const double value)
	{
		float v = static_cast<float>(value);
		v = endian::h_to_le(v);
		bStore(pos, v);
	}

	inline void writer_stl_binary::writeHeader()
	{
		memset(buffer, 0, 84);
		uint32At(80, m.indices.size());
		write(84);
	}

	inline void writer_stl_binary::writeFacets()
	{
		std::size_t j;
		auto vertSet = [&, this](const vec3 & v)
		{
			this->real32At(j, v.x); j += 4;
			this->real32At(j, v.y); j += 4;
			this->real32At(j, v.z); j += 4;
		};

		memset(buffer, 0, 50);
		for (std::size_t f = 0; f < m.indices.size(); ++f)
		{
			j = 0;

			vec3 norm(0);
			if (m.normalsPerFacet()) norm = m.normals[f];
			vertSet(norm);

			const idx3 & idx = m.indices[f];
			vertSet(m.points[idx.a]);
			vertSet(m.points[idx.b]);
			vertSet(m.points[idx.c]);

			write(50);
		}
	}

	inline void writer_stl_binary::write()
	{
		writeHeader();
		writeFacets();
	}

}

#endif // __INC_MIO_WRITER_STL_BINARY_HPP__