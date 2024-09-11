@echo off
setlocal enabledelayedexpansion

set N1=1000
set N2=32200
set delta=3120

set N=%N1%

:loop
if %N% GTR %N2% goto end

lab1-seq.exe %N% >> out\lab1-seq.txt
lab1-par-1.exe %N% >> out\lab1-1.txt
lab1-par-10.exe %N% >> out\lab1-10.txt
lab1-par-12.exe %N% >> out\lab1-12.txt
lab1-par-20.exe %N% >> out\lab1-20.txt

set /a N=%N%+%delta%
goto loop

:end