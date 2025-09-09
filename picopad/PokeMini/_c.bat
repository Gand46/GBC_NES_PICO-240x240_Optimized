@echo off
rem Compilation script for the PokeMini emulator port

set TARGET=PokeMini
set GRPDIR=picopad
set MEMMAP=

if "%1"=="" goto default
..\\..\\_c1.bat %1
exit /b

:default
rem Default build for PicoPad10
..\\..\\_c1.bat picopad10
