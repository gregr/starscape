// cursorbrushstate.h
// Gregory Rosenblatt
// 5/13/05

#ifndef Starscape_Gui_CursorBrushState_H_
#define Starscape_Gui_CursorBrushState_H_

#include "gui/appearance/cursorbrush.h"

namespace Starscape {

	namespace Gui {

		/** Keeps the active and default cursor brushes for an interface. */
		class CursorBrushState {
		public:
			CursorBrushState()
				: cursorBrushDefault_(0), cursorBrush_(0)	{}
			/** Sets a custom brush to use when drawing the cursor. */
			void SetBrush(const CursorBrush& brush) {
				cursorBrush_ = &brush;
			}
			/** Resets the cursor brush to the provided default. */
			void ResetBrush()	{ cursorBrush_ = cursorBrushDefault_; }
			void Draw(const CoordScalar& xpos, const CoordScalar& ypos) {
				if (!cursorBrush_)
					return;
				Graphics::ResetColor();
				cursorBrush_->Draw(MakeRect(xpos, ypos, 0, 0));
			}
		private:
			friend class Interface;
			/** Sets the default brush to use when drawing the cursor. */
			void SetDefaultBrush(const CursorBrush& brush) {
				cursorBrushDefault_ = &brush;
				ResetBrush();
			}
			const CursorBrush	*cursorBrushDefault_, *cursorBrush_;
		};
	}
}

#endif
