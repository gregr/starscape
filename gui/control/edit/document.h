// document.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_Document_H_
#define Starscape_Gui_Document_H_

#include "gui/control/edit/documentparser.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <cassert>
//#include <iostream>	// debug

namespace Starscape {

	namespace Gui {

		/** Stores the current state information of the caret. */
		class CaretState {
		public:
			CaretState()
				: currentPos_(0), markedPos_(0)	{}
			/** Moves the caret to the given position. */
			void SetPosition(unsigned int pos)	{ currentPos_ = pos; }
			/** The actual caret position. */
			unsigned int GetPosition() const	{ return currentPos_; }
			/** Sets the marked position to be the current position. */
			void MarkCurrent()	{ markedPos_ = currentPos_; }
			/** The marked position of the caret for selection. */
			unsigned int GetMarkedPosition() const	{ return markedPos_; }
			/** Advances to the next position. */
			void Advance()	{ ++currentPos_; }
			/** Retreats to the previous position. */
			void Retreat()	{ assert(currentPos_ > 0); --currentPos_; }
		private:
			unsigned int	currentPos_, markedPos_;
		};

		/** Determines if the caret is selecting multiple characters. */
		inline bool CaretHasSelection(const CaretState& caret) {
			return (caret.GetPosition() != caret.GetMarkedPosition());
		}

		/** Returns the starting position of the caret's selected text. */
		inline unsigned int CaretSelectionStartPos(const CaretState& caret) {
			return std::min(caret.GetPosition(), caret.GetMarkedPosition());
		}

		/** Returns the ending position of the caret's selected text. */
		inline unsigned int CaretSelectionEndPos(const CaretState& caret) {
			return std::max(caret.GetPosition(), caret.GetMarkedPosition());
		}

		/** Returns the length of the text selected by the caret. */
		inline unsigned int CaretSelectionLength(const CaretState& caret) {
			return std::abs(static_cast<int>(caret.GetPosition()
											-caret.GetMarkedPosition()));
		}

		class DocumentBrush;

		/** The data component of gui controls that manipulate text. */
		class Document {
			enum { DEFAULT_LINESIZE = 1024
				,DEFAULT_MAXSIZE = unsigned(-1) };
		public:
			/** Creates a Document with the given starting data. */
			Document(const DocumentBrush& brush, const DocumentMarkup& markup
					,const SizeScalar& width, const char* text="")
				: text_(text), parser_(markup)
				,width_(width), lineSize_(DEFAULT_LINESIZE)
				,maxSize_(DEFAULT_MAXSIZE), brush_(brush)
			{
				assert(width_ > 0);
				LimitTextLength();
				DisableTextWrapping();
			}
			Document(const Document& doc)
				: text_(doc.text_), parser_(doc.parser_)
				,width_(doc.width_), lineSize_(doc.lineSize_)
				,maxSize_(doc.maxSize_), brush_(doc.brush_)
			{
				assert(width_ > 0);
//				std::cout << "copy made: " << text_ << "\n";
				LimitTextLength();
				DisableTextWrapping();
			}
			/** Modifies the width of the document for text-wrapping. */
			void ChangeWidth(const SizeScalar& width) {
				assert(width > 0);
				width_ = width;
				UpdateNewLineIndices();
			}
			/** The width of the document for text-wrapping. */
			const SizeScalar& GetWidth() const	{ return width_; }
			/** Limits the number of characters per line.
			 * @param maxChars Must be larger than 0.
			 */
			void SetMaxCharsPerLine(unsigned int maxChars) {
				assert(maxChars > 0);
				lineSize_ = maxChars;
				UpdateNewLineIndices();
			}
			/** The maximum number of characters per line. */
			unsigned int GetLineSize() const	{ return lineSize_; }
			/** Limits the maximum number of characters that can be stored.
			 */
			void SetMaxSize(unsigned int maxSize) {
				assert(maxSize > 0);
				maxSize_ = maxSize;
				LimitTextLength();
			}
			/** The maximum number of characters that can be stored. */
			unsigned int GetMaxSize() const	{ return maxSize_; }
			/** Long lines will wrap within the drawing region. */
			void EnableTextWrapping() {
				calcIndices_
					= boost::bind(&Document::CalcWrappedNewLineIndices
								,this);
				UpdateNewLineIndices();
			}
			/** Long lines will continue outside the drawing region. */
			void DisableTextWrapping() {
				calcIndices_
					= boost::bind(&Document::CalcUnwrappedNewLineIndices
								,this);
				UpdateNewLineIndices();
			}
			/** Replaces the selected data with the provided characters. */
			void ReplaceSelection(CaretState& caret
								,const char* str, unsigned int numChars) {
				unsigned int startPos = CaretSelectionStartPos(caret);
				unsigned int selectionLength = CaretSelectionLength(caret);
				unsigned int validSpace = selectionLength + GetMaxSize()
											- text_.length();
				if (numChars > validSpace)
					numChars = validSpace;
				text_.replace(startPos, selectionLength, str, numChars);
				caret.SetPosition(startPos+numChars);
				caret.MarkCurrent();
				UpdateNewLineIndices();
				MoveCaretOutOfSkipRegion(caret);
			}
			/** Deletes a character at the given position. */
			void RemoveCharAtPos(CaretState& caret, unsigned int pos) {
				assert(pos < text_.length());
				text_.erase(pos, 1);
				caret.SetPosition(pos);
				caret.MarkCurrent();
				UpdateNewLineIndices();
				MoveCaretOutOfSkipRegion(caret);
			}
			/** Appends the given string at the end of the document. */
			void AppendString(const char* str) {
				text_.append(str);
				LimitTextLength();
				UpdateNewLineIndices();
			}
			/** Erases the top line of the data contained. */
			void EraseTopLine() {
				text_.erase(0, GetNewLineIndices().front().newLinePos+1);
				UpdateNewLineIndices();
			}
			/** Erases all of the data contained in this document. */
			void Clear()	{ text_.clear(); UpdateNewLineIndices(); }
			/** Provides the complete text string. */
			const PooledString& GetText() const	{ return text_; }
			/** Sets the string value of the text. */
			void SetText(const char* str) {
				text_ = str;
				LimitTextLength();
				UpdateNewLineIndices();
			}
			/** Refreshes the memoization structure for new lines. */
			void UpdateNewLineIndices() {
				assert(calcIndices_);
				calcIndices_();
			}
			/** Retrieve the memoization structure for new line breaks. */
			DocumentParser::IndexVector& GetNewLineIndices() {
				return parser_.GetIndices();
			}
			const DocumentParser::IndexVector& GetNewLineIndices() const {
				return parser_.GetIndices();
			}
			const Size& GetCounters() const {
				return parser_.GetCounters();
			}
			const DocumentBrush& GetBrush() const {
				return brush_;
			}
		private:
			Document& operator=(const Document& doc);
			typedef boost::function<void ()>	CalcIndicesFunc;
			void CalcWrappedNewLineIndices() {
				parser_.ParseWrapped(*this);
			}
			void CalcUnwrappedNewLineIndices() {
				parser_.ParseUnwrapped(*this);
			}
			void LimitTextLength() {
				if (text_.length() > GetMaxSize())
					text_.resize(GetMaxSize());
			}
			void MoveCaretOutOfSkipRegion(CaretState& caret);
			PooledString			text_;
			DocumentParser			parser_;
			SizeScalar				width_;
			unsigned int			lineSize_, maxSize_;
			CalcIndicesFunc			calcIndices_;	// can handle text wrap
			const DocumentBrush&	brush_;
		};

