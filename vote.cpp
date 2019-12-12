#include "boost/filesystem.hpp"  

//directories
#define VOTERS_DIR ".."
#define VOTER_PRIVATE_DIR "Voter" //append ID
#define BALLOT_BOX "../BallotBox"

//root CA certificate
#define ROOT_CRT_FILE "rootCA.crt"

//names of the existing files
#define VOTER_KEY 			"voter" //append ID
#define VOTER_KEY_SIGNED 	"voter" //append ID
#define VOTER_CRT 			"voter" //append ID
#define VOTER_CRT_SIGNED 	"voter" //append ID"
#define VOTER_KEY_EXTENSION 		".key"
#define VOTER_KEY_EXTENSION_SIGNED 	".sign"
#define VOTER_CRT_EXTENSION 		".crt"
#define VOTER_CRT_EXTENSION_SIGNED 	".sign"
#define ELEC_KEY 		"publicKeyFile.dat"
#define ELEC_KEY_SIGNED "publicKeyFile.sign"
#define VOTE_INPUT 			"input.txt"
#define VOTE_INPUT_SIGNED 	"input.sign"

//names of the files to create
#define VOTE "vote.txt"
#define VOTE_ENCRYPTED "vote.seal"
#define VOTE_SIGNED "vote.sign" 


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
	}
	else{
		std::cout << "INVALID\n\n usage: vote <ID_number>";
		return 1;
	}
}

//check if a subject's signature is certified
bool check_signature(std::string& CA, std::string& subject, std::string& signed_subject){
	std::string key = "CApublic.key";
	std::string result = "";
	std::string certified = "Verified OK";

	system(("openssl x509 -pubkey -noout -in " + CA + " > " + key).c_str());
	certified = ssystem( ("openssl dgst -sha256 -verify " + key + " -signature " + signed_subject + " " + subject).c_str());
	
	remove(("CApublic.key").c_str());

	return result == certified;
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
    return result;
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
	std::string  id 		= "";
	unsigned int id_int = 0;
	//get user ID
	identify_voter(argc, argv, id);
	id_int = std::atoi(argv[1]);

	//check if user (directory) exists
	std::string voterPath = VOTERS_DIR + '/' VOTER_PRIVATE_DIR + id;
	if ( ! boost::filesystem::exists( voterPath ) ) {
		std::cout << "Provided ID does not match any existing voter\n.";
		return 2;
	}

	//put the paths for the needed files in strings (for the sake of easiness)
	//existing files
	std::string root_crt 			= voterPath  + '/' + ROOT_CRT_FILE;
	std::string voter_key 			= voterPath  + '/' + VOTER_KEY 			+ id + VOTER_KEY_EXTENSION;
	std::string voter_key_signed	= voterPath  + '/' + VOTER_KEY_SIGNED 	+ id + VOTER_KEY_EXTENSION_SIGNED;
	std::string voter_crt 			= voterPath  + '/' + VOTER_CRT 			+ id + VOTER_CRT_EXTENSION;
	std::string voter_crt_signed	= voterPath  + '/' + VOTER_CRT_SIGNED 	+ id + VOTER_CRT_EXTENSION_SIGNED;
	std::string election_key		= voterPath  + '/' + ELEC_KEY;
	std::string election_key_signed	= voterPath  + '/' + ELEC_KEY_SIGNED;
	std::string input 				= voterPath  + '/' + VOTE_INPUT;
	std::string input_signed		= voterPath  + '/' + VOTE_INPUT_SIGNED;

	//files to create
	std::string vote				= voterPath  + '/' + VOTE;
	std::string vote_encrypted 		= voterPath  + '/' + VOTE_ENCRYPTED;
	std::string vote_signed			= voterPath	 + '/' + VOTE_SIGNED;

	
	//validate existing files
	if (! check_signature(root_crt, voter_key, voter_key_signed)){
		std::cout << "Voter private key NOT certified. Exiting...\n";
		return -3;
	}
	check_signature(root_crt, voter_crt, voter_crt_signed){
		std::cout << "Voter certificate NOT certified. Exiting...\n";
		return -3;
	}
	check_signature(root_crt, election_key, election_key_signed){
		std::cout << "Election key NOT certified. Exiting...\n";
		return -3;
	}
	check_signature(root_crt, input, input_signed){
		std::cout << "Input file NOT certified. Exiting...\n";
		return -3;
	}

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

		std::cout << candidates + "available candidates to vote on, and "  << votes << " to distribute.\n" << std::endl;

		std::cout << "Choose a candidate to vote on (from 1 to " << candidates << "):";
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

		std::cout << "\n\n";
	}

	//write the vote in an ouput file (final_vote)
	write_vote(vote, final_vote);

	/////////////////////////////////////////////////////
	////  encrypt the vote file ---> vote_encrypted  ////
	/////////////////////////////////////////////////////			


	/////////////////////////////////////////////////////
	////  sign the encrypted vote ---> vote_signed  /////
	/////////////////////////////////////////////////////			


	//write to Ballot Box
	//Voter directory in the Ballot Box
	std::string ballotVoter = BALLOT_BOX + '/' + VOTER_PRIVATE_DIR + id;
	//see if there are already votes from this voter
	if (! boost::filesystem::exists(ballotVoter){
		system(("mkdir " + ballotVoter).c_str());
		system(("touch " + ballotVoter + '/' + "counter0").c_str());
	}

	//get the number of votes already done from this voter
	std::string counterFile = ssystem(("ls " + ballotVoter + " | grep counter").c_str());
	int counter = std::atoi(counterFile.substr(7)) + 1;

	//update counter file
	system(("mv " + ballotVoter + '/' + counterFile + " " 
		+ ballotVoter + '/' + "counter" + std::to_string(counter)).c_str());

	//create directory with the vote to send to the Ballot Box
	std::string voteDirectory = "vote" + std::to_string(counter);
	system(("mkdir "+voteDirectory).c_str());
	system(("cp " + voter_crt + " " + voteDirectory).c_str());
	system(("mv " + vote_encrypted + " " + voteDirectory).c_str());
	system(("mv " + vote_signed + " " + voteDirectory).c_str());

	//put the vote in the Ballot box
	system(("mv "+ voteDirectory + " " + ballotVoter));

	//erase the raw vote text file
	system(("rm " + final_vote).c_str());	

	return 0;
}