// scrollbarbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_ScrollBarBrush_H_
#define Starscape_Gui_ScrollBarBrush_H_

#include "gui/appearance/scroll/slidertrackbrush.h"
#include "gui/appearance/button/buttonbrush.h"

namespace Starscape {

	namespace Gui {

		/** Contains the brushes for the components of a scrollbar. */
		class ScrollBarBrush {
		public:
			ScrollBarBrush(const SliderTrackBrush& trackBrush
						,const ButtonBrush& incButtonBrush
						,const ButtonBrush& decButtonBrush)
				: trackBrush_(trackBrush)
				,incButtonBrush_(incButtonBrush)
				,decButtonBrush_(decButtonBrush)	{}
			const SliderTrackBrush& GetSliderTrackBrush() const {
				return trackBrush_;
			}
			const ButtonBrush& GetIncButtonBrush() const {
				return incButtonBrush_;
			}
			const ButtonBrush& GetDecButtonBrush() const {
				return decButtonBrush_;
			}
		private:
			const SliderTrackBrush&	trackBrush_;
			const ButtonBrush&		incButtonBrush_;
			const ButtonBrush&		decButtonBrush_;
		};
	}
}

#endif
