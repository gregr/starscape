// interpreter.h
// Gregory Rosenblatt
// 4/25/05

#ifndef Starscape_Python_Interpreter_H_
#define Starscape_Python_Interpreter_H_

#include "Python.h"
#include "python/output.h"

namespace Starscape {

	namespace Log {
		class Logger;
	}

	namespace Python {

		const char* const PROMPT_PRIMARY =	">>> ";
		const char* const PROMPT_SECONDARY =	"... ";

		void InitializeOutput();

		/** Prints the version string for the interpreter. */
		inline void PrintVersion() {
			PrintString("Python ");
			PrintString(Py_GetVersion());
			PrintString("\n");
		}

		/** Manages the initialization of the interpreter. */
		class Interpreter {
		public:
			Interpreter() {
				assert(!Py_IsInitialized());
				Py_Initialize();
				InitializeOutput();
				PrintString("Starting the embedded interpreter...\n");
				PrintVersion();
			}
			~Interpreter()	{ Py_Finalize(); }
		};

		/** Retrieves the logger that python uses for output. */
		Log::Logger& GetOutputLogger();

		/** Processes a string through the interpreter.
		 * @return 0 on success, -1 if there was a python exception.
		 */
		inline int ExecuteString(const char* str) {
			return PyRun_SimpleString(str);
		}

		int ExecuteInteractiveMultilineString(const char* str);

		int ExecuteInteractiveString(const char* str);

		/** Determines whether to proceed with secondary input. */
		template <class StringType>
		bool CheckPrimaryInput(const StringType& input) {
			if (!input.empty()) {
				if (input[input.length()-1] == ':')
					return true;
			}
			return false;
		}

		/** Determines whether to continue secondary input. */
		template <class StringType>
		bool CheckSecondaryInput(const StringType& input) {
			return !input.empty();
		}
	}
}

#endif
