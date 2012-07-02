// coord.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_Coord_H_
#define Starscape_Coord_H_

#include "util/scalarpair.h"
#include "util/scalar.h"

namespace Starscape {

	/** A pair of dimensions representing a location. */
	typedef ScalarPair<CoordScalar>	Coord;
//	struct Coord {
		/** Constructs a zero-ed coordinate. */
//		Coord() : x(0), y(0)	{}
		/** Constructs a coordinate at the given x and y positions. */
//		Coord(const CoordScalar& xpos, const CoordScalar& ypos)
//			: x(xpos), y(ypos)	{}
		/** Adds the value of c to this. */
//		Coord& operator+=(const Coord& c) {
//			x += c.x;
//			y += c.y;
//			return *this;
//		}
		/** Subtracts the value of c from this. */
//		Coord& operator-=(const Coord& c) {
//			x -= c.x;
//			y -= c.y;
//			return *this;
//		}
//		CoordScalar	x, y;
//	};

	/** Calculates the sum of two coordinate values. */
//	inline Coord operator+(const Coord& c1, const Coord& c2) {
//		return Coord(c1.x+c2.x, c1.y+c2.y);
//	}

	/** Calculates the difference between two coordinate values. */
//	inline Coord operator-(const Coord& c1, const Coord& c2) {
//		return Coord(c1.x-c2.x, c1.y-c2.y);
//	}
}

#endif
