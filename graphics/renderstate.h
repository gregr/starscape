// renderstate.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_RenderState_H_
#define Starscape_Graphics_RenderState_H_

#include "graphics/config/opengl.h"

namespace Starscape {

	namespace Graphics {

		/** Prepares the next frame for rendering.
		 * Clears the display and resets the model-view matrix.
		 */
		inline void BeginRender() {
			assert(!glGetError());	// start using this
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			assert(!glGetError());
		}

		/** Updates the display to reflect the rendered frame. */
		inline void EndRender() {
			assert(!glGetError());
			SwapOpenGLBuffers();
			assert(!glGetError());
		}

		/** Prepares for the use of alpha-blending. */
		inline void EnableBlending() {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);
		}

		/** Disables alpha-blending. */
		inline void DisableBlending() {
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}

		/** Prepares for use of the alpha-rejection test. */
		inline void EnableAlphaTest() {
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glDepthMask(GL_FALSE);
		}

		/** Disables alpha-rejection testing. */
		inline void DisableAlphaTest() {
			glDepthMask(GL_TRUE);
			glDisable(GL_ALPHA_TEST);
		}

		/** Enables back-face culling. */
		inline void EnableCulling() {
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
		}

		/** Disablees back-face culling. */
		inline void DisableCulling() {
			glDisable(GL_CULL_FACE);
		}
	}
}

#endif
