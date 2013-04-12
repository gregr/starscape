// documentbrush.cpp
// Gregory Rosenblatt
// 5/16/05

#include "gui/appearance/edit/documentbrush.h"
//#include <iostream>	// debug

namespace Starscape {

	namespace Gui {

		struct DocumentDrawState {
			DocumentDrawState(const Rect& rect, const Document& document
					,const CaretState* caret
					,const CoordScalar& x
					,const CoordScalar& y
					,unsigned int lineSkip)
				: caretCoord(-1, 0)
				,text(document.GetText().c_str()), xpos(x), ypos(y)
				,pos(0), index(0)
				,newLineIndices_(document.GetNewLineIndices())
				,itrNewLine_(newLineIndices_.begin())
				,itrEvent_(itrNewLine_->eventArray.begin())
				,docLength_(GetDocumentLength(document))
				,lineSkip_(lineSkip)
				,xLeftMargin_(xpos)
				,yVisibleStart_(rect.y-(lineSkip))
				,yVisibleEnd_(int(rect.y)+int(rect.h))
			{
				if (caret!=0) {
					selectStart_ = CaretSelectionStartPos(*caret);
					selectEnd_ = CaretSelectionEndPos(*caret);
					caretPos_ = caret->GetPosition();
				}
				else {
					selectStart_ = docLength_;
					selectEnd_ = docLength_;
					caretPos_ = docLength_;
				}
			}
			Graphics::Color	curTextColor;
			Coord caretCoord;
			const char* text;
			CoordScalar xpos, ypos;
			unsigned int pos, index;
			const DocumentParser::IndexVector&			newLineIndices_;
			DocumentParser::IndexVector::const_iterator	itrNewLine_;
			DocEventVector::const_iterator				itrEvent_;
		private:
			unsigned int selectStart_, selectEnd_
						,caretPos_, docLength_;
			unsigned int lineSkip_;
			int xLeftMargin_, yVisibleStart_, yVisibleEnd_;
			int LeftMarginX() const	{ return xLeftMargin_; }
		public:
			void DrawTextFragment(const Graphics::ImageFont& font) const {
//				if (pos < index) {
//					std::cout << pos << " " << index;
//				}
				assert(pos >= index);
				font.DrawText(text+index, pos-index, xpos, ypos);
			}
			unsigned int AdvancementWidth(
							const Graphics::ImageFont& font) const {
				assert(pos >= index);
				return font.GetTextWidth(text+index, pos-index);
			}
			void UpdateCaretCoord()	{
				assert(itrNewLine_ != newLineIndices_.end());
				char ch = CurrentChar();
				if (pos == caretPos_) {
					if ((pos == itrNewLine_->newLinePos)
							&& ((ch != '\n') && (ch != 0))) {
						caretCoord.x = xLeftMargin_;
						caretCoord.y = ypos + LineSkip();
					}
					else {
						caretCoord.x = xpos;
						caretCoord.y = ypos;
					}
				}
			}
			char CurrentChar() const	{ return text[pos]; }
			int VisibleStartY() const	{ return yVisibleStart_; }
			int VisibleEndY() const	{ return yVisibleEnd_; }
			unsigned int LineSkip() const	{ return lineSkip_; }
			unsigned int SelectStart() const	{ return selectStart_; }
			unsigned int SelectEnd() const	{ return selectEnd_; }
			unsigned int DocLength() const	{ return docLength_; }
			void SynchEventIterator() {
				itrEvent_ = itrNewLine_->eventArray.begin();
			}
			void StartNewLine(unsigned int startPos) {
				StartAtPos(startPos);
				pos = startPos;
				xpos = xLeftMargin_;
				ypos += LineSkip();
			}
			void StartAtPos(unsigned int startPos) {
				index = startPos;
			}
		};

		void DocumentBrush::DrawTextAndAdvance(DocumentDrawState& state) const {
			state.DrawTextFragment(*font_);
			state.xpos += state.AdvancementWidth(*font_);
		}

