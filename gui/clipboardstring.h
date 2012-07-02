// clipboardstring.h
// Gregory Rosenblatt
// 5/11/05

#ifndef Starscape_Gui_ClipboardString_H_
#define Starscape_Gui_ClipboardString_H_

#include "pooledstring.h"
#include <istream>

namespace Starscape {

	namespace Gui {

		/** Provides access to clipboard data in string form. */
		const PooledString& GetClipboardString();

		/** Sets the clipboard data to the given string. */
		void SetClipboardString(const PooledString&);

		/** Loads the clipboard with data from a stream. */
		void SetClipboardFromStream(std::istream& in);
	}
}

#endif
