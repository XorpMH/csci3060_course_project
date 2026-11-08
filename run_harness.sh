#!/bin/bash

set -e

echo "Building harness..."

g++ -std=c++17 \
Prototype/harness.cpp \
Prototype/functions.cpp \
Prototype/user.cpp \
Prototype/game.cpp \
-o harness.exe

echo "Build successful."
echo "Running harness..."
echo

./harness