#include "boost/filesystem.hpp"

#include "../utils/utils.cpp"

//voter space directories
#define VOTERS_DIR 			".."
#define VOTER_PRIVATE_DIR 	"Voter" //append ID

//names of the existing files
#define VOTER_KEY 					"voter" //append ID
#define VOTER_KEY_EXTENSION 		".key"

#define VOTER_KEY_SIGNED 			"voter" //append ID
#define VOTER_KEY_SIGNED_EXTENSION 	".sign"

#define VOTER_CRT_SIGNED 			"voter" //append ID
#define VOTER_CRT_SIGNED_EXTENSION 	".crt.sign"

//executable to encrypt a file with the SEAL library
#define SEAL_ENCRYPT	"seal/seal_encrypt"


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////         auxiliar functions         ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

//get the voter ID
void identify_voter(int& argc, char* argv[], std::string& id){
	if (argc == 2)
		id = argv[1];
	else if (argc == 1){
		std::cout << "Type your ID number:";
		std::getline(std::cin, id);
	}else{
		std::cout << "INVALID\n\n usage: vote <ID_number>";
		exit(-1);
	}
}

//write each vote in a diff output file
void write_votes(std::vector<unsigned int>& v, std::string& rawVote,//raw votes
				std::string& encryptedVote, std::string& Eleckey, 	//encrypted votes
				std::string voter_key, std::string signedVote,		//signed votes
				std::string dir)									//directory to act on
{		
	int iter = 0;
	for (const auto val : v){
		++iter;
		//filenames
		std::string raw_vote 		= dir + std::to_string(iter) + rawVote;

		std::string encrypted_vote 	= dir + std::to_string(iter) + encryptedVote;
		std::string elec_key 		= Eleckey;

		std::string signed_vote		= dir + std::to_string(iter) + signedVote;

		//write the raw vote
		std::ofstream raw(raw_vote);
		raw << val;
		raw.close();

		//encrypt the vote with the SEAL library using the election key
		system((SEAL_ENCRYPT + std::string(" ") + raw_vote + std::string(" ")
				 + encrypted_vote + std::string(" ") + elec_key).c_str());

		//sign the encrypted vote with the voter private key
		system(("openssl dgst -sha256 -sign " + voter_key + " -out " 
				+ signed_vote + " " + encrypted_vote).c_str());

		//erase the raw vote text file
		system(("rm " + raw_vote).c_str());
	}
}

