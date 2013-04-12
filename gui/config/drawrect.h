// drawrect.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Gui_Config_DrawRect_H_
#define Starscape_Gui_Config_DrawRect_H_

#include "gui/config/color.h"
#include "util/scalar.h"

namespace Starscape {

	namespace Gui {

		/** Draws the given rectangle using the current color. */
		inline void DrawRect(const Rect& rect) {
			glRecti(rect.x, rect.y, rect.x+rect.w, rect.y+rect.h);
		}

		typedef Graphics::Color	RectColorGradient[4];

		/** Draws a shaded rectangle using the color gradient provided.
		 * The gradient contains a color for each vertex of the rectangle.
		 * - Vertex colors are stored in this order:
		 *  - Top Left
		 *  - Bottom Left
		 *  - Bottom Right
		 *  - Top Right
		 */
		inline void DrawShadedRect(const Rect& rect
									,const RectColorGradient& gradient) {
			glBegin(GL_QUADS);
			gradient[0].MakeCurrent();
			glVertex2i(rect.x, rect.y);
			gradient[1].MakeCurrent();
			glVertex2i(rect.x, rect.y+rect.h);
			gradient[2].MakeCurrent();
			glVertex2i(rect.x+rect.w, rect.y+rect.h);
			gradient[3].MakeCurrent();
			glVertex2i(rect.x+rect.w, rect.y);
			glEnd();
		}
	}
}

#endif
