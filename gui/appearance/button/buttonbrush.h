// buttonbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_ButtonBrush_H_
#define Starscape_Gui_ButtonBrush_H_

#include "util/size.h"
#include "util/coord.h"

namespace Starscape {

	namespace Gui {

		class ButtonBrush {
			enum { DEFAULT_SIZE = 20 };
		public:
			virtual ~ButtonBrush()	{}
			void Draw(const Rect& region, const char* caption
					,bool pressed, bool hovering) const {
				if (pressed)
					DrawPressed(region, caption);
				else if (hovering)
					DrawHover(region, caption);
				else
					DrawUnpressed(region, caption);
			}
			virtual Size GetPreferredSize(const char* caption) const {
				return Size(DEFAULT_SIZE, DEFAULT_SIZE);
			}
			virtual Coord GetCaptionOffset() const {
				return Coord();
			}
			virtual void DrawUnpressed(const Rect& region
									,const char* caption) const = 0;
			virtual void DrawPressed(const Rect& region
									,const char* caption) const = 0;
			virtual void DrawHover(const Rect& region
									,const char* caption) const = 0;
		};
	}
}

#endif
