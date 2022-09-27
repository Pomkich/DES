#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include "constants.h"

using namespace std;

char bin_hex(std::string bits) {
	static std::map<std::string, char> hex_bits = {	// for better perfomance static
		{"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
		{"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
		{"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
		{"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'},
	};
	return hex_bits[bits];
}

std::string byte_bin(unsigned char ch) {
	std::string bits = "";
	for (int i = 0; i < 8; i++) {
		char bit = ((bool)(ch << i & 0b10000000) + '0');
		bits.push_back(bit);
	}
	return bits;
}

unsigned char* permutate_P(unsigned char block[8]) {
	unsigned char perm_block[8];
	memset(perm_block, 0, 8);
	for (int i = 0; i < 64; i++) {
		int offset_byte = (P[i / 16][i % 16] - 1) / 8;
		int offset_bit = (P[i / 16][i % 16] - 1) % 8;
		int bit = ((block[offset_byte] << offset_bit) & 0b10000000);
		perm_block[i / 8] |= ((0b10000000 & bit) >> (i % 8));
	}
	return perm_block;
}

int main() {
	unsigned char block[8] = {
		0b00000000, 0b00000000, 0b00000000, 0b00000000,
		0b11111111, 0b11111111, 0b11111111, 0b11111111
	};

	auto new_block = permutate_P(block);

	return 0;
}

/*
// добавление битов четности в исходный ключ
void add_key_bits(std::string& key) {
	for (int i = 0; i < 8; i++) {
		auto substr = key.substr(i * 8, 7);
		if (std::count(substr.begin(), substr.end(), '1') % 2 != 1)
			key.insert(key.begin() + (i * 8) + 7, '1');
		else
			key.insert(key.begin() + (i * 8) + 7, '0');
	}
}

// первая перестановка исходного блока текста
std::string permutate_P(std::string bits) {
	std::string perm_bits(block_size, '0');
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 16; j++) {
			perm_bits[i * 16 + j] = bits[P[i][j]];
		}
	}
	return perm_bits;
}

std::string permutate_key(std::string key) {
	std::string perm_key(start_key_size, '0');
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 14; j++) {
			perm_key[i * 14 + j] = key[key_perm_table[i][j]];
		}
	}
	return perm_key;
}


*/

/*	// read file
std::ifstream infile("test.txt");

	//get length of file
	infile.seekg(0, std::ios::end);
	size_t length = infile.tellg();
	infile.seekg(0, std::ios::beg);

	char* buffer = new char[length];

	//read file
	infile.read(buffer, length);

	std::cout << length << std::endl;

	for (int i = 0; i < length; i++) {
		if (i == 16) std::cout << std::endl;
		auto bits = byte_bin(buffer[i]);
		std::cout << bin_hex(bits.substr(0, 4)) << bin_hex(bits.substr(4, 4)) << " ";
	}

*/