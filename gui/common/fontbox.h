// fontbox.h
// Gregory Rosenblatt
// 5/16/05

#ifndef Starscape_Gui_FontBox_H_
#define Starscape_Gui_FontBox_H_

#include "gui/control/edit/editbox.h"
#include "gui/control/button/pushbutton.h"
#include "pooledvector.h"
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

namespace Starscape {

	namespace Gui {

		class FontBox : public Window {
		public:
			enum {
				DEFAULT_ROW_SIZE = 16
				,DEFAULT_NUM_GLYPHS = 256
			};
			FontBox(const EditBoxBrush& editBrush
					,const ButtonBrush& buttonBrush
					,unsigned int pitch = DEFAULT_ROW_SIZE
					,unsigned int numGlyphs = DEFAULT_NUM_GLYPHS)
				: edit_(editBrush)
			{
				AddChild(edit_);
				buttonArray_.reserve(numGlyphs);
				char caption[] = " ";
				unsigned int index = 0;
				Coord buttonPos;
				unsigned int maxWidth = 0;
				while (index < numGlyphs) {
					unsigned int rowEnd = index+pitch;
					for (; index < rowEnd; ++index) {
						caption[0] = index;
						buttonArray_.push_back(
								ButtonPtr(new PushButton(buttonBrush
									,boost::bind(&FontBox::InsertGlyph
												,this, index)
									,caption)));
						PushButton& button = *buttonArray_.back();
						AddChild(button);
						button.SetPos(buttonPos.x, buttonPos.y);
						buttonPos.x += button.Width();
					}
					if (buttonPos.x > static_cast<int>(maxWidth))
						maxWidth = buttonPos.x;
					buttonPos.x = 0;
					buttonPos.y += buttonArray_.back()->Height();
				}
				edit_.SetSize(maxWidth, edit_.Height()/4);
				edit_.SetPos(0, buttonPos.y);
				SetSize(maxWidth, buttonPos.y+edit_.Height());
			}
			void InsertGlyph(char glyph) {
				char str[] = { glyph, 0 };
				edit_.InsertString(str);
			}
		private:
			void OnRender()	{}
			typedef boost::shared_ptr<PushButton>	ButtonPtr;
			typedef PooledVector<ButtonPtr>::Type	GlyphButtonArray;
			EditBox				edit_;
			GlyphButtonArray	buttonArray_;
		};
	}
}

#endif
