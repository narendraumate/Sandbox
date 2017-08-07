#!/bin/bash

case "$OSTYPE" in
	#darwin*) ./Tools/MacOS/premake5/premake5 --file=premake5.lua --os=macosx xcode4;;
	darwin*) ./Tools/MacOS/premake5/premake5 --file=premake5.lua --os=macosx gmake;;
	linux*) ./Tools/Linux/premake5/premake5 --file=premake5.lua --os=linux gmake;;
esac
