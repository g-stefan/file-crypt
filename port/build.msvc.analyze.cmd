@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> analyze file-crypt

set CXX=cl /analyze /analyze:stacksize 65536 /analyze:max_paths 1024

call port\build.msvc.make.cmd
