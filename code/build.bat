@echo off
IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
set compiler_flags=-Zi
set additional_include=/I w:\external_libs\SDL2-2.0.3\include ^
                       /I w:\external_libs\SDL2_image-2.0.0\include ^
                       /I w:\external_libs\SDL2_mixer-2.0.0\include ^
                       /I w:\external_libs\SDL2_ttf-2.0.12\include 
set library_dirs=/LIBPATH:w:\external_libs\SDL2-2.0.3\lib\x64 ^
                 /LIBPATH:w:\external_libs\SDL2_image-2.0.0\lib\x64 ^
                 /LIBPATH:w:\external_libs\SDL2_mixer-2.0.0\lib\x64 ^
                 /LIBPATH:w:\external_libs\SDL2_ttf-2.0.12\lib\x64 
set additional_depend=-incremental:no -opt:ref SDL2.lib ^
                       SDL2_image.lib SDL2_mixer.lib SDL2_ttf.lib

del *.pdb> NUL2> NUL

echo WAITING FOR PDB > lock.tmp

cl %compiler_flags% ..\code\geometry_wars.cpp %additional_include% -FmGeometryWars.map ^
-LD /link %additional_depend% %library_dirs% -incremental:no -opt:ref -PDB:geometry_wars%random%.pdb ^
-EXPORT:GameUpdateAndRender

del lock.tmp

cl %compiler_flags% ..\code\win32_main.cpp %additional_include% ^
/link %additional_depend% %library_dirs%
popd