		/** Returns a document's currently selected sub-string. */
		inline PooledString GetSelectedSubString(const Document& document
												,const CaretState& caret) {
			return document.GetText().substr(CaretSelectionStartPos(caret)
											,CaretSelectionLength(caret));
		}

		/** Returns a document's current length. */
		inline size_t GetDocumentLength(const Document& document) {
			return document.GetText().length();
		}

		/** Returns the number of lines in this document. */
		inline unsigned int GetDocumentNumLines(const Document& document) {
			return document.GetNewLineIndices().size();
		}

		/** Returns the current line position of the caret. */
		inline unsigned int GetDocumentLineAtPos(const Document& document
												,const CaretState& caret) {
			const DocumentParser::IndexVector& newLineIndices
										= document.GetNewLineIndices();
			unsigned int line = 0;
			while (caret.GetPosition() > newLineIndices[line].newLinePos)
				++line;
			return line;
		}

		/** Moves the caret to the beginning of the next word. */
		inline void MoveCaretToNextWord(const Document& document
										,CaretState& caret) {
			caret.SetPosition(std::min(
						(document.GetText().find_first_not_of(" \t\n"
							,(document.GetText().find_first_of(" \t\n"
								,caret.GetPosition()))))
						,GetDocumentLength(document)));
		}

		/** Moves the caret to the beginning of the previous word. */
		inline void MoveCaretToPreviousWord(const Document& document
											,CaretState& caret) {
			unsigned int pos = document.GetText().find_last_of(" \t\n"
							,(document.GetText().find_last_not_of(" \t\n"
								,caret.GetPosition()-1)))+1;
			if (pos >= caret.GetPosition())
				pos = 0;
			caret.SetPosition(pos);
		}

		/** Moves the caret to the end of the current line. */
		void MoveCaretToEndOfLine(const Document& document
								,CaretState& caret);

		/** Moves the caret to the beginning of the current line. */
		void MoveCaretToStartOfLine(const Document& document
									,CaretState& caret);

		/** Moves the caret forwards. */
		void MoveCaretAdvance(const Document& document, CaretState& caret);

		/** Moves the caret backwards. */
		void MoveCaretRetreat(const Document& document, CaretState& caret);

		/** Moves the caret up a line. */
		void MoveCaretUp(const Document& document, CaretState& caret);

		/** Moves the caret down a line. */
		void MoveCaretDown(const Document& document, CaretState& caret);

		/** Moves the caret up a line. */
/*		inline void MoveCaretUp(const Document& document
								,CaretState& caret) {
			unsigned int linePos = document.GetText().rfind('\n'
												,caret.GetPosition()-1)+1;
			if (linePos > caret.GetPosition())
				return;
			unsigned int prevLinePos = document.GetText().rfind('\n'
												,linePos-2)+1;
			if (prevLinePos >= linePos)
				prevLinePos = 0;
			caret.SetPosition(std::min(
						(linePos-1)
						,(prevLinePos + (caret.GetPosition()-linePos))));
		}
*/
		/** Moves the caret down a line. */
/*		inline void MoveCaretDown(const Document& document
								,CaretState& caret) {
			unsigned int nextLinePos = document.GetText().find('\n'
												,caret.GetPosition())+1;
			if ((nextLinePos > GetDocumentLength(document))
					|| (nextLinePos < caret.GetPosition()))
				return;
			unsigned int nextLineEnd = std::min(
										GetDocumentLength(document)
										,(document.GetText().find('\n'
												,nextLinePos)));
			unsigned int linePos = document.GetText().rfind('\n'
												,caret.GetPosition()-1)+1;
			if (linePos > caret.GetPosition())
				linePos = 0;
			caret.SetPosition(std::min(
						nextLineEnd
						,(nextLinePos + (caret.GetPosition()-linePos))));
		}
*/	}
}

#endif
