#!/bin/bash

case "$OSTYPE" in
	#darwin*) ./Tools/MacOS/premake5/premake5 --file=premake5.lua --os=macosx clean;rm -rf Build;rm -rf *.xc*/;;
	darwin*) ./Tools/MacOS/premake5/premake5 --file=premake5.lua --os=macosx clean;rm -rf Build;rm Makefile *.make;;
	linux*) ./Tools/Linux/premake5/premake5 --file=premake5.lua --os=linux clean;rm -rf Build;rm Makefile *.make;;
esac
