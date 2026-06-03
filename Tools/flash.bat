::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:: Select and call a flash-tool 
:: STM32_programmer is used if the enviroment variable STM_PATH is set to the
:: STM32-Cube-IDE directory
:: Otherwise EBlink is used, if EB_SCRIPT_PATH is defined
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@echo off

:: Suche nach Flash-Programmer

if defined xEB_SCRIPT_PATH (
  goto :EBLINK  
)

if defined STM_PATH (
  for /r "%STM_PATH%" %%F in (STM32_Programmer_CLI.exe) do (
    if exist "%%F" (
        set "CLI_PATH=%%~dpF"
        goto :STM32_PROGRAMMER
    )
  )
)

goto :ERROR

:STM32_PROGRAMMER
echo ==========================
echo Using STM32_programmer_CLI
echo ==========================
set "PATH=%PATH%;%CLI_PATH%"
STM32_Programmer_CLI.exe -c port=SWD -w %1 -v -run
goto :END

:EBLINK
echo ============
echo Using EBlink
echo ============
eblink -F verify,run,file=%1
goto :END

:ERROR
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
echo Error: No flash tool found!
echo. 
echo Install STM32-Cube-IDE and set enviroment variable STM_PATH to STM32-Cube-IDE directory
echo. 
echo or
echo. 
echo Install EBlink / EmBitz
echo +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

:END
