// scalarpair.h
// Gregory Rosenblatt
// 5/10/05

#ifndef Starscape_ScalarPair_H_
#define Starscape_ScalarPair_H_

namespace Starscape {

	/** Represents a pair of scalar values. */
	template <typename ScalarType>
	struct ScalarPair {
		/** Constructs a (zero, zero) pair. */
		ScalarPair() : x(0), y(0)	{}
		/** Constructs a pair with the given x and y positions. */
		ScalarPair(const ScalarType& xpos, const ScalarType& ypos)
			: x(xpos), y(ypos)	{}
		/** Adds the value of c to this. */
		ScalarPair& operator+=(const ScalarPair& c) {
			x += c.x;
			y += c.y;
			return *this;
		}
		/** Subtracts the value of c from this. */
		ScalarPair& operator-=(const ScalarPair& c) {
			x -= c.x;
			y -= c.y;
			return *this;
		}
		ScalarType	x, y;
	};

	/** Calculates the sum of two scalar pairs. */
	template <typename ScalarType>
	inline ScalarPair<ScalarType> operator+(const ScalarPair<ScalarType>& c1
										,const ScalarPair<ScalarType>& c2) {
		return ScalarPair<ScalarType>(c1.x+c2.x, c1.y+c2.y);
	}

	/** Calculates the difference between two scalar pairs. */
	template <typename ScalarType>
	inline ScalarPair<ScalarType> operator-(const ScalarPair<ScalarType>& c1
										,const ScalarPair<ScalarType>& c2) {
		return ScalarPair<ScalarType>(c1.x-c2.x, c1.y-c2.y);
	}
}

#endif
