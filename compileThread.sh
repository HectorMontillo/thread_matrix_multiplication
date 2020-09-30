#!/bin/bash
echo "Bash version ${BASH_VERSION}..."
g++ -std=c++11 -pthread $1.cpp -o $1 
echo "Excuted: g++ -std=c++11 -pthread $1.cpp -o $1 "
