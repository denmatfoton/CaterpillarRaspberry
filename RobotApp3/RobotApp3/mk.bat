@echo off

set PATH=%PATH%;C:\SysGCC\raspberry\bin

make -j 8 %*
