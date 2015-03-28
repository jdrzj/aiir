#include <iostream>
#include <string.h>
#include "CryptoUtils.h"
#include "Attack.h"

int main(int argc, const char * argv[]) {

    //to tylko moje kody sprawdzające czy klasy działają ;)
    CryptoUtils util;
    Attack *dict = new Attack(AttackType::dictionary);
    dict->setDictionaryFileName("lemon.txt");
    std::string s = "lemon";
    Hash sha1 = util.generateSHA1(s);
    unsigned char* uc = sha1.getByteArray();
    std::cout << util.convertHashToHexRep(sha1) << std::endl;
    dict->defeatKey();
    return 0;
}
