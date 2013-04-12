// label.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_Label_H_
#define Starscape_Gui_Label_H_

#include "gui/appearance/edit/documentbrush.h"

namespace Starscape {

	namespace Gui {

		/** A non-interactive text resource displayed in an interface. */
		class Label {
		public:
			/** Constructs a Label with the given string value. */
			Label(const DocumentBrush& brush, const DocumentMarkup& markup
					,const char* str="")
				: brush_(brush)
				,document_(brush, markup, SizeScalar(-1), str)	{}
			/** Sets the string value of the displayed text. */
			void SetString(const char* str)	{ document_.SetText(str); }
			/** Retrieves the current string value. */
			const PooledString& GetString() const {
				return document_.GetText();
			}
			/** The text will be wrapped within the given width. */
			void EnableTextWrapping(const SizeScalar& width) {
				document_.ChangeWidth(width);
				document_.EnableTextWrapping();
			}
			/** The text will not wrap within a boundary. */
			void DisableTextWrapping()	{ document_.DisableTextWrapping(); }
			/** Draws this text at the given position. */
			void Draw(const CoordScalar& xpos, const CoordScalar& ypos) {
				Rect rect = { xpos, ypos, SizeScalar(-1), SizeScalar(-1) };
				brush_.Draw(rect, document_, 0, xpos, ypos, true);
			}
		private:
			const DocumentBrush&	brush_;
			Document				document_;
		};
	}
}

#endif
