// command.h
// Gregory Rosenblatt
// 3/21/05

#ifndef Starscape_Gui_Command_H_
#define Starscape_Gui_Command_H_

#include <boost/function.hpp>

namespace Starscape {

	namespace Gui {

		/** A function object without arguments or return type.
		 * Compatible with objects created by boost::bind().
		 */
		typedef boost::function<void ()>		Command;

		/** A function object with a boolean switch parameter.
		 * Compatible with objects created by boost::bind().
		 */
		typedef boost::function<void (bool)>	SwitchCommand;
	}
}

#endif
