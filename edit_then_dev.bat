@echo off
REM Quietly delete the 'bin' directory and its contents if it exists
if exist bin (
    echo Deleting existing bin directory...
    rmdir /s /q bin
    if exist bin (
        echo Failed to delete bin directory.
        exit /b 1
    )
)

REM Create the 'bin' directory
echo Creating bin directory...
mkdir bin
if not exist bin (
    echo Failed to create bin directory.
    exit /b 1
)

REM Run scons
echo Running scons for initial build...
scons p=windows vsproj=no dev_build=no
if errorlevel 1 (
    echo Scons initial build failed.
    exit /b 1
)

if exist bin_stable (
	del /s /f /q bin_stable
	rd bin_stable
	echo Deleting old stable build
)

REM Rename the 'bin' directory to 'bin_stable'
echo Renaming bin directory to bin_stable...
rename bin bin_stable
if exist bin_stable (
    echo Successfully renamed bin to bin_stable.
) else (
    echo Failed to rename bin directory.
    exit /b 1
)

REM Create the 'bin' directory for development build
echo Creating bin directory for development build...
mkdir bin
if not exist bin (
    echo Failed to create bin directory for development build.
    exit /b 1
)

REM Run scons for development build
echo Running scons for development build...
scons p=windows vsproj=yes dev_build=yes
if errorlevel 1 (
    echo Scons development build failed.
    exit /b 1
)
echo Development build completed successfully.
