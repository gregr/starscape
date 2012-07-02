// clipper.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Graphics_Clipper_H_
#define Starscape_Graphics_Clipper_H_

#include "graphics/config/opengl.h"
#include "util/rect.h"
#include "pooledvector.h"

namespace Starscape {

	namespace Graphics {

		/** Handles the clipping of rectangular regions.
		 * Clipping is enabled when this object is constructed.
		 * Stack-based clipping can be achieved using ClipRegion objects.
		 */
		class Clipper {
		public:
			/** Creates the clipper based on the passed display region.
			 * @param displayRegion The main display's rectangular region.
			 */
			Clipper(const Rect& displayRegion) {
				rectStack_.push_back(displayRegion);
				glEnable(GL_SCISSOR_TEST);
			}
			// disables clipping upon destruction
			~Clipper()	{ glDisable(GL_SCISSOR_TEST); }
			/** Resizes the region describing the main application display.
			 * When the display is resized this function must be called to
			 * synchronize the clipper.
			 * @param region The main display's rectangular region.
			 */
			void ResizeDisplayRegion(const Rect& region) {
				assert(!rectStack_.empty());
				rectStack_.front() = region;
				glEnable(GL_SCISSOR_TEST);
			}
		private:
			// non-copyable
			Clipper(const Clipper&);
			void operator=(const Clipper&);
			// clipping functionality
			void ClipRegion(const Rect& region) const {
				glScissor(region.x, rectStack_.front().h-(region.y+region.h)
						,region.w, region.h);
			}
			void ClipTopRegion() const {
				assert(!rectStack_.empty());
				ClipRegion(rectStack_.back());
			}
			// clip-region stack management
			void PushRegion(const Rect& region) {
				assert(!rectStack_.empty());
				Rect& back = rectStack_.back();
				Rect next = CalcRectUnion(back, region);
				if ((next.w > back.w) || (next.h > back.h))
					rectStack_.push_back(back);
				else
					rectStack_.push_back(next);
				ClipTopRegion();
			}
			void PopRegion() {
				assert(rectStack_.size()>1);
				rectStack_.pop_back();
				ClipTopRegion();
			}
			typedef PooledVector<Rect>::Type	RectStack;
			RectStack	rectStack_;
		public:
			friend class ClipRegion;
			/** Performs the stack-based clipping of a region using a Clipper.
			 * The clipping of this region is synchronized with this object.
			 */
			class ClipRegion {
			public:
				/** Clips the region that this object is constructed with.
				 * @param clipper The clipper to use as a stack.
				 * @param region The region to clip.
				 */
				ClipRegion(Clipper& clipper, const Rect& region)
					: clipper_(clipper)	{ clipper_.PushRegion(region); }
				// unclips this region upon destruction
				~ClipRegion()	{ clipper_.PopRegion(); }
			private:
				// non-copyable
				ClipRegion(const ClipRegion&);
				void operator=(const ClipRegion&);
				Clipper&	clipper_;
			};
		};
	}
}

#endif
