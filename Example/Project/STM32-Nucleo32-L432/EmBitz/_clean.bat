@echo off
echo cleaning Example (%cd%)
echo ----------------

rmdir /S /Q .\Bin     2>>nul

del .\*.depend        2>>nul
del .\*.elay          2>>nul
del .\*.eds           2>>nul

echo.