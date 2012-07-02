// editboxbrush.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_EditBoxBrush_H_
#define Starscape_Gui_EditBoxBrush_H_

#include "gui/appearance/edit/documentbrush.h"

namespace Starscape {

	namespace Gui {

		class CursorBrush;

		class EditBoxBrush {
		public:
			EditBoxBrush(const DocumentBrush& docBrush
						,const DocumentMarkup& markup
						,const CursorBrush& cursorBrush
						,const Graphics::Color& background)
				: docBrush_(docBrush), markup_(markup)
				,cursorBrush_(cursorBrush)
				,bgColor_(background)	{}
			void Draw(const Rect& rect, Document& document
					,const CaretState* caret
					,const CoordScalar& xpos
					,const CoordScalar& ypos
					,bool isReadOnly) const {
				Graphics::UnbindTextures();
				bgColor_.MakeCurrent();
				DrawRect(rect);
				docBrush_.Draw(rect, document, caret
							,xpos, ypos, isReadOnly);
			}
			const DocumentBrush& GetDocumentBrush() const {
				return docBrush_;
			}
			const DocumentMarkup& GetDocumentMarkup() const {
				return markup_;
			}
			const CursorBrush& GetCursorBrush() const {
				return cursorBrush_;
			}
			SizeScalar GetPreferredWidth(unsigned int charsPerLine) const {
				return (charsPerLine*docBrush_.GetMaxCharWidth());
			}
			Size GetPreferredSize(unsigned int charsPerLine
								,unsigned int numLines) const {
				assert(numLines > 0);
				return Size((GetPreferredWidth(charsPerLine)
								+docBrush_.GetCaretWidth())
							,((numLines-1)*docBrush_.GetLineHeight()
								+docBrush_.GetCharHeight()));
			}
		private:
			const DocumentBrush&	docBrush_;
			const DocumentMarkup&	markup_;
			const CursorBrush&		cursorBrush_;
			Graphics::Color			bgColor_;
		};
	}
}

#endif
