#include <iostream>
#include <string.h>
#include "CryptoUtils.h"
#include "Attack.h"

int main(int argc, const char * argv[]) {

    //to tylko moje kody sprawdzające czy klasy działają ;)
    CryptoUtils util;
    Attack *dict = new Attack(AttackType::dictionary, "dfdd7bce2ad9f89d7204dd83161d66d1e521759c");
    dict->setDictionaryFileName("lemon.txt");
    std::string s = "lemon";
    Hash sha1 = util.generateSHA1(s);
    unsigned char* uc = sha1.getByteArray();
    // /Users/jedrzejurbanski/Documents/pwr/6/aiir/node/crypto/john.txt
    dict->setDictionaryFileName("john.txt");
    dict->defeatKey();
    return 0;
}
