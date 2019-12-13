#!/bin/bash

VOTERS=3

rm -Rf ../Counter ../BallotBox 

for (( counter=1; counter<=$VOTERS; counter++ ))
do
	rm -Rf ../Voter$counter
done

rm input.sign input.txt

rm rootCA.key rootCA.crt

cd ElectionKey
rm -Rf CMakeFiles
rm CMakeCache.txt cmake_install.cmake Makefile
rm electionKey electionPublicKeyFile.dat electionPublicKeyFile.sign electionSecretKeyFile.dat.enc
cd ..

cd ShamirSecretSharing
rm splitKeyShares splitKeyShares.o 
cd ..

cd Weights
rm -Rf CMakeFiles
rm CMakeCache.txt cmake_install.cmake Makefile
rm weights
cd ..