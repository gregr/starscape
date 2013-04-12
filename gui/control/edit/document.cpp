// document.cpp
// Gregory Rosenblatt
// 5/16/05

#include "gui/appearance/edit/documentbrush.h"
//#include <iostream>

namespace Starscape {

	namespace Gui {

		typedef std::pair<unsigned int, const LineIndex*>	LineStartIndexPair;

		LineStartIndexPair GetStartOfLine(const Document& document
										,unsigned int pos) {
			const DocumentParser::IndexVector& newLineIndices
										= document.GetNewLineIndices();
			DocumentParser::IndexVector::const_reverse_iterator itr
										= newLineIndices.rbegin()
										,end = newLineIndices.rend();
			const char* text = document.GetText().c_str();
			while ((pos < itr->newLinePos)) {
				++itr;
				if (itr == end)
					return std::make_pair(0, &newLineIndices.front());
			}
			unsigned int target = itr->newLinePos;
			char current = text[target];
			if ((current == '\n') || (current == 0)) {
				++target;
				if (pos < target) {
					++itr;
					if (itr == end)
						return std::make_pair(0, &newLineIndices.front());
					target = itr->newLinePos;
					current = text[target];
					if ((current == '\n'))
						++target;
				}
			}
			return std::make_pair(target, &(*(itr-1)));
		}

		void Document::MoveCaretOutOfSkipRegion(CaretState& caret) {
			unsigned int pos = caret.GetPosition();
			LineStartIndexPair lineStart = GetStartOfLine(*this, pos);
			DocEventVector::const_iterator
					itr = lineStart.second->eventArray.begin()
					,end = lineStart.second->eventArray.end();
			for (; itr != end; ++itr) {
				if (pos >= itr->pos) {
					if (itr->type == DocEvent::DOC_SKIP) {
						unsigned int target = itr->pos + itr->data;
						if (pos < (target)) {
							caret.SetPosition(target);
							caret.MarkCurrent();
							pos = caret.GetPosition();
						}
					}
				}
				else
					break;
			}
		}

		unsigned int GetEndOfLine(const Document& document
								,unsigned int pos) {
			const DocumentParser::IndexVector& newLineIndices
										= document.GetNewLineIndices();
			DocumentParser::IndexVector::const_iterator itr
										= newLineIndices.begin();
			const char* text = document.GetText().c_str();
			while (pos > itr->newLinePos)
				++itr;
			unsigned int target = itr->newLinePos;
			char current = text[target];
			if ((current != '\n') && (current != 0)) {
				--target;
				if (pos > target) {
					target = (itr+1)->newLinePos;
					current = text[target];
					if ((current != '\n') && (current != 0))
						--target;
				}
			}
			return target;
		}

		void MoveCaretToEndOfLine(const Document& document
								,CaretState& caret) {
			caret.SetPosition(GetEndOfLine(document, caret.GetPosition()));
		}

		void MoveCaretToStartOfLine(const Document& document
									,CaretState& caret) {
			caret.SetPosition(GetStartOfLine(document
											,caret.GetPosition()).first);
		}

		void MoveCaretAdvance(const Document& document, CaretState& caret) {
			unsigned int pos = caret.GetPosition();
			LineStartIndexPair lineStart = GetStartOfLine(document, pos);
			DocEventVector::const_iterator
					itr = lineStart.second->eventArray.begin()
					,end = lineStart.second->eventArray.end();
			for (; itr != end; ++itr) {
				if (pos <= itr->pos) {
					if (pos == itr->pos) {
						if (itr->type == DocEvent::DOC_SKIP) {
							caret.SetPosition(pos + itr->data);
							pos = caret.GetPosition();
						}
					}
					else
						break;
				}
			}
			if (pos < GetDocumentLength(document))
				caret.Advance();
		}

		void MoveCaretRetreat(const Document& document, CaretState& caret) {
			unsigned int pos = caret.GetPosition();
			LineStartIndexPair lineStart = GetStartOfLine(document, pos);
			DocEventVector::const_reverse_iterator
					itr = lineStart.second->eventArray.rbegin()
					,end = lineStart.second->eventArray.rend();
			for (; itr != end; ++itr) {
				if (pos > itr->pos) {
					if (itr->type == DocEvent::DOC_SKIP) {
						unsigned int target = itr->pos;
						if (pos <= (target + itr->data)) {
							caret.SetPosition(target);
							pos = caret.GetPosition();
						}
						else
							break;
					}
				}
			}
			if (pos > 0)
				caret.Retreat();
		}

		void MoveCaretUp(const Document& document
						,CaretState& caret) {
			unsigned int pos = caret.GetPosition();
			LineStartIndexPair lineStart = GetStartOfLine(document, pos);
			if (lineStart.first == 0)
				return;
			// walk from start to pos
			unsigned int offset = pos-lineStart.first;
			DocEventVector::const_iterator
					itr = lineStart.second->eventArray.begin()
					,end = lineStart.second->eventArray.end();
			for (; itr != end; ++itr) {
				if (pos <= itr->pos)
					break;
				if (itr->type == DocEvent::DOC_TAB)
					offset += (itr->data-1);
				else if (itr->type == DocEvent::DOC_SKIP) {
					assert(offset >= itr->data);
					offset -= itr->data;
				}
			}
			LineStartIndexPair prevLineStart
							= GetStartOfLine(document, lineStart.first-1);
			// walk to new pos
			itr = prevLineStart.second->eventArray.begin();
			end = prevLineStart.second->eventArray.end();
			pos = prevLineStart.first+offset;
			for (; itr != end; ++itr) {
				if (pos <= itr->pos)
					break;
				if (itr->type == DocEvent::DOC_TAB)
					pos -= std::min((pos-itr->pos), (itr->data-1));
				else if (itr->type == DocEvent::DOC_SKIP)
					pos += itr->data;
			}
			caret.SetPosition(std::min(pos, (lineStart.first-1)));
		}

		void MoveCaretDown(const Document& document
						,CaretState& caret) {
			unsigned int pos = caret.GetPosition();
			unsigned int lineEnd = GetEndOfLine(document, pos);
			if (lineEnd == GetDocumentLength(document))
				return;
			unsigned int nextLineEnd = GetEndOfLine(document, lineEnd+1);
			LineStartIndexPair lineStart = GetStartOfLine(document, pos);
			// walk from start to pos
			unsigned int offset = pos-lineStart.first;
			DocEventVector::const_iterator
					itr = lineStart.second->eventArray.begin()
					,end = lineStart.second->eventArray.end();
			for (; itr != end; ++itr) {
				if (pos <= itr->pos)
					break;
				if (itr->type == DocEvent::DOC_TAB)
					offset += (itr->data-1);
				else if (itr->type == DocEvent::DOC_SKIP) {
					assert(offset >= itr->data);
					offset -= itr->data;
				}
			}
			LineStartIndexPair nextLineStart =
								GetStartOfLine(document, lineEnd+1);
			// walk to new pos
			itr = nextLineStart.second->eventArray.begin();
			end = nextLineStart.second->eventArray.end();
			pos = nextLineStart.first+offset;
			for (; itr != end; ++itr) {
				if (pos <= itr->pos)
					break;
				if (itr->type == DocEvent::DOC_TAB)
					pos -= std::min((pos-itr->pos), (itr->data-1));
				else if (itr->type == DocEvent::DOC_SKIP)
					pos += itr->data;
			}
			caret.SetPosition(std::min(pos, nextLineEnd));
		}
	}
}
