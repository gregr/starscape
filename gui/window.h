// window.h
// Gregory Rosenblatt
// 5/20/05

#ifndef Starscape_Gui_Window_H_
#define Starscape_Gui_Window_H_

#include "gui/widget.h"
#include "gui/config/keyboard.h"
#include "util/coord.h"
#include "pooledlist.h"
#include <cassert>

namespace Starscape {

	namespace Gui {

		class ActiveState;

		class CursorBrushState;

		/** A hierarchical and event-handling user-interface object. */
		class Window : public Widget {
		public:
			/** Constructs a default window without a parent. */
			Window(const CoordScalar& xpos=0
					,const CoordScalar& ypos=0
					,const SizeScalar& width=0
					,const SizeScalar& height=0)
				: Widget(xpos, ypos, width, height)
				,parent_(0), pickable_(true)	{}
			// parent-child relations are non-assignable
			Window(const Window& w)
				: Widget(w), offset_(w.offset_)
				,parent_(0), pickable_(w.pickable_)	{}
			Window& operator=(const Window& w) {
				Widget::operator=(w);
				offset_ = w.offset_;
				pickable_ = w.pickable_;
				return *this;
			}
			~Window()	{ if (HasParent()) RemoveFromParent(); }
			/** Sets the x-offset of the window's client area. */
			void SetOffsetX(const CoordScalar& xoff)	{ offset_.x = xoff; }
			/** Sets the y-offset of the window's client area. */
			void SetOffsetY(const CoordScalar& yoff)	{ offset_.y = yoff; }
			/** The window rect's x-offset with respect to its content. */
			const CoordScalar& OffsetX() const	{ return offset_.x; }
			/** The window rect's y-offset with respect to its content. */
			const CoordScalar& OffsetY() const	{ return offset_.y; }
			/** Determines if the window has a parent. */
			bool HasParent() const	{ return (parent_!=0); }
			/** Retrieves a reference to the window's parent. */
			Window& GetParent() const {
				assert(HasParent());
				return *parent_;
			}
			/** Adds a window to this window's hierarchical list of children.
			 * Automatically sets the child's parent to be this window.
			 */
			void AddChild(Window& child) {
				childList_.push_front(&child);
				child.SetParent(*this, childList_.begin());
			}
			/** Moves this window to the top of its parent's z-order. */
			void MoveToTop() {
				if (!HasParent())
					return;
				GetParent().MoveChildToTop(*this);
				GetParent().MoveToTop();
			}
			/** Handles an update event.
			 * @param deltaTime Time elapsed (in ms) since the last update. */
			virtual void OnUpdate(unsigned int deltaTime)	{}
			/** Performs any special behavior after the window is resized.
			 * If dimensions need changing, SetSize() should be called first.
			 * Only called for special real-time resizing events. */
			virtual void OnResize()	{}
			/** Handles the opportunity to grab and restrain mouse movement.
			 * @return The window region to restrict movement to, if any. */
			virtual const Window* OnMouseGrab(int button)	{ return 0; }
			/** Determines whether to release the mouse if it was grabbed. */
			virtual bool OnMouseRelease(int button)	{ return true; }
			/** Determines whether this window should grab keyboard input. */
			virtual void OnKeyboardGrab(ActiveState& state)	{}
			/** Handles this window's loss of keyboard input focus. */
			virtual void OnKeyboardRelease()	{}
			/** Handles the event of a key being pressed. */
			virtual void OnKeyDown(const Input::KeyInfo& key)	{}
			/** Handles the event of a key being released. */
			virtual void OnKeyUp(const Input::KeyInfo& key)	{}
			/** Handles the event of a mouse button being pressed. */
			virtual void OnButtonDown(int button)	{}
			/** Handles the event of a mouse button being released. */
			virtual void OnButtonUp(int button)	{}
			/** Handles the event of the mouse being moved. */
			virtual void OnMouseMove(const Coord& delta)	{}
			/** Handles the event of the mouse entering the window region. */
			virtual void OnMouseOver(CursorBrushState& cursor)	{}
			/** Handles the event of the mouse leaving the window region. */
			virtual void OnMouseLeave(CursorBrushState& cursor)	{}
			/** Handles the loss of window focus. */
			virtual void OnLoseFocus()	{}
			/** Acts on any important state changes in child windows. */
			virtual void OnChildStateChange(const Window& child)	{}
			/** Makes this window selectable by the mouse cursor. */
			void SetPickable()	{ pickable_ = true; }
			/** Prevents this window from being selected by the cursor. */
			void SetUnpickable()	{ pickable_ = false; }
			/** Determines if this window can be selected by the cursor.
			 * To be pickable a window must also be visible.
			 */
			bool IsPickable() const	{ return (pickable_ && IsVisible()); }
		protected:
			typedef PooledList<Window*>::Type	WindowList;
			typedef WindowList::iterator		WindowIterator;
			/** The local x-position of the cursor in this window. */
			const CoordScalar& CursorX() const	{ return cursorLocal_.x; }
			/** The local y-position of the cursor in this window. */
			const CoordScalar& CursorY() const	{ return cursorLocal_.y; }
			/** The local coord position of the cursor in this window. */
			const Coord& CursorCoord() const	{ return cursorLocal_; }
			/** Signals that an important state change occurred. */
			void AlertParentOfStateChange() {
				GetParent().OnChildStateChange(*this);
			}
			/** Signals an update for this window and all of its children. */
			void UpdateAll(unsigned int deltaTime) {
				OnUpdate(deltaTime);
				for (WindowList::reverse_iterator itr = childList_.rbegin();
						itr != childList_.rend(); ++itr)
					(*itr)->UpdateAll(deltaTime);
			}
			/** Renders this window content within its parent's region. */
			template <class ClipperType>
			void RenderInParent(ClipperType& clipper
								,const CoordScalar& parentx
								,const CoordScalar& parenty) {
				if (!IsVisible())
					return;
				UpdateRenderingRect(parentx, parenty);
				typename ClipperType::ClipRegion clip(clipper
													,GetRenderingRect());
				OnRender();
				RenderChildren(clipper);
			}
			/** Renders all children at this window's rendering position. */
			template <class ClipperType>
			void RenderChildren(ClipperType& clipper) {
				for (WindowList::reverse_iterator itr = childList_.rbegin();
						itr != childList_.rend(); ++itr)
					(*itr)->RenderInParent(clipper
										,GetRenderingRect().x-OffsetX()
										,GetRenderingRect().y-OffsetY());
			}
			/** Removes this window from its parent's list of children. */
			void RemoveFromParent() {
				assert(HasParent());
				GetParent().childList_.erase(self_);
				parent_ = 0;
			}
		private:
			friend class Cursor;
			/* Sets this window's parent to the referenced window. */
			void SetParent(Window& parent, const WindowIterator& self) {
				assert(&parent!=this);
				parent_ = &parent;
				self_ = self;
			}
			/* Moves the given child to the top of this window's z-order.
			 * @param child The child to move to the top.
			 */
			void MoveChildToTop(Window& child) {
				// remove window from current position in list
				child.RemoveFromParent();
				// replace it on top
				childList_.push_front(&child);
				child.SetParent(*this, childList_.begin());
			}
			/* Selects the first pickable window directly under the cursor.
			 * The internal cursor positional state information is updated in
			 * this window and in its children.
			 */
			Window* SelectWindowUnderCursor(const Coord& pos) {
				Window* pChild = 0;
				if (IsPickable()) {
					cursorLocal_.x = pos.x-PosX();
					cursorLocal_.y = pos.y-PosY();
					if ((SizeScalar(CursorX()) < Width())
							&& (SizeScalar(CursorY()) < Height())) {
						for (WindowIterator itr = childList_.begin();
								itr != childList_.end(); ++itr) {
							pChild = (*itr)->SelectWindowUnderCursor(
															cursorLocal_);
							if (pChild != 0)
								return pChild;
						}
						pChild = this;
					}
				}
				return pChild;
			}
		private:
			WindowList		childList_;
			Coord			offset_, cursorLocal_;
			Window*			parent_;
			WindowIterator	self_;
			bool			pickable_;
		};
	}
}

#endif
