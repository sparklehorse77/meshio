#ifndef __INC_MIO_READER_STL_BINARY_HPP__
#define __INC_MIO_READER_STL_BINARY_HPP__

#include "mio_mesh.hpp"
#include "mio_exception.hpp"
#include "mio_endian.hpp"

#include <cstdint>
#include <fstream>
#include <cstring>
#include <string>

namespace mio
{

	class reader_stl_binary
	{
	public:
		static void load(const std::string & filename, mesh & m);
		static void load(std::istream & in, mesh & m);

	private:
		bool good() const;
		void read(std::size_t nbyte);
		void readHeader();
		void readFacets();
		void read();

		template<typename TVALUE>
		void bLoad(const std::size_t pos, TVALUE & value) const;
		unsigned uint32At(const std::size_t i) const;
		double real32At(const std::size_t i) const;

		reader_stl_binary(std::istream & in, mesh & _m) : is(in), m(_m), numFaces(0) { m.clear(); }
		std::istream & is;
		mesh & m;
		std::size_t numFaces;

		char buffer[84];
	};

	inline void reader_stl_binary::load(const std::string & filename, mesh & m)
	{
		std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);
		if (!in.is_open()) throw exception("reader_stl_binary::load: could not open input file");
		load(in, m);
	}

	inline bool reader_stl_binary::good() const
	{
		return (is.good());
	}
	
	inline void reader_stl_binary::read(std::size_t nbyte)
	{
		is.read(buffer, nbyte);
 		if (!is) throw exception("reader_stl_binary::read: fail on read");
	}

	template<typename TVALUE>
	inline void reader_stl_binary::bLoad(const std::size_t pos, TVALUE & value) const
	{
		TVALUE v;
		memcpy(&v, &buffer[pos], sizeof(TVALUE));
		value = endian::le_to_h(v);
	}

	inline unsigned reader_stl_binary::uint32At(const std::size_t i) const
	{
		std::uint32_t value = 0;
		bLoad(i, value);
		return static_cast<unsigned>(value);
	}

	inline double reader_stl_binary::real32At(const std::size_t i) const
	{
		float value = 0;
		bLoad(i, value);
		return static_cast<double>(value);
	}

	inline void reader_stl_binary::readHeader()
	{
		read(84);
		numFaces = uint32At(80);
	}
	
	inline void reader_stl_binary::readFacets()
	{
		std::size_t j;
		auto readVert = [&, this](vec3 & v) 
		{
			v.x = this->real32At(j); j += 4;
			v.y = this->real32At(j); j += 4;
			v.z = this->real32At(j); j += 4;
		};

		idx3 idx;
		vec3 vec;
		for (std::size_t f = 0; f < numFaces; ++f)
		{
			read(50);

			idx.a = m.points.size();
			idx.b = idx.a + 1;
			idx.c = idx.b + 1;

			j = 0;
			readVert(vec);
			m.normals.push_back(vec);

			for (unsigned k = 0; k < 3; ++k)
			{
				readVert(vec);
				m.points.push_back(vec);
			}

			m.indices.push_back(idx);
		}
	}

	inline void reader_stl_binary::read()
	{
		readHeader();
		readFacets();
	}
	
	inline void reader_stl_binary::load(std::istream & in, mesh & m)
	{
		reader_stl_binary(in, m).read();
	}

}

#endif // __INC_MIO_READER_STL_BINARY_HPP__