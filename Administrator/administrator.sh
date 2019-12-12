#!/bin/bash

CANDIDATES=2
VOTERS=3
VOTES=$CANDIDATES
TRUSTEES=5
THRESHOLD_TRUSTEES=4


#> /dev/null 2>&1

printf "\n\n--->Generating Root key and Root CA Certificate\n\n"
# Generate the Root Key - This will be the public and private key of the
# Certification Authority
openssl genrsa -out rootCA.key 2048
# Create and self sign the Root CA Certificate
openssl req -new -x509 -days 3650 -key rootCA.key -out rootCA.crt -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-10/OU=CA-10/CN=CA10/emailAddress=example@tecnico.ulisboa.pt"

printf "\n\n--->Creating a file with the properties of the election and sign it\n\n"
# Creating a file with the properties of the election 
# that will be the input in the voter app
echo $CANDIDATES > input.txt
echo $VOTES >> input.txt
# Signing this file
openssl dgst -sha256 -sign rootCA.key -out input.sign input.txt

printf "\n\n--->Installing the root certificate in the tally official app\n\n"
# Creating directorie of tally official app
mkdir ../TallyOfficial

# Installing the root certificate in the tally official app
cp rootCA.crt ../TallyOfficial

printf "\n\n--->Generating the election key - a special homomorphic key - sign it and copy it to the tally official app\n\n"
# Generating the election key
cd ElectionKey
# Compiling and running file to generate the election key 
cmake .
make
./electionKey
# Signing the file
openssl dgst -sha256 -sign ../rootCA.key -out electionPublicKeyFile.sign electionPublicKeyFile.dat 
cp {electionPublicKeyFile.dat,electionPublicKeyFile.sign} ../../TallyOfficial
cd ..

printf "\n\n--->Voter app\n\n"
# Installing on each voter app
for (( i=1; i<=$VOTERS; i++ ))
do
	printf "\n\n---------- Voter $i ----------\n\n"
	# Creating directory
	mkdir ../Voter$i
	cd ../Voter$i

	printf "\n\n--->Copying the file and signed file with the properties of the election\n\n"
	# Copying the signed file with the properties of the election
	cp ../Administrator/{input.txt,input.sign} ./
	
	printf "\n\n--->Installing the root CA certificate\n\n"
	# Installing the root CA certificate
	cp ../Administrator/rootCA.crt ./

	printf "\n\n--->Generating the voter private key and certificate - signs both documents and installs them\n\n"
	# Generating the voter key pair
	openssl genrsa -out voter$i.key 1024
	# Generating the certificate request
	openssl req -new -key voter$i.key -out voter$i.csr -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-10/OU=Voter$i/CN=Voter$i/emailAddress=example@tecnico.ulisboa.pt"
	# Using the certificate and the private key from our CA to generate the Voter certificate
	openssl x509 -req -in voter$i.csr -out voter$i.crt -sha1 -CA rootCA.crt -CAkey ../Administrator/rootCA.key -CAcreateserial -days 3650
	# Signing both files
	openssl dgst -sha256 -sign ../Administrator/rootCA.key -out voter$iKey.sign voter$i.key
	openssl dgst -sha256 -sign ../Administrator/rootCA.key -out voter$iCert.sign voter$i.csr

	printf "\n\n--->Installing the eletion public key\n\n"
	# Installing the eletion public key
	cp ../Administrator/ElectionKey/{electionPublicKeyFile.dat,electionPublicKeyFile.sign} ../Voter$i
done

cd ../Administrator/ShamirSecretSharing

printf "\n\n--->Encrypting election secret key with a random generated password and deletes unencripted file\n\n"
# Encrypts electionSecretKeyFile with a random generated password and deletes unencripted file
openssl rand -hex 16 > pass.txt
openssl enc -aes-256-cbc -salt -in ../ElectionKey/electionSecretKeyFile.dat -out ../ElectionKey/electionSecretKeyFile.dat.enc -pass file:pass.txt -iter 10
rm ../ElectionKey/electionSecretKeyFile.dat

printf "\n\n--->Spliting the password of the encrypted election private key using sss and distributing each of the shares by the trustees\n\n"
# Spliting the password of the encrypted election private key using Shamir’s
# secret sharing and distributing each of the shares by the trustees
make #> /dev/null 2>&1
./splitKeyShares $TRUSTEES $THRESHOLD_TRUSTEES
rm pass.txt # Deleting the original password
#make clean > /dev/null

printf "\n\n--->Moving all the signed shares to the counter\n\n"
# Creating directorie of the counter
mkdir ../../Counter

for (( i=1; i<=$TRUSTEES; i++ ))
do
	# Signing each share
	openssl dgst -sha256 -sign ../rootCA.key -out share$i.sign share$i.txt
	# Moving each share to the counter
	mv {share$i.txt,share$i.sign} ../../Counter
done

printf "\n\n--->Assigning a weight to each voter and encrypts it with the election public key\n\n"
# Assigning a weight to each voter and encrypts it with the election public key
cd ../Weights
cmake .
make
./weights ./electionPublicKeyFile.dat $VOTERS
# Signing the file
openssl dgst -sha256 -sign ../rootCA.key -out encryptedWeightsFile.sign encryptedWeightsFile.dat
mv {encryptedWeightsFile.dat,encryptedWeightsFile.sign} ../../TallyOfficial

# COMPILES TALLY AND VOTER
