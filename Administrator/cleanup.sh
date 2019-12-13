#!/bin/bash

VOTERS=3

rm -Rf ../BallotBox

for (( counter=1; counter<=$VOTERS; counter++ ))
do
	rm -Rf ../Voter$counter
done

rm input.sign input.txt
rm rootCA.key rootCA.crt

cd ElectionKey
rm -Rf CMakeFiles
rm CMakeCache.txt cmake_install.cmake Makefile
rm electionKey electionPublicKeyFile.dat electionPublicKeyFile.sign
cd ..

cd ShamirSecretSharing
rm splitKeyShares splitKeyShares.o 
cd ..

cd Weights
rm -Rf CMakeFiles
rm CMakeCache.txt cmake_install.cmake Makefile
rm weights
cd ..

cd ../TallyOfficial
rm -Rf CMakeFiles
rm CMakeCache.txt cmake_install.cmake Makefile
rm rootCA.crt
rm input.sign input.txt
rm electionPublicKeyFile.dat electionPublicKeyFile.sign
rm tally
rm -Rf WeightsEncrypted
cd ..

cd VoterApp
rm vote
cd seal
rm -Rf CMakeFiles
rm CMakeCache.txt cmake_install.cmake Makefile
rm seal_encrypt
cd ../..
