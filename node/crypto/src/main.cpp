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
    rainbow->setLettersCount(2);
    rainbow->setChainsRange(0, 5);
    rainbow->defeatKey();

    Attack *rainbow_md5 = new Attack(AttackType::rainbow, "3416a75f4cea9109507cacd8e2f2aefc", HashingFunction::MD5);
    rainbow->setLettersCount(2);
    rainbow_md5->setChainsRange(0, 400);
    rainbow_md5->defeatKey();

    Attack *rainbow_md5_v2 = new Attack(AttackType::rainbow, "202cb962ac59075b964b07152d234b70", HashingFunction::MD5);
    rainbow_md5_v2->setLettersCount(3);
    rainbow_md5_v2->setChainsRange(0, 1000);
    rainbow_md5_v2->defeatKey();

    Attack *bruteForce = new Attack(AttackType::bruteForce, "761f22b2c1593d0bb87e0b606f990ba4974706de", HashingFunction::SHA1);
	bruteForce->defeatKey();

    Attack *bruteForce_md5 = new Attack(AttackType::bruteForce, "2fe04e524ba40505a82e03a2819429cc", HashingFunction::MD5);
    bruteForce_md5->defeatKey();
    return 0;
}
