// surface.h
// Gregory Rosenblatt
// 5/11/05

#ifndef Starscape_Raster_Surface_H_
#define Starscape_Raster_Surface_H_

#include "SDL.h"
#include <cassert>

namespace Starscape {

	namespace Raster {

		typedef SDL_Color	SurfaceColor;

		/** Provides the main interface for manipulating SDL surfaces.
		 * This class cannot be constructed directly.
		 * @sa ManagedSurface.
		 */
		class Surface {
			// non-copyable
			Surface(const Surface&);
			void operator=(const Surface&);
		public:
			/** Blits this surface's image to the destination surface.
			 * The final blit region is saved in dstRect after clipping.
			 * @param dest The destination Surface.
			 * @param dstRect The position to blit to.  Size is ignored.
			 * @return 0 on success, otherwise -1.
			 */
			int BlitTo(const Surface& dest, SDL_Rect& dstRect) const {
				assert(IsValid());
				return SDL_BlitSurface(surface_, 0, dest.surface_, &dstRect);
			}
			/** Blits a sub-image of this surface to the destination surface.
			 * The final blit region is saved in dstRect after clipping.
			 * @param srcRect The rectangular region to blit from.
			 * @param dest The destination Surface.
			 * @param dstRect The position to blit to.  Size is ignored.
			 * @return 0 on success, otherwise -1.
			 */
			int BlitRectTo(SDL_Rect& srcRect, const Surface& dest
						,SDL_Rect& dstRect) const {
				assert(IsValid());
				return SDL_BlitSurface(surface_, &srcRect
									,dest.surface_, &dstRect);
			}
			/** Fills a rectangular region on this surface with a solid color.
			 * The actual filled region is saved in dstRect after clipping.
			 * @param rect Rectangular area to fill.
			 * @param color The color to fill the rectangle with.
			 * @return 0 on success, otherwise -1.
			 */
			int FillRect(SDL_Rect& rect, Uint32 color) const {
				assert(IsValid());
				return SDL_FillRect(surface_, &rect, color);
			}
			/** Sets the clipping rectangle for blits to this surface. */
			void SetClipRect(SDL_Rect& rect) const {
				assert(IsValid());
				SDL_SetClipRect(surface_, &rect);
			}
			/** Removes this surface's current clipping rectangle. */
			void UnsetClipRect() const {
				assert(IsValid());
				SDL_SetClipRect(surface_, 0);
			}
			/** Creates a format-dependent color index from RGB values. */
			Uint32 MakeColor(Uint8 r, Uint8 g, Uint8 b) const {
				assert(IsValid());
				return SDL_MapRGB(surface_->format, r, g, b);
			}
			/** Sets the transparent color for this surface.
			 * @return 0 on success, otherwise -1.
			 */
			int SetColorKey(Uint32 color) const {
				assert(IsValid());
				return SDL_SetColorKey(surface_, SDL_SRCCOLORKEY, color);
			}
			/** Removes any color key present for this surface.
			 * @return 0 on success, otherwise -1.
			 */
			int RemoveColorKey() const {
				assert(IsValid());
				return SDL_SetColorKey(surface_, 0, 0);
			}
			/** Sets the alpha-related flags and alpha blending value.
			 * @param alphaFlags The alpha blending flags.
			 * @param alpha The alpha blending value.
			 * @return 0 on success, otherwise -1.
			 */
			int SetAlpha(Uint32 alphaFlags, Uint8 alpha) const {
				assert(IsValid());
				return SDL_SetAlpha(surface_, alphaFlags, alpha);
			}
			/** Retrieves any alpha-related flags that are currently set. */
			Uint32 GetAlphaFlags() const {
				assert(IsValid());
				return (surface_->flags&(SDL_SRCALPHA|SDL_RLEACCELOK));
			}
			/** Retrieves the current alpha value being used for blending. */
			Uint8 GetAlphaVal() const {
				assert(IsValid());
				return (surface_->format->alpha);
			}
			/** Fills the entire surface with a solid color.
			 * @param color 32-bit format-dependent color index.
			 * @return 0 on success, otherwise -1.
			 */
			int Clear(Uint32 color = 0) const {
				assert(IsValid());
				return SDL_FillRect(surface_, 0, color);
			}
			/** Allocates space for this surface to store original data.
			 * The data created is a default 32-bit software surface.
			 * Previous data is destroyed before a new surface is created.
			 * @param width The width of the surface data to generate.
			 * @param height The height of the surface data to generate.
			 * @return true on success, false if there was an error.
			 */
			bool CreateNewData(unsigned int width, unsigned int height) {
				Destroy();
				surface_ = SDL_CreateRGBSurface(
							SDL_SWSURFACE
							,width, height
							,32
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
							,0xff000000
							,0x00ff0000
							,0x0000ff00
							,0x000000ff
#else
							,0x000000ff
							,0x0000ff00
							,0x00ff0000
							,0xff000000
#endif
							);
				return IsValid();
			}
			/** Optimizes the surface to match the format of the display.
			 * @return true on success, false if there was an error.
			 */
			bool FormatToDisplay() {
				assert(IsValid());
				SDL_Surface* surf = SDL_DisplayFormat(surface_);
				if (surf==0)
					return false;
				Destroy();
				surface_ = surf;
				return true;
			}
			/** Determines if this is a valid surface to be used. */
			bool IsValid() const	{ return (surface_!=0); }
			/** The surface width. */
			int Width() const	{ assert(IsValid()); return surface_->w; }
			/** The surface height. */
			int Height() const	{ assert(IsValid()); return surface_->h; }
			/** The raw data stored by the surface. */
			const void* GetPixelData() const	{ return surface_->pixels; }
			/** Saves the surface as a bitmap image.
			 * @param fileName The name of the file to create.
			 * @return 0 on success, -1 if there was an error.
			 */
			int SaveAsBitmap(const char* fileName) const {
				return SDL_SaveBMP(surface_, fileName);
			}
		protected:
			// only constructed within derived classes
			Surface() : surface_(0)	{}
			explicit Surface(SDL_Surface* surface) : surface_(surface)	{}
			// procedure for freeing a surface
			void Destroy() const {
				if (IsValid())
					SDL_FreeSurface(surface_);
			}
			SDL_Surface*	surface_;
		};

