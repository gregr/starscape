// listboxbrush.h
// Gregory Rosenblatt
// 5/20/05

#ifndef Starscape_Gui_ListBoxBrush_H_
#define Starscape_Gui_ListBoxBrush_H_

#include "gui/appearance/button/buttonbrush.h"
#include "gui/config/drawrect.h"
#include "gui/config/image.h"

namespace Starscape {

	namespace Gui {

		class ListBoxBrush {
		public:
			ListBoxBrush(const ButtonBrush& itemButtonBrush
						,const Graphics::Color& backgroundColor)
				: backgroundColor_(backgroundColor)
				,itemButtonBrush_(itemButtonBrush)	{}
			void Draw(const Rect& region) const {
				Graphics::UnbindTextures();
				backgroundColor_.MakeCurrent();
				DrawRect(region);
			}
			Size GetPreferredSize(unsigned int charsPerName
								,unsigned int numItems) const {
				PooledString name;
				name.resize(charsPerName, 'W');
				Size itemSize = itemButtonBrush_.GetPreferredSize(name.c_str());
				return Size(itemSize.x, itemSize.y*numItems);
			}
			const ButtonBrush& GetItemButtonBrush() const {
				return itemButtonBrush_;
			}
		private:
			Graphics::Color		backgroundColor_;
			const ButtonBrush&	itemButtonBrush_;
		};
	}
}

#endif
