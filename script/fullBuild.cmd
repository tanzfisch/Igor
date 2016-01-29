@echo off

set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.exe"

cd ..\src\Igor\vs12

echo Build Debug ...
%DEVENV% IgorSDKFullBuild.sln /Rebuild Debug
echo Build Release ...
%DEVENV% IgorSDKFullBuild.sln /Rebuild Release
echo done