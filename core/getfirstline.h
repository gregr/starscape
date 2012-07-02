// getfirstline.h
// Gregory Rosenblatt
// 3/22/05

#ifndef Starscape_GetFirstLine_H_
#define Starscape_GetFirstLine_H_

#include "pooledstring.h"

namespace Starscape {

	/** Gets a substring that is the first line of the given string. */
	inline PooledString GetFirstLine(const PooledString& str) {
		return str.substr(0, str.find_first_of('\n'));
	}
}

#endif
