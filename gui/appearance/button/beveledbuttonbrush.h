// beveledbuttonbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_BeveledButtonBrush_H_
#define Starscape_Gui_BeveledButtonBrush_H_

#include "gui/appearance/button/buttonbrush.h"
#include "gui/appearance/bevelbrush.h"

namespace Starscape {

	namespace Gui {

		class BeveledButtonBrush : public ButtonBrush {
			enum { SIZE_INDENT = 1 };
		public:
			BeveledButtonBrush(const Graphics::Color& bevelColor)
				: bevelColor_(bevelColor)	{}
			Coord GetCaptionOffset() const {
				return Coord(SIZE_INDENT, SIZE_INDENT);
			}
			void DrawUnpressed(const Rect& region
							,const char* caption) const {
				Graphics::UnbindTextures();
				DrawBeveledPanel(region, bevelColor_);
			}
			void DrawPressed(const Rect& region
							,const char* caption) const {
				Graphics::UnbindTextures();
				DrawBeveledPanelPressed(region, bevelColor_);
			}
			void DrawHover(const Rect& region
						,const char* caption) const {
				DrawUnpressed(region, caption);
			}
		protected:
			BevelBrush	bevelColor_;
		};
	}
}

#endif
