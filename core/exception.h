// exception.h
// Gregory Rosenblatt
// 11/5/04

#ifndef Starscape_Exception_H_
#define Starscape_Exception_H_

#include <stdexcept>

namespace Starscape {

	/** Library-specific exception class derived from std::runtime_error.
	 * A complete error string is provided by calling what().
	 */
	class Exception : public std::runtime_error	{
	public:
		/**
		 * @param msg: failure description
		 * @param apiErr: API specific error message as applicable
		 */
		Exception(const char* msg, const char* apiErr = "n/a")
			: std::runtime_error(std::string(msg)
			+std::string("\nAPI Error: ")+std::string(apiErr))	{}
	};
}

#endif
