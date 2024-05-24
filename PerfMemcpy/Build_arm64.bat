@echo off

set CURPATH=%cd%


rem App Client
call Build_debug_release_arm64.bat %CURPATH%\PerfMemcpy.vcxproj


