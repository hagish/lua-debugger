lua-debugger v0.1
=================

A small and very basic graphical lua debugger based on a modified version of RemDebug.

features
--------
* crossplatform (but there are only buildfiles for linux so far)
* breakpoints
* interactive expression evaluation (look at content of lua variables)
* stepping
* easy integration in embedded lua
* support multiple lua states in parallel
* lua and debugger communicates via tcp sockets

lua requirements for using debug engine
---------------------------------------
* lua is allowed to load/require modules
* lfs - http://keplerproject.github.com/luafilesystem/
* luasocket - http://w3.impa.br/~diego/software/luasocket/
* access to lua debug methods like debug.sethook

known problems
--------------
* lua debug instrumentation is slow

install
-------
in linux: cd build ; cmake . ; make ; cd .. ; ./lua-debugger

usage
-----
* load and start debug engine into your lua code
  
  require"remdebug.engine"
  remdebug.engine.start()
  
* you can find remdebug under remdebug/src
* perhaps it is necessary to add remdebug to your LUA paths

  # remdebug, lfs, luasocket
  LUA_PATH="/usr/share/lua/5.1/?.lua;PATH_TO_REMDEBUG_IN_LUA_DEBUGGER/src/?.lua;$LUA_PATH"
  export LUA_PATH
  # lfs, luasocket
  LUA_CPATH="/usr/lib/lua/5.1/?.so;$LUA_CPATH"
  export LUA_CPATH

* startup the debug gui
* start your lua application
* lua-debugger should open a tab for each lua that executing remdebug
* currently remdebug connects to localhost (search for localhost in the remdebug directory to change this)

license
-------
* lua-debugger - MIT
* RemDebug - http://www.keplerproject.org/remdebug/license.html (~MIT)
* wxWidget - wxWindows Licence - http://www.wxwidgets.org/about/newlicen.htm (~LGPL)
* wxScintilla - same as wxWindows/wxWidget
* icons - Public Domain - http://tango.freedesktop.org/Tango_Desktop_Project

credits
-------
* RemDebug - http://www.keplerproject.org/remdebug
* wxWidget - http://www.wxwidgets.org/
* wxScintilla - http://wxcode.sourceforge.net/components/wxscintilla/
* wxFormBuilder - http://wxformbuilder.org/
* cmake - http://www.cmake.org/
* Tango Desktop Project - http://tango.freedesktop.org/Tango_Desktop_Project
* boost - http://www.boost.org/
* libpoco - http://pocoproject.org/

similar tools
-------------
* https://github.com/jjensen/lua-tilde - C# windows
* http://code.google.com/p/lua-debugger/ - Toy debugger
