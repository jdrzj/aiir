#ifndef CRYPTO_RAINBOW_CRACKER
#define CRYPTO_RAINBOW_CRACKER

#include "CryptoUtils.h"
#include <string>
#include <vector>

// Klasa reprezentuja łańcuch, który ma zostać poddany
// kolejnym wywołaniom funkcji hashujących i redukujących.
// hashing_function : enum, funkcja hashująca, np. SHA-1
// character_set	: zawiera wszystkie znaki dozwolone w haśle
// initial_password : początkowe hasło, które ma posłużyć jako baza
//					  przy dalszym generowaniu hashy.
// chain_length		: długość łańcucha, liczba iteracji do wykonania
// password_length  : rozważana w danym łańcuchu długość hasła
// table_index		: indeks tabeli, używany w funkcji redukującej
// iterations 		: liczba iteracji do wykonania
class Chain
{

public:
	Chain(HashingFunction hashing_function, std::string character_set,
		std::string initial_password, int chain_length, int password_length,
		int table_index, int iterations)
		: hashing_function_(hashing_function) 
		, character_set_(character_set)
		, initial_password_(initial_password)
		, chain_length_(chain_length)
		, password_length_(password_length)
		, table_index_(table_index)
		, iterations_(iterations) { };

	HashingFunction hashing_function_;
	std::string character_set_;
	std::string initial_password_;
	int chain_length_;
	int password_length_;
	int table_index_;
	int iterations_;
};

class RainbowCracker
{
public:
	// Sprawdza, czy w danym łańcuchu znalazł się szukany hash
	std::string checkChain(const Chain& chain, Hash& desired_hash,
		HashingFunction function);
	void generateChains(int pass_length, HashingFunction
		function);
	std::string run(Hash& desired_hash, HashingFunction
		function);

private:
	std::vector<Chain> chains;

	// Zwraca hash po poddaniu go funkcji redukującej
	std::string reduce(Hash& hash, int position, int table_index,
		int password_length);
	Chain generateVerySimpleChain(const std::string first_text, int iterations,
		int pass_length, HashingFunction function);
};

#endif // CRYPTO_RAINBOW_CRACKER