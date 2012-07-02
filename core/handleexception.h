// handleexception.h
// Gregory Rosenblatt
// 11/21/04

#ifndef Starscape_HandleException_H_
#define Starscape_HandleException_H_

#include <iostream>
#include <exception>

namespace Starscape {

	/** The standard exception handler. */
	inline void HandleException() {
		try {
			throw;	// rethrow the exception to see what type it is
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "Unhandled exception!" << std::endl;
		}
	}
}

#endif
