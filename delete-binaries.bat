@echo off
setlocal

:: This script is designed to avoid the need to manually delete bineries folders and regenate project files. 

set PROJECT_ROOT=%~dp0

echo Deleting Binaries and other stuff in %PROJECT_ROOT%...

rmdir /S /Q "%PROJECT_ROOT%\Binaries" 2>nul

rmdir /S /Q "%PROJECT_ROOT%\Saved\Cooked" 2>nul
rmdir /S /Q "%PROJECT_ROOT%\Saved\StagedBuilds" 2>nul
rmdir /S /Q "%PROJECT_ROOT%\Saved\Temp" 2>nul

rmdir /S /Q "%PROJECT_ROOT%\Intermediate\AssetRegistryCache" 2>nul
rmdir /S /Q "%PROJECT_ROOT%\Intermediate\ReimportCache" 2>nul
::rmdir /S /Q "%PROJECT_ROOT%\Intermediate\ProjectFiles" 2>nul
rmdir /S /Q "%PROJECT_ROOT%\Intermediate\Build" 2>nul
rmdir /S /Q "%PROJECT_ROOT%\Intermediate\Plugins" 2>nul
rmdir /S /Q "%PROJECT_ROOT%\Intermediate\DatasmithContentTemp" 2>nul

set PLUGINS_ROOT=%PROJECT_ROOT%\Plugins
for /f "delims=" %%D in ('dir "%PLUGINS_ROOT%" /A:D /B') do (
	if NOT %%D == FMODStudio call :plugin-cleaning %%D
)

:exit
exit /B 0

:plugin-cleaning
setlocal

set PLUGIN_NAME=%~1
set PLUGIN_PATH=%PLUGINS_ROOT%\%PLUGIN_NAME%
echo Deleting Binaries and Intermediate folder in %PLUGIN_PATH%...

rmdir /S /Q "%PLUGIN_PATH%\Binaries" 2>nul
rmdir /S /Q "%PLUGIN_PATH%\Intermediate" 2>nul

exit /B 0
endlocal

endlocal