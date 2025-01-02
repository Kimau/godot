@echo off
REM Quietly delete the 'bin' directory and its contents if it exists
if exist bin (
    echo Deleting existing bin directory...
    rmdir /s /q bin
    if exist bin (
        echo Failed to delete bin directory.
        powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm10.wav').PlaySync()
        exit /b 1
    )
)

REM Create the 'bin' directory
echo Creating bin directory...
mkdir bin
if not exist bin (
    echo Failed to create bin directory.
    powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm10.wav').PlaySync()
    exit /b 1
)

REM Run scons
echo Running scons for initial build...
scons p=windows vsproj=no dev_build=no
if errorlevel 1 (
    echo Scons initial build failed.
    powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm10.wav').PlaySync()
    exit /b 1
)

echo -------------------------------------------------------------

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
    powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm10.wav').PlaySync()
    exit /b 1
)

REM Create the 'bin' directory for development build
echo Creating bin directory for development build...
mkdir bin
if not exist bin (
    echo Failed to create bin directory for development build.
    powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm10.wav').PlaySync()
    exit /b 1
)

echo -------------------------------------------------------------

REM Run scons for development build
echo Running scons for development build...
scons p=windows vsproj=yes dev_build=yes
if errorlevel 1 (
    echo Scons development build failed.
    powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm10.wav').PlaySync()
    exit /b 1
)

REM Copy Steam API DLL to both bin directories
echo Copying Steam API DLL to bin folders...
copy /Y "modules\platform\steamworks\redistributable_bin\steam_api64.dll" "bin_stable\"
if errorlevel 1 (
    echo Failed to copy Steam API DLL to bin_stable.
    powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm10.wav').PlaySync()
    exit /b 1
)

copy /Y "modules\platform\steamworks\redistributable_bin\steam_api64.dll" "bin\"
if errorlevel 1 (
    echo Failed to copy Steam API DLL to bin.
    powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm10.wav').PlaySync()
    exit /b 1
)

echo Development build completed successfully.
powershell -c (New-Object Media.SoundPlayer 'C:\Windows\Media\alarm05.wav').PlaySync()
REM EXIT

