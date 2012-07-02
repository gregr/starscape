// context.h
// Gregory Rosenblatt
// 5/17/05

#ifndef Starscape_System_Context_H_
#define Starscape_System_Context_H_

#include "raster/display.h"
#include "log/log.h"

namespace Starscape {

	namespace System {

		const char* const ERR_CONTEXT_INIT =
			"System: Context initialization has failed.";

		/** The SDL windowing and event-receiving context.
		 * Manages the main application window and dispatches events.
		 */
		class Context {
		public:
			/** Creates a context supporting the SDL subsystems indicated.
			 * The video subsystem is supported by default.
			 * @param flags Indicates the desired subsystems to initialize.
			 */
			explicit Context(Uint32 flags = SDL_INIT_VIDEO)	{
				if (SDL_Init(flags) != 0)
					throw Exception(ERR_CONTEXT_INIT, SDL_GetError());
			}
			// shuts down SDL
			~Context()	{ SDL_Quit(); }
			/** Opens the main application window.
			 * @param width The window width.
			 * @param height The window height.
			 * @param bpp The bit depth.  0 to use the current display mode.
			 * @param caption The caption.
			 * @param fullscreen Whether to open in fullscreen mode.
			 * @param resizable Whether to provide a resizing border.
			 * @return The display metrics of the application window opened.
			 */
			Raster::DisplayMetrics OpenWindow(unsigned int width
												,unsigned int height
												,bool resizable=true
												,bool fullscreen=false) {
				if (resizable)
					display_.SetResizable();
				if (fullscreen)
					display_.SetFullscreen();
				display_.Open(width, height);
				Log::Logger& logger = Log::Get();
				logger << "Opened a new system window:"
					<< "\n\t-Dimensions: " << width << " x " << height
					<< "\n\t-Resizable?: " << resizable
					<< "\n\t-Fullscreen?: " << fullscreen << "\n";
				logger.Flush();
				return Raster::DisplayMetrics(display_);
			}
			/** Checks the message queue for new events to dispatch.
			 * @param handler The event handler to dispatch messages to.
			 * @return false if the application should quit, true otherwise.
			 */
			template <class HandlerType>
			bool PollEvents(HandlerType& handler) {
				while (SDL_PollEvent(&event_)) {
					switch (event_.type) {
					case SDL_MOUSEMOTION:
						handler.OnMouseMotion(event_.motion.x
											,event_.motion.y
											,event_.motion.xrel
											,event_.motion.yrel);
						break;
					case SDL_MOUSEBUTTONDOWN:
						handler.OnMouseButtonDown(event_.button.button);
						break;
					case SDL_MOUSEBUTTONUP:
						handler.OnMouseButtonUp(event_.button.button);
						break;
					case SDL_KEYDOWN:
						handler.OnKeyDown(event_.key.keysym);
						break;
					case SDL_KEYUP:
						handler.OnKeyUp(event_.key.keysym);
						break;
					case SDL_VIDEORESIZE:
						display_.Resize(event_.resize.w, event_.resize.h);
						{
							Log::Logger& logger = Log::Get();
							logger << "System window resized:"
								<< "\n\t-New Dimensions: "
								<< event_.resize.w << " x " << event_.resize.h
								<< "\n";
							logger.Flush();
						}
						handler.OnResize();	// width and height unnecessary
						break;
					case SDL_ACTIVEEVENT:
						if (event_.active.state & SDL_APPACTIVE) {
							Log::Logger& logger = Log::Get();
							if (event_.active.gain)
								logger << "System window regained focus.\n";
							else
								logger << "System window minimized.\n";
							logger.Flush();
							handler.OnActivate(static_cast<bool>(
												event_.active.gain));
						}
						break;
					case SDL_QUIT:
						if (handler.OnQuit()) {
							Log::Logger& logger = Log::Get();
							logger << "Quit message: shutting down...\n";
							logger.Flush();
							return false;
						}
						break;
					}
				}
				return true;
			}
		private:
			Raster::Display	display_;
			SDL_Event		event_;
		};
	}
}

#endif
