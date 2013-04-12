// interface.h
// Gregory Rosenblatt
// 5/13/05

#ifndef Starscape_Gui_Interface_H_
#define Starscape_Gui_Interface_H_

#include "gui/appearancemanager.h"
#include "gui/cursorbrushstate.h"
#include "gui/cursor.h"
#include "gui/config/clipper.h"
#include "gui/config/display.h"

namespace Starscape {

	namespace Gui {

		/** A window abstraction of the system window.
		 * This is the base class for a GUI's root window.
		 */
		class Interface : public Window {
		public:
			/** Creates an Interface with the specified dimensions.
			 * @param renderer The renderer to use when drawing the GUI.
			 * @param cursorImage The default cursor image to draw.
			 */
			Interface(const Raster::DisplayMetrics& metrics
					,AppearanceManager& appearance)
				: Window(0, 0, metrics.Width(), metrics.Height())
				,displayMetrics_(metrics)
				,clipper_(MakeRect(0, 0, displayMetrics_.Width()
							,displayMetrics_.Height()))
				,activeState_(*this)
				,cursor_(*this, activeState_, cursorBrush_)
				,appearance_(appearance), active_(true)
			{
				Input::EnableUnicode();
				Graphics::SetOrthoViewport(displayMetrics_.Width()
										,displayMetrics_.Height());
			}
			~Interface()	{ Input::DisableUnicode(); }
			/** Propogates a signal to update through the entire GUI. */
			void Update(unsigned int deltaTime=0)	{ UpdateAll(deltaTime); }
			/** Renders the entire GUI and cursor. */
			void Render() {
				if (!active_)
					return;
				Graphics::BeginRender();
				OnRender();
				RenderChildren(clipper_);
				cursorBrush_.Draw(GetCursor().PosX(), GetCursor().PosY());
				Graphics::EndRender();
			}
			/** Resizes the interface when the display is resized. */
			void Resize() {
				Graphics::SetOrthoViewport(displayMetrics_.Width()
										,displayMetrics_.Height());
				clipper_.ResizeDisplayRegion(MakeRect(0, 0
											,displayMetrics_.Width()
											,displayMetrics_.Height()));
				appearance_.ReacquireResources();
				SetSize(displayMetrics_.Width(), displayMetrics_.Height());
				UpdateRenderingRect(0, 0);
				cursor_.ResetFreeRegion();
				OnResize();
			}
			/** Handles a change in the active-state */
			void Activate(bool isActive) {
				active_ = isActive;	// instead Render() function pointer?
			}
			/** The window that is currently active. */
			Window& ActiveWindow() {
				return activeState_.GetActiveFocus();
			}
			/** The window that has keyboard-focus. */
			Window& KeyboardInputFocus() {
				return activeState_.GetKeyboardFocus();
			}
			/** The cursor representation. */
			Cursor& GetCursor()	{ return cursor_; }
			// message handling
			void OnKeyboardGrab(ActiveState& state) {
				state.SetKeyboardFocus(*this);
			}
		protected:
			/** References the dimensions of the display.
			 * @return A reference to the system's current display metrics.
			 */
			const Raster::DisplayMetrics& GetDisplayMetrics() const {
				return displayMetrics_;
			}
//			/** Retrieve the appearance manager for this interface. */
//			AppearanceManager& GetAppearance()	{ return appearance_; }
			/** Sets the default image to use when drawing the cursor. */
			void SetDefaultCursorBrush(const CursorBrush& brush) {
				cursorBrush_.SetDefaultBrush(brush);
			}
			/* Default blank rendering. */
			void OnRender()	{}
		private:
			Raster::DisplayMetrics	displayMetrics_;
			Graphics::Clipper		clipper_;
			ActiveState				activeState_;
			CursorBrushState		cursorBrush_;
			Cursor					cursor_;
			AppearanceManager&		appearance_;
			bool					active_;
		};
	}
}

#endif
