// poolallocator.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_PoolAllocator_H_
#define Starscape_PoolAllocator_H_

#define STARSCAPE_USE_STD_ALLOCATOR
//#define STARSCAPE_USE_BOOST_POOL_ALLOCATOR
// add gnu pool allocators?

#ifdef STARSCAPE_USE_BOOST_POOL_ALLOCATOR
#include <boost/pool/pool_alloc.hpp>
#endif
#ifdef STARSCAPE_USE_STD_ALLOCATOR
#include <memory>
#endif

namespace Starscape {

	template <typename T>
	struct PoolAllocator {
#ifdef STARSCAPE_USE_BOOST_POOL_ALLOCATOR
		typedef boost::pool_allocator<T>	Type;
		typedef boost::fast_pool_allocator<T>	FastType;
#endif
#ifdef STARSCAPE_USE_STD_ALLOCATOR
		typedef std::allocator<T>	Type;
		typedef std::allocator<T>	FastType;
#endif
	};
}

#endif
