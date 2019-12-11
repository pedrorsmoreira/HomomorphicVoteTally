#include <iostream>
#include <fstream>
#include "seal/seal.h"
using namespace std;
using namespace seal;

int main()
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

	// Generating the secret and public keys
	KeyGenerator keygen(context);
	PublicKey public_key = keygen.public_key();
	SecretKey secret_key = keygen.secret_key();

	// Writing secret and public keys in a file
	ofstream electionPublicKeyFile;
	electionPublicKeyFile.open("electionPublicKeyFile.dat", ios::binary | ios::trunc);
	public_key.save(electionPublicKeyFile);
	electionPublicKeyFile.close();

	ofstream electionSecretKeyFile;
	electionSecretKeyFile.open("electionSecretKeyFile.dat", ios::binary | ios::trunc);
	secret_key.save(electionSecretKeyFile);
	electionSecretKeyFile.close();

	return 0;
}
