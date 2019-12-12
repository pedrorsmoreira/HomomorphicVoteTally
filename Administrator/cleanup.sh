#!/bin/bash

rm -Rf ../Counter ../TallyOfficial ../Voter*

rm input.sign input.txt pass.txt

rm rootCA.key rootCA.crt

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