		void DocumentBrush::DrawTextAndAdvanceSelected(
											DocumentDrawState& state) const {
			unsigned int advanceWidth = state.AdvancementWidth(*font_);
			DrawSelectionRect(state.xpos, state.ypos
							,advanceWidth, state.LineSkip());
			state.DrawTextFragment(*font_);
			state.xpos += advanceWidth;
		}

		inline unsigned int CalcTabbedWidth(const DocumentBrush& brush
											,const DocumentDrawState& state) {
			return (state.itrEvent_->data * brush.GetMaxCharWidth());
		}

		inline void HandleDocTab(const DocumentBrush& brush
								,DocumentDrawState& state) {
			state.xpos += CalcTabbedWidth(brush, state);
			++state.pos;
		}

		inline void HandleDocTabSelected(const DocumentBrush& brush
										,DocumentDrawState& state) {
			unsigned int tabWidth = CalcTabbedWidth(brush, state);
			brush.DrawSelectionRect(state.xpos, state.ypos
									,tabWidth, state.LineSkip());
			state.xpos += tabWidth;
			++state.pos;
		}

		inline void HandleDocSkip(DocumentDrawState& state) {
			state.pos += state.itrEvent_->data;
		}

		inline void HandleDocSetColor(DocumentDrawState& state) {
			state.curTextColor
					= Graphics::MakeColorFromData(state.itrEvent_->data);
			state.curTextColor.MakeCurrent();
		}

		inline void HandleDocResetColor(const DocumentBrush& brush
										,DocumentDrawState& state) {
			state.curTextColor = brush.GetTextColor();
			state.curTextColor.MakeCurrent();
		}

		inline void HandleDocEvent(const DocumentBrush& brush
									,DocumentDrawState& state) {
			switch (state.itrEvent_->type) {
			case DocEvent::DOC_TAB:
				HandleDocTab(brush, state);
				break;
			case DocEvent::DOC_SKIP:
				HandleDocSkip(state);
				break;
			case DocEvent::DOC_SETCOLOR:
				HandleDocSetColor(state);
				break;
			case DocEvent::DOC_RESETCOLOR:
				HandleDocResetColor(brush, state);
				break;
			}
			state.StartAtPos(state.pos);
		}

		inline void HandleDocEventSelected(const DocumentBrush& brush
											,DocumentDrawState& state) {
			switch (state.itrEvent_->type) {
			case DocEvent::DOC_TAB:
				HandleDocTabSelected(brush, state);
				break;
			case DocEvent::DOC_SKIP:
				HandleDocSkip(state);
				break;
			case DocEvent::DOC_SETCOLOR:
				HandleDocSetColor(state);
				break;
			case DocEvent::DOC_RESETCOLOR:
				HandleDocResetColor(brush, state);
				break;
			}
			state.StartAtPos(state.pos);
		}

		void DocumentBrush::FindVisibleStart(DocumentDrawState& state) const {
			if (state.ypos >= state.VisibleStartY())
				return;
			DocumentParser::IndexVector::const_iterator end
												= state.newLineIndices_.end();
			for (; state.itrNewLine_ != end; ++state.itrNewLine_) {
				state.SynchEventIterator();
				DocEventVector::const_iterator eventEnd
									= state.itrNewLine_->eventArray.end();
				for (; state.itrEvent_ != eventEnd; ++state.itrEvent_) {
					if (state.itrEvent_->type == DocEvent::DOC_SETCOLOR)
						HandleDocSetColor(state);
					else if (state.itrEvent_->type == DocEvent::DOC_RESETCOLOR)
						HandleDocResetColor(*this, state);
				}
				state.ypos += state.LineSkip();
				if (state.ypos >= state.VisibleStartY()) {
					state.pos = state.itrNewLine_->newLinePos;
					if (state.CurrentChar() == '\n')
						++state.pos;
					state.StartAtPos(state.pos);
					++state.itrNewLine_;
					state.SynchEventIterator();
					return;
				}
			}
			state.pos = state.DocLength();
			state.StartAtPos(state.pos);
		}

