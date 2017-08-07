REM Tools\Windows\premake4\premake4 --file=premake4.lua --os=windows clean
REM Tools\Windows\premake5\premake5 --file=premake5.lua --os=windows clean
MSBuild.exe Sandbox.sln /t:Clean /p:Configuration=Debug
MSBuild.exe Sandbox.sln /t:Clean /p:Configuration=Release
