# RGL
 Redifexx's (My) C++ OpenGL Engine
 Currently in Early Development :)
 Fork as you please.
 ![image](https://github.com/Redifexx/RGL/assets/44237862/fd36eb12-0107-4095-9aad-c894e834404d)

## How to Use
 To run in Visual Studio, make sure it's set to Release x64. 
 If you get a linker error or missing dependency error do the following:
 1) Select RGL in the Solution Explorer
 2) Go to Project > Properties > C/C++ > General
 3) Add ./RGL/Libraries/include to Additional Include Directories
 4) Go to VC++ Directories
 5) Add ./RGL/Libraries/include to Include Directories (redundant but safe)
 6) Add ./RGL/Libraries/lib to Library Directories
 7) Ensure that Linker > Input > Additional Directories includes the following:
    - kernel32.lib
    - ser32.lib
    - gdi32.lib
    - winspool.lib
    - comdlg32.lib
    - advapi32.lib
    - shell32.lib
    - ole32.lib
    - oleaut32.lib
    - uuid.lib
    - odbc32.lib
    - odbccp32.lib
8) If not, add them from ./RGL/Libraries

You can run from Visual Studio or you can navigate to ./RGL/x64/Release and run the .exe, also you can run the .exe from anywhere on your PC as long as it's moved with the Release folder.
Enjoy :)
