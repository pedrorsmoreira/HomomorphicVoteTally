void computesHomomofphically(std::string vote_encrypted)
{
	ifstream voteEncryptedFile;

	voteEncryptedFile.open(vote_encrypted, ios::binary);
	if (!voteEncryptedFile.is_open()) {
		cout << "Unable to open Vote Encrypted File" << endl;
		return 1;
	}

	


}



int main(int argc, char* argv[])
{
	//Initializations
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

		// RESTO DAS MERDAS

	} 


	return 0;
}