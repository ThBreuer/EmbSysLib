@echo off
echo build (%cd%)
echo -----

set pfad="%cd%"

mkdir "%EMBSYSLIB%\Doc\_Log" 2>>nul
mkdir "%EMBSYSLIB%\Doc\HTML" 2>>nul

cd "%EMBSYSLIB%\Doc\_Doxygen"

for %%f in (*.doxyfile) do @doxygen %%f

doxygen main.doxyfile

cd %pfad%

echo.