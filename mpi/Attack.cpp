#include <iostream>
#include <fstream>
#include <math.h>
#include "boost/dynamic_bitset.hpp"
#include "CryptoUtils.h"
#include "Attack.h"
#include "RainbowCracker.h"
#include <cstring>

Attack::Attack(const AttackType& t, const std::string& k, HashingFunction f)
    : type(t)
    , key(k)
    , function(f) {}

Attack::~Attack() {}

void Attack::defeatKey()
{
    if(this->type == AttackType::rainbow)
    {
        rainbowAttack();
    }
    else if(this->type == AttackType::bruteForce)
	{
		bruteForceAttack();
	}
};

void Attack::defeatKey(int rank, int size)
{
  if(this->type == AttackType::dictionary)
  {
    dictionaryAttack(dictionary_file_name, rank, size);
  }
}

void Attack::setDictionaryFileName(const std::string& file_name)
{
    this->dictionary_file_name = file_name;
};

void Attack::dictionaryAttack(std::string& file_name, int rank, int size)
{
    std::fstream dictionary;
    dictionary.open(file_name, std::ios::in);
    std::string line;

    if(dictionary.good())
    {
        std::cout << "GO" << std::endl;
        int i = 0;
        while (++i, std::getline(dictionary, line))
        {
            if(i % size == rank)
            {
                std::cout << "defeat key: " << this->key << std::endl;
                std::cout << "hackifyin " << line << " by " << rank << std::endl;
                hackify(line);
            }
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
    rainbow.generateChains(chains_range_start, chains_range_end, 2, function);
    std::string pass = rainbow.run(desired_hash, function);
    if (pass != "")
    {
        std::cout << "JACKPOT!" << std::endl;
        std::cout << this->key << " is " << "sha1(" << pass << ")"<< std::endl;
    }
}

void Attack::bruteForceAttack()
{
	Hash desiredHash = CryptoUtils::convertHexRepToHash(key, function);
	std::string pass = " ";
	int stringPosition = 0;
	while(true)
	{
    std::cout << pass << std::endl;
		Hash passHash = CryptoUtils::generateSHA1(pass);
		if(CryptoUtils::convertHashToHexRep(passHash) == this->key)
		{
			std::cout << "success!" << std::endl;
			std::cout << this->key << " is " << "sha1(" << pass << ")"<< std::endl;
			break;
		}
		else
		{
			CryptoUtils::incrementString(pass, stringPosition);
		}
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
        checkSuffixes(pass2, 0);
    }
}

void Attack::checkSuffixes(std::string pass, int level = 0)
{
    const int max_level = 3;
    if(level != max_level)
    {
        std::string pass2 = pass;
        for(int i = 0; i < suffixes.length(); i++)
        {
            Hash h = CryptoUtils::generateSHA1(pass2);
            //TODO: here should be method areHashesEqual

            if(CryptoUtils::convertHashToHexRep(h) == this->key)
            {
                std::cout << "JACKPOT!" << std::endl;
                std::cout << this->key << " is " << "sha1(" << pass2 << ")"<< std::endl;
                //exit(0);
            }

            pass2 = pass;
            pass2 += suffixes[i];

            checkSuffixes(pass2, level+1);
        }
    }
}

void Attack::setChainsRange(int start, int end)
{
    chains_range_start = start;
    chains_range_end = end;
}
