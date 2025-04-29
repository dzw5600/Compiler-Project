# Compiler-Project

C* is a statically typed language that enforces all type checks at
compile time.​

Uses familiar C‑style syntax (braces, semicolons) and standard
control structures (if, for, while).​

Supports primitive types: int, float, bool, char, and string.​

Omits pointers and manual memory management for a simpler,
safer model.​

Designed as a minimal C subset to illustrate and teach each
compiler phase.​

​
### How to Run

1. Clone the repo to your local Windows system.
2. **Important!**: To be able to compile to C++, ensure that you have g++ installed **and specified in your environment variables PATH**.
    - See this Microsoft tutorial on one way to install g++: https://code.visualstudio.com/docs/cpp/config-mingw
4. To compile and run the included example scripts, simply launch the `run_cstar.bat` file. This will open an interactive console window which will iteratively ask you if you would like to compile and run each `.cstar` file in the current directory.
5. You may also compile and run scripts individually, including ones you have written yourself. Simply invoke `compiler.exe` on the desired `.cstar` file to compile it to C++ immediately, like this: `compiler.exe myscript.cstar`
    - (If you are using PowerShell and encountering errors when trying to compile, try using the regular command prompt (`cmd.exe`) instead.)
7. To try out the automated test suite functionality, launch the `test_all.bat` file.
    - This will step through all the `.cstar` scripts in the `\tests` folder, compile them to C++, and compare the std outputs to expected outputs, predefined in `\tests\expected` and using the `.expected` file type.
