// pooledlist.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_PooledList_H_
#define Starscape_PooledList_H_

#include "poolallocator.h"
#include <list>

namespace Starscape {

	/** A std::list that allocates from a memory pool. */
	template <typename DataType>
	struct PooledList {
		typedef std::list<DataType
						,typename PoolAllocator<DataType>::FastType >	Type;
	};
}

#endif
