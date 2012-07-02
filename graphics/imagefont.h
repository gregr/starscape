// imagefont.h
// Gregory Rosenblatt
// 5/12/05

#ifndef Starscape_Graphics_ImageFont_H_
#define Starscape_Graphics_ImageFont_H_

#include "graphics/imagetexture.h"
#include "graphics/config/fontdata.h"
#include "util/scalar.h"
#include <algorithm>
//#include <iostream>

#define STARSCAPE_IMAGEFONT_DUMPTOFILE	1
#define STARSCAPE_IMAGEFONT_DUMPFILENAME	"dumpedfont.bmp"

namespace Starscape {

	namespace Graphics {

		/** Allows immediate-mode rendering of text in a particular font.
		 * Maps the ascii characters of the given font to an image.
		 */
		class ImageFont {
			struct Glyph {
				Glyph() : tu1(0.0f), tu2(0.0f), tv1(0.0f), advance(0)	{}
				GLfloat tu1, tu2, tv1;
				unsigned int advance;
			};
		public:
			enum Quality { SOLID_TEXT, BLENDED_TEXT };
			/** Creates an ImageFont with the chosen quality.
			 * @param font The font to map.
			 * @param quality The rendering quality to use.
			 */
			ImageFont(const Raster::Font& font, const Quality& quality)
				: lineSkip_(font.GetLineSkip())
				,charHeight_(font.GetHeight())
				,image_(CreateMapWithQuality(quality, font))	{}
			/** Retrieves the width required to draw the given character. */
			SizeScalar GetCharWidth(char ch) const {
				return GetGlyph(ch).advance;
			}
			/** Retrieves the width required to draw the given text. */
			unsigned int GetTextWidth(const char* text
									,unsigned int numChars) const {
				unsigned int width = 0;
				const char* end = text+numChars;
//				try {
				for (const char* itr = text; itr != end; ++itr)
					width += GetCharWidth(*itr);
//				} catch (...) { std::cerr << "gettextwidth\n"; throw; }
				return width;
			}
			/** The vertical skip to use when drawing new lines. */
			const SizeScalar& GetLineSkip() const	{ return lineSkip_; }
			/** The maximum height of any character in this font. */
			const SizeScalar& GetCharHeight() const	{ return charHeight_; }
			/** The maximum width of characters drawn in this font. */
			const SizeScalar& GetMaxCharWidth() const {
				return maxCharWidth_;
			}
			/** Draws a single character at the given position.
			 * The desired color should be set before drawing.
			 */
			void DrawChar(char ch, int xpos, int ypos) const {
				Bind();
				glBegin(GL_QUADS);
				DrawGlyph(GetGlyph(ch), xpos, ypos);
				glEnd();
			}
			/** Draws a string of text at the given position.
			 * The desired color should be set before drawing.
			 */
			void DrawText(const char* text, unsigned int numChars
						,int xpos, int ypos) const {
//				try {
				Bind();
				glBegin(GL_QUADS);
				const char* end = text+numChars;
				for (const char* itr = text; itr != end; ++itr) {
					const Glyph& glyph = GetGlyph(*itr);
					DrawGlyph(glyph, xpos, ypos);
					xpos += glyph.advance;
				}
				glEnd();
//				} catch (...) { std::cerr << "drawtext\n"; throw; }
			}
			/** Sets this image as the current texture to draw with. */
			void Bind() const	{ image_.Bind(); }
			/** Releases the image data in preparation to reacquire it. */
			void Release()	{ image_.Release(); }
		private:
			enum { MAP_PITCH=16, GLYPH_MARGIN=2, MAX_GLYPHS=256 };
			void DrawGlyph(const Glyph& glyph, int xpos, int ypos) const {
				glTexCoord2f(glyph.tu1, glyph.tv1);
				glVertex2i(xpos, ypos);
				glTexCoord2f(glyph.tu1, glyph.tv1+th_);
				glVertex2i(xpos, ypos+charHeight_);
				glTexCoord2f(glyph.tu2, glyph.tv1+th_);
				glVertex2i(xpos+glyph.advance, ypos+charHeight_);
				glTexCoord2f(glyph.tu2, glyph.tv1);
				glVertex2i(xpos+glyph.advance, ypos);
			}
			const Glyph& GetGlyph(char index) const {
//				assert(static_cast<unsigned char>(index) < MAX_GLYPHS);
				return glyph_[static_cast<unsigned char>(index)];
			}
			Raster::ManagedSurface CreateMapWithQuality(const Quality& qual
												,const Raster::Font& font) {
				if (qual == SOLID_TEXT)
					return CreateMap(&Raster::CreateGlyphSurface<
									Raster::SolidTextGenerator>
									,font);
				return CreateMap(&Raster::CreateGlyphSurface<
								Raster::BlendedTextGenerator>
								,font);
			}
			template <typename TextCreateFunc>
			Raster::ManagedSurface CreateMap(const TextCreateFunc& create
											,const Raster::Font& font) {
				unsigned int numRows = MAX_GLYPHS / MAP_PITCH;
				if (MAX_GLYPHS % MAP_PITCH)
					++numRows;
				Raster::GlyphAttrib glyphList[MAX_GLYPHS];
				unsigned int widthOfRow[numRows];
				std::fill(widthOfRow, widthOfRow+numRows, 0);	// init to 0
				unsigned int currentRow = 0;
				unsigned int index = 0;
				while (index < MAX_GLYPHS) {
					unsigned int rowEnd = std::min(
									static_cast<unsigned int>(MAX_GLYPHS)
									,(index+MAP_PITCH));
					for (; index < rowEnd; ++index) {
						Raster::GlyphAttrib& gla = glyphList[index];
						font.GetGlyphAttrib(gla, index);
						unsigned int glyphWidth =
								std::max(Raster::GlyphWidth(gla)
									,static_cast<unsigned int>(gla.advance));
						widthOfRow[currentRow] += glyphWidth+GLYPH_MARGIN;
					}
					++currentRow;
				}
				unsigned int width = *(std::max_element(widthOfRow
													,widthOfRow+numRows));
				unsigned int height = (charHeight_+GLYPH_MARGIN) * numRows;
				Raster::ManagedSurface mapSurface = Raster::CreateNewSurface(
															width, height);
				Rect pos = { 0, 0, 0, 0 };
				Raster::SurfaceColor fg = { 255, 255, 255, 0 };
				unsigned int finalWidth = MakePowerOfTwo(width);
				unsigned int finalHeight = MakePowerOfTwo(height);
				index = 0;
				while (index < MAX_GLYPHS) {
					unsigned int rowEnd = std::min(
										static_cast<unsigned int>(MAX_GLYPHS)
										,(index+MAP_PITCH));
					for (; index < rowEnd; ++index) {
						Raster::ManagedSurface glyphSurface =
											create(index, font, fg);
						Raster::TransferSurfaceData(glyphSurface
													,mapSurface, pos);
						Raster::GlyphAttrib& gla = glyphList[index];
						unsigned int glyphWidth =
								std::max(Raster::GlyphWidth(gla)
									,static_cast<unsigned int>(gla.advance));
						Glyph& g = glyph_[index];
						g.tu1 = static_cast<GLfloat>(pos.x)/finalWidth;
						g.tu2 = static_cast<GLfloat>(pos.x+glyphWidth)
													/finalWidth;
						g.tv1 = static_cast<GLfloat>(pos.y)/finalHeight;
						g.advance = gla.advance;
						pos.x += glyphWidth+GLYPH_MARGIN;
					}
					pos.y += charHeight_+GLYPH_MARGIN;
					pos.x = 0;
				}
				maxCharWidth_ = GetCharWidth('W');
				th_ = static_cast<GLfloat>(charHeight_) / finalHeight;
#if STARSCAPE_IMAGEFONT_DUMPTOFILE == 1
				mapSurface.SaveAsBitmap(STARSCAPE_IMAGEFONT_DUMPFILENAME);
#endif
				return mapSurface;
			}
			Glyph			glyph_[MAX_GLYPHS];
			SizeScalar		lineSkip_, charHeight_, maxCharWidth_;
			GLfloat			th_;	// texture line height
			ImageTexture	image_;
		};

		inline void DrawFontText(const ImageFont& font, const char* str
								,const CoordScalar& xpos
								,const CoordScalar& ypos) {
			font.DrawText(str, std::strlen(str), xpos, ypos);
		}

		inline unsigned int GetFontTextWidth(const ImageFont& font
											,const char* str) {
			return font.GetTextWidth(str, std::strlen(str));
		}
	}
}

#endif
