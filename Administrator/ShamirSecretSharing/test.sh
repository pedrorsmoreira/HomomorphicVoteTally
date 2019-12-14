#!/usr/bin/bash

openssl rand -hex 16 > password.txt
openssl enc -aes-256-cbc -salt -in original.txt -out original.txt.enc -pass file:password.txt -iter 10
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

openssl enc -aes-256-cbc -d -in original.txt.enc -out restored.txt -pass file:recovered_password.txt -iter 10