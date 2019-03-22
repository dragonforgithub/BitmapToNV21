title Clear Build Cache

@echo off

color 0a


del /f/s/q "%~dp0\build\*.*"
RD /s/q "%~dp0\build\"

del /f/s/q "%~dp0\.idea\*.*"
RD /s/q "%~dp0\.idea\"

del /f/s/q "%~dp0\.gradle\*.*"
RD /s/q "%~dp0\.gradle\"






del /f/s/q "%~dp0\app\.externalNativeBuild\*.*"
RD /s/q "%~dp0\app\.externalNativeBuild\"

del /f/s/q "%~dp0\app\build\*.*"
RD /s/q "%~dp0\app\build\"



pause