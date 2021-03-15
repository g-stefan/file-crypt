@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> sign file-crypt

pushd output
for /r %%i in (*.exe) do call grigore-stefan.sign "File Crypt" "%%i"
popd
