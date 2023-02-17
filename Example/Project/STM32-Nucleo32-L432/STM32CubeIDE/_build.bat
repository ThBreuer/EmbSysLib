@echo off
echo build all %cd%
echo =========

set pfad=%cd%

for /R /D %%f in (*.*) do (
  if exist %%f\_build.bat (
    cd %%f
    call _build.bat
    cd %pfad%
  )
)

echo.
