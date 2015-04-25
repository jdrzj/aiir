#ifndef CRYPTO_CRYPTO_UTILS_H
#define CRYPTO_CRYPTO_UTILS_H

#include <iostream>
#include <string>
#include <memory>
#include <utility>

// Jak głosi dokumentacja OpenSSL:
// SHA_DIGEST_LENGTH == 20 bytes of output
// MD5_DIGEST_LENGTH == 16 bytes of output
constexpr int SHA_HASH_LENGTH = 20;
constexpr int MD5_HASH_LENGTH = 16;

enum class HashingFunction { SHA1, MD5 };


// Klasa obudowująca hasze zwracane przez OpenSSL.
// Na razie obudowuje tylko SHA1, ale w całkiem niedalekiej
// przeszłości powinna zostać rozszerzona.
class Hash
{
	HashingFunction function_;
	int size_;
	std::shared_ptr<unsigned char> hash_byte_rep_;

public:

	// Konstruktor
	Hash(HashingFunction function)
	: function_(function)
	{
		if (function_ == HashingFunction::SHA1)
			size_ = SHA_HASH_LENGTH;
		else if (function_ == HashingFunction::MD5)
			size_ = MD5_HASH_LENGTH;
		// ... i tak dalej

		hash_byte_rep_ = std::shared_ptr<unsigned char>(new unsigned char[size_]);
	}

	// Destruktor
	~Hash()
	{
	}

	// Konstruktor kopiujący
	Hash(const Hash& other)
		: function_(other.function_)
		, size_(other.size_)
		, hash_byte_rep_(other.hash_byte_rep_)
	{
	}

	// Operator przypisania
	Hash& operator=(Hash& other)
	{
		std::swap(function_, other.function_);
		std::swap(size_, other.size_);
		std::swap(hash_byte_rep_, other.hash_byte_rep_);
		return *this;
	}

	int getSize() const
	{
		return size_;
	}

	// Zwraca wskaźnik na hash w postaci tablicy bajtów, takiej, jaką otrzymaliśmy
	// z OpenSSL-owego SHA-1.
	unsigned char* getByteArray()
	{
		return hash_byte_rep_.get();
	}
};

class CryptoUtils
{
public:
	static Hash generateSHA1(const std::string& text);
	static Hash generateMD5(const std::string& text);
	static std::string convertHashToHexRep(Hash& hash);
	static Hash convertHexRepToHash(const std::string& hex_hash, HashingFunction function);
	static bool areHashesEqual(Hash& first, Hash& second);
	static void incrementString(std::string &s, int stringPosition);
};

#endif // CRYPTO_CRYPTO_UTILS_H
