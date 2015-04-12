#include "CryptoUtils.h"

#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

// Na podstawie zmiennej std::string generuje i zwraca jej hash stworzony
// przy pomocy funkcji SHA1 dostępnej w bibliotece OpenSSL.
Hash CryptoUtils::generateSHA1(const std::string& text)
{
	const unsigned char* text_ptr = reinterpret_cast<const unsigned char*>(text.c_str());
	Hash hash(HashingFunction::SHA1);
	SHA1(text_ptr, text.size(), hash.getByteArray());
	return hash;
}

// Konwertuje hasha trzymanego w pamięci jako tablica
// unsigned charów na postać stringa zawierającego heksadecymalne wartości.
std::string CryptoUtils::convertHashToHexRep(Hash& hash)
{
	std::stringstream ss;
    ss << std::hex;
    for (int i(0); i < hash.getSize(); ++i)
        ss << static_cast<int>(hash.getByteArray()[i]);
    return ss.str();
}

// Konwertuje string zawierającego hash w postaci heksadecymalnej na jego postać
// unsigned char-ową
Hash CryptoUtils::convertHexRepToHash(const std::string& hashed_hex, HashingFunction function)
{
	std::string hex_hash = std::string(hashed_hex);
	int size = hex_hash.size();
	for(int i = 2; i < size; i += 2)
    {
		hex_hash.insert(i, " ");
		i += 1;
		size = hex_hash.size();
	}
	Hash hash(function);
	std::istringstream hex_hash_stream(hex_hash);
	unsigned int tmp;
	int i = 0;
	while (hex_hash_stream >> std::hex  >> tmp)
	{
		hash.getByteArray()[i] = tmp;
		++i;
	}
	return hash;
}


// Porównuje reprezentacje dwóch hashy w postaci dwóch wektorów
// 20-bajtowych.
bool CryptoUtils::areHashesEqual(Hash& first, Hash& second)
{
	return std::equal(first.getByteArray(), first.getByteArray() + first.getSize(),
		second.getByteArray());
}
