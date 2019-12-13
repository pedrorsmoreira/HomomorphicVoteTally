#include <iostream>

#define WEIGHTS 		"encryptedWeightsFile.dat"
#define WEIGHTS_SIGNED 	"encryptedWeightsFile.sign"

Chypertext zeroInChypertext()
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
	ifstream publicKeyFile;
	publicKeyFile.open(ELEC_KEY, ios::binary);
	if (publicKeyFile.is_open())
		public_key.unsafe_load(context, publicKeyFile);
	else {
		cout << "Unable to open Public Key File" << endl;
		return 1;
	}
	publicKeyFile.close();

	// Constructing an instance of Encryptor - to be able to encrypt
	Encryptor encryptor(context, public_key);

	Plaintext zero_plain("0");
	Ciphertext zero_encrypted;
	encryptor.encrypt(zero_plain, zero_encrypted);

	return zero_encrypted;
}


Chypertext stringToCiphertext(std::string word)
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

	Chypertext newChypertext;
	fstream file;
	file.open("conversion.txt", ios::trunc);
	if (!voteEncryptedFile.is_open()) {
		cout << "Unable to open Conversion File - string to chypertext" << endl;
		return 1;
	}
	file << word;

	newChypertext.unsafe_load(context, file);

	return newChypertext;
}

std::vector<Chypertext> generateVectorOfChypertext(std::string vote_encrypted)
{
	ifstream voteEncryptedFile;
	voteEncryptedFile.open(vote_encrypted, ios::binary);
	if (!voteEncryptedFile.is_open()) {
		cout << "Unable to open Vote Encrypted File" << endl;
		return 1;
	}

	std::string line;
	std::vector<Chypertext> votesOfVoter;
	while ( getline (inputFile, line) ) {
		cout << line << '\n';

		votesOfVoter.push_back(stringToCiphertext(line));
    }

    return votesOfVoter;
}

Chypertext sumResult(Chypertext encrypted1, Chypertext encrypted2)
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

Chypertext multiplyResult(Chypertext encrypted1, Chypertext encrypted2)
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
	std::vector<Chypertext> voteVecChypertext;
	Chypertext checksum;
	std::vector<Chypertext> results;
	std::vector<Chypertext> weights;

	//Initializations
	//get the voting parameters
	unsigned int candidates = 0;
	unsigned int votes = 0;
	get_voting_params(input, candidates, votes);

	checksum = zeroInChypertext();
	for (int i = 0; i < candidates; ++i)
		results.push_back(zeroInChypertext());

	if (!check_signature(ROOT_CRT_FILE, WEIGHTS, WEIGHTS_SIGNED)){
		std::cout << "Weights NOT certified. Exiting...\n";
		exit(-3);
	}

	weights = generateVectorOfChypertext(WEIGHTS);

	std::string votePath = "";
	std::string voter_crt = "";
	std::string vote_encrypted = "";
	std::string vote_signed = "";

	std::vector<std::string> votersVec;

	//get a string with all the folders of the voters
	std::string voters = ssystem(("ls " + BALLOT_BOX).c_str());

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
			cout << id << "\n";

			votePath 		= ballotVoter 	+ std::string("/") + VOTE_DIR + id;

			voter_crt 		= votePath 		+ std::string("/") + VOTER_CRT + id + VOTER_CRT_EXTENSION;
			vote_encrypted 	= voterPath 	+ std::string("/") + VOTE_ENCRYPTED;
			vote_signed 	= voterPath 	+ std::string("/") + VOTE_SIGNED;

			//validate existing files
			if (check_signature(voter_crt, vote_encrypted, vote_signed))
				break;
			else {
				std::cout << "Vote " + id + " NOT certified. Cheking next one...\n";
			}
		}

		voteVecChypertext = generateVectorOfChypertext(vote_encrypted);

		// Computes homomorphically:
		for (int i = 0; i < candidates; ++i) {
			// the checksum for each vote and adds it to an accumulator
			checksum = sumResult(checksum, voteVecChypertext[i]);
			// the result of the election
			results[i] = sumResult(results[i], multiplyResult(voteVecChypertext[i], weights[voter]));
		}
	}

	return 0;
}