@echo off
setlocal enabledelayedexpansion

echo Compiling the compiler...
g++ main.cpp codegenerator.cpp parser.cpp tokenizer.cpp semanticAnalyzer.cpp -o compiler
if errorlevel 1 (
    echo Compilation failed!
    pause
    exit /b
)

:: Loop through all .cstar files in the directory
set foundAny=false
for %%f in (*.cstar) do (
    set foundAny=true
    echo.
    echo ===============================
    echo Running compiler on: %%f
    echo ===============================
    compiler.exe "%%f"

    :: Prompt user to continue
    choice /m "Continue to next file"
    if errorlevel 2 (
        echo User cancelled. Exiting.
        exit /b
    )
)

if "!foundAny!"=="false" (
    echo No .cstar files found in this directory.
)
echo No more files found.
pause