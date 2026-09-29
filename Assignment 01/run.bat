@echo off
cd /d "%~dp0"

".\Graph Representations.exe" In.txt Matrix.txt List.txt

echo.
echo Exit code: %errorlevel%
pause