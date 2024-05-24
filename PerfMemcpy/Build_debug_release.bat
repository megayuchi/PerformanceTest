@echo off
set CURCONFIG=debug
set CURPLATFORM=win32
call "msbuild.exe" %1 /p:configuration=%CURCONFIG% /p:platform=%CURPLATFORM% /t:rebuild
if not "%ERRORLEVEL%" == "0" pause

set CURCONFIG=debug
set CURPLATFORM=x64
call "msbuild.exe" %1 /p:configuration=%CURCONFIG% /p:platform=%CURPLATFORM% /t:rebuild
if not "%ERRORLEVEL%" == "0" pause

set CURCONFIG=release
set CURPLATFORM=win32
call "msbuild.exe" %1 /p:configuration=%CURCONFIG% /p:platform=%CURPLATFORM% /t:rebuild
if not "%ERRORLEVEL%" == "0" pause

set CURCONFIG=release
set CURPLATFORM=x64
call "msbuild.exe" %1 /p:configuration=%CURCONFIG% /p:platform=%CURPLATFORM% /t:rebuild
if not "%ERRORLEVEL%" == "0" pause
