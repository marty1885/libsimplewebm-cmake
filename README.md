# libsimplewebm-cmake

Fork of the original [zaps166/libsimplewebm](https://github.com/zaps166/libsimplewebm) but upgraded to C++14 (17?), newer libwebp support and CMake.

## Build

```bash
mkdir build
cmake ..
make -j
```

## TODOs:

- [ ] Replace manual memory managment with `unique_ptr`
- [ ] Cleanup lifetime control
