// pooleddeque.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_PooledDeque_H_
#define Starscape_PooledDeque_H_

#include "poolallocator.h"
#include <deque>

namespace Starscape {

	/** A std::deque that allocates from a memory pool. */
	template <typename DataType>
	struct PooledDeque {
		typedef std::deque<DataType
						,typename PoolAllocator<DataType>::Type >	Type;
	};
}

#endif
