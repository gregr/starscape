// slidertrackbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_SliderTrackBrush_H_
#define Starscape_Gui_SliderTrackBrush_H_

#include "gui/appearance/scroll/sliderbrush.h"
#include "gui/config/drawrect.h"
#include "gui/config/image.h"
#include "gui/config/color.h"

namespace Starscape {

	namespace Gui {

		/** Displays a SliderTrack and contains a brush for its slider. */
		class SliderTrackBrush {
		public:
			SliderTrackBrush(const SliderBrush& sliderBrush
							,const Graphics::Color& color)
				: color_(color), sliderBrush_(sliderBrush)	{}
			void Draw(const Rect& rect) const {
				Graphics::UnbindTextures();
				color_.MakeCurrent();
				DrawRect(rect);
			}
			const SliderBrush& GetSliderBrush() const {
				return sliderBrush_;
			}
			Size GetPreferredSize() const {
				return GetSliderBrush().GetPreferredSize();
			}
		private:
			Graphics::Color		color_;
			const SliderBrush&	sliderBrush_;
		};
	}
}

#endif
