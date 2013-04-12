// createimage.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Raster_CreateImage_H_
#define Starscape_Raster_CreateImage_H_

#include "raster/managedsurface.h"
#include "SDL_image.h"

namespace Starscape {
		
	namespace Raster {

		const char* const ERR_CREATE_IMAGE =
			"CreateImage: Failed to load the image.";

		/** Opens an image from file and loads it into a ManagedSurface.
		 * Implements image-loading using the SDL_image library.
		 * @param fileName The image file to load.
		 * @return A ManagedSurface containing the image data.
		 */
		inline ManagedSurface CreateImage(const char* fileName) {
			ManagedSurface image(IMG_Load(fileName));
			if (!image.IsValid())
				throw Exception(ERR_CREATE_IMAGE, IMG_GetError());
			return image;
		}
	}
}

#endif
