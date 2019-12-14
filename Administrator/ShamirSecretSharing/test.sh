#!/usr/bin/bash

openssl rand -hex 32 > password.txt
openssl enc -aes-256-cbc -salt -in original.txt -out original.txt.enc -pass file:password.txt -iter 10

make clean > /dev/null
make > /dev/null
./splitKeyShares 5 4

echo ""
echo ""
echo ""
echo ""
echo ""

make clean-test > /dev/null
make test > /dev/null
./test

openssl enc -aes-256-cbc -d -in original.txt.enc -out restored.txt -pass file:recovered_password.txt -iter 10