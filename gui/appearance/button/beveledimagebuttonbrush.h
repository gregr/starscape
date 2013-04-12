// beveledimagebuttonbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_PlainIconButtonBrush_H_
#define Starscape_Gui_PlainIconButtonBrush_H_

#include "gui/appearance/button/buttonbrush.h"
#include "gui/appearance/beveledimage.h"

namespace Starscape {

	namespace Gui {

		/** A button brush that displays a beveled image. */
		class BeveledImageButtonBrush : public ButtonBrush {
			enum { SIZE_INDENT = 1 };
		public:
			BeveledImageButtonBrush(const BeveledImage& image)
				: image_(image)	{}
			Size GetPreferredSize(const char* caption) const {
				return Size(image_.Width(), image_.Height());
			}
			void DrawUnpressed(const Rect& region
							,const char* caption) const {
				Graphics::UnbindTextures();
				image_.Draw(region);
			}
			void DrawPressed(const Rect& region
							,const char* caption) const {
				Graphics::UnbindTextures();
				image_.DrawPressed(region, SIZE_INDENT);
			}
			void DrawHover(const Rect& region
						,const char* caption) const {
				DrawUnpressed(region, caption);
			}
		private:
			BeveledImage	image_;
		};
	}
}

#endif
