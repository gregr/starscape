// clipboardstring.cpp
// Gregory Rosenblatt
// 5/11/05

#include "gui/clipboardstring.h"
using namespace std;

namespace Starscape {

	namespace Gui {

		PooledString clipboardData;	// general purpose clipboard data

		const PooledString& GetClipboardString() {
			return clipboardData;
		}

		void SetClipboardString(const PooledString& data) {
			clipboardData = data;
		}

		void SetClipboardFromStream(std::istream& in) {
			if (in) {
				PooledString line;
				getline(in, line);
				if (in) {
					clipboardData = line;
					getline(in, line);
					while (in) {
						clipboardData += PooledString("\n")+line;
						getline(in, line);
					}
				}
			}
		}
	}
}
