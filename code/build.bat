@echo off
IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
set compiler_flags=-Zi
set additional_include=w:\external_libs\SDL2-2.0.3\include
set library_dirs=w:\external_libs\SDL2-2.0.3\lib\x64
set additional_depend=-incremental:no -opt:ref SDL2.lib
cl %compiler_flags% ..\code\win32_main.cpp /I %additional_include% ^
/link %additional_depend% /LIBPATH:%library_dirs%
popd