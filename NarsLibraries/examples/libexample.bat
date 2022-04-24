@echo off

if exist ".\libexamples\"
(
	powershell -Command "& {rd -r '.\libexamples\'}"
)