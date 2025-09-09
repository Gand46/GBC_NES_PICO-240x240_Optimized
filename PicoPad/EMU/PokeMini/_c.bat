@echo off
rem Compilation...

set TARGET=PokeMini
set GRPDIR=EMU
set MEMMAP=

if "%1"=="" goto default
if "%1"=="picopadvga" goto stop
if "%1"=="picopad08" goto stop
..\..\..\_c1.bat %1

goto stop

:default
..\..\..\_c1.bat picopad10

:stop
