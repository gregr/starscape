// application.h
// Gregory Rosenblatt
// 5/20/05

#ifndef Starscape_App_Application_H_
#define Starscape_App_Application_H_

#include "app/interfacehandler.h"
#include "system/context.h"
#include "system/windowmanagement.h"
#include "util/timer.h"
#include "handleexception.h"

namespace Starscape {

	class Application {
	public:
		Application(const char* caption
					,unsigned int width, unsigned int height
					,bool resizable=true, bool fullscreen=false)
			: metrics_(context_.OpenWindow(width, height
										,resizable, fullscreen))
		{
			System::SetCaption(caption);
			System::HideCursor();
		}
		const Raster::DisplayMetrics& GetDisplayMetrics() const {
			return metrics_;
		}
		template <class InterfaceType>
		int Run(InterfaceType& iface) {
			try {
				Timer deltaTime;
				InterfaceHandler<InterfaceType> handler(iface);
				for (;;) {
					if (!context_.PollEvents(handler))
						break;
					iface.Update(deltaTime());
					iface.Render();
//					System::ShortPause();
				}
			}
			catch (...) {
				HandleException();
			}
			return 0;
		}
	private:
		System::Context			context_;
		Raster::DisplayMetrics	metrics_;
	};
}

#endif
