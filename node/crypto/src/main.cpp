#include <iostream>
#include <string.h>
#include "CryptoUtils.h"
#include "Attack.h"

int main(int argc, const char * argv[]) {

    CryptoUtils util;
    Attack *dict = new Attack(AttackType::dictionary, "bf6184d77abc7cb4597ce65305ea7de17eba8627");
    dict->setDictionaryFileName("john.txt");
    dict->defeatKey();
    return 0;
}
