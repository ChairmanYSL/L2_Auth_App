@echo off
setlocal enabledelayedexpansion

set CUR_DIR=%cd%
set TARGET_DIR=%CUR_DIR:\src\main\jni=\build\outputs\apk\debug%

if not exist "%TARGET_DIR%" (
    echo Target directory does not exist: %TARGET_DIR%
    exit /b 1
)

cd /d "%TARGET_DIR%"
if errorlevel 1 (
    echo Could not change to target directory.
    exit /b 1
)

adb install -r app-debug.apk

if errorlevel 1 (
    echo adb install failed.
    exit /b 1
)

endlocal
