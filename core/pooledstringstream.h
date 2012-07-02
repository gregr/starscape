// pooledstringstream.h
// Gregory Rosenblatt
// 1/23/05

#ifndef Starscape_PooledStringStream_H_
#define Starscape_PooledStringStream_H_

#include "pooledstring.h"
#include <sstream>

namespace Starscape {

	/** A std::stringstream that allocates from a memory pool. */
	typedef std::basic_stringstream<PooledString::value_type
								,PooledString::traits_type
								,PooledString::allocator_type>
			PooledStringStream;

	/** A std::istringstream that allocates from a memory pool. */
	typedef std::basic_istringstream<PooledString::value_type
								,PooledString::traits_type
								,PooledString::allocator_type>
			PooledInputStringStream;

	/** A std::stringstream that allocates from a memory pool. */
	typedef std::basic_ostringstream<PooledString::value_type
								,PooledString::traits_type
								,PooledString::allocator_type>
			PooledOutputStringStream;
}

#endif
