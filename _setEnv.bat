@echo off
echo Set enviroment variable EMBSYSLIB to current path

setx EMBSYSLIB "%cd%"

if /I "%ERRORLEVEL%" EQU "0" (
	set EMBSYSLIB
	echo.
) else (
	echo Error: EMBSYSLIB not set.
)

@pause