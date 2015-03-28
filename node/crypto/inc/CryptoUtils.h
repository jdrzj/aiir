#include <iostream>
#include <string>
#include <memory>
#include <utility>

// Jak głosi dokumentacja OpenSSL:
// SHA_DIGEST_LENGTH == 20 bytes of output
constexpr int SHA_HASH_LENGTH = 20;

enum class HashingFunction { SHA1 /*MD5, itd.*/ };


// Klasa obudowująca hasze zwracane przez OpenSSL.
// Na razie obudowuje tylko SHA1, ale w całkiem niedalekiej
// przeszłości powinna zostać rozszerzona.
class Hash
{
	HashingFunction function;
	int size;
	std::shared_ptr<unsigned char> hash_byte_rep;

public:

	// Konstruktor
	Hash(HashingFunction function)
	{
		this->function = function;
		if (this->function == HashingFunction::SHA1)
			size = SHA_HASH_LENGTH;
		// ... i tak dalej

		hash_byte_rep = std::shared_ptr<unsigned char>(new unsigned char[20]);
	}

	// Destruktor
	~Hash()
	{
	}

	// Konstruktor kopiujący
	Hash(const Hash& other)
    	: function(other.function)
    	, size(other.size)
		, hash_byte_rep(other.hash_byte_rep)
	{
	}

	// Operator przypisania
	Hash& operator=(Hash& other)
	{
		std::swap(function, other.function);
		std::swap(size, other.size);
		std::swap(hash_byte_rep, other.hash_byte_rep);
		return *this;
	}

	int getSize() const
	{
		return size;
	}

	// Zwraca wskaźnik na hash w postaci tablicy bajtów, takiej, jaką otrzymaliśmy
	// z OpenSSL-owego SHA-1. Dla niepustego wektora (obudowującego tablicę), zwraca &front().
	// Uwaga: Wykorzystywane do zapisu przez funkcję generate SHA-1, więc musi być prealokowane
	// tak, by móc pomieścić odpowiedni rozmiar hasha (patrz wywołanie metody reserve w konstruktorze).
	unsigned char* getByteArray()
	{
		return hash_byte_rep.get();
	}
};

class CryptoUtils
{
public:
	static Hash generateSHA1(const std::string& text);
	static std::string convertHashToHexRep(Hash& hash);
	static bool areHashesEqual(Hash& first, Hash& second);
};
