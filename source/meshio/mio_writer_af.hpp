#ifndef __INC_MIO_WRITER_AF_HPP__
#define __INC_MIO_WRITER_AF_HPP__

#include "mio_mesh.hpp"
#include "mio_af_tokens.hpp"

#include <algorithm>
#include <fstream>
#include <string>

namespace mio
{

	class writer_af
	{
	public:
		static void save(const std::string & str, const mesh & m);
		static void save(std::ostream & in, const mesh & m);
	private:
		writer_af();
	};

	inline void writer_af::save(const std::string & str, const mesh & m)
	{
		std::ofstream out(str, std::ios_base::out | std::ios_base::trunc);
		if (!out.is_open()) throw exception("writer_af::save: could not open output file");
		save(out, m);
	}

	inline void writer_af::save(std::ostream & out, const mesh & m)
	{
		std::size_t index;

		auto vout = [&](const vec3 & v){ out << index++ << " " << v.x << " " << v.y << " " << v.z << std::endl; };
		auto iout = [&](const idx3 & i){ out << index++ << " " << i.a << " " << i.b << " " << i.c << std::endl; };

		out << af_tokens::header();

		if (!m.points.empty())
		{
			index = 0;
			out << std::endl << af_tokens::points() << std::endl;
			std::for_each(m.points.begin(), m.points.end(), vout);
		}

		if (!m.normals.empty() && (m.normals.size() == m.points.size()))
		{
			index = 0;
			out << std::endl << af_tokens::normals() << std::endl;
			std::for_each(m.normals.begin(), m.normals.end(), vout);
		}

		if (!m.indices.empty())
		{
			index = 0;
			out << std::endl << af_tokens::facets() << std::endl;
			std::for_each(m.indices.begin(), m.indices.end(), iout);
		}
	}

}

#endif // __INC_MIO_WRITER_AF_HPP__