/*
void write_vote(std::string filePATH, std::vector<unsigned int>& v){
	std::ofstream ouput(filePATH);
	for (const auto val : v)ouput << val << "\n";
}
*/

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////         main         ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
	std::string  id;
	unsigned int id_int = 0;
	//get user ID
	identify_voter(argc, argv, id);
	id_int = std::atoi(id.c_str());


	//check if user (directory) exists
	std::string voterPath = VOTERS_DIR + std::string("/") + VOTER_PRIVATE_DIR + id;
	if ( ! boost::filesystem::exists( voterPath ) ) {
		std::cout << "Provided ID does not match any existing voter\n.";
		exit(-2);
	}

	std::cout << "\n\nVoter " + id + " successefully identified.\n";

	//put the paths for the needed files in strings (for the sake of easiness)
	//existing files
	std::string root_crt 			= voterPath  + std::string("/") + ROOT_CRT_FILE;
	std::string voter_key 			= voterPath  + std::string("/") + VOTER_KEY 			+ id + VOTER_KEY_EXTENSION;
	std::string voter_key_signed	= voterPath  + std::string("/") + VOTER_KEY_SIGNED 	+ id + VOTER_KEY_SIGNED_EXTENSION;
	std::string voter_crt 			= voterPath  + std::string("/") + VOTER_CRT 			+ id + VOTER_CRT_EXTENSION;
	std::string voter_crt_signed	= voterPath  + std::string("/") + VOTER_CRT_SIGNED 	+ id + VOTER_CRT_SIGNED_EXTENSION;
	std::string election_key		= voterPath  + std::string("/") + ELEC_KEY;
	std::string election_key_signed	= voterPath  + std::string("/") + ELEC_KEY_SIGNED;
	std::string input 				= voterPath  + std::string("/") + VOTE_INPUT;
	std::string input_signed		= voterPath  + std::string("/") + VOTE_INPUT_SIGNED;

	//directory that will contain the vote
	std::string vote_directory		= VOTE_DIR;

	//files to create
	std::string vote_file			= VOTE_FILE;
	std::string vote_encrypted 		= VOTE_ENCRYPTED;
	std::string vote_signed			= VOTE_SIGNED;
	
	//validate existing files
	if (! check_signature(root_crt, voter_key, voter_key_signed)){
		std::cout << "Voter private key NOT certified. Exiting...\n";
		exit(-3);
	}
	if (! check_signature(root_crt, voter_crt, voter_crt_signed)){
		std::cout << "Voter certificate NOT certified. Exiting...\n";
		exit(-3);
	}
	if (! check_signature(root_crt, election_key, election_key_signed)){
		std::cout << "Election key NOT certified. Exiting...\n";
		exit(-3);
	}
	if (! check_signature(root_crt, input, input_signed)){
		std::cout << "Input file NOT certified. Exiting...\n";
		exit(-3);
	}


	std::cout << "Voter " + id + " successefully certified.\n\n";

	//get the voting parameters
	unsigned int nrCandidates = 0;
	unsigned int nrVotes = 0;
	get_voting_params(input, nrCandidates, nrVotes);
	
	//create the vector that will contain the votes
	std::vector<unsigned int> final_vote (nrCandidates, 0); 	

	//read the vote from the voter (from the cmd)
	while (nrVotes > 0){
		std::string aux;
		unsigned int selectedCand, selectedVotes;

		std::cout << "There are " + std::to_string(nrCandidates) + " available candidates to vote on, and "  << nrVotes << " vote(s) to distribute." << std::endl;

		std::cout << "Choose a candidate to vote on (from 1 to " << std::to_string(nrCandidates) << "):";
		std::getline(std::cin, aux);
		selectedCand = std::atoi(aux.c_str());

		if (selectedCand < 1 || selectedCand > nrCandidates){
			std::cout << "Invalid candidate.\n\n";
			continue;
		}

		std::cout << "\nChoose the number of votes for the candidate ([" << std::to_string(nrVotes) << ']' << "votes left):";
		std::getline(std::cin, aux);
		selectedVotes = std::atoi(aux.c_str());

		if (selectedVotes < 1 || selectedVotes > nrVotes){
			std::cout << "\n\n------Invalid value-------\n\n";
			continue;
		}

		nrVotes -= selectedVotes;
		final_vote[selectedCand-1] += selectedVotes;

		std::cout << "\n\n";
	}

	//write to Ballot Box
	//Voter directory in the Ballot Box
	std::string ballotVoter = BALLOT_BOX + std::string("/") + VOTER_PRIVATE_DIR + id;
	//see if there are already votes from this voter
	if (! boost::filesystem::exists(ballotVoter)){
		system(("mkdir " + ballotVoter).c_str());
		system(("touch " + ballotVoter + std::string("/") + "counter0").c_str());

	}

	//get the number of votes already done from this voter
	std::string counterFile = ssystem(("ls " + ballotVoter + " | grep counter").c_str());
	int counter = std::atoi((counterFile.substr(7)).c_str()) + 1;

	//update counter file
	system(("mv " + ballotVoter + std::string("/") + counterFile + " " + ballotVoter + std::string("/") + "counter" + std::to_string(counter)).c_str());

	//create directory with the vote to send to the Ballot Box
	std::string voteDirectory 		= vote_directory + std::to_string(counter);
	std::string votes_per_cand		= voteDirectory + std::string("/") + VOTES_PER_CAND;
	system(("mkdir "+voteDirectory).c_str());
	system(("cp " + voter_crt + " " + voteDirectory).c_str());
	system(("mkdir "+ votes_per_cand).c_str());

	//encrypt and sign the votes
	write_votes(final_vote, vote_file, 
		vote_encrypted, election_key, 
		voter_key, vote_signed,  
		votes_per_cand + std::string("/"));

	//put the vote in the Ballot box
	system(("mv "+ voteDirectory + " " + ballotVoter).c_str());


	std::cout << "\n-----------Vote successefully submitted-----------\n\n";	

	return 0;
}