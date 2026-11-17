@echo off
cd /d "%~dp0"
rem Rebuilds the server from source, then packages the single-file launcher into dist\LostWorldsMQ.exe.
rem To skip the (slow) rebuild and just repackage what is in bin\, run:  package.ps1  without -Rebuild
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0package.ps1" -Rebuild
pause
