#ifndef __INC_MIO_ENDIAN_HPP__
#define __INC_MIO_ENDIAN_HPP__

#include <algorithm>

namespace mio
{


	class endian
	{
	public:
		template<typename TVALUE>
		static TVALUE h_to_le(const TVALUE & v);

		template<typename TVALUE>
		static TVALUE le_to_h(const TVALUE & v);

		static bool isLE();
	private:
		endian();

		template<typename TBASE>
		struct byteswapper
		{
			byteswapper(const TBASE & v) : value(v) {}
			static const std::size_t size = sizeof(TBASE);
			union
			{
				struct
				{
					char x[sizeof(TBASE)];
				};
				TBASE value;
			};
			TBASE swap() { std::reverse(x, &x[size]); return value; }
		};
	};

	template<typename TVALUE>
	inline TVALUE endian::h_to_le(const TVALUE & v)
	{
		if (isLE()) return v;
		return byteswapper<TVALUE>(v).swap();
	}

	template<typename TVALUE>
	inline TVALUE endian::le_to_h(const TVALUE & v)
	{
		if (isLE()) return v;
		return byteswapper<TVALUE>(v).swap();
	}

	inline bool endian::isLE()
	{
		static const unsigned i = 1;
		return (reinterpret_cast<const char *>(&i)[0] != 0);
	}
}

#endif // __INC_MIO_ENDIAN_HPP__