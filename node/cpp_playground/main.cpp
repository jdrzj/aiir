//
//  main.cpp
//  dic_attack
//
//  Created by Jędrzej Urbański on 22.03.2015.
//  Copyright (c) 2015 Jędrzej Urbański. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <typeinfo>
#include <openssl/sha.h>

std::string sha(unsigned char * phrase, int length) {

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(phrase, length, hash);
    std::stringstream stream;
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++){
        stream << std::setw(2) << std::setfill('0') << std::hex << (int)hash[i];
    }

    return stream.str();
}



int main(int argc, const char * argv[]) {



    if (argc == 1){
        std::cout << "No password given" << std::endl;
        return 0;
    }

    const char * dictionary_file = argv[2];
    std::string password = argv[1];

    std::cout << "Password to hack: " << password << std::endl;
    std::cout << "Dictionary: " << dictionary_file << std::endl;

    std::fstream dictionary;
    dictionary.open(dictionary_file, std::ios::in);
    std::string line;

    if (dictionary.good()) {
        unsigned char *temp_uc;
        std::cout << "Starting hackin..." << std::endl;
        while (std::getline(dictionary, line)){

            //std::cout << line << std::endl;
            int len = line.length();
            temp_uc = new unsigned char[len];
            strcpy( (char*) temp_uc, line.c_str() );
            //std::cout << "długość - " << len << "  line - " << line << "  temp_uc - " << temp_uc << std::endl;

            if(sha(temp_uc, len) == password){
                std::cout << "JACKPOT!" << std::endl;
                std::cout << line << std::endl;
                std::cout << password << std::endl;
                return 0;
            }
        }
        std::cout << "no match" << std::endl;
    } else {
        std::cout << "ERR with file" << std::endl;
    }


    return 0;
}
