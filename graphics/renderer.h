// renderer.h
// Gregory Rosenblatt
// 4/10/05

#ifndef Starscape_Graphics_Renderer_H_
#define Starscape_Graphics_Renderer_H_

#include "graphics/font/imagefontmanager.h"
#include "graphics/image/imagemanager.h"
#include "graphics/basic/texturemanager.h"
#include "graphics/basic/color.h"
#include "graphics/state/clipper.h"
#include "graphics/state/renderstate.h"
#include "graphics/display.h"
#include "log/log.h"

namespace Starscape {

	namespace Graphics {

		const char* const ERR_LOGRENDERSTATUS =	"Renderer: An OpenGL error has occurred.";

		/** Checks for any rendering errors and logs them as necessary. */
		inline void LogRenderStatus() {
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) {
				Log::Message(ERR_LOGRENDERSTATUS);
				Log::Message(gluErrorString(error));
			}
		}

		/** Prepares the next frame for rendering.
		 * Clears the display and resets the model-view matrix.
		 */
		inline void BeginRender() {
			assert(!glGetError());	// start using this
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
			assert(!glGetError());
		}

		/** Updates the display to reflect the rendered frame. */
		inline void EndRender() {
			assert(!glGetError());
			SwapOpenGLBuffers();
//			LogRenderStatus();
			assert(!glGetError());
		}

		/** The interface used by visual objects to draw themselves to the display. */
		class Renderer {
		public:
			Renderer(const DisplayMetrics& metrics)
				: displayMetrics_(metrics)
				,clipper_(MakeRect(0, 0, displayMetrics_.Width()
							,displayMetrics_.Height()))
				{ ResizeViewport(); }
			/** Makes internal updates in the event of the display being resized. */
			void OnResize() {
				ResizeViewport();
				clipper_.ResizeDisplayRegion(MakeRect(0, 0, displayMetrics_.Width()
												,displayMetrics_.Height()));
				ReacquireResources();
			}
			/** References the dimensions of the display.
			 * @return A reference to the system's current display metrics.
			 */
			const DisplayMetrics& GetDisplayMetrics() const	{ return displayMetrics_; }
			/** Provides access to the Clipper object for this renderer.
			 * @return A reference to the Clipper that can be used to construct a ClipRegion.
			 */
			Clipper& GetClipper()	{ return clipper_; }
			/** Retrieves a particular font resource.
			 * @param fileName The name of the file containing the font.
			 * @param size The point size to use.
			 * @return A handle to the font resource.
			 */
			FontHandle GetFont(const char* fileName, int size) {
				return fontManager_.Get(FontManager::KeyType(
									std::make_pair(PooledString(fileName), size)));
			}
			/** Retrieves an ImageFont resource.
			 * @param font The font to use for this ImageFont.
			 * @param quality The quality of the ImageFont to create.
			 * @return A handle to the font resource.
			 */
			ImageFontHandle GetImageFont(const FontHandle& font
										,const ImageFont::QualityFlag& quality) {
				return imageFontManager_.Get(ImageFontManager::KeyType(font, quality));
			}
			/** Retrieves an image-surface resource.
			 * @param fileName The name of the file containing the image.
			 * @return A handle to the image resource.
			 */
			ImageHandle GetImage(const char* fileName
								,bool colorKeyUsed, const SDL_Color& colorKey) {
				return imageManager_.Get(ImageManager::KeyType(fileName
														,colorKeyUsed, colorKey));
			}
			/** Retrieves a texture resource.
			 * @param fileName The name of the file containing the texture data.
			 * @return A handle to the texture resource.
			 */
			TextureHandle GetTexture(const char* fileName) {
				return textureManager_.Get(fileName);
			}
		private:
			void ResizeViewport() {
				assert(!glGetError());
				// states
//				EnableCulling();
				glShadeModel(GL_SMOOTH);
				glEnable(GL_TEXTURE_2D);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				EnableBlending();
				// clearing
				glClearDepth(1.0f);
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glViewport(0, 0, static_cast<GLsizei>(GetDisplayMetrics().Width())
								,static_cast<GLsizei>(GetDisplayMetrics().Height()));
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0.0, static_cast<GLdouble>(GetDisplayMetrics().Width())
							,static_cast<GLdouble>(GetDisplayMetrics().Height()), 0.0
							,0.0, 1.0);
				glMatrixMode(GL_MODELVIEW);	// default matrix mode
				assert(!glGetError());
			}
			void ReacquireResources() {
				// release all resources first
				imageFontManager_.ReleaseAll();
				imageManager_.ReleaseAll();
				textureManager_.ReleaseAll();
				// then reacquire
				imageFontManager_.ReacquireAll();
				imageManager_.ReacquireAll();
				textureManager_.ReacquireAll();
			}
			// non-copyable
			Renderer(const Renderer&);
			void operator=(const Renderer&);
			FontManager			fontManager_;
			ImageFontManager	imageFontManager_;
			ImageManager		imageManager_;
			TextureManager		textureManager_;
			DisplayMetrics		displayMetrics_;
			Clipper				clipper_;
		};
	}
}

#endif
