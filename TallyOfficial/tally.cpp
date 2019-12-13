#include <iostream>
#include <fstream>

#include "../utils/utils.cpp"

#include "seal/seal.h"
using namespace seal;

#define WEIGHTS 		"encryptedWeightsFile.dat"
#define WEIGHTS_SIGNED 	"encryptedWeightsFile.sign"

Ciphertext zeroInCiphertext()
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

	if (!check_signature(ROOT_CRT_FILE, ELEC_KEY, ELEC_KEY_SIGNED)){
		std::cout << "Election public key NOT certified. Exiting...\n";
		exit(-3);
	}

	// Loading the election public key from the file
	PublicKey public_key;
	std::ifstream publicKeyFile;
	publicKeyFile.open(ELEC_KEY, std::ios::binary);
	if (publicKeyFile.is_open())
		public_key.unsafe_load(context, publicKeyFile);
	else {
		std::cout << "Unable to open Public Key File" << std::endl;
		exit(-3);
	}
	publicKeyFile.close();

	// Constructing an instance of Encryptor - to be able to encrypt
	Encryptor encryptor(context, public_key);

	Plaintext zero_plain("0");
	Ciphertext zero_encrypted;
	encryptor.encrypt(zero_plain, zero_encrypted);

	return zero_encrypted;
}


Ciphertext stringToCiphertext(std::string word)
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
	std::fstream file;
	file.open("conversion.txt", std::ios::trunc);
	if (!file.is_open()) {
		std::cout << "Unable to open Conversion File - string to Ciphertext" << std::endl;
		exit(-3);
	}
	file << word;

	newCiphertext.unsafe_load(context, file);

	return newCiphertext;
}

std::vector<Ciphertext> generateVectorOfCiphertext(std::string vote_encrypted)
{
	std::ifstream voteEncryptedFile;
	voteEncryptedFile.open(vote_encrypted, std::ios::binary);
	if (!voteEncryptedFile.is_open()) {
		std::cout << "Unable to open Vote Encrypted File" << std::endl;
		exit(-3);
	}

	std::string line;
	std::vector<Ciphertext> votesOfVoter;
	while ( getline (voteEncryptedFile, line) ) {
		std::cout << line << '\n';

		votesOfVoter.push_back(stringToCiphertext(line));
    }

    return votesOfVoter;
}

Ciphertext sumResult(Ciphertext encrypted1, Ciphertext encrypted2)
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

	Evaluator evaluator(context);

	evaluator.add_inplace(encrypted1, encrypted2);

	return encrypted1;
}

Ciphertext multiplyResult(Ciphertext encrypted1, Ciphertext encrypted2)
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

	Evaluator evaluator(context);

	evaluator.multiply_inplace(encrypted1, encrypted2);

	return encrypted1;
}

int main(int argc, char* argv[])
{
	std::vector<Ciphertext> voteVecCiphertext;
	Ciphertext checksum;
	std::vector<Ciphertext> results;

	//Initializations
	//get the voting parameters
	if (!check_signature(ROOT_CRT_FILE, VOTE_INPUT, VOTE_INPUT_SIGNED)){
		std::cout << "Weights NOT certified. Exiting...\n";
		exit(-3);
	}
	unsigned int candidates = 0;
	unsigned int votes = 0;
	get_voting_params(VOTE_INPUT, candidates, votes);

	checksum = zeroInCiphertext();
	for (int i = 0; i < candidates; ++i)
		results.push_back(zeroInCiphertext());

	if (!check_signature(ROOT_CRT_FILE, WEIGHTS, WEIGHTS_SIGNED)){
		std::cout << "Weights NOT certified. Exiting...\n";
		exit(-3);
	}

	std::vector<Ciphertext> weights = generateVectorOfCiphertext(WEIGHTS);

	std::string votePath = "";
	std::string voter_crt = "";
	std::string vote_encrypted = "";
	std::string vote_signed = "";

	std::vector<std::string> votersVec;

	//get a string with all the folders of the voters
	std::string voters = ssystem((std::string("ls ") + BALLOT_BOX).c_str());

	//get each voter folder (string) to the vector
	std::string delimiter = " ";
	size_t pos = 0;
	std::string token = "";
	while ((pos = voters.find(delimiter)) != std::string::npos) {
		token = voters.substr(0, pos);
		votersVec.push_back(token);
	    std::cout << token << std::endl;
	    voters.erase(0, pos + delimiter.length());
	}
	std::cout << voters << std::endl;

	for(const auto voter : votersVec) {
		
		//Voter directory in the Ballot Box
		std::string ballotVoter = BALLOT_BOX + std::string("/") + voter;
		
		//get the number of votes casts by this voter
		std::string counterFile = ssystem(("ls " + ballotVoter + " | grep counter").c_str());
		int counter = std::atoi((counterFile.substr(7)).c_str());

		//we start with the last vote
		for (int id = counter; id > 0; id--) {
			std::cout << id << "\n";

			votePath 		= ballotVoter 	+ std::string("/") + VOTE_DIR + std::to_string(id);

			voter_crt 		= votePath 		+ std::string("/") + VOTER_CRT + std::to_string(id) + VOTER_CRT_EXTENSION;
			vote_encrypted 	= votePath 	+ std::string("/") + VOTE_ENCRYPTED;
			vote_signed 	= votePath 	+ std::string("/") + VOTE_SIGNED;

			//validate existing files
			if (check_signature(voter_crt, vote_encrypted, vote_signed))
				break;
			else {
				std::cout << "Vote " + std::to_string(id) + " NOT certified. Cheking next one...\n";
			}
		}

		voteVecCiphertext = generateVectorOfCiphertext(vote_encrypted);

		// Computes homomorphically:
		for (int i = 0; i < candidates; ++i) {
			// the checksum for each vote and adds it to an accumulator
			checksum = sumResult(checksum, voteVecCiphertext[i]);
			// the result of the election
			results[i] = sumResult(results[i], multiplyResult(voteVecCiphertext[i], weights[voter]));
		}
	}

	return 0;
}