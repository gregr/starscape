// opengl.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_Config_OPENGL_H_
#define Starscape_Graphics_Config_OPENGL_H_

// This file hides details of how opengl is included.
// use the SDL header by default
#include "SDL_opengl.h"

namespace Starscape {

	namespace Graphics {

		inline void SwapOpenGLBuffers() {
			SDL_GL_SwapBuffers();
		}
	}
}

#endif
