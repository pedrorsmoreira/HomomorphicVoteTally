#!/bin/bash

CANDIDATES =
VOTERS = 
VOTES = $CANDIDATES

# Generate the Root Key - This will be the public and private key of the
# Certification Authority
# APAGAR If you want a password protected key just put the -des3 option
openssl genrsa -out rootCA.key 2048 > /dev/null 2>&1
# Create and self sign the Root CA Certificate
# APAGAR -nodes: if this option is specified then if a private key is created it will not be encrypted.
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
# Signing this file
openssl dgst -sha256 -sign rootCA.key -out publicKeyFile.sign publicKeyFile.dat
cd ..

# Installing on each voter app
for (( i = 1; i <= $VOTERS; i++ ))
do
	mkdir ../Voter$i

	# Copying the signed file with the properties of the election
	cp input.sign ../Voter$i
	
	# Installing the root CA certificate
	cp rootCA.crt ../Voter$i

	# Generating the voter key pair
	# APAGAR If you want a password protected key just put the -des3 option
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
	cp ./ElectionKey/publicKeyFile.sign ../Voter$i
done

SPLIT CENAS

# Assigning a weight to each voter and encrypts it with the election public key
cd ./Weights
cmake .
make
./weights ./publicKeyFile.dat $VOTERS



openssl enc -aes-256-cbc -salt -in file.txt -out file.txt.enc -k PASS
