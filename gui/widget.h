// widget.h
// Gregory Rosenblatt
// 5/19/05

#ifndef Starscape_Gui_Widget_H_
#define Starscape_Gui_Widget_H_

#include "util/rect.h"

namespace Starscape {

	namespace Gui {

		/** An object drawn as part of the graphical user-interface. */
		class Widget {
		public:
			/** Constructs a Widget with optional dimensions. */
			Widget(const CoordScalar& xpos=0, const CoordScalar& ypos=0
					,const SizeScalar& width=0, const SizeScalar& height=0)
				: rect_(MakeRect(xpos, ypos, width, height))
				,renderingRect_(rect_), visible_(true)
			{}
			Widget(const Widget& w)
				: rect_(w.rect_), visible_(w.visible_)	{}
			Widget& operator=(const Widget& w) {
				rect_ = w.rect_;
				visible_ = w.visible_;
				return *this;
			}
			virtual ~Widget()	{}
			/** Makes the widget visible. */
			void Show()	{ visible_ = true; }
			/** Hides the widget from view. */
			void Hide()	{ visible_ = false; }
			/** The visibility of the widget. */
			bool IsVisible() const	{ return visible_; }
			/** Sets the position of the widget. */
			void SetPos(const CoordScalar& xpos, const CoordScalar& ypos) {
				SetRectPos(rect_, xpos, ypos);
			}
			/** Sets the size of the widget. */
			void SetSize(const SizeScalar& width, const SizeScalar& height) {
				SetRectSize(rect_, width, height);
			}
			/** Sets the x-position of the widget. */
			void SetPosX(const CoordScalar& xpos)	{ rect_.x = xpos; }
			/** Sets the y-position of the widget. */
			void SetPosY(const CoordScalar& ypos)	{ rect_.y = ypos; }
			/** Sets the width of the widget. */
			void SetWidth(const SizeScalar& width)	{ rect_.w = width; }
			/** Sets the height of the widget. */
			void SetHeight(const SizeScalar& height)	{ rect_.h = height; }
			/** The x-position of the widget. */
			const CoordScalar& PosX() const	{ return rect_.x; }
			/** The y-position of the widget. */
			const CoordScalar& PosY() const	{ return rect_.y; }
			/** The width of the widget. */
			const SizeScalar& Width() const	{ return rect_.w; }
			/** The height of the widget. */
			const SizeScalar& Height() const	{ return rect_.h; }
		protected:
			/** Draws the widget using the given renderer. */
			virtual void OnRender() = 0;
			/** Updates the rendering rect according to a global translation.
			 * A widget maintaining its position relative to another object
			 * would use that object's position as the translation coordinates.
			 */
			void UpdateRenderingRect(const CoordScalar& xtrans
									,const CoordScalar& ytrans) {
				renderingRect_ = MakeRect(xtrans+PosX(), ytrans+PosY()
										,Width(), Height());
			}
			/** The global dimensions to be used for rendering this widget. */
			const Rect& GetRenderingRect() const	{ return renderingRect_; }
			/** The global dimensions to be used for rendering this widget. */
			Rect& GetRenderingRect()	{ return renderingRect_; }
		private:
			Rect	rect_;			// local coordinates
			Rect	renderingRect_;	// global coordinates to be drawn at
			bool	visible_;
		};
	}
}

#endif
