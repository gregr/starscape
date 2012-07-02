// surfacedata.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_Config_SURFACEDATA_H_
#define Starscape_Graphics_Config_SURFACEDATA_H_

// provide Surface, ManagedSurface, SurfaceColor
// ,CreateNewSurface(), TransferSurfaceData() and CreateImage()
// Raster::Surface must provide:
// Width(), Height(), SetColorKey() and PixelData()
#include "raster/transfersurfacedata.h"
#include "raster/createimage.h"
//#include "graphics/config/opengl.h"
#include "makepoweroftwo.h"

namespace Starscape {

	namespace Raster {

		/** Creates a surface with power-of-two dimensions from source. */
		inline ManagedSurface CreateSurfacePowerOfTwo(const Surface& source) {
			assert(source.IsValid());
			// create new surface with dimensions that are a power of 2
			ManagedSurface target = CreateNewSurface(
										MakePowerOfTwo(source.Width())
										,MakePowerOfTwo(source.Height()));
			// make an alpha-independent blit to copy data
			SDL_Rect pos = { 0, 0, 0, 0 };
			TransferSurfaceData(source, target, pos);
			return target;
		}
	}
}

#endif
