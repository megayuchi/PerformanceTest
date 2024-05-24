@echo off
set CURCONFIG=debug
set CURPLATFORM=arm64
call "msbuild.exe" %1 /p:configuration=%CURCONFIG% /p:platform=%CURPLATFORM% /t:rebuild
if not "%ERRORLEVEL%" == "0" pause

set CURCONFIG=release
set CURPLATFORM=arm64
call "msbuild.exe" %1 /p:configuration=%CURCONFIG% /p:platform=%CURPLATFORM% /t:rebuild
if not "%ERRORLEVEL%" == "0" pause
