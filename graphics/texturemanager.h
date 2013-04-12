// texturemanager.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_TextureManager_H_
#define Starscape_Graphics_TextureManager_H_

#include "graphics/texture.h"
#include "resourcemanager.h"
#include "pooledstring.h"

namespace Starscape {

	/* Loader for mipmapped Texture objects containing data from file.
	 * This class is used in conjuction with the ResourceManager template.
	 */
	namespace Graphics {

		// allow color comparison
		inline bool operator<(const Raster::SurfaceColor& c1
							,const Raster::SurfaceColor& c2) {
			if (c1.r < c2.r)
				return true;
			else if (c1.r == c2.r) {
				if (c1.g < c2.g)
					return true;
				else if (c1.g == c2.g) {
					return (c1.b < c2.b);
				}
			}
			return false;
		}

		struct TextureKey {
			TextureKey(const char* fileName, bool used
					,const Raster::SurfaceColor& ck)
				: name(fileName), colorKey(ck), keyUsed(used)	{}
			bool operator<(const TextureKey& key) const {
				if (name < key.name)
					return true;
				else if (name == key.name) {
					if (keyUsed < key.keyUsed)
						return true;
					else if (keyUsed == key.keyUsed)
						return (colorKey < key.colorKey);
				}
				return false;
			}
			PooledString			name;
			Raster::SurfaceColor	colorKey;
			bool					keyUsed;
		};

		/* Loader for Texture objects containing data from file.
		 * This class is used in conjunction with a ResourceManager.
		 */
		template <class TextureType = Texture<> >
		struct TextureLoader {
			typedef TextureType	ResourceType;
			typedef TextureKey	KeyType;
			typedef typename ResourcePtr<ResourceType>::Type	ResourcePtr;
			ResourcePtr operator()(const KeyType& key) const {
				Raster::ManagedSurface
						data = Raster::CreateImage(key.name.c_str());
				if (key.keyUsed)
					SetColorKey(data, key.colorKey);
				return ResourcePtr(new ResourceType(data));
			}
		};

		/** A manager for Texture resources. */
		class TextureManager : public ResourceManager<TextureLoader<> > {
		public:
			/** Reacquires all textures. */
			void ReacquireAll() {
				ReleaseAll();
				ReloadAll();
			}
		private:
			/* Releases all fonts in preparation to reacquire them. */
			void ReleaseAll() {
				OperateOnAll(boost::bind(&Texture<>::Release, _1));
			}
		};
		
		typedef TextureManager::ResourceHandleType	TextureHandle;
	}
}

#endif
