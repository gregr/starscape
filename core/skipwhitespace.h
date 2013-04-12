// skipwhitespace.h
// Gregory Rosenblatt
// 11/20/04

#ifndef Starscape_SkipWhitespace_H_
#define Starscape_SkipWhitespace_H_

#include <istream>
#include <cctype>

namespace Starscape {

	// skips whitespace characters to find
	// the next portion of data to be read
	inline void SkipWhitespace(std::istream& in) {
		while (std::isspace(in.get()));
		in.unget();
	}

	// endian-independent serialization and deserialization of primitive types
	// compressing serialization and deserialization functions
	// (i.e. unsigned integers 7-bit for data
	// and hijack 1-bit to tell if there's another byte)
}

#endif
