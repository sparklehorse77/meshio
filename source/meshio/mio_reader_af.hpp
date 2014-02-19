#ifndef __INC_MIO_READER_AF_HPP__
#define __INC_MIO_READER_AF_HPP__

#include "mio_mesh.hpp"
#include "mio_exception.hpp"
#include "mio_util.hpp"
#include "mio_af_tokens.hpp"

#include <cassert>
#include <fstream>
#include <string>

namespace mio
{

	class reader_af
	{
	public:
		static void load(const std::string & str, mesh & m);
		static void load(std::istream & in, mesh & m);

	private:
		bool good() const;
		void read(vec3 & v);
		void read(idx3 & i);
		bool readWord();
		void readVertices(mesh::VectorCollection & v);
		void readPoints();
		void readNormals();
		void readFacets();
		void read();

	private: 
		reader_af(std::istream & _in, mesh & _m) : is(_in), m(_m) { m.clear(); }
		std::istream & is;
		mesh & m;
		std::string buffer;
	};

	inline void reader_af::load(const std::string & str, mesh & m)
	{
		std::ifstream is(str, std::ios_base::in | std::ios_base::binary);
		if (!is.is_open()) throw exception("reader_af::load: could not open input file");
		load(is, m);
	}
	
	inline void reader_af::load(std::istream & in, mesh & m)
	{
		reader_af(in, m).read();
	}

	inline bool reader_af::good() const
	{
		return (is.good() && !is.eof());
	}

	inline void reader_af::read(vec3 & v)
	{
		if (!readWord()) throw exception("reader_af::read(vec3&): file read failed");
		v.x = std::stod(buffer);

		if (!readWord()) throw exception("reader_af::read(vec3&): file read failed");
		v.y = std::stod(buffer);

		if (!readWord()) throw exception("reader_af::read(vec3&): file read failed");
		v.z = std::stod(buffer);
	}

	inline void reader_af::read(idx3 & i)
	{
		if (!readWord()) throw exception("reader_af::read(idx3&): file read failed");
		i.a = static_cast<std::size_t>(std::stoul(buffer));

		if (!readWord()) throw exception("reader_af::read(idx3&): file read failed");
		i.b = static_cast<std::size_t>(std::stoul(buffer));

		if (!readWord()) throw exception("reader_af::read(idx3&): file read failed");
		i.c = static_cast<std::size_t>(std::stoul(buffer));
	}

	inline bool reader_af::readWord()
	{
		buffer.clear();
		is >> buffer;
		return (!buffer.empty());
	}

	inline void reader_af::readPoints()
	{
		while (good() && !util::equalsNoCase_ASCII(buffer, af_tokens::points()))
		{
			readWord();
		}

		if (good())
		{
			while (true)
			{
				if (!readWord()) return;
				if (util::equalsNoCase_ASCII(buffer, af_tokens::normals()) ||
					util::equalsNoCase_ASCII(buffer, af_tokens::facets()))
					return;

				std::size_t vidx = static_cast<std::size_t>(std::stoul(buffer));
				assert(vidx == m.points.size());

				vec3 v;
				read(v);
				m.points.push_back(v);
			}
		}
	}

	inline void reader_af::readNormals()
	{
		if (good() && util::equalsNoCase_ASCII(buffer, af_tokens::normals()))
		{
			while (good())
			{
				if (!readWord()) return;
				if (util::equalsNoCase_ASCII(buffer, af_tokens::facets())) return;

				std::size_t vidx = static_cast<std::size_t>(std::stoul(buffer));
				assert(vidx == m.normals.size());

				vec3 v;
				read(v);
				m.normals.push_back(v);
			}
		}
	}

	inline void reader_af::readFacets()
	{
		if (good() && util::equalsNoCase_ASCII(buffer, af_tokens::facets()))
		{
			while (good())
			{
				if (!readWord()) return;
				std::size_t fidx = static_cast<std::size_t>(std::stoul(buffer));
				assert(fidx == m.indices.size());

				idx3 i;
				read(i);
				m.indices.push_back(i);
			}
		}
	}

	inline void reader_af::read()
	{
		readPoints();
		readNormals();
		readFacets();
	}
}

#endif // __INC_MIO_READER_AF_HPP__