// imagetexture.h
// Gregory Rosenblatt
// 5/12/05

#ifndef Starscape_Graphics_ImageTexture_H_
#define Starscape_Graphics_ImageTexture_H_

#include "graphics/texture.h"

namespace Starscape {

	namespace Graphics {
		
		/** A texture used for drawing images in an orthogonal scene.
		 * Images are drawn by using the ImageRegion class.
		 */
		class ImageTexture : public Texture<ImageMapper> {
		public:
			/** Creates an ImageTexture using the provided Surface data.
			 * @param surface The surface to use as image data.
			 */
			ImageTexture(const Raster::Surface& surface)
				: Texture<ImageMapper>(surface)	{}
		};
	}
}

#endif
