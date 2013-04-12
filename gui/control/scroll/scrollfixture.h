// scrollfixture.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_ScrollFixture_H_
#define Starscape_Gui_ScrollFixture_H_

#include "gui/control/scroll/scrollbar.h"
#include "gui/control/scroll/windowscrollcontroller.h"

namespace Starscape {

	namespace Gui {

		template <class ChildWindowType>
		class ScrollFixture : public Window {
		public:
			ScrollFixture(const ChildWindowType& window
						,const ScrollBarBrush& vBrush
						,const ScrollBarBrush& hBrush)
				: child_(window)
				,vController_(child_), hController_(child_)
				,vScroll_(vBrush, vController_)
				,hScroll_(hBrush, hController_)
				,enableVertical_(true)
				,enableHorizontal_(true)
			{
				AddChild(child_);
				AddChild(vScroll_);
				AddChild(hScroll_);
				SetSizeAroundChild();
				OnResize();
				ValidateScrollBars();
			}
			ChildWindowType& GetChild()	{ return child_; }
			const ChildWindowType& GetChild() const	{ return child_; }
			void OnResize() {
				ResizeChild();
				vScroll_.SetLength(Height());
				hScroll_.SetLength(ChildWidth());
				vScroll_.SetPos(ChildWidth(), 0);
				hScroll_.SetPos(0, ChildHeight());
				AlertStateChanged();
			}
			void OnChildStateChange(const Window& child) {
				if (&child == &child_)
					AlertStateChanged();
			}
		private:
			void OnRender()	{}
			void AlertStateChanged() {
				ValidateScrollBars();
				vScroll_.AlertStateChanged();
				hScroll_.AlertStateChanged();
			}
			void ValidateScrollBars() {
				if (enableVertical_) {
					if (child_.OffsetYTotal() <= 0)
						DisableVerticalScroll();
				}
				else if (child_.OffsetYTotal() > 0)
					EnableVerticalScroll();
				if (enableHorizontal_) {
					if (child_.OffsetXTotal() <= 0)
						DisableHorizontalScroll();
				}
				else if (child_.OffsetXTotal() > 0)
					EnableHorizontalScroll();
			}
			void EnableVerticalScroll() {
				enableVertical_ = true;
				vScroll_.Show();
				vScroll_.SetPickable();
				ResizeChild();
				hScroll_.SetLength(ChildWidth());
			}
			void EnableHorizontalScroll() {
				enableHorizontal_ = true;
				hScroll_.Show();
				hScroll_.SetPickable();
				ResizeChild();
			}
			void DisableVerticalScroll() {
				enableVertical_ = false;
				vScroll_.Hide();
				vScroll_.SetUnpickable();
				ResizeChild();
				hScroll_.SetLength(ChildWidth());
				child_.SetOffsetY(0);
			}
			void DisableHorizontalScroll() {
				enableHorizontal_ = false;
				hScroll_.Hide();
				hScroll_.SetUnpickable();
				ResizeChild();
				child_.SetOffsetX(0);
			}
			void ResizeChild() {
				child_.SetSize(ChildWidth(), ChildHeight());
				child_.OnResize();
			}
			void SetSizeAroundChild() {
				SizeScalar width = child_.Width();
				SizeScalar height = child_.Height();
				if (enableVertical_)
					width += vScroll_.Width();
				if (enableHorizontal_)
					height += hScroll_.Height();
				SetSize(width, height);
			}
			SizeScalar ChildWidth() const {
				if (enableVertical_)
					return (Width()-vScroll_.Width());
				return Width();
			}
			SizeScalar ChildHeight() const {
				if (enableHorizontal_)
					return (Height()-hScroll_.Height());
				return Height();
			}
			ChildWindowType	child_;
			VerticalWindowScrollController<ChildWindowType>		vController_;
			HorizontalWindowScrollController<ChildWindowType>	hController_;
			VScrollBar		vScroll_;
			HScrollBar		hScroll_;
			bool			enableVertical_, enableHorizontal_;
		};
	}
}

#endif
