// fontmanager.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Raster_FontManager_H_
#define Starscape_Raster_FontManager_H_

#include "raster/font.h"
#include "raster/fontcontext.h"
#include "resourcemanager.h"
#include "pooledstring.h"

namespace Starscape {

	namespace Raster {

		const char* const ERR_GRAPHICS_FONTMANAGER_INIT =
					"FontManager: Could not initialize TTF.";

		struct FontKey {
			FontKey(const char* fileName, int ptSize)
				: name(fileName), size(ptSize)	{}
			bool operator<(const FontKey& key) const {
				if (name < key.name)
					return true;
				else if (name == key.name)
					return (size < key.size);
				return false;
			}
			PooledString	name;
			int				size;
		};

		/* Creates Font resources to be used with a ResourceManager. */
		struct FontLoader {
			typedef Font							ResourceType;
			typedef FontKey							KeyType;
			typedef boost::shared_ptr<ResourceType>	ResourcePtr;
			ResourcePtr operator()(const KeyType& key) const {
				return ResourcePtr(new Font(key.name.c_str()
											,key.size));
			}
		};

		/** Manages Font resources and provides an interface to access them.
		 * This manager initializes the truetype font system.
		 * The system is automatically shutdown upon destruction.
		 */
		class FontManager : public ResourceManager<FontLoader> {
			FontContext	context_;	// no other font context should be active
		};

		typedef FontManager::ResourceHandleType	FontHandle;
	}
}

#endif
