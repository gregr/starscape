// surfaceregion.h
// Gregory Rosenblatt
// 5/8/05

#ifndef Starscape_Raster_SurfaceRegion_H_
#define Starscape_Raster_SurfaceRegion_H_

#include "raster/surface.h"

namespace Starscape {

	namespace Raster {

		/** A rectangular region within a Surface that emulates blitting.
		 * The purpose of this class is to allow a single surface to
		 * store several sub-images for the sake of efficiency.
		 */
		class SurfaceRegion {
		public:
			/** Takes a surface and the region containing an image.
			 * @param surface The surface containing the desired image.
			 * @param region The region containing the desired image data.
			 */
			SurfaceRegion(const Surface& surface, const SDL_Rect& region)
				: surface_(surface), region_(region)	{}
			/** Blits this region's image to the destination surface.
			 * The final blit region is saved in dstRect after clipping.
			 * @param dest The destination Surface.
			 * @param dstRect The rectangular region to blit to.
			 * @return 0 on success, otherwise -1.
			 */
			int BlitTo(const Surface& dest, SDL_Rect& dstRect) {
				return surface_.BlitRectTo(region_, dest, dstRect);
			}
			/** The region width. */
			int Width() const	{ return region_.w; }
			/** The region height. */
			int Height() const	{ return region_.h; }
		private:
			const Surface&	surface_;
			SDL_Rect		region_;
		};
	}
}

#endif
