// asciihextoint.h
// Gregory Rosenblatt
// 4/10/05

#ifndef Starscape_AsciiHexToInt_H_
#define Starscape_AsciiHexToInt_H_

namespace Starscape {

	inline int GetHexDigit(char ch) {
		if ((ch >= '0') && (ch <= '9'))
			return(ch - '0');
		if ((ch >= 'a') && (ch <= 'f'))
			return(ch - 'a' + 10);
		if ((ch >= 'A') && (ch <= 'F'))
			return(ch - 'A' + 10); 
		return -1;
	}

	/** Converts a hex value in ascii to an unsigned integer. */
	inline unsigned int AsciiHexToInt(const char* str) {
		unsigned int result = 0;
		int	digit;
		while((digit = GetHexDigit(*str++)) != -1)
			result = (result*16) + digit;
		return result;
	}
}

#endif
