#!/bin/bash

g++ -std=c++17 \
Prototype/harness.cpp \
Prototype/functions.cpp \
Prototype/user.cpp \
Prototype/game.cpp \
-o harness

./harness