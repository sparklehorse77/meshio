#ifndef __INC_MIO_EXCEPTION_HPP__
#define __INC_MIO_EXCEPTION_HPP__

#include <stdexcept>

namespace mio
{

	class exception : public std::exception
	{
	public:
		exception(const char * m) throw() : std::exception(), message(m) {}
		virtual ~exception() throw() {}
		virtual const char * what() const { return message; }
		const char * message;
	};

}

#endif // __INC_MIO_EXCEPTION_HPP__