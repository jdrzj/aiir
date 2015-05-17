#ifndef ATTACK_H
#define ATTACK_H

#include "CryptoUtils.h"

enum class AttackType { dictionary, rainbow, bruteForce};

class Attack {
    AttackType type;
    std::string key;
    HashingFunction function;
    std::string dictionary_file_name;
    int chains_range_start;
    int chains_range_end;
    int letters_count;
    int world_rank;
    const std::string suffixes = "1234567890!@#$^&*()_+{}.";

public:
    Attack(const AttackType& t, const std::string& k, HashingFunction
        function);
    ~Attack();
    void setDictionaryFileName(const std::string& file_name);
    std::string defeatKey();
    std::string checkSuffixes(std::string pass, int level);
    void setChainsRange(int start, int end);
    void setLettersCount(int count);
    void setWorldRank(int rank);
private:
    std::string dictionaryAttack();
    std::string rainbowAttack();
    std::string hackify(std::string pass);
    std::string bruteForceAttack();
};

#endif