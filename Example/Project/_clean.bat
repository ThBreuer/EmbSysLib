@echo off

set pfad="%cd%"

echo cleaning all %pfad%
echo.

for /R /D %%f in (*.*) do (
  if exist %%f\_clean.bat (
    echo ===============================================
    cd %%f
    call _clean.bat
    cd %pfad%
  )
)
