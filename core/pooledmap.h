// pooledmap.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_PooledMap_H_
#define Starscape_PooledMap_H_

#include "poolallocator.h"
#include <map>

namespace Starscape {

	/** A std::map that allocates from a memory pool. */
	template <typename KeyType, typename DataType
				,class CompareType = std::less<KeyType> >
	struct PooledMap {
		typedef std::map<KeyType, DataType, CompareType
						,typename PoolAllocator<std::pair<KeyType, DataType>
												>::FastType>	Type;
	};
}

#endif
