@echo off
echo cleaning Example (%cd%)
echo ----------------

rmdir /S /Q .\Bin     2>>nul
rmdir /S /Q .\.settings    2>>nul
rmdir /S /Q .\Debug     2>>nul

del .\*.bak           2>>nul
del .\*.dep*          2>>nul
del .\*.axf           2>>nul
del .\*.layout        2>>nul
del .\*.uvguix.*      2>>nul
del .\*.scvd          2>>nul
del .\main.exe        2>>nul
del .\JLinkLog.txt    2>>nul
del ".\Target 1.bat"  2>>nul

echo.