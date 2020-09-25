#!/bin/bash

cp out.txt.bak out.txt
make -B
./pls.exe -e
