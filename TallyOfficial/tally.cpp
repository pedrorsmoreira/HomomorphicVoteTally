#include <iostream>
#include <fstream>
#include <string>

#include "../utils/utils.cpp"

#include "seal/seal.h"
using namespace seal;

#define CANDIDATE_VOTE_DIR 	"votes_per_cand"
#define CANDIDATE_VOTE 		"vote"
#define WEIGHTS 			"encryptedWeightsFile"
#define DAT_EXTENSION 		".dat"
#define TXT_EXTENSION 		".txt"
#define SIGNED_EXTENSION 	".sign"
#define SEAL_EXTENSION 	 	".seal"

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
		exit(-1);
	}

	// Loading the election public key from the file
	PublicKey public_key;
	std::ifstream publicKeyFile;
	publicKeyFile.open(ELEC_KEY, std::ios::binary);
	if (publicKeyFile.is_open())
		public_key.unsafe_load(context, publicKeyFile);
	else {
		std::cout << "Unable to open Public Key File" << std::endl;
		exit(-1);
	}
	publicKeyFile.close();

	// Constructing an instance of Encryptor - to be able to encrypt
	Encryptor encryptor(context, public_key);

	Plaintext zero_plain("0");
	Ciphertext zero_encrypted;
	encryptor.encrypt(zero_plain, zero_encrypted);

	return zero_encrypted;
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
	Ciphertext checksum;
	std::vector<Ciphertext> results;
	std::vector<Ciphertext> weights;
	std::vector<Ciphertext> voteVecCiphertext;
	bool valid = true;
	int voterID = 0;


	std::string ballotVoter 			= "";
	std::string counterFile 			= "";
	std::string votePath 				= "";
	std::string voterCrt 				= "";
	std::string candidatesVotePath 		= "";
	std::string candidateVoteFile 		= "";
	std::string candidateVoteFileSigned = "";

	//Initializations
	//get the voting parameters
	if (!check_signature(ROOT_CRT_FILE, VOTE_INPUT, VOTE_INPUT_SIGNED)){
		std::cout << "Input file NOT certified. Exiting...\n";
		exit(-1);
	}

	unsigned int nrCandidates = 0;
	unsigned int useless = 0;
	unsigned int nrVoters = 0;
	get_voting_params(VOTE_INPUT, nrCandidates, useless, nrVoters);

printf("nrCandidates %d nrVotes %d nrVoters %d\n", nrCandidates, useless, nrVoters);

	checksum = zeroInCiphertext();
	for (int i = 0; i < nrCandidates; ++i)
		results.push_back(zeroInCiphertext());

printf("RESULTS AND CHECKSUM DONE\n");

	std::string weightsFile = "";
	std::string weightsFileSigned = "";
	for (int i = 0; i < nrVoters; ++i) {
		weightsFile 		= std::string("./WeightsEncrypted/") + WEIGHTS + std::to_string(i+1) + DAT_EXTENSION;
		weightsFileSigned 	= std::string("./WeightsEncrypted/") + WEIGHTS + std::to_string(i+1) + SIGNED_EXTENSION;
		if (!check_signature(ROOT_CRT_FILE, weightsFile, weightsFileSigned)){
			std::cout << "Weights NOT certified. Exiting...\n";
			exit(-1);
		}

		weights.push_back(generateCiphertext(weightsFile));
	}

printf("WEIGHTS DONE\n");
	
	//get a string with all the folders of the voters
	std::string voters = ssystem((std::string("ls ") + BALLOT_BOX).c_str());

	std::vector<std::string> votersVec;
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
		++voterID;

printf("---> %s\n", voter);
		
		//Voter directory in the Ballot Box
		ballotVoter = BALLOT_BOX + std::string("/") + voter;

		//get the number of votes casts by this voter
		counterFile = ssystem(("ls " + ballotVoter + " | grep counter").c_str());
		int counter = std::atoi((counterFile.substr(7)).c_str());

printf("counter %d\n", counter);
		
		//we start with the last vote
		for (int id = counter; id > 0; id--) {

printf("id %d\n", id);
			
			votePath = ballotVoter + std::string("/") + VOTE_DIR + std::to_string(id);
			voterCrt = votePath + std::string("/") + VOTER_CRT + std::to_string(id) + VOTER_CRT_EXTENSION;
			candidatesVotePath = votePath + std::string("/") + CANDIDATE_VOTE_DIR;

			for (int j = 0; j < nrCandidates; ++j) {
				candidateVoteFile 		= candidatesVotePath + std::string("/") + std::to_string(j) + CANDIDATE_VOTE + SEAL_EXTENSION;
				candidateVoteFileSigned = candidatesVotePath + std::string("/") + std::to_string(j) + CANDIDATE_VOTE + SIGNED_EXTENSION;
				if (!check_signature(voterCrt, candidateVoteFile, candidateVoteFileSigned)) {
					std::cout << "Candidate Vote NOT certified. Voter " << id << "NOT valid\n";
					valid = false;
					break;
				}

				voteVecCiphertext[j] = generateCiphertext(candidateVoteFile);
			}

			if (valid) break;
			else valid = true;
		}

		if (valid) {
			for (int j = 0; j < nrCandidates; ++j) {
				// Computes homomorphically:
				// the checksum for each vote and adds it to an accumulator
				checksum = sumResult(checksum, voteVecCiphertext[j]);
				// the result of the election - weight is the one of the voter
				results[j] = sumResult(results[j], multiplyResult(voteVecCiphertext[j], weights[voterID]));
			}
		} else
			std::cout << "No vote from the voter " << voterID << "\n";
	}

	// Writes a file with the checksum 
	std::ofstream checksumFile;
	checksumFile.open("../Counter/checksum.txt", std::ios::binary | std::ios::app);
	checksum.save(checksumFile);
	checksumFile.close();

	std::ofstream resultsFile;
	std::string output = "";
	for (int i = 0; i < nrCandidates; ++i) {
		output = std::string("../Counter/results") + std::to_string(i+1) + TXT_EXTENSION;
		resultsFile.open(output.c_str(), std::ios::binary | std::ios::app);
		results[i].save(resultsFile);
		resultsFile.close();
	}

	return 0;
}


