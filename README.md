This is the beginning of a 3D game programming system, including a GUI library built from scratch.

Drawing inspiration from the [CRAPL][Might], I've decided to excavate my first real C++ project.  This version is actually a rewrite of a rewrite of a rewrite of the original code I wrote as I learned both the C++ language, and how to program *at all*.  This code was born towards the end of 1999 and took a beating until May 2005.  I dropped this project soon after learning about and incorporating Python as an embedded interpreter for scripting.  Perhaps you can figure out why I suddenly stopped working on my C++ code.

The original graphics code was based on DirectDraw, with the first rewrite incorporating Direct3D.  However, after reading the popular [NeHe][NeHe] tutorials, I decided to use OpenGL instead.  Finally, I discovered [SDL][SDL], which let me more easily build something that was cross-platform.

This code originally used [RakNet][RakNet] for a proof-of-concept networking system (see the now-useless chat terminal in the demo), but it seemed like a PITA to revive that part of it.  RakNet is behind a registration wall.

Make no mistake: this is terrible code.  The [WTF][Eng] count was extremely high as I figured out how to get it working again.  If you study this for the purpose of learning how to design and write good code, you *will* experience brain damage.

Oh, and good luck getting it to build on Windows again.  I only bothered to make it build on Ubuntu before releasing it.  To build, install the requisite packages as mentioned in installation.txt, pray, then run make.
[Might]: http://matt.might.net/articles/crapl/
[NeHe]: http://nehe.gamedev.net/
[SDL]: http://www.libsdl.org/
[RakNet]: http://www.jenkinssoftware.com/
[Eng]: http://www.bernie-eng.com/blog/2011/03/07/wtf-count-the-only-true-metric-for-code-quality/
