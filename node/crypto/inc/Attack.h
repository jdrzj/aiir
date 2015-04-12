
enum class AttackType { dictionary /*rainbow brute force*/};

class Attack {
    AttackType type;
    std::string key;
    std::string dictionary_file_name;
    const std::string suffixes = "1234567890!@#$^&*()_+{}.";

public:
    Attack(const AttackType& t, const std::string& k);
    ~Attack();
    void setDictionaryFileName(const std::string& file_name);
    void defeatKey();
    void check_suffixes(std::string pass, int level);
private:
    void dictionaryAttack(std::string& file_name);
    void hackify(std::string pass);
};
