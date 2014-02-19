#ifndef __INC_MIO_AF_TOKENS_HPP__
#define __INC_MIO_AF_TOKENS_HPP__

namespace mio
{
	struct af_tokens
	{
		static const std::string & header() { static const std::string t("#Mesh"); return t; }
		static const std::string & points() { static const std::string t("Points"); return t; }
		static const std::string & normals() { static const std::string t("Normals"); return t; }
		static const std::string & facets() { static const std::string t("Facets"); return t; }
	};
}

#endif // __INC_MIO_AF_TOKENS_HPP__