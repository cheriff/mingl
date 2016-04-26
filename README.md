# mingl

Simple OpenGL (4.1) skeleton for MacOS.

Uses:
* [GLFW](https://github.com/glfw/glfw) for wrangling windows
* [GLM](https://github.com/g-truc/glm) for managing math

Caveats/Features:
 * Intended to be a series ot jumping off points for other projects
 * Mainly focussed on readable, factorable code rather than straight-out performance or 'good software engineering'
 * C++ for use with 3rd party libs, code is still mostly C-ish (printf/malloc/etc)
   - But 'auto' is nice!
 * Pulls in own dependancy libs. Hacking makefiles could probably link against globally installed Frameworks or static/dynamic libs
   - Such as if you have already installed glfw3 via other means.

Building:
 1. Ensure devtools are installed. Things like gcc, make should just work from a terminal
 2. GLFW requires [CMake](https://cmake.org/). Fairly sure I just installed via [Homebrew](http://brew.sh/)
 3. Clone this repo
 4. Type 'make'.

Dependancies are downloaded and built (if necessary) into extern/code. The file external_code.mk notes which revision of upstream repos are known to work on my machines at least. Later or earlier versions likely work fine, too.

Resulting headers and static libs are installed into extern/include and extern/lib for use by sample apps.

