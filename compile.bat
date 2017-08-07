REM MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:Build

MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:SampleCommonGl
MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:SampleNativeDx
MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:SampleNativeGl
MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:SampleNativeMl
MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:SampleCommonVk
MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:SamplePuzzleMl
MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:SamplePuzzleVk

REM C++ 17 Issues
REM MSBuild.exe Sandbox.sln /m /p:Configuration=Debug /t:SampleRiddleZa
