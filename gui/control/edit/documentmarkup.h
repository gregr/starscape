// documentmarkup.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Gui_DocumentMarkup_H_
#define Starscape_Gui_DocumentMarkup_H_

#include "util/scalar.h"
#include "pooledstring.h"
#include "pooledmap.h"

namespace Starscape {

	namespace Gui {

		class Document;

		struct MarkupState {
			MarkupState() : pos(0), seek(0)	{}
			unsigned int pos;
			char seek;
		};

		class DocumentMarkup {
		public:
			DocumentMarkup()	{}
			virtual ~DocumentMarkup()	{}
			virtual unsigned int GenerateMarkup(const Document& document
								,const char* text, unsigned int pos
								,LineIndex& line, SizeScalar& xpos
								,MarkupState& state) const;
		};

		class TagMarkup : public DocumentMarkup {
		public:
			unsigned int GenerateMarkup(const Document& document
								,const char* text, unsigned int pos
								,LineIndex& line, SizeScalar& xpos
								,MarkupState& state) const;
		};

		class KeywordMarkup : public DocumentMarkup {
		public:
			typedef PooledMap<PooledString, unsigned int>::Type	KeywordColorMap;
			// take a map of keywords to colors?
			KeywordMarkup(const KeywordColorMap& kcm) : keywordMap_(kcm)	{}
			unsigned int GenerateMarkup(const Document& document
								,const char* text, unsigned int pos
								,LineIndex& line, SizeScalar& xpos
								,MarkupState& state) const;
		private:
			KeywordColorMap	keywordMap_;
		};
	}
}

#endif
