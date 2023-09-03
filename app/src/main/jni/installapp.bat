@echo off

set CUR_DIR=%cd%

set TARGET_DIR=!CUR_DIR:\src\main\jni=\build\outputs\apk\debug!

cd %TARGET_DIR%
adb install -r app-debug.apk

endlocal
