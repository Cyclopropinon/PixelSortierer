#!/bin/bash

g++ main.cpp -o Pixelsortierer -O3 -march=native

cp -v Pixelsortierer testumgebung/

cp -v Pixelsortierer testumgebung/b/
cp -v Pixelsortierer testumgebung/gif/
