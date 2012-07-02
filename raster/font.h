// font.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Raster_Font_H_
#define Starscape_Raster_Font_H_

#include "raster/managedsurface.h"
#include "exception.h"
#include "SDL_ttf.h"

namespace Starscape {

	namespace Raster {

		const char* const ERR_FONT_OPENFONT =	"Font: Could not open font.";

		const char* const ERR_FONT_GETGLYPHATTRIB =
			"Font: Could not retrieve glyph attribute.";

		/** Contains the drawing attributes of a particular font glyph. */
		struct GlyphAttrib {
			int minx, maxx
				,miny, maxy
				,advance;
		};

		inline unsigned int GlyphWidth(const GlyphAttrib& glyph) {
			return glyph.maxx-glyph.minx;
		}

		inline unsigned int GlyphHeight(const GlyphAttrib& glyph) {
			return glyph.maxy-glyph.miny;
		}

		/** Provides the interface for manipulating SDL true-type fonts.
		 * Objects of this class are intended to be unique.
		 */
		class Font {
		public:
			/** Allocates a true-type font.
			 * @param name The file name of the font to be loaded.
			 * @param size The point size to be used with this font.
			 */
			Font(const char* name, int size)
				: font_(TTF_OpenFont(name, size))
			{
				if (!IsValid())
					throw Exception(ERR_FONT_OPENFONT, TTF_GetError());
			}
			// deallocates the true-type font
			~Font()	{ Close(); }
			/** Creates a surface containing a solid glyph.
			 * @param glyph The glyph to be drawn.
			 * @param fg The color to draw the text in.
			 * @return The resulting surface.
			 */
			ManagedSurface CreateSolidGlyphSurface(Uint16 glyph
												,const SDL_Color& fg) const {
				assert(IsValid());
				return ManagedSurface(TTF_RenderGlyph_Solid(font_, glyph, fg));
			}
			/** Creates a surface containing a shaded glyph.
			 * @param glyph The glyph to be drawn.
			 * @param fg The color to draw the text in.
			 * @param bg The background color to draw text on.
			 * @return The resulting surface.
			 */
			ManagedSurface CreateShadedGlyphSurface(Uint16 glyph
												,const SDL_Color& fg
												,const SDL_Color& bg) const {
				assert(IsValid());
				return ManagedSurface(TTF_RenderGlyph_Shaded(font_, glyph
															,fg, bg));
			}
			/** Creates a surface containing a blended glyph.
			 * @param glyph The glyph to be drawn.
			 * @param fg The color to draw the text in.
			 * @return The resulting surface.
			 */
			ManagedSurface CreateBlendedGlyphSurface(Uint16 glyph
												,const SDL_Color& fg) const {
				assert(IsValid());
				return ManagedSurface(TTF_RenderGlyph_Blended(font_
															,glyph, fg));
			}
			/** Creates a surface containing solid text.
			 * @param text The text-string to be drawn.
			 * @param fg The color to draw the text in.
			 * @return The resulting surface.
			 */
			ManagedSurface CreateSolidTextSurface(const char* text
												,const SDL_Color& fg) const {
				assert(IsValid());
				return ManagedSurface(TTF_RenderText_Solid(font_, text, fg));
			}
			/** Creates a surface containing shaded text.
			 * @param text The text-string to be drawn.
			 * @param fg The color to draw the text in.
			 * @param bg The background color to draw text on.
			 * @return The resulting surface.
			 */
			ManagedSurface CreateShadedTextSurface(const char* text
												,const SDL_Color& fg
												,const SDL_Color& bg) const {
				assert(IsValid());
				return ManagedSurface(TTF_RenderText_Shaded(font_, text
															,fg, bg));
			}
			/** Creates a surface containing blended text.
			 * @param text The text-string to be drawn.
			 * @param fg The color to draw the text in.
			 * @return The resulting surface.
			 */
			ManagedSurface CreateBlendedTextSurface(const char* text
												,const SDL_Color& fg) const {
				assert(IsValid());
				return ManagedSurface(TTF_RenderText_Blended(font_, text, fg));
			}
			/** Returns 1 if this is a fixed-width font, otherwise 0. */
			int IsFixedWidth() const {
				assert(IsValid());
				return TTF_FontFaceIsFixedWidth(font_);
			}
			/** The height of a line of text drawn with this font. */
			int GetHeight() const {
				assert(IsValid());
				return TTF_FontHeight(font_);
			}
			/** The proper vertical offset to use for multiple lines. */
			int GetLineSkip() const {
				assert(IsValid());
				return TTF_FontLineSkip(font_);
			}
			/** The width of a line of text drawn with this font. */
			int GetTextWidth(const char* str) const {
				assert(IsValid());
				int width, height;
				TTF_SizeText(font_, str, &width, &height);
				return width;
			}
			/** Retrives the attributes of a single character of this font.
			 * @param attrib The attrib struct to fill with info.
			 * @param ch The character glyph to assess.
			 */
			void GetGlyphAttrib(GlyphAttrib& attrib, Uint16 ch) const {
				if (0 != TTF_GlyphMetrics(font_, ch, &(attrib.minx)
									,&(attrib.maxx), &(attrib.miny)
									,&(attrib.maxy), &(attrib.advance)))
					throw Exception(ERR_FONT_GETGLYPHATTRIB, TTF_GetError());
			}
		private:
			// non-copyable
			Font(const Font&);
			void operator=(const Font&);
			// an object of this class should always be valid
			bool IsValid() const	{ return (font_!=0); }
			// close the font
			void Close() { assert(IsValid()); TTF_CloseFont(font_); }
			TTF_Font*	font_;
		};
	}
}

#endif
