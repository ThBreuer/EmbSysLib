@echo off
echo cleaning Example (%cd%)
echo ----------------

rmdir /S /Q .\Bin         2>> nul
rmdir /S /Q .\default     2>> nul
rmdir /S /Q .\release     2>> nul

del .\*.exe               2>> nul
del .\*.bak               2>> nul
del .\*.dep*              2>> nul
del .\*.layout            2>> nul
del .\*.cscope_file_list  2>> nul
del ".\mem.bin"           2>> nul
del ".\flash.bin"         2>> nul
del ".\TxD.txt"           2>> nul
del ".\RxD.txt"           2>> nul

echo.