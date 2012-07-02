// editbox.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_EditBox_H_
#define Starscape_Gui_EditBox_H_

#include "gui/appearance/edit/editboxbrush.h"
#include "gui/window.h"

namespace Starscape {

	namespace Gui {

		class CursorBrushState;

		/** An interface control that allows interaction with text. */
		class EditBox : public Window {
		public:
			enum { DEFAULT_CHARS_PER_LINE = 48
					,DEFAULT_NUM_LINES = 12 };
			EditBox(const EditBoxBrush& brush, const char* text=""
					,bool readOnly = false
					,unsigned int numCharsPerLine = DEFAULT_CHARS_PER_LINE
					,unsigned int numLines = DEFAULT_NUM_LINES)
				: content_(brush.GetDocumentBrush(), brush.GetDocumentMarkup()
						,brush.GetPreferredWidth(numCharsPerLine), text)
				,caretPtr_(0), brush_(brush)
				,isReadOnly_(readOnly), isSelecting_(false)
			{
				Size size = brush_.GetPreferredSize(numCharsPerLine
													,numLines);
				SetSize(size.x, size.y);
			}
			/** Sets the text content. */
			void SetText(const char* str) {
				content_.SetText(str);
				caret_.SetPosition(0);
				caret_.MarkCurrent();
				AlertParentOfStateChange();
			}
			/** Retrieves a reference to the text content. */
			const PooledString& GetText() const	{ return content_.GetText(); }
			/** Appends the given string to the end of the document. */
			void AppendString(const char* str) {
				content_.AppendString(str);
				AlertParentOfStateChange();
			}
			/** Inserts the given string at the caret position. */
			void InsertString(const char* str) {
				content_.ReplaceSelection(caret_, str, std::strlen(str));
				AlertParentOfStateChange();
			}
			/** Erases the top line of the document. */
			void EraseTopLine() {
				content_.EraseTopLine();
			}
			/** Sets the caret position to the end of the document. */
			void MoveCaretToEnd() {
				caret_.SetPosition(GetDocumentLength(content_));
				caret_.MarkCurrent();
			}
			/** The number of lines in the document. */
			unsigned int GetNumLines() {
				return GetDocumentNumLines(content_);
			}
			/** Allow modification of the content. */
			void SetReadAndWrite()	{ isReadOnly_ = false; }
			/** Prevent modification of the content. */
			void SetReadOnly()	{ isReadOnly_ = true; }
			/** Enables document word-wrap. */
			void EnableTextWrapping() {
				content_.EnableTextWrapping();
				AlertParentOfStateChange();
			}
			/** Disables document word-wrap. */
			void DisableTextWrapping() {
				content_.DisableTextWrapping();
				AlertParentOfStateChange();
			}
			// message handling
			const Window* OnMouseGrab(int button);
			bool OnMouseRelease(int button);
			void OnKeyboardGrab(ActiveState& state);
			void OnKeyboardRelease();
			void OnKeyDown(const Input::KeyInfo& key);
			void OnButtonDown(int button);
			void OnButtonUp(int button);
			void OnMouseMove(const Coord& delta);
			void OnMouseOver(CursorBrushState& cursor);
			void OnMouseLeave(CursorBrushState& cursor);
			void OnResize() {
				content_.ChangeWidth(Width());
				CenterOnContent();
			}
			// scroll controls
			const SizeScalar& OffsetXInterval() const {
				return brush_.GetDocumentBrush().GetMaxCharWidth();
			}
			const SizeScalar& OffsetYInterval() const {
				return brush_.GetDocumentBrush().GetLineHeight();
			}
			int OffsetXTotal() const {
				return (
						content_.GetCounters().x
						+brush_.GetDocumentBrush().GetCaretWidth()
						-Width());
			}
			int OffsetYTotal() const {
				return ((
						(content_.GetCounters().y-1)
						*OffsetYInterval())
						+brush_.GetDocumentBrush().GetCharHeight()
						-Height());
			}
		protected:
			void AlertParentOfStateChange() {
				CenterOnContent();
				Window::AlertParentOfStateChange();
			}
			void CenterOnContent() {
				int xtotal = OffsetXTotal();
				if (xtotal > 0) {
					if (OffsetX() > xtotal)
					   SetOffsetX(xtotal);
				} else SetOffsetX(0);
				int ytotal = OffsetYTotal();
				if (ytotal > 0) {
					if (OffsetY() > ytotal)
						SetOffsetY(ytotal);
				} else SetOffsetY(0);
			}
			bool HasFocus() const	{ return (caretPtr_!=0); }
			void OnRender();
			void HandleKeyPress(const Input::KeyInfo& key);
			void Copy();
			void Cut();
			virtual void Paste(const PooledString& str);
			void MoveCaretToMousePos() {
				caret_.SetPosition(
						brush_.GetDocumentBrush().CoordToPos(content_
											,Coord(CursorX()+OffsetX()
												,CursorY()+OffsetY())));
				assert(caret_.GetPosition() <= GetDocumentLength(content_));
				CoordScalar xcoord = brush_.GetDocumentBrush().PosToCoord(
													content_
													,caret_.GetPosition()).x;
				if (xcoord < OffsetX())
						SetOffsetX(xcoord);
			}
			void ScrollToCaret() {
				Coord caretCoord = brush_.GetDocumentBrush().PosToCoord(
														content_
														,caret_.GetPosition());
				if (caretCoord.x > static_cast<signed int>(Width()+OffsetX()
								-brush_.GetDocumentBrush().GetCaretWidth()))
					SetOffsetX(caretCoord.x
							+brush_.GetDocumentBrush().GetCaretWidth()
							-Width());
				else if (caretCoord.x < OffsetX())
					SetOffsetX(caretCoord.x);
				if (caretCoord.y > static_cast<signed int>(Height()+OffsetY()
								-brush_.GetDocumentBrush().GetLineHeight()))
					SetOffsetY(caretCoord.y
								+brush_.GetDocumentBrush().GetLineHeight()
								-Height());
				else if (caretCoord.y < OffsetY())
					SetOffsetY(caretCoord.y);
			}
			Document			content_;
			CaretState			caret_;
			const CaretState*	caretPtr_;	// determines if box has focus
			const EditBoxBrush&	brush_;
			bool				isReadOnly_, isSelecting_;
		};
	}
}

#endif
