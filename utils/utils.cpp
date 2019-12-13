#include <iostream>

#include "seal/seal.h"
using namespace seal;

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
#define VOTES_PER_CAND           "votes_per_cand"

#define VOTE_FILE               "vote.txt"
#define VOTE_ENCRYPTED          "vote.seal"
#define VOTE_SIGNED             "vote.sign"

//for debugging
void print(std::string& s){
    std::cout << "\n" + s + "\n";
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

//check if a subject's signature is certified
bool check_signature(std::string CA, std::string subject, std::string signed_subject){
    std::string key = "CApublic.key";
    std::string result = "";
    std::string certified = "Verified OK";

    system(("openssl x509 -pubkey -noout -in " + CA + " > " + key).c_str());
    result = ssystem( ("openssl dgst -sha256 -verify " + key + " -signature " + signed_subject + " " + subject).c_str());

    remove("CApublic.key");

    return result == certified;//return result.find(certified) != std::string::npos;
}

//get the number of candidates and number of votes to distribute
void get_voting_params(std::string filePATH, unsigned int& candidates, unsigned int& votes_nr, unsigned int& voters_nr){
    std::ifstream input(filePATH);
    input >> candidates >> votes_nr >> voters_nr;
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