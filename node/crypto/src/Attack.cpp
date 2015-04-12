#include <iostream>
#include <fstream>
#include <math.h>
#include "boost/dynamic_bitset.hpp"
#include "CryptoUtils.h"
#include "Attack.h"
#include "RainbowCracker.h"

Attack::Attack(const AttackType& t, const std::string& k, HashingFunction
    f)
    : type(t)
    , key(k)
    , function(f) {}

Attack::~Attack() {}

void Attack::defeatKey()
{
    if(this->type == AttackType::dictionary)
    {
        dictionaryAttack(dictionary_file_name);
    }
    else if(this->type == AttackType::rainbow)
    {
        rainbowAttack();
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
        while (std::getline(dictionary, line))
        {
            hackify(line);
        }
        std::cout << "no match" << std::endl;
    }
    else
    {
        std::cout << "no such file " << file_name << std::endl;
    }
};

void Attack::rainbowAttack()
{
    std::cout << "defeat key: " << this->key << std::endl;
    Hash desired_hash = CryptoUtils::convertHexRepToHash(key, function);
    RainbowCracker rainbow;
    rainbow.generateChains(2, function);
    std::string pass = rainbow.run(desired_hash, function);
    if (pass != "")
    {
        std::cout << "JACKPOT!" << std::endl;
        std::cout << this->key << " is " << "sha1(" << pass << ")"<< std::endl;
    }
}

void Attack::hackify(std::string pass)
{
    //liczba zapisana bitowo
    // 0 - downcase
    // 1 - uppercase

    std::string s;
    std::string pass2;
    int x = pass.length();
    for(int i = 0; i < pow(2, pass.length()); i++)
    {
        pass2 = pass;
        const boost::dynamic_bitset<> b2(x, i);
        boost::to_string(b2, s);
        for(int j = 0; j < s.length(); j++)
        {
            if(s[j] == '1')
            {
                pass2[j] = toupper(pass[j]);
            }
            else if (s[j] == '0')
            {
                pass2[j] = tolower(pass[j]);
            }
        }
        check_suffixes(pass2, 0);
    }
}

void Attack::check_suffixes(std::string pass, int level = 0)
{
    const int max_level = 3;
    if(level != max_level)
    {
        std::string pass2 = pass;
        for(int i = 0; i < suffixes.length(); i++)
        {
            Hash h = CryptoUtils::generateSHA1(pass2);
            //TODO: here should be method areHashesEqual
            std::cout << pass2 << " = " << CryptoUtils::convertHashToHexRep(h) << std::endl;
            if(CryptoUtils::convertHashToHexRep(h) == this->key)
            {
                std::cout << "JACKPOT!" << std::endl;
                std::cout << this->key << " is " << "sha1(" << pass2 << ")"<< std::endl;
                return;
            }

            pass2 = pass;
            pass2 += suffixes[i];

            check_suffixes(pass2, level+1);
        }
    }
}
