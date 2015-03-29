
enum class AttackType { dictionary /*rainbow brute force*/};

class Attack {
    AttackType type;
    std::string key;
    std::string dictionary_file_name;

public:
    Attack(const AttackType& t, const std::string& k);
    ~Attack();
    void setDictionaryFileName(const std::string& file_name);
    void defeatKey();
private:
    void dictionaryAttack(std::string& file_name);
};
