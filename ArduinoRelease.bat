@echo off

if exist ".\Library Releases\NarsLibraries\" (
	powershell -Command "& {rd -r '.\Library Releases\NarsLibraries'}"
	echo Deleted Directory
) else (
	echo No directory to delete
)

if exist ".\Library Releases\NarsLibraries.zip" (
	powershell -Command "& {rd -r '.\Library Releases\NarsLibraries.zip'}"
	echo Deleted Directory
) else (
	echo No directory to delete
)

echo Copying...

echo d | xcopy /d /y ".\Libraries\Nars-Arduino-Libraries\src\keywords.txt" ".\Library Releases\NarsLibraries"
echo d | xcopy /d /y ".\Libraries\Nars-Arduino-Libraries\src\NarsLibraries.cpp" ".\Library Releases\NarsLibraries"
echo d | xcopy /d /y ".\Libraries\Nars-Arduino-Libraries\src\NarsLibraries.h" ".\Library Releases\NarsLibraries"
echo d | xcopy /d /y ".\Libraries\Nars-Arduino-Libraries\library.properties" ".\Library Releases\NarsLibraries"
echo d | xcopy /d /y /s ".\Examples\Arduino Examples\" ".\Library Releases\NarsLibraries\examples"