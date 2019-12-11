#include <iostream>
#include <fstream>
#include "seal/seal.h"
using namespace std;
using namespace seal;

int main () {
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

    // APAGAR o que está aqui em baixo nao e pa ser feito aqui
    /*
    // Constructing an instance of Encryptor - to be able to encrypt
    Encryptor encryptor(context, public_key);

    // Constructing an instance of Evaluator - Computations on the ciphertexts
    // are performed with the Evaluator class
    Evaluator evaluator(context);

    // Constructing an instance of Decryptor - to decrypt our results
    Decryptor decryptor(context, secret_key);
    */

    // Writing secret and public keys in a file
    ofstream publicKeyFile;
    publicKeyFile.open(publicKeyFile.dat”, ios::out | ios::app | ios::binary);
    publicKeyFile << public_key << endl;
    publicKeyFile.close();

    ofstream secretKeyFile;
    secretKeyFile.open(secretKeyFile.dat”, ios::out | ios::app | ios::binary);
    secretKeyFile << secret_key << endl;
    secretKeyFile.close();

    return 0;
}
