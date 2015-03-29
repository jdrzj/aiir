#include <iostream>
#include <fstream>
#include "CryptoUtils.h"
#include "Attack.h"

Attack::Attack(const AttackType& t, const std::string& k):type(t),key(k){};
Attack::~Attack(){};

void Attack::defeatKey()
{
    if(this->type == AttackType::dictionary)
    {
        dictionaryAttack(dictionary_file_name);
    }
};

void Attack::setDictionaryFileName(const std::string& file_name)
{
    this->dictionary_file_name = file_name;
};

void Attack::dictionaryAttack(std::string& file_name)
{
    //a tutaj dzieci, dzieje się magia!
    std::fstream dictionary;
    dictionary.open(file_name, std::ios::in);
    std::string line;

    std::cout << "defeat key: " << this->key << std::endl;

    if(dictionary.good())
    {
        std::cout << "GO" << std::endl;
        unsigned char * temp_uc;
        while (std::getline(dictionary, line)){
            //std::cout << line << "\n";
            Hash h = CryptoUtils::generateSHA1(line);
            //TODO: here should be method areHashesEqual
            if(CryptoUtils::convertHashToHexRep(h) == this->key){
                std::cout << "JACKPOT!" << std::endl;
                std::cout << this->key << " is " << "sha1(" << line << ")"<< std::endl;
                return ;
            }
        }
        std::cout << "no match" << std::endl;
    }else{
        std::cout << "no such file " << file_name << std::endl;
    }
};
