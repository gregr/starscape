// documentparser.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_Gui_DocumentParser_H_
#define Starscape_Gui_DocumentParser_H_

#include "util/size.h"
#include "pooledstring.h"
#include "pooledvector.h"
#include <cassert>

namespace Starscape {

	namespace Gui {

		struct DocEvent {
			enum Type {
//				DOC_NONE,	// may be required with skip... nah
				DOC_TAB,
//				DOC_SELECT,
//				DOC_DESELECT,
				DOC_SKIP,
				DOC_SETCOLOR,
				DOC_RESETCOLOR
			};
			DocEvent(unsigned int indexPos, Type eventType
					,unsigned int eventData = 0)
				: type(eventType), pos(indexPos), data(eventData)	{}
			Type			type;
			unsigned int	pos, data;
		};

		//boost::pool_allocator has a bug where this would cause a segfault
		typedef PooledVector<DocEvent>::Type	DocEventVector;
		//typedef std::vector<DocEvent>	DocEventVector;

		struct LineIndex {
			DocEventVector	eventArray;
			unsigned int	newLinePos;
		};

		class Document;

		class DocumentMarkup;

		class DocumentParser {
		public:
			typedef PooledVector<LineIndex>::Type	IndexVector;
//			typedef std::vector<LineIndex>	IndexVector;
			DocumentParser(const DocumentMarkup& markup)
				: markup_(markup)	{}
			DocumentParser(const DocumentParser& parser)
				: markup_(parser.markup_)	{}
			void ParseUnwrapped(const Document& document);
			void ParseWrapped(const Document& document);
			IndexVector& GetIndices() {
				assert(!lineIndices_.empty());
				return lineIndices_;
			}
			const IndexVector& GetIndices() const {
				assert(!lineIndices_.empty());
				return lineIndices_;
			}
			const Size& GetCounters() const	{ return counters_; }
		private:
			// function pointer stuff?
//			void Evaluate(const PooledString& text
//						,unsigned int& pos) {
//			}
			IndexVector				lineIndices_;
			Size					counters_;
			const DocumentMarkup&	markup_;
		};
	}
}

#endif
