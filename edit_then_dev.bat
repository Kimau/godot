@echo off
:: Initialize progress bar
powershell -command "$progressPreference='Continue'; $progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.DontShowOnTaskbar = $false }"

REM Quietly delete the 'bin' directory and its contents if it exists
echo Step 1/8: Preparing environment...
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(12, 100) }"

if exist bin (
    echo Deleting existing bin directory...
    rmdir /s /q bin
    if exist bin (
        echo Failed to delete bin directory.
        powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressState(4) }"
        powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm10.wav').PlaySync()"
        exit /b 1
    )
)

REM Create the 'bin' directory
echo Step 2/8: Creating initial bin directory...
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(25, 100) }"

echo Creating bin directory...
mkdir bin
if not exist bin (
    echo Failed to create bin directory.
    powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressState(4) }"
    powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm10.wav').PlaySync()"
    exit /b 1
)

REM Run scons
echo Step 3/8: Running initial scons build...
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(37, 100) }"

echo Running scons for initial build...
scons p=windows vsproj=no dev_build=no
if errorlevel 1 (
    echo Scons initial build failed.
    powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressState(4) }"
    powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm10.wav').PlaySync()"
    exit /b 1
)

echo -------------------------------------------------------------

echo Step 4/8: Managing stable build directory...
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(50, 100) }"

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
    powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressState(4) }"
    powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm10.wav').PlaySync()"
    exit /b 1
)

echo Step 5/8: Creating development bin directory...
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(62, 100) }"

REM Create the 'bin' directory for development build
echo Creating bin directory for development build...
mkdir bin
if not exist bin (
    echo Failed to create bin directory for development build.
    powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressState(4) }"
    powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm10.wav').PlaySync()"
    exit /b 1
)

echo -------------------------------------------------------------

echo Step 6/8: Running development scons build...
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(75, 100) }"

REM Run scons for development build
echo Running scons for development build...
scons p=windows vsproj=yes dev_build=yes tests=yes
if errorlevel 1 (
    echo Scons development build failed.
    powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressState(4) }"
    powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm10.wav').PlaySync()"
    exit /b 1
)

echo Step 7/8: Copying Steam API DLL to bin folders...
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(87, 100) }"

REM Copy Steam API DLL to both bin directories
echo Copying Steam API DLL to bin folders...
copy /Y "modules\platform\steamworks\redistributable_bin\steam_api64.dll" "bin_stable\"
if errorlevel 1 (
    echo Failed to copy Steam API DLL to bin_stable.
    powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressState(4) }"
    powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm10.wav').PlaySync()"
    exit /b 1
)

echo Step 8/8: Finalizing...
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(95, 100) }"

copy /Y "modules\platform\steamworks\redistributable_bin\steam_api64.dll" "bin\"
if errorlevel 1 (
    echo Failed to copy Steam API DLL to bin.
    powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressState(4) }"
    powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm10.wav').PlaySync()"
    exit /b 1
)

powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(100, 100) }"

echo Development build completed successfully.
powershell -c "[System.Media.SoundPlayer]::new('C:\Windows\Media\alarm05.wav').PlaySync()"

:: Clear progress bar
powershell -command "$progress = New-Object -ComObject Shell.Application; $handle = (Get-Process -PID $PID).MainWindowHandle; $progress.Windows() | Where-Object { $_.HWND -eq $handle } | ForEach-Object { $_.SetProgressValue(0, 100) }"

REM EXIT