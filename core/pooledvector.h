// pooledvector.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_PooledVector_H_
#define Starscape_PooledVector_H_

#include "poolallocator.h"
#include <vector>

namespace Starscape {

	/** A std::vector that allocates from a memory pool. */
	template <typename DataType>
	struct PooledVector {
		typedef std::vector<DataType
							,typename PoolAllocator<DataType>::Type >	Type;
	};
}

#endif
