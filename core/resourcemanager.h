// resourcemanager.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_ResourceManager_H_
#define Starscape_ResourceManager_H_

#include "pooledmap.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>
#include <algorithm>

namespace Starscape {

	/** Type that must be returned by a resource loader. */
	template <typename ResourceType>
	struct ResourcePtr {
		typedef boost::shared_ptr<ResourceType>	Type;
	};

	// used internally by handle and manager
	template <typename ResourceType>
	struct ResourcePtrPtr {
		typedef boost::shared_ptr<
				typename ResourcePtr<ResourceType>::Type>	Type;
	};

	/** Allows indirect use of a resource with a pointer-like interface. */
	template <class Resource>
	class ResourceHandle {
		typedef typename ResourcePtrPtr<Resource>::Type	ResourcePtrPtr;
	public:
		/** A handle is constructed directly from a resource reference. */
		explicit ResourceHandle(const ResourcePtrPtr& resource)
			: resource_(resource)	{}
		/** Dereferences the handle. */
		Resource& operator*() const	{ return *Get(); }
		/** Calls a method through the handle. */
		Resource* operator->() const	{ return Get(); }
	private:
		Resource* Get() const	{ return resource_->get(); }
		ResourcePtrPtr	resource_;
	};

	/** Loads, stores and ensures the uniqueness of resource objects.
	 * Resources will remained cached while there are handles to them.
	 * ResourceLoader is a functor that loads a particular resource type.
	 * - Functor requirements
	 * 	- Must provide declarations for the resource and key types.
	 * 	- operator() must take a key and return a shared_ptr to the resource.
	 */
	template <class ResourceLoader>
	class ResourceManager {
	public:
		typedef typename ResourceLoader::ResourceType	ResourceType;
		typedef typename ResourceLoader::KeyType		KeyType;
		typedef ResourceHandle<ResourceType>			ResourceHandleType;
	private:
		typedef typename ResourcePtr<ResourceType>::Type	ResourcePtr;
		typedef typename ResourcePtrPtr<ResourceType>::Type	ResourcePtrPtr;
		typedef boost::weak_ptr<ResourcePtr>	WeakResourcePtrPtr;
		typedef typename PooledMap<KeyType
						,WeakResourcePtrPtr >::Type	ResourceMap;
		ResourceMap	map_;
		// noncopyable semantics
		ResourceManager(const ResourceManager&);
		void operator=(const ResourceManager&);
	public:
		ResourceManager()	{}	// needed due to copy-constructor
		/** Retrieves the resource indexed by the given key.
		 * The resource will be loaded if it is not already present.
		 * @param key The unique index for a resource.
		 * @param load A functor for loading the resource if necessary.
		 * @return	A handle to the indexed resource.
		 */
		ResourceHandleType Get(const KeyType& key
							,ResourceLoader load = ResourceLoader()) {
			// ensures the resource pointer lasts until assignment
			ResourcePtrPtr	ptr;
			typename ResourceMap::iterator itr = map_.find(key);
			// if resource is already loaded test for expiration
			if (itr != map_.end()) {
				if (itr->second.expired()) {
					// if expired, reload
					ptr = ResourcePtrPtr(new ResourcePtr(load(key)));
					itr->second = ptr;
				}
				else
					ptr = ResourcePtrPtr(itr->second);
			}
			else {	// otherwise, load the resource using provided functor
				ptr = ResourcePtrPtr(new ResourcePtr(load(key)));
				map_.insert(std::make_pair(key, WeakResourcePtrPtr(ptr)));
			}
			// return the handle to the new resource
			return ResourceHandleType(ptr);
		}
		/** Reloads the resource with the given key if it is present. */
		void Reload(const KeyType& key
					,ResourceLoader load = ResourceLoader()) {
			typename ResourceMap::iterator itr = map_.find(key);
			if (itr == map_.end())
				return;
			if (itr->second.expired())
				return;
			ResourcePtrPtr ptr(itr->second);
			*ptr = load(key);
		}
		/** Reloads all currently stored resources. */
		void ReloadAll(ResourceLoader load = ResourceLoader()) {
			typename ResourceMap::iterator itr = map_.begin()
										,end = map_.end();
			for (; itr != end; ++itr) {
				if (!itr->second.expired()) {
					ResourcePtrPtr ptr(itr->second);
					*ptr = load(itr->first);
				}
			}
		}
	private:
		template <typename OperationType>
		static void Operate(const typename ResourceMap::iterator& itr
							,const OperationType& operation) {
			WeakResourcePtrPtr ptr = (itr->second);
			if (!ptr.expired())	// operate only if not expired
				operation(**ptr.lock());
		}
	protected:
		/** Performs the given operation on all contained resources.
		 * Derived managers are intended to make use of this template.
		 * - Function objects can be created using boost::bind().
		 * @param operation	A unary function object taking the resource.
		 */
		template <typename OperationType>
		void OperateOnAll(const OperationType& operation) {
			typename ResourceMap::iterator itr = map_.begin(),
											end = map_.end();
			for (; itr != end; ++itr)
				Operate(itr, operation);
//			std::for_each(map_.begin(), map_.end()
//					,boost::bind(&Operate<OperationType>, _1, operation));
		}
	};
}

#endif
