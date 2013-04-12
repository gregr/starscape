// appearancemanager.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Gui_AppearanceManager_H_
#define Starscape_Gui_AppearanceManager_H_

#include "gui/config/font.h"
#include "gui/config/image.h"

namespace Starscape {

	namespace Gui {

		class AppearanceManager {
		public:
			/** Retrieves a font resource.
			 * @param fileName The filename of the font to use.
			 * @param ptSize The point size of the font.
			 * @param quality The quality of the font to create.
			 * @return A handle to the font resource.
			 */
			Graphics::ImageFontHandle GetFont(
								const char* fileName, int ptSize
								,const Graphics::ImageFont::Quality& quality) {
				return imageFontManager_.Get(
						Graphics::ImageFontManager::KeyType(fileName, ptSize
															,quality));
			}
			/** Retrieves an image resource.
			 * @param fileName The name of the file containing the image.
			 * @param colorKeyUsed Whether to use the provided color key.
			 * @param colorKey The provided color key.
			 * @return A handle to the image resource.
			 */
			Graphics::ImageHandle GetImage(
								const char* fileName, bool colorKeyUsed
								,const Raster::SurfaceColor& colorKey) {
				return imageManager_.Get(
						Graphics::ImageManager::KeyType(fileName, colorKeyUsed
														,colorKey));
			}
			/** Reloads the resources if they are invalidated. */
			void ReacquireResources() {
				imageFontManager_.ReacquireAll();
				imageManager_.ReacquireAll();
			}
		private:
			Graphics::ImageFontManager	imageFontManager_;
			Graphics::ImageManager		imageManager_;
		};
	}
}

#endif
