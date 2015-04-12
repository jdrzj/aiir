#include "RainbowCracker.h"
#include "CryptoUtils.h"
#include <vector>

std::string RainbowCracker::reduce(Hash& hash, int position, int table_index, int password_length)
{
	int extracted_digits = 0;
	std::string new_key;
	std::string hex_rep = CryptoUtils::convertHashToHexRep(hash);
	for (auto it = hex_rep.begin(); it != hex_rep.end(); ++it)
	{
		unsigned char tmp = *it;
		if (tmp >= '0' && tmp <= '9')
		{
			//std::cout << tmp << std::endl;
			new_key += tmp;
			if (++extracted_digits == password_length)
				break;
		}
	}
	return new_key;
}

std::string RainbowCracker::run(Hash& desired_hash, HashingFunction function)
{
	std::string pass = "";
	for (auto it = chains.begin(); it != chains.end(); ++it)
	{
		pass = checkChain(*it, desired_hash, function);
		if (pass != "")
			return pass;
	}
	return pass;
}

void RainbowCracker::generateChains(int pass_length, HashingFunction function)
{
	chains.push_back(generateVerySimpleChain("00", 100, 2, HashingFunction::SHA1));
	chains.push_back(generateVerySimpleChain("01", 100, 2, HashingFunction::SHA1));
	chains.push_back(generateVerySimpleChain("02", 100, 2, HashingFunction::SHA1));
	chains.push_back(generateVerySimpleChain("03", 100, 2, HashingFunction::SHA1));
	chains.push_back(generateVerySimpleChain("04", 100, 2, HashingFunction::SHA1));
	chains.push_back(generateVerySimpleChain("04", 100, 2, HashingFunction::SHA1));
}

Chain RainbowCracker::generateVerySimpleChain(const std::string initial_text, int iterations,
	int pass_length, HashingFunction function)
{
	std::string current_text = std::string(initial_text);
	// Trzeba poprawić, żeby nie trzeba było robić takich brzydkich ifów :)
	if (function == HashingFunction::SHA1)
	{
		for (int i(0); i < iterations; ++i)
		{
			Hash hash = CryptoUtils::generateSHA1(current_text);
			std::string x = CryptoUtils::convertHashToHexRep(hash);
			current_text = reduce(hash, 0, 0, pass_length);
		}
	}
	return Chain(HashingFunction::SHA1, "0123456789", initial_text,
		0, pass_length, 0, iterations);
}

std::string RainbowCracker::checkChain(const Chain& chain, Hash& desired_hash, HashingFunction function)
{
	std::string text = std::string(chain.initial_password_);
	// Trzeba poprawić, żeby nie trzeba było robić takich brzydkich ifów :)
	if (function == HashingFunction::SHA1)
	{
		for (int i(0); i < chain.iterations_; ++i)
		{
			Hash hash = CryptoUtils::generateSHA1(text);
			std::string x = CryptoUtils::convertHashToHexRep(hash);
			if (CryptoUtils::areHashesEqual(hash, desired_hash))
				return text;
			text = reduce(hash, 0, 0, chain.password_length_);
		}
	}
	return "";
}