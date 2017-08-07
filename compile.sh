#!/bin/bash

case "$OSTYPE" in
	#darwin*) xcodebuild -list -workspace Sandbox.xcworkspace | sed -n '/Schemes/,/^$/p' | grep -v "Schemes:" | while read scheme; do if [ "$scheme" == "SampleCommonGl" ] || [ "$scheme" == "SampleNativeGl" ] || [ "$scheme" == "SampleNativeMl" ] || [ "$scheme" == "SamplePuzzleMl" ] || [ "$scheme" == "SampleRiddleZa" ]; then xcodebuild -workspace Sandbox.xcworkspace -scheme ${scheme} -config Debug; xcodebuild -workspace Sandbox.xcworkspace -scheme ${scheme} -config Release; fi done;;
	darwin*) make config=debug -j24;make config=release verbose=true -j24;;
	linux*) make config=debug -j24;make config=release verbose=true -j24;;
esac
