#include <iostream>
#include <fstream>
#include <string>

extern "C"{
	#include "sss/sss.h"
	#include "sss/randombytes.h"
	#include <assert.h>
}

#include "../utils/utils.cpp"

#include "seal/seal.h"
using namespace seal;

#define TXT_EXTENSION			".txt"
#define SIGNED_EXTENSION 		".sign"

//root CA certificate
#define ROOT_CRT_FILE 			"rootCA.crt"

#define COUNTER_INPUT 			"inputCounter.txt"
#define COUNTER_INPUT_SIGNED 	"inputCounter.sign"

#define RESULTS					"results"

#define CHECKSUM				"checksum.txt"

//names of the existing files
#define SHARE 					"share" //append ID

void get_sss_info(std::string filePATH, unsigned int& trustees, unsigned int& threshold_trustees, unsigned int& nrCandidates, unsigned int& nrVoters){
	std::ifstream input(filePATH);
	input >> trustees >> threshold_trustees >> nrCandidates >> nrVoters;
}

Ciphertext generateCiphertext(std::string filename)
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
    
    Ciphertext newCiphertext;

    std::ifstream encryptedFile;
    encryptedFile.open(filename, std::ios::binary);
    if (!encryptedFile.is_open()) {
        std::cout << "Unable to open Encrypted File" << std::endl;
        exit(-3);
    }

    newCiphertext.unsafe_load(context, encryptedFile);

    return newCiphertext;
}

Plaintext Decrypt(Ciphertext cypher)
{
	// Scheme : BFV
	EncryptionParameters parms(scheme_type::BFV);
	// 1st Parameter
	size_t poly_modulus_degree = 4096;
	parms.set_poly_modulus_degree(poly_modulus_degree);
	// 2nd Parameter
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	// 3rd Parameter
	parms.set_plain_modulus(1024);
	// Starting
	auto context = SEALContext::Create(parms);

	// Loading the election public key from the file
	ifstream secretKeyFile;
	SecretKey secret_key;
	secretKeyFile.open(PRIVATE_KEY_FILE_PATH, ios::binary);
	if (secretKeyFile.is_open())
		secret_key.unsafe_load(context, secretKeyFile);
	else {
		cout << "Unable to open Private Key File" << endl;
		exit(-1);
	}
	secretKeyFile.close();

	// Constructing an instance of Decryptor - to be able to decrypt
	Decryptor decryptor(context, secret_key);

	Plaintext plaintext;
	decryptor.decrypt(cypher, plaintext);

	return plaintext;
}


int main()
{
	//validate input file
	if (! check_signature(ROOT_CRT_FILE, COUNTER_INPUT, COUNTER_INPUT_SIGNED)){
		std::cout << "Input file NOT certified. Exiting...\n";
		exit(-1);
	}

	//get essential information
	unsigned int trustees = 0, threshold_trustees = 0, nrCandidates = 0, nrVoters = 0;
	get_sss_info(COUNTER_INPUT, trustees, threshold_trustees, nrCandidates, nrVoters);

	printf("\ntrustees %d\nthreshold_trustees %d\nnrCandidates %d\nnrVoters %d\n\n", trustees, threshold_trustees, nrCandidates, nrVoters);

	Ciphertext checksum;
	std::vector<Ciphertext> results(nrCandidates);

	checksum = generateCiphertext(CHECKSUM);
	for (int i = 0; i < nrCandidates; i++)
		results[i] = generateCiphertext(RESULTS + std::to_string(i+1) + TXT_EXTENSION);


	--------

	unsigned char restored_buf[sss_MLEN+1], c;
	FILE *fp;
	char filename[20];
	sss_Share restored_shares[trustees];
	int j = 0;


	std::string share 		= "";
	std::string share_signed = "";

	for (int i = 0; i < trustees; i++) {
		share 			= SHARE + std::to_string(i+1) + TXT_EXTENSION;
		share_signed 	= SHARE + std::to_string(i+1) + SIGNED_EXTENSION;

		//checkar

		snprintf(filename, sizeof(filename), "share%d.txt", i+1);
		fp = fopen(filename, "rb");
		j = 0;

		while (j < sss_SHARE_LEN) {
			c = fgetc(fp);
			shares2[i][j++] = c;
		}
		fclose(fp);
	}

	// Combine some of the shares to restore the original secret
	int tmp = sss_combine_shares(restored_buf, restored_shares, threshold_trustees);

	// PRINTS
	printf("\n");
    for (j = 0; j < sss_MLEN; ++j)
    	printf("restored_buf[%d] %d\n", j, restored_buf[j]);
    printf("\n");
    // PRINTS

    assert(tmp == 0);

    fp = fopen("recovered_password.txt", "wb");
	for (j = 0; j < sss_MLEN; j++) {
		fputc(restored[j], fp);
	}
	fputc('\0', fp);
	fclose(fp);

	//decrypts the checksum
	int checksum_dec = atoi((Decrypt(checksum).to_string()).c_str()) ;
	printf("CHECK É %d\n", checksum_dec);

	return 0;
}