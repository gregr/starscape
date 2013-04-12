// cursor.h
// Gregory Rosenblatt
// 5/20/05

#ifndef Starscape_Gui_Cursor_H_
#define Starscape_Gui_Cursor_H_

#include "gui/activestate.h"
#include "gui/window.h"
#include "util/coordstate.h"

namespace Starscape {

	namespace Gui {

		/** A representation for mouse input in a graphical user-interface.
		 * To be interfaced with a mouse device as a listener for input.
		 */
		class Cursor {
		public:
			/** Constructs a cursor owned by the given window.
			 * @param owner The window which manages the cursor.  This will
			 * normally by an interface window derived from the main class.
			 */
			Cursor(Window& owner, ActiveState& state
					,CursorBrushState& brush)
				: owner_(owner), local_(&owner), activeState_(state)
				,cursorBrush_(brush)	{ ResetFreeRegion(); }
			/** The x-position of the cursor. */
			int PosX() const	{ return pos_.GetCoord().x; }
			/** The y-position of the cursor. */
			int PosY() const	{ return pos_.GetCoord().y; }
			/** Immediately brings the cursor to the given location. */
			void Warp(unsigned int xpos, unsigned int ypos) {
				if (xpos >= owner_.Width())
					xpos = owner_.Width()-1;
				if (ypos >= owner_.Height())
					ypos = owner_.Height()-1;
				ResetCoordState(pos_, Coord(xpos, ypos));
			}
			/** Handles the event of the mouse input device moving. */
			void OnMove(int xrel, int yrel) {
				int xpos = PosX() + xrel;
				int ypos = PosY() + yrel;
				int xbound = freeRegion_.x+freeRegion_.w-1;
				int ybound = freeRegion_.y+freeRegion_.h-1;
				// check restrictions
				if (xpos < freeRegion_.x)
					xpos = freeRegion_.x;
				else if (xpos > xbound)
					xpos = xbound;
				if (ypos < freeRegion_.y)
					ypos = freeRegion_.y;
				else if (ypos > ybound)
					ypos = ybound;
				pos_ = Coord(xpos, ypos);
				local_->OnMouseMove(pos_.CalcRelCoord());
				UpdateLocalWindow();
			}
			/** Handles the event of a mouse button being pressed. */
			void OnButtonDown(int button) {
				activeState_.SetActiveFocus(*local_);
				activeState_.GetActiveFocus().OnButtonDown(button);
				const Window* grabber =
					activeState_.GetActiveFocus().OnMouseGrab(button);
				if (grabber)
					RestrictFreeRegionTo(*grabber);
			}
			/** Handles the event of a mouse button being released. */
			void OnButtonUp(int button) {
				local_->OnButtonUp(button);
				if (local_->OnMouseRelease(button))
					ResetFreeRegion();
			}
			/** Releases mouse movement restrictions back to the owner. */
			void ResetFreeRegion() {
				freeRegion_ = MakeRect(0, 0, owner_.Width(), owner_.Height());
			}
		private:
			// select the window directly under the cursor
			void UpdateLocalWindow() {
				Window& oldLocal = *local_;
				local_ = owner_.SelectWindowUnderCursor(pos_.GetCoord());
				assert(local_!=0);
				if (local_!=&oldLocal) {
					oldLocal.OnMouseLeave(cursorBrush_);
					local_->OnMouseOver(cursorBrush_);
				}
			}
			// restrict mouse movement to the bounds of the given window
			void RestrictFreeRegionTo(const Window& region) {
				typedef PooledList<const Window*>::Type	WindowList;
				WindowList regionList;
				const Window* curRegion = &region;
				// fill region hierarchy list
				while (curRegion->HasParent()) {
					regionList.push_front(curRegion);
					curRegion = &(curRegion->GetParent());
				}
				// prepare to narrow down the region
				ResetFreeRegion();
				Rect rect = { 0, 0, 0, 0 };
				WindowList::const_iterator
					itr = regionList.begin(),
					end = regionList.end();
				// find largest common region
				for (; itr!=end; ++itr) {
					const Window& w = **itr;
					rect.x += w.PosX();
					rect.y += w.PosY();
					rect.w = w.Width();
					rect.h = w.Height();
					SetRectUnion(freeRegion_, rect);
					rect.x -= w.OffsetX();
					rect.y -= w.OffsetY();
				}
			}
		private:
			CoordState			pos_;
			Rect				freeRegion_;
			Window&				owner_;
			Window				*local_;
			ActiveState&		activeState_;
			CursorBrushState&	cursorBrush_;
		};
	}
}

#endif
