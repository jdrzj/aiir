
enum class AttackType { dictionary, rainbow, /*brute force*/};

class Attack {
    AttackType type;
    std::string key;
    HashingFunction function;
    std::string dictionary_file_name;
    int chains_range_start;
    int chains_range_end;
    const std::string suffixes = "1234567890!@#$^&*()_+{}.";

public:
    Attack(const AttackType& t, const std::string& k, HashingFunction
        function);
    ~Attack();
    void setDictionaryFileName(const std::string& file_name);
    void defeatKey();
    void check_suffixes(std::string pass, int level);
    void setChainsRange(int start, int end);
private:
    void dictionaryAttack(std::string& file_name);
    void rainbowAttack();
    void hackify(std::string pass);
};
