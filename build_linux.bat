@echo off
set IMAGE_NAME=pawnmap-builder

echo [1/2] Building Docker image...
docker build -t %IMAGE_NAME% .

echo [2/2] Running build process...
docker run --rm -v "%cd%:/app" %IMAGE_NAME%

echo.
echo Operation finished!
pause