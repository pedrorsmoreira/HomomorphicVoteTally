#!/usr/bin/bash

openssl rand -hex 16 -out pass.txt
openssl enc -aes-256-cbc -salt -in test.txt -out test.txt.enc -pass file:pass.txt
make clean
make
./keyshares 5 4

make clean-test
make test
./test

openssl enc -aes-256-cbc -d -in test.txt.enc -out test1.txt -pass file:recovered_pass.txt
