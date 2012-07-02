// bevelbrush.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Gui_BevelBrush_H_
#define Starscape_Gui_BevelBrush_H_

#include "gui/config/drawrect.h"

namespace Starscape {

	namespace Gui {

		const unsigned int BORDER_SIZE	= 2;
		const unsigned int FRAME_SIZE	= BORDER_SIZE+2;

		/** Contains the color information for drawing bevels. */
		struct BevelBrush {
			explicit BevelBrush(const Graphics::Color& c)
				: color(c), lighter(GetLighterColor(color))
				,darker(GetDarkerColor(color))
				,darkest(GetDarkerColor(darker))	{}
			Graphics::Color	color, lighter
							,darker, darkest;
		};

		// helper functions follow . . .

		inline void DrawBorderUpperLeft(const Rect& rect) {
			DrawRect(MakeRect(rect.x, rect.y, 1, rect.h));
			DrawRect(MakeRect(rect.x, rect.y, rect.w, 1));
		}

		inline void DrawBorderLowerRight(const Rect& rect) {
			DrawRect(MakeRect(rect.x+rect.w-1, rect.y, 1, rect.h));
			DrawRect(MakeRect(rect.x, rect.y+rect.h-1, rect.w, 1));
		}

		inline void DrawBorder(const Rect& rect
							,const Graphics::Color& upperLeft
							,const Graphics::Color& lowerRight) {
			upperLeft.MakeCurrent();
			DrawBorderUpperLeft(rect);
			lowerRight.MakeCurrent();
			DrawBorderLowerRight(rect);
		}

		inline void DrawFullBorder(const Rect& rect
								,const Graphics::Color& c1
								,const Graphics::Color& c2
								,const Graphics::Color& c3
								,const Graphics::Color& c4) {
			DrawBorder(rect, c1, c2);
			DrawBorder(MakeRect(rect.x+1, rect.y+1
								,rect.w-2, rect.h-2)
						,c3, c4);
		}

		/** Draws a bevel of the given color over the given rect. */
		inline void DrawBevel(const Rect& rect
							,const BevelBrush& bevel) {
			DrawFullBorder(rect, bevel.color, bevel.darkest
							,bevel.lighter, bevel.darker);
		}

		/** Draws a sink of the given color over the given rect. */
		inline void DrawSink(const Rect& rect
							,const BevelBrush& bevel) {
			DrawFullBorder(rect, bevel.darker, bevel.lighter
							,bevel.darkest, bevel.color);
		}

		/** Draws a panel of the given color over the given rect. */
		inline void DrawBeveledPanel(const Rect& rect
									,const BevelBrush& bevel) {
			DrawBevel(rect, bevel);
			bevel.color.MakeCurrent();
			DrawRect(MakeRect(rect.x+BORDER_SIZE
							,rect.y+BORDER_SIZE
							,rect.w-(BORDER_SIZE+BORDER_SIZE)
							,rect.h-(BORDER_SIZE+BORDER_SIZE)));
		}

		/** Draws a pressed panel of the given color over the given rect. */
		inline void DrawBeveledPanelPressed(const Rect& rect
										,const BevelBrush& bevel) {
			bevel.color.MakeCurrent();
			DrawRect(rect);
			bevel.darker.MakeCurrent();
			DrawBorderUpperLeft(rect);
			DrawBorderLowerRight(rect);
		}

		/** Draws a frame of the given color over the given rect. */
		inline void DrawBeveledFrame(const Rect& rect
							,const BevelBrush& bevel) {
			DrawBevel(rect, bevel);
			DrawFullBorder(MakeRect(rect.x+BORDER_SIZE
									,rect.y+BORDER_SIZE
									,rect.w-(BORDER_SIZE+BORDER_SIZE)
									,rect.h-(BORDER_SIZE+BORDER_SIZE))
							,bevel.color, bevel.color
							,bevel.color, bevel.color);
		}
	}
}

#endif
