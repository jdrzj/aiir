#include <iostream>
#include <fstream>
#include <math.h>
#include "boost/dynamic_bitset.hpp"
#include "CryptoUtils.h"
#include "Attack.h"
#include "RainbowCracker.h"
#include <cstring>

std::fstream& go_to_line(std::fstream& file, unsigned int num);


Attack::Attack(const AttackType& t, const std::string& k, HashingFunction
    f)
    : type(t)
    , key(k)
    , function(f) {}

Attack::~Attack() {}

std::string Attack::defeatKey()
{
    if(this->type == AttackType::dictionary)
    {
       return dictionaryAttack();
    }
    else if(this->type == AttackType::rainbow)
    {
        return rainbowAttack();
    } 
    else if(this->type == AttackType::bruteForce)
    {
        return bruteForceAttack();
    }
};

void Attack::setDictionaryFileName(const std::string& file_name)
{
    this->dictionary_file_name = file_name;
};

std::string Attack::dictionaryAttack()
{
    std::fstream dictionary;
    std::string pass = "";
    dictionary.open(dictionary_file_name, std::ios::in);
    std::string line;
    int subtask_size = chains_range_end - chains_range_start;
    int lines_visited = 0;
    if (dictionary.good() && dictionary.is_open())
    {
        go_to_line(dictionary, chains_range_start);
        std::cout << "SLAVE #" << world_rank + 1 << ": Starting at line: " << chains_range_start << std::endl;
        std::cout << "SLAVE #" << world_rank + 1 << ": Subtask size: " << subtask_size << std::endl;
        unsigned char * temp_uc;
        while (lines_visited < subtask_size && std::getline(dictionary, line))
        {
            ++lines_visited;
            pass = hackify(line);
            if (pass != "")
                break;
        }
        std::cout << "SLAVE #" << world_rank + 1 << ": Lines visited: " << lines_visited << std::endl;
        dictionary.close();
    }
    else
    {
        if (dictionary.is_open())
            dictionary.close();
        else
            std::cout << "SLAVE #" << world_rank + 1 << ": No such file: " << dictionary_file_name << std::endl;
    }
    return (pass != "") ? pass : ".";
};

std::string Attack::rainbowAttack()
{
    Hash desired_hash = CryptoUtils::convertHexRepToHash(key, function);
    RainbowCracker rainbow;
    rainbow.generateChains(chains_range_start, chains_range_end, letters_count, function);
    std::string pass = rainbow.run(desired_hash, function);
    return pass;
}


std::string Attack::bruteForceAttack()
{
    Hash desiredHash = CryptoUtils::convertHexRepToHash(key, function);
    std::string pass = " ";
    int stringPosition = 0;
    int subtask_size = chains_range_end - chains_range_start;
    // iterate to the subtask start
    for (int i=0; i<chains_range_start; i++)
    {
        CryptoUtils::incrementString(pass, stringPosition);
    }
    for (int i=0; i<subtask_size; i++)
    {
        Hash passHash;
        //std::cout << pass << std::endl;
        if (function == HashingFunction::SHA1)
        {
            Hash tmp = CryptoUtils::generateSHA1(pass);
            passHash = tmp;
        }
        else if (function == HashingFunction::MD5)
        {
            Hash tmp = CryptoUtils::generateMD5(pass);
            passHash = tmp;
        }
        if(CryptoUtils::convertHashToHexRep(passHash) == this->key) 
        {
            return pass;
        }
        else
        {
            CryptoUtils::incrementString(pass, stringPosition);
        }
    }
    return "";
}

std::string Attack::hackify(std::string pass)
{
    //liczba zapisana bitowo
    // 0 - downcase
    // 1 - uppercase

    std::string s;
    std::string pass2;
    std::string result = "";
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
        result = checkSuffixes(pass2, 0);
        if (result != "")
            break;
    }
    return result;
}

std::string Attack::checkSuffixes(std::string pass, int level)
{
    const int max_level = 3;
    std::string result = "";
    if(level != max_level)
    {
        std::string pass2 = pass;
        for(int i = 0; i < suffixes.length(); i++)
        {
            Hash h;
            if (function == HashingFunction::SHA1)
            {
                Hash tmp = CryptoUtils::generateSHA1(pass2);
                h = tmp;
            }
            else if (function == HashingFunction::MD5)
            {
                Hash tmp = CryptoUtils::generateMD5(pass2);
                h = tmp;
            }
            //TODO: here should be method areHashesEqual
            std::cout << pass2 << " = " << CryptoUtils::convertHashToHexRep(h) << std::endl;
            if(CryptoUtils::convertHashToHexRep(h) == this->key)
            {
                //std::cout << "JACKPOT!" << std::endl;
                //std::cout << this->key << " is " << "a hashed version of " << pass << std::endl;
                result = pass2;
                break;
            }

            pass2 = pass;
            pass2 += suffixes[i];

            result = checkSuffixes(pass2, level+1);
            if (result != "")
                break;
        }
    }
    return result;
}

void Attack::setChainsRange(int start, int end)
{
    chains_range_start = start;
    chains_range_end = end;
}

void Attack::setLettersCount(int count)
{
    letters_count = count;
}

void Attack::setWorldRank(int rank)
{
    world_rank = rank;
}

std::fstream& go_to_line(std::fstream& file, unsigned int num)
{
    file.seekg(std::ios::beg);
    for (int i=0; i < num - 1; ++i)
    {
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}
