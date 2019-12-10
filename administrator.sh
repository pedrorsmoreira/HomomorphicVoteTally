#!/bin/bash

echo "Hello World"

# Create the Root Key - This will be the public and private key of the
# Certification Authority
# APAGAR If you want a password protected key just put the -des3 option
openssl genrsa -out rootCA.key 2048 > /dev/null 2>&1

# Create and self sign the Root CA Certificate
# APAGAR -nodes: if this option is specified then if a private key is created it will not be encrypted.
openssl req -new -x509 -days 3650 -key rootCA.key -out rootCA.crt -subj "/C=PT/ST=Lisbon/L=Lisbon/O=CSC-10/OU=CA-10/CN=CA10/emailAddress=diogoegrodrigues@tecnico.ulisboa.pt"

# Creating Directories
mkdir ./TallyOfficialApp

#Installing the root certificate in the tally official app
cp rootCA.crt ./TallyOfficialApp