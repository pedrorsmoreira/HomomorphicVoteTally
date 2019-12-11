#include "boost/filesystem.hpp"  

//directories
#define VOTERS_DIR ".."
#define VOTER_PRIVATE_DIR "Voter" //append ID
#define BALLOT_BOX "../BallotBox"

//root CA certificate
#define ROOT_CRT_FILE "rootCA.crt"

//voter private key and certificate
#define VOTER_KEY_FILE_NAME "voter" //append ID
#define VOTER_CRT_FILE_NAME "voter" //append ID
#define VOTER_KEY_FILE_EXTENSION ".key"
#define VOTER_CRT_FILE_EXTENSION ".crt"

//election public key
#define ELEC_KEY_FILE "publicKeyFile.dat"

//d files
#define INPUT_VOTE_FILE "input.txt"
#define OUTPUT_VOTE_FILE "vote.txt"
#define SEAL_ENCRYPTED_vote_VOTE_FILE "vote.seal"
#define VOTER_SIGNED_VOTE_FILE "vote.signed"

using namespace boost::filesystem; 

//declared global
std::string  id 		= "";
unsigned int id_integer = 0;



//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////         auxiliar functions         ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

//get the voter ID
void idenify_user(int& argc, char* argv[]){
	if (argc == 2){
		id = argv[1];
		id_integer = std::atoi(argv[1]);
	}
	else if (argc == 1){
		std::cout << "Type your ID number:";
		std::getline(std::cin, id);
		id_integer = std::atoi(argv[1]);
	}
	else{
		std::cout << "INVALID\n\n usage: vote <ID_numbernumber>"
		return 1;
	}
}

//get the number of candidates and number of votes to distribute
void get_voting_params(std::string filePATH, unsigned int& candidates, unsigned int& votes_nr){
	std::ifstream input(filePATH);
	input >> candidates >> votes_nr;
}

//write the vote in an output file
void write_vote(std::string filePATH, std::vector<unsigned int>& v){
	std::ofstream ouput(filePATH);
	for (const auto $val : v)ouput << val << "\n";
}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         main         ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
	//get user ID
	idenify_user(argc, argv);

	//check if user (directory) exists
	std::string voterPath = VOTERS_DIR + '/' VOTER_PRIVATE_DIR + id;
	if ( !exists( voterPath ) ) {
		std::cout << "Provided ID does not match any existing voter\n."
		return 2;
	}

	//put the paths for the needed files in strings (for easiness)
	std::string root_crt 			= voterPath  + '/' + ROOT_CRT_FILE;
	std::string voter_key 			= voterPath  + '/' + VOTER_KEY_FILE_NAME + VOTER_KEY_FILE_EXTENSION;
	std::string voter_crt 			= voterPath  + '/' + VOTER_CRT_FILE_NAME + VOTER_CRT_FILE_EXTENSION;
	std::string elec_key 			= voterPath  + '/' + ELEC_KEY_FILE;
	std::string input 				= voterPath  + '/' + INPUT_VOTE_FILE;
	std::string vote				= voterPath  + '/' + OUTPUT_VOTE_FILE;
	std::string encrypted_vote 		= voterPath  + '/' + OUTPUT_VOTE_ENCRYPTED_FILE;
	std::string signed_vote			= BALLOT_BOX + '/' + VOTER_SIGNED_VOTE_FILE;



	///////////////////////////////////
	// decrypt the input params file //
	///////////////////////////////////

	//get the voting parameters
	unsigned int candidates = 0;
	unsigned int votes = 0;
	get_voting_params(input, candidates, votes);
	
	//create the vector that will contain the votes
	std::vector<unsigned int> final_vote (candidates, 0); 	

	//read the vote from the voter (from the cmd)
	while (votes > 0){
		std::string aux;
		unsigned int selectedCand, selectedVotes;

		std::cout << candidates + "available candidates to vote on, and " 
		  << votes << " to distribute.\n" << std::endl;

		std::cout << "Choose a candidate to vote on (from 1 to " << candidates << "):"
		std::getline(std::cin, aux);
		selectedCand = std::atoi(argv[1]);

		if (selectedCand < 1 || selectedCand > candidates){
			std::cout << "Invalid candidate.\n\n";
			continue;
		}

		std::cout << "\nChoose the number of votes for the candidate ([" << votes << ']' << "votes left):";
		std::getline(std::cin, aux);
		selectedVotes = std::atoi(argv[1]);

		if (selectedVotes < 1 || selectedVotes > votes){
			std::cout << "Invalid value.\n\n";
			continue;
		}

		votes -= selectedVotes;
		final_vote[candidate-1] += selectedVotes;

		std::cout << "\n\n"
	}

	//write the vote in an ouput file
	write_vote(vote, final_vote);

	///////////////////////////////////
	////   encrypt the vote file   ////
	///////////////////////////////////			//função do cão
	seal_encrypt_with_public_key(vote, encrypted_vote, signed_vote);


	return 0;
}