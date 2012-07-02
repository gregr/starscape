// rect.h
// Gregory Rosenblatt
// 5/8/05

#ifndef Starscape_Rect_H_
#define Starscape_Rect_H_

#include "util/scalar.h"

namespace Starscape {

	/** Creates a Rect using the given values. */
	inline Rect MakeRect(const CoordScalar& x, const CoordScalar& y
						,const SizeScalar& w, const SizeScalar& h) {
		Rect rect = { x, y, w, h };
		return rect;
	}

	/** Sets only the position data of a Rect. */
	inline void SetRectPos(Rect& rect
						,const CoordScalar& x, const CoordScalar& y) {
		rect.x = x;
		rect.y = y;
	}

	/** Sets only the dimensions of a Rect. */
	inline void SetRectSize(Rect& rect
						,const SizeScalar& w, const SizeScalar& h) {
		rect.w = w;
		rect.h = h;
	}

	/** Calculates the union of two rects, and stores it in the first.
	 * If the resulting Rect is larger than either of the originals then
	 * the union did not exist.
	 */
	inline void SetRectUnion(Rect& rect, const Rect& r) {
		CoordScalar rx2 = r.x + r.w;
		CoordScalar ry2 = r.y + r.h;
		CoordScalar rectx2 = rect.x + rect.w;
		CoordScalar recty2 = rect.y + rect.h;
		if (r.x > rect.x)
			rect.x = r.x;
		if (r.y > rect.y)
			rect.y = r.y;
		rect.w = ((rx2 < rectx2) ? rx2-rect.x : rectx2-rect.x);
		rect.h = ((ry2 < recty2) ? ry2-rect.y : recty2-rect.y);
	}

	/** Calculates the union of two rects and returns it.
	 * If the resulting Rect is larger than either of the originals then
	 * the union did not exist.
	 * @return A rect containing the union of the rect parameters.
	 */
	inline Rect CalcRectUnion(const Rect& first, const Rect& second) {
		Rect result = first;
		SetRectUnion(result, second);
		return result;
	}

	/** Constructs a Rect using streamed data. */
	template <class StreamType>
	Rect MakeRectFromStream(StreamType& in) {
		Rect rect;
		in >> rect.x >> rect.y >> rect.w >> rect.h;
		return rect;
	}
}

#endif
