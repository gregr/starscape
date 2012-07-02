// line.h
// Gregory Rosenblatt
// 2/16/05

#ifndef Starscape_Line_H_
#define Starscape_Line_H_

#include "util/coord.h"

namespace Starscape {

	/** Represents a line in two-dimensional space. */
	struct Line {
		Line()	{}
		Line(const Coord& c1, const Coord& c2)
			: a(c1), b(c2)	{}
		Coord	a, b;
	};
}

#endif
