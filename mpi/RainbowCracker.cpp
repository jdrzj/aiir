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
	if (extracted_digits < password_length)
	{
		for (int i = 0; i < password_length - extracted_digits; ++i)
			new_key += "0";
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

void RainbowCracker::generateChains(int start, int end, int pass_length,
	HashingFunction function)
{
	for (int i = start; i <= end; ++i)
	{
		chains.push_back(generateVerySimpleChain(std::to_string(i), 100, pass_length,
			function));
	}
}

Chain RainbowCracker::generateVerySimpleChain(const std::string initial_text, int iterations,
	int pass_length, HashingFunction function)
{
	std::string current_text = std::string(initial_text);
	for (int i(0); i < iterations; ++i)
	{
		Hash hash;
		if (function == HashingFunction::SHA1)
		{
			Hash tmp = CryptoUtils::generateSHA1(current_text);
			hash = tmp;
		}
		else if (function == HashingFunction::MD5)
		{
			Hash tmp = CryptoUtils::generateMD5(current_text);
			hash = tmp;
		}
		current_text = reduce(hash, 0, 0, pass_length);
	}
	return Chain(function, "0123456789", initial_text,
		0, pass_length, 0, iterations);
}

std::string RainbowCracker::checkChain(const Chain& chain, Hash& desired_hash, HashingFunction function)
{
	std::string text = std::string(chain.initial_password_);
	for (int i(0); i < chain.iterations_; ++i)
	{
		Hash hash;
		if (function == HashingFunction::SHA1)
		{
			Hash tmp = CryptoUtils::generateSHA1(text);
			hash = tmp;
		}
		else if (function == HashingFunction::MD5)
		{
			Hash tmp = CryptoUtils::generateMD5(text);
			hash = tmp;
		}
		//std::cout << CryptoUtils::convertHashToHexRep(hash) << std::endl;
		if (CryptoUtils::areHashesEqual(hash, desired_hash))
			return text;
		text = reduce(hash, 0, 0, chain.password_length_);
	}
	return "";
}