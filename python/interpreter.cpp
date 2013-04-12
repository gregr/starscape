// interpreter.cpp
// Gregory Rosenblatt
// 5/11/05

#include "python/interpreter.h"
#include "python/starscape_output_wrap.cxx"
#include "pooledstring.h"

namespace Starscape {

	namespace Python {

		void InitializeOutput() {
			SWIG_init();
			ExecuteString("import sys\n"
						"sys.path.append('')\n"
						"import starscape_output\n"
						"class StdoutCatcher:\n"
						"\tdef write(self, str):\n"
						"\t\tstarscape_output.PrintString(str)\n"
						"class StderrCatcher:\n"
						"\tdef write(self, str):\n"
						"\t\tstarscape_output.PrintString(str)\n"
						"sys.stdout = StdoutCatcher()\n"
						"sys.stderr = StderrCatcher()\n"
						"import code\n"
						"import threading\n"
						"import thread\n"
						"def killswitch():\n"
						"\tthread.interrupt_main()\n"
						"\tprint \"Script terminated due to system hang.\"\n"
						"console = code.InteractiveConsole(locals())");
		}

		void PrepareString(PooledString& str) {
			PooledString::iterator itr = str.begin();
			for (; itr != str.end(); ++itr) {
				switch (*itr) {
				case '\"':
				case '\\':
					itr = str.insert(itr, '\\');
					++itr;
					break;
				default:
					break;
				}
			}
		}

		int ExecuteStringGuarded(const char* str) {
			ExecuteString("consoleTimer = threading.Timer(3.0, killswitch)\n"
							"consoleTimer.start()\n");
			int result = ExecuteString(str);
			ExecuteString("consoleTimer.cancel()");
			return result;
		}

		int ExecuteInteractiveMultilineString(const char* str) {
			return ExecuteStringGuarded(str);
		}

		int ExecuteInteractiveString(const char* str) {
			PooledString input = str;
			PrepareString(input);
			input = PooledString("console.push(\"") + input + "\")";
			return ExecuteStringGuarded(input.c_str());
		}
	}
}
