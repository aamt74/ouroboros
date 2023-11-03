# rpf-longdiv-solve

A tool to solve the ouroborosrings puzzle in professor Stewart's cabinet of mathematical curiosities.

## requirements

- modern c++ compiler
- boost
- cmake

## build

```
$ cd ${PROJ}
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

## run

```
$ cd ${PROJ}
$ ./build/ouroboros 2
$ ./build/ouroboros 3
$ ./build/ouroboros 4
```
