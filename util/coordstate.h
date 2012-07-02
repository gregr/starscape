// coordstate.h
// Gregory Rosenblatt
// 3/16/05

#ifndef Starscape_CoordState_H_
#define Starscape_CoordState_H_

#include "util/coord.h"

namespace Starscape {

	/** A coordinate that retains information about its last state. */
	class CoordState {
	private:
		Coord	coord_, coordOld_;
	public:
		/** Constructs a zero-ed coordinate for each state. */
		CoordState()	{}
		/** Constructs an identical coordinate for each state. */
		explicit CoordState(const Coord& c) : coord_(c), coordOld_(c)	{}
		/** Constructs using the given state information. */
		CoordState(const Coord& current, const Coord& old)
			: coord_(current), coordOld_(old)	{}
		/** Constructs a copy. */
		CoordState(const CoordState& cs)
			: coord_(cs.coord_), coordOld_(cs.coordOld_)	{}
		/** Copies the given state information. */
		CoordState& operator=(const CoordState& cs) {
			coord_ = cs.coord_;
			coordOld_ = cs.coordOld_;
			return *this;
		}
		/** Assigns a new coordinate to the state information. */
		CoordState& operator=(const Coord& coord) {
			coordOld_ = coord_;
			coord_ = coord;
			return *this;
		}
		/** Retrieves the current coordinate state. */
		const Coord& GetCoord() const	{ return coord_; }
		/** Calculates the change in position since the last state. */
		Coord CalcRelCoord() const	{ return coord_-coordOld_; }
	};

	/** Updates the state information with a new coordinate. */
	inline void UpdateCoordState(CoordState& cs, const Coord& coord) {
		cs = coord;
	}

	/** Resets all of the state information to the given coordinate. */
	inline void ResetCoordState(CoordState& cs, const Coord& c) {
		cs = CoordState(c);
	}
}

#endif
