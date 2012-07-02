// imagemanager.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_ImageManager_H_
#define Starscape_Graphics_ImageManager_H_

#include "graphics/texturemanager.h"
#include "graphics/imagetexture.h"

namespace Starscape {

	namespace Graphics {

		/** A manager for ImageTexture resources. */
		class ImageManager : public ResourceManager<
							 			TextureLoader<ImageTexture> > {
		public:
			/** Reacquires all images. */
			void ReacquireAll() {
				ReleaseAll();
				ReloadAll();
			}
		private:
			/* Releases all images in preparation to reacquire them. */
			void ReleaseAll() {
				OperateOnAll(boost::bind(&ImageTexture::Release, _1));
			}
		};

		typedef ImageManager::ResourceHandleType	ImageHandle;
	}
}

#endif
