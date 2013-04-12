// scrollcontroller.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Gui_ScrollController_H_
#define Starscape_Gui_ScrollController_H_

#include <boost/function.hpp>

namespace Starscape {

	namespace Gui {

		// todo: remove conditional callbacks...?
		// assert the validity of commands in constructor?
		/** A set of callbacks that bind a scrollbar to a target. */
		struct ScrollController {
			typedef double RatioType;
			typedef boost::function<void ()>	AdjustFunc;
			typedef boost::function<void (const RatioType&)>	SlideFunc;
			typedef boost::function<RatioType ()>	RefreshFunc;
			ScrollController(const AdjustFunc& inc, const AdjustFunc& dec
					,const SlideFunc& slide, const RefreshFunc& refresh)
				: inc_(inc), dec_(dec), slide_(slide), refresh_(refresh)
			{}
			void Increment() const	{ if (inc_) inc_(); }
			void Decrement() const	{ if (dec_) dec_(); }
			void SlideToRatio(const RatioType& ratio) const {
				if (slide_) slide_(std::min(ratio, 1.0));
			}
			RatioType GetRefreshedRatio() const {
				if (refresh_)
					return refresh_();
				return 0.0;
			}
		private:
			AdjustFunc	inc_, dec_;
			SlideFunc	slide_;
			RefreshFunc	refresh_;
		};
	}
}

#endif
