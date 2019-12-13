#!/bin/bash

CANDIDATES=2
VOTES=$CANDIDATES
VOTERS=3
TRUSTEES=5
THRESHOLD_TRUSTEES=4

# Deleting all the files from previous excutions
bash cleanup.sh > /dev/null 2>&1

# Generate the Root Key - This will be the public and private key of the
# Certification Authority
openssl genrsa -out rootCA.key 2048 > /dev/null 2>&1
# Create and self sign the Root CA Certificate
openssl req -new -x509 -days 3650 -key rootCA.key -out rootCA.crt -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-10/OU=CA-10/CN=CA10/emailAddress=example@tecnico.ulisboa.pt"

# Creating a file with the properties of the election 
# that will be the input in the voter app and in the tally official app
echo $CANDIDATES > input.txt
echo $VOTES >> input.txt
echo $VOTERS >> input.txt
# Signing this file
openssl dgst -sha256 -sign rootCA.key -out input.sign input.txt
# Copying the signed file with the properties of the election
cp {input.txt,input.sign} ../TallyOfficial

# Preparing a file to send to the counter
echo $TRUSTEES > inputCounter.txt
echo $THRESHOLD_TRUSTEES >> inputCounter.txt
echo $CANDIDATES >> inputCounter.txt
echo $VOTERS >> inputCounter.txt
# Signing this file
openssl dgst -sha256 -sign rootCA.key -out inputCounter.sign inputCounter.txt
# Copying the signed file with the properties of the election
mv {inputCounter.txt,inputCounter.sign} ../Counter
cp rootCA.crt ../Counter

# Installing the root certificate in the tally official app
cp rootCA.crt ../TallyOfficial

# Generating the election key
cd ElectionKey
# Compiling and running file to generate the election key 
cmake . > /dev/null
make > /dev/null
./electionKey
# Signing the file
openssl dgst -sha256 -sign ../rootCA.key -out electionPublicKeyFile.sign electionPublicKeyFile.dat 
cp {electionPublicKeyFile.dat,electionPublicKeyFile.sign} ../../TallyOfficial
cd ..

# Installing on each voter app
for (( i=1; i<=$VOTERS; i++ ))
do
	# Creating directory
	mkdir ../Voter$i
	cd ../Voter$i

	# Copying the signed file with the properties of the election
	cp ../Administrator/{input.txt,input.sign} ./
	
	# Installing the root CA certificate
	cp ../Administrator/rootCA.crt ./

	# Generating the voter key pair
	openssl genrsa -out voter$i.key 1024 > /dev/null 2>&1
	# Generating the certificate request
	openssl req -new -key voter$i.key -out voter$i.csr -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-10/OU=Voter$i/CN=Voter$i/emailAddress=example@tecnico.ulisboa.pt"
	# Using the certificate and the private key from our CA to generate the Voter certificate
	openssl x509 -req -in voter$i.csr -out voter$i.crt -sha1 -CA rootCA.crt -CAkey ../Administrator/rootCA.key -CAcreateserial -days 3650 > /dev/null 2>&1
	# Signing both files
	openssl dgst -sha256 -sign ../Administrator/rootCA.key -out voter$i.sign voter$i.key
	openssl dgst -sha256 -sign ../Administrator/rootCA.key -out voter$i.crt.sign voter$i.crt

	# Installing the eletion public key
	cp ../Administrator/ElectionKey/{electionPublicKeyFile.dat,electionPublicKeyFile.sign} ../Voter$i
done

cd ../Administrator/ShamirSecretSharing

# Encrypts electionSecretKeyFile with a random generated password and deletes unencripted file
openssl rand -hex 16 > pass.txt
openssl enc -aes-256-cbc -salt -in ../ElectionKey/electionSecretKeyFile.dat -out ../ElectionKey/electionSecretKeyFile.dat.enc -pass file:pass.txt -iter 10
#rm ../ElectionKey/electionSecretKeyFile.dat
mv ../ElectionKey/electionSecretKeyFile.dat.enc ../../Counter

# Spliting the password of the encrypted election private key using Shamir’s
# secret sharing and distributing each of the shares by the trustees
make > /dev/null
./splitKeyShares $TRUSTEES $THRESHOLD_TRUSTEES
rm pass.txt # Deleting the original password
#make clean > /dev/null

for (( i=1; i<=$TRUSTEES; i++ ))
do
	# Signing each share
	openssl dgst -sha256 -sign ../rootCA.key -out share$i.sign share$i.txt
	# Moving each share to the counter
	mv {share$i.txt,share$i.sign} ../../Counter
done

# Assigning a weight to each voter and encrypts it with the election public key
cd ../Weights

rm -Rf WeightsEncrypted > /dev/null 2>&1
mkdir WeightsEncrypted
cmake . > /dev/null
make > /dev/null
rm -Rf ../../TallyOfficial/WeightsEncrypted > /dev/null 2>&1
./weights ../ElectionKey/electionPublicKeyFile.dat $VOTERS
# Signing the files
for (( i=1; i<=$VOTERS; i++ ))
do
	openssl dgst -sha256 -sign ../rootCA.key -out ./WeightsEncrypted/encryptedWeightsFile$i.sign ./WeightsEncrypted/encryptedWeightsFile$i.dat
done

mv WeightsEncrypted/ ../../TallyOfficial/

cd ../..

# Creating directorie of the Ballot Box
mkdir ./BallotBox

# COMPILES TALLY AND VOTER
cd VoterApp
cd seal
cmake . > /dev/null
make > /dev/null
cd ../..