		bool DocumentBrush::DrawText(DocumentDrawState& state
									,unsigned int endPos) const {
			if (state.pos >= endPos)
				return false;
			unsigned int nextEnd = state.itrNewLine_->newLinePos;
			while (nextEnd < endPos) {
				DocEventVector::const_iterator end
									= state.itrNewLine_->eventArray.end();
				for (; state.itrEvent_ != end; ++state.itrEvent_) {
					state.pos = state.itrEvent_->pos;
					if (state.pos > nextEnd)
						state.pos = nextEnd;
					DrawTextAndAdvance(state);
					HandleDocEvent(*this, state);
				}
				state.pos = nextEnd;
				state.DrawTextFragment(*font_);
				if (state.CurrentChar() == '\n')
					state.StartNewLine(state.pos+1);
				else
					state.StartNewLine(state.pos);
				if (state.ypos >= state.VisibleEndY())
					return true;
				++state.itrNewLine_;
				state.SynchEventIterator();
				nextEnd = state.itrNewLine_->newLinePos;
			}
			DocEventVector::const_iterator end
									= state.itrNewLine_->eventArray.end();
			for (; state.itrEvent_ != end; ++state.itrEvent_) {
				if (state.itrEvent_->pos >= endPos) {
					state.pos = endPos;
					DrawTextAndAdvance(state);
					state.StartAtPos(state.pos);
					return false;
				}
				state.pos = state.itrEvent_->pos;
				DrawTextAndAdvance(state);
				HandleDocEvent(*this, state);
			}
			state.pos = endPos;
			if (state.index < state.pos) {
				DrawTextAndAdvance(state);
				state.StartAtPos(state.pos);
			}
			return false;
		}

		bool DocumentBrush::DrawSelectedText(DocumentDrawState& state
											,unsigned int endPos) const {
			if (state.pos >= endPos)
				return false;
			unsigned int nextEnd = state.itrNewLine_->newLinePos;
			while (nextEnd < endPos) {
				DocEventVector::const_iterator end
									= state.itrNewLine_->eventArray.end();
				for (; state.itrEvent_ != end; ++state.itrEvent_) {
					state.pos = state.itrEvent_->pos;
					if (state.pos > nextEnd)
						state.pos = nextEnd;
					DrawTextAndAdvanceSelected(state);
					HandleDocEventSelected(*this, state);
				}
				state.pos = nextEnd;
				DrawTextAndAdvanceSelected(state);
				if (state.CurrentChar() == '\n')
					state.StartNewLine(state.pos+1);
				else
					state.StartNewLine(state.pos);
				if (state.ypos >= state.VisibleEndY())
					return true;
				++state.itrNewLine_;
				state.SynchEventIterator();
				nextEnd = state.itrNewLine_->newLinePos;
			}
			DocEventVector::const_iterator end
									= state.itrNewLine_->eventArray.end();
			for (; state.itrEvent_ != end; ++state.itrEvent_) {
				if (state.itrEvent_->pos >= endPos) {
					state.pos = endPos;
					DrawTextAndAdvanceSelected(state);
					state.StartAtPos(state.pos);
					return false;
				}
				state.pos = state.itrEvent_->pos;
				DrawTextAndAdvanceSelected(state);
				HandleDocEventSelected(*this, state);
			}
			state.pos = endPos;
			if (state.index < state.pos) {
				DrawTextAndAdvanceSelected(state);
				state.StartAtPos(state.pos);
			}
			return false;
		}

		void DocumentBrush::DrawTextWhileVisible(
									DocumentDrawState& state) const {
			if (DrawText(state, state.SelectStart()))
				return;
			state.UpdateCaretCoord();
			if (DrawSelectedText(state, state.SelectEnd()))
				return;
			state.UpdateCaretCoord();
			state.curTextColor.MakeCurrent();
			DrawText(state, state.DocLength());
		}

		void DocumentBrush::DrawCaret(const DocumentDrawState& state) const {
			Graphics::UnbindTextures();
			caretColor_.MakeCurrent();
			DrawRect(MakeRect(state.caretCoord.x
							,state.caretCoord.y
							,caretWidth_, state.LineSkip()));
		}

