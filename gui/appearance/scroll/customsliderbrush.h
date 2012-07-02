// customsliderbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_CustomSliderBrush_H_
#define Starscape_Gui_CustomSliderBrush_H_

#include "gui/appearance/scroll/sliderbrush.h"
#include "gui/appearance/beveledimage.h"

namespace Starscape {

	namespace Gui {

		/** Displays as a plain beveled slider. */
		class PlainSliderBrush : public SliderBrush {
		public:
			PlainSliderBrush(const Graphics::Color& color
							,const SizeScalar& width = DEFAULT_SIZE
							,const SizeScalar& height = DEFAULT_SIZE)
				: color_(color), width_(width), height_(height) {}
			void Draw(const Rect& rect) const {
				Graphics::UnbindTextures();
				DrawBeveledPanel(rect, color_);
			}
			Size GetPreferredSize() const {
				return Size(width_, height_);
			}
		private:
			BevelBrush	color_;
			SizeScalar	width_, height_;
		};

		/** Displays a slider as an image. */
		class ImageSliderBrush : public SliderBrush {
		public:
			ImageSliderBrush(const StaticImage& image)
				: image_(image)	{}
			void Draw(const Rect& rect) const {
				Graphics::ResetColor();
				image_.Draw(rect);
			}
			Size GetPreferredSize() const {
				return Size(image_.Width(), image_.Height());
			}
		private:
			StaticImage	image_;
		};

		/** Displays a beveled slider with an image drawn on top. */
		class BeveledImageSliderBrush : public SliderBrush {
		public:
			BeveledImageSliderBrush(const BeveledImage& image)
				: image_(image)	{}
			void Draw(const Rect& rect) const {
				Graphics::UnbindTextures();
				image_.Draw(rect);
			}
			Size GetPreferredSize() const {
				return Size(image_.Width(), image_.Height());
			}
		private:
			BeveledImage	image_;
		};
	}
}

#endif
