// imageregion.h
// Gregory Rosenblatt
// 5/9/05

#ifndef Starscape_Graphics_ImageRegion_H_
#define Starscape_Graphics_ImageRegion_H_

#include "graphics/imagetexture.h"
#include "util/scalar.h"

namespace Starscape {

	namespace Graphics {

		/** Represents a sub-image contained in an ImageTexture.
		 * Requires the parent ImageTexture to be bound before drawing.
		 */
		class ImageRegion {
		public:
			/** Creates an empty ImageTexture. */
			ImageRegion() : u1_(0), v1_(0), u2_(0), v2_(0) {
				region_.x = 0; region_.y = 0; region_.w = 0; region_.h = 0;
			}
			/** Creates an ImageRegion describing a region of the given image.
			 * @param image The source of image data.
			 * @param region The region of the image to describe.
			 */
			ImageRegion(const ImageTexture& image, const Rect& region)
				: region_(region)
				,u1_(region.x*image.WidthRatio())
				,v1_(region.y*image.HeightRatio())
				,u2_((region.x+region.w)*image.WidthRatio())
				,v2_((region.y+region.h)*image.HeightRatio())
			{}
			/** Creates an ImageRegion that encompasses an entire image.
			 * @param image The source of image data.
			 */
			explicit ImageRegion(const ImageTexture& image)
				: u1_(0), v1_(0)
				,u2_(image.TextureWidth()), v2_(image.TextureHeight())
			{
				region_.x = 0; region_.y = 0;
				region_.w = image.RectWidth(); region_.h = image.RectHeight();
			}
			/** Draws the image region stretched to fit the destination.
			 * Requires that the appropriate parent ImageTexture be bound.
			 * @param dstRect The destination region to draw on.
			 */
			void DrawStretched(const Rect& dstRect) const {
				glBegin(GL_QUADS);
				glTexCoord2f(u1_, v1_);
				glVertex2i(dstRect.x, dstRect.y);
				glTexCoord2f(u1_, v2_);
				glVertex2i(dstRect.x, dstRect.y+dstRect.h);
				glTexCoord2f(u2_, v2_);
				glVertex2i(dstRect.x+dstRect.w, dstRect.y+dstRect.h);
				glTexCoord2f(u2_, v1_);
				glVertex2i(dstRect.x+dstRect.w, dstRect.y);
				glEnd();
			}
			/** Draws the image region without stretching its size.
			 * Requires that the appropriate parent ImageTexture be bound.
			 * @param posRect The position to draw at.  Size is ignored.
			 */
			void Draw(const Rect& posRect) const {
				glBegin(GL_QUADS);
				glTexCoord2f(u1_, v1_);
				glVertex2i(posRect.x, posRect.y);
				glTexCoord2f(u1_, v2_);
				glVertex2i(posRect.x, posRect.y+Height());
				glTexCoord2f(u2_, v2_);
				glVertex2i(posRect.x+Width(), posRect.y+Height());
				glTexCoord2f(u2_, v1_);
				glVertex2i(posRect.x+Width(), posRect.y);
				glEnd();
			}
			/** The width of the region. */
			const SizeScalar& Width() const	{ return region_.w; }
			/** The height of the region. */
			const SizeScalar& Height() const	{ return region_.h; }
		private:
			Rect	region_;
			GLfloat	u1_, v1_, u2_, v2_;
		};
	}
}

#endif
