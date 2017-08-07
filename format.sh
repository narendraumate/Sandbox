#!/bin/sh

find Library/Common/native Sample Source Verify \( -name \*.mm -o -name \*.m -o -name \*.h -o -name \*.cpp \) -print | xargs clang-format -i -style=file
