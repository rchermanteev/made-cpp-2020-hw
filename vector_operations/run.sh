#!/bin/bash

set -e

g++ -std=c++17 -I./ test/test.cpp src/vector_ops.cpp -o vector_ops_test
./vector_ops_test

echo All tests passed!