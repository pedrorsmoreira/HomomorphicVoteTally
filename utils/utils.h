#ifndef UTILS
#define UTILS

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define BALLOT_BOX              "../BallotBox"

//root CA certificate
#define ROOT_CRT_FILE           "rootCA.crt"

#define VOTER_CRT               "voter" //append ID
#define VOTER_CRT_EXTENSION     ".crt"

#define ELEC_KEY                "electionPublicKeyFile.dat"
#define ELEC_KEY_SIGNED         "electionPublicKeyFile.sign"

#define VOTE_INPUT              "input.txt"
#define VOTE_INPUT_SIGNED       "input.sign"

//name of the directory that will contain the vote
#define VOTE_DIR                "Vote" //vote counter will be appended

#define VOTE_ENCRYPTED          "vote.seal"
#define VOTE_SIGNED             "vote.sign"



void print(std::string& s);

std::string ssystem (const char *command) ;

bool check_signature(std::string& CA, std::string& subject, std::string& signed_subject);

void get_voting_params(std::string filePATH, unsigned int& candidates, unsigned int& votes_nr);


#endif