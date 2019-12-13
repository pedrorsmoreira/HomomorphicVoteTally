#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <assert.h>
#include "sss/sss.h"
#include "sss/randombytes.h"
#include "boost/filesystem.hpp"
#include "seal_encrypt.h"

using namespace std;
using namespace seal;

//#define COUNTER 					"../counter"

#define checksumPATH				"checksum.txt"
#define resultsPATH					"results.txt"

//root CA certificate
#define ROOT_CRT_FILE 				"rootCA.crt"

//names of the existing files
#define SHARE 						"share" //append ID
#define SHARE_EXTENSION 			".txt"

#define SHARE 						"share" //append ID
#define SHARE_SIGNED_EXTENSION 		".sign"

#define COUNTER_INPUT 				"input.txt"
#define COUNTER_INPUT_SIGNED 		"input.sign"

//name of the file to create with the private key
#define PRIVATE_KEY_FILE			"electionSecretKeyFile.dat"

//get the number of candidates and number of votes to distribute
void get_sss_info(string filePATH, int& num_shares, int& shares_threshold, int& candidates, int& votes){
	ifstream input(filePATH);
	input >> trustees >> threshold >> candidates >> votes;
}

//executes the input string on the terminal and returns the output of the command
std::string ssystem (const char *command) {
    char tmpname [L_tmpnam];
    std::tmpnam ( tmpname );
    std::string scommand = command;
    std::string cmd = scommand + " >> " + tmpname;
    std::system(cmd.c_str());
    std::ifstream file(tmpname, std::ios::in | std::ios::binary );
    std::string result;
    if (file) {
        while (!file.eof()) result.push_back(file.get())
            ;
        file.close();
    }
    remove(tmpname);

    //remove \0 and \n
    if (! result.empty())
    	result.pop_back();
    if (! result.empty())
    	result.pop_back();

    return result;
}

//check if a subject's signature is certified by CA
bool check_signature(std::string& CA, std::string& subject, std::string& signed_subject){
	string key = "CApublic.key";
	string result = "";
	string certified = "Verified OK";

	system(("openssl x509 -pubkey -noout -in " + CA + " > " + key).c_str());
	result = ssystem( ("openssl dgst -sha256 -verify " + key + " -signature " + signed_subject + " " + subject).c_str());

	remove("CApublic.key");

	return result == certified;//return result.find(certified) != std::string::npos;
}

Plaintext Decrypt(Chypertext cypher)
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
	ifstream privateKeyFile;
	SecretKey privatekey;
	privateKeyFile.open(PRIVATE_KEY_FILE_PATH, ios::binary);
	if (privateKeyFile.is_open())
		private_key.unsafe_load(context, privateKeyFile);
	else {
		cout << "Unable to open Private Key File" << endl;
		return 1;
	}
	privateKeyFile.close();

	// Constructing an instance of Decryptor - to be able to decrypt
	Decryptor decryptor(context, private_key);

	Plaintext plaintext;
	decryptor.decrypt(cypher, plaintext);

	return plaintext;
}

Chypertext stringToCiphertext(string str)
{
	fstream ct;
	Ciphertext cypher;

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

	ct.open("aux_file.txt", fstream::binary | fstream::out | fstream::trunc | fstream::in);
	ct << str;
	cypher.unsafe_load(context, ct);
	ct.close();
	remove("aux_file.txt");
	
	return cypher;
}

Chypertext generateChypertext(string checksumPath)
{
	ifstream checksum_file;
	checksum_file.open(checksumPath, ios::binary);
	if (!checksum_file.is_open()) {
		cout << "Unable to open results File" << endl;
		return 1;
	}

	string line;
	Chypertext checksum;

	getline(checksum_file, line);
	//cout << "checksum" << line << '\n';
	checksum = stringToCiphertext(line);
    return checksum;
}

vector<Chypertext> generateVectorOfChypertext(string resultsPath)
{
	ifstream results_file;
	results_file.open(resultsPath, ios::binary);
	if (!results_file.is_open()) {
		cout << "Unable to open results File" << endl;
		return 1;
	}

	string line;
	vector<Chypertext> results;

	while ( getline(results_file, line) ) {
		//cout << line << '\n';
		results.push_back(stringToCiphertext(line));
    }

    return results;
}

int main(int argc, char* argv[]) {

	Chypertext checksum;
	vector<Chypertext> results;

	checksum = generateChypertext(checksumPATH);
	results = generateVectorOfChypertext(resultsPATH);

	string root_crt = ROOT_CRT_FILE;

	string input = COUNTER_INPUT;
	string input_signed = COUNTER_INPUT_SIGNED;



	//validate input file
	if (! check_signature(root_crt, input, input_signed)){
		std::cout << "Input file NOT certified. Exiting...\n";
		exit(-3);
	}

	//get essential information
	int num_shares = 0;
	int shares_threshold = 0;
	int candidates = 0;
	int votes = 0;
	get_voting_params(input, num_shares, shares_threshold, candidates, votes);

	uint8_t restored[sss_MLEN] = {0};
	FILE *fp;
	char filename[20];
	sss_Share shares[num_shares];
	sss_Share x;
	int j = 0;

	for (int i = 0; i < num_shares; i++) {
		string share = SHARE + to_string(i+1) + SHARE_EXTENSION;
		string share_signed = SHARE + to_string(i+1) + SHARE_SIGNED_EXTENSION;

		if (! check_signature(root_crt, share, share_signed)){
			std::cout << "Trustee NOT certified. Exiting...\n";
			exit(-3);
		}

		strcpy(filename, share.c_str());
		//snprintf(filename, sizeof(filename), "share%d.txt", i+1);
		fp = fopen(filename, "rb");
		j = 0;
		do {
			char c = fgetc(fp);
			*x = c;
			if (feof(fp) || c == '\n') break;
			shares[i][j++] = *x;
		} while(1);
	}

	int tmp;
	// Combine some of the shares to restore the original secret ( restore is the private key)
	tmp = sss_combine_shares(restored, shares, shares_threshold);
	assert(tmp == 0);

	// Escreve num ficheiro a pass dps de juntar as shares
	fp = fopen("recovered_pass.txt", "wb");
	for (j = 0; j < sss_MLEN; j++) {
		fputc(restored[j], fp);
	}
	fputc('\0', fp);
	fclose(fp);

	string path_to_enc_sk = "../Administrator/ElectionKey/electionSecretKeyFile.dat.enc";
	string path_to_sk = "electionSecretKeyFile.dat";
  
	system(("openssl enc -aes-256-cbc -d -in " + path_to_enc_sk + " -out " + PRIVATE_KEY_FILE " -pass file:recovered_pass.txt -iter 10").c_str());

	int checksum_dec;
	vector<int> results_dec;

	//decrypts the checksum
	checksum_dec = atoi(((Decrypt(checksum)).to_string()).c_str());

	//checks the checksum validity
	if(checksum_dec == candidates * votes)
	{
		cout << "Elections results:\n";

		//decryptes the elections results
		for(int i = 0; i < results.size(); i++) {
			results_dec[i] = atoi(((Decrypt(results[i])).to_string()).c_str());
			cout << "Candidate" + to_string(i+1) + ": " + results_dec[i] + "votes\n";
		}
		cout << "\n";
	}
}





	


