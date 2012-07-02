// staticimage.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Gui_StaticImage_H_
#define Starscape_Gui_StaticImage_H_

#include "gui/config/image.h"

namespace Starscape {

	namespace Gui {

		/** An image component used in constructing controls. */
		class StaticImage {
		public:
			/** Creates the image using the given handle and region. */
			StaticImage(const Graphics::ImageHandle& image
					,const Graphics::ImageRegion& region)
				: image_(image), region_(region)	{}
			/** Draws the image at the given position with normal size. */
			void Draw(const Rect& pos) const {
				image_->Bind();
				region_.Draw(pos);
			}
			/** Draws the image to fit the given rect area. */
			void DrawStretched(const Rect& rect) const {
				image_->Bind();
				region_.DrawStretched(rect);
			}
			/** The normal width of the image. */
			const SizeScalar& Width() const	{ return region_.Width(); }
			/** The normal height of the image. */
			const SizeScalar& Height() const	{ return region_.Height(); }
		private:
			Graphics::ImageHandle	image_;
			Graphics::ImageRegion	region_;
		};
	}
}

#endif
