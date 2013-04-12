// managedsurface.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Raster_ManagedSurface_H_
#define Starscape_Raster_ManagedSurface_H_

#include "raster/surface.h"
#include "exception.h"

namespace Starscape {

	namespace Raster {

		const char* const ERR_CREATE_NEW_SURFACE_CREATENEWDATA =
			"CreateNewSurface(): Could not create new surface data.";

		/* A wrapper to provide ManagedSurface with reference semantics.
		 * Allows ManagedSurface to be assigned (or constructed from)
		 * the result of a function which returns a ManagedSurface by value.
		 *
		 * All ManagedSurfaceRef interactions should happen behind the scenes.
		 */
		struct ManagedSurfaceRef {
			SDL_Surface*	surf_;
			explicit ManagedSurfaceRef(SDL_Surface* s)
				: surf_(s)	{}
		};
		
		/** A resource-managing derivative of the Surface class.
		 * Provides ownership transfer semantics.
		 */
		class ManagedSurface : public Surface {
		public:
			/** A ManagedSurface is initialized with a surface resource. */
			explicit ManagedSurface(SDL_Surface* surface = 0)
				: Surface(surface)	{}
			/** Copying transfers resource ownership. */
			ManagedSurface(ManagedSurface& ms) : Surface(ms.Release())	{}
			/** Assignment transfers resource ownership. */
			ManagedSurface& operator=(ManagedSurface& ms) {
				return Assign(ms);
			}
			/** Destroys the surface when this object is destroyed. */
			~ManagedSurface()	{ Destroy(); }
		public:
			// automatic conversions
			ManagedSurface(ManagedSurfaceRef ref)
				: Surface(ref.surf_)	{}
			ManagedSurface& operator=(ManagedSurfaceRef ref) {
				Reset(ref.surf_);
				return *this;
			}
			operator ManagedSurfaceRef() {
				return ManagedSurfaceRef(Release());
			}
		protected:
			// resets the surface data and destroys the old.
			void Reset(SDL_Surface* surface) {
				if (surface != surface_) {
					Destroy();
					surface_ = surface;
				}
			}
			// relinquishes control of the surface
			SDL_Surface* Release() {
				SDL_Surface* pSurf = surface_;
				surface_ = 0;
				return pSurf;
			}
			// takes over ownership of another surface
			ManagedSurface& Assign(ManagedSurface& ms) {
				Reset(ms.Release());
				return *this;
			}
		};

		/** Creates a new managed surface with the given dimensions. */
		inline ManagedSurface CreateNewSurface(unsigned int width
											,unsigned int height) {
			ManagedSurface surface;
			if (!surface.CreateNewData(width, height))
				throw Exception(ERR_CREATE_NEW_SURFACE_CREATENEWDATA
								,SDL_GetError());
			return surface;
		}
	}
}

#endif
