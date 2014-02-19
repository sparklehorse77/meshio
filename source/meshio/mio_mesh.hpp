#ifndef __INC_MIO_MESH_HPP__
#define __INC_MIO_MESH_HPP__

#include <vector>

namespace mio
{

	struct vec3
	{
		vec3() {}
		vec3(const double v) : x(v), y(v), z(v) {}
		vec3(const double _x,const double _y,const double _z) : x(_x), y(_y), z(_z) {}

		double x;
		double y;
		double z;
	};

	struct idx3
	{
		idx3() {}
		idx3(const std::size_t v) : a(v), b(v), c(v) {}
		idx3(const std::size_t _a, const std::size_t _b, const std::size_t _c) : a(_a), b(_b), c(_c) {}

		std::size_t a;
		std::size_t b;
		std::size_t c;
	};

	struct mesh
	{
		typedef std::vector<vec3> VectorCollection;
		typedef std::vector<idx3> IndexCollection;

		void clear()								{ points.clear(), normals.clear(), indices.clear(); }
		bool empty() const							{ return (points.empty() && normals.empty() && indices.empty()); }
		bool normalsPerVertex() const				{ return (normals.size() == points.size());  }
		bool normalsPerFacet() const				{ return (normals.size() == indices.size()); }
		void reservePoints(const std::size_t n)		{ points.reserve(n); }
		void reserveNormals(const std::size_t n)	{ normals.reserve(n); }
		void reserveFacets(const std::size_t n)		{ indices.reserve(n); }

		VectorCollection points;
		VectorCollection normals;
		IndexCollection indices;
	};

}

#endif // __INC_MIO_MESH_HPP__