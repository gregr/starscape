// cursorbrush.h
// Gregory Rosenblatt
// 5/13/05

#ifndef Starscape_Gui_CursorBrush_H_
#define Starscape_Gui_CursorBrush_H_

#include "gui/appearance/staticimage.h"
#include "util/coord.h"

namespace Starscape {

	namespace Gui {

		/** An image that represents the cursor.
		 * Drawn at an offset to properly represent the pointer.
		 */
		class CursorBrush {
		public:
			/** Creates a cursor from the provided image and offset.
			 * @param image The static image to draw.
			 * @param offset The relative position actually pointed to.
			 */
			CursorBrush(const StaticImage& image
					,const Coord& offset = Coord())
				: image_(image), offset_(offset)	{}
			/** Draws the cursor so that it points to the given position. */
			void Draw(const Rect& pos) const {
				image_.Draw(MakeRect(pos.x-offset_.x
									,pos.y-offset_.y
									,0, 0));
			}
		private:
			StaticImage	image_;
			Coord		offset_;
		};
	}
}

#endif
