// windowscrollcontroller.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_WindowScrollController_H_
#define Starscape_Gui_WindowScrollController_H_

#include "gui/control/scroll/scrollcontroller.h"
#include "gui/window.h"
#include <boost/bind.hpp>

namespace Starscape {

	namespace Gui {

		template <class WindowType>
		struct VerticalWindowScrollController : public ScrollController {
			typedef VerticalWindowScrollController<WindowType>	ThisType;
			typedef ScrollController::RatioType	RatioType;
			VerticalWindowScrollController(WindowType& window)
				: ScrollController(boost::bind(&ThisType::ScrollUp, this)
								,boost::bind(&ThisType::ScrollDown, this)
								,boost::bind(&ThisType::MoveToPos, this, _1)
								,boost::bind(&ThisType::GetPos, this))
				,target_(window)	{}
			void ScrollUp() {
				int offset = target_.OffsetY()-target_.OffsetYInterval();
				if (offset < 0)
					offset = 0;
				target_.SetOffsetY(offset);
			}
			void ScrollDown() {
				int offset = target_.OffsetY()+target_.OffsetYInterval();
				if (offset > target_.OffsetYTotal())
					offset = target_.OffsetYTotal();
				target_.SetOffsetY(offset);
			}
			void MoveToPos(const RatioType& ratio) {
				int offset = (target_.OffsetYTotal()
						-static_cast<int>(ratio*target_.OffsetYTotal()));
				if (ratio > 0.0)
					offset -= offset % target_.OffsetYInterval();
				target_.SetOffsetY(offset);
			}
			RatioType GetPos() {
				return (1.0 - static_cast<RatioType>(target_.OffsetY())
							/target_.OffsetYTotal());
			}
		private:
			WindowType&	target_;
		};

		template <class WindowType>
		struct HorizontalWindowScrollController : public ScrollController {
			typedef HorizontalWindowScrollController<WindowType>	ThisType;
			typedef ScrollController::RatioType	RatioType;
			HorizontalWindowScrollController(WindowType& window)
				: ScrollController(boost::bind(&ThisType::ScrollRight, this)
								,boost::bind(&ThisType::ScrollLeft, this)
								,boost::bind(&ThisType::MoveToPos, this, _1)
								,boost::bind(&ThisType::GetPos, this))
				,target_(window)	{}
			void ScrollRight() {
				int offset = target_.OffsetX()+target_.OffsetXInterval();
				if (offset > target_.OffsetXTotal())
					offset = target_.OffsetXTotal();
				target_.SetOffsetX(offset);
			}
			void ScrollLeft() {
				int offset = target_.OffsetX()-target_.OffsetXInterval();
				if (offset < 0)
					offset = 0;
				target_.SetOffsetX(offset);
			}
			void MoveToPos(const RatioType& ratio) {
				int offset = static_cast<int>(ratio*target_.OffsetXTotal());
				if (ratio < 1.0)
					offset -= offset % target_.OffsetXInterval();
				target_.SetOffsetX(offset);
			}
			RatioType GetPos() {
				return (static_cast<RatioType>(target_.OffsetX())
							/target_.OffsetXTotal());
			}
		private:
			WindowType&	target_;
		};

		template <class WindowType>
		VerticalWindowScrollController<WindowType>
		MakeVerticalWindowScrollController(WindowType& window) {
			return VerticalWindowScrollController<WindowType>(window);
		}

		template <class WindowType>
		HorizontalWindowScrollController<WindowType>
		MakeHorizontalWindowScrollController(WindowType& window) {
			return HorizontalWindowScrollController<WindowType>(window);
		}
	}
}

#endif
