# ouroboros

A tool to solve the ouroborosrings puzzle in professor Stewart's cabinet of mathematical curiosities.

## requirements

- modern c++ compiler
- vcpkg
- ninja
- cmake

## build
```
$ cd ${PROJ}
$ cmake --preset default
$ cmake --build --preset default
```

## run

```
$ cd ${PROJ}
$ ./build/ouroboros 2
$ ./build/ouroboros 3
$ ./build/ouroboros 4
```
