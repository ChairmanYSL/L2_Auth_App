@echo off
setlocal enabledelayedexpansion

:: 获取当前目录
set CUR_DIR=%cd%

:: 从当前路径替换为目标路径
set TARGET_DIR=!CUR_DIR:\src\main\jni=\build\outputs\apk\debug!

cd %TARGET_DIR%
adb install -r app-debug.apk

endlocal
