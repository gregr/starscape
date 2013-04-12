// makepoweroftwo.h
// Gregory Rosenblatt
// 3/16/05

#ifndef Starscape_MakePowerOfTwo_H_
#define Starscape_MakePowerOfTwo_H_

namespace Starscape {

	/** Gets the first power of two larger than the given dimension. */
	inline unsigned int MakePowerOfTwo(unsigned int dimension) {
		unsigned int expansion = 1;
		while (expansion < dimension)
			expansion <<= 1;
		return expansion;
	}
}

#endif
