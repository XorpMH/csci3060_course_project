
#!/bin/bash

# Stop if compilation fails
set -e

# Compile frontend and its dependencies
g++ -std=c++17 \
    Prototype/frontend.cpp \
    Prototype/functions.cpp \
    Prototype/user.cpp \
    Prototype/game.cpp \
    -o frontend

# Run the frontend
./frontend