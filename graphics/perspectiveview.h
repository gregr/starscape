// perspectiveview.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_PerspectiveView_H_
#define Starscape_Graphics_PerspectiveView_H_

#include "graphics/renderstate.h"
#include "graphics/config/displaymetrics.h"
#include "util/scalar.h"

namespace Starscape {

	namespace Graphics {

		/** Causes the display to use perspective projection for rendering.
		 * The view projection remains in a perspective state for as long as
		 * an object of this class is in scope.
		 */
		class PerspectiveView {
		public:
			/** Creates a perspective view for the duration of this object's life.
			 * @param display The display metrics to be used to compute the viewport.
			 * @param region The target region for rendering.
			 */
			PerspectiveView(const Raster::DisplayMetrics& dm, const Rect& region)
					: metrics_(dm) {
				PushPerspectiveView(region);
			}
			/** Creates a perspective view for the duration of this object's life.
			 * The target region for rendering is the entire display viewport.
			 * @param display The display metrics to be used to compute the viewport.
			 */
			PerspectiveView(const Raster::DisplayMetrics& dm) : metrics_(dm) {
				Rect region = { 0, 0, metrics_.Width(), metrics_.Height() };
				PushPerspectiveView(region);
			}
			~PerspectiveView()	{ PopPerspectiveView(); }
		private:
			void PushPerspectiveView(const Rect& region) {
				// blending may not be necessary, this should be moved somewhere else
				DisableBlending();
				glClear(GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST);
				glViewport(static_cast<GLsizei>(region.x)
						,static_cast<GLsizei>(metrics_.Height()-(region.y+region.h))
						,static_cast<GLsizei>(region.w)
						,static_cast<GLsizei>(region.h));
				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
				glLoadIdentity();
				glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 25.0);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();
			}
			void PopPerspectiveView() {
				glDisable(GL_DEPTH_TEST);
				glViewport(0, 0, static_cast<GLsizei>(metrics_.Width())
								,static_cast<GLsizei>(metrics_.Height()));
				glMatrixMode(GL_PROJECTION);
				glPopMatrix();
				glMatrixMode(GL_MODELVIEW);
				glPopMatrix();
				// blending is used in ortho mode right now : move this eventually
				EnableBlending();
			}
			const Raster::DisplayMetrics&	metrics_;
		};
	}
}

#endif
