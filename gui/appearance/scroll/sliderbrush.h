// sliderbrush.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Gui_SliderBrush_H_
#define Starscape_Gui_SliderBrush_H_

#include "util/size.h"

namespace Starscape {

	namespace Gui {

		/** Base class brush for displaying a slider. */
		class SliderBrush {
		public:
			enum { DEFAULT_SIZE = 20 };
			virtual ~SliderBrush()	{}
			virtual void Draw(const Rect& rect) const = 0;
			/** Preferred size in coord form. */
			virtual Size GetPreferredSize() const {
				return Size(DEFAULT_SIZE, DEFAULT_SIZE);
			}
		};
	}
}

#endif
