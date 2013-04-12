// imagebuttonbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_ImageButtonBrush_H_
#define Starscape_Gui_ImageButtonBrush_H_

#include "gui/appearance/button/buttonbrush.h"
#include "gui/appearance/staticimage.h"
#include "gui/config/color.h"

namespace Starscape {

	namespace Gui {

		class ImageButtonBrush : public ButtonBrush {
		public:
			ImageButtonBrush(const StaticImage& unpressed
							,const StaticImage& pressed
							,const StaticImage& hover)
				: imageUnpressed_(unpressed)
				,imagePressed_(pressed), imageHover_(hover)	{}
			Size GetPreferredSize(const char* caption) const {
				return Size(imageUnpressed_.Width()
							,imageUnpressed_.Height());
			}
			void DrawUnpressed(const Rect& region
							,const char* caption) const {
				Graphics::ResetColor();
				imageUnpressed_.Draw(region);
			}
			void DrawPressed(const Rect& region
							,const char* caption) const {
				Graphics::ResetColor();
				imagePressed_.Draw(region);
			}
			void DrawHover(const Rect& region
						,const char* caption) const {
				Graphics::ResetColor();
				imageHover_.Draw(region);
			}
			StaticImage	imageUnpressed_, imagePressed_, imageHover_;
		};
	}
}

#endif
