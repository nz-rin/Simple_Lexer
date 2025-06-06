#!/usr/bin/env bash

echo "Building Main"
g++ -DLEX_IMPL main.cxx -o main  -Wall -Wextra

if [[ $? != 0 ]]; then
	echo "FAILED"
else
	echo "COMPILED SUCCESSFULLY"
fi
