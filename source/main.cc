#include "meshio/mio.hpp"

#include <iostream>
#include <string>
#include <list>

typedef std::list<mio::mesh> MeshList;

void load(const std::string & filename, MeshList & m)
{
	m.push_back(mio::mesh());
	mio::file_reader::load(filename, m.back());

	std::cout << "loaded mesh has " 
		<< m.back().points.size() << " points, " 
		<< m.back().normals.size() << " normals and " 
		<< m.back().indices.size() << " indices." << std::endl;

	if (m.size() > 1) 
	{
		const mio::mesh & f = m.front();
		const mio::mesh & b = m.back();

		if (f.points.size() != b.points.size()) std::cout << "\t!points mismatch" << std::endl;
		if (f.indices.size() != b.indices.size()) std::cout << "\t!points mismatch" << std::endl;
	}
}

int main(int argc, char ** argv)
{
	MeshList m;
	load(TESTFILE_STL_BIN, m);

	{
		const std::string ioname = std::string(TESTIO).append("/test.ascii.stl");
		mio::writer_stl_ascii::save(ioname, m.back());
		load(ioname, m);
	}

	{
		const std::string ioname = std::string(TESTIO).append("/test.ascii.af");
		mio::writer_af::save(ioname, m.back());
		load(ioname, m);
	}

	{
		const std::string ioname = std::string(TESTIO).append("/test.bin.stl");
		mio::writer_stl_binary::save(ioname, m.back());
		load(ioname, m);
	}
}

