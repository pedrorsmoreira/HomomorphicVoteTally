#!/bin/bash

rm rootCA.key rootCA.crt

rm input.txt input.sign pass.txt

rm -Rf ../TallyOfficial ../Voter* ../Counter

cd ElectionKey
rm -Rf CMakeFiles
rm CMakeCache.txt
cd ..

cd ShamirSecretSharing
rm splitKeyShares 
cd ..

cd Weights
rm -Rf CMakeFiles
rm CMakeCache.txt
rm encryptedWeightsFile.sign