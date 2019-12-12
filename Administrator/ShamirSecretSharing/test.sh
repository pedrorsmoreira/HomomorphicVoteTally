#!/usr/bin/bash

openssl rand -hex 16 > pass.txt
openssl enc -aes-256-cbc -salt -in test.dat -out test.dat.enc -pass file:pass.txt -iter 10
make clean
make
./splitKeyShares 5 4

echo ""
echo ""
echo ""
echo ""
echo ""

make clean-test
make test
./test

openssl enc -aes-256-cbc -d -in test.dat.enc -out test1.dat -pass file:recovered_pass.txt -iter 10
