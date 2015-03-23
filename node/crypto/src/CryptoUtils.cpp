#include "CryptoUtils.h"

#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// Na podstawie zmiennej std::string generuje i zwraca jej hash stworzony
// przy pomocy funkcji SHA1 dostępnej w bibliotece OpenSSL.
Hash CryptoUtils::generateSHA1(const std::string& text)
{
	const unsigned char* text_ptr = reinterpret_cast<const unsigned char*>(text.c_str());
	Hash hash(HashingFunction::SHA1);
	SHA1(text_ptr, text.size(), hash.getByteArray());
	return hash;
}

// Konwertuje hasha trzymanego w pamięci jako wektor 20
// bajtów na postać stringa zawierającego heksadecymalne wartości.
std::string CryptoUtils::convertHashToHexRep(Hash& hash)
{
	std::stringstream ss;
    ss << std::hex;
    for (int i(0); i < hash.getSize(); ++i)
        ss << static_cast<int>(hash.getByteArray()[i]);
    return ss.str();
}

// Porównuje reprezentacje dwóch hashy w postaci dwóch wektorów
// 20-bajtowych.
bool CryptoUtils::areHashesEqual(Hash& first, Hash& second)
{
	return std::equal(first.getByteVector().begin(), first.getByteVector().end(),
		second.getByteVector().begin());
}