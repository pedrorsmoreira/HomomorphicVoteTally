#include <iostream>
#include <fstream>

#include <string>
#include <ctime>

#include "seal/seal.h"
using namespace std;
using namespace seal;

#define FILE 0
#define RANDOM 1

bool is_file_exist(const char *fileName)
{
	ifstream infile(fileName);
	return infile.good();
}

int main(int argc, char* argv[])
{
	PublicKey public_key;
	ifstream weightsFile;

	// Check the number of parameters - the Weights File is not mandatory
    if (argc < 3) {
        // Tell the user how to run the program
        cerr << "Usage: " << argv[0] << " <Election Public Key File> <Number of Voters> <Weights File>" << endl;
        return 1;
    }

    int numberOfVoters = atoi(argv[2]);
    int howToGenerateWeights = RANDOM;

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

	// Loading the election public key from the file
	ifstream publicKeyFile;
	publicKeyFile.open(argv[1], ios::binary);
	if (publicKeyFile.is_open())
		public_key.unsafe_load(context, publicKeyFile);
	else {
		cout << "Unable to open Public Key File" << endl;
		return 1;
	}
	publicKeyFile.close();

	// Constructing an instance of Encryptor - to be able to encrypt
	Encryptor encryptor(context, public_key);

    // Checking if weights are to be loaded from file or randomly generated
	if (argc == 4 && is_file_exist(argv[3])) {
		howToGenerateWeights = FILE;
		weightsFile.open(argv[3]);
		if (!weightsFile.is_open()) {
			cout << "Unable to open Weights File" << endl;
			return 1;
		}
	} else
		howToGenerateWeights = RANDOM;

	int weight = 0;
	ofstream encryptedWeightsFile;
	encryptedWeightsFile.open("encryptedWeightsFile.dat", ios::app);
	for (int i = 0; i < numberOfVoters; ++i)
	{
		if (howToGenerateWeights == FILE)
			weightsFile >> weight;
		else
			weight = rand() % 100 + 1;

		Plaintext weight_plain(to_string(weight));
		Ciphertext weight_encrypted;
		encryptor.encrypt(weight_plain, weight_encrypted);
		weight_encrypted.save(encryptedWeightsFile);		
	}

	weightsFile.close();
	encryptedWeightsFile.close();

	return 0;
}