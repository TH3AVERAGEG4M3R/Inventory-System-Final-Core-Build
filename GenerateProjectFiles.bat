@echo off
echo =====================================
echo   SPATIAL INVENTORY SYSTEM SETUP
echo =====================================
echo.

REM Check if we're in the right directory
if not exist "InventorySystem.uproject" (
    echo ERROR: InventorySystem.uproject not found!
    echo Please run this script from the InventorySystem project folder.
    pause
    exit /b 1
)

echo Checking Unreal Engine installation...

REM Check multiple possible UE installations (prioritize 5.5.4)
set "UE_ROOT="
set "UE_VERSION="

REM Check UE 5.5.4 first (target version)
if exist "C:\Program Files\Epic Games\UE_5.54" (
    set "UE_ROOT=C:\Program Files\Epic Games\UE_5.54"
    set "UE_VERSION=5.5.4"
    goto :found_ue
)

REM Check UE 5.5
if exist "C:\Program Files\Epic Games\UE_5.5" (
    set "UE_ROOT=C:\Program Files\Epic Games\UE_5.5"
    set "UE_VERSION=5.5"
    goto :found_ue
)

REM Check UE 5.4
if exist "C:\Program Files\Epic Games\UE_5.4" (
    set "UE_ROOT=C:\Program Files\Epic Games\UE_5.4"
    set "UE_VERSION=5.4"
    goto :found_ue
)

REM Check UE 5.3
if exist "C:\Program Files\Epic Games\UE_5.3" (
    set "UE_ROOT=C:\Program Files\Epic Games\UE_5.3"
    set "UE_VERSION=5.3"
    goto :found_ue
)

REM Check UE 5.2
if exist "C:\Program Files\Epic Games\UE_5.2" (
    set "UE_ROOT=C:\Program Files\Epic Games\UE_5.2"
    set "UE_VERSION=5.2"
    goto :found_ue
)

REM Check UE 5.1
if exist "C:\Program Files\Epic Games\UE_5.1" (
    set "UE_ROOT=C:\Program Files\Epic Games\UE_5.1"
    set "UE_VERSION=5.1"
    goto :found_ue
)

REM Check launcher installed UE
for /d %%i in ("C:\Program Files\Epic Games\UE_*") do (
    if exist "%%i\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
        set "UE_ROOT=%%i"
        set "UE_VERSION=%%~ni"
        goto :found_ue
    )
)

echo ERROR: Could not find Unreal Engine installation!
echo.
echo Please ensure you have Unreal Engine 5.1 or newer installed.
echo Expected location: C:\Program Files\Epic Games\UE_X.X
echo.
echo If you have UE installed elsewhere, please modify this script 
echo or copy the UBT command manually.
pause
exit /b 1

:found_ue
echo Found Unreal Engine %UE_VERSION% at: %UE_ROOT%
echo.

REM Check if UnrealBuildTool exists
set "UBT_PATH=%UE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe"
if not exist "%UBT_PATH%" (
    echo ERROR: UnrealBuildTool not found at expected location!
    echo Path: %UBT_PATH%
    pause
    exit /b 1
)

echo Generating Visual Studio project files...
echo This may take a few moments...
echo.

REM Generate project files
"%UBT_PATH%" -projectfiles -project="%~dp0InventorySystem.uproject" -game -rocket -progress

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ====================================
    echo        SETUP SUCCESSFUL! 
    echo ====================================
    echo.
    echo Project files generated successfully!
    echo.
    echo NEXT STEPS:
    echo 1. Open "InventorySystem.sln" in Visual Studio
    echo 2. Set configuration to "Development Editor" ^(dropdown at top^)
    echo 3. Build the solution ^(Ctrl+Shift+B^)
    echo 4. Once compiled, open "InventorySystem.uproject" in Unreal Engine
    echo 5. Press Tab in Play mode to test the inventory system!
    echo.
    echo The project includes:
    echo - Complete C++ inventory system
    echo - Example character with test items
    echo - Tab key mapped to open/close inventory
    echo - 12 equipment slots with drag-and-drop
    echo - Container system ^(backpacks, vests^)
    echo.
) else (
    echo.
    echo ERROR: Failed to generate project files!
    echo Error code: %ERRORLEVEL%
    echo.
    echo Possible causes:
    echo - Visual Studio not installed with C++ tools
    echo - Unreal Engine installation corrupted
    echo - Insufficient permissions
    echo.
    echo Please check your Unreal Engine installation and try again.
)

echo.
echo Press any key to exit...
pause > nul