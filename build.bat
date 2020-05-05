mkdir build
cd build

conan install .. --build missing -s build_type=Debug
cmake .. -G "Visual Studio 15 Win64" -DCMAKE_BUILD_TYPE=Debug
cmake --build .
build\bin\tests.exe
