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

@echo on

echo d | xcopy /d /y /s ".\Testing\Arduino Libraries Test Folder\NarsSerialCom_Test" ".\Libraries\Nars-Arduino-Libraries\examples\NarsSerialCom_Test"
echo d | xcopy /d /y /s ".\Testing\Arduino Libraries Test Folder\NarsWireCom Test Folder" ".\Libraries\Nars-Arduino-Libraries\examples"
echo d | xcopy /d /y ".\Libraries\Nars-Arduino-Libraries\src\keywords.txt" ".\Library Releases\NarsLibraries"
echo d | xcopy /d /y ".\Libraries\Nars-Arduino-Libraries\src\NarsLibraries.cpp" ".\Library Releases\NarsLibraries"
echo d | xcopy /d /y ".\Libraries\Nars-Arduino-Libraries\src\NarsLibraries.h" ".\Library Releases\NarsLibraries"
echo d | xcopy /d /y ".\Libraries\Nars-Arduino-Libraries\library.properties" ".\Library Releases\NarsLibraries"
echo d | xcopy /d /y /s ".\Libraries\Nars-Arduino-Libraries\examples" ".\Library Releases\NarsLibraries\examples"
powershell -Command "& {Compress-Archive '.\Library Releases\NarsLibraries\*' '.\Library Releases\NarsLibraries.zip'}"
powershell -Command "& {rd -r '.\Library Releases\NarsLibraries'}"