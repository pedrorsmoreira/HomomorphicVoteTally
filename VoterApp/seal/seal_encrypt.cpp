#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include "seal/seal.h"

using namespace std;
using namespace seal;

//encrypts the input file with the publi_key file and writes the encrypted message in the output file
void seal_encrypt_with_public_key(char* input, char* output, char* public_Key_File)
{
	// BFV encryption scheme
	EncryptionParameters parms(scheme_type::BFV);

	// Defining encryption parameters

	// degree of the `polynomial modulus'
	size_t poly_modulus_degree = 4096;
	parms.set_poly_modulus_degree(poly_modulus_degree);

	// [ciphertext] `coefficient modulus'
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

	// plaintext modulus
	parms.set_plain_modulus(1024);

	// Constructing a SEALContext object
	auto context = SEALContext::Create(parms);

	PublicKey public_key;
	ifstream publicKeyFile;
	publicKeyFile.open(public_Key_File, ios::binary);
	if (publicKeyFile.is_open())
		public_key.unsafe_load(context, publicKeyFile);
	else {
		cout << "Unable to open Public Key File" << endl;
		return;
	}
	publicKeyFile.close();

	// Constructing an instance of Encryptor - to be able to encrypt
	Encryptor encryptor(context, public_key);

	// Output File
	ofstream outputFile;
	outputFile.open(output, ios::app);
	if (!outputFile.is_open()) {
		cout << "Unable to open Output File" << endl;
		return;
	}

	// Input File
	ifstream inputFile;
	inputFile.open(input);
	if (!inputFile.is_open()) {
		cout << "Unable to open Input File" << endl;
		return;
	}

	std::string line;
	while ( getline (inputFile, line) ) {
		//cout << line << '\n';

		//Plaintext vote_plain(to_string((uint8_t) line.c_str()));
		Plaintext vote_plain(line);
		Ciphertext vote_encrypted;
		encryptor.encrypt(vote_plain, vote_encrypted);
		vote_encrypted.save(outputFile);
    }

    inputFile.close();
    outputFile.close();

    return;
}


int main(int argc, char* argv[]) {
	if (argc != 4){
		printf("Wrong usage of seal_encrypt!\n Usage: seal_encrypt <in_file> <out_file> <elecion_key>\n");
		exit(-1);
	}

	seal_encrypt_with_public_key(argv[1], argv[2], argv[3]);

	return 5;
}