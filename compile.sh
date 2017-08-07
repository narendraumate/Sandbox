#!/bin/bash

case "$OSTYPE" in
	#darwin*) xcodebuild -list -workspace Solution.xcworkspace | sed -n '/Schemes/,/^$/p' | grep -v "Schemes:" | while read scheme; do if [ "$scheme" == "SampleCommonGl" ] || [ "$scheme" == "SampleNativeGl" ] || [ "$scheme" == "SampleNativeMl" ] || [ "$scheme" == "SamplePuzzleMl" ] || [ "$scheme" == "SampleRiddleZa" ]; then xcodebuild -workspace Solution.xcworkspace -scheme ${scheme} -config Debug; xcodebuild -workspace Solution.xcworkspace -scheme ${scheme} -config Release; fi done;;
	darwin*) make verbose=1 config=debug_macos -j24;make verbose=1 config=release_macos -j24;;
	linux*) make verbose=1 config=debug_linux -j24;make verbose=1 config=release_linux -j24;;
esac
