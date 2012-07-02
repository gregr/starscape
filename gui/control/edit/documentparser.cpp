// documentparser.cpp
// Gregory Rosenblatt
// 5/16/05

#include "gui/appearance/edit/documentbrush.h"
//#include "gui/control/documentparser.h"
#include "gui/control/edit/documentmarkup.h"
//#include "gui/control/document.h"
//#include "asciihextoint.h"
//#include <iostream>	// debug

namespace Starscape {

	namespace Gui {

		void DocumentParser::ParseUnwrapped(const Document& document) {
//			std::cout << "parse\n";
			lineIndices_.clear();
			counters_.x = 0;
			counters_.y = 1;
			const char* text = document.GetText().c_str();
			unsigned int length = document.GetText().length();
			unsigned int lineSize = document.GetLineSize();
			unsigned int pos = 0;
			unsigned int lastIndex = pos;
			unsigned int tabStart = lastIndex;
			SizeScalar xpos = 0;
			lineIndices_.push_back(LineIndex());
			MarkupState markupState;
//			std::cout << "die\n";
			for (; pos < length; ++pos) {
				char ch = text[pos];
				if (ch == '\n') {
//					std::cout << "newline\n";
					lineIndices_.back().newLinePos = pos;
					lineIndices_.push_back(LineIndex());
					lastIndex = pos+1;
					tabStart = lastIndex;
					counters_.x = std::max(static_cast<SizeScalar>(counters_.x)
																,xpos);
					++counters_.y;
					xpos = 0;
//					std::cout << "newline out\n";
				}
				else {
					if ((pos-lastIndex) > lineSize) {
//						std::cout << "oversize\n";
						lineIndices_.back().newLinePos = pos;
						lineIndices_.push_back(LineIndex());
						lastIndex = pos;
						tabStart = lastIndex;
						counters_.x = std::max(static_cast<SizeScalar>(
																counters_.x)
																,xpos);
						++counters_.y;
						xpos = 0;
//						std::cout << "oversize out\n";
					}
					if (ch == '\t') {
//						std::cout << "tab\n";
						unsigned int tabSpace =
								document.GetBrush().TabSpacing(pos, tabStart);
						DocEvent event(pos, DocEvent::DOC_TAB, tabSpace);
//						std::cout << "why do you fucking die: "
//							<< lineIndices_.size() << " "
//							<< lineIndices_.back().eventArray.size() << "\n";
						lineIndices_.back().eventArray.push_back(event);
						xpos += tabSpace 
								* document.GetBrush().GetMaxCharWidth();
						tabStart = pos+1;
//						std::cout << "tab out\n";
					}
					else {
//						std::cout << "diedie\n";
						pos = markup_.GenerateMarkup(document, text
										,pos, lineIndices_.back(), xpos
										,markupState);
//						xpos += document.GetBrush().GetCharWidth(ch);
					}
				}
			}
			counters_.x = std::max(static_cast<SizeScalar>(counters_.x)
															,xpos);
			lineIndices_.back().newLinePos = length;
		}

		void DocumentParser::ParseWrapped(const Document& document) {
			lineIndices_.clear();
			counters_.x = 0;
			counters_.y = 1;
			const char* text = document.GetText().c_str();
			unsigned int length = document.GetText().length();
			unsigned int pos = 0;
			unsigned int lastIndex = pos;
			unsigned int tabStart = lastIndex;
			SizeScalar xpos = 0;
			lineIndices_.push_back(LineIndex());
			MarkupState markupState;
			for (; pos < length; ++pos) {
				char ch = text[pos];
				if (ch == '\n') {
					lineIndices_.back().newLinePos = pos;
					lineIndices_.push_back(LineIndex());
					lastIndex = pos+1;
					tabStart = lastIndex;
					++counters_.y;
					xpos = 0;
				}
				else {
					if (ch == '\t') {
						unsigned int tabSpace =
								document.GetBrush().TabSpacing(pos, tabStart);
						xpos += tabSpace
								* document.GetBrush().GetMaxCharWidth();
						DocEvent event(pos, DocEvent::DOC_TAB, tabSpace);
						lineIndices_.back().eventArray.push_back(event);
						tabStart = pos+1;
					}
					else {
						pos = markup_.GenerateMarkup(document, text
										,pos, lineIndices_.back(), xpos
										,markupState);
//						xpos += document.GetBrush().GetCharWidth(ch);
					}
					if (std::isspace(ch)) {
						lastIndex = pos+1;
						char next = text[lastIndex];
						if ((next == '\n') || (next == 0))
							lastIndex = pos;
					}
					if (xpos >= document.GetWidth()) {	// then wrap
						if (lineIndices_.size() > 1) {
							unsigned int lastNewLine
									= (lineIndices_.end()-2)->newLinePos;
							// if lastIndex was already wrapped
							if ((lastNewLine == lastIndex)
									|| (lastNewLine == lastIndex-1))
								lastIndex = pos;
						}
						else
							lastIndex = pos;	// last index was doc start
						++counters_.y;
						if (std::isspace(ch))
							xpos = 0;
						else
							xpos = document.GetBrush().GetTextWidth(
														text+lastIndex
														,(pos+1-lastIndex));
						lineIndices_.back().newLinePos = lastIndex;
						lineIndices_.push_back(LineIndex());
						tabStart = lastIndex;
					}
				}
			}
			counters_.x = document.GetWidth()
						- document.GetBrush().GetCaretWidth();
			lineIndices_.back().newLinePos = length;
		}
	}
}
