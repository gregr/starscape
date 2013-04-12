// documentbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_DocumentBrush_H_
#define Starscape_Gui_DocumentBrush_H_

#include "gui/control/edit/document.h"
#include "gui/config/font.h"
#include "gui/config/drawrect.h"
//#include "graphics/renderer.h"
#include "util/rect.h"
#include "util/coord.h"

namespace Starscape {

	namespace Gui {

		struct DocumentDrawState;

		class DocumentBrush {
		public:
			enum {
				DEFAULT_TABSIZE = 4
		   		,DEFAULT_CARET_WIDTH = 2
			};
			DocumentBrush(const Graphics::ImageFontHandle& font
						,const Graphics::Color& textColor
						,const Graphics::Color& selectTextColor
					   	,const Graphics::Color& caretColor
						,const Graphics::Color& selectColor)
				: tabSize_(DEFAULT_TABSIZE)
				,caretWidth_(DEFAULT_CARET_WIDTH)
				,font_(font), textColor_(textColor)
				,selectTextColor_(selectTextColor)
				,caretColor_(caretColor), selectColor_(selectColor)
			{}
			/** Draws a document and caret within specified parameters.
			 * The xpos and ypos take a control's scrolling offsets
			 * into account.  The rect is the control's renderrect.
			 * If caret is null, the caret and selection are not drawn.
			 * @param rect The rectangular region to draw in.
			 * @param document The Document to draw.
			 * @param caret The CaretState to draw with.
			 * @param xpos The x-position of the document.
			 * @param ypos The y-position of the document.
			 * @param isReadOnly If true, the caret will not be drawn.
			 */
			void Draw(const Rect& rect, const Document& document
					,const CaretState* caret
					,const CoordScalar& xpos
					,const CoordScalar& ypos
					,bool isReadOnly) const;
			/** Adjusts the size of tabs when drawn.
			 * @param tabSize Must be larger than 0.
			 */
			void SetTabSize(const SizeScalar& tabSize) {
				assert(tabSize > 0);
				tabSize_ = tabSize;
			}
			/** Determines the size of the caret in pixels. */
			void SetCaretWidth(const SizeScalar& caretWidth) {
				caretWidth_ = caretWidth;
			}
			/** The width of the caret as drawn. */
			const SizeScalar& GetCaretWidth() const	{ return caretWidth_; }
			/** The height of a line of text. */
			const SizeScalar& GetLineHeight() const {
				return font_->GetLineSkip();
			}
			/** The maximum height of any character in the document. */
			const SizeScalar& GetCharHeight() const {
				return font_->GetCharHeight();
			}
			/** The width of a specific character. */
			SizeScalar GetCharWidth(char ch) const {
				return font_->GetCharWidth(ch);
			}
			/** The maximum width of any character in the document. */
			const SizeScalar& GetMaxCharWidth() const {
				return font_->GetMaxCharWidth();
			}
			/** The normal text color. */
			const Graphics::Color& GetTextColor() const {
				return textColor_;
			}
			/** The width of a string of text. */
			unsigned int GetTextWidth(const char* text
									,unsigned int numChars) const {
				return font_->GetTextWidth(text, numChars);
			}
			/** Retrieves the total number of lines and longest line length.
			 * @return Size: x=the longest length y=number of lines
			 */
//			Size GetLineAndLengthCount(const Document& document) const;
			/** Converts a caret position to local coordinates. */
			Coord PosToCoord(const Document& document
							,unsigned int pos) const;
			/** Converts local coordinates to a caret position. */
			unsigned int CoordToPos(const Document& document
									,const Coord& coord) const;
			unsigned int TabSpacing(unsigned int pos
								,unsigned int posStart) const {
				return (tabSize_-((pos-posStart)%tabSize_));
			}
			void DrawSelectionRect(const CoordScalar& xpos
								,const CoordScalar& ypos
								,const SizeScalar& width
								,const SizeScalar& height) const {
				Graphics::UnbindTextures();
				selectColor_.MakeCurrent();
				DrawRect(MakeRect(xpos, ypos, width, height));
				font_->Bind();
				selectTextColor_.MakeCurrent();
			}
		private:
			void FindVisibleStart(DocumentDrawState& state) const;
			bool DrawText(DocumentDrawState& state
						,unsigned int endPos) const;
			bool DrawSelectedText(DocumentDrawState& state
								,unsigned int endPos) const;
			void DrawTextWhileVisible(DocumentDrawState& state) const;
			void DrawCaret(const DocumentDrawState& state) const;
			// draw advance
			void DrawTextAndAdvance(DocumentDrawState& state) const;
			void DrawTextAndAdvanceSelected(DocumentDrawState& state) const;
			SizeScalar					tabSize_, caretWidth_;
			Graphics::ImageFontHandle	font_;
			Graphics::Color				textColor_, selectTextColor_
										,caretColor_, selectColor_;
		};
	}
}

#endif
