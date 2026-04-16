echo.
echo == build.bat ==
echo Working directory: %CD%
echo.
echo Using generator: %GENERATOR%
echo Build type: %BUILD_TYPE%
echo Compiler:
g++ --version
echo.
echo Configuring with CMake...
echo Building...
echo Build succeeded.
popd
@echo off
REM Script para compilar el proyecto con CMake y g++ en Windows (MSYS2/MinGW)

REM Crear el directorio de compilación
if exist build (
    rmdir /s /q build
)
mkdir build
cd build

REM Ejecutar CMake con MinGW Makefiles
echo Configurando CMake con g++ (MinGW)...
cmake .. -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc

if %ERRORLEVEL% NEQ 0 (
    echo Error en la configuracion de CMake
    exit /b 1
)

REM Compilar el proyecto
echo Compilando...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Error durante la compilacion
    exit /b 1
)

echo ========================================
echo Compilacion completada exitosamente
echo ========================================
echo.
echo Para ejecutar los tests:
echo   tests_runner.exe
echo.
cd ..
