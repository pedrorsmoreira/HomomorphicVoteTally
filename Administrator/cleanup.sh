#!/bin/bash

rm -Rf ../Counter ../TallyOfficial ../Voter*

rm input.sign input.txt

rm rootCA.key rootCA.crt

cd ElectionKey
rm -Rf CMakeFiles
rm CMakeCache.txt cmake_install.cmake
rm electionKey electionPublicKey.dat electionPublicKey.sign electionSecretKey.dat.enc
cd ..

cd ShamirSecretSharing
rm splitKeyShares splitKeyShares.o 
cd ..

cd Weights
rm -Rf CMakeFiles
rm CMakeCache.txt cmake_install.cmake
rm weights
cd ..