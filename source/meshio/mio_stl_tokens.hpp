#ifndef __INC_MIO_STL_TOKENS_HPP__
#define __INC_MIO_STL_TOKENS_HPP__

namespace mio
{
	struct stl_tokens
	{
		static const std::string & solid_start() { static const std::string t("solid"); return t; }
		static const std::string & solid_end() { static const std::string t("endsolid"); return t; }
		static const std::string & facet_start() { static const std::string t("facet"); return t; }
		static const std::string & facet_end() { static const std::string t("endfacet"); return t; }
		static const std::string & loop_start_a() { static const std::string t("outer"); return t; }
		static const std::string & loop_start_b() { static const std::string t("loop"); return t; }
		static const std::string & loop_end() { static const std::string t("endloop"); return t; }
		static const std::string & normal() { static const std::string t("normal"); return t; }
		static const std::string & vertex() { static const std::string t("vertex"); return t; }
	};
}

#endif // __INC_MIO_STL_TOKENS_HPP__