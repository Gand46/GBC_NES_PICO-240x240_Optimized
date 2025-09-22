@echo off
if "%1"=="" goto default

set TARGET=DOOM
set GRPDIR=EMU
set MEMMAP=

..\..\..\_c1.bat %1
goto stop

:default
..\..\..\_c1.bat picopad10
:stop