		void DocumentBrush::Draw(const Rect& rect, const Document& document
								,const CaretState* caret
								,const CoordScalar& xpos
								,const CoordScalar& ypos
								,bool isReadOnly) const {
			DocumentDrawState state(rect, document, caret
								,xpos, ypos, font_->GetLineSkip());
			font_->Bind();
			HandleDocResetColor(*this, state);
			FindVisibleStart(state);
			DrawTextWhileVisible(state);
			if ((caret != 0) && (!isReadOnly) && (state.caretCoord.x != -1))
				DrawCaret(state);
		}

		/* Converts a caret position to local coordinates. */
		Coord DocumentBrush::PosToCoord(const Document& document
										,unsigned int pos) const {
			assert(pos <= GetDocumentLength(document));
			const char* text = document.GetText().c_str();
			const DocumentParser::IndexVector& newLineIndices
											= document.GetNewLineIndices();
			unsigned int line = 0, index, lastIndex = 0;
			while (pos > (index = newLineIndices[line].newLinePos)) {
				lastIndex = index;
				if (text[index] == '\n')
					++lastIndex;
				++line;
			}
			if (pos == index) {
				char ch = text[index];
				if ((ch != '\n') && (ch != 0)) {
					lastIndex = index;
					++line;
				}
			}
			int xpos = 0;
			DocEventVector::const_iterator
					itr = newLineIndices[line].eventArray.begin()
					,end = newLineIndices[line].eventArray.end();
			const Graphics::ImageFont& font = *font_;
			for (; itr != end; ++itr) {
				if (pos <= itr->pos)
					break;
				if (itr->type == DocEvent::DOC_TAB) {
					xpos += font.GetTextWidth(text+lastIndex
											,itr->pos - lastIndex)
							+(itr->data * font.GetMaxCharWidth());
					lastIndex = itr->pos+1;
				}
				else if (itr->type == DocEvent::DOC_SKIP) {
					xpos += font.GetTextWidth(text+lastIndex
											,itr->pos - lastIndex);
					lastIndex = itr->pos+itr->data;
				}
			}
			if (lastIndex < pos)
				xpos += font.GetTextWidth(text+lastIndex, pos-lastIndex);
			return Coord(xpos, (line * font.GetLineSkip()));
		}

		/* Converts local coordinates to a caret position. */
		unsigned int DocumentBrush::CoordToPos(const Document& document
												,const Coord& coord) const {
			const DocumentParser::IndexVector& newLineIndices
											= document.GetNewLineIndices();
			const Graphics::ImageFont& font = *font_;
			size_t line = static_cast<unsigned int>(
								static_cast<float>(coord.y)
								/(font.GetLineSkip()));
			line = std::min(line, newLineIndices.size()-1);
			unsigned int lastIndex = 0;
			const char* text = document.GetText().c_str();
			if (line > 0) {
				lastIndex = newLineIndices[line-1].newLinePos;
				if (text[lastIndex] == '\n')
					++lastIndex;
			}
			unsigned int currentPos = lastIndex;
			unsigned int endPos = newLineIndices[line].newLinePos;
			char ch = text[endPos];
			if ((ch != '\n') && (ch != 0))
				--endPos;
			int widthLeft = coord.x;
			DocEventVector::const_iterator
					itr = newLineIndices[line].eventArray.begin()
					,end = newLineIndices[line].eventArray.end();
			for (; itr != end; ++itr) {
				unsigned int nextPos = itr->pos;
				while (currentPos < nextPos) {
					widthLeft -= font.GetCharWidth(text[currentPos]);
					if (widthLeft <= 0)
						return currentPos;
					++currentPos;
				}
				if (itr->type == DocEvent::DOC_TAB) {
					widthLeft -= itr->data * font.GetMaxCharWidth();
					if (widthLeft <= 0)
						return currentPos;
				}
				else if (itr->type == DocEvent::DOC_SKIP)
					currentPos += itr->data;
			}
			while (currentPos < endPos) {
				widthLeft -= font.GetCharWidth(text[currentPos]);
				if (widthLeft <= 0)
					break;
				++currentPos;
			}
			return currentPos;
		}
	}
}
