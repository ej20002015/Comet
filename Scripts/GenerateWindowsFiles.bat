@echo off
pushd %~dp0\..\
call Vendor\Premake5\premake5.exe vs2019
popd
PAUSE