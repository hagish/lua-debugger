##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
## Update:      2008/12 by Werner Smekal
##---------------------------------------------------------------------------
 
# define minimum cmake version
cmake_minimum_required(VERSION 2.6.2)

SET(CMAKE_BUILD_TYPE Debug)

# Our project is called 'minimal' this is how it will be called in
# visual studio, and in our makefiles. 
project(lua-debugger)
 
# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
 
##---------------------------------------------------
## Please set your wxWidgets configuration here
##--------------------------------------------------- 
 
# Here you can define what libraries of wxWidgets you need for your
# application. You can figure out what libraries you need here;
# http://www.wxwidgets.org/manuals/2.8/wx_librarieslist.html
# We need the Find package for wxWidgets to work
# NOTE: if you're using aui, include aui in this required components list.
 
# It was noticed that when using MinGW gcc it is essential that 'core' is mentioned before 'base'.
find_package(wxWidgets COMPONENTS core base REQUIRED)
 
##---------------------------------------------------
## Actual config file starts here
##--------------------------------------------------- 
 
# wxWidgets include (this will do all the magic to configure everything)
include( ${wxWidgets_USE_FILE} )
 
# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
set(SRCS 
	../main.cpp 
	../Server.cpp
	../DebugThread.cpp 
	../gui/Layout.cpp 
	../gui/DebugPanel.cpp 
	../gui/MainFrame.cpp 
	../utils/StringUtils.cpp
	../utils/FileUtils.cpp
	
	../wxscintilla_1.69.2/src/wxscintilla.cpp
	../wxscintilla_1.69.2/src/PlatWX.cpp
	../wxscintilla_1.69.2/src/scintilla/src/LexSmalltalk.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexSpice.cxx
	../wxscintilla_1.69.2/src/scintilla/src/WindowAccessor.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexLisp.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexVB.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexTeX.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexOthers.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexMSSQL.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexForth.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexPB.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexMMIXAL.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexLua.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexHTML.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexHaskell.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexAPDL.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexRebol.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexTADS3.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexEiffel.cxx
	../wxscintilla_1.69.2/src/scintilla/src/ViewStyle.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexAVE.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexSpecman.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexBullant.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexVHDL.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexBash.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexPerl.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexPS.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexVerilog.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexMPT.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexOpal.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexCsound.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexBasic.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexFortran.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexCrontab.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexCPP.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexKix.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexCLW.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexCSS.cxx
	../wxscintilla_1.69.2/src/scintilla/src/RESearch.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexPOV.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexCaml.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexAsm.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexScriptol.cxx
	../wxscintilla_1.69.2/src/scintilla/src/KeyWords.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexErlang.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexNsis.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexAda.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexBaan.cxx
	../wxscintilla_1.69.2/src/scintilla/src/KeyMap.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexPascal.cxx
	../wxscintilla_1.69.2/src/scintilla/src/AutoComplete.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexEScript.cxx
	../wxscintilla_1.69.2/src/scintilla/src/Editor.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexConf.cxx
	../wxscintilla_1.69.2/src/scintilla/src/Indicator.cxx
	../wxscintilla_1.69.2/src/scintilla/src/DocumentAccessor.cxx
	../wxscintilla_1.69.2/src/scintilla/src/CallTip.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexAU3.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexFlagship.cxx
	../wxscintilla_1.69.2/src/scintilla/src/ExternalLexer.cxx
	../wxscintilla_1.69.2/src/scintilla/src/Style.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexPython.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexInno.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexAsn1.cxx
	../wxscintilla_1.69.2/src/scintilla/src/XPM.cxx
	../wxscintilla_1.69.2/src/scintilla/src/Document.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LineMarker.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexYAML.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexMetapost.cxx
	../wxscintilla_1.69.2/src/scintilla/src/PropSet.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexRuby.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexSQL.cxx
	../wxscintilla_1.69.2/src/scintilla/src/UniConversion.cxx
	../wxscintilla_1.69.2/src/scintilla/src/ContractionState.cxx
	../wxscintilla_1.69.2/src/scintilla/src/CellBuffer.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexLout.cxx
	../wxscintilla_1.69.2/src/scintilla/src/ScintillaBase.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexTCL.cxx
	../wxscintilla_1.69.2/src/scintilla/src/StyleContext.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexMatlab.cxx
	../wxscintilla_1.69.2/src/scintilla/src/CharClassify.cxx
	../wxscintilla_1.69.2/src/scintilla/src/LexGui4Cli.cxx
	../wxscintilla_1.69.2/src/ScintillaWX.cpp
) 
INCLUDE_DIRECTORIES(
	../ 
	../gui 
	../utils
	../wxscintilla_1.69.2/include
	../wxscintilla_1.69.2/src/scintilla/include
	../wxscintilla_1.69.2/src/scintilla/src
)

add_definitions( -DGTK )
add_definitions( -DSCI_LEXER )
add_definitions( -DLINK_LEXERS )
 
# Here we define the executable minimal.exe or minimal on other systems
# the above paths and defines will be used in this build
add_executable(../lua-debugger WIN32 ${SRCS})

FIND_PACKAGE(Boost 1.42 COMPONENTS regex REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

find_library(POCO_FOUNDATION PocoFoundation)
find_library(POCO_NET PocoNet)
find_library(POCO_UTIL PocoUtil)

find_package(Threads REQUIRED)
target_link_libraries(../lua-debugger ${CMAKE_THREAD_LIBS_INIT})

# We add to our target 'minimal' the wxWidgets libraries. These are
# set for us by the find script. If you need other libraries, you
# can add them here as well. 
target_link_libraries(../lua-debugger ${wxWidgets_LIBRARIES})
target_link_libraries(../lua-debugger ${Boost_LIBRARIES})
target_link_libraries(../lua-debugger ${POCO_FOUNDATION} ${POCO_NET} ${POCO_UTIL})