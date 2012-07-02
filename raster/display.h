// display.h
// Gregory Rosenblatt
// 5/8/05

#ifndef Starscape_Raster_Display_H_
#define Starscape_Raster_Display_H_

#include "raster/surface.h"
#include "exception.h"
#include "SDL_opengl.h"

namespace Starscape {

	namespace Raster {

		const char* const ERR_DISPLAYSURFACE_SETVIDEOMODE =
			"DisplaySurface: Unable to set specified video mode.";
		const char* const ERR_DISPLAY_REPEATEDOPEN =
			"Display: An attempt was made to open a second video display.";
		const char* const ERR_DISPLAY_COLORDEPTH =
			"Display: Unable to provide the requested color bits.";
		const char* const ERR_DISPLAY_DEPTHBUFFER =
			"Display: Unable to provide the requested depth buffer bits.";
		const char* const ERR_DISPLAY_DOUBLEBUFFER =
			"Display: Unable to enable double buffering.";
		const char* const ERR_DISPLAY_RETRIEVEVIDEOINFO =
			"Display: Unable to retrieve video info.";
		
		/** Implements the main SDL video surface.
		 * It includes support for clearing and double-buffering.
		 */
		class DisplaySurface : private Surface {
		public:
			/** Creates the video surface and opens the actual SDL window.
			 * If this fails, an exception is thrown.
			 * @param width The width of the surface and window.
			 * @param height The height of the surface and window.
			 * @param bpp The bit depth of the surface.
			 * @param flags Contains the surface properties to use.
			 */
			void SetVideoMode(unsigned int width, unsigned int height
							,int bpp, Uint32 flags) {
				if (!(surface_ = SDL_SetVideoMode(width, height, bpp, flags)))
					throw Exception(ERR_DISPLAYSURFACE_SETVIDEOMODE
									,SDL_GetError());
			}
		};

		inline void SetGLAttributes() {
			if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5)!=0)
				throw Exception(ERR_DISPLAY_COLORDEPTH, SDL_GetError());
			if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5)!=0)
				throw Exception(ERR_DISPLAY_COLORDEPTH, SDL_GetError());
			if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5)!=0)
				throw Exception(ERR_DISPLAY_COLORDEPTH, SDL_GetError());
			if (SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1)!=0)
				throw Exception(ERR_DISPLAY_COLORDEPTH, SDL_GetError());
			if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16)!=0)
				throw Exception(ERR_DISPLAY_DEPTHBUFFER, SDL_GetError());
			if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)!=0)
				throw Exception(ERR_DISPLAY_DOUBLEBUFFER, SDL_GetError());
		}

		/** Represents the video display device. */
		class Display {
		public:
			Display() : width_(0), height_(0), flags_(SDL_OPENGL)	{}
			/** Creates the display surface and opens the SDL Window.
			 * @param width The display width.
			 * @param height The display height.
			 */
			void Open(unsigned int width, unsigned int height) {
				// make sure a window is only opened once
				if (SDL_GetVideoSurface())	// should be null
					throw Exception(ERR_DISPLAY_REPEATEDOPEN);
				SetGLAttributes();
				Resize(width, height);
			}
			/** Resizes the display surface and SDL Window.
			 * @param width The new display width.
			 * @param height The new display height.
			 */
			void Resize(unsigned int width, unsigned int height) {
				width_ = width;
				height_ = height;
				screen_.SetVideoMode(width, height, 0, flags_);
			}
			/** Sets the fullscreen option in the creation flags. */
			void SetFullscreen()	{ flags_ |= SDL_FULLSCREEN; }
			/** Disables the fullscreen option in the creation flags. */
			void SetWindowed()		{ flags_ &= ~SDL_FULLSCREEN; }
			/** Sets the resizable option in the creation flags. */
			void SetResizable()		{ flags_ |= SDL_RESIZABLE; }
			/** Turns off any display creation options that were set. */
			void ResetFlags()		{ flags_ = SDL_OPENGL; }
			/** The display width. */
			unsigned int Width() const	{ return width_; }
			/** The display height. */
			unsigned int Height() const	{ return height_; }
		private:
			DisplaySurface	screen_;
			unsigned int	width_, height_;
			Uint32			flags_;
		};

		/** A handle used to query the current display properties. */
		class DisplayMetrics {
		public:
			DisplayMetrics(const Display& display) : display_(display)	{}
			/** The display width. */
			unsigned int Width() const	{ return display_.Width(); }
			/** The display height. */
			unsigned int Height() const	{ return display_.Height(); }
		private:
			const Display&	display_;
		};
	}
}
	
#endif
