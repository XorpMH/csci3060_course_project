#!/bin/bash

g++ -std=c++17 \
Prototype/frontend.cpp \
Prototype/functions.cpp \
Prototype/user.cpp \
Prototype/game.cpp \
-o frontend

./frontend "$@"