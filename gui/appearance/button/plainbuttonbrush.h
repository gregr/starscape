// plainbuttonbrush.h
// Gregory Rosenblatt
// 5/17/05

#ifndef Starscape_Gui_PlainButtonBrush_H_
#define Starscape_Gui_PlainButtonBrush_H_

#include "gui/appearance/button/buttonbrush.h"
#include "gui/config/drawrect.h"

namespace Starscape {

	namespace Gui {

		class PlainButtonBrush : public ButtonBrush {
		public:
			PlainButtonBrush(const Graphics::Color& unpressedColor
							,const Graphics::Color& pressedColor
							,const Graphics::Color& hoverColor)
				: unpressedColor_(unpressedColor)
				,pressedColor_(pressedColor)
				,hoverColor_(hoverColor)	{}
			void DrawUnpressed(const Rect& region
							,const char* caption) const {
				DrawArea(region, unpressedColor_);
			}
			void DrawPressed(const Rect& region
							,const char* caption) const {
				DrawArea(region, pressedColor_);
			}
			void DrawHover(const Rect& region
						,const char* caption) const {
				DrawArea(region, hoverColor_);
			}
		private:
			void DrawArea(const Rect& region
						,const Graphics::Color& color) const {
				Graphics::UnbindTextures();
				color.MakeCurrent();
				DrawRect(region);
			}
		private:
			Graphics::Color	unpressedColor_, pressedColor_, hoverColor_;
		};
	}
}

#endif
