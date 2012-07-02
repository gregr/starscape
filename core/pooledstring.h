// pooledstring.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_PooledString_H_
#define Starscape_PooledString_H_

#include "poolallocator.h"
#include <string>

namespace Starscape {

	/** A std::basic_string that allocates from a memory pool. */
	template <typename CharType>
	struct PooledBasicString {
		typedef std::basic_string<CharType, std::char_traits<CharType>
						,typename PoolAllocator<CharType>::Type >	Type;
	};

	/** A std::string that allocates from a memory pool. */
	typedef PooledBasicString<char>::Type	PooledString;
}

#endif
