// panelbuttonbrush.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_Gui_PanelButtonBrush_H_
#define Starscape_Gui_PanelButtonBrush_H_

#include "gui/appearance/button/beveledbuttonbrush.h"

namespace Starscape {

	namespace Gui {

		/** Plain, but drawn as a bevel when the mouse is hovering. */
		class PanelButtonBrush : public BeveledButtonBrush {
		public:
			PanelButtonBrush(const Graphics::Color& color)
				: BeveledButtonBrush(color)	{}
			void DrawUnpressed(const Rect& region
							,const char* caption) const {
				Graphics::UnbindTextures();
				bevelColor_.color.MakeCurrent();
				DrawRect(region);
			}
		};
	}
}

#endif
