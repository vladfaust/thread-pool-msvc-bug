# ThreadPool bug on CLang MSVC

It is not really a ThreadPool bug, but rather the `std::function` move constructor bug on MSCV.
See https://stackoverflow.com/a/16152351/3645337, https://stackoverflow.com/a/48999075/3645337, and finally https://developercommunity.visualstudio.com/content/problem/108672/unable-to-move-stdpackaged-task-into-any-stl-conta.html.

What I want is to compile the code using **both** MSVC headers and CLang, with C++20.
I need futures to be able to convey the information!

Pull requests are highly welcome.

## Progress

Results so far on my machine:

With MinGW64 GNU CC (using MinGW std implementation):

```sh
C:/Dev/msys64/mingw64/bin/g++ -std=c++2a -o test-gcc.exe test.cpp
./test-gcc.exe
# OK
```

With MinGW64 Clang (using MinGW std implementation):

```sh
C:/Dev/msys64/mingw64/bin/clang++ -std=c++2a -o test-clang-mingw.exe test.cpp
./test-clang-mingw.exe
# OK
```

With CLang + MSVC build tools (using MSVC std implementation):

```sh
C:/Dev/LLVM/10/Prebuilt/bin/clang++.exe -std=c++2a -o test-clang-msvc.exe -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -loldnames test.cpp
# Compiles
./test-clang-msvc.exe
# Test fails! Future value is 1 instead of 1024
```

## License

Copyright (c) 2016-2019 Viktor Kirilov (Doctest)
Copyright (c) 2020 Vladislav Faust (This code)

The code is licensed under MIT.
