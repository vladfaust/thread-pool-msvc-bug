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
In file included from test.cpp:7:
In file included from ././thread_pool.cpp:1:
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.27.29110\include\functional:879:51: error:
      call to deleted constructor of 'std::packaged_task<int ()>'
    explicit _Func_impl_no_alloc(_Other&& _Val) : _Callee(_STD forward<_Other>(_Val)) {}
                                                  ^       ~~~~~~~~~~~~~~~~~~~~~~~~~~
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.27.29110\include\functional:889:35: note:
      in instantiation of function template specialization 'std::_Func_impl_no_alloc<std::packaged_task<int      ()>, void>::_Func_impl_no_alloc<const std::packaged_task<int ()> &, 0>' requested here
            return ::new (_Where) _Func_impl_no_alloc(_Callee);
                                  ^
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.27.29110\include\functional:879:14: note:
      in instantiation of member function 'std::_Func_impl_no_alloc<std::packaged_task<int ()>,
      void>::_Copy' requested here
    explicit _Func_impl_no_alloc(_Other&& _Val) : _Callee(_STD forward<_Other>(_Val)) {}
             ^
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.27.29110\include\functional:998:58: note:
      in instantiation of function template specialization 'std::_Func_impl_no_alloc<std::packaged_task<int      ()>, void>::_Func_impl_no_alloc<std::packaged_task<int ()>, 0>' requested here
            _Set(::new (static_cast<void*>(&_Mystorage)) _Impl(_STD forward<_Fx>(_Val)));
                                                         ^
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.27.29110\include\functional:1126:15: note:
      in instantiation of function template specialization
      'std::_Func_class<void>::_Reset<std::packaged_task<int ()> >' requested here
        this->_Reset(_STD move(_Func));
              ^
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.27.29110\include\future:581:39: note:
      in instantiation of function template specialization 'std::function<void
      ()>::function<std::packaged_task<int ()>, 0>' requested here
    _Packaged_state(_Fty2&& _Fnarg) : _Fn(_STD forward<_Fty2>(_Fnarg)) {}
                                      ^
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.27.29110\include\future:1349:61: note:
      in instantiation of function template specialization 'std::_Packaged_state<void
      ()>::_Packaged_state<std::packaged_task<int ()> >' requested here
    explicit packaged_task(_Fty2&& _Fnarg) : _MyPromise(new _MyStateType(_STD forward<_Fty2>(_Fnarg))) {}
                                                            ^
././thread_pool.hpp:75:29: note: in instantiation of function template specialization
      'std::packaged_task<void ()>::packaged_task<std::packaged_task<int ()>, 0>' requested here
        TaskEntry(priority, std::packaged_task<void()>(std::move(task))));
                            ^
C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.27.29110\include\future:1413:5: note:
      'packaged_task' has been explicitly marked deleted here
    packaged_task(const packaged_task&) = delete;
    ^
1 error generated.
```

## License

Copyright (c) 2016-2019 Viktor Kirilov (Doctest)
Copyright (c) 2020 Vladislav Faust (This code)

The code is licensed under MIT.
