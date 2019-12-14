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
	std::input >> trustees >> threshold_trustees >> nrCandidates >> nrVoters;
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

	uint8_t restored_buf[sss_MLEN] = {0};
	FILE *fp;
	char filename[20];
	sss_Share restored_shares[trustees];
	sss_Share x;
	int j = 0;

	std::string share 		= "";
	std::string share_signed = "";

	for (int i = 0; i < trustees; i++) {
		share 			= SHARE + std::to_string(i+1) + TXT_EXTENSION;
		share_signed 	= SHARE + std::to_string(i+1) + SIGNED_EXTENSION;


		snprintf(filename, sizeof(filename), share.c_str(), i+1);
		fp = fopen(filename, "rb");
		j = 0;
		do {
			char c = fgetc(fp);
			*x = c;
			if (feof(fp) || c == '\n') break;
			restored_shares[i][j++] = *x;
		} while(1);
	}

	int tmp;

	// Combine some of the shares to restore the original secret
	tmp = sss_combine_shares(restored_buf, restored_shares, threshold_trustees);

	// PRINTS
	printf("\n");
    for (j = 0; j < sss_MLEN; ++j)
    	printf("restored_buf[%d] %d\n", j, restored_buf[j]);
    printf("\n");
    // PRINTS

    assert(tmp == 0);

    // Escreve num ficheiro a pass dps de juntar as shares
	fp = fopen("recovered_password.txt", "wb");
	for (j = 0; j < sss_MLEN; j++)
		fputc(restored_buf[j], fp);
	fputc('\0', fp);
	fclose(fp);


	return 0;
}