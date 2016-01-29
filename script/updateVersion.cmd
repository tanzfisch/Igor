@echo off

set FILE_PATH=%1
set FILE_NAME=%2

SubWCRev.exe .. %FILE_PATH%\%FILE_NAME%.template %FILE_PATH%\%FILE_NAME%.h

