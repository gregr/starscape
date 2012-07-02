// documentmarkup.cpp
// Gregory Rosenblatt
// 5/16/05

#include "gui/appearance/edit/documentbrush.h"
#include "gui/control/edit/documentmarkup.h"
#include "asciihextoint.h"
//#include <iostream>	// debug

namespace Starscape {

	namespace Gui {

		unsigned int DocumentMarkup::GenerateMarkup(const Document& document
								,const char* text, unsigned int pos
								,LineIndex& line, SizeScalar& xpos
								,MarkupState& state) const {
//			std::cout << "yes\n";
			xpos += document.GetBrush().GetCharWidth(text[pos]);
			return pos;
		}

		unsigned int TagMarkup::GenerateMarkup(const Document& document
								,const char* text, unsigned int pos
								,LineIndex& line, SizeScalar& xpos
								,MarkupState& state) const {
			const char* const RESET_STR	= "reset";
			const char TAG_START = '<';
			const char TAG_END = '>';
			char ch = text[pos];
			if (ch == TAG_START) {
				unsigned int start = pos;
				++pos;
				PooledString value;
				while ((ch = text[pos]) != TAG_END) {
					if ((std::isspace(ch)) || (ch == 0)) {
						xpos += document.GetBrush().GetCharWidth(TAG_START);
						return start;
					}
					value += ch;
					++pos;
				}
				line.eventArray.push_back(DocEvent(start, DocEvent::DOC_SKIP
										,(pos-start+1)));
				if (value == RESET_STR)
					line.eventArray.push_back(DocEvent(pos+1
											,DocEvent::DOC_RESETCOLOR));
				else
					line.eventArray.push_back(DocEvent(pos+1
											,DocEvent::DOC_SETCOLOR
											,AsciiHexToInt(value.c_str())));
			}
			else
				xpos += document.GetBrush().GetCharWidth(ch);
			return pos;
		}

		unsigned int KeywordMarkup::GenerateMarkup(const Document& document
								,const char* text, unsigned int pos
								,LineIndex& line, SizeScalar& xpos
								,MarkupState& state) const {
			if (pos >= state.pos) {
				state.pos = pos;
				char ch = text[state.pos];
				if (!state.seek && std::isalpha(ch)) {
					PooledString word;
					word += ch;
					++state.pos;
					while (std::isalnum(ch = text[state.pos])) {
						word += ch;
						++state.pos;
					}
					KeywordColorMap::const_iterator
							entry = keywordMap_.find(word);
					if (entry != keywordMap_.end()) {
						line.eventArray.push_back(
								DocEvent(pos, DocEvent::DOC_SETCOLOR
										,entry->second));
						line.eventArray.push_back(
								DocEvent(state.pos, DocEvent::DOC_RESETCOLOR));
					}
				}
				else if (ch == '\"') {
					KeywordColorMap::const_iterator
							entry = keywordMap_.find("\"");
					if (entry != keywordMap_.end()) {
						++state.pos;
						if (!state.seek) {
							line.eventArray.push_back(
									DocEvent(pos, DocEvent::DOC_SETCOLOR
											,entry->second));
							state.seek = '\"';
						}
						else if (state.seek == '\"') {
							line.eventArray.push_back(
									DocEvent(state.pos
											,DocEvent::DOC_RESETCOLOR));
							state.seek = 0;
						}
					}
				}
				else if (ch == '\'') {
					KeywordColorMap::const_iterator
							entry = keywordMap_.find("\'");
					if (entry != keywordMap_.end()) {
						++state.pos;
						if (!state.seek) {
							line.eventArray.push_back(
									DocEvent(pos, DocEvent::DOC_SETCOLOR
											,entry->second));
							state.seek = '\'';
						}
						else if (state.seek == '\'') {
							line.eventArray.push_back(
									DocEvent(state.pos
											,DocEvent::DOC_RESETCOLOR));
							state.seek = 0;
						}
					}
				}
			}
			return DocumentMarkup::GenerateMarkup(document, text
										,pos, line, xpos, state);
		}
	}
}
