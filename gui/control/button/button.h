// button.h
// Gregory Rosenblatt
// 5/18/05

#ifndef Starscape_Gui_Button_H_
#define Starscape_Gui_Button_H_

#include "gui/appearance/button/buttonbrush.h"
#include "gui/window.h"
#include "pooledstring.h"

namespace Starscape {

	namespace Gui {

		/** Executes a command when pressed and then released. */
		class Button : public Window {
		public:
			Button(const ButtonBrush& brush, const char* caption = "")
				: caption_(caption), brush_(brush)
				,hovering_(false), pressed_(false)
			{
				Size size = brush.GetPreferredSize(caption_.c_str());
				SetSize(size.x, size.y);
			}
			void OnMouseOver(CursorBrushState& cursor) {
				hovering_ = true;
			}
			void OnMouseLeave(CursorBrushState& cursor) {
				hovering_ = false;
			}
		private:
			PooledString		caption_;
			const ButtonBrush&	brush_;
			bool				hovering_;
		protected:
			bool	pressed_;
			void OnRender() {
				brush_.Draw(GetRenderingRect(), caption_.c_str()
							,IsPressed(), IsHovering());
			}
			bool IsHovering() const	{ return hovering_; }
			bool IsPressed() const	{ return pressed_; }
			const PooledString& GetCaption() const	{ return caption_; }
		};
	}
}

#endif
