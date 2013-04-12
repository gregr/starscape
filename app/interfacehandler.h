// interfacehandler.h
// Gregory Rosenblatt
// 5/7/05

#ifndef Starscape_App_InterfaceHandler_H_
#define Starscape_App_InterfaceHandler_H_

#include "gui/cursor.h"
#include "input/mouse.h"
#include "system/eventhandler.h"

namespace Starscape {

	/** An event handler that to dispatches messages to a GUI. */
	template <class InterfaceType>
	class InterfaceHandler : public System::EventHandler {
	public:
		/** Creates a handler to dispatch messages to the given interface.
		 * @param interface The main interface in a GUI hierarchy.
		 * @param cursor The cursor to bind mouse input to.
		 */
		InterfaceHandler(InterfaceType& intrf)
			: intrf_(intrf), mouse_(intrf.GetCursor())	{}
		/** Informs the GUI that the system window has been resized. */
		void OnResize()	{ intrf_.Resize(); }
		/** Notifies the GUI of a change in the active-state. */
		void OnActivate(bool isActive) {
			intrf_.Activate(isActive);
		}
		/** Dispatches a key-down message to the active window. */
		void OnKeyDown(const Input::KeyInfo& key) {
			intrf_.KeyboardInputFocus().OnKeyDown(key);
		}
		/** Dispatches a key-up message to the active window. */
		void OnKeyUp(const Input::KeyInfo& key) {
			intrf_.KeyboardInputFocus().OnKeyUp(key);
		}
		/** Synchronizes the cursor with mouse movement. */
		void OnMouseMotion(int x, int y, int xrel, int yrel) {
			mouse_.OnMotion(x, y, xrel, yrel);
		}
		/** Sends a mouse-button-down message through the cursor. */
		void OnMouseButtonDown(int button) {
			mouse_.OnButtonDown(button);
		}
		/** Sends a mouse-button-up message through the cursor. */
		void OnMouseButtonUp(int button) {
			mouse_.OnButtonUp(button);
		}
	private:
		InterfaceType&				intrf_;
		Input::Mouse<Gui::Cursor>	mouse_;
	};
}

#endif
