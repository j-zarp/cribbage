#!/bin/bash

if [ ! -z "$1" ]; then
  filename=$1
  g++ -std=c++11 -O3 -o ${filename%.*} $1
else
  echo "you have to provide the file you want to compile, e.g. compile_cpp <myfile.cpp>"
fi
