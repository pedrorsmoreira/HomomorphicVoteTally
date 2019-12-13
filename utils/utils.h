#ifndef UTILS
#define UTILS

#include <iostream>

void print(std::string& s);

std::string ssystem (const char *command) ;

bool check_signature(std::string& CA, std::string& subject, std::string& signed_subject);

void get_voting_params(std::string filePATH, unsigned int& candidates, unsigned int& votes_nr);


#endif