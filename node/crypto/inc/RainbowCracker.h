#ifndef CRYPTO_RAINBOW_CRACKER
#define CRYPTO_RAINBOW_CRACKER

#include "CryptoUtils.h"
#include <string>

// Klasa reprezentuja łańcuch, który ma zostać poddany
// kolejnym wywołaniom funkcji hashujących i redukujących.
// hashing_function : enum, funkcja hashująca, np. SHA-1
// character_set	: zawiera wszystkie znaki dozwolone w haśle
// initial_password : początkowe hasło, które ma posłużyć jako baza
//					  przy dalszym generowaniu hashy.
// chain_length		: długość łańcucha, liczba iteracji do wykonania
// password_length  : rozważana w danym łańcuchu długość hasła
// table_index		: indeks tabeli, używany w funkcji redukującej
// hash_wanted      : poszukiwany hash
class Chain
{

	Chain(HashingFunction hashing_function, std::string character_set,
		std::string initial_password, int chain_length, int password_length,
		int table_index, Hash hash_wanted)
		: hashing_function_(hashing_function) 
		, character_set_(character_set)
		, initial_password_(initial_password)
		, chain_length_(chain_length)
		, password_length_(password_length)
		, table_index_(table_index)
		, hash_wanted_(hash_wanted) { };

	HashingFunction hashing_function_;
	std::string character_set_;
	std::string initial_password_;
	int chain_length_;
	int password_length_;
	int table_index_;
	Hash hash_wanted_;
};

class RainbowCracker
{
public:
	// Sprawdza, czy w danym łańcuchu znalazł się szukany hash
	std::string check_chain(const Chain& chain);

private:
	// Zwraca hash po poddaniu go funkcji redukującej
	std::string reduce(const Hash& hash, int position, int table_index,
		int password_length);
};

#endif // CRYPTO_RAINBOW_CRACKER