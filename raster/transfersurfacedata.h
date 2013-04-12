// transfersurfacedata.h
// Gregory Rosenblatt
// 5/8/05

#ifndef Starscape_Raster_TransferSurfaceData_H_
#define Starscape_Raster_TransferSurfaceData_H_

#include "raster/surface.h"
#include "exception.h"

namespace Starscape {

	namespace Raster {

		const char* const ERR_TRANSFER_SURFACE_DATA_BLIT =
			"TransferSurfaceData: Could not blit data.";
		const char* const ERR_TRANSFER_SURFACE_DATA_SETALPHA =
			"TransferSurfaceData: Could not set alpha flags.";

		/** Transfers the raw data from one surface to another.
		 * Source and destination surfaces do not need the same format.
		 */
		inline void TransferSurfaceData(const Surface& source
										,const Surface& destination
										,const SDL_Rect& dstPos) {
			SDL_Rect pos = dstPos;
			Uint32 alphaFlags = source.GetAlphaFlags();
			if ((alphaFlags & SDL_SRCALPHA) == SDL_SRCALPHA) {	// if alpha
				Uint8 alpha = source.GetAlphaVal();
				// disable alpha temporarily
				if (source.SetAlpha(0, 0) != 0)
					throw Exception(ERR_TRANSFER_SURFACE_DATA_SETALPHA
									,SDL_GetError());
				// surface information transfer
				if (source.BlitTo(destination, pos) != 0)
					throw Exception(ERR_TRANSFER_SURFACE_DATA_BLIT
									,SDL_GetError());
				// re-enable the alpha flags that were present
				if (source.SetAlpha(alphaFlags, alpha) != 0)
					throw Exception(ERR_TRANSFER_SURFACE_DATA_SETALPHA
									,SDL_GetError());
			}
			else	// if no alpha, simply transfer data
				if (source.BlitTo(destination, pos) != 0)
					throw Exception(ERR_TRANSFER_SURFACE_DATA_BLIT
									,SDL_GetError());
		}
	}
}

#endif
