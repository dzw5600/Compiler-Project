@echo off
setlocal enabledelayedexpansion

rem ───── Configuration ─────────────────────────────────────
set COMPILER=compiler.exe
set TESTDIR=tests
set EXPDIR=tests\expected
set TMPDIR=%TEMP%\cstar_test

if exist "%TMPDIR%" rd /s /q "%TMPDIR%"
mkdir "%TMPDIR%"

echo.
echo Running end-to-end tests...
echo.

rem ───── Loop over all .cstar and dispatch to :RunTest ─────
for %%F in ("%TESTDIR%\*.cstar") do (
  call :RunTest "%%~F"
)

echo.
echo All done.
pause
exit /b


:RunTest
rem %1 == full path to .cstar
set "FULLPATH=%~1"
set "FNAME=%~n1"

echo ==============================================
echo Testing: %FNAME%
echo ----------------------------------------------

rem 1) compile .cstar → C++
"%COMPILER%" "%FULLPATH%" >"%TMPDIR%\compile.log" 2>&1
if errorlevel 1 (
  echo   [FAIL] compiler error
  type "%TMPDIR%\compile.log"
  goto :EOF
)

rem 2) locate the generated C++ in tests\ with a leading underscore
set "GEN_CPP=%TESTDIR%\_%FNAME%_output.cpp"
if not exist "%GEN_CPP%" (
  echo   [FAIL] missing generated C++: %GEN_CPP%
  goto :EOF
)

rem 3) compile the generated C++
g++ -std=c++11 "%GEN_CPP%" -o "%TMPDIR%\%FNAME%.exe" >"%TMPDIR%\build.log" 2>&1
if errorlevel 1 (
  echo   [FAIL] generated code failed to compile
  type "%TMPDIR%\build.log"
  goto :EOF
)

rem 4) run it
"%TMPDIR%\%FNAME%.exe" >"%TMPDIR%\run.log"

rem 5) compare to expected
set "EXPECT=%EXPDIR%\%FNAME%.expected"
if not exist "%EXPECT%" (
  echo   [WARN] no expected output for %FNAME%
  goto :EOF
)

fc /N "%EXPECT%" "%TMPDIR%\run.log" >nul
if errorlevel 1 (
  echo   [FAIL] output mismatch
  fc /N "%EXPECT%" "%TMPDIR%\run.log"
) else (
  echo   [PASS]
)

goto :EOF
