// imagefontmanager.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_ImageFontManager_H_
#define Starscape_Graphics_ImageFontManager_H_

#include "graphics/imagefont.h"
#include "resourcemanager.h"
#include "pooledstring.h"

namespace Starscape {

	namespace Graphics {

		struct ImageFontKey {
			ImageFontKey(const char* fileName, int ptSize
						,const ImageFont::Quality& q)
				: name(fileName), size(ptSize), quality(q)	{}
			bool operator<(const ImageFontKey& key) const {
				if (name < key.name)
					return true;
				else if (name == key.name) {
					if (size < key.size)
						return true;
					else if (size == key.size)
						return (quality < key.quality);
				}
				return false;
			}
			PooledString		name;
			int					size;
			ImageFont::Quality	quality;
		};

		/* Creates ImageFont resources to be used with a ResourceManager. */
		struct ImageFontLoader {
			typedef ImageFont						ResourceType;
			typedef ImageFontKey					KeyType;
			typedef boost::shared_ptr<ResourceType>	ResourcePtr;
			ResourcePtr operator()(const KeyType& key) const {
				Raster::Font font(key.name.c_str(), key.size);
				return ResourcePtr(new ResourceType(font, key.quality));
			}
		};

		/** A manager for ImageFont resources. */
		class ImageFontManager : public ResourceManager<ImageFontLoader> {
		public:
			/** Reacquires all fonts. */
			void ReacquireAll() {
				ReleaseAll();
				ReloadAll();
			}
		private:
			/* Releases all fonts in preparation to reacquire them. */
			void ReleaseAll() {
				OperateOnAll(boost::bind(&ImageFont::Release, _1));
			}
			Raster::FontContext	context_;	// must be unique
		};
		
		typedef ImageFontManager::ResourceHandleType	ImageFontHandle;
	}
}

#endif
