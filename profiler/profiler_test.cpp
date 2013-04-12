// main.cpp
// Gregory Rosenblatt
// Update 1/23/05

// starscape
#include "exception.h"
#include "profilelogger.h"
// while you're at it, test that this compiles
#include "resourcemanager.h"
#include <sdl.h>
#include <iostream>
#include <fstream>
using namespace Starscape;
using namespace std;

const char* const LOGGER_OUTPUT_FILE = "profilelog.txt";

class IntLoader {
public:
	typedef int	ResourceType;
	typedef PooledString	KeyType;
	typedef int& ResourceRefType;
	IntLoader() : add_(0)	{}
	IntLoader(int addend) : add_(addend)	{}
	int operator()(const PooledString& str) {
		return str.length()+add_;
	}
private:
	int	add_;
};

typedef ResourceManager<IntLoader>	IntManager;

int main(int argc, char* argv[]) {

	try {
		SDL_Init(0);
		atexit(SDL_Quit);

		PooledString resKey = "BAAAAAAA hahahaha";
		PooledString resKey2 = "BAAAAAAA hohohoho";
		IntManager manager;
		bool result = manager.IsLoaded(resKey);
		if (result)
			cerr << "how the hell is it already loaded?!" << endl;
		int& resourceHandle = manager.Get(resKey);
		cout << "Resource Value is: " << resourceHandle
			<<" and should be: " << resKey.length() << endl;
		result = manager.IsLoaded(resKey);
		if (!result)
			cerr << "why isn't this already loaded?!?!" << endl;
		int& handle2 = manager.Get(resKey);
		cout << "Equal vals?: " << (resourceHandle == handle2) << endl;
		cout << "Identical addresses?: " << (&resourceHandle == &handle2) << endl;
		
		int& addedVal = manager.Get(resKey2, IntLoader(5));
		cout << "addedVal: " << addedVal << endl;
		
		// data
		int a=0, b=0, c=1, x=200, y=300, z=0;
		int i, j;
		// test profiler
		{
		PROFILE("Main Loop")
		for (i = 0; i < 10000; ++i) {
			PROFILE("Inside Loop")
			{
				PROFILE("Series 1")
				{
					PROFILE("Square")
					z = x*x + y*y;
					x += c;
					y -= c;
				}

				{
					PROFILE("Cubic")
					z = x*x*y + y*y*x;
					x += c;
					y -= c;
				}

				{
					PROFILE("Linear")
					if (x == 0) x = 1;
					if (y == 0) y = 1;
					z = x*x/y + y*y/x;
					x += c*2;
					y += c*2;
				}
			}

			{
				PROFILE("Series 2")
				for (j = 0; j < 10; ++j) {
					PROFILE("Inside Loop")
					if (j % 2)
					{
						PROFILE("Odd")
						++a;
					}
					else
					{
						PROFILE("Even")
						++b;
					}
					c = 4*a-3*b;
				}
			}
		}
		}
		ProfileLogger profLogger;
		profLogger.AddChannel(cout);
		profLogger.LogResults();
		profLogger.ResetProfiler();
		ofstream fout(LOGGER_OUTPUT_FILE);
		if (!fout)
			throw Exception("Unable to open log file.");
		profLogger.WriteHistoryToStream(fout);
	}
	catch (Starscape::Exception& e) {
		cerr << "Starscape exception caught!" << endl;
		cerr << "Error: " << e.what() << endl;
	}
	catch (...) {
		cerr << "unhandled exception!" << endl;
		return 1;
	}
	return 0;
}
