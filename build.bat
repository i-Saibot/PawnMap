@echo off
set /p VERSION="Enter PawnMap version: "
set IMAGE_NAME=pawnmap-builder

echo [1/3] Building Docker image...
docker build -t %IMAGE_NAME% .

echo [2/3] Running build process...
if exist build_linux rd /s /q build_linux
docker run --rm -v "%cd%:/app" %IMAGE_NAME%

echo [3/3] Creating archives for PawnMap v%VERSION%...

set WIN_DLL=Release\PawnMap.dll
set INCLUDE_FILE=pawn\PawnMap.inc

if exist temp_win rd /s /q temp_win
mkdir temp_win
if exist "%WIN_DLL%" copy "%WIN_DLL%" "temp_win\"
if exist "%INCLUDE_FILE%" copy "%INCLUDE_FILE%" "temp_win\"

powershell Compress-Archive -Path "temp_win\*" -DestinationPath "PawnMap-%VERSION%-win32.zip" -Force

docker run --rm -v "%cd%:/app" %IMAGE_NAME% sh -c "mkdir -p temp_linux && cp /app/Release/PawnMap.so temp_linux/ && cp /app/pawn/PawnMap.inc temp_linux/ && tar -czf PawnMap-%VERSION%-linux.tar.gz -C temp_linux PawnMap.so PawnMap.inc"

rd /s /q temp_win
docker run --rm -v "%cd%:/app" %IMAGE_NAME% rm -rf temp_linux

echo.
echo === Done! ===
echo Created: PawnMap-%VERSION%-win32.zip (contains: .dll, .inc)
echo Created: PawnMap-%VERSION%-linux.tar.gz (contains: .so, .inc)
pause