@echo off
make
echo completado
echo %cd%
copy payload.bin "C:\Users\Administrador\Documents\GitHub\StarDustCFWPack\SD_card_root\payload.bin"
%systemroot%\system32\timeout.exe 10

