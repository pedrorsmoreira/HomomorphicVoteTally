#!/bin/bash

CANDIDATES =
VOTERS = 
VOTES = $CANDIDATES
TRUSTEES =
THRESHOLD_TRUSTEES =

# Generate the Root Key - This will be the public and private key of the
# Certification Authority
openssl genrsa -out rootCA.key 2048 > /dev/null 2>&1
# Create and self sign the Root CA Certificate
openssl req -new -x509 -days 3650 -key rootCA.key -out rootCA.crt -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-10/OU=CA-10/CN=CA10/emailAddress=example@tecnico.ulisboa.pt"

# Creating a file with the properties of the election 
# that will be the input in the voter app
echo $CANDIDATES > input.txt
echo $VOTES >> input.txt
# Signing this file
openssl dgst -sha256 -sign rootCA.key -out input.sign input.txt

# Creating directorie of tally official app
mkdir ../TallyOfficial

#Installing the root certificate in the tally official app
cp rootCA.crt ../TallyOfficial

# Generating the election key
cd ElectionKey
# Compiling and running file to generate the election key 
cmake .
make
./electionKey
# Signing the file
openssl dgst -sha256 -sign rootCA.key -out electionPublicKeyFile.sign electionPublicKeyFile.dat
cp {electionPublicKeyFile.dat, electionPublicKeyFile.sign} ../../TallyOfficial
cd ..

# Installing on each voter app
for (( i = 1; i <= $VOTERS; i++ ))
do
	mkdir ../Voter$i

	# Copying the signed file with the properties of the election
	cp {input.txt, input.sign} ../Voter$i
	
	# Installing the root CA certificate
	cp rootCA.crt ../Voter$i

	# Generating the voter key pair
	openssl genrsa -out voter$i.key 1024
	# Generating the certificate request
	openssl req -new -key voter$i.key -out voter$i.csr #-subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-10/OU=Voter$i/CN=Voter$i/emailAddress=example@tecnico.ulisboa.pt"
	# Using the certificate and the private key from our CA to sign the Voter certificate
	openssl x509 -req -in voter$i.csr -out voter$i.crt -sha1 -CA rootCA.crt -CAkey rootCA.key -CAcreateserial -days 3650
	# Defining an archive file format to bundle the private key with the voter certificate
	openssl pkcs12 -export -in voter$i.crt -inkey voter$i.key -name "Voter $i Cert" -out voter$i.p12
	openssl pkcs12 -in voter$i.p12 -clcerts -nokeys -info
	# Making the bundle accessible to other users
	chmod 444 voter$i.p12
	# Installing the voter private key and certificate
	cp voter$i.p12 ../Voter$i

	# Installing the eletion public key
	cp {./ElectionKey/electionPublicKeyFile.dat, ./ElectionKey/electionPublicKeyFile.sign} ../Voter$i
done

# Encrypts electionSecretKeyFile and deletes unencripted file
openssl rand -hex 16 -out pass.txt
openssl enc -aes-256-cbc -salt -in ./ElectionKey/electionSecretKeyFile.dat -out ./ElectionKey/electionSecretKeyFile.dat.enc -pass file:pass.txt
rm ./ElectionKey/electionSecretKeyFile.dat

# Spliting the password of the encrypted election private key using Shamir’s
# secret sharing and distributing each of the shares by the trustees
cd ShamirSecretSharing
make #> /dev/null 2>&1
./splitKeyShares $TRUSTEES $THRESHOLD_TRUSTEES
rm ./pass.txt # Deleting the original password
#make clean > /dev/null
cd ..

# Creating directorie of the counter
mkdir ../Counter

for (( i = 1; i <= $TRUSTEES; i++ ))
do
	# Signing each share
	openssl dgst -sha256 -sign rootCA.key -out share$i.sign share$i.txt
	# Moving each share to the counter
	mv ./ShamirSecretSharing/{share$i.txt, share$i.sign} ../Counter
done

# Assigning a weight to each voter and encrypts it with the election public key
cd ./Weights
cmake .
make
./weights ./electionPublicKeyFile.dat $VOTERS
# Signing the file
openssl dgst -sha256 -sign rootCA.key -out encryptedWeightsFile.sign encryptedWeightsFile.dat
mv {encryptedWeightsFile.dat, encryptedWeightsFile.sign} ../TallyOfficial

# COMPILES TALLY AND VOTER
