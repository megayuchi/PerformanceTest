@echo off

set CURPATH=%cd%


rem App Client
call Build_debug_release.bat %CURPATH%\PerfMemcpy.vcxproj


