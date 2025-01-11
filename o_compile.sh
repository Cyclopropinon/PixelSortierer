#!/bin/bash

g++ main_opencl.cpp -o Oixelsortierer -lOpenCL -O3 -march=native

cp -v Oixelsortierer testumgebung/

cp -v Oixelsortierer testumgebung/b/
cp -v Oixelsortierer testumgebung/gif/
