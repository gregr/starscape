// scrollbar.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_Scrollbar_H_
#define Starscape_Gui_Scrollbar_H_

#include "gui/appearance/scroll/scrollbarbrush.h"
#include "gui/control/scroll/slidertrack.h"
#include "gui/control/button/pushbutton.h"
#include <boost/bind.hpp>

namespace Starscape {

	namespace Gui {

		/** A flexible scrollbar that interacts through a controller. */
		template <class OrientationType>
		class ScrollBar : public Window {
			typedef SliderTrack<OrientationType>	SliderTrackType;
		public:
			ScrollBar(const ScrollBarBrush& brush
					,const ScrollController& controller)
				: track_(brush.GetSliderTrackBrush(), controller)
				,incButton_(brush.GetIncButtonBrush()
					,boost::bind(&SliderTrackType::Increment, &track_))
				,decButton_(brush.GetDecButtonBrush()
					,boost::bind(&SliderTrackType::Decrement, &track_))
			{
				AddChild(track_);
				AddChild(incButton_);
				AddChild(decButton_);
				SetSize(track_.Width(), track_.Height());
			}
			void SetLength(const SizeScalar& length) {
				OrientationType::SetLength(*this, length);
				track_.SetLength(length
						-(OrientationType::GetLength(incButton_)
						+OrientationType::GetLength(decButton_)));
				OrientationType::SetLayout(track_
									,incButton_, decButton_);
			}
			void AlertStateChanged()	{ track_.AlertStateChanged(); }
		private:
			void OnRender()	{}
			SliderTrackType	track_;
			PushButton		incButton_, decButton_;
		};

		typedef ScrollBar<VerticalOrientation>		VScrollBar;
		typedef ScrollBar<HorizontalOrientation>	HScrollBar;
	}
}

#endif
