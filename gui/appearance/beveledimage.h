// beveledimage.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Gui_BeveledImage_H_
#define Starscape_Gui_BeveledImage_H_

#include "gui/appearance/staticimage.h"
#include "gui/appearance/bevelbrush.h"

namespace Starscape {

	namespace Gui {

		/** A StaticImage drawn on top of a beveled panel. */
		class BeveledImage {
		public:
			BeveledImage(const StaticImage& image
						,const Graphics::Color& bevelColor)
				: image_(image)
				,bevelColor_(bevelColor)	{}
			/** Draws the image at the given position with normal size. */
			void Draw(const Rect& pos) const {
				DrawBeveledPanel(MakeRect(pos.x, pos.y, Width(), Height())
								,bevelColor_);
				Graphics::ResetColor();
				image_.Draw(pos);
			}
			/** Draws the image to fit the given rect area. */
			void DrawStretched(const Rect& rect) const {
				DrawBeveledPanel(rect, bevelColor_);
				Graphics::ResetColor();
				image_.DrawStretched(rect);
			}
			/** Draws the image at the given position with normal size. */
			void DrawPressed(const Rect& pos
							,int indent) const {
				DrawBeveledPanelPressed(MakeRect(pos.x, pos.y
										,Width(), Height())
										,bevelColor_);
				Graphics::ResetColor();
				image_.Draw(MakeRect(pos.x+indent, pos.y+indent
									,0, 0));
			}
			/** Draws the image to fit the given rect area. */
			void DrawPressedStretched(const Rect& rect
									,int indent) const {
				DrawBeveledPanelPressed(rect, bevelColor_);
				Graphics::ResetColor();
				image_.DrawStretched(MakeRect(rect.x+indent
									,rect.y+indent, rect.w, rect.h));
			}
			/** The normal width of the image. */
			const SizeScalar& Width() const	{ return image_.Width(); }
			/** The normal height of the image. */
			const SizeScalar& Height() const	{ return image_.Height(); }
		private:
			StaticImage	image_;
			BevelBrush	bevelColor_;
		};
	}
}

#endif
