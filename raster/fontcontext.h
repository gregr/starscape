// fontcontext.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Raster_FontContext_H_
#define Starscape_Raster_FontContext_H_

#include "exception.h"
#include "SDL_ttf.h"

namespace Starscape {

	namespace Raster {

		const char* const ERR_RASTER_FONTCONTEXT_INIT =
				"FontContext: Could not initialize SDL-TTF.";

		struct FontContext {
			FontContext() {
				// there should only be one fontcontext
				// therefore, this assertion should always hold true
				assert(!TTF_WasInit());
				if (TTF_Init() != 0)
					throw Exception(ERR_RASTER_FONTCONTEXT_INIT
									,TTF_GetError());
			}
			~FontContext()	{ TTF_Quit(); }
		};
	}
}

#endif
