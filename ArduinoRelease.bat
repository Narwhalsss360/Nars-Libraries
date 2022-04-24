@echo off

Set "SolutionDir=%~dp0"
Set "ArduinoLibDir=E:\Users\sandr\Documents\Arduino\libraries"

if exist "%SolutionDir%Library Releases\NarsLibraries\" (
	powershell -Command "& {rd -r '%SolutionDir%Library Releases\NarsLibraries'}"
	echo Deleted Release folder Directory.
) else (
	echo No Release folder to delete.
)

if exist "%SolutionDir%Library Releases\NarsLibraries.zip" (
	powershell -Command "& {rd -r '%SolutionDir%Library Releases\NarsLibraries.zip'}"
	echo Deleted Zip Directory.
) else (
	echo No Zip to delete.
)

:ask
set /P a=Include Arduino?[Y/N]?
if /I "%a%" EQU "Y" goto :includeArduino
if /I "%a%" EQU "N" goto :noArduino
goto :ask

:includeArduino
if exist "%ArduinoLibDir%\NarsLibraries" (
	powershell -Command "& { rd -r '%ArduinoLibDir%\NarsLibraries'}"
	echo Deleted Arduino library folder.
) else (
	echo No Arduino library folder to delete.
)

echo d | xcopy /d /y "%SolutionDir%Libraries\Nars-Arduino-Libraries\src\keywords.txt" "%ArduinoLibDir%\NarsLibraries"
echo d | xcopy /d /y "%SolutionDir%Libraries\Nars-Arduino-Libraries\src\NarsLibraries.cpp" "%ArduinoLibDir%\NarsLibraries"
echo d | xcopy /d /y "%SolutionDir%Libraries\Nars-Arduino-Libraries\src\NarsLibraries.h" "%ArduinoLibDir%\NarsLibraries"
echo d | xcopy /d /y "%SolutionDir%Libraries\Nars-Arduino-Libraries\library.properties" "%ArduinoLibDir%\NarsLibraries"
echo d | xcopy /d /y /s "%SolutionDir%Examples\Arduino Examples\" "%ArduinoLibDir%\NarsLibraries\examples"
goto :noArduino
exit

:noArduino
echo d | xcopy /d /y "%SolutionDir%Libraries\Nars-Arduino-Libraries\src\keywords.txt" "%SolutionDir%Library Releases\NarsLibraries"
echo d | xcopy /d /y "%SolutionDir%Libraries\Nars-Arduino-Libraries\src\NarsLibraries.cpp" "%SolutionDir%Library Releases\NarsLibraries"
echo d | xcopy /d /y "%SolutionDir%Libraries\Nars-Arduino-Libraries\src\NarsLibraries.h" "%SolutionDir%Library Releases\NarsLibraries"
echo d | xcopy /d /y "%SolutionDir%Libraries\Nars-Arduino-Libraries\library.properties" "%SolutionDir%Library Releases\NarsLibraries"
echo d | xcopy /d /y /s "%SolutionDir%Examples\Arduino Examples\" "%SolutionDir%Library Releases\NarsLibraries\examples"
exit