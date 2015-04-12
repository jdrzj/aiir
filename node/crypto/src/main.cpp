#include <iostream>
#include <string.h>
#include "CryptoUtils.h"
#include "RainbowCracker.h"
#include "Attack.h"

int main(int argc, const char * argv[]) {
    Attack *dict = new Attack(AttackType::dictionary, "bf6184d77abc7cb4597ce65305ea7de17eba8627", HashingFunction::SHA1);
    dict->setDictionaryFileName("john.txt");
    dict->defeatKey();

    Attack *rainbow = new Attack(AttackType::rainbow, "761f22b2c1593d0bb87e0b606f990ba4974706de", HashingFunction::SHA1);
    rainbow->setChainsRange(0, 5);
    rainbow->defeatKey();

    return 0;
}
