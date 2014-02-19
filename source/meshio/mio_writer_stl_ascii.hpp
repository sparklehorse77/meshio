#ifndef __INC_MIO_WRITER_STL_ASCII_HPP__
#define __INC_MIO_WRITER_STL_ASCII_HPP__

#include "mio_mesh.hpp"
#include "mio_stl_tokens.hpp"

#include <algorithm>
#include <fstream>
#include <string>

namespace mio
{
	class writer_stl_ascii
	{
	public:
		static void save(const std::string & str, const mesh & m);
		static void save(std::ostream & out, const mesh & m);
	private:
		void write();
		void write_tabs();
		void write(const std::string & prefix, const vec3 & v);
		void write_solid_start(const char * name);
		void write_facet_start(const vec3 & n);
		void write_facet_end();
		void write_vertices(const vec3 & a, const vec3 & b, const vec3 & c);
		void write_solid_end(const char * name);

		writer_stl_ascii(std::ostream & s, const mesh & _m) : os(s), m(_m), tabs(0) {}
		std::ostream & os;
		const mesh & m;
		std::size_t tabs;
	};

	inline void writer_stl_ascii::save(const std::string & str, const mesh & m)
	{
		std::ofstream out(str, std::ios_base::out | std::ios_base::trunc);
		if (!out.is_open()) throw exception("writer_stl_ascii::save: could not open output file");
		save(out, m);
	}

	inline void writer_stl_ascii::save(std::ostream & out, const mesh & m)
	{
		writer_stl_ascii(out, m).write();
	}

	inline void writer_stl_ascii::write()
	{
		write_solid_start("unnamed");
		{
			for (std::size_t j = 0; j < m.indices.size(); ++j)
			{
				const idx3 & i = m.indices[j];

				vec3 n(0);
				const vec3 & a = m.points[i.a];
				const vec3 & b = m.points[i.b];
				const vec3 & c = m.points[i.c];

				if (m.normalsPerFacet()) n = m.normals[j];

				write_facet_start(n);
				write_vertices(a, b, c);
				write_facet_end();
			}
		}
		write_solid_end("unnamed");
	}

	inline void writer_stl_ascii::write_tabs()
	{
		for (std::size_t i = 0; i < tabs; ++i) os << "\t";
	}

	inline void writer_stl_ascii::write(const std::string & prefix, const vec3 & v)
	{
		os << prefix << " " << v.x << " " << v.y << " " << v.z << std::endl;
	}

	inline void writer_stl_ascii::write_solid_start(const char * name)
	{
		write_tabs();
		os << stl_tokens::solid_start() << " " << name << std::endl;
		++tabs;
	}
	
	inline void writer_stl_ascii::write_facet_start(const vec3 & n)
	{
		write_tabs();
		os << stl_tokens::facet_start() << " ";
		write(stl_tokens::normal(), n);
		++tabs;
	}
	
	inline void writer_stl_ascii::write_facet_end()
	{
		--tabs;
		write_tabs();
		os << stl_tokens::facet_end() << std::endl;
	}

	inline void writer_stl_ascii::write_vertices(const vec3 & a, const vec3 & b, const vec3 & c)
	{
		write_tabs();
		os << stl_tokens::loop_start_a() << " " << stl_tokens::loop_start_b() << std::endl;
		++tabs;

		write_tabs();
		write(stl_tokens::vertex(), a);
		write_tabs();
		write(stl_tokens::vertex(), b);
		write_tabs();
		write(stl_tokens::vertex(), c);
		--tabs;

		write_tabs();
		os << stl_tokens::loop_end() << std::endl;
	}
	
	inline void writer_stl_ascii::write_solid_end(const char * name)
	{
		--tabs;
		write_tabs();
		os << stl_tokens::solid_end() << " " << name << std::endl;
	}

}

#endif // __INC_MIO_WRITER_STL_ASCII_HPP__