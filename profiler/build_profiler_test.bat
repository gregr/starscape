g++ -o ../bin/profiler_test -I%BOOST% -Id:\msys\1.0\local\include\SDL profiler.cpp profiler_test.cpp -Ld:\msys\1.0\local\lib -lSDL_ttf -lmingw32 -lSDLmain -lSDL -mwindows 2> errors.txt