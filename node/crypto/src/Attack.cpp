#include <iostream>
#include <fstream>
#include "CryptoUtils.h"
#include "Attack.h"

Attack::Attack(const AttackType& t):type(t){};
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
};
