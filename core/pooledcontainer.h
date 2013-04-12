// pooledcontainer.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_PooledContainer_H_
#define Starscape_PooledContainer_H_

#include "poolallocator.h"

// Mainly an example template... probably better to create and use
// specific ones, such as PooledMap.
namespace Starscape {

	/** Simplifies the declaration of a container that uses a memory pool. */
	template <template <typename T, class A> class Container
			,typename ValueType>
	struct PooledContainer {
		/** General-purpose pooled container.
		 * Most efficient for containers using contiguous memory chunks.
		 */
		typedef Container<ValueType
						,typename PoolAllocator<ValueType>::Type >
				Type;
		/** A pooled container optimized for single-chunk containers.
		 * Only use with containers that allocate like std::list<>
		 * that do not allocate contiguous memory chunks.
		 */
		typedef Container<ValueType
						,typename PoolAllocator<ValueType>::FastType >
				FastType;
	};
}

#endif