		/** Fills a rectangular region on this surface with a solid color.
		 * The dimensions actually filled are saved in dstRect after clipping.
		 * @param surface The surface to fill on.
		 * @param rect Rectangular area to fill.
		 * @param color Format-independent color to fill the rectangle with.
		 * @return 0 on success, otherwise -1.
		 */
		inline int FillRect(const Surface& surface, SDL_Rect& rect
							,const SDL_Color& color) {
			return surface.FillRect(rect
								,surface.MakeColor(color.r, color.g, color.b));
		}

		/** Creates a surface dependent color index from RGB color.
		 * @param surface The surface for mapping the color index.
		 * @param color Format independent RGB color.
		 * @return A 32-bit surface dependent color index.
		 */
		inline Uint32 MakeColor(const Surface& surface
							,const SDL_Color& color) {
			return surface.MakeColor(color.r, color.g, color.b);
		}

		/** Sets the transparent color for this surface.
		 * @param surface The surface that will have its color key set.
		 * @param color Format-independent color to use as transparent.
		 * @return 0 on success, otherwise -1.
		 */
		inline int SetColorKey(const Surface& surface
							,const SDL_Color& color) {
			return surface.SetColorKey(MakeColor(surface, color));
		}

		/** Fills the entire surface with a solid color.
		 * @param surface The surface to clear.
		 * @param color Format independent RGB color description to clear with.
		 * @return 0 on success, otherwise -1.
		 */
		inline int Clear(const Surface& surface, const SDL_Color& color) {
			return surface.Clear(surface.MakeColor(color.r, color.g, color.b));
		}

		/** Constructs a surface color using streamed data. */
		template <class StreamType>
		SDL_Color MakeSurfaceColorFromStream(StreamType& in) {
			unsigned short r, g, b;
			in >> r >> g >> b;
			SDL_Color color = { r, g, b, 0 };
			return color;
		}
	}
}

#endif
