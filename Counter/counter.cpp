#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>

extern "C"{
	#include "sss/sss.h"
	#include "sss/randombytes.h"
	#include <assert.h>
}

#include "../utils/utils.cpp"

#include "seal/seal.h"
using namespace seal;
using namespace std;

//#define COUNTER 					"../counter"

#define CHECKSUM_FILE				"checksum.txt"

#define RESULTS						"results"
#define RESULTS_EXTENSION			".txt"

//root CA certificate
#define ROOT_CRT_FILE 				"rootCA.crt"

//names of the existing files
#define SHARE 						"share" //append ID
#define SHARE_EXTENSION 			".txt"

#define SHARE 						"share" //append ID
#define SHARE_SIGNED_EXTENSION 		".sign"

#define COUNTER_INPUT 				"inputCounter.txt"
#define COUNTER_INPUT_SIGNED 		"inputCounter.sign"

//name of the file to create with the private key
#define PRIVATE_KEY_FILE_PATH		"electionSecretKeyFile.txt"












void getpass()
{	
	char file_name[30], file_sign_name[30];
	ifstream fp;
	ofstream pass_file, test;
	sss_Share shares2[shares_threshold];
	unsigned char recover[130], c;

	
	for (int j = 0;j < shares_threshold; j++)
	{
		snprintf(file_name, sizeof(file_name), "pass_%d.txt", j+1);
		snprintf(file_sign_name, sizeof(file_sign_name), "pass_%d.sha256", j+1);
		
		if( !verifySignature("rootCA.crt", file_sign_name, file_name) ){
			cout << "At least one of the electionPrivateKey shares signature is wrong!" << endl;
			exit(EXIT_FAILURE);
		}
		
		fp.open(file_name, ios::binary);
		test.open("test.txt", ios::trunc | ios::binary);
		int w = 0;
		while ( w < sss_SHARE_LEN )
		{
			c = fp.get();
		
			shares2[j][w] = c;
			test.put(c);
			w++;
		}
		fp.close();
		test.close();
	}
	
	int tmp;
	// Combine some of the shares to restore the original secret
	tmp = sss_combine_shares(recover, shares2, shares_threshold);
	assert(tmp == 0);
	
	// Escreve num ficheiro a pass dps de juntar as shares
	pass_file.open("recovered_pass.txt", ios::trunc);
	for (int j = 0;j< 64;j++) {
		pass_file.put(recover[j]);
	}
	pass_file.put('\0');
	pass_file.close();

}













//get the number of candidates and number of votes to distribute
void get_sss_info(string filePATH, unsigned int& trustees, unsigned int& threshold, unsigned int& candidates, unsigned int& voters){
	ifstream input(filePATH);
	input >> trustees >> threshold >> candidates >> voters;
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

Ciphertext generateCiphertext(std::string filename)
{
printf("filename %s\n", filename.c_str());
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

int main(int argc, char* argv[])
{
	Ciphertext checksum;
	vector<Ciphertext> results;

	//validate input file
	if (! check_signature(ROOT_CRT_FILE, COUNTER_INPUT, COUNTER_INPUT_SIGNED)){
		std::cout << "Input file NOT certified. Exiting...\n";
		exit(-1);
	}

	//get essential information
	unsigned int num_shares = 0, shares_threshold = 0, candidates = 0, voters = 0;
	get_sss_info(COUNTER_INPUT, num_shares, shares_threshold, candidates, voters);

	checksum = generateCiphertext(CHECKSUM_FILE);
	for (int i = 0; i < candidates; i++)
		results.push_back(generateCiphertext(RESULTS + to_string(i+1) + RESULTS_EXTENSION));

	getpass();

	/*
	uint8_t restored[sss_MLEN] = {0};
	FILE *fp;
	char filename[20];
	sss_Share shares[num_shares];
	sss_Share x;
	int j = 0;

	for (int i = 0; i < num_shares; i++) {
		string share 		= SHARE + to_string(i+1) + SHARE_EXTENSION;
		string share_signed = SHARE + to_string(i+1) + SHARE_SIGNED_EXTENSION;

		if (! check_signature(ROOT_CRT_FILE, share, share_signed)){
			std::cout << "Trustee NOT certified. Exiting...\n";
			exit(-1);
		}

		//strcpy(filename, share.c_str());
		/*snprintf(filename, sizeof(filename), "share%d.txt", i+1);
		fp = fopen(filename, ios::binary);
		j = 0;
		do {
			char c = fgetc(fp);
			//printf("C é %c e J %d e I é %d ----\n", c, j, i);
			*x = c;
			if (feof(fp) || c == '\n') break;
			shares[i][j++] = *x;
		} while(1);

		ifstream fp;
		fstream pass_file, test;
		unsigned char recover[130], c;

		fp.open(filename, ios::binary);
		test.open("test.txt", ios::trunc | ios::binary);
		int w = 0;
		while ( w < sss_SHARE_LEN )
		{
			c = fp.get();
		
			shares[j][w] = c;
			test.put(c);
			w++;
		}
		fp.close();
		test.close();
	}

	int tmp;
	// Combine some of the shares to restore the original secret (restore is the private key)
	//for (int ii = 0; ii < sss_MLEN; +ii)
	//	if (restored[ii] != 0)
	//		printf("%d ---- %u\n", ii, restored[ii]);
	tmp = sss_combine_shares(restored, shares, shares_threshold);printf("AAAQQQQUUIIII\n");
	//assert(tmp == 0);

	// Escreve num ficheiro a pass dps de juntar as shares
	fp = fopen("recovered_pass.txt", "wb");
	for (j = 0; j < sss_MLEN; j++) {
		fputc(restored[j], fp);
	}
	fputc('\0', fp);
	fclose(fp);

	for (int i = 0; i < sss_MLEN; ++i)
    	printf("restored[%d] %d\n", i, restored[i]);

    */

	string path_to_enc_sk 	= "electionSecretKeyFile.txt.enc";
	string path_to_sk 		= "electionSecretKeyFile.txt";
  
	system(("openssl enc -aes-256-cbc -d -in " + path_to_enc_sk + " -out " + PRIVATE_KEY_FILE_PATH " -pass file:recovered_pass.txt -iter 10").c_str());

	int checksum_dec;
	vector<int> results_dec;

	//decrypts the checksum
	checksum_dec = atoi(((Decrypt(checksum)).to_string()).c_str());
	printf("CHECK É %d\n", checksum_dec);
	//checks the checksum validity
	if(checksum_dec == candidates * voters)
	{
		cout << "Elections results:\n";

		//decryptes the elections results
		for(int i = 0; i < results.size(); i++) {
			results_dec.push_back(atoi(((Decrypt(results[i])).to_string()).c_str()));
			cout << "Candidate" + to_string(i+1) + ": " + to_string(results_dec[i]) + "votes\n";
		}
		cout << "\n";
	}
}
