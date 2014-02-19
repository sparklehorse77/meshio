#ifndef __INC_MIO_READER_STL_ASCII_HPP__
#define __INC_MIO_READER_STL_ASCII_HPP__

#include "mio_mesh.hpp"
#include "mio_util.hpp"
#include "mio_stl_tokens.hpp"

#include <cassert>
#include <fstream>
#include <string>

namespace mio
{

	class reader_stl_ascii
	{
	public:
		static bool isSTL(std::istream & in);
		static void load(const std::string & filename, mesh & m);
		static void load(std::istream & in, mesh & m);

	private:
		bool good() const;

		bool checkWord(const std::string & token) const;
		void checkWordThrow(const std::string & token) const;
		void nextWord(const std::string & token);
		void nextWord();
		bool readWord();
		void read(vec3 & v);
		void readFacets();
		void readFacet();
		void readNormal();
		void readVerts();
		void read();

	private:
		reader_stl_ascii(std::istream & in, mesh & _m) : is(in), m(_m){ m.clear(); }
		std::istream & is;
		mesh & m;
		std::string buffer;
	};

	inline bool reader_stl_ascii::isSTL(std::istream & in)
	{
		return (util::nextWordEquals_ASCII(stl_tokens::solid_start(), in, true));
	}

	inline void reader_stl_ascii::load(const std::string & filename, mesh & m)
	{
		std::ifstream in(filename, std::ios_base::in | std::ios_base::binary);
		if (!in.is_open()) throw exception("reader_stl_ascii::load: could not open input file");
		load(in, m);
	}

	inline void reader_stl_ascii::load(std::istream & in, mesh & m)
	{
		reader_stl_ascii(in, m).read();
	}

	inline bool reader_stl_ascii::good() const
	{
		return (is.good() && !is.eof());
	}

	inline bool reader_stl_ascii::checkWord(const std::string & token) const
	{
		return util::equalsNoCase_ASCII(buffer, token);
	}

	inline void reader_stl_ascii::checkWordThrow(const std::string & token) const
	{
		if (!checkWord(token)) throw exception("reader_stl_ascii::checkWordThrow: parse error");
	}

	inline void reader_stl_ascii::nextWord(const std::string & token)
	{
		nextWord();
		checkWordThrow(token);
	}

	inline void reader_stl_ascii::nextWord()
	{
		if (!readWord()) throw exception("reader_stl_ascii::nextWord: unexpected end of stream");
	}

	inline bool reader_stl_ascii::readWord()
	{
		buffer.clear();
		is >> buffer;
		return (!buffer.empty());
	}

	inline void reader_stl_ascii::read(vec3 & v)
	{
		if (!readWord()) throw exception("reader_stl_ascii::read(vec3&): file read failed");
		v.x = std::stod(buffer);

		if (!readWord()) throw exception("reader_stl_ascii::read(vec3&): file read failed");
		v.y = std::stod(buffer);

		if (!readWord()) throw exception("reader_stl_ascii::read(vec3&): file read failed");
		v.z = std::stod(buffer);
	}

	inline void reader_stl_ascii::readFacets()
	{
		// advance to facet start block.
		nextWord();
		if (!checkWord(stl_tokens::facet_start())) nextWord(stl_tokens::facet_start());

		while (checkWord(stl_tokens::facet_start())) 
			readFacet();

		checkWord(stl_tokens::solid_end());
	}

	inline void reader_stl_ascii::readFacet()
	{
		nextWord(stl_tokens::normal());

		{
			vec3 n;
			read(n);
			m.normals.push_back(n);
		}

		nextWord(stl_tokens::loop_start_a());
		nextWord(stl_tokens::loop_start_b());

		idx3 i;
		i.a = m.points.size();
		i.b = i.a + 1;
		i.c = i.b + 1;

		for (unsigned i = 0; i < 3; ++ i)
		{
			nextWord(stl_tokens::vertex());
			vec3 v;
			read(v);
			m.points.push_back(v);
		}

		nextWord(stl_tokens::loop_end());
		nextWord(stl_tokens::facet_end());

		m.indices.push_back(i);

		nextWord();
	}

	inline void reader_stl_ascii::read()
	{
		nextWord(stl_tokens::solid_start());
		readFacets();
	}

}

#endif // __INC_MIO_READER_STL_ASCII_HPP__