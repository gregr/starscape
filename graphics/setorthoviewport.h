// setorthoviewport.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_SetOrthoViewport_H_
#define Starscape_Graphics_SetOrthoViewport_H_

#include "graphics/renderstate.h"

namespace Starscape {

	namespace Graphics {

		inline void SetOrthoViewport(unsigned int width
									,unsigned int height) {
			assert(!glGetError());
			// states
//			EnableCulling();
			glShadeModel(GL_SMOOTH);
			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			EnableBlending();
			// clearing
			glClearDepth(1.0f);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glViewport(0, 0, static_cast<GLsizei>(width)
							,static_cast<GLsizei>(height));
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, static_cast<GLdouble>(width)
					,static_cast<GLdouble>(height), 0.0
					,0.0, 1.0);
			glMatrixMode(GL_MODELVIEW);	// default matrix mode
			assert(!glGetError());
		}
	}
}

#endif
