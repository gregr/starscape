// textsurface.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Raster_TextSurface_H_
#define Starscape_Raster_TextSurface_H_

#include "raster/font.h"
#include "raster/transfersurfacedata.h"

namespace Starscape {

	namespace Raster {

		const char* const ERR_CREATEGLYPHSURFACE_GENERATE =
			"CreateGlyphSurface: Error generating surface.";

		const char* const ERR_CREATETEXTSURFACE_GENERATE =
			"CreateTextSurface: Error generating surface.";

		/** Creates a surface with a low-quality solid glyph.
		 * Surface background is transparent.
		 */
		struct SolidGlyphGenerator {
			ManagedSurface operator()(const Font& font, Uint16 glyph
									,const SurfaceColor& fg) const {
				return font.CreateSolidGlyphSurface(glyph, fg);
			}
		};

		/** Creates a surface with a high-quality glyph.
		 * Surface background is transparent.
		 */
		struct BlendedGlyphGenerator {
			ManagedSurface operator()(const Font& font, Uint16 glyph
									,const SurfaceColor& fg) const {
				return font.CreateBlendedGlyphSurface(glyph, fg);
			}
		};

		/** Creates a surface with low-quality solid text.
		 * Surface background is transparent.
		 */
		struct SolidTextGenerator {
			ManagedSurface operator()(const Font& font, const char* text
									,const SurfaceColor& fg) const {
				return font.CreateSolidTextSurface(text, fg);
			}
		};

		/** Creates a surface with high-quality text.
		 * Surface background is transparent.
		 */
		struct BlendedTextGenerator {
			ManagedSurface operator()(const Font& font, const char* text
									,const SurfaceColor& fg) const {
				return font.CreateBlendedTextSurface(text, fg);
			}
		};

		/** Returns the longest width of any string in the given list.
		 * @param font The font to use when determining text width.
		 * @param stringList The list of strings to compare.
		 */
		template <typename StringList>
		unsigned int GetLongestTextWidth(const Font& font
										,const StringList& stringList) {
			unsigned int maxWidth = 0;
			for (typename StringList::const_iterator itr = stringList.begin();
					itr != stringList.end(); ++itr) {
				unsigned int width = font.GetTextWidth(itr->c_str());
				if (width > maxWidth)
					maxWidth = width;
			}
			return maxWidth;
		}

		/** Creates a surface containing a glyph with the given properties. */
		template <typename TextGenFunc>
		ManagedSurface CreateGlyphSurface(const Uint16 glyph
										,const Font& font
										,const SurfaceColor& fg) {
			TextGenFunc generate;
			char buffer[] = " ";
			if (glyph != 0)
				buffer[0] = static_cast<char>(glyph);
			ManagedSurface surface = generate(font, buffer, fg);
			if (!surface.IsValid())
				throw Exception(ERR_CREATEGLYPHSURFACE_GENERATE
								,TTF_GetError());
			return surface;
		}

		/** Creates a surface containing text with given properties. */
		template <typename StringType, typename TextGenFunc>
		ManagedSurface CreateTextSurface(const StringType& text
							,const Font& font
							,const SurfaceColor& fg) {
			TextGenFunc generate;
			const char* str = " ";
			if (text != "")
				str = text.c_str();
			ManagedSurface surface = generate(font, str, fg);
			if (!surface.IsValid())
				throw Exception(ERR_CREATETEXTSURFACE_GENERATE
								,TTF_GetError());
			return surface;
		}

		/** Creates a surface containing multiple lines of text from a list. */
//		template <typename StringListType, typename TextGenFunc>
//		ManagedSurface CreateMultiTextSurface(
//										const StringListType& stringList
//										,const Font& font
//										,const SurfaceColor& fg
//										,const TextGenFunc& generate) {
//			unsigned int lineSkip = font.GetLineSkip();
//			unsigned int height = lineSkip * stringList.size();
//			unsigned int width = GetLongestTextWidth(font, stringList);
//			ManagedSurface finalSurface = CreateNewSurface(width, height);
//			SDL_Rect pos = { 0, 0, 0, 0 };
//			typename StringListType::const_iterator itr = stringList.begin()
//													,end = stringList.end();
//			for (; itr != end; ++itr) {
//				ManagedSurface surface = CreateTextSurface(*itr, font
//														,fg, generate);
//				TransferSurfaceData(surface, finalSurface, pos);
//				pos.y += lineSkip;
//			}
//			return finalSurface;
//		}
	}
}

